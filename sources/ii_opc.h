/*****************************************************************************************/
// functionen macros: fehlende adressierungsarte (MCF nur Dx support) ohne ax
// zusammen mit op.h
/*****************************************************************************************/
ii_lset_opc:.macro
	ii_lset_opeag	\1,c			// dx,ax
	ii_lset_opea	\1,d			// (ax), (ax)+
	ii_lset_opea	\1,e			// -(ax),d16(ax)
	ii_lset_opeag	\1,f			// d8(ax,dy)
	lea	table+0x\1b8*4,a0
	move.l	#ii_0x\1b8,(a0)+		// xxx.w
	move.l	#ii_0x\1b9,(a0)+		// xxx.l
	.endm

/******************************************************/
ii_opc:.macro	// 1=code 2=operation 3 = normal oder immediat
	opcdx	\1,\2,l,c,\3		// dx,ax
	opia	\1,\2,l,d,\3		// (ax),(ax)+
	opdia	\1,\2,l,e,\3		// -(ax),d16(ax)
	opd8a	\1,\2,l,f,\3		// d8(ax),xxx
	.endm
//*******************************************************************************3
/******************************************************/
// byt word long
/******************************************************/
opcdx:	.macro	//register: \1=code \2 = operation \3 = size \4=size and adressierungsart 5 = immediate oder normal
ii_0x\1\40:
#ifdef	halten_opc
	halt
#endif
	.ifc	\3,b
		op\5smd	\2,d0_off+3(a7),d0_off+3(a7),\3
	.else
		.ifc		\3,w
			op\5smd	\2,d0_off+2(a7),d0_off+2(a7),\3
		.else
			op\5smd	\2,d0_off(a7),d0_off(a7),\3
		.endif
	.endif
ii_0x\1\41:
	.ifc	\3,b
		op\5smd	\2,d1_off+3(a7),d1_off+3(a7),\3
	.else
		.ifc		\3,w
			op\5smd	\2,d1_off+2(a7),d1_off+2(a7),\3
		.else
			op\5smd	\2,d1_off(a7),d1_off(a7),\3
		.endif
.endif
ii_0x\1\42:
	op\5smd	\2,d2,d2,\3
ii_0x\1\43:
	op\5smd	\2,d3,d3,\3
ii_0x\1\44:
	op\5smd	\2,d4,d4,\3
ii_0x\1\45:
	op\5smd	\2,d5,d5,\3
ii_0x\1\46:
	op\5smd	\2,d6,d6,\3
ii_0x\1\47:
	op\5smd	\2,d7,d7,\3
.endm
//-----------------------------------------------------
opcsmd:.macro		// dx: 1=opieration	2=ea src 3=ea dest 4=size
#ifdef	halten_opc
	halt
#endif

.ifc	\4,l
	move.l	(a0)+,d0
.else
	.ifc	\4,w
		mvs.w	(a0)+,d0
	.else
		move.w	(a0)+,d0
		extb.l	d0
	.endif
.endif
.ifc	\4,l
	move.l	\2,d1
.else
	mvs.\4	\2,d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d1
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	d1
	set_cc0
	move.\4		d1,\3
	ii_end
	.endm;

opacsmd:.macro		// ax: 1=opieration	2=ea src 3=ea dest 4=size
#ifdef	halten_opc
	halt
#endif

.ifc	\4,l
	move.l	(a0)+,d0
.else
	.ifc	\4,w
		mvs.w	(a0)+,d0
	.else
		move.w	(a0)+,d0
		extb.l	d0
	.endif
.endif
.ifc	\2,usp
		move.l	usp,a1
		move.l	a1,d1
.else
		move.l		\2,d1
.endif
	\1	d1
	set_cc0
.ifc	\3,usp
	move.l		d1,a1
	move.l		a1,usp
.else
	move.l		d1,\3
.endif
	ii_end
	.endm;

opcsia:.macro		// (ax) (ax)+ -(ax): 1=opieration 2=adress register 3= src 4=dest 5=size
#ifdef	halten_opc
	halt
#endif

.ifc	\4,l
	move.l	(a0)+,d0
.else
	.ifc	\4,w
		mvs.w	(a0)+,d0
	.else
		move.w	(a0)+,d0
		extb.l	d0
	.endif
.endif
	move.l	\2,a1
.ifc	\5,l
	move.l	\3,d1
.else
	mvs.\5	\3,d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d1
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	d1
	set_cc0
	move.\5	d1,\4
	ii_end
	.endm;

opcsd16a:.macro		// d16(ax): 1=opieration 2=adress register 3=size
#ifdef	halten_opc
	halt
#endif

.ifc	\4,l
	move.l	(a0)+,d0
.else
	.ifc	\4,w
		mvs.w	(a0)+,d0
	.else
		move.w	(a0)+,d0
		extb.l	d0
	.endif
.endif
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
.ifc	\3,l
	move.l	(a1),d1
.else
	mvs.\3	(a1),d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d1
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	d1
	set_cc0
	move.\3	d1,(a1)
	ii_end
	.endm;

opcsd8a:.macro		// indirect: 1=opieration 2=adress register 3=size
#ifdef	halten_opc
	halt
#endif

.ifc	\4,l
	move.l	(a0)+,d0
.else
	.ifc	\4,w
		mvs.w	(a0)+,d0
	.else
		move.w	(a0)+,d0
		extb.l	d0
	.endif
.endif
	move.l	d0,_d0_save
	move.l	\2,a1
	jsr		ewf
	move.l	_d0_save,d0
.ifc	\3,l
	move.l	(a1),d1
.else
	mvs.\3	(a1),d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d1
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	d1
	set_cc0
	move.\3	d1,(a1)
	ii_end
	.endm;

opcsxx:.macro		// indirect: 1=opieration 2=size 3=size adresse
#ifdef	halten_opc
	halt
#endif

.ifc	\2,l
	move.l	(a0)+,d0
.else
	.ifc	\2,w
		mvs.w	(a0)+,d0
	.else
		move.w	(a0)+,d0
		extb.l	d0
	.endif
.endif
	move.\3	(a0)+,a1
.ifc	\2,l
	move.l	(a1),d1
.else
	mvs.\2	(a1),d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d1
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	d1
	set_cc0
	move.\2	d1,(a1)
	ii_end
	.endm;
