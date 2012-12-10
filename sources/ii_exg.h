//--------------------------------------------------------------------
// exg
//--------------------------------------------------------------------
.text
ii_exg_lset:.macro 
/*	ii_lset_dxu	c,40	//dx,d0
	ii_lset_dxu	c,41	//dx,d1
	ii_lset_dxu	c,42	//dx,d2
	ii_lset_dxu	c,43	//dx,d3
	ii_lset_dxu	c,44	//dx,d4
	ii_lset_dxu	c,45	//dx,d5
	ii_lset_dxu	c,46	//dx,d6
	ii_lset_dxu	c,47	//dx,d7
 	ii_lset_dxu	c,48	//ax,a0
	ii_lset_dxu	c,49	//ax,a1
	ii_lset_dxu	c,4a	//ax,a2
	ii_lset_dxu	c,4b	//ax,a3
	ii_lset_dxu	c,4c	//ax,a4
	ii_lset_dxu	c,4d	//ax,a5
	ii_lset_dxu	c,4e	//ax,a6
	ii_lset_dxu	c,4f	//ax,a7 */ -->setting by "and"
 	ii_lset_dxu	c,88	//dx,a0
	ii_lset_dxu	c,89	//dx,a1
	ii_lset_dxu	c,8a	//dx,a2
	ii_lset_dxu	c,8b	//dx,a3
	ii_lset_dxu	c,8c	//dx,a4
	ii_lset_dxu	c,8d	//dx,a5
	ii_lset_dxu	c,8e	//dx,a6
	ii_lset_dxu	c,8f	//dx,a7  
.endm
//---------------------------------------------------------------------------------------------
ii_exg_func:.macro
// exg dx,dy
	ii_exg_dx_dx 14,d0_off(a7)
	ii_exg_dx_dx 34,d1_off(a7)
	ii_exg_dx_dx 54,d2
	ii_exg_dx_dx 74,d3
	ii_exg_dx_dx 94,d4
	ii_exg_dx_dx b4,d5
	ii_exg_dx_dx d4,d6
	ii_exg_dx_dx f4,d7
// exg ax,ay
	ii_exg_to_ax 14,a0_off(a7)
	ii_exg_to_ax 34,a1_off(a7)
	ii_exg_to_ax 54,a2
	ii_exg_to_ax 74,a3
	ii_exg_to_ax 94,a4
	ii_exg_to_ax b4,a5
	ii_exg_to_ax d4,a6
	ii_exg_to_ax f4,usp
// exg dx,ay
	ii_exg_to_ax 18,d0_off(a7)
	ii_exg_to_ax 38,d1_off(a7)
	ii_exg_to_ax 58,d2
	ii_exg_to_ax 78,d3
	ii_exg_to_ax 98,d4
	ii_exg_to_ax b8,d5
	ii_exg_to_ax d8,d6
	ii_exg_to_ax f8,d7
.endm		
//---------------------------------------------------------------------------------------------
exg_macro:.macro
#ifdef	halten_exg
	halt
#endif
	move.l	\1,a1
.ifc	\2,usp
	move.l	a1,d0
	move.l	\2,a1
	move.l	a1,\1	
	move.l	d0,a1
.else
	.ifc	\1,usp
		move.l	a1,d0
		move.l	\2,a1
		move.l	a1,\1	
		move.l	d0,a1
	.else
		move.l	\2,\1
	.endif
.endif
	move.l	a1,\2
	ii_end
.endm
ii_exg_dx_dx:.macro
ii_0xc\10:
	exg_macro	\2,d0_off(a7)
ii_0xc\11:
	exg_macro	\2,d1_off(a7)
ii_0xc\12:
	exg_macro	\2,d2
ii_0xc\13:
	exg_macro	\2,d3
ii_0xc\14:
	exg_macro	\2,d4
ii_0xc\15:
	exg_macro	\2,d5
ii_0xc\16:
	exg_macro	\2,d6
ii_0xc\17:
	exg_macro	\2,d7
.endm
ii_exg_to_ax:.macro
ii_0xc\18:
	exg_macro	\2,a0_off(a7)
ii_0xc\19:
	exg_macro	\2,a1_off(a7)
ii_0xc\1a:
	exg_macro	\2,a2
ii_0xc\1b:
	exg_macro	\2,a3
ii_0xc\1c:
	exg_macro	\2,a4
ii_0xc\1d:
	exg_macro	\2,a5
ii_0xc\1e:
	exg_macro	\2,a6
ii_0xc\1f:
	exg_macro	\2,usp
.endm