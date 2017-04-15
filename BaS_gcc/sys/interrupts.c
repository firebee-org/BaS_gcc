/*
 * Interrupts
 *
 * Handle interrupts, the levels.
 *
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
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */

#include <bas_types.h>
#include "MCF5475.h"
#include "bas_utils.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "exceptions.h"
#include "interrupts.h"
#include "bas_printf.h"
#include "startcf.h"
#include "cache.h"
#include "util.h"
#include "dma.h"
#include "pci.h"
#include <stdarg.h>

// #define DEBUG
#include "debug.h"

#ifndef MAX_ISR_ENTRY
#define MAX_ISR_ENTRY   (20)
#endif


struct isrentry
{
    int vector;
    bool (*handler)(void *, void *);
    void *hdev;
    void *harg;
};

static struct isrentry isrtab[MAX_ISR_ENTRY];      /* list of interrupt service routines */


/*
 * clear the table of interrupt service handlers
 */
void isr_init(void)
{
    memset(isrtab, 0, sizeof(isrtab));
}

bool isr_set_prio_and_level(int int_source, int priority, int level)
{
    if (int_source > 8 && int_source <= 62)
    {
        /*
        * preset interrupt control registers with level and priority
        */
        dbg("set MCF_INTC_ICR(%d) to priority %d, level %d\r\n",
            int_source, priority, level);
        MCF_INTC_ICR(int_source) = MCF_INTC_ICR_IP(priority) |
                                   MCF_INTC_ICR_IL(level);
    }
    else if (int_source >= 1 && int_source <= 8)
    {
        dbg("interrrupt control register for vector %d is read only!\r\n");
    }
    else
    {
        err("invalid vector - interrupt control register not set.\r\n");
        return false;
    }

    return true;
}

/*
 * enable internal int source in interrupt controller
 */
bool isr_enable_int_source(int int_source)
{
    dbg("anding int_source %d, MCF_INTC_IMR%c = 0x%08x, now 0x%08x\r\n",
        int_source,
        int_source < 32 && int_source > 0 ? 'L' :
                    int_source >= 32 && int_source <= 62 ? 'H' : 'U',
        int_source < 32 && int_source > 0 ? ~(1 << int_source) :
                    int_source >= 32 && int_source <= 62 ? ~(1 << (int_source - 32)) : 0,
        MCF_INTC_IMRH);

    if (int_source < 32 && int_source > 0)
    {
        MCF_INTC_IMRL &= ~(1 << int_source);
    }
    else if (int_source >= 32 && int_source <= 62)
    {
        MCF_INTC_IMRH &= ~(1 << (int_source - 32));
    }
    else
    {
        dbg("vector %d does not correspond to an internal interrupt source\r\n");
        return false;
    }

    return true;
}

/*
 * This function places an interrupt handler in the ISR table,
 * thereby registering it so that the low-level handler may call it.
 *
 * The two parameters are intended for the first arg to be a
 * pointer to the device itself, and the second a pointer to a data
 * structure used by the device driver for that particular device.
 */
bool isr_register_handler(int vector, int level, int priority, bool (*handler)(void *, void *), void *hdev, void *harg)
{
    int index;
    int int_source;

    if ((vector <= 0) || (handler == NULL))
    {
        dbg("illegal vector or handler (vector=%x, handler=%p)!\r\n", vector, handler);

        return false;
    }

    for (index = 0; index < MAX_ISR_ENTRY; index++)
    {
        if (isrtab[index].vector == vector)
        {
            /* one cross each, only! */
            dbg("already set handler with this vector (%d, %d)\r\n", vector);

            return false;
        }

        if (isrtab[index].vector == 0)
        {
            isrtab[index].vector = vector;
            isrtab[index].handler = handler;
            isrtab[index].hdev = hdev;
            isrtab[index].harg = harg;

            int_source = vector - 64;

            if (int_source >= 0)
            {
                if (!isr_enable_int_source(int_source))
                {
                    dbg("failed to enable internal interrupt souce %d in IMRL/IMRH\r\n", int_source);
                    return false;
                }

                if (!isr_set_prio_and_level(int_source, priority, level))
                {
                    dbg("failed to set priority and level for interrupt source %d\r\n", int_source);
                    return false;
                }
            }
            return true;
        }
    }
    dbg("no available slots to register handler for vector %d\n\r", vector);

    return false;   /* no available slots */
}

