/*****************************************************************************************/
// opertionen
/*****************************************************************************************/
ii_lset_op:.macro
//byt
	ii_lset_opea	\1,0			// dx,ax
	ii_lset_opea	\1,1			// (ax), (ax)+
	ii_lset_opea	\1,2			// -(ax),d16(ax)
	ii_lset_opeag	\1,3			// d8(ax,dy)
	lea	table+0x\1\238*4,a0
	move.l	#ii_0x\138,(a0)+		// xxx.w
	move.l	#ii_0x\139,(a0)+		// xxx.l
//word
	ii_lset_opea	\1,4			// dx,ax
	ii_lset_opea	\1,5			// (ax), (ax)+
	ii_lset_opea	\1,6			// -(ax),d16(ax)
	ii_lset_opeag	\1,7			// d8(ax,dy)
	lea	table+0x\178*4,a0
	move.l	#ii_0x\178,(a0)+		// xxx.w
	move.l	#ii_0x\179,(a0)+		// xxx.l
//long
	ii_lset_opea	\1,8			// dx,ax
	ii_lset_opea	\1,9			// (ax), (ax)+
	ii_lset_opea	\1,a			// -(ax),d16(ax)
	ii_lset_opeag	\1,b			// d8(ax,dy)
	lea	table+0x\1b8*4,a0
	move.l	#ii_0x\1b8,(a0)+		// xxx.w
	move.l	#ii_0x\1b9,(a0)+		// xxx.l
	.endm

ii_lset_opeag:.macro				// 0x1120-0x1127 
	lea	table+0x\1\20*4,a0
	move.l	#ii_0x\1\20,(a0)+
	move.l	#ii_0x\1\21,(a0)+
	move.l	#ii_0x\1\22,(a0)+
	move.l	#ii_0x\1\23,(a0)+
	move.l	#ii_0x\1\24,(a0)+
	move.l	#ii_0x\1\25,(a0)+
	move.l	#ii_0x\1\26,(a0)+
	move.l	#ii_0x\1\27,(a0)+
	.endm;
	
ii_lset_opeau:.macro				// 0x1128-0x112f 
	lea	table+0x\1\28*4,a0
	move.l	#ii_0x\1\28,(a0)+
	move.l	#ii_0x\1\29,(a0)+
	move.l	#ii_0x\1\2a,(a0)+
	move.l	#ii_0x\1\2b,(a0)+
	move.l	#ii_0x\1\2c,(a0)+
	move.l	#ii_0x\1\2d,(a0)+
	move.l	#ii_0x\1\2e,(a0)+
	move.l	#ii_0x\1\2f,(a0)+
	.endm;

ii_lset_opea:.macro
	ii_lset_opeag	\1,\2
	ii_lset_opeau	\1,\2
	.endm
/******************************************************/
ii_op:.macro	// 1=code 2=operation 3 = normal oder immediat/quick
// byt
	opdx	\1,\2,b,0,\3		// dx,ax
	opia	\1,\2,b,1,\3		// (ax),(ax)+
	opdia	\1,\2,b,2,\3		// -(ax),d16(ax)
	opd8a	\1,\2,b,3,\3		// d8(ax),xxx
// word
	opdx	\1,\2,w,4,\3		// dx,ax
	opia	\1,\2,w,5,\3		// (ax),(ax)+
	opdia	\1,\2,w,6,\3		// -(ax),d16(ax)
	opd8a	\1,\2,w,7,\3		// d8(ax),xxx
// long
	opdx	\1,\2,l,8,\3		// dx,ax
	opia	\1,\2,l,9,\3		// (ax),(ax)+
	opdia	\1,\2,l,a,\3		// -(ax),d16(ax)
	opd8a	\1,\2,l,b,\3		// d8(ax),xxx
	.endm
/******************************************************/
// byt word long
/******************************************************/
opdx:	.macro	//register: \1=code \2 = operation \3 = size \4=size and adressierungsart 5 = immediate oder normal
ii_0x\1\40:
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
//ax
ii_0x\1\48:
	opa\5smd	\2,a0_off(a7),a0_off(a7),\3
