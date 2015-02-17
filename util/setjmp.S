		.globl		_setjmp
		.globl		_longjmp

_setjmp:	move.l		4(sp),a0		// address of jmp_buf[]
		move.l		(sp),(a0)		// save return address
		movem.l		d2-d7/a2-a7,4(a0)	// save registers to jmp_buf
		clr.l		d0
		rts

_longjmp:	move.l		4(sp),a0		// address of jmp_buf[]
		move.l		8(sp),d0		// value to return
		jne		not_0			// value may not be 0
		moveq.l		#1,d0
not_0:		movem.l		4(a0),d2-d7/a2-a7	// restore registers
		move.l		(a0),(sp)		// restore saved return address
		rts
