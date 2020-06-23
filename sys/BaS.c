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
#include "bas_utils.h"
#include "sd_card.h"
#include "wait.h"

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
#include "usb.h"
#include "video.h"
#include "driver_mem.h"

// #define DEBUG
#include "debug.h"

/* imported routines */
extern int vec_init();
extern void set_ide_access_mode(void);

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

bool fpga_configured = false;           /* for FPGA JTAG configuration */

extern volatile long _VRAM; /* start address of video ram from linker script */


#if defined(MACHINE_FIREBEE)

static bool i2c_transfer_finished(void)
{
    if (MCF_I2C_I2SR & MCF_I2C_I2SR_IIF)
        return true;

    return false;
}

static void wait_i2c_transfer_finished(void)
{
    waitfor(1000, i2c_transfer_finished);      /* wait until interrupt bit has been set */
    MCF_I2C_I2SR &= ~MCF_I2C_I2SR_IIF;          /* clear interrupt bit (byte transfer finished */
}

static bool i2c_bus_free(void)
{
    return (MCF_I2C_I2SR & MCF_I2C_I2SR_IBB);
}

/*
 * TFP410 (DVI) on
 */
static void dvi_on(void)
{
    uint8_t receivedByte;
    uint8_t dummyByte;          /* only used for a dummy read */
    int num_tries = 0;

    xprintf("DVI digital video output initialization: ");

    MCF_I2C_I2FDR = 0x3c;       /* divide system clock by 1280: 100kHz standard */

    do
    {
        /* disable all i2c interrupt routing targets */
        MCF_I2C_I2ICR = 0x0;    // ~(MCF_I2C_I2ICR_IE | MCF_I2C_I2ICR_RE | MCF_I2C_I2ICR_TE | MCF_I2C_I2ICR_BNBE);

        /* disable i2c, disable i2c interrupts, slave, receive, i2c = acknowledge, no repeat start */
        MCF_I2C_I2CR = 0x0;

        /* repeat start, transmit acknowledge */
        MCF_I2C_I2CR = MCF_I2C_I2CR_RSTA | MCF_I2C_I2CR_TXAK;

        receivedByte = MCF_I2C_I2DR;                /* read a byte */
        MCF_I2C_I2SR = 0x0;                         /* clear status register */
        MCF_I2C_I2CR = 0x0;                         /* clear control register */

        MCF_I2C_I2ICR = MCF_I2C_I2ICR_IE;           /* route i2c interrupts to cpu */

        /* i2c enable, master mode, transmit acknowledge */
        MCF_I2C_I2CR = MCF_I2C_I2CR_IEN | MCF_I2C_I2CR_MSTA | MCF_I2C_I2CR_MTX;

        MCF_I2C_I2DR = 0x7a;                        /* send data: address of TFP410 */
        wait_i2c_transfer_finished();

        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)       /* next try if no acknowledge */
            goto try_again;

        MCF_I2C_I2DR = 0x00;                        /* send data: SUB ADRESS 0 */
        wait_i2c_transfer_finished();

        MCF_I2C_I2CR |= MCF_I2C_I2CR_RSTA;          /* repeat start */
        MCF_I2C_I2DR = 0x7b;                        /* begin read */

        wait_i2c_transfer_finished();
        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)       /* next try if no acknowledge */
            goto try_again;

#ifdef _NOT_USED_
        MCH_I2C_I2CR &= ~MCF_I2C_I2CR_MTX;          /* FIXME: not clear where this came from ... */