ii_0x\1\49:
	opa\5smd	\2,a1_off(a7),a1_off(a7),\3
ii_0x\1\4a:
	opa\5smd	\2,a2,a2,\3
ii_0x\1\4b:
	opa\5smd	\2,a3,a3,\3
ii_0x\1\4c:
	opa\5smd	\2,a4,a4,\3
ii_0x\1\4d:
	opa\5smd	\2,a5,a5,\3
ii_0x\1\4e:
	opa\5smd	\2,a6,a6,\3
ii_0x\1\4f:
	opa\5smd	\2,usp,usp,\3
.endm;
//-----------------------------------------------
opia:	.macro	// (ax) \1=code \2 = operation \3 = size \4=size and adressierungsart 5 = immediate oder normal
//(ax)
ii_0x\1\40:
	op\5sia	\2,a0_off(a7),(a1),(a1),\3
ii_0x\1\41:
	op\5sia	\2,a1_off(a7),(a1),(a1),\3
ii_0x\1\42:
	op\5smd	\2,(a2),(a2),\3
ii_0x\1\43:
	op\5smd	\2,(a3),(a3),\3
ii_0x\1\44:
	op\5smd	\2,(a4),(a4),\3
ii_0x\1\45:
	op\5smd	\2,(a5),(a5),\3
ii_0x\1\46:
	op\5smd	\2,(a6),(a6),\3
ii_0x\1\47:
	op\5sia	\2,usp,(a1),(a1),\3
//(ax)+
ii_0x\1\48:
	op\5sia	\2,a0_off(a7),(a1),(a1)+,\3
ii_0x\1\49:
	op\5sia	\2,a1_off(a7),(a1),(a1)+,\3
ii_0x\1\4a:
	op\5smd	\2,(a2),(a2)+,\3
ii_0x\1\4b:
	op\5smd	\2,(a3),(a3)+,\3
ii_0x\1\4c:
	op\5smd	\2,(a4),(a4)+,\3
ii_0x\1\4d:
	op\5smd	\2,(a5),(a5)+,\3
ii_0x\1\4e:
	op\5smd	\2,(a6),(a6)+,\3
ii_0x\1\4f:
	op\5sia	\2,usp,(a1),(a1)+,\3
.endm;
//-----------------------------------------------
opdia:	.macro	// -(ax) \1=code \2 = operation \3 = size \4 size and adressierungsart 5 = immediate oder normal
ii_0x\1\40:
	op\5sia	\2,a0_off(a7),-(a1),(a1),\3
ii_0x\1\41:
	op\5sia	\2,a1_off(a7),-(a1),(a1),\3
ii_0x\1\42:
	op\5smd	\2,-(a2),(a2),\3
ii_0x\1\43:
	op\5smd	\2,-(a3),(a3),\3
ii_0x\1\44:
	op\5smd	\2,-(a4),(a4),\3
ii_0x\1\45:
	op\5smd	\2,-(a5),(a5),\3
ii_0x\1\46:
	op\5smd	\2,-(a6),(a6),\3
ii_0x\1\47:
	op\5sia	\2,usp,-(a1),(a1),\3

ii_0x\1\48:
	op\5sd16a	\2,a0_off(a7),\3
ii_0x\1\49:
	op\5sd16a	\2,a1_off(a7),\3
ii_0x\1\4a:
	op\5sd16a	\2,a2,\3
ii_0x\1\4b:
	op\5sd16a	\2,a3,\3
ii_0x\1\4c:
	op\5sd16a	\2,a4,\3
ii_0x\1\4d:
	op\5sd16a	\2,a5,\3
ii_0x\1\4e:
	op\5sd16a	\2,a6,\3
ii_0x\1\4f:
	op\5sd16a	\2,usp,\3
.endm;
//-----------------------------------------------
opd8a:	.macro	// d8(ax,dy) \1=code \2 = operation \3 = size \4=size and adressierungsart 5 = immediate oder normal
ii_0x\1\40:
	op\5sd8a	\2,a0_off(a7),\3
