/*
 * Interrupts
 *
 * Handle interrupts, the levels.
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

#include <stdint.h>
#include <stdbool.h>
#include "MCF5475.h"
#include "bas_utils.h"
#include "bas_printf.h"
#include "exceptions.h"
#include "interrupts.h"
#include "bas_printf.h"
#include "startcf.h"
#include "cache.h"
#include "util.h"

extern void (*rt_vbr[])(void);
#define VBR	rt_vbr

#define IRQ_DEBUG
#if defined(IRQ_DEBUG)
#define dbg(format, arg...) do { xprintf("DEBUG %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif

/*
 * register an interrupt handler at the Coldfire interrupt controller and add the handler to
 * the interrupt vector table
 */
int register_interrupt_handler(uint8_t source, uint8_t level, uint8_t priority, uint8_t intr, void (*handler)(void))
{
	int ipl;
	int i;
	volatile uint8_t *ICR = &MCF_INTC_ICR01 - 1;
	uint8_t lp;

	source &= 63;
	priority &= 7;

	if (source < 1 || source > 63)
	{
		dbg("interrupt source %d not defined\r\n", source);
		return -1;
	}

	lp = MCF_INTC_ICR_IL(level) | MCF_INTC_ICR_IP(priority);

	/* check if this combination is already set somewhere */
	for (i = 1; i < 64; i++)
	{
		if (ICR[i] == lp)
		{
			dbg("level %d and priority %d already used for interrupt source %d!\r\n",
					level, priority, i);
			return -1;
		}
	}

	/* disable interrupts */
	ipl = set_ipl(7);

	VBR[64 + source] = handler;	/* first 64 vectors are system exceptions */

	/* set level and priority in interrupt controller */
	ICR[source] = lp;

	/* set interrupt mask to where it was before */
	set_ipl(ipl);

	return 0;
}

#ifndef MAX_ISR_ENTRY
#define MAX_ISR_ENTRY   (20)
#endif


typedef struct
{
	int     vector;
	int     (*handler)(void *, void *);
	void    *hdev;
	void    *harg;
} ISRENTRY;

ISRENTRY isrtab[MAX_ISR_ENTRY];


void isr_init(void)
{
	int index;

	for (index = 0; index < MAX_ISR_ENTRY; index++)
	{
		isrtab[index].vector = 0;
		isrtab[index].handler = 0;
		isrtab[index].hdev = 0;
		isrtab[index].harg = 0;
	}
}


int isr_register_handler(int vector, int (*handler)(void *, void *), void *hdev, void *harg)
{
	/*
	 * This function places an interrupt handler in the ISR table,
	 * thereby registering it so that the low-level handler may call it.
	 *
	 * The two parameters are intended for the first arg to be a
	 * pointer to the device itself, and the second a pointer to a data
	 * structure used by the device driver for that particular device.
	 */
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
			isrtab[index].vector = 0;
			isrtab[index].handler = 0;
			isrtab[index].hdev = 0;
			isrtab[index].harg = 0;

			return;
		}
	}
	dbg("no such handler registered (handler=%p\r\n", handler);
}


bool isr_execute_handler(int vector)
{
	/*
	 * This routine searches the ISR table for an entry that matches
	 * 'vector'.  If one is found, then 'handler' is executed.
	 */
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
 */
void pic_interrupt_handler(void)
{
	uint8_t rcv_byte;

	rcv_byte = MCF_PSC3_PSCRB_8BIT;
	if (rcv_byte == 2)	// PIC requests RTC data
	{
		uint8_t *rtc_reg= (uint8_t *) 0xffff8961;
		uint8_t *rtc_data = (uint8_t *) 0xffff8963;
		int index = 0;

		xprintf("PIC interrupt requesting RTC data\r\n");

		MCF_PSC3_PSCTB_8BIT = 0x82;		// header byte to PIC
		do
		{
			*rtc_reg = 0;
			MCF_PSC3_PSCTB_8BIT = *rtc_data;
		} while (index++ < 64);
	}
}

extern int32_t video_sbt;
extern int32_t video_tlb;

void video_addr_timeout(void)
{
	uint32_t addr = 0x0L;
	uint32_t *src;
	uint32_t *dst;
	uint32_t asid;

	dbg("video address timeout\r\n");
	flush_and_invalidate_caches();

	do
	{
		uint32_t tlb;
		uint32_t page_attr;

		/*
		 * search tlb entry id for addr (if not available, the MMU
		 * will provide a new one based on its LRU algorithm)
		 */
		MCF_MMU_MMUAR = addr;
		MCF_MMU_MMUOR =
				MCF_MMU_MMUOR_STLB |
				MCF_MMU_MMUOR_RW |
				MCF_MMU_MMUOR_ACC;
		NOP();
		tlb = (MCF_MMU_MMUOR >> 16) & 0xffff;

		/*
		 * retrieve tlb entry with the found TLB entry id
		 */
		MCF_MMU_MMUAR = tlb;
		MCF_MMU_MMUOR =
				MCF_MMU_MMUOR_STLB |
				MCF_MMU_MMUOR_ADR |
				MCF_MMU_MMUOR_RW |
				MCF_MMU_MMUOR_ACC;
		NOP();

		asid = (MCF_MMU_MMUTR >> 2) & 0x1fff;	/* fetch ASID of page */;
		if (asid != sca_page_ID)                /* check if screen area */
		{
			addr += 0x100000;
			continue;                           /* next page */
		}

		/* modify found TLB entry */
		if (addr == 0x0)
		{
			page_attr =
				MCF_MMU_MMUDR_LK |
				MCF_MMU_MMUDR_SZ(0) |
				MCF_MMU_MMUDR_CM(0) |
				MCF_MMU_MMUDR_R |
				MCF_MMU_MMUDR_W |
				MCF_MMU_MMUDR_X;
		}
		else
		{
			page_attr =
				MCF_MMU_MMUTR_SG |
				MCF_MMU_MMUTR_V;
		}


		MCF_MMU_MMUTR = addr;
		MCF_MMU_MMUDR = page_attr;
		MCF_MMU_MMUOR =
				MCF_MMU_MMUOR_STLB |
				MCF_MMU_MMUOR_ADR |
				MCF_MMU_MMUOR_ACC |
				MCF_MMU_MMUOR_UAA;
		NOP();

		dst = (uint32_t *) 0x60000000 + addr;
		src = (uint32_t *) addr;
		while (dst < (uint32_t *) 0x60000000 + addr + 0x10000)
		{
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
		}



		addr += 0x100000;
	} while (addr < 0xd00000);
	video_tlb = 0x2000;
	video_sbt = 0;
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

	MCF_EPORT_EPFR |= (1 << 6);	/* clear int6 from edge port */

	if (video_sbt != 0 && (video_sbt - 0x70000000) > MCF_SLT0_SCNT)
	{
		video_addr_timeout();
		handled = true;
	}

	/*
	 * check if ACSI DMA interrupt
	 */

	if (FALCON_MFP_IERA & (1 << 7))
	{
		/* ACSI interrupt is enabled */
		if (FALCON_MFP_IPRA & (1 << 7))
		{
			irq6_acsi_dma_interrupt();
			handled = true;
		}
	}

	if (FALCON_MFP_IPRA || FALCON_MFP_IPRB)
	{
		blink_led();
	}

	return handled;
}