#endif /* _NOT_USED_ */
        MCF_I2C_I2CR &= 0xef;                       /* ... this actually disables the I2C module... */
        dummyByte = MCF_I2C_I2DR;                   /* dummy read */

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR |= MCF_I2C_I2CR_TXAK;          /* transmit acknowledge enable */
        receivedByte = MCF_I2C_I2DR;                /* read a byte */

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR = MCF_I2C_I2CR_IEN;            /* stop */

        dummyByte = MCF_I2C_I2DR; // dummy read

        if (receivedByte != 0x4c)
            goto try_again;

        MCF_I2C_I2CR = 0x0; // stop
        MCF_I2C_I2SR = 0x0; // clear sr

        waitfor(10000, i2c_bus_free);

        MCF_I2C_I2CR = 0xb0; // on tx master
        MCF_I2C_I2DR = 0x7A;

        wait_i2c_transfer_finished();

        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
            goto try_again;

        MCF_I2C_I2DR = 0x08; // SUB ADRESS 8

        wait_i2c_transfer_finished();

        MCF_I2C_I2DR = 0xbf; // ctl1: power on, T:M:D:S: enable

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR = 0x80; // stop
        dummyByte = MCF_I2C_I2DR; // dummy read
        MCF_I2C_I2SR = 0x0; // clear sr

        waitfor(10000, i2c_bus_free);

        MCF_I2C_I2CR = 0xb0;
        MCF_I2C_I2DR = 0x7A;

        wait_i2c_transfer_finished();

        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
            goto try_again;

        MCF_I2C_I2DR = 0x08; // SUB ADRESS 8

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR |= 0x4; // repeat start
        MCF_I2C_I2DR = 0x7b; // beginn read

        wait_i2c_transfer_finished();

        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
            goto try_again;

        MCF_I2C_I2CR &= 0xef; // switch to rx
        dummyByte = MCF_I2C_I2DR; // dummy read

        wait_i2c_transfer_finished();
        MCF_I2C_I2CR |= 0x08; // txak=1

        wait(50);

        receivedByte = MCF_I2C_I2DR;

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR = 0x80; // stop

        dummyByte = MCF_I2C_I2DR; // dummy read

try_again:
        num_tries++;
    } while ((receivedByte != 0xbf) && (num_tries < 10));

    if (num_tries >= 10)
    {
        xprintf("FAILED!\r\n");
    }
    else
    {
        xprintf("finished\r\n");
    }

    (void) dummyByte;           /* Avoid warning */
}


/*
 * AC97
 */
static void init_ac97(void)
{
    // PSC2: AC97 ----------
    int i;
    int zm;
    int va;
    int vb;
    int vc;

    xprintf("AC97 sound chip initialization: ");
    MCF_PAD_PAR_PSC2 = MCF_PAD_PAR_PSC2_PAR_RTS2_RTS    // PSC2=TX,RX BCLK,CTS->AC'97
           | MCF_PAD_PAR_PSC2_PAR_CTS2_BCLK
             | MCF_PAD_PAR_PSC2_PAR_TXD2
             | MCF_PAD_PAR_PSC2_PAR_RXD2;
    MCF_PSC2_PSCMR1 = 0x0;
    MCF_PSC2_PSCMR2 = 0x0;
    MCF_PSC2_PSCIMR = 0x0300;
    MCF_PSC2_PSCSICR = 0x03;    //AC97
    MCF_PSC2_PSCRFCR = 0x0f000000;
    MCF_PSC2_PSCTFCR = 0x0f000000;
    MCF_PSC2_PSCRFAR = 0x00F0;
    MCF_PSC2_PSCTFAR = 0x00F0;

    for (zm = 0; zm < 100000; zm++) // wiederholen bis synchron
    {
        MCF_PSC2_PSCCR = 0x20;
        MCF_PSC2_PSCCR = 0x30;
        MCF_PSC2_PSCCR = 0x40;
        MCF_PSC2_PSCCR = 0x05;

        // MASTER VOLUME -0dB
        MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
        MCF_PSC2_PSCTB_AC97 = 0x02000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02

        for (i = 2; i < 13; i++)
        {
            MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
        }

        // read register
        MCF_PSC2_PSCTB_AC97 = 0xc0000000;   //START SLOT1 + SLOT2, FIRST FRAME
        MCF_PSC2_PSCTB_AC97 = 0x82000000;   //SLOT1:master volume

        for (i = 2; i < 13; i++)
        {
            MCF_PSC2_PSCTB_AC97 = 0x00000000;   //SLOT2-12:RD REG ALLES 0
        }
        wait(50);

        va = MCF_PSC2_PSCTB_AC97;
        if ((va & 0x80000fff) == 0x80000800) {
            vb = MCF_PSC2_PSCTB_AC97;
            vc = MCF_PSC2_PSCTB_AC97;

            /* FIXME: that looks more than suspicious (Fredi?) */
            /* fixed with parentheses to avoid compiler warnings, but this looks still more than wrong to me */
            if (((va & 0xE0000fff) == 0xE0000800) & (vb == 0x02000000) & (vc == 0x00000000)) {
                goto livo;
            }
        }
    }
    xprintf(" NOT");
livo:
    // AUX VOLUME ->-0dB
    MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
    MCF_PSC2_PSCTB_AC97 = 0x16000000;   //SLOT1:WR REG AUX VOLUME adr 0x16
    MCF_PSC2_PSCTB_AC97 = 0x06060000;   //SLOT1:VOLUME
    for (i = 3; i < 13; i++) {
        MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
    }

    // line in VOLUME +12dB
    MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
    MCF_PSC2_PSCTB_AC97 = 0x10000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02
    for (i = 2; i < 13; i++) {
        MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
    }
    // cd in VOLUME 0dB
    MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
    MCF_PSC2_PSCTB_AC97 = 0x12000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02
    for (i = 2; i < 13; i++) {
        MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
    }
    // mono out VOLUME 0dB
    MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
    MCF_PSC2_PSCTB_AC97 = 0x06000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02
    MCF_PSC2_PSCTB_AC97 = 0x00000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02
    for (i = 3; i < 13; i++) {
        MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
    }
    MCF_PSC2_PSCTFCR |= MCF_PSC_PSCTFCR_WFR;    //set EOF
    MCF_PSC2_PSCTB_AC97 = 0x00000000;   //last data
    xprintf(" finished\r\n");
}
#endif /* MACHINE_FIREBEE */

