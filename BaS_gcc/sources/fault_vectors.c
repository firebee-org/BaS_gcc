#include "MCF5475.h"
#include "bas_types.h"
#include "bas_printf.h"

/*
 * provide an early exception vector branch table to catch exceptions _before_ VBR has been setup eventually
 * (to RAMBAR0, in exceptions.S)
 */

typedef void (*exception_handler)(void);
extern exception_handler SDRAM_VECTOR_TABLE[];

void fault_handler(uint32_t format_status, uint32_t pc)
{
	xprintf("\007\007exception!\r\n");
	xprintf("format_status: %lx\r\n", format_status);
	xprintf("pc: %lx\r\n", pc);
	xprintf("processor halted.\r\n");
}

void handler(void)
{
	/*
	 * for standard routines, we'd have to save registers here.
	 * Since we do not intend to return anyway, we just ignore that
	 */
	__asm__ __volatile__("move.l	(sp),-(sp)\n\t"		/* format, fault status and status register values */
						 "move.l	8(sp),-(sp)\n\t"	/* the program counter where the fault originated */
						 "bra		_fault_handler\n\t"
						 "halt\n\t"
							: : :);
}

void setup_vectors(void)
{
	int i;

	for (i = 0; i < 256; i++)
	{
		SDRAM_VECTOR_TABLE[i] = handler;
	}

	/*
	 * make sure VBR points to our table
	 */
	__asm__ __volatile__("clr.l		d0\n\t"
						 "movec		d0,VBR\n\t"
						 "move.l	d0,_rt_vbr");
}
