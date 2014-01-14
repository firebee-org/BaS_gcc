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

extern void (*rt_vbr[])(void);
#define VBR	rt_vbr

#define IRQ_DEBUG
#if defined(IRQ_DEBUG)
#define dbg(format, arg...) do { xprintf("DEBUG: " format, ##arg); } while (0)
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
		dbg("%s: interrupt source %d not defined\r\n", __FUNCTION__, source);
		return -1;
	}

	lp = MCF_INTC_ICR_IL(level) | MCF_INTC_ICR_IP(priority);

	/* check if this combination is already set somewhere */
	for (i = 1; i < 64; i++)
	{
		if (ICR[i] == lp)
		{
			dbg("%s: level %d and priority %d already used for interrupt source %d!\r\n", __FUNCTION__,
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
		dbg("%s: illegal vector or handler!\r\n", __FUNCTION__);
		return false;
	}

	for (index = 0; index < MAX_ISR_ENTRY; index++)
	{
		if (isrtab[index].vector == vector) 
		{
			/* one cross each, only! */
			dbg("%s: already set handler with this vector (%d, %d)\r\n", __FUNCTION__, vector);
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
	dbg("%s: no available slots to register handler for vector %d\n\r", __FUNCTION__, vector);

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
	dbg("%s: no such handler registered (handler=%p\r\n", __FUNCTION__, handler);
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
	 * First locate a BaS Interrupt Service Routine handler.
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
	dbg("%s: no BaS isr handler for vector %d found\r\n", __FUNCTION__, vector);
	return retval;
}

