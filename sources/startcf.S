#include <MCF5475.h>

_startup:
		.extern	_initialize_hardware
		.extern _rt_mbar

		bra.s	warmstart
		jmp		___BOOT_FLASH + 8 		/* that's also our reset vector */

		/* disable interrupts */
warmstart:
		move.w #0x2700,sr

		/* Initialize MBAR */

		move.l	#__MBAR,d0
		movec	d0,MBAR
		move.l	d0,_rt_mbar

		/* mmu off */
		move.l	#__MMUBAR+1,d0
		movec	d0,MMUBAR

		clr.l	d0
		move.l	d0,MCF_MMU_MMUCR

		/* Initialize RAMBARs: locate SRAM and validate it */
		move.l	#__RAMBAR0 + 0x7,d0 	/* supervisor only */
		movec	d0,RAMBAR0
		move.l	#__RAMBAR1 + 0x1,d0
		movec	d0,RAMBAR1

		/* set stack pointer to end of SRAM1 */
		lea 	__SUP_SP,a7

		/* instruction cache on */
		move.l	#0x000C8100,d0
		movec	d0,cacr
		nop

		/* initialize any hardware specific issues */
		bra    _initialize_hardware
