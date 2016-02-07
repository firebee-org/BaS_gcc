/*
 * BaS
 *
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 - 2012 F. Aschwanden
 * Copyright 2011 - 2012 V. Riviere
 * Copyright 2012        M. Froeschle
 */

#include <bas_types.h>

#include "MCF5475.h"
#include "startcf.h"
#include "sysinit.h"
#include "util.h"
#include "cache.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "bas_types.h"
#include "sd_card.h"
#include "wait.h"

#include "diskio.h"
#include "ff.h"
#include "s19reader.h"
#include "mmu.h"
#include "dma.h"
#include "net.h"
#include "eth.h"
#include "nbuf.h"
#include "nif.h"
#include "fec.h"
#include "bootp.h"
#include "interrupts.h"
#include "exceptions.h"
#include "net_timer.h"
#include "pci.h"
#include "video.h"

//#define BAS_DEBUG
#if defined(BAS_DEBUG)
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif
#define err(format, arg...) do { xprintf("ERROR: %s(): " format, __FUNCTION__, ##arg); } while (0)

/* imported routines */
extern int vec_init();

/* Symbols from the linker script */
extern uint8_t _STRAM_END[];
#define STRAM_END ((uint32_t)_STRAM_END)
extern uint8_t _TOS[];
#define TOS ((uint32_t)_TOS) /* final TOS location */
extern uint8_t _FASTRAM_END[];
#define FASTRAM_END ((uint32_t)_FASTRAM_END)
extern uint8_t _EMUTOS[];
#define EMUTOS ((uint32_t)_EMUTOS) /* where EmuTOS is stored in flash */
extern uint8_t _EMUTOS_SIZE[];
#define EMUTOS_SIZE ((uint32_t)_EMUTOS_SIZE) /* size of EmuTOS, in bytes */

/*
 * check if it is possible to transfer data to PIC
 */
static inline bool pic_txready(void)
{
    if (MCF_PSC3_PSCSR & MCF_PSC_PSCSR_TXRDY)
    {
        return true;
    }

    return false;
}

/*
 * check if it is possible to receive data from PIC
 */
static inline bool pic_rxready(void)
{
    if (MCF_PSC3_PSCSR & MCF_PSC_PSCSR_RXRDY)
    {
        return true;
    }

    return false;
}

void write_pic_byte(uint8_t value)
{
    /*
     * Wait until the transmitter is ready or 1000us are passed
     */
    waitfor(1000, pic_txready);

    /*
     * Transmit the byte
     */
    *(volatile uint8_t*)(&MCF_PSC3_PSCTB_8BIT) = value; // Really 8-bit
}

uint8_t read_pic_byte(void)
{
    /*
     * Wait until a byte has been received or 1000us are passed
     */
    waitfor(1000, pic_rxready);

    /*
     * Return the received byte
     */
    return * (volatile uint8_t *) (&MCF_PSC3_PSCTB_8BIT); // Really 8-bit
}

void pic_init(void)
{
    char answer[4] = "OLD";

    xprintf("initialize the PIC: ");

    /*
     * Send the PIC initialization string
     */
    write_pic_byte('A');
    write_pic_byte('C');
    write_pic_byte('P');
    write_pic_byte('F');

    /*
     * Read the 3-char answer string. Should be "OK!".
     */
    answer[0] = read_pic_byte();
    answer[1] = read_pic_byte();
    answer[2] = read_pic_byte();
    answer[3] = '\0';

    if (answer[0] != 'O' || answer[1] != 'K' || answer[2] != '!')
    {
        dbg("PIC initialization failed. Already initialized?\r\n");
    }
    else
    {
        xprintf("%s\r\n", answer);
    }
}

void nvram_init(void)
{
    int i;

    xprintf("Restore the NVRAM data: ");

    /* Request for NVRAM backup data */
    write_pic_byte(0x01);

    /* Check answer type */
    if (read_pic_byte() != 0x81)
    {
        // FIXME: PIC protocol error
        xprintf("FAILED\r\n");
        return;
    }

    /* Restore the NVRAM backup to the FPGA */
    for (i = 0; i < 64; i++)
    {
        uint8_t data = read_pic_byte();
        * (volatile uint8_t*) 0xffff8961 = i;
        * (volatile uint8_t*) 0xffff8963 = data;
    }

    xprintf("finished\r\n");
}

