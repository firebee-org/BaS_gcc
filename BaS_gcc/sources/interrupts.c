/*
 * Interrupts
 *
 * Handle interrupts, the levels.
 *
 */

#include <stdint.h>
#include "bas_utils.h"
#include "interrupts.h"
#include "MCF5475.h"

extern uint8_t _rtl_vbr[];
#define VBR	((uint32_t **) &_rtl_vbr[0])

/*
 * register an interrupt handler at the Coldfire interrupt controller and add the handler to the interrupt vector table
 */
int register_handler(uint8_t priority, uint8_t intr, void (*func)())
{
	int i;
	uint8_t level = 0b01111111;
	uint32_t **adr = VBR;

	intr &= 63;
	priority &= 7;

	if (intr <= 0)
		return -1;

	for (i = 1; i < 64; i++)
		if (i != intr)
		{
			if ((MCF_INTC_ICR(i) & 7) == priority)
				CLEAR_BIT_NO(level, (MCF_INTC_ICR(i) >> 3) & 7);
		}

	for (i = 0; 1 < 7; i++)
		if (level & (1 << i))
			break;

	if (i >= 7)
		return -1;

	/*
	 * Make sure priority level is high, before changing registers
	 */
	__asm__ volatile (
			"move.w sr,d0\n\t"
			"move.w d0,-(sp) \n\t"
			"move.w #0x2700,sr\n\t"
			:
			:
			: "sp","d0","memory"
	);

	if (intr < 32)
		CLEAR_BIT(MCF_INTC_IMRL, (1 << intr));
	else
		CLEAR_BIT(MCF_INTC_IMRH, (1 << (intr - 32)));

	MCF_INTC_ICR(intr) = MCF_INTC_ICR_IP(priority) | MCF_INTC_ICR_IL(i);

	adr[64 + intr] = (uint32_t *) func;	/* first 64 vectors are system exceptions */

	/*
	 * Return the saved priority level
	 */
	__asm__ volatile (
			"move.w (sp)+,d2\n\t"
			"move.w d2,sr\n\t"
			:
			:
			: "sp","d2","memory"
	);

	return 0;
}