void isr_remove_handler(bool (*handler)(void *, void *))
{
    /*
     * This routine removes from the ISR table all
     * entries that matches 'handler'.
     */
    int index;

    for (index = 0; index < MAX_ISR_ENTRY; index++)
    {
        if (isrtab[index].handler == handler)
        {
            memset(&isrtab[index], 0, sizeof(struct isrentry));

            return;
        }
    }
    dbg("no such handler registered (handler=%p\r\n", handler);
}

#ifdef DEBUG
static char *vector_to_str[] =
{
    "initial stack pointer",                    /* 0 */
    "initial program counter",                  /* 1 */
    "access error",                             /* 2 */
    "address error",                            /* 3 */
    "illegal instruction",                      /* 4 */
    "divide by zero",                           /* 5 */
    "reserved6",                                /* 6 */
    "reserved7",                                /* 7 */
    "privilege violation",                      /* 8 */
    "trace",                                    /* 9 */
    "unimplemented line-a opcode",              /* 10 */
    "unimplemented line-f opcode",              /* 11 */
    "non-PC breakpoint debug interrupt",        /* 12 */
    "PC breakpoint debug interrupt",            /* 13 */
    "format error",                             /* 14 */
    "uninitialized interrupt",                  /* 15 */
    "reserved16",
    "reserved17",
    "reserved18",
    "reserved19",
    "reserved20",
    "reserved21",
    "reserved22",
    "reserved23",
    "spurious interrupt",                       /* 24 */
    "level 1 autovector",                       /* 25 */
    "level 2 autovector",                       /* 26 */
    "level 3 autovector",                       /* 27 */
    "level 4 autovector",                       /* 28 */
    "level 5 autovector",                       /* 29 */
    "level 6 autovector",                       /* 30 */
    "level 7 autovector",                       /* 31 */
    "trap #0",                                  /* 32 */
    "trap #1",                                  /* 33 */
    "trap #2",                                  /* 34 */
    "trap #3",                                  /* 35 */
    "trap #4",                                  /* 36 */
    "trap #5",                                  /* 37 */
    "trap #6",                                  /* 38 */
    "trap #7",                                  /* 39 */
    "trap #8",                                  /* 40 */
    "trap #9",                                  /* 41 */
    "trap #10"                                  /* 42 */
    "trap #11",                                 /* 43 */
    "trap #12",                                 /* 44 */
    "trap #13",                                 /* 45 */
    "trap #14",                                 /* 46 */
    "trap #15",                                 /* 47 */
    "floating point branch on unordered condition", /* 48 */
    "floting point inexact result",             /* 49 */
    "floating point divide by zero",            /* 50 */
    "floating point underflow",                 /* 51 */
    "floating point operand error",             /* 52 */
    "floating point overflow",                  /* 53 */
    "floating point NaN",                       /* 54 */
    "floating point denormalized number",       /* 55 */
    "reserved56",                               /* 56 */
    "reserved57",
    "reserved58",
    "reserved59",
    "reserved60",
    "unsupported instruction",                  /* 61 */
    "reserved62",                               /* 62 */
    "reserved63",                               /* 63 */
    "", "",
    "edge port 1",                              /* 1 */
    "edge port 2",                              /* 2 */
    "edge port 3",                              /* 3 */
    "edge port 4",                              /* 4 */
    "edge port 5",                              /* 5 */
    "edge port 6",                              /* 6 */
    "edge port 7",                              /* 7 */
    "unused8",
    "unused9",
    "unused10",
    "unused11",
    "unused12",
    "unused13",
    "unused14",
    "USB endpoint 0",                           /* 15 */
    "USB endpoint 1",                           /* 16 */
    "USB endpoint 2",                           /* 17 */
    "USB endpoint 3",                           /* 18 */
    "USB endpoint 4",                           /* 19 */
    "USB endpoint 5",                           /* 20 */
    "USB endpoint 6",                           /* 21 */
    "USB general interrupt",                    /* 22 */
    "USB core interrupt",                       /* 23 */
    "USB OR interrupt",                         /* 24 */
    "DSPI over/underflow",                      /* 25 */
    "DSPI receive FIFO overflow",               /* 26 */
    "DSPI receive FIFO drain",                  /* 27 */
    "DSPI transmit FIFO underflow",             /* 28 */
    "DSPI transfer complete",                   /* 29 */
    "DSPI trasmit FIFO full",                   /* 30 */
    "DSPI end of queue",                        /* 31 */
    "PSC3",                                     /* 32 */
    "PSC2",                                     /* 33 */
    "PSC1",                                     /* 34 */
    "PSC0",                                     /* 35 */
    "Comm timer",                               /* 36 */
    "SEC",                                      /* 37 */
    "FEC1",                                     /* 38 */
    "FEC0",                                     /* 39 */
    "I2C",                                      /* 40 */
    "PCI arbiter",                              /* 41 */
    "comm bus PCI",                             /* 42 */
    "XLB PCI",                                  /* 43 */
    "not used44",
    "not used45",
    "not used46",
    "XLB arbiter to CPU",                       /* 47 */
    "multichannel DMA",                         /* 48 */
    "FlexCAN 0 error",                          /* 49 */
    "FlexCAN 0 bus off",                        /* 50 */
    "FlexCAN 0 message buffer",                 /* 51 */
    "not used52"
    "slice timer 1",                            /* 53 */
    "slice timer 0",                            /* 54 */
    "FlexCAN 1 error",                          /* 55 */
    "FlexCAN 1 bus off",                        /* 56 */
    "FlexCAN 1 message buffer",                 /* 57 */
    "not used58",
    "GPT3",                                     /* 59 */
    "GPT2",                                     /* 60 */
    "GPT1",                                     /* 61 */
    "GPT0",                                     /* 62 */
    "not used63"
};
#endif /* DEBUG */

