/********************************************************/
/*   user/supervisor handler
/********************************************************/

.include "startcf.h"

.extern _rt_cacr;
.extern _rt_mod;
.extern _rt_ssp;
.extern _rt_usp;
.extern ___MMUBAR

/* Register read/write macros */
#define MCF_MMU_MMUCR            			___MMUBAR
#define MCF_MMU_MMUOR                       ___MMUBAR+0x04
#define MCF_MMU_MMUSR                       ___MMUBAR+0x08
#define MCF_MMU_MMUAR                       ___MMUBAR+0x10
#define MCF_MMU_MMUTR                       ___MMUBAR+0x14
#define MCF_MMU_MMUDR                       ___MMUBAR+0x18

.public _privileg_violation
.public cpusha

.text
_privileg_violation:
	move.w		#0x2700,sr
	lea			-12(a7),a7
	movem.l		d0/a0/a5,(a7)
#ifndef cf_stack
	lea			0x52f0,a0
	move.l		#0x20,(a0)		// set auf 68030
#endif
	lea			_rt_mod,a0		// zugriff setzen
	tst.b		(a0)			// vom rt_supervisormodus?
	bne			pv_work			// ja->
// tatsächlich privileg violation
	mov3q.l		#-1,(a0)		// sr_mod setzen
	move.l		usp,a5			// usp holen
	move.l		a5,8(a0)		// sichern
	move.l		4(a0),a5		// rt_ssp holen
#ifdef cf_stack
	move.l		16(a7),-(a5)	// pc verschieben
	move.l		12(a7),-(a5)	// sr verschieben
	bset		#5,2(a5)		// auf super setzen
#else
	move.w		12(a7),-(a5)	// vector nr.
	move.l		16(a7),-(a5)	// pc verschieben
	move.w		14(a7),-(a5)	// sr verschieben
	bset		#5,(a5)			// auf super
#endif
	move.l		a5,usp
	move.l		12(a0),a5		// rt_vbr
	lea			0x18(a5),a5		// vector
	move.l		(a5),16(a7)		// vector privileg violation
	movem.l		(a7),d0/a0/a5	// register zurück
	lea			12(a7),a7
	rte
// privileg violation
pv_work:	
	move.l		16(a7),a5		// fault pc
	move.b		(a5),d0			// fault code
	cmp.b		#0x4e,d0		// 1.byt 0x4e
	beq			pv_4e			// ja->
	cmp.b		#0x46,d0		// 1.byt 0x46
	beq			pv_46			// ja->
	cmp.b		#0x40,d0		// 1.byt 0x40
	beq			pv_40			// ja->
	cmp.b		#0xf4,d0		// 0xf4?
	beq			pv_f4
	cmp.b		#0xf3,d0		// 0xf3?
	beq			pv_f3
// hierher sollt man nicht kommen
	nop
	halt
	nop
// code 0x4exx  ********************************************
pv_4e:
	move.b		1(a5),d0
	cmp.b		#0x73,d0		//rte?
	beq			pv_rte			//ja->
	cmp.b		#0x72,d0		//stop?
	beq			pv_stop			//ja->
	cmp.b		#0x7B,d0		//movec?
	beq			pv_movec		//ja->
// move usp	
	btst		#3,d0			// to or from
	bne			pv_usp_to_ax	// usp -> ax
// move ax->usp	
	cmp.b		#0x60,d0		//movec?
	beq			pv_a0_usp		//ja->
	cmp.b		#0x61,d0		//movec?
	beq			pv_a1_usp		//ja->
	cmp.b		#0x62,d0		//movec?
	beq			pv_a2_usp		//ja->
	cmp.b		#0x63,d0		//movec?
	beq			pv_a3_usp		//ja->
	cmp.b		#0x64,d0		//movec?
	beq			pv_a4_usp		//ja->
	cmp.b		#0x65,d0		//movec?
	beq			pv_a5_usp		//ja->
	cmp.b		#0x66,d0		//movec?
	beq			pv_a6_usp		//ja->
	halt
	bra			pv_a7_usp		//ja->
