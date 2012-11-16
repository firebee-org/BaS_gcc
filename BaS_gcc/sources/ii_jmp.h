//--------------------------------------------------------------------
// extension word format missing
//--------------------------------------------------------------------
ii_\1_func:.macro
ii_0x\20:
#ifdef	halten_\1
	halt
#endif
	move.l	a0_off(a7),a1
	\1_macro
ii_0x\21:
#ifdef	halten_\1
	halt
#endif
	move.l	a1_off(a7),a1
	\1_macro
ii_0x\22:
#ifdef	halten_\1
	halt
#endif
	move.l	a2,a1
	\1_macro
ii_0x\23:
#ifdef	halten_\1
	halt
#endif
	move.l	a3,a1
	\1_macro
ii_0x\24:
#ifdef	halten_\1
	halt
#endif
	move.l	a4,a1
	\1_macro
ii_0x\25:
#ifdef	halten_\1
	halt
#endif
	move.l	a5,a1
	\1_macro
ii_0x\26:
#ifdef	halten_\1
	halt
#endif
	move.l	a6,a1
	\1_macro
ii_0x\27:
#ifdef	halten_\1
	halt
#endif
	move.l	usp,a1
	\1_macro
ii_0x\2b:
#ifdef	halten_\1
	halt
#endif
	move.l	a0,a1
	\1_macro
.endm		
