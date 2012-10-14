#include <MCF5475.h>

void startup(void)
{
	asm("\n\t"
	".extern	_initialize_hardware\n\t"
	"bra.s	warmstart\n\t"
	"jmp	___BOOT_FLASH + 8 | ist zugleich reset vector\n\t"
	"| disable interrupts\n\t"
"warmstart:\n\t"
	"| disable interrupts\n\t"
	"move.w #0x2700,sr\n\t"
	);

	/* Initialize MBAR */
	asm("MOVE.L	#__MBAR,D0\n\t");
	asm("MOVEC	D0,MBAR\n\t");
	asm("MOVE.L	D0,_rt_mbar\n\t");

	/* mmu off */
	asm("move.l	#__MMUBAR+1,d0\n\t");
	asm("movec	d0,MMUBAR");	/* set mmubar */

	* (volatile uint32_t *) MCF_MMU_MMUCR = 0L;	/* MMU off */

	asm(
    "|/* Initialize RAMBARs: locate SRAM and validate it */\n\t"
   	"move.l	#__RAMBAR0 + 0x7,d0\n\t | supervisor only"
   	"movec  d0,RAMBAR0\n\t"
   	"move.l #__RAMBAR1 + 0x1,d0\n\t"""
   	"movec  d0,RAMBAR1\n\t"
	"| STACKPOINTER AUF ENDE SRAM1\n\t"
	"lea 	__SUP_SP,a7\n\t"
	"| instruction cache on\n\t"
	"move.l	#0x000C8100,d0\n\t"
 	"movec	d0,cacr\n\t"
 	"nop\n\t"
	"| initialize any hardware specific issues\n\t"
	"bra    _initialize_hardware\n\t"
	);
}