// move usp->ax	
pv_usp_to_ax:
	move.l		8(a0),a5		//rt_usp holen
	cmp.b		#0x68,d0		//movec?
	beq			pv_usp_a0		//ja->
	cmp.b		#0x69,d0		//movec?
	beq			pv_usp_a1		//ja->
	cmp.b		#0x6a,d0		//movec?
	beq			pv_usp_a2		//ja->
	cmp.b		#0x6b,d0		//movec?
	beq			pv_usp_a3		//ja->
	cmp.b		#0x6c,d0		//movec?
	beq			pv_usp_a4		//ja->
	cmp.b		#0x6d,d0		//movec?
	beq			pv_usp_a5		//ja->
	cmp.b		#0x6e,d0		//movec?
	beq			pv_usp_a6		//ja->
// usp->a7
	move.l		a5,4(a0)		// rt usp -> rt ssp
	move.l		a5,usp			// und setzen
	bra			pv_usp_ax
// a0->usp
pv_a0_usp:		move.l 4(a7),a5
	bra			pv_ax_usp	
// a1->usp
pv_a1_usp:		move.l a1,a5
	bra			pv_ax_usp	
// a2->usp
pv_a2_usp:		move.l a2,a5
	bra			pv_ax_usp	
// a3->usp
pv_a3_usp:		move.l a3,a5
	bra			pv_ax_usp	
// a4->usp
pv_a4_usp:		move.l a4,a5
	bra			pv_ax_usp	
// a5->usp
pv_a5_usp:		move.l 8(a7),a5
	bra			pv_ax_usp	
// a6->usp
pv_a6_usp:		move.l a6,a5
	bra			pv_ax_usp	
// a7->usp
pv_a7_usp:		move.l 4(a0),a5		// rt_ssp -> a5
pv_ax_usp:
	move.l		a5,8(a0)			// usp -> rt_usp
	addq.l		#2,16(a7)			// next
	movem.l		(a7),d0/a0/a5		// register zurück
	lea			12(a7),a7
	rte
// usp->a0
pv_usp_a0:
	move.l		a5,4(a7)
	bra			pv_usp_ax
pv_usp_a1:
	move.l		a5,a1
	bra			pv_usp_ax
pv_usp_a2:
	move.l		a5,a2
	bra			pv_usp_ax
pv_usp_a3:
	move.l		a5,a3
	bra			pv_usp_ax
pv_usp_a4:
	move.l		a5,a4
	bra			pv_usp_ax
pv_usp_a5:
	move.l		a5,8(a7)
	bra			pv_usp_ax
pv_usp_a6:
	move.l		a5,a6
pv_usp_ax:
	addq.l		#2,16(a7)		// next
	movem.l		(a7),d0/a0/a5	// register zurück
	lea			12(a7),a7
	rte
// rte
pv_rte:
	move.l		usp,a5
#ifdef cf_stack
	move.l		(a5)+,12(a7)	// sr verschieben
	move.l		(a5)+,16(a7)	// pc verschieben
#else
	move.w		(a5)+,14(a7)	// sr verschieben
	move.l		(a5)+,16(a7)	// pc verschieben
	move.w		(a5)+,12(a7)	// vector
#endif
	bclr		#5,14(a7)		// war es von super?
	bne			pv_rte_sup		// ja->
	clr.l		(a0)			// rt_mod auf user
	move.l		a5,4(a0)		// rt_ssp sichern
	move.l		8(a0),a5		// rt_usp holen
pv_rte_sup:
	move.l		a5,usp			// usp setzen
	movem.l		(a7),d0/a0/a5	// register zurück
	lea			12(a7),a7
	rte
// stop
pv_stop:
	move.b		2(a5),d0		// sr wert
	and.l		#0x0700,d0		// int mask
	cmp.w		#0x700,d0
	beq			stop7
	cmp.w		#0x600,d0
	beq			stop6
	cmp.w		#0x500,d0
	beq			stop5
	cmp.w		#0x400,d0
	beq			stop4
	cmp.w		#0x300,d0
	beq			stop3
	cmp.w		#0x200,d0
	beq			stop2
	cmp.w		#0x100,d0
	beq			stop1
	stop		#0x2000
	bra			stop_weiter
