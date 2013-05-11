//
// XHDI entry point
//
		.extern	_xhdi_call

		.globl	_xhdi_vec
		.globl	_xhdi_sd_install

//
// this is where the XHDI cookie points to:
//

		.text
_xhdi_vec:
		lea     -12(sp),sp		// save all used registers according to XHDI spec
		movem.l d1/a0-a1,(sp)

		pea     16(sp)			// forward address of parameters on stack
		jsr		_xhdi_call		// to internal routine
		addq.l  #4,sp			// correct stack

		movem.l (sp),d1/a0-a1	// restore registers
		lea     12(sp),sp
		rts

		.data
_old_vector:
		.ds.l	1

		.text

//
// trap #0 handler to bring the address of the disk routines into TOS
//
_xhdi_sd_install:
		move.l	4(sp),d0		// address of the old XHDI vector
		move.l	d0,_old_vector	// save it - just in case we need it later
		move.l	#_xhdi_vec,d0	// return our BaS vector to TOS
		move.l	d0,a0			//
		rte
