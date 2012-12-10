/*****************************************************************************************/
// opertionen
/*****************************************************************************************/
ii_lset_shd:.macro
	ii_lset_shdx	e0			//r d0
	ii_lset_shdx	e2			//r d1
	ii_lset_shdx	e4			//r d2
	ii_lset_shdx	e6			//r d3
	ii_lset_shdx	e8			//r d4
	ii_lset_shdx	ea			//r d5
	ii_lset_shdx	ec			//r d6
	ii_lset_shdx	ee			//r d7
	
	ii_lset_shdx	e1			//l d0
	ii_lset_shdx	e3			//l d1
	ii_lset_shdx	e4			//l d2
	ii_lset_shdx	e5			//l d3
	ii_lset_shdx	e9			//l d4
	ii_lset_shdx	eb			//l d5
	ii_lset_shdx	ed			//l d6
	ii_lset_shdx	ef			//l d7
	.endm
	
ii_lset_shdx:.macro
//byt
	ii_lset_opea	\1,0			// as,ls #im,dx
	ii_lset_opea	\1,1			// rox,ro #im,dx
	ii_lset_opea	\1,2			// as,ls dy,dx
	ii_lset_opea	\1,3			// rox,ro dy,dx
//word
	ii_lset_opea	\1,4			// as,ls #im,dx
	ii_lset_opea	\1,5			// rox,ro #im,dx
	ii_lset_opea	\1,6			// as,ls dy,dx
	ii_lset_opea	\1,7			// rox,ro dy,dx
//long
//	ii_lset_opea	\1,8			// as,ls #im,dx -> vorhanden
	ii_lset_opea	\1,9			// rox,ro #im,dx
//	ii_lset_opea	\1,a			// as,ls dy,dx	-> vorhanden
	ii_lset_opea	\1,b			// rox,ro dy,dx
	.endm
/******************************************************/
ii_shd:.macro	// 1=code 2=operation 3 = normal, direct oder immediat
// byt
	opdx	\1,\2,b,0,\3		// dx
// word
	opdx	\1,\2,w,4,\3		// dx
// long
	opdx	\1,\2,l,8,\3		// dx
	.endm
/******************************************************/
// byt word long routinen
/******************************************************/
sh_asr:	.macro		// asr -> 1=operation 2 = dx 3 = dy/im 4 = size b/w
	mvs.\4		\2,d1
	sh_shal		\1,\2,\3,\4
	.endm	

sh_lsr:	.macro		// asl -> 1=operation 2 = dx 3 = dy/im 4 = size b/w
	mvz.\4		\2,d1
	sh_shal		\1,\2,\3,\4
	.endm
	
sh_shal:.macro
	move.w		\3,d0
	\1.l		d0,d1
	set_cc0
	move.\4		d1,\2
	.endm	
	
sh_all:	.macro		// asl/lsl -> 1=operation 2 = dx 3 = dy/im 4 = size b/w
	mvz.\4		\2,d1
.ifc	\4,b
	byterev.l	d1
.else
	swap.w		d1
.endif
	sh_asr		\1,\2,\3,\4
	.endm	
	
sh_ror:	.macro		// ror -> 1=operation 2 = dx 3 = dy/im 4 = size b/w/l
	move.\4	\2,d1	/
	move.w	\3,d0
.ifc	\4,b
	lsl.l	#8,d1
	move.b	\2,d1
	and.l	#0x7,d0
	lsr.l	d0,d1
.else
	.ifc	\4,w
		swap.w	d1
		move.w	\2,d1
		and.l	#0xf,d0
		lsr.l	d0,d1
	.else
		and.l	#0x1f,d0
		lsr.l	d0,d1
		move.l	d1,a1
		move.l	\2,d1
		sub.l	#32,d0
		neg.l	d0
		lsl.l	d0,d1
		add.l	a1,d1
	.endif
.endif
	move.\4	d1,\2
	move.w	ccr,d0
	and.l	#1,d1	// ist auch carry bit
	or.l	d1,d0	
	move.b	d0,ccr_off(a7)
	.endm
	
