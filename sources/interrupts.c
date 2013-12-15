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
#include "MCF5475.h"
#include "bas_utils.h"
#include "bas_printf.h"
#include "exceptions.h"
#include "interrupts.h"

extern void (*rt_vbr[])(void);
#define VBR	rt_vbr

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
		xprintf("%s: interrupt source %d not defined\r\n", __FUNCTION__, source);
		return -1;
 	}

	lp = MCF_INTC_ICR_IL(level) | MCF_INTC_ICR_IP(priority);
	
	/* check if this combination is already set somewhere */
	for (i = 1; i < 64; i++)
	{
		if (ICR[i] == lp)
		{
			xprintf("%s: level %d and priority %d already used for interrupt source %d!\r\n", __FUNCTION__,
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