/*
 * This routine searches the ISR table for an entry that matches
 * 'vector'. If one is found, then 'handler' is executed.
 *
 * This routine returns either true or false where
 *  true = interrupt has been handled, return to caller
 *  false= interrupt has been handled or hasn't, but needs to be forwarded to TOS
 */
bool isr_execute_handler(int vector)
{
    int index;

    dbg("vector = %d (%s)\r\n", vector, vector_to_str[vector]);

    /*
     * locate an interrupt service routine handler.
     */
    for (index = 0; index < MAX_ISR_ENTRY; index++)
    {
        if (isrtab[index].vector == vector)
        {
            isrtab[index].handler(isrtab[index].hdev, isrtab[index].harg);
            return true;
        }
    }
    err("no isr handler for vector %d found. Spurious?\r\n", vector);

    return true;
}

#if defined(MACHINE_FIREBEE)
/*
 * PIC interrupt handler for Firebee
 *
 * Handles PIC requests that come in from PSC3 serial interface. Currently, that
 * is RTC/NVRAM requests only
 */
bool pic_interrupt_handler(void *arg1, void *arg2)
{
    uint8_t rcv_byte;

    dbg("PIC interrupt\r\n");

    rcv_byte = read_pic_byte();
    if (rcv_byte == 2)                  /* PIC requests RTC data */
    {
        volatile uint8_t *rtc_reg = (uint8_t *) 0xffff8961;
        volatile uint8_t *rtc_data = (uint8_t *) 0xffff8963;
        int index = 0;

        err("PIC interrupt: requesting RTC data\r\n");

        write_pic_byte(0x82);     // header byte to PIC
        do
        {
            *rtc_reg = index;
            write_pic_byte(*rtc_data);
        } while (++index < 64);
    }
    return true;
}
#endif /* MACHINE_FIREBEE */

bool xlbpci_interrupt_handler(void *arg1, void *arg2)
{
    uint32_t reason;

    dbg("XLB PCI interrupt\r\n");

    reason = MCF_PCI_PCIISR;

    if (reason & MCF_PCI_PCIISR_RE)
    {
        dbg("Retry error. Retry terminated or max retries reached. Cleared\r\n");
        MCF_PCI_PCIISR |= MCF_PCI_PCIISR_RE;
    }

    if (reason & MCF_PCI_PCIISR_IA)
    {
        dbg("Initiator abort. No target answered in time. Cleared.\r\n");
        MCF_PCI_PCIISR |= MCF_PCI_PCIISR_IA;
    }

    if (reason & MCF_PCI_PCIISR_TA)
    {
        dbg("Target abort. Cleared.\r\n");
        MCF_PCI_PCIISR |= MCF_PCI_PCIISR_TA;
    }

    return true;
}

bool pciarb_interrupt_handler(void *arg1, void *arg2)
{
    dbg("PCI ARB interrupt\r\n");

    MCF_PCIARB_PASR |= MCF_PCIARB_PASR_EXTMBK(0x1f) | MCF_PCIARB_PASR_ITLMBK;
    return true;
}