#define KBD_ACIA_CONTROL        * ((uint8_t *) 0xfffffc00)
#define MIDI_ACIA_CONTROL       * ((uint8_t *) 0xfffffc04)
#define MFP_INTR_IN_SERVICE_A   * ((uint8_t *) 0xfffffa0f)
#define MFP_INTR_IN_SERVICE_B   * ((uint8_t *) 0xfffffa11)

void acia_init()
{
    xprintf("init ACIA: ");
    /* init ACIA */
    KBD_ACIA_CONTROL = 3;                   /* master reset */
    NOP();

    MIDI_ACIA_CONTROL = 3;                  /* master reset */
    NOP();

    KBD_ACIA_CONTROL = 0x96;                /* clock div = 64, 8N1, RTS low, TX int disable, RX int enable */
    NOP();

    MFP_INTR_IN_SERVICE_A = 0xff;
    NOP();

    MFP_INTR_IN_SERVICE_B = 0xff;
    NOP();

    xprintf("finished\r\n");
}

void enable_coldfire_interrupts()
{
    xprintf("enable interrupts: ");
#if defined(MACHINE_FIREBEE)
    FBEE_INTR_CONTROL = 0L;                 /* disable all interrupts */
#endif /* MACHINE_FIREBEE */

    MCF_EPORT_EPPAR = 0xaaa8;               /* all interrupts on falling edge */

#if defined(MACHINE_FIREBEE)
    /*
     * TIN0 on the Coldfire is connected to the FPGA. TIN0 triggers every write
     * access to 0xff8201 (vbasehi), i.e. everytime the video base address is written
     */
    MCF_GPT0_GMS = MCF_GPT_GMS_ICT(1) |     /* timer 0 on, video change capture on rising edge */
            MCF_GPT_GMS_IEN |
            MCF_GPT_GMS_TMS(1);             /* route GPT0 interrupt on interrupt controller */
    MCF_INTC_ICR62 = MCF_INTC_ICR_IL(7) |
                     MCF_INTC_ICR_IP(6);    /* interrupt level 7, interrupt priority 7 */
    MCF_INTC_IMRH = 0xbffffffe;             /* psc3 and timer 0 int on */
#endif

    xprintf("finished\r\n");
}

void enable_pci_interrupts()
{
    dbg("enable PCI interrupts\r\n");
    MCF_EPORT_EPIER = 0xfe;                 /* int 1-7 on */
    MCF_EPORT_EPFR = 0xff;                  /* clear all pending interrupts */
    MCF_INTC_IMRL = 0xffffff00;             /* int 1-7 on */
    MCF_INTC_IMRH = 0;
#if defined(MACHINE_FIREBEE)
    FBEE_INTR_ENABLE = FBEE_INTR_INT_IRQ7 | /* enable pseudo bus error */
            FBEE_INTR_INT_MFP_IRQ6 |        /* enable MFP interrupts */
            FBEE_INTR_INT_FPGA_IRQ5 |       /* enable Firebee (PIC, PCI, ETH PHY, DVI, DSP) interrupts */
            FBEE_INTR_INT_VSYNC_IRQ4 |      /* enable vsync interrupts */
            FBEE_INTR_PCI_INTA |            /* enable PCI interrupts */
            FBEE_INTR_PCI_INTB |
            FBEE_INTR_PCI_INTC |
            FBEE_INTR_PCI_INTD;
            ;
#elif defined(MACHINE_M5484LITE)
    /*
     * MCF 5484 interrupts are configured at the CPLD for the FireEngine
     */

    /* TODO: enable PCI interrupts on the LITEKIT */
#elif defined(MACHINE_M54455)
    /* MCF 54455 interrupts are configured at the FPGA */

    /* TODO: enable PCI interrupts on the MCF54455 */
#else
#error unknown machine!
#endif
}

void disable_coldfire_interrupts()
{
#if defined(MACHINE_FIREBEE)
    FBEE_INTR_ENABLE = 0;                   /* disable all interrupts */
#endif /* MACHINE_FIREBEE */

    MCF_EPORT_EPIER = 0x0;
    MCF_INTC_IMRL = 0xfffffffe;
    MCF_INTC_IMRH = 0xffffffff;
}



NIF nif1;
#if defined(MACHINE_M5484LITE)
    /*
     * on the MCF 5484 LITEKIT, the second FEC interface is usable
     */
NIF nif2;
#endif

bool spurious_interrupt_handler(void *arg1, void *arg2)
{
    dbg("IMRH=%lx, IMRL=%lx\r\n", MCF_INTC_IMRH, MCF_INTC_IMRL);
    dbg("IPRH=%lx, IPRL=%lx\r\n", MCF_INTC_IPRH, MCF_INTC_IPRL);
    dbg("IRLR=%x\r\n", MCF_INTC_IRLR);

    return true;
}