ii_0x\1\41:
	op\5sd8a	\2,a1_off(a7),\3
ii_0x\1\42:
	op\5sd8a	\2,a2,\3
ii_0x\1\43:
	op\5sd8a	\2,a3,\3
ii_0x\1\44:
	op\5sd8a	\2,a4,\3
ii_0x\1\45:
	op\5sd8a	\2,a5,\3
ii_0x\1\46:
	op\5sd8a	\2,a6,\3
ii_0x\1\47:
	op\5sd8a	\2,usp,\3

ii_0x\1\48:
	op\5sxx		\2,\3,w
ii_0x\1\49:
	op\5sxx		\2,\3,l
.endm;
//-----------------------------------------------
opnsmd:.macro		// direct dx: 1=operation	2=ea src 3=ea dest 4=size
#ifdef	halten_op
	halt
#endif
.ifc	\4,l
	move.l	\2,d1
.else
	mvs.\4	\2,d1
.endif
	.ifc	\1,negx
		move.b		sr_off+1(a7),d1			//ccr holen
		move		d1,ccr					//setzen
	.endif		
	\1	d1
	set_cc0
	move.\4		d1,\3
	ii_end
	.endm;

opansmd:.macro		// direct ax: 1=operation	2=ea src 3=ea dest 4=size
#ifdef	halten_op
	halt
#endif

.ifc	\2,usp
		move.l	usp,a1
		move.l	a1,d1
.else
		move.l		\2,d1
.endif
	\1	d1
.ifc	\3,usp
	move.l		d1,a1
	move.l		a1,usp
.else
	move.l		d1,\3
.endif
	ii_end
.endm;

opnsia:.macro		// indirect: 1=operation 2=adress register 3= src 4=dest 5=size
#ifdef	halten_op
	halt
#endif
	move.l	\2,a1
.ifc	\5,l
	move.l	\3,d1
.else
	mvs.\5	\3,d1
.endif
	.ifc	\1,negx
		move.b		sr_off+1(a7),d1			//ccr holen
		move		d1,ccr					//setzen
	.endif		
	\1	d1
	set_cc0
	move.\5	d1,\4
	ii_end
	.endm;

opnsd16a:.macro		// indirect: 1=operation 2=adress register 3=size
#ifdef	halten_op
	halt
#endif
	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
.ifc	\3,l
	move.l	(a1),d1
.else
	mvs.\3	(a1),d1
.endif
	.ifc	\1,negx
		move.b		sr_off+1(a7),d1			//ccr holen
		move		d1,ccr					//setzen
	.endif		
	\1	d1
	set_cc0
	move.\3	d1,(a1)
	ii_end
	.endm;

opnsd8a:.macro		// indirect: 1=operation 2=adress register 3=size
#ifdef	halten_op
	halt
#endif
	move.l	\2,a1
	jsr		ewf
.ifc	\3,l
	move.l	(a1),d1
.else
	mvs.\3	(a1),d1
.endif
	.ifc	\1,negx
		move.b		sr_off+1(a7),d1			//ccr holen
		move		d1,ccr					//setzen
	.endif		
	\1	d1
	set_cc0
	move.\3	d1,(a1)
	ii_end
	.endm;

opnsxx:.macro		// indirect: 1=operation 2=size 3=size adresse
#ifdef	halten_op
	halt
#endif

.ifc	\2,l
	move.l	(a1),d1
.else
	mvs.\2	(a1),d1
.endif
	move.\3	(a0)+,a1
	.ifc	\1,negx
		move.b		sr_off+1(a7),d1			//ccr holen
		move		d1,ccr					//setzen
	.endif		
	\1	d1
	set_cc0
	move.\2	d1,(a1)
	ii_end
	.endm;
//*******************************************************************************3
opismd:.macro		// immediate dx: 1=opieration	2=ea src 3=ea dest 4=size
#ifdef	halten_op
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
	\1	d0,d1
	set_cc0
.ifnc	\1,cmp.l
	move.\4		d1,\3
.endif
	ii_end
.endm;