bool xlbarb_interrupt_handler(void *arg1, void *arg2)
{
    uint32_t status = MCF_XLB_XARB_SR;

    dbg("arg1=0x%08x arg2=0x%08x\r\n", arg1, arg2);

    /*
     * TODO: we should probably issue a bus error when this occors
     */
    err("XLB arbiter interrupt\r\n");
    err("captured address: 0x%08lx\r\n", MCF_XLB_XARB_ADRCAP);

    MCF_XLB_XARB_ADRCAP = 0x0L;
    MCF_XLB_XARB_SIGCAP = 0x0L;

    if (status & MCF_XLB_XARB_SR_AT)
        err("address tenure timeout\r\n");
    if (status & MCF_XLB_XARB_SR_DT)
        err("data tenure timeout\r\n");
    if (status & MCF_XLB_XARB_SR_BA)
        err("bus activity tenure timeout\r\n");
    if (status & MCF_XLB_XARB_SR_TTM)
        err("TBST/TSIZ mismatch\r\n");
    if (status & MCF_XLB_XARB_SR_ECW)
        err("external control word read/write\r\n");
    if (status & MCF_XLB_XARB_SR_TTR)
        err("TT reserved\r\n");
    if (status & MCF_XLB_XARB_SR_TTA)
        err("TT address only\r\n");
    if (status & MCF_XLB_XARB_SR_MM)
        err("multiple masters at priority 0\r\n");
    if (status & MCF_XLB_XARB_SR_SEA)
        err("slave error acknowledge\r\n");

    /*
     * acknowledge interrupt
     */
    MCF_XLB_XARB_SR = status;       /* rwc bits */

    return true;
}

#if defined(MACHINE_FIREBEE)
/*
 * This gets called from irq5 in exceptions.S
 *
 * IRQ5 are the "FBEE" (PIC, ETH PHY, PCI, DVI monitor sense and DSP) interrupts multiplexed by the FPGA interrupt handler
 */
bool irq5_handler(void *arg1, void *arg2)
{
    uint32_t pending_interrupts = FBEE_INTR_PENDING;

    dbg("IRQ5!\r\n");
    if (pending_interrupts & FBEE_INTR_PIC)
    {
        dbg("PIC interrupt\r\n");
        FBEE_INTR_CLEAR = FBEE_INTR_PIC;
    }

    if (pending_interrupts & FBEE_INTR_ETHERNET)
    {
        dbg("ethernet 0 PHY interrupt\r\n");
        FBEE_INTR_CLEAR = FBEE_INTR_ETHERNET;
    }

    if (pending_interrupts & FBEE_INTR_DVI)
    {
        dbg("DVI monitor sense interrupt\r\n");
        FBEE_INTR_CLEAR = FBEE_INTR_DVI;
    }

    if (pending_interrupts & FBEE_INTR_PCI_INTA ||
        pending_interrupts & FBEE_INTR_PCI_INTB ||
        pending_interrupts & FBEE_INTR_PCI_INTC ||
        pending_interrupts & FBEE_INTR_PCI_INTD)
    {
        int handle;

        if ((handle = pci_get_interrupt_cause() != -1))
        {
            pci_call_interrupt_chain(handle, 0L);
        }
        dbg("PCI interrupt IRQ5\r\n");
        FBEE_INTR_CLEAR = FBEE_INTR_PCI_INTA |
                          FBEE_INTR_PCI_INTB |
                          FBEE_INTR_PCI_INTC |
                          FBEE_INTR_PCI_INTD;
    }

    if (pending_interrupts & FBEE_INTR_DSP)
    {
        dbg("DSP interrupt\r\n");
        FBEE_INTR_CLEAR = FBEE_INTR_DSP;
    }

    MCF_EPORT_EPFR |= (1 << 5);             /* clear interrupt from edge port */

    return true;
}

/*
 * blink the Firebee's LED to show we are still alive
 */
void blink_led(void)
{
    static uint16_t blinker = 0;

    if ((blinker++ & 0x80) > 0)
    {
        MCF_GPIO_PODR_FEC1L |= (1 << 4);    /* LED off */
    }
    else
    {
        MCF_GPIO_PODR_FEC1L &= ~(1 << 4);   /* LED on */
    }
}

/*
 * Atari MFP interrupt registers.
 *
 * TODO: should go into a header file
 */