stop1:
	stop		#0x2100
	bra			stop_weiter
stop2:
	stop		#0x2200
	bra			stop_weiter
stop3:
	stop		#0x2300
	bra			stop_weiter
stop4:
	stop		#0x2400
	bra			stop_weiter
stop5:
	stop		#0x2500
	bra			stop_weiter
stop6:
	stop		#0x2600
	bra			stop_weiter
stop7:
	stop		#0x2700
stop_weiter:
	addq.l		#4,16(a7)		// next 
	movem.l		(a7),d0/a0/a5	// register zurück
	lea			12(a7),a7
	rte
// movec ???????
pv_movec:
	move.w		2(a5),d0			// 2.word holen
	and.l		#0xf000,d0
	btst		#15,d0				// addressregister?
	bne			pv_movec_ax			// ja->
	tst.w		d0					// d0?
	bne			pvm_d1				// nein->
	move.l		(a7),-(a7)			// d0 holen und sichern
	bra			pvm_me
pvm_d1:
	cmp.w		#0x1000,d0			// d1?
	bne			pvm_d2				// nein->
	move.l		d1,-(a7)			// d1 holen und sichern
	bra			pvm_me				// fertig machen
pvm_d2:
	cmp.w		#0x2000,d0			// d1?
	bne			pvm_d3				// nein->
	move.l		d2,-(a7)			// d2 holen und sichern
	bra			pvm_me				// fertig machen
pvm_d3:
	cmp.w		#0x3000,d0			// d1?
	bne			pvm_d4				// nein->
	move.l		d3,-(a7)			// d3 holen und sichern
	bra			pvm_me				// fertig machen
pvm_d4:
	cmp.w		#0x4000,d0			// d1?
	bne			pvm_d5				// nein->
	move.l		d4,-(a7)			// d4 holen und sichern
	bra			pvm_me				// fertig machen
pvm_d5:
	cmp.w		#0x5000,d0			// d1?
	bne			pvm_d6				// nein->
	move.l		d5,-(a7)			// d5 holen und sichern
	bra			pvm_me				// fertig machen
pvm_d6:
	cmp.w		#0x6000,d0			// d1?
	bne			pvm_d7				// nein->
	move.l		d6,-(a7)			// d6 holen und sichern
	bra			pvm_me				// fertig machen
pvm_d7:
	move.l		d7,-(a7)			// d7 holen und sichern
	bra			pvm_me				// fertig machen
pv_movec_ax:
	cmp.w		#0x8000,d0			// a0?
	bne			pvm_a1				// nein->
	move.l		4(a7),-(a7)			// a0 holen und sichern
	bra			pvm_me				// fertig machen
pvm_a1:
	cmp.w		#0x9000,d0			// a0?
	bne			pvm_a2				// nein->
	move.l		a1,-(a7)			// a1 holen und sichern
	bra			pvm_me				// fertig machen
pvm_a2:
	cmp.w		#0xa000,d0			// a0?
	bne			pvm_a3				// nein->
	move.l		a2,-(a7)			// a2 holen und sichern
	bra			pvm_me				// fertig machen
pvm_a3:
	cmp.w		#0xb000,d0			// a0?
	bne			pvm_a4				// nein->
	move.l		a3,-(a7)			// a3 holen und sichern
	bra			pvm_me				// fertig machen
pvm_a4:
	cmp.w		#0xc000,d0			// a0?
	bne			pvm_a5				// nein->
	move.l		a4,-(a7)			// a4 holen und sichern
	bra			pvm_me				// fertig machen
pvm_a5:
	cmp.w		#0xd000,d0			// a0?
	bne			pvm_a6				// nein->
	move.l		8(a7),-(a7)			// a5 holen und sichern
	bra			pvm_me				// fertig machen
pvm_a6:
	cmp.w		#0xe000,d0			// a0?
	bne			pvm_a7				// nein->
	move.l		a6,-(a7)			// a6 holen und sichern
	bra			pvm_me				// fertig machen
pvm_a7:
	move.l		4(a7),-(a7)			// a7 holen und sichern