sh_rol:	.macro		// rol -> 1=operation 2 = dx 3 = dy/im 4 = size b/w/l
	move.\4	\2,d1	
	move.w	\3,d0
.ifc	\4,b
	lsl.l	#8,d1
	move.b	\2,d1
	and.l	#0x7,d0
	lsl.l	d0,d1
	lsr.l	#8,d1
	moveq	#7,d0
.else
	.ifc	\4,w
		swap.w	d1
		move.w	\2,d1
		and.l	#0xf,d0
		lsr.l	d0,d1
		swap.w	d1
		moveq	#15,d0
	.else
		and.l	#0x1f,d0
		lsl.l	d0,d1
		move.l	d1,a1
		move.l	\2,d1
		sub.l	#32,d0
		neg.l	d0
		lsr.l	d0,d1
		add.l	a1,d1
		moveq	#31,d0
	.endif
.endif
	move.\4	d1,\2
	lsr.l	d0,d1		// carry bit schieben
	move.w	ccr,d0
	and.l	#1,d1
	or.l	d1,d0	
	move.b	d0,ccr_off(a7)
	.endm
	
sh_roxr:	.macro		// roxr -> 1=operation 2 = dx 3 = dy/im 4 = size b/w/l
	clr.l	d0
	addx.l	d0,d0
ifc	\4,b
	mvz.b	\2,d1
	lsl.l	#1,d1
	add.l	d0,d1
	lsl.l	#8,d1
	move.b	\2,d1
	move.w	\3,d0
	and.l	#0x7,d0
	lsr.l	d0,d1
	set_cc0
else
	.ifc	\4,w
		mvz.b	\2,d1
		lsl.l	#1,d1
		add.l	d0,d1
		lsl.l	#8,d1
		lsl.l	#8,d1
		move.w	\2,d1
		move.w	\3,d0
		and.l	#0xf,d0
		lsr.l	d0,d1
		set_cc0
	.else
		bitrev.l d0
		move.l	\2,d1
		lsr.l	#1,d1
		add.l	d0,d1
		move.w	\3,d0
		subq.l	#1,d0
		and.l	#0x1f,d0
		lsr.l	d0,d1
		move.l	d1,a1
		set_cc1
		move.l	\2,d1
		sub.l	#32,d0
		neg.l	d0
		lsl.l	d0,d1
		add.l	a1,d1
	.endif
.endif
	move.\4	d1,\2
	.endm
	
sh_roxl:	.macro		// roxl -> 1=operation 2 = dx 3 = dy/im 4 = size b/w/l
	clr.l	d0
	addx.l	d0,d0
ifc	\4,b
	mvz.b	\2,d1
	lsl.l	#1,d1
	add.l	d0,d1
	lsl.l	#8,d1
	move.b	\2,d1
	lsl.l	#8,d1
	lsl.l	#7,d1
	move.w	\3,d0
	and.l	#0x7,d0
	lsl.l	d0,d1
	set_cc0
	byterev.l d1
else
	.ifc	\4,w
		mvz.b	\2,d1
		lsl.l	#1,d1
		add.l	d0,d1
		lsl.l	#8,d1
		lsl.l	#7,d1
		mvz.w	\2,d0
		lsr.l	#1,d0
		add.l	d0,d1
		move.w	\3,d0
		and.l	#0xf,d0
		lsl.l	d0,d1
		set_cc0
		swap.w	d1
	.else
		move.l	\2,d1
		lsl.l	#1,d1
		add.l	d0,d1
		move.w	\3,d0
		subq.l	#1,d0
		and.l	#0x1f,d0
		lsl.l	d0,d1
		move.l	d1,a1
		set_cc1
		move.l	\2,d1
		sub.l	#32,d0
		neg.l	d0
		lsr.l	d0,d1
		add.l	a1,d1
	.endif
.endif
	move.\4	d1,\2
	.endm
	
	