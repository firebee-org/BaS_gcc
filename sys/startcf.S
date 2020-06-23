
//
// This object file must be the first to be linked,
// so it will be placed at the very beginning of the ROM.
//

	.equ	MCF_MMU_MMUCR, __MMUBAR + 0

	.globl	_rom_header
	.globl	_rom_entry

	.extern	_initialize_hardware
	.extern	_rt_mbar

/* ROM header */
_rom_header:
	//
	// The first long is supposed to be the initial SP.
	// We replace it by bra.s to allow running the ROM from the first byte.
	// Then we add a fake jmp instruction for pretty disassembly.
	//
	bra.s	_rom_entry			// Short jump to the real entry point
	.short	0x4ef9				// Fake jmp instruction
	// The second long is the initial PC
	.long	_rom_entry			// Real entry point

/* ROM entry point */
_rom_entry:
	// disable interrupts
	move.w #0x2700,sr

#if !defined(MACHINE_M54455)			// MCF54455 does not have the MBAR register
	/* Initialize MBAR */
	move.l	#__MBAR,d0
	movec	d0,MBAR
	move.l	d0,_rt_mbar
#endif

	/* mmu off */
	move.l	#__MMUBAR+1,d0
	movec	d0,MMUBAR

	clr.l	d0
	move.l	d0,MCF_MMU_MMUCR
	nop

#if !defined(MACHINE_M54455)			// MCF54455 does not have RAMBAR0 and RAMBAR1 registers */

	// Initialize RAMBARs: locate SRAM and validate it
	move.l	#__RAMBAR0 + 0x7,d0		// supervisor only
	movec	d0,RAMBAR0
	move.l	#__RAMBAR1 + 0x1,d0
	movec	d0,RAMBAR1
#else
	move.l	#__RAMBAR0 + 0x7,d0
	movec	d0,RAMBAR
#endif

	// set stack pointer to end of SRAM
	lea 	__SUP_SP,a7
	move.l	#0,(sp)

	// Initialize the processor caches.
	// The instruction cache is fully enabled.
	// The data cache is enabled, but cache-inhibited by default.
	// Later, the MMU will fully activate the data cache for specific areas.
	// It is important to enable both caches now, otherwise cpushl would hang.

	move.l	#0xa50c8120,d0
	movec	d0,cacr
	andi.l	#0xfefbfeff,d0			// Clear invalidate bits
	move.l	d0,_rt_cacr

	// early initialization of any hardware needed before BaS() execution
	bra	_initialize_hardware

