// movem
_ii_movem_lset: .macro
// movem rx,xxx.L
	ii_lset		0x48f9
// movem rx,-(ax)
	
// movem (ax)+,rx
	ii_lset		0x4cd8
	ii_lset		0x4cd9
	ii_lset		0x4cda
	ii_lset		0x4cdb
	ii_lset		0x4cdc
	ii_lset		0x4cdd
	ii_lset		0x4cde
	ii_lset		0x4cdf
// movem xxx.L,rx
	ii_lset		0x4cf9
.endm
//***********************************************************************************/
_ii_movem_func: .macro
//-------------------------------------------------------------------
// movem.l
//--------------------------------------------------------------------
// movem.l (ax)+,reg
//--------------------------------------------------------------------
//-------------------------------------------------------------------------------
ii_0x4cd8:		// movem.l (a0)+,reglist
	mvm_macro	0x4cd0,0x41e8,2
ii_0x4cd9:		// movem.l (a1)+,reglist
	mvm_macro	0x4cd1,0x43e9,2
ii_0x4cda:		// movem.l (a2)+,reglist
	mvm_macro	0x4cd2,0x45ea,2
ii_0x4cdb:		// movem.l (a3)+,reglist
	mvm_macro	0x4cd3,0x47eb,2
ii_0x4cdc:		// movem.l (a4)+,reglist
	mvm_macro	0x4cd4,0x49ec,2
ii_0x4cdd:		// movem.l (a5)+,reglist
	mvm_macro	0x4cd5,0x4bed,2
ii_0x4cde:		// movem.l (a6)+,reglist
	mvm_macro	0x4cd6,0x4dee,2
ii_0x4cdf:		// movem.l (a7)+,reglist
	mvm_macro	0x4cd7,0x4fef,2
//----------------------------------------------------------------------------
ii_0x48f9:		// movem.l reg,xxx.L
		move.w		(a0)+,d0
		move.l		(a0)+,a1
		movemrm_macro
		ii_end
//---------------------------------------------------------------------------------------------
ii_0x4cf9:		// movem.l xxx.L,reg
		move.w		(a0)+,d0
		move.l		(a0)+,a1
		movemmr_macro
		ii_end
.endm
//==============================================================
mvm_macro:.macro
halt
	lea			az_reg_table,a1
	mvz.b		(a0),d1
	mvz.b		0(a1,d1)+,d0
	mvz.b		1(a0),d1
	mvz.b		0(a1,d1)+,d1
	add.l		d0,d1
	lea			___RAMBAR1,a1
	move.l		a1,pc_off(a7)
	move.l		a1,d0
	addq.l		#1,d0
   	movec      	d0,RAMBAR1
	move.w		#\1,(a1)+			// movem.x (ax),reg_list
	move.w		(a0)+,(a1)+			// register list
	move.w		#\2,(a1)+			// lea 0(ax),ax
	lsl.l		#\3,d1				// * anzahl byts pro wert
	move.w		d1,(a1)+			// offset von lea
	move.w		#0x4ef9,(a1)+		// jmp.l
	move.l		a0,(a1)				// rücksprungadresse
   	move.l 		#___RAMBAR1 + 0x81,d0	// instruction
   	movec      	d0,RAMBAR1
	movem.l		(a7),d0/d1/a0/a1
	lea			ii_ss(a7),a7		// stack erhöhen
	rte
	.endm
	.long		0
az_reg_table:
	.byte		0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4		// 0-f
	.byte		1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5		// 10-1f
	.byte		1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5		// 20-2f
	.byte		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6		// 30-3f
	.byte		1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5		// 40-4f
	.byte		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6		// 50
	.byte		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6		// 60
	.byte		3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7		// 70
	.byte		1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5		// 80-8f
	.byte		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6		// 90
	.byte		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6		// a0
	.byte		3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7		// b0
	.byte		2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6		// c0
	.byte		3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7		// d0
	.byte		3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7		// e0
	.byte		4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8		// f0
//---------------------------------------------------------------------------------
movemrm_macro:.macro				// in d0 register liste, in a1 zieladresse
halt
		tst.b	d0					// datenregister zu verschieben?
		bne		mrm_dx				// ja->
		lsr.l	#8,d0				// sonst zu addressregister
		jmp		mmrm_nd7			// ->
mrm_dx:
		lsr.l	#1,d0
		bcc		mmrm_nd0
		move.l	d0_off(a7),(a1)+
mmrm_nd0:
		lsr.l	#1,d0
		bcc		mmrm_nd1
		move.l	d1_off(a7),(a1)+
mmrm_nd1:
		lsr.l	#1,d0
		bcc		mmrm_nd2
		move.l	d2,(a1)+