#ifdef MACHINE_FIREBEE
static void wait_pll(void)
{
    int32_t trgt = MCF_SLT0_SCNT - 100000;
    do
    {
        ;
    } while ((* (volatile int16_t *) 0xf0000800 < 0) && MCF_SLT0_SCNT > trgt);
}

static volatile uint8_t *pll_base = (volatile uint8_t *) 0xf0000600;

static void init_pll(void)
{
    xprintf("FPGA PLL initialization: ");

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x48) = 27;     /* loopfilter  r */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x08) = 1;      /* charge pump 1 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x00) = 12;     /* N counter high = 12 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x40) = 12;     /* N counter low = 12 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x114) = 1;     /* ck1 bypass */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x118) = 1;     /* ck2 bypass */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x11c) = 1;     /* ck3 bypass */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x10) = 1;      /* ck0 high  = 1 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x50) = 1;      /* ck0 low = 1 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x144) = 1;     /* M odd division */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x44) = 1;      /* M low = 1 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x04) = 145;    /* M high = 145 = 146 MHz */

    wait_pll();

    * (volatile uint8_t *) 0xf0000800 = 0;              /* set */

    xprintf("finished\r\n");
}

/*
 * INIT VIDEO DDR RAM
 */
static void init_video_ddr(void) {
    xprintf("init video RAM: ");

    * (volatile uint16_t *) 0xf0000400 = 0xb;   /* set cke = 1, cs=1, config = 1 */
    NOP();

    _VRAM = 0x00050400; /* IPALL */
    NOP();

    _VRAM = 0x00072000; /* load EMR pll on */
    NOP();

    _VRAM = 0x00070122; /* load MR: reset pll, cl=2, burst=4lw */
    NOP();

    _VRAM = 0x00050400; /* IPALL */
    NOP();

    _VRAM = 0x00060000; /* auto refresh */
    NOP();

    _VRAM = 0x00060000; /* auto refresh */
    NOP();

    /* FIXME: what's this? */
    _VRAM = 0x00070022; /* load MR dll on */
    NOP();

    * (uint32_t *) 0xf0000400 = 0x01070082; /* fifo on, refresh on, ddrcs und cke on, video dac on, Falcon shift mode on */

    xprintf("finished\r\n");
}
#endif /* MACHINE_FIREBEE */


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

#ifdef _NOT_USED_
#if defined(MACHINE_FIREBEE)
    /*
     * TIN0 on the Coldfire is connected to the FPGA. TIN0 triggers every write
     * access to 0xff8201 (vbasehi), i.e. everytime the video base address is written
     */
    MCF_GPT0_GMS = MCF_GPT_GMS_ICT(1) |     /* timer 0 on, video change capture on rising edge */
            MCF_GPT_GMS_IEN |
            MCF_GPT_GMS_TMS(1);             /* route GPT0 interrupt on interrupt controller */
    MCF_INTC_ICR62 = MCF_INTC_ICR_IL(7) |
                     MCF_INTC_ICR_IP(6);    /* interrupt level 7, interrupt priority 6 */
    MCF_INTC_IMRH = 0xbffffffe;             /* psc3 and timer 0 int on */
#endif
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
#elif defined(MACHINE_M5484LITE) || defined(MACHINE_M5475EVB)
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
#if defined(MACHINE_M5484LITE) || defined(MACHINE_M5475EVB)
    /*
     * on the MCF 5484 LITEKIT, the second FEC interface is usable
     */