pvm_me:
	move.w		2(a5),d0			// 2.word holen
	andi.l		#0xf,d0				// nur letzte 4 bits
	move.l		(a7)+,8(a0,d0*4)	// start bei +8, *4 weil long
	jsr			cpusha				// gesammten cache flushen
	rte
// code 0x46xx *****************************************
pv_46:	
	move.b		1(a5),d0	
	cmp.b		#0xfc,d0		//#d16->sr
	beq			im_sr			//ja->
//move dx->sr (sr und rt_mod ist supervisor sonst wäre es privileg violation
	cmp.b		#0xc0,d0		//d0->sr?
	bne			d1_sr			//nein->
	move.w		2(a7),d0		//hier ist d0 gesichert
	bra			d0_sr
d1_sr:
	cmp.b		#0xc1,d0		//d1->sr?
	bne			d2_sr			//nein->
	move.w		d1,d0
	bra			d0_sr
d2_sr:
	cmp.b		#0xc2,d0		//d2->sr?
	bne			d3_sr
	move.w		d2,d0
	bra			d0_sr
d3_sr:
	cmp.b		#0xc3,d0		//d3->sr?
	bne			d4_sr
	move.w		d3,d0
	bra			d0_sr
d4_sr:
	cmp.b		#0xc4,d0		//d4->sr?
	bne			d5_sr
	move.w		d4,d0
	bra			d0_sr
d5_sr:
	cmp.b		#0xc5,d0		//d5->sr?
	bne			d6_sr
	move.w		d5,d0
	bra			d0_sr
d6_sr:
	cmp.b		#0xc6,d0		//d6->sr?
	bne			d7_sr
	move.w		d6,d0
	bra			d0_sr
d7_sr:
	move.w		d7,d0			// sonst d7->sr
d0_sr:
	addq.l		#2,16(a7)		// next 
	bra			pv_set_sr_end	// fertig machen
// move #xxxx,sr	
im_sr:
	addq.l		#4,16(a7)		// next 
	move.w		2(a5),d0		// data
pv_set_sr_end:
	bclr		#13,d0			// war super?
	bne			pv_sre2			// ja ->
	clr.l		(a0)
	move.l		usp,a5			// usp
	move.l		a5,4(a0)		// rt_ssp speichern
	move.l		8(a0),a5		// rt_usp holen
	move.l		a5,usp			// setzen
pv_sre2:
	move.w		d0,14(a7)		// sr setzen
	movem.l		(a7),d0/a0/a5	// register zurück
	lea			12(a7),a7
	rte
// code 0x40xx  *****************************************
pv_40:
	move.b		1(a5),d0		// 2.byt
	cmp.b		#0xe7,d0
	beq			pv_strldsr
// move sr->dx
	move.l		12(a7),a5		// sr holen
	tst.b		(a0)			// super?
	beq			pv_40_user		// nein?
	lea			0x2000(a5),a5	// super zuaddieren
pv_40_user:
	cmp.b		#0xc0,d0
	bne			nsr_d1
	move.w		a5,2(a7)
	bra			sr_dx_end
nsr_d1:
	cmp.b		#0xc1,d0
	bne			nsr_d2
	move.w		a5,d1
	bra			sr_dx_end
nsr_d2:
	cmp.b		#0xc2,d0
	bne			nsr_d3
	move.w		a5,d2
	bra			sr_dx_end
nsr_d3:
	cmp.b		#0xc3,d0
	bne			nsr_d4
	move.w		a5,d3
	bra			sr_dx_end
nsr_d4:
	cmp.b		#0xc4,d0
	bne			nsr_d5
	move.w		a5,d4
	bra			sr_dx_end
nsr_d5:
	cmp.b		#0xc5,d0
	bne			nsr_d6
	move.w		a5,d5
	bra			sr_dx_end
nsr_d6:
	cmp.b		#0xc6,d0
	bne			nsr_d7
	move.w		a5,d6
	bra			sr_dx_end
nsr_d7:
	move.w		a5,d7
	halt
sr_dx_end:
	addq.l		#2,16(a7)		// next 
	movem.l		(a7),d0/a0/a5	// register zurück
	lea			12(a7),a7
	rte
// strldsr
pv_strldsr:
	nop
	halt
	nop