#define FALCON_MFP_IERA     *((volatile uint8_t *) 0xfffffa07)
#define FALCON_MFP_IERB     *((volatile uint8_t *) 0xfffffa09)
#define FALCON_MFP_IPRA     *((volatile uint8_t *) 0xfffffa0b)
#define FALCON_MFP_IPRB     *((volatile uint8_t *) 0xfffffa0d)
#define FALCON_MFP_IMRA     *((volatile uint8_t *) 0xfffffa13)
#define FALCON_MFP_IMRB     *((volatile uint8_t *) 0xfffffa15)

bool irq6_acsi_dma_interrupt(void)
{
    dbg("ACSI DMA interrupt\r\n");

    /*
     * TODO: implement handler
     */

    return false;
}

bool irq6_handler(uint32_t sf1, uint32_t sf2)
{
    //err("IRQ6!\r\n");

    if (FALCON_MFP_IPRA || FALCON_MFP_IPRB)
    {
        blink_led();
    }

    MCF_EPORT_EPFR |= (1 << 6); /* clear int6 from edge port */

    return false;               /* always forward IRQ6 to TOS */
}

#else /* MACHINE_FIREBEE */

bool irq5_handler(void *arg1, void *arg2)
{
    MCF_EPORT_EPFR |= (1 << 5); /* clear int5 from edge port */

    return true;
}

bool irq6_handler(void *arg1, void *arg2)
{
    err("IRQ6!\r\n");

    MCF_EPORT_EPFR |= (1 << 6); /* clear int6 from edge port */

    return false;       /* always forward IRQ6 to TOS */
}

/*
 * This gets called from irq7 in exceptions.S
 * Once we arrive here, the SR has been set to disable interrupts and the gcc scratch registers have been saved
 */
bool irq7_handler(void)
{
    int32_t handle;
    int32_t value = 0;
    int32_t newvalue;

    MCF_EPORT_EPFR |= (1 << 7);
    dbg("IRQ7!\r\n");
    if ((handle = pci_get_interrupt_cause()) > 0)
    {
        newvalue = pci_call_interrupt_chain(handle, value);
        if (newvalue == value)
        {
            dbg("interrupt not handled!\r\n");
        }
    }
    MCF_EPORT_EPFR |= (1 << 7); /* clear int7 from edge port */

    return true;
}
#endif /* MACHINE_M548X */

#if defined(MACHINE_FIREBEE)
/*
 * this is the higlevel interrupt service routine for gpt0 timer interrupts.
 *
 * It is called from handler_gpt0 in exceptions.S
 *
 * The gpt0 timer is not used as a timer, but as interrupt trigger by the FPGA which fires
 * everytime the video base address high byte (0xffff8201) gets written by user code (i.e.
 * everytime the video base address is set).
 * The interrupt service routine checks if that page was already set as a video page (in that
 * case it does nothing), if not (if we have a newly set page), it sets up an MMU mapping for
 * that page (effectively rerouting any further access to Falcon video RAM to Firebee FPGA
 * video RAM starting at 0x60000000) and copies SDRAM contents of that page to the video
 * RAM page.
 */
bool gpt0_interrupt_handler(void *arg0, void *arg1)
{
    dbg("gpt0 handler called\n\r");

    MCF_GPT0_GMS &= ~1;     /* rearm trigger */
    NOP();
    MCF_GPT0_GMS |= 1;

    return true;
}
#endif /* MACHINE_FIREBEE */


uint32_t set_ipl(uint32_t ipl)
{
    uint32_t ret;

    __asm__ __volatile__(
        "		move.w	sr,%[ret]\r\n"			/* retrieve status register */
        "		andi.l	#0x07,%[ipl]\n\t"		/* mask out ipl bits on new value */
        "		lsl.l	#8,%[ipl]\n\t"			/* shift them to position */
        "		move.l	%[ret],d0\n\t"			/* retrieve original value */
        "		andi.l	#0x0000f8ff,d0\n\t"		/* clear ipl part */
        "		or.l	%[ipl],d0\n\t"			/* or in new value */
        "		move.w	d0,sr\n\t"				/* put it in place */
        "		andi.l	#0x0700,%[ret]\r\n"		/* mask out ipl bits */
        "		lsr.l	#8,%[ret]\r\n"			/* shift them to position */
        : [ret] "=&d" (ret)		/* output */
        : [ipl] "d" (ipl)		/* input */
        : "d0", "cc"	/* clobber */
    );

    return ret;
}
