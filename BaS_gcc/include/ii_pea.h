//--------------------------------------------------------------------
// pea
//--------------------------------------------------------------------
.text
ii_pea_lset:.macro
	ii_lset_opeag	48,7
	ii_lset	0x487b 
.endm
//---------------------------------------------------------------------------------------------
ii_pea_func:.macro
ii_0x4870:
#ifdef	halten_pea
	halt
#endif
	move.l	a0_off(a7),a1
	pea_macro
ii_0x4871:
#ifdef	halten_pea
	halt
#endif
	move.l	a1_off(a7),a1
	pea_macro
ii_0x4872:
#ifdef	halten_pea
	halt
#endif
	move.l	a2,a1
	pea_macro
ii_0x4873:
#ifdef	halten_pea
	halt
#endif
	move.l	a3,a1
	pea_macro
ii_0x4874:
#ifdef	halten_pea
	halt
#endif
	move.l	a4,a1
	pea_macro
ii_0x4875:
#ifdef	halten_pea
	halt
#endif
	move.l	a5,a1
	pea_macro
ii_0x4876:
#ifdef	halten_pea
	halt
#endif
	move.l	a6,a1
	pea_macro
ii_0x4877:
#ifdef	halten_pea
	halt
#endif
	move.l	usp,a1
	pea_macro
ii_0x487b:
#ifdef	halten_pea
	halt
#endif
	move.l	a0,a1
	pea_macro
.endm		
//---------------------------------------------------------------------------------------------
pea_macro:.macro
	jsr		ewf
	move.l	(a1),d0
	move.l	usp,a1
	move.l	d0,-(a1)
	move.l	a1,usp
	ii_end
.endm