// code 0xf4xx  ***********************************
pv_f4:
	addq.l		#2,16(a7)			// next instr
	move.b		1(a5),d0			// 2.byt
	bsr			pv_ax_a0			// richtiges register
	move.b		1(a5),d0			// 2.byt
	cmp.b		#0x30,d0			// >0xf430
	blo			pv_intouch
// cpushl
	cpushl		bc,(a0)
	movem.l		(a7),d0/a0/a5
	lea			12(a7),a7
	rte
pv_intouch:
	intouch		(a0)
	movem.l		(a7),d0/a0/a5
	lea			12(a7),a7
	rte
// subroutine register ax->a0
pv_ax_a0:
	and.l		#0x7,d0				// nur register nummer
	subq.l		#1,d0
	bmi			pv_a0_a0
	subq.l		#1,d0
	bmi			pv_a1_a0
	subq.l		#1,d0
	bmi			pv_a2_a0
	subq.l		#1,d0
	bmi			pv_a3_a0
	subq.l		#1,d0
	bmi			pv_a4_a0
	subq.l		#1,d0
	bmi			pv_a5_a0
	subq.l		#1,d0
	bmi			pv_a6_a0
	move.l		a7,a0
	rts
pv_a0_a0:
	move.l		8(a7),a0
	rts
pv_a1_a0:
	move.l		a1,a0
	rts
pv_a2_a0:
	move.l		a2,a0
	rts
pv_a3_a0:
	move.l		a3,a0
	rts
pv_a4_a0:
	move.l		a4,a0
	rts
pv_a5_a0:
	move.l		12(a7),a0
	rts
pv_a6_a0:
	move.l		a6,a0
	rts
// code 0xf4xx  ***********************************
pv_f3:
	addq.l		#2,16(a7)			// next instr
	move.b		1(a5),d0			// 2. byt
	cmp.b		#0x40,d0
	bgt			pv_frestore
//fsave (ax) oder d16(ax)
	jsr			pv_ax_a0			// richtiges register holen
	move.b		1(a5),d0			
	cmp.b		#0x20,d0
// +d16
	blt			pv_f3_ax
	addq.l		#2,16(a7)			// next instr
	clr.l		d0
	move.w		2(a0),d0			// d16
	add.l		d0,a0
pv_f3_ax:
	fsave		(a0)	
	movem.l		(a7),d0/a0/a5
	lea			12(a7),a7
	rte
pv_frestore:
	cmp.b		#0x7a,d0
	beq			pv_f_d16pc
// frestore (ax) oder d16(ax)
	jsr			pv_ax_a0			// richtiges register holen
	move.b		1(a5),d0			
	cmp.b		#0x60,d0
	blt			pv_frestore_ax
pv_fend:
	addq.l		#2,16(a7)			// next instr
	clr.l		d0
	move.w		2(a0),d0			// d16
	add.l		d0,a0
pv_frestore_ax:
	frestore	(a0)	
	movem.l		(a7),d0/a0/a5
	lea			12(a7),a7
	rte
// frestore d16(pc)
pv_f_d16pc:
	move.l		16(a7),a0			// pc holen
	bra			pv_fend
//*****************************************************
cpusha:	 
 	lea			-16(a7),a7
	movem.l		d0-d2/a0,(a7)		// register sichern
	move		sr,d2
	nop
	move		#0x2700,sr			// no interrupts

	clr.l		d0
	clr.l		d1
	move.l		d0,a0
cfa_setloop:
	cpushl		bc,(a0)				// flush
	lea			0x10(a0),a0			// index+1
	addq.l		#1,d1				// index+1
	cmpi.w		#512,d1				// alle sets?
	bne			cfa_setloop			// nein->
	clr.l		d1
	addq.l		#1,d0
	move.l		d0,a0
	cmpi.w		#4,d0				// all ways?
	bne			cfa_setloop			// nein->
	nop
	move.l		_rt_cacr,d0			// holen
	movec		d0,cacr				// setzen
	move.w		d2,sr				// alte interrupt maske
	movem.l		(a7),d0-d2/a0		// register zurück
	lea			16(a7),a7

	rts	
//*******************************************************33

