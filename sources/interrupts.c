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
#include "interrupts.h"

extern uint32_t rt_vbr[];
#define VBR	rt_vbr

/*
 * register an interrupt handler at the Coldfire interrupt controller and add the handler to
 * the interrupt vector table
 */
int register_interrupt_handler(uint8_t priority, uint8_t source, void (*func)())
{
	int i;
	uint8_t level = 0b01111111;
	uint32_t **adr = &VBR[0];

	source &= 63;
	priority &= 7;

	if (source <= 0)
		return -1;

	for (i = 1; i < 64; i++)
	{
		if (i != source)
		{
			if ((MCF_INTC_ICR(i) & 7) == priority)
				CLEAR_BIT_NO(level, (MCF_INTC_ICR(i) >> 3) & 7);
		}
	}

	for (i = 0; i <= 7; i++)
		if (level & (1 << i))
			break;

	if (i > 7)
		return -1;

	/*
	 * Make sure priority level is high, before changing registers
	 */
	__asm__ volatile (
			"move.w sr,d0\n\t"
			"move.w d0,srsave \n\t"
			"move.w #0x2700,sr\n\t"
			"	.data\n\t"
			"srsave:	ds.w	1\n\t"
			"	.text\n\t"
			:
			:
			: "d0","memory"
	);

	if (source < 32)
		CLEAR_BIT(MCF_INTC_IMRL, (1 << source));
	else
		CLEAR_BIT(MCF_INTC_IMRH, (1 << (source - 32)));

	MCF_INTC_ICR(source) = MCF_INTC_ICR_IP(priority) | MCF_INTC_ICR_IL(i);

	adr[64 + source] = (uint32_t *) func;	/* first 64 vectors are system exceptions */

	/*
	 * Return the saved priority level
	 */
	__asm__ volatile (
			"move.w srsave,d2\n\t"
			"move.w d2,sr\n\t"
			:
			:
			: "d2","memory"
	);

	return 0;
}