NIF nif2;
#endif

bool spurious_interrupt_handler(void *arg1, void *arg2)
{
    err("spurious interrupt\r\n");
    err("IMRH=%lx, IMRL=%lx\r\n", MCF_INTC_IMRH, MCF_INTC_IMRL);
    err("IPRH=%lx, IPRL=%lx\r\n", MCF_INTC_IPRH, MCF_INTC_IPRL);
    err("IRLR=%x\r\n", MCF_INTC_IRLR);

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
    if (!isr_register_handler(64 + INT_SOURCE_XLBPCI, 3, 0, xlbpci_interrupt_handler, NULL, NULL))
    {
        dbg("Error: unable to register isr for XLB PCI interrupts\r\n");
    }


    /*
     * initialize arbiter timeout registers
     */
    MCF_XLB_XARB_ADRTO = 0x1fffff;
    MCF_XLB_XARB_DATTO = 0x1fffff;
    MCF_XLB_XARB_BUSTO = 0xffffff;


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

    if (!isr_register_handler(64 + INT_SOURCE_XLBARB, 7, 1, xlbarb_interrupt_handler, NULL, NULL))
    {
        dbg("Error: unable to register isr for XLB ARB interrupts\r\n");
    }
}

void ide_init(void)
{
    /* IDE reset */
    * (volatile uint8_t *) (0xffff8802 - 2) = 14;
    * (volatile uint8_t *) (0xffff8802 - 0) = 0x80;
    wait_ms(1);

    * (volatile uint8_t *) (0xffff8802 - 0) = 0;

    set_ide_access_mode();
}

/*
 * probe for NEC compatible USB host controller and install if found
 */
void init_usb(void)
{
    extern struct pci_device_id ohci_usb_pci_table[];
    extern struct pci_device_id ehci_usb_pci_table[];
    struct pci_device_id *board;
    int32_t handle;
    int usb_found = 0;
    int index = 0;

    /*
     * disabled for now
     */
    return;

    inf("USB controller initialization:\r\n");

    do
    {
        handle = pci_find_classcode(PCI_CLASS_SERIAL_USB | PCI_FIND_BASE_CLASS | PCI_FIND_SUB_CLASS, index++);
        dbg("handle 0x%02x\r\n", handle);
        if (handle > 0)
        {
            long id;
            long pci_class;

            xprintf("serial USB found at bus=0x%x, dev=0x%x, fnc=0x%x (0x%x)\r\n",
                    PCI_BUS_FROM_HANDLE(handle),
                    PCI_DEVICE_FROM_HANDLE(handle),
                    PCI_FUNCTION_FROM_HANDLE(handle),
                    handle);
            id = swpl(pci_read_config_longword(handle, PCIIDR));
            pci_class = swpl(pci_read_config_longword(handle, PCIREV));

            if (pci_class >> 8 == PCI_CLASS_SERIAL_USB_EHCI)
            {
                board = ehci_usb_pci_table;
                while (board->vendor)
                {
                    if ((board->vendor == PCI_VENDOR_ID(id)) && board->device == PCI_DEVICE_ID(id))
                    {
                        if (usb_init(handle, board) >= 0)
                        {
                            usb_found++;
                        }
                    }
                    board++;
                }
            }
            if (pci_class >> 8 == PCI_CLASS_SERIAL_USB_OHCI)
            {
                board = ohci_usb_pci_table;
                while (board->vendor)
                {
                    if ((board->vendor == PCI_VENDOR_ID(id)) && board->device == PCI_DEVICE_ID(id))
                    {
                        if (usb_init(handle, board) >= 0)
                            usb_found++;
                    }
                    board++;
                }
            }
        }
    } while (handle >= 0);

    xprintf("finished (found %d USB controller(s))\r\n", usb_found);
}


/* Jump into the OS */
typedef void void_func(void);
struct rom_header
{
    void *initial_sp;
    void_func *initial_pc;
};

/*
 * fix ST RAM header (address 0x0 and 0x4). FreeMiNT uses these vectors on CTRL-ALT-DEL.
 *
 * Beware: Newer compilers refuse to dereference pointers to NULL and abort (trap #7) if the following
 * attribute isn't set.
 */
static void fix_stram_header() __attribute__((optimize("no-delete-null-pointer-checks")));
static void fix_stram_header()
{
    struct rom_header *bas_header = (struct rom_header *) TARGET_ADDRESS;
    struct rom_header *stram_header = (struct rom_header *) 0x0;

    *stram_header = *bas_header;
}