/*
 * initialize the interrupt handler tables to dispatch interrupt requests from Coldfire devices
 */
void init_isr(void)
{
    isr_init();                             /* need to call that explicitely, otherwise isr table might be full */

    /*
     * register spurious interrupt handler
     */
    if (!isr_register_handler(24, 6, 6, spurious_interrupt_handler, NULL, NULL))
    {
        dbg("unable to register spurious interrupt handler\r\n");
    }

    /*
     * register the FEC interrupt handler
     */
    if (!isr_register_handler(64 + INT_SOURCE_FEC0, 5, 1, fec0_interrupt_handler, NULL, (void *) &nif1))
    {
        dbg("unable to register isr for FEC0\r\n");
    }

    /*
     * Register the DMA interrupt handler
     */

    if (!isr_register_handler(64 + INT_SOURCE_DMA, 5, 3, dma_interrupt_handler, NULL, NULL))
    {
        dbg("unable to register isr for DMA\r\n");
    }

#if defined(MACHINE_FIREBEE)
    /*
     * register GPT0 timer interrupt vector
     */
    if (!isr_register_handler(64 + INT_SOURCE_GPT0, 5, 2, gpt0_interrupt_handler, NULL, NULL))
    {
        dbg("unable to register isr for GPT0 timer\r\n");
    }

    /*
     * register the PIC interrupt handler
     */
    if (!isr_register_handler(64 + INT_SOURCE_PSC3, 5, 5, pic_interrupt_handler, NULL, NULL))
    {
        dbg("Error: unable to register ISR for PSC3\r\n");
    }
#endif /* MACHINE_FIREBEE */

    /*
     * register the XLB PCI interrupt handler
     */
    if (!isr_register_handler(64 + INT_SOURCE_XLBPCI, 7, 0, xlbpci_interrupt_handler, NULL, NULL))
    {
        dbg("Error: unable to register isr for XLB PCI interrupts\r\n");
    }

    MCF_XLB_XARB_IMR = MCF_XLB_XARB_IMR_SEAE |          /* slave error acknowledge interrupt */
                       MCF_XLB_XARB_IMR_MME |           /* multiple master at prio 0 interrupt */
                       MCF_XLB_XARB_IMR_TTAE |          /* TT address only interrupt */
                       MCF_XLB_XARB_IMR_TTRE |          /* TT reserved interrupt enable */
                       MCF_XLB_XARB_IMR_ECWE |          /* external control word interrupt */
                       MCF_XLB_XARB_IMR_TTME |          /* TBST/TSIZ mismatch interrupt */
                       MCF_XLB_XARB_IMR_BAE;            /* bus activity tenure timeout interrupt */

    if (!isr_register_handler(64 + INT_SOURCE_PCIARB, 5, 0, pciarb_interrupt_handler, NULL, NULL))
    {
        dbg("Error: unable to register isr for PCIARB interrupts\r\n");

        return;
    }
    MCF_PCIARB_PACR = MCF_PCIARB_PACR_EXTMINTEN(0x1f) | /* external master broken interrupt */
                      MCF_PCIARB_PACR_INTMINTEN;        /* internal master broken interrupt */
}

