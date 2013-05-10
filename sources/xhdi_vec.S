//
// XHDI entry point
//
		.extern	_xhdi_call
		.globl	_xhdi_vec

_xhdi_vec:
		lea     -12(sp),sp
		movem.l d1/a0-a1,(sp)
		pea     16(sp)
		jsr		_xhdi_call
		addq.l  #4,sp
		movem.l (sp),d1/a0-a1
		lea     12(sp),sp
		rts
