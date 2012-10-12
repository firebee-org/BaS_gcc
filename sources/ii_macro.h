/*******************************************************/
// constanten
/*******************************************************/
.extern ___RAMBAR1
.extern _rt_cacr
.extern _rt_mod
.extern _rt_ssp
.extern _rt_usp
.extern _rt_vbr
.extern _d0_save
.extern _a7_save

ii_ss	=		16
d0_off	=		0
d1_off	=		4
a0_off	=		8
a1_off	=		12
format_off =	16
sr_off	=		18
ccr_off =		19
pc_off	=		20

#define 	table 	0x20000000-0x8000-0xF000*4 // Adresse Sprungtabelle 	-> 8000=Sprungbereich mod cod, 61k(ohne 0xFxxx!)x4= tabelle

/*******************************************************/
// allgemeine macros
/*******************************************************/
ii_end:	.macro
		move.l a0,pc_off(a7)
		movem.l (a7),d0/d1/a0/a1
		lea ii_ss(a7),a7
		rte
		.endm;

set_cc0:.macro
		move.w	ccr,d0 
		move.b	d0,ccr_off(a7)
		.endm;

ii_esr:	.macro					// geht nicht!!??
		movem.l		(a7),d0/d1/a0/a1
		lea			ii_ss+8(a7),a7		// stack erhöhen
		move.w		d0,_d0_save			// d0.w sicheren
		move.w		-6(a7),d0			// sr holen
		move.w		d0,sr				// sr setzen
		nop
		move.w		_d0_save,d0			// d0.w zurück
		.endm;

ii_end_mvm:.macro
		move.l	a0_off(a7),a0
		lea		16(a7),a7
		rte
		.endm;

ii_endj:.macro
		movem.l	(a7),d0/d1/a0/a1		// register zurück
		lea		ii_ss(a7),a7			// korr
		rte								// ende
		.endm;
		
set_nzvc:.macro							// set ccr bits nzvc
		move.w	ccr,d1
		bclr	#4,d1
		btst	#4,ccr_off(a7)
		beq		snzvc2\@
		bset	#4,d1
snzvc2\@:
		move.b	d1,ccr_off(a7)
		.endm;
		
set_cc1:.macro
		move.w	ccr,d1
		move.b	d1,ccr_off(a7)
		.endm;

set_cc_b:.macro
		move.w	ccr,d1
		btst	#7,d0					// byt negativ?
		beq		set_cc_b2\@
		bset	#3,d1					// make negativ
set_cc_b2\@:
		move.b	d1,ccr_off(a7)
		.endm;
		
set_cc_w:.macro
		move.w	ccr,d1
		btst	#15,d0					// byt negativ?
		beq		set_cc_w2\@
		bset	#3,d1					// make negativ
set_cc_w2\@:
		move.b	d1,ccr_off(a7)
		.endm;
		
get_pc:	.macro
		lea.l		(a0),a1
		.endm;

//--------------------------------------------------------------------
ii_lset:.macro
		lea	table+\1*4,a0
		move.l #ii_\1,(a0)
		.endm;
ii_lset_dx:.macro				// 0x1.22 -> z.B. 1=d,2=4 ->0xd040 -> 0xde40
		ii_lset_dxg	\1,\2
		ii_lset_dxu	\1,\2
		.endm;
ii_lset_dxg:.macro				// gerade: 0x1.22 -> z.B. 1=d,2=4 ->0xd040 -> 0xde40
		lea 	table+0x\10\2*4,a0
		move.l	#ii_0x\10\2,(a0)
		lea		0x800(a0),a0	// 4 * 0x200
		move.l	#ii_0x\12\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\14\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\16\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\18\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\1a\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\1c\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\1e\2,(a0)
		.endm;
ii_lset_dxu:.macro				// ungerade: 0x1.22 -> z.B. 1=d,2=4 ->0xd140 -> 0xdf40
		lea	table+0x\11\2*4,a0
		move.l	#ii_0x\11\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\13\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\15\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\17\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\19\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\1b\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\1d\2,(a0)
		lea		0x800(a0),a0
		move.l	#ii_0x\1f\2,(a0)
		.endm;