void BaS(void)
{
    uint8_t *src;
    uint8_t *dst = (uint8_t *) TOS;

#if defined(MACHINE_FIREBEE)        // initialize FireBee specific hardware components */
    fpga_configured = init_fpga();

    init_pll();
    init_video_ddr();
    dvi_on();
    init_ac97();
    pic_init();
    nvram_init();
#endif /* MACHINE_FIREBEE */

    driver_mem_init();

    xprintf("initialize MMU: ");
    mmu_init();
    xprintf("finished\r\n");

    xprintf("initialize Coldfire DMA: ");
    dma_init();
    xprintf("finished\r\n");

    xprintf("copy EmuTOS: ");
    /* copy EMUTOS */
    src = (uint8_t *) EMUTOS;
    memcpy(dst, src, EMUTOS_SIZE);
    xprintf("finished\r\n");

    xprintf("flush caches: ");
    flush_and_invalidate_caches();
    xprintf("finished\r\n");

    xprintf("initialize exception vector table: ");
    vec_init();
    xprintf("finished\r\n");


    memset((void *) 0x0200, 0x0, 0x0400);

#if defined(MACHINE_FIREBEE)
    xprintf("IDE reset: \r\n");
    ide_init();
    xprintf("finished\r\n");

    xprintf("enable MMU: ");
    MCF_MMU_MMUCR = MCF_MMU_MMUCR_EN;           /* MMU on */
    NOP();                                      /* force pipeline sync */
    xprintf("finished\r\n");

    xprintf("enable video: ");

    /*
     * ATARI video modes "modeline"
     *
     * horizontal:
     * high word:   h_total
     * low word:    hsync_start
     *
     * vertical:
     * high word    v_total
     * low word     vsync_start
     *
     * can be calculated with umc ("universal modeline generator")
     *
     */
    struct atari_video_timing
    {
        uint16_t total;
        uint16_t sync_start;
    };

    static volatile struct atari_video_timing *hor_640x480 = (volatile struct atari_video_timing *) 0xf0000410;
    static volatile struct atari_video_timing *ver_640x480 = (volatile struct atari_video_timing *) 0xf0000414;
    static volatile struct atari_video_timing *hor_320x240 = (volatile struct atari_video_timing *) 0xf0000418;
    static volatile struct atari_video_timing *ver_320x240 = (volatile struct atari_video_timing *) 0xf000041c;

#undef VIDEO_25MHZ

#ifdef VIDEO_25MHZ
    hor_640x480->total = 0x320;         /* 800 */
    hor_640x480->sync_start = 0x2ba;    /* 698 */
    ver_640x480->total = 0x20c;         /* 524 */
    ver_640x480->sync_start = 0x20a;    /* 522 */

    hor_320x240->total = 0x190;         /* 400 */
    hor_320x240->sync_start = 0x15d;    /* 349 */
    ver_320x240->total = 0x20c;         /* 524 */
    ver_320x240->sync_start = 0x20a;    /* 522 */
#else /* 32 MHz */
    hor_640x480->total = 0x370;         /* 880 */
    hor_640x480->sync_start = 0x2ba;    /* 698 */
    ver_640x480->total = 0x20d;         /* 525 */
    ver_640x480->sync_start = 0x20a;    /* 522 */

    hor_320x240->total = 0x2a0;         /* 672 */
    hor_320x240->sync_start = 0x1e0;    /* 480 */
    ver_320x240->total = 0x5a0;         /* 480 */
    ver_320x240->sync_start = 0x160;    /* 352 */
#endif

    /* fifo on, refresh on, ddrcs and cke on, video dac on */
    * (volatile uint32_t *) 0xf0000400 = 0x01070082;

    xprintf("finished\r\n");
#endif /* MACHINE_FIREBEE */

    sd_card_init();


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
    // init_usb();

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

        void_func* FireTOS = (void_func*) FIRETOS;
        FireTOS();  // Should never return
    }

    /*
     * fix initial pc/sp in ST RAM for FreeMiNT. It expects valid values there
     * like on original STs (where these values reside in ROM) and uses them on
     * CTRL-ALT-DELETE reboots.
     */
    fix_stram_header();

    xprintf("call EmuTOS\r\n");
    struct rom_header *os_header = (struct rom_header *) TOS;
    os_header->initial_pc();
}