opaismd:.macro		// immediate ax: 1=opieration	2=ea src 3=ea dest 4=size
#ifdef	halten_op
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
	\1	d0,d1
.ifnc	\1,cmp.l
.ifc	\3,usp
	move.l		d1,a1
	move.l		a1,usp
.else
	move.l		d1,\3
.endif
.endif
	ii_end
.endm;

opisia:.macro		// indirect: 1=opieration 2=adress register 3= src 4=dest 5=size
#ifdef	halten_op
	halt
#endif

.ifc	\5,l
	move.l	(a0)+,d0
.else
	.ifc	\5,w
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
	\1	d0,d1
	set_cc0
.ifnc	\1,cmp.l
	move.\5	d1,\4
.endif
	ii_end
.endm;

opisd16a:.macro		// indirect: 1=opieration 2=adress register 3=size
#ifdef	halten_op
	halt
#endif

.ifc	\3,l
	move.l	(a0)+,d0
.else
	.ifc	\3,w
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
	\1	d0,d1
	set_cc0
.ifnc	\1,cmp.l
	move.\3	d1,(a1)
.endif
	ii_end
	.endm;

opisd8a:.macro		// indirect: 1=opieration 2=adress register 3=size
#ifdef	halten_op
	halt
#endif

.ifc	\3,l
	move.l	(a0)+,d0
.else
	.ifc	\3,w
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
	\1	d0,d1
	set_cc0
.ifnc	\1,cmp.l
	move.\3	d1,(a1)
.endif
	ii_end
	.endm;

opisxx:.macro		// immediate: 1=opieration 2=size 3=size adresse
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
	\1	d0,d1
	set_cc0
.ifnc	\1,cmp.l
	move.\2	d1,(a1)
.endif
	ii_end
	.endm;
//*******************************************************************************3
opqsmd:.macro		// quick: 1=opieration	2=ea src 3=ea dest 4=size
.ifc	\4,l
	move.l	\2,d1
.else
	mvs.\4	\2,d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d0
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	,d1
	set_cc0
	move.\4		d1,\3
	ii_end
	.endm;

opaqsmd:.macro		// quick: 1=opieration	2=ea src 3=ea dest 4=size
.ifc	\2,usp
		move.l	usp,a1
		move.l	a1,d1
.else
		move.l		\2,d1
.endif
	\1	,d1
.ifc	\3,usp
	move.l		d1,a1
	move.l		a1,usp
.else
	move.l		d1,\3
.endif
	ii_end
	.endm;

opqsia:.macro		// indirect: 1=opieration 2=adress register 3= src 4=dest 5=size
#ifdef	halten_op
	halt
#endif

	move.l	\2,a1
.ifc	\5,l
	move.l	\3,d1
.else
	mvs.\5	\3,d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d0
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	,d1
	set_cc0
	move.\5	d1,\4
	ii_end
	.endm;

opqsd16a:.macro		// indirect: 1=opieration 2=adress register 3=size
#ifdef	halten_op
	halt
#endif

	move.l	\2,a1
	mvs.w	(a0)+,d1
	add.l	d1,a1
.ifc	\3,l
	move.l	(a1),d1
.else
	mvs.\3	(a1),d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d0
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	,d1
	set_cc0
	move.\3	d1,(a1)
	ii_end
	.endm;

opqsd8a:.macro		// indirect: 1=opieration 2=adress register 3=size
#ifdef	halten_op
	halt
#endif

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
	move.l	d0_off(a7),d0
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	,d1
	set_cc0
	move.\3	d1,(a1)
	ii_end
	.endm;

opqsxx:.macro		// quick: 1=opieration 2=size 3=size adresse
#ifdef	halten_op
	halt
#endif

	move.\3	(a0)+,a1
.ifc	\2,l
	move.l	(a1),d1
.else
	mvs.\2	(a1),d1
.endif
.ifc	\1,eor.l d0
	move.l	d0_off(a7),d0
.endif
.ifc	\1,eor.l d1
	move.l	d1_off(a7),d1
.endif
	\1	,d1
	set_cc0
	move.\2	d1,(a1)
	ii_end
	.endm;
