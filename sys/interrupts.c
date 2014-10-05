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

extern void (*rt_vbr[])(void);
#define VBR	rt_vbr

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

/*
 * This function places an interrupt handler in the ISR table,
 * thereby registering it so that the low-level handler may call it.
 *
 * The two parameters are intended for the first arg to be a
 * pointer to the device itself, and the second a pointer to a data
 * structure used by the device driver for that particular device.
 */
int isr_register_handler(int vector, int (*handler)(void *, void *), void *hdev, void *harg)
{
    int index;

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
 * 'vector'.  If one is found, then 'handler' is executed.
 */
bool isr_execute_handler(int vector)
{
    int index;
    bool retval = false;

    /*
     * locate a BaS Interrupt Service Routine handler.
     */
    for (index = 0; index < MAX_ISR_ENTRY; index++)
    {
        if (isrtab[index].vector == vector)
        {
            retval = true;

            if (isrtab[index].handler(isrtab[index].hdev, isrtab[index].harg))
            {
                return retval;
            }
        }
    }
    dbg("no BaS isr handler for vector %d found\r\n", vector);

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
    if (rcv_byte == 2)	// PIC requests RTC data
    {
        uint8_t *rtc_reg = (uint8_t *) 0xffff8961;
        uint8_t *rtc_data = (uint8_t *) 0xffff8963;
        int index = 0;

        xprintf("PIC interrupt: requesting RTC data\r\n");

        MCF_PSC3_PSCTB_8BIT = 0x82;		// header byte to PIC
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

bool irq6_interrupt_handler(uint32_t sf1, uint32_t sf2)
{
    bool handled = false;

    //err("IRQ6!\r\n");
    MCF_EPORT_EPFR |= (1 << 6);	/* clear int6 from edge port */

    if (FALCON_MFP_IPRA || FALCON_MFP_IPRB)
    {
        blink_led();
    }

    return handled;
}

#if defined(MACHINE_FIREBEE)
/*
 * This gets called from irq5 in exceptions.S
 * Once we arrive here, the SR has been set to disable interrupts and the gcc scratch registers have been saved
 */
int irq5_handler(void)
{
    int32_t handle;
    int32_t value = 0;
    int32_t newvalue;

    err("FPGA_INTR_CONTROL = 0x%08x\r\n", * FPGA_INTR_CONTROL);
    err("FPGA_INTR_ENABLE  = 0x%08x\r\n", * FPGA_INTR_ENABLE);
    err("FPGA_INTR_CLEAR   = 0x%08x\r\n", * FPGA_INTR_CLEAR);
    err("FPGA_INTR_PENDING = 0x%08x\r\n", * FPGA_INTR_PENDING);

    * FPGA_INTR_CLEAR &= ~0x20000000UL;     /* clear interrupt from FPGA */
    err("\r\nFPGA_INTR_CLEAR = 0x%08x\r\n", * FPGA_INTR_CLEAR);
    //MCF_EPORT_EPFR |= (1 << 5);             /* clear interrupt from edge port */

    //xprintf("IRQ5!\r\n");

    if ((handle = pci_get_interrupt_cause()) > 0)
    {
        newvalue = pci_call_interrupt_chain(handle, value);
        if (newvalue == value)
        {
            dbg("interrupt not handled!\r\n");
            return 1;
        }
    }
    return 0;
}
#endif /* MACHINE_FIREBEE */

#ifdef MACHINE_M5484LITE
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
#define vbasehi		(* (volatile uint8_t *) 0xffff8201)
#define vbasemid	(* (volatile uint8_t *) 0xffff8203)
#define vbaselow	(* (volatile uint8_t *) 0xffff820d)

#define vwrap		(* (volatile uint16_t *) 0xffff8210)
#define vde			(* (volatile uint16_t *) 0xffff82aa)
#define vdb			(* (volatile uint16_t *) 0xffff82a8)
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
    dbg("screen base = 0x%x\r\n", vbasehi);

    MCF_GPT0_GMS &= ~1;		/* rearm trigger */
    NOP();
    MCF_GPT0_GMS |= 1;
}
#endif /* MACHINE_FIREBEE */
