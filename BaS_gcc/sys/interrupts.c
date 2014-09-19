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
#include "bas_string.h"
#include "exceptions.h"
#include "interrupts.h"
#include "bas_printf.h"
#include "startcf.h"
#include "cache.h"
#include "util.h"
#include "dma.h"

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

		xprintf("PIC interrupt requesting RTC data\r\n");

		MCF_PSC3_PSCTB_8BIT = 0x82;		// header byte to PIC
		do
		{
			*rtc_reg = 0;
			MCF_PSC3_PSCTB_8BIT = *rtc_data;
		} while (index++ < 64);
	}
	return 1;
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
	uint32_t video_address;
	uint32_t video_end_address;
	int page_number;
	bool already_set;
	extern uint32_t _STRAM_END;

	dbg("screen base = 0x%x\r\n", vbasehi);

	if (vbasehi < 2)			/* screen base lower than 0x20000? */
	{
		goto rearm_trigger;		/* do nothing */
	}
	else if (vbasehi >= 0xd0)	/* higher than 0xd00000 (normal Falcon address)? */
	{
		video_sbt = MCF_SLT0_SCNT;	/* FIXME: no idea why we need to save the time here */
	}
	video_address = (vbasehi << 16) | (vbasemid << 8) | vbaselow;

	page_number = video_address >> 20;				/* calculate a page number */
	already_set = (video_tlb & (1 << page_number));	/* already in bitset? */
	video_tlb |= page_number;						/* set it */

	if (! already_set)	/* newly set page, need to copy contents */
	{
		flush_and_invalidate_caches();
		dma_memcpy((uint8_t *) video_address + 0x60000000, (uint8_t *) video_address, 0x100000);

		/*
		 * create an MMU TLB entry for the new video page
		 */

		/*
		 * first search for an existing entry with our address. If none is found,
		 * the MMU will propose a new one
		 */
		MCF_MMU_MMUAR = video_address;
		MCF_MMU_MMUOR = 0x106;
		NOP();

		/*
		 * take this MMU TLB entry and set it to our video address and page mapping
		 */
		MCF_MMU_MMUAR = (MCF_MMU_MMUOR >> 16) & 0xffff;	/* set TLB id */

		MCF_MMU_MMUTR = video_address |
						MCF_MMU_MMUTR_ID(sca_page_ID) |	/* set video page ID */
						MCF_MMU_MMUTR_SG |				/* shared global */
						MCF_MMU_MMUTR_V;				/* valid */
		MCF_MMU_MMUDR = (video_address + 0x60000000) |	/* physical address */
						MCF_MMU_MMUDR_SZ(0) |			/* 1 MB page size */
						MCF_MMU_MMUDR_CM(0) |			/* writethrough */
						MCF_MMU_MMUDR_R |				/* readable */
						MCF_MMU_MMUDR_W |				/* writeable */
						MCF_MMU_MMUDR_X;				/* executable */
		MCF_MMU_MMUOR = 0x10b;							/* update TLB entry */
	}

	/*
	 * Calculate the effective screen memory size to see if we need to map another page
	 * in case the new screen spans more than one single page
	 */
	video_end_address = video_address + (vde - vdb) * vwrap;
	if (video_end_address < _STRAM_END)
	{
		page_number = video_end_address >> 20;			/* calculate a page number */
		already_set = (video_tlb & (1 << page_number));	/* already in bitset? */
		video_tlb |= page_number;						/* set it */

		if (! already_set)	/* newly set page, need to copy contents */
		{
			flush_and_invalidate_caches();
			dma_memcpy((uint8_t *) video_end_address + 0x60000000, (uint8_t *) video_end_address, 0x100000);

			/*
			 * create an MMU TLB entry for the new video page
			 */

			/*
			 * first search for an existing entry with our address. If none is found,
			 * the MMU will propose a new one
			 */
			MCF_MMU_MMUAR = video_end_address;
			MCF_MMU_MMUOR = 0x106;
			NOP();

			/*
			 * take this MMU TLB entry and set it to our video address and page mapping
			 */
			MCF_MMU_MMUAR = (MCF_MMU_MMUOR >> 16) & 0xffff;	/* set TLB id */

			MCF_MMU_MMUTR = video_end_address |
							MCF_MMU_MMUTR_ID(sca_page_ID) |	/* set video page ID */
							MCF_MMU_MMUTR_SG |				/* shared global */
							MCF_MMU_MMUTR_V;				/* valid */
			MCF_MMU_MMUDR = (video_end_address + 0x60000000) |	/* physical address */
							MCF_MMU_MMUDR_SZ(0) |			/* 1 MB page size */
							MCF_MMU_MMUDR_CM(0) |			/* writethrough */
							MCF_MMU_MMUDR_R |				/* readable */
							MCF_MMU_MMUDR_W |				/* writeable */
							MCF_MMU_MMUDR_X;				/* executable */
			MCF_MMU_MMUOR = 0x10b;							/* update TLB entry */
		}
	}
rearm_trigger:
	MCF_GPT0_GMS &= ~1;		/* rearm trigger */
	NOP();
	MCF_GPT0_GMS |= 1;
}
#endif /* MACHINE_FIREBEE */