void BaS(void)
{
    uint8_t *src;
    uint8_t *dst = (uint8_t *) TOS;

#if defined(MACHINE_FIREBEE)        /* LITE board has no pic and (currently) no nvram */
    pic_init();
    nvram_init();
#endif /* MACHINE_FIREBEE */

    xprintf("initialize MMU: ");
    mmu_init();
    xprintf("finished\r\n");

    xprintf("initialize Coldfire DMA: ");
    dma_init();
    xprintf("finished\r\n");

    xprintf("copy EmuTOS: ");
    /* copy EMUTOS */
    src = (uint8_t *) EMUTOS;
    dma_memcpy(dst, src, EMUTOS_SIZE);
    xprintf("finished\r\n");

    xprintf("initialize exception vector table: ");
    vec_init();
    xprintf("finished\r\n");

    xprintf("flush caches: ");
    flush_and_invalidate_caches();
    xprintf("finished\r\n");
    xprintf("enable MMU: ");
    MCF_MMU_MMUCR = MCF_MMU_MMUCR_EN;           /* MMU on */
    NOP();                                      /* force pipeline sync */
    xprintf("finished\r\n");

#if defined(MACHINE_FIREBEE)
    xprintf("IDE reset: ");
    /* IDE reset */
    * (volatile uint8_t *) (0xffff8802 - 2) = 14;
    * (volatile uint8_t *) (0xffff8802 - 0) = 0x80;
    wait(1);

    * (volatile uint8_t *) (0xffff8802 - 0) = 0;

    xprintf("finished\r\n");
    xprintf("enable video: ");

    /*
     * video setup (25MHz)
     */
    * (volatile uint32_t *) 0xf0000410 = 0x032002ba;  /* horizontal 640x480 */
    * (volatile uint32_t *) 0xf0000414 = 0x020c020a;  /* vertical 640x480 */
    * (volatile uint32_t *) 0xf0000418 = 0x0190015d;  /* horizontal 320x240 */
    * (volatile uint32_t *) 0xf000041c = 0x020c020a; /* vertical 320x230 */

    /* fifo on, refresh on, ddrcs and cke on, video dac on */
    * (volatile uint32_t *) 0xf0000400 = 0x01070082;

    xprintf("finished\r\n");
#endif /* MACHINE_FIREBEE */

    sd_card_init();

    /*
     * memory setup
     */
    memset((void *) 0x200, 0, 0x400);

#if defined(MACHINE_FIREBEE)
    /* set Falcon bus control register */
    /* sets bit 3 and 6. Both are undefined on an original Falcon? */

    * (volatile uint8_t *) 0xffff8007 = 0x48;
#endif /* MACHINE_FIREBEE */

    /* ST RAM */

    * (uint32_t *) 0x42e = STRAM_END;   /* phystop TOS system variable */
    * (uint32_t *) 0x420 = 0x752019f3;  /* memvalid TOS system variable */
    * (uint32_t *) 0x43a = 0x237698aa;  /* memval2 TOS system variable */
    * (uint32_t *) 0x51a = 0x5555aaaa;  /* memval3 TOS system variable */

    /* TT-RAM */

    * (uint32_t *) 0x5a4 = FASTRAM_END; /* ramtop TOS system variable */
    * (uint32_t *) 0x5a8 = 0x1357bd13;  /* ramvalid TOS system variable */

#if defined(MACHINE_FIREBEE) /* m5484lite has no ACIA and no dip switch... */
    acia_init();
#endif /* MACHINE_FIREBEE */

    srec_execute("BASFLASH.S19");

    /* Jump into the OS */
    typedef void void_func(void);
    struct rom_header
    {
        void *initial_sp;
        void_func *initial_pc;
    };

    xprintf("BaS initialization finished, enable interrupts\r\n");
    init_isr();

    enable_coldfire_interrupts();
    MCF_INTC_IMRH = 0;
    MCF_INTC_IMRL = 0;

    dma_irq_enable();
    fec_irq_enable(0, 5, 1);

    enable_pci_interrupts();
    init_pci();
    video_init();

    /* initialize USB devices */
    init_usb();

    set_ipl(7);     /* disable interrupts */

    /*
     * start FireTOS if DIP switch is set accordingly
     */
    if (!(DIP_SWITCH & (1 << 6)))
    {
        extern uint8_t _FIRETOS[];
#define FIRETOS ((uint32_t)_FIRETOS) /* where FireTOS is stored in flash */

        /* make sure MMU is disabled */
        MCF_MMU_MMUCR = 0;  /* MMU off */
        NOP();              /* force pipeline sync */

        /* ST RAM */

        * (uint32_t *) 0x42e = STRAM_END;   /* phystop TOS system variable */
        * (uint32_t *) 0x420 = 0x752019f3;  /* memvalid TOS system variable */
        * (uint32_t *) 0x43a = 0x237698aa;  /* memval2 TOS system variable */
        * (uint32_t *) 0x51a = 0x5555aaaa;  /* memval3 TOS system variable */

        /* TT-RAM */

        * (uint32_t *) 0x5a4 = FASTRAM_END; /* ramtop TOS system variable */
        * (uint32_t *) 0x5a8 = 0x1357bd13;  /* ramvalid TOS system variable */

        xprintf("call FireTOS\r\n");
        /* Jump into FireTOS */
        typedef void void_func(void);
        void_func* FireTOS = (void_func*) FIRETOS;
        FireTOS();  // Should never return
    }

    xprintf("call EmuTOS\r\n");
    struct rom_header *os_header = (struct rom_header *) TOS;
    os_header->initial_pc();
}
