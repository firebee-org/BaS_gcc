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

#define IRQ_DEBUG
#if defined(IRQ_DEBUG)
#define dbg(format, arg...) do { xprintf("DEBUG %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif
#define err(format, arg...) do { xprintf("DEBUG %s(): " format, __FUNCTION__, ##arg); } while (0)


#ifndef MAX_ISR_ENTRY
#define MAX_ISR_ENTRY   (20)
#endif


struct isrentry
{
    int     vector;
    int     (*handler)(void *, void *);
    void    *hdev;
    void    *harg;
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
 * enable internal int source in DMA controller
 */
bool isr_enable_int_source(int int_source)
{
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
        err("vector %d does not correspond to an internal interrupt source\r\n");
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
bool isr_register_handler(int vector, int level, int priority, int (*handler)(void *, void *), void *hdev, void *harg)
{
    int index;
    int int_source;

    if ((vector == 0) || (handler == NULL))
    {
        dbg("illegal vector or handler!\r\n");

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

            if (!isr_enable_int_source(int_source))
            {
                err("failed to enable internal interrupt souce %d in IMRL/IMRH\r\n", int_source);
                return false;
            }

            if (!isr_set_prio_and_level(int_source, priority, level))
            {
                err("failed to set priority and level for interrupt source %d\r\n", int_source);
                return false;
            }

            return true;
        }
    }
    dbg("no available slots to register handler for vector %d\n\r", vector);

    return false;   /* no available slots */
}

void isr_remove_handler(int (*handler)(void *, void *))
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

/*
 * This routine searches the ISR table for an entry that matches
 * 'vector'. If one is found, then 'handler' is executed.
 */
bool isr_execute_handler(int vector)
{
    int index;
    bool retval = false;

    dbg("vector = %d\r\n", vector);

    /*
     * locate an interrupt service routine handler.
     */
    for (index = 0; index < MAX_ISR_ENTRY; index++)
    {
        if (isrtab[index].vector == vector)
        {
            return isrtab[index].handler(isrtab[index].hdev, isrtab[index].harg);
        }
    }
    err("no isr handler for vector %d found. Spurious?\r\n", vector);

    return retval;
}

/*
 * PIC interrupt handler for Firebee
 *
 * Handles PIC requests that come in from PSC3 serial interface. Currently, that
 * is RTC/NVRAM requests only
 */
int pic_interrupt_handler(void *arg1, void *arg2)
{
    uint8_t rcv_byte;

    rcv_byte = MCF_PSC3_PSCRB_8BIT;
    if (rcv_byte == 2)  // PIC requests RTC data
    {
        uint8_t *rtc_reg = (uint8_t *) 0xffff8961;
        uint8_t *rtc_data = (uint8_t *) 0xffff8963;
        int index = 0;

        err("PIC interrupt: requesting RTC data\r\n");

        MCF_PSC3_PSCTB_8BIT = 0x82;     // header byte to PIC
        do
        {
            *rtc_reg = 0;
            MCF_PSC3_PSCTB_8BIT = *rtc_data;
        } while (index++ < 64);
    }
    return 1;
}

int xlbpci_interrupt_handler(void *arg1, void *arg2)
{
    dbg("XLB PCI interrupt\r\n");

    return 1;
}

int pciarb_interrupt_handler(void *arg1, void *arg2)
{
    dbg("PCI ARB interrupt\r\n");

    return 1;
}

#if defined(MACHINE_FIREBEE)
/*
 * This gets called from irq5 in exceptions.S
 *
 * IRQ5 are the "FBEE" (PIC, ETH PHY, PCI, DVI monitor sense and DSP) interrupts multiplexed by the FPGA interrupt handler
 *
 * Once we arrive here, the SR has been set to disable interrupts and the gcc scratch registers have been saved
 */
int irq5_handler(void *arg1, void *arg2)
{
    uint32_t pending_interrupts = FBEE_INTR_PENDING;

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
        dbg("PCI interrupt\r\n");
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

    if (pending_interrupts & FBEE_INTR_VSYNC || pending_interrupts & FBEE_INTR_HSYNC)
    {
        /* hsync and vsync should go to TOS unhandled */
        return 1;
    }

    MCF_EPORT_EPFR |= (1 << 5);             /* clear interrupt from edge port */

    return 0;
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
    bool handled = false;

    // err("IRQ6!\r\n");
    MCF_EPORT_EPFR |= (1 << 6); /* clear int6 from edge port */

    if (FALCON_MFP_IPRA || FALCON_MFP_IPRB)
    {
        blink_led();
    }

    return handled;
}

#else /* MACHINE_FIREBEE */

int irq5_handler(void *arg1, void *arg2)
{
    return 0;
}

bool irq6_handler(void *arg1, void *arg2)
{
    err("IRQ6!\r\n");

    return 0;
}

/*
 * This gets called from irq7 in exceptions.S
 * Once we arrive here, the SR has been set to disable interrupts and the gcc scratch registers have been saved
 */
void irq7_handler(void)
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
void gpt0_interrupt_handler(void)
{
    dbg("handler called\n\r");

    MCF_GPT0_GMS &= ~1;     /* rearm trigger */
    NOP();
    MCF_GPT0_GMS |= 1;
}
#endif /* MACHINE_FIREBEE */
