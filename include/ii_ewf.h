//--------------------------------------------------------------------
// extension word format missing
//--------------------------------------------------------------------
.text
ii_ewf_lset:.macro
// pea
	ii_lset_opeag	48,7
	ii_lset	0x487b 
// jmp
	ii_lset_opeag	4e,f
	ii_lset	0x4efb 
// jsr
	ii_lset_opeag	4e,b
	ii_lset	0x4ebb 
// tas
	ii_lset_opeag	4a,f
	ii_lset	0x4ebb 
// tst.b
	ii_lset_opeag	4a,3
	ii_lset	0x4ebb 
// tst.w
	ii_lset_opeag	4a,7
	ii_lset	0x4ebb 
// tst.l
	ii_lset_opeag	4a,b
	ii_lset	0x4ebb 
// clr.b
	ii_lset_opeag	42,3
	ii_lset	0x423b 
// clr.w
	ii_lset_opeag	42,7
	ii_lset	0x423b 
// clr.l
	ii_lset_opeag	42,b
	ii_lset	0x423b 
.endm
//---------------------------------------------------------------------------------------------
ii_ewf_func:.macro
	ewf_func_macro	pea,487
	ewf_func_macro	jmp,4ef
	ewf_func_macro	jsr,4eb
	ewf_func_macro	tas,4af
	ewf_func_macro	tstb,4a3
	ewf_func_macro	tstw,4a7
	ewf_func_macro	tstl,4ab
	ewf_func_macro	clrb,423
	ewf_func_macro	clrw,427
	ewf_func_macro	clrl,42b
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

jmp_macro:.macro
	jsr		ewf
	move.l	a1,a0
	ii_end
.endm

jsr_macro:.macro
	jsr		ewf
	move.l	a1,d0
	move.l	usp,a1
	move.l	a0,-(a1)
	move.l	a1,usp
	move.l	d0,a0
	ii_end
.endm
	
tas_macro:.macro
	jsr		ewf
	tas		(a1)
	set_cc0
	ii_end
.endm
	
tstb_macro:.macro
	jsr		ewf
	tst.b	(a1)
	set_cc0
	ii_end
.endm
	
tstw_macro:.macro
	jsr		ewf
	tst.w	(a1)
	set_cc0
	ii_end
.endm
	
tstl_macro:.macro
	jsr		ewf
	tst.l	(a1)
	set_cc0
	ii_end
.endm
	
clrb_macro:.macro
	jsr		ewf
	clr.b	(a1)
	set_cc0
	ii_end
.endm
	
clrw_macro:.macro
	jsr		ewf
	clr.w	(a1)
	set_cc0
	ii_end
.endm
	
clrl_macro:.macro
	jsr		ewf
	clr.l	(a1)
	set_cc0
	ii_end
.endm
//--------------------------------------------------------------------
ewf_func_macro:.macro	//1=art 2=code
ii_0x\20:
#ifdef	halten_ewf
	halt
#endif
	move.l	a0_off(a7),a1
	\1_macro
ii_0x\21:
#ifdef	halten_ewf
	halt
#endif
	move.l	a1_off(a7),a1
	\1_macro
ii_0x\22:
#ifdef	halten_ewf
	halt
#endif
	move.l	a2,a1
	\1_macro
ii_0x\23:
#ifdef	halten_ewf
	halt
#endif
	move.l	a3,a1
	\1_macro
ii_0x\24:
#ifdef	halten_ewf
	halt
#endif
	move.l	a4,a1
	\1_macro
ii_0x\25:
#ifdef	halten_ewf
	halt
#endif
	move.l	a5,a1
	\1_macro
ii_0x\26:
#ifdef	halten_ewf
	halt
#endif
	move.l	a6,a1
	\1_macro
ii_0x\27:
#ifdef	halten_ewf
	halt
#endif
	move.l	usp,a1
	\1_macro
ii_0x\2b:
#ifdef	halten_ewf
	halt
#endif
	move.l	a0,a1
	\1_macro
.endm		