mmrm_nd2:
		lsr.l	#1,d0
		bcc		mmrm_nd3
		move.l	d3,(a1)+
mmrm_nd3:
		lsr.l	#1,d0
		bcc		mmrm_nd4
		move.l	d4,(a1)+
mmrm_nd4:
		lsr.l	#1,d0
		bcc		mmrm_nd5
		move.l	d5,(a1)+
mmrm_nd5:
		lsr.l	#1,d0
		bcc		mmrm_nd6
		move.l	d6,(a1)+
mmrm_nd6:
		lsr.l	#1,d0
		bcc		mmrm_nd7
		move.l	d7,(a1)+
mmrm_nd7:
		tst.b	d0					// addressregister zu verschieben?
		beq		mmrm_na7
		lsr.l	#1,d0
		bcc		mmrm_na0
		move.l	a0_off(a7),(a1)+
mmrm_na0:
		lsr.l	#1,d0
		bcc		mmrm_na1
		move.l	a1_off(a7),(a1)+
mmrm_na1:
		lsr.l	#1,d0
		bcc		mmrm_na2
		move.l	a2,(a1)+
mmrm_na2:
		lsr.l	#1,d0
		bcc		mmrm_na3
		move.l	a3,(a1)+
mmrm_na3:
		lsr.l	#1,d0
		bcc		mmrm_na4
		move.l	a4,(a1)+
mmrm_na4:
		lsr.l	#1,d0
		bcc		mmrm_na5
		move.l	a5,(a1)+
mmrm_na5:
		lsr.l	#1,d0
		bcc		mmrm_na6
		move.l	a6,(a1)+
mmrm_na6:
		lsr.l	#1,d0
		bcc		mmrm_na7
		move.l	a0,d1		// sichern
		move.l	usp,a0		// ist ja usp
		move.l	a0,(a1)+	// nach a0
		move.l	d1,a0		// pc zurück
mmrm_na7:
		.endm
//---------------------------------------------------------------------------------------------
movemmr_macro:.macro				// in d0 register liste, in a1 source adr
halt
		tst.b	d0					// datenregister zu verschieben?
		bne		mmr_dx				// ja->
		lsr.l	#8,d0				// sonst zu addressregister
		bra		mmmr_nd7			// ->
mmr_dx:
		lsr.l	#1,d0
		bcc		mmmr_nd0
		move.l	(a1)+,d0_off(a7)
mmmr_nd0:
		lsr.l	#1,d0
		bcc		mmmr_nd1
		move.l	(a1)+,d1_off(a7)
mmmr_nd1:
		lsr.l	#1,d0
		bcc		mmmr_nd2
		move.l	(a1)+,d2
mmmr_nd2:
		lsr.l	#1,d0
		bcc		mmmr_nd3
		move.l	(a1)+,d3
mmmr_nd3:
		lsr.l	#1,d0
		bcc		mmmr_nd4
		move.l	(a1)+,d4
mmmr_nd4:
		lsr.l	#1,d0
		bcc		mmmr_nd5
		move.l	(a1)+,d5
mmmr_nd5:
		lsr.l	#1,d0
		bcc		mmmr_nd6
		move.l	(a1)+,d6
mmmr_nd6:
		lsr.l	#1,d0
		bcc		mmmr_nd7
		move.l	(a1)+,d7
mmmr_nd7:
		tst.b	d0					// addressregister zu verschieben?
		beq		mmmr_na7			// nein->
		lsr.l	#1,d0
		bcc		mmmr_na0
		move.l	(a1)+,a0_off(a7)
mmmr_na0:
		lsr.l	#1,d0
		bcc		mmmr_na1
		move.l	(a1)+,a1_off(a7)
mmmr_na1:
		lsr.l	#1,d0
		bcc		mmmr_na2
		move.l	(a1)+,a2
mmmr_na2:
		lsr.l	#1,d0
		bcc		mmmr_na3
		move.l	(a1)+,a3
mmmr_na3:
		lsr.l	#1,d0
		bcc		mmmr_na4
		move.l	(a1)+,a4
mmmr_na4:
		lsr.l	#1,d0
		bcc		mmmr_na5
		move.l	(a1)+,a5
mmmr_na5:
		lsr.l	#1,d0
		bcc		mmmr_na6
		move.l	(a1)+,a6
mmmr_na6:
		lsr.l	#1,d0
		bcc		mmmr_na7
		move.l	a0,d1		// sichern
		move.l	(a1)+,a0	// nach a0
		move.l	a0,usp		// war ja usp
		move.l	d1,a0		// pc zurück
mmmr_na7:
		.endm		
