//--------------------------------------------------------------------
// movep
//--------------------------------------------------------------------
.text
ii_movep_lset:.macro 
	ii_lset_opeau	01,0	//movep.w d(a0-7),d0
	ii_lset_opeau	03,0	//movep.w d(a0-7),d1
	ii_lset_opeau	05,0	//movep.w d(a0-7),d2
	ii_lset_opeau	07,0	//movep.w d(a0-7),d3
	ii_lset_opeau	09,0	//movep.w d(a0-7),d4
	ii_lset_opeau	0b,0	//movep.w d(a0-7),d5
	ii_lset_opeau	0d,0	//movep.w d(a0-7),d6
	ii_lset_opeau	0f,0	//movep.w d(a0-7),d7

	ii_lset_opeau	01,4	//movep.w d0,d(a0-7)
	ii_lset_opeau	03,4	//movep.w d1,d(a0-7)
	ii_lset_opeau	05,4	//movep.w d2,d(a0-7)
	ii_lset_opeau	07,4	//movep.w d3,d(a0-7)
	ii_lset_opeau	09,4	//movep.w d4,d(a0-7)
	ii_lset_opeau	0b,4	//movep.w d5,d(a0-7)
	ii_lset_opeau	0d,4	//movep.w d6,d(a0-7)
	ii_lset_opeau	0f,4	//movep.w d7,d(a0-7)

	ii_lset_opeau	01,8	//movep.l d(a0-7),d0
	ii_lset_opeau	03,8	//movep.l d(a0-7),d1
	ii_lset_opeau	05,8	//movep.l d(a0-7),d2
	ii_lset_opeau	07,8	//movep.l d(a0-7),d3
	ii_lset_opeau	09,8	//movep.l d(a0-7),d4
	ii_lset_opeau	0b,8	//movep.l d(a0-7),d5
	ii_lset_opeau	0d,8	//movep.l d(a0-7),d6
	ii_lset_opeau	0f,8	//movep.l d(a0-7),d7

	ii_lset_opeau	01,c	//movep.l d0,d(a0-7)
	ii_lset_opeau	03,c	//movep.l d1,d(a0-7)
	ii_lset_opeau	05,c	//movep.l d2,d(a0-7)
	ii_lset_opeau	07,c	//movep.l d3,d(a0-7)
	ii_lset_opeau	09,c	//movep.l d4,d(a0-7)
	ii_lset_opeau	0b,c	//movep.l d5,d(a0-7)
	ii_lset_opeau	0d,c	//movep.l d6,d(a0-7)
	ii_lset_opeau	0f,c	//movep.l d7,d(a0-7)
.endm
//---------------------------------------------------------------------------------------------
ii_movep_func:.macro
//movep.w d(a0-7),d0-7
	ii_movep		010,d0_off(a7),wad
	ii_movep		030,d1_off(a7),wad
	ii_movep		050,d2,wad
	ii_movep		070,d3,wad
	ii_movep		090,d4,wad
	ii_movep		0b0,d5,wad
	ii_movep		0d0,d6,wad
	ii_movep		0f0,d7,wad
//movep.w d0-7,d(a0-7)
	ii_movep		014,d0_off(a7),wda
	ii_movep		034,d1_off(a7),wda
	ii_movep		054,d2,wda
	ii_movep		074,d3,wda
	ii_movep		094,d4,wda
	ii_movep		0b4,d5,wda
	ii_movep		0d4,d6,wda
	ii_movep		0f4,d7,wda
//movep.l d(a0-7),d0-7
	ii_movep		018,d0_off(a7),lad
	ii_movep		038,d1_off(a7),lad
	ii_movep		058,d2,lad
	ii_movep		078,d3,lad
	ii_movep		098,d4,lad
	ii_movep		0b8,d5,lad
	ii_movep		0d8,d6,lad
	ii_movep		0f8,d7,lad
//movep.l d0-7,d(a0-7)
	ii_movep		01c,d0_off(a7),lda
	ii_movep		03c,d1_off(a7),lda
	ii_movep		05c,d2,lda
	ii_movep		07c,d3,lda
	ii_movep		09c,d4,lda
	ii_movep		0bc,d5,lda
	ii_movep		0dc,d6,lda
	ii_movep		0fc,d7,lda
.endm		
//---------------------------------------------------------------------------------------------
ii_movep:.macro	//1=code ziffer 1-3 2=register 3=art
ii_0x\18:
#ifdef	halten_movep
	halt
#endif
	move.l			a0_off(a7),a1
	ii_movep\3_up1	\2
ii_0x\19:
#ifdef	halten_movep
	halt
#endif
	move.l			a1_off(a7),a1
	ii_movep\3_up1	\2
ii_0x\1a:
#ifdef	halten_movep
	halt
#endif
	move.l			a2,a1
	ii_movep\3_up1	\2
ii_0x\1b:
#ifdef	halten_movep
	halt
#endif
	move.l			a3,a1
	ii_movep\3_up1	\2
ii_0x\1c:
#ifdef	halten_movep
	halt
#endif
	move.l			a4,a1
	ii_movep\3_up1	\2
ii_0x\1d:
#ifdef	halten_movep
	halt
#endif
	move.l			a5,a1
	ii_movep\3_up1	\2
ii_0x\1e:
#ifdef	halten_movep
	halt
#endif
	move.l			a6,a1
	ii_movep\3_up1	\2
ii_0x\1f:
#ifdef	halten_movep
	halt
#endif
	move.l			usp,a1
	ii_movep\3_up1	\2
.endm	
	
ii_movepwad_up1:.macro
	mvs.w			(a0)+,d1
	add.l			d1,a1
	move.b			(a1),d0
	lsl.l			#8,d0
	move.b			2(a1,d1.l),d0
	move.w			d0,\1
	ii_end
.endm

ii_movepwda_up1:.macro
	mvs.w			(a0)+,d1
	add.l			d1,a1
	move.w			\1,d0
	move.b			d0,2(a1)
	lsr.l			#8,d0
	move.b			d0,(a1)
	ii_end
.endm
	
ii_moveplad_up1:.macro
	mvs.w			(a0)+,d1
	add.l			d1,a1
	move.b			(a1),d0
	lsl.l			#8,d0
	move.b			2(a1),d0
	lsl.l			#8,d0
	move.b			4(a1),d0
	lsl.l			#8,d0
	move.b			6(a1),d0
	move.l			d0,\1
	ii_end
.endm

ii_moveplda_up1:.macro
	mvs.w			(a0)+,d1
	add.l			d1,a1
	move.l			\1,d0
	move.b			d0,6(a1)
	lsr.l			#8,d0
	move.b			d0,4(a1)
	lsr.l			#8,d0
	move.b			d0,2(a1)
	lsr.l			#8,d0
	move.b			d0,(a1)
	ii_end
.endm
