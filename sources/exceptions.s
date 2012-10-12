/********************************************************/
/*   exception vectoren intialisieren
/********************************************************/

.include "startcf.h"

.extern ___Bas_base
.extern ___SUP_SP
.extern ___BOOT_FLASH
.extern ___RAMBAR0
.extern _rt_cacr
.extern _rt_mod
.extern _rt_ssp
.extern _rt_usp
.extern _rt_vbr
.extern _illegal_instruction
.extern _privileg_violation
.extern _mmutr_miss
.extern ___MBAR
.extern ___MMUBAR
.extern _video_tlb
.extern _video_sbt
.extern cpusha

/* Register read/write macros */
#define MCF_MMU_MMUCR            			___MMUBAR
#define MCF_MMU_MMUOR                       ___MMUBAR+0x04
#define MCF_MMU_MMUSR                       ___MMUBAR+0x08
#define MCF_MMU_MMUAR                       ___MMUBAR+0x10
#define MCF_MMU_MMUTR                       ___MMUBAR+0x14
#define MCF_MMU_MMUDR                       ___MMUBAR+0x18  

#define MCF_EPORT_EPPAR                     ___MBAR+0xF00
#define MCF_EPORT_EPDDR                     ___MBAR+0xF04
#define MCF_EPORT_EPIER                     ___MBAR+0xF05
#define MCF_EPORT_EPDR                      ___MBAR+0xF08
#define MCF_EPORT_EPPDR                     ___MBAR+0xF09
#define MCF_EPORT_EPFR                      ___MBAR+0xF0C

#define MCF_GPIO_PODR_FEC1L                 ___MBAR+0xA07

#define MCF_PSC0_PSCTB_8BIT                 ___MBAR+0x860C

#define MCF_PSC3_PSCRB_8BIT                 ___MBAR+0x890C
#define MCF_PSC3_PSCTB_8BIT                 ___MBAR+0x890C

.public	_vec_init

//mmu ---------------------------------------------------
/* Register read/write macros */
#define MCF_MMU_MMUCR            			___MMUBAR
#define MCF_MMU_MMUOR                       ___MMUBAR+0x04
#define MCF_MMU_MMUSR                       ___MMUBAR+0x08
#define MCF_MMU_MMUAR                       ___MMUBAR+0x10
#define MCF_MMU_MMUTR                       ___MMUBAR+0x14
#define MCF_MMU_MMUDR                       ___MMUBAR+0x18


/* Bit definitions and macros for MCF_MMU_MMUCR */
#define MCF_MMU_MMUCR_EN                     (0x1)
#define MCF_MMU_MMUCR_ASM                    (0x2)

/* Bit definitions and macros for MCF_MMU_MMUOR */
#define MCF_MMU_MMUOR_UAA                    (0x1)
#define MCF_MMU_MMUOR_ACC                    (0x2)
#define MCF_MMU_MMUOR_RW                     (0x4)
#define MCF_MMU_MMUOR_ADR                    (0x8)
#define MCF_MMU_MMUOR_ITLB                   (0x10)
#define MCF_MMU_MMUOR_CAS                    (0x20)
#define MCF_MMU_MMUOR_CNL                    (0x40)
#define MCF_MMU_MMUOR_CA                     (0x80)
#define MCF_MMU_MMUOR_STLB                   (0x100)
#define MCF_MMU_MMUOR_AA(x)                  (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_MMU_MMUSR */
#define MCF_MMU_MMUSR_HIT                    (0x2)
#define MCF_MMU_MMUSR_WF                     (0x8)
#define MCF_MMU_MMUSR_RF                     (0x10)
#define MCF_MMU_MMUSR_SPF                    (0x20)

/* Bit definitions and macros for MCF_MMU_MMUAR */
#define MCF_MMU_MMUAR_FA(x)                  (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_MMU_MMUTR */
#define MCF_MMU_MMUTR_V                      (0x1)
#define MCF_MMU_MMUTR_SG                     (0x2)
#define MCF_MMU_MMUTR_ID(x)                  (((x)&0xFF)<<0x2)
#define MCF_MMU_MMUTR_VA(x)                  (((x)&0x3FFFFF)<<0xA)

/* Bit definitions and macros for MCF_MMU_MMUDR */
#define MCF_MMU_MMUDR_LK                     (0x2)
#define MCF_MMU_MMUDR_X                      (0x4)
#define MCF_MMU_MMUDR_W                      (0x8)
#define MCF_MMU_MMUDR_R                      (0x10)
#define MCF_MMU_MMUDR_SP                     (0x20)
#define MCF_MMU_MMUDR_CM(x)                  (((x)&0x3)<<0x6)
#define MCF_MMU_MMUDR_SZ(x)                  (((x)&0x3)<<0x8)
#define MCF_MMU_MMUDR_PA(x)                  (((x)&0x3FFFFF)<<0xA)

#define	 std_mmutr	(MCF_MMU_MMUTR_SG|MCF_MMU_MMUTR_V)
#define	 mmuord_d	(                   MCF_MMU_MMUOR_ACC|MCF_MMU_MMUOR_UAA)
#define	 mmuord_i	(MCF_MMU_MMUOR_ITLB|MCF_MMU_MMUOR_ACC|MCF_MMU_MMUOR_UAA)
#define	 wt_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(00)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
#define	 cb_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(01)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
#define	 nc_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(10)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
//---------------------------------------------------
/*********************************************************************
*
* General Purpose Timers (GPT)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_GPT0_GMS                         ___MBAR+0x800

/*********************************************************************
*
* Slice Timers (SLT)
*
*********************************************************************/

#define MCF_SLT0_SCNT                        ___MBAR+0x908

/**********************************************************/
// macros
/**********************************************************/
irq:	.macro	vector,int_mask,clr_int
		move.w	#0x2700,sr			// disable interrupt
		subq.l	#8,a7
		movem.l	d0/a5,(a7)			// register sichern
		lea		MCF_EPORT_EPFR,a5
		move.b	#clr_int,(a5)		// clear int pending
// test auf protect mode ---------------------
		move.b	DIP_SWITCHa,d0
		btst	#7,d0 
		bne		@irq_protect		// ja->
// -------------------------------------------		
		movem.l	(a7),d0/a5			// register zurück
		addq.l	#8,a7
		move.l	vector,-(a7)
		move	#0x2\200,sr
		rts
@irq_protect:
		move.l	usp,a5				// usp holen  
		tst.b	_rt_mod				// supervisor? 
		bne		@sev_supint			// ja -> 
		mov3q.l	#-1,_rt_mod			// auf supervisor setzen
		move.l	a5,_rt_usp			// rt_usp speichern 
		move.l	_rt_ssp,a5			// rt_ssp holen
#ifdef cf_stack
		move.l	12(a7),-(a5)		// pc transferieren 
		move.l	8(a7),-(a5)			// sr,vec
#else
		move.w	8(a7),-(a5)			// vector nr.
		move.l	12(a7),-(a5)		// pc verschieben
		move.w	10(a7),-(a5)		// sr verschieben
#endif
		bra		@irq_end
@sev_supint:
#ifdef cf_stack
		move.l	12(a7),-(a5)		// pc transferieren 
		move.l	8(a7),-(a5)			// sr,vec
		bset	#5,2(a5)			// auf super setzen
#else
		move.w	8(a7),-(a5)			// vector nr.
		move.l	12(a7),-(a5)		// pc verschieben
		move.w	10(a7),-(a5)		// sr verschieben
		bset	#5,(a5)				// auf super
#endif
@irq_end:
		move.l	a5,usp				// usp setzen
		lea		vector,a5
		adda.l	_rt_vbr,a5
		move.l	(a5),12(a7)			// vectoradresse eintragen
		move.b	#int_mask,10(a7)	// intmaske setzen
		movem.l	(a7),d0/a5			// register zurück
		addq.l	#8,a7
		rte							// und weg 
		.endm
/*********************************************************/
.text
_vec_init:
 		mov3q.l	#-1,_rt_mod			// rt_mod auf super
 		clr.l	_rt_ssp
 		clr.l	_rt_usp
 		clr.l	_rt_vbr
		move.l	#___RAMBAR0,d0		// sind in rambar0
		movec	d0,VBR
		move.l	d0,a0
		move.l	a0,a2
init_vec:
		move.l	#256,d0
		lea		std_exc_vec(pc),a1		// standard vector
init_vec_loop:
		move.l	a1,(a2)+			// mal standard vector für alle setzen
		subq.l	#1,d0
		bne		init_vec_loop

		move.l	#___SUP_SP,(a0)
		lea		reset_vector(pc),a1
		move.l	a1,0x04(a0)
		lea		acess(pc),a1
		move.l	a1,0x08(a0)
		
		move.b	DIP_SWITCHa,d0		// ++ vr
		btst	#7,d0 
		beq		no_protect_vectors
		
		lea		_illegal_instruction(pc),a1
		move.l	a1,0x0c(a0)
		lea		_illegal_instruction(pc),a1
		move.l	a1,0x10(a0)
		lea		zero_divide(pc),a1
		move.l	a1,0x14(a0)
		lea		_privileg_violation(pc),a1
		move.l	a1,0x20(a0)
		lea		linea(pc),a1
  		move.l	a1,0x28(a0)
		lea		linef(pc),a1
  		move.l	a1,0x2c(a0)
		lea		format(pc),a1
 		move.l	a1,0x38(a0)
 		
 // floating point overflow
 		lea		flpoow(pc),a1
  		move.l	a1,0xc0(a0)
 		lea		flpoow(pc),a1
  		move.l	a1,0xc4(a0)
 		lea		flpoow(pc),a1
  		move.l	a1,0xc8(a0)
 		lea		flpoow(pc),a1
  		move.l	a1,0xcc(a0)
 		lea		flpoow(pc),a1
  		move.l	a1,0xd0(a0)
 		lea		flpoow(pc),a1
  		move.l	a1,0xd4(a0)
 		lea		flpoow(pc),a1
  		move.l	a1,0xd8(a0)
 		lea		flpoow(pc),a1
  		move.l	a1,0xdc(a0)
no_protect_vectors:
		

// int 1-7
		lea		irq1(pc),a1
		move.l	a1,0x104(a0)
		lea		irq2(pc),a1
		move.l	a1,0x108(a0)
		lea		irq3(pc),a1
		move.l	a1,0x10c(a0)
		lea		irq4(pc),a1
		move.l	a1,0x110(a0)
		lea		irq5(pc),a1
		move.l	a1,0x114(a0)
		lea		irq6(pc),a1
		move.l	a1,0x118(a0)
		lea		irq7(pc),a1
		move.l	a1,0x11c(a0)
//psc_vectors
		lea		psc3(pc),a1
		move.l	a1,0x180(a0)
//timer 1 vectors
		lea		timer0(pc),a1
		move.l	a1,0x1f8(a0)
		rts
/********************************************************/
/*   exception vector routinen
/********************************************************/
vector_table_start:
std_exc_vec:
		move.w	#0x2700,sr			// disable interrupt
		subq.l	#8,a7
		movem.l	d0/a5,(a7)			// register sichern
// test auf protect mode -------------------------------
		move.b	DIP_SWITCHa,d0
		btst	#7,d0 
		bne		stv_protect			// ja->
//------------------------------------------------------
		move.w	8(a7),d0			// vector holen
		and.l	#0x3fc,d0			// vector nummer ausmaskieren
		add.l	_rt_vbr,d0			// + basis
		move.l	d0,a5
		move.l	(a5),d0
		move.l	4(a7),a5			// a5 zurück
		move.l	d0,4(a7)
		move.w	10(a7),d0
		bset	#13,d0				// super
		move.w	d0,sr				// orginal sr wert in super setzen
		move.l	(a7)+,d0			// d0 zurück
		rts
stv_protect:
		move.l	usp,a5				// usp holen  
		tst.b	_rt_mod				// supervisor? 
		bne		sev_sup				// ja -> 
		mov3q.l	#-1,_rt_mod			// auf supervisor setzen
		move.l	a5,_rt_usp			// rt_usp speichern 
		move.l	_rt_ssp,a5			// rt_ssp holen
#ifdef cf_stack
		move.l	12(a7),-(a5)		// pc transferieren 
		move.l	8(a7),d0			// sr holen
		move.l	d0,-(a5)			// sr transferieren 
		swap	d0					// vec -> lw
#else
		move.w	8(a7),d0			// vector holen
		move.w	d0,-(a5)			// ablegen
		move.l	12(a7),-(a5)		// pc transferieren 
		move.w	10(a7),-(a5)		// sr transferieren
#endif
		move.l	a5,usp				// usp setzen
		and.l	#0x3fc,d0			// vector nummer ausmaskieren
		add.l	_rt_vbr,d0			// + basis
		move.l	d0,a5
		move.l	(a5),12(a7)			// hier geht's weiter
		movem.l	(a7),d0/a5			// register zurück
		addq.l	#8,a7
		rte							// und weg 
sev_sup:
#ifdef cf_stack
		move.l	12(a7),-(a5)		// pc transferieren 
		move.l	8(a7),d0			// sr holen
		bset	#13,d0				// war aus rt super
		move.l	d0,-(a5)			// sr transferieren 
		swap	d0					// vec -> lw
#else
		move.w	8(a7),d0			// vector holen
		move.w	d0,-(a5)			// ablegen
		move.l	12(a7),-(a5)		// pc transferieren 
		move.w	10(a7),-(a5)		// sr transferieren
		bset	#5,(a5)				// war aus super
#endif
		move.l	a5,usp				// usp setzen
		and.l	#0x3fc,d0			// vector nummer ausmaskieren
		add.l	_rt_vbr,d0			// + basis
		move.l	d0,a5
		move.l	(a5),12(a7)			// hier geht's weiter
		movem.l	(a7),d0/a5			// register zurück
		addq.l	#8,a7
		rte							// und weg 
//*******************************************
reset_vector:
		move.w	#0x2700,sr			// disable interrupt
		move.l	#0x31415926,d0
		cmp.l	0x426,d0			// reset vector gültg?
		beq		std_exc_vec			// ja->
		jmp		___BOOT_FLASH 		// sonst kaltstart
acess:
		move.w	#0x2700,sr			// disable interrupt
		move.l	d0,-(sp)			// ++ vr
		move.w	4(sp),d0
		andi.l	#0x0c03,d0
		cmpi.l	#0x0401,d0
		beq		access_mmu
		cmpi.l	#0x0402,d0
		beq		access_mmu
		cmpi.l	#0x0802,d0
		beq		access_mmu
		cmpi.l	#0x0c02,d0
		beq		access_mmu
		bra		bus_error
access_mmu:
		move.l	MCF_MMU_MMUSR,d0
		btst	#1,d0
		bne		bus_error
		move.l	MCF_MMU_MMUAR,d0
		cmp.l	#___Bas_base,d0		// max User RAM Bereich
		bge		bus_error			// grösser -> bus error
		bra		_mmutr_miss
bus_error:
		move.l	(sp)+,d0
		bra		std_exc_vec
		
zero_divide:
		move.w	#0x2700,sr			// disable interrupt
		move.l	a0,-(a7)
		move.l	d0,-(a7)
		move.l	12(a7),a0			// pc
		move.w	(a0)+,d0			// befehlscode
		btst	#7,d0				// long?
		beq		zd_word				// nein->
		addq.l	#2,a0
zd_word:
		and.l	0x3f,d0				// ea ausmaskieren
		cmp.w	#0x08,d0			// -(ax) oder weniger
		ble		zd_end
		addq.l	#2,a0
		cmp.w	#0x39,d0			// xxx.L
		bne		zd_nal
		addq.l	#2,a0
		bra		zd_end
zd_nal:	cmp.w	#0x3c,d0			// immediate?
		bne		zd_end				// nein->
		btst	#7,d0				// long?
		beq		zd_end				// nein
		addq.l	#2,a0
zd_end:
		move.l	a0,12(a7)
		move.l	(a7)+,d0
		move.l	(a7)+,a0		
		rte
		
linea:	
		move.w	#0x2700,sr			// disable interrupt
		halt
		nop
		nop
linef:	
		move.w	#0x2700,sr			// disable interrupt
		halt
		nop
		nop
format:
		move.w	#0x2700,sr			// disable interrupt
		halt
		nop
		nop
//floating point		
flpoow:
		move.w	#0x2700,sr			// disable interrupt
		halt
		nop
		nop
irq1:
		irq		0x64,1,0x02
irq2:				// hbl
								//		move.b	#3,2(a7)
								//		rte
		irq		0x68,2,0x04
irq3:
		irq		0x6c,3,0x08
irq4:				// vbl
		irq		0x70,4,0x10
irq5:				// acp
		irq		0x74,5,0x20
irq6:				// mfp
		move.w	#0x2700,sr			// disable interrupt
		subq.l	#8,a7
		movem.l	d0/a5,(a7)			// register sichern
		lea		MCF_EPORT_EPFR,a5
		move.b	#0x40,(a5)			// clear int6
// test auf timeout screen adr change -------------------------------------------------------
		move.l	_video_sbt,d0
  		beq		irq6_non_sca		// wenn 0 nichts zu tun
  		sub.l	#0x70000000,d0		// 14 sec abzählen
		lea		MCF_SLT0_SCNT,a5
  		cmp.l	(a5),d0				// aktuelle zeit weg
  		ble		irq6_non_sca		// noch nicht abgelaufen
  		lea		-28(a7),a7
  		movem.l	d0-d4/a0-a1,(a7)	// register sichern
  		clr.l	d3					// beginn mit 0
		bsr		cpusha				// cache leeren
 // eintrag suchen
 irq6_next_sca:
 		move.l	d3,d0
		move.l	d0,MCF_MMU_MMUAR	// addresse
 		move.l	#0x106,d4
 		move.l	d4,MCF_MMU_MMUOR	// suchen -> 
 		nop
 		move.l	MCF_MMU_MMUOR,d4
 		clr.w	d4
 		swap	d4
 		move.l	d4,MCF_MMU_MMUAR
		mvz.w	#0x10e,d4
 		move.l	d4,MCF_MMU_MMUOR	// einträge holen aus mmu
 		nop
 		move.l	MCF_MMU_MMUTR,d4	// ID holen
 		lsr.l	#2,d4				// bit 9 bis 2
 		cmp.w	#sca_page_ID,d4		// ist screen change ID?
 		bne		irq6_sca_pn			// nein -> page keine screen area next
// eintrag ändern 		
		add.l	#std_mmutr,d0
		move.l	d3,d1				// page 0?
		beq		irq6_sca_pn0		// ja ->
		add.l	#cb_mmudr,d1		// sonst page cb
		bra		irq6_sca_pn1c
irq6_sca_pn0:
		add.l	#wt_mmudr|MCF_MMU_MMUDR_LK,d1	// page wt and locked
irq6_sca_pn1c:
		mvz.w	#0x10b,d2			// MMU update 
		move.l	d0,MCF_MMU_MMUTR	
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR	// setze tlb data only  
		nop
// page copy
		move.l	d3,a0
		add.l	#0x60000000,a0
		move.l	d3,a1
		move.l	#0x10000,d4			// die ganze page
irq6_vcd0_loop:						
		move.l	(a0)+,(a1)+			// page copy
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		subq.l	#1,d4
		bne		irq6_vcd0_loop
		nop
irq6_sca_pn:
		add.l	#0x00100000,d3		// next
		cmp.l	#0x00d00000,d3		// ende?
		blt		irq6_next_sca		// nein->

		move.l	#0x2000,d0
		move.l	d0,_video_tlb		// anfangszustand wieder herstellen
  		clr.l	_video_sbt			// zeit löschen

  		movem.l	(a7),d0-d4/a0-a1	// register zurück
  		lea		28(a7),a7
irq6_non_sca:
// test auf acsi dma -----------------------------------------------------------------
		lea		0xfffffa0b,a5
 		bset	#7,-4(a5)			// int ena
		btst.b	#7,(a5)				// acsi dma int?
		beq		non_acsi_dma
		bsr		acsi_dma
non_acsi_dma:
// ----------------------------------------------------------------------------------
		tst.b	(a5)
		bne		irq6_1
		tst.b	2(a5)
		bne		irq6_1
		movem.l	(a7),d0/a5
		addq.l	#8,a7
		rte	
irq6_1:
		lea		MCF_GPIO_PODR_FEC1L,a5
		bclr.b	#4,(a5)					// led on
		lea		blinker(pc),a5
		addq.l	#1,(a5)					// +1
		move.l	(a5),d0
		and.l	#0x80,d0
		bne		irq6_2
		lea		MCF_GPIO_PODR_FEC1L,a5
		bset.b	#4,(a5)					// led off
irq6_2:
// test auf protect mode ---------------------
		move.b	DIP_SWITCHa,d0
		btst	#7,d0 
		bne		irq6_3				// ja->
// -------------------------------------------		
		move.l	0xF0020000,a5		// vector holen
		add.l	_rt_vbr,a5			// basis
		move.l	(a5),d0				// vector holen
		move.l	4(a7),a5			// a5 zurück
		move.l	d0,4(a7)			// vector eintragen
		move.l	(a7)+,d0			// d0 zurück
		move	#0x2600,sr
		rts
irq6_3:
		move.l	usp,a5				// usp holen  
		tst.b	_rt_mod				// supervisor? 
		bne		sev_sup6			// ja -> 
		mov3q.l	#-1,_rt_mod			// auf supervisor setzen
		move.l	a5,_rt_usp			// rt_usp speichern 
		move.l	_rt_ssp,a5			// rt_ssp holen
#ifdef cf_stack
		move.l	12(a7),-(a5)		// pc transferieren 
		move.l	8(a7),-(a5)			// sr transferieren 
#else
		move.w	8(a7),-(a5)			// vector transferieren
		move.l	12(a7),-(a5)		// pc transferieren 
		move.w	10(a7),-(a5)		// sr transferieren
#endif
		move.l	a5,usp				// usp setzen
		move.l  0xF0020000,a5		// vector holen: intack routine
		add.l	_rt_vbr,a5			// virtuelle VBR des Systems
		move.l	(a5),12(a7)			// hier gehts weiter
		movem.l	(a7),d0/a5			// register zurück
		addq.l	#8,a7
		move.b	#6,2(a7)			// intmaske setzen
		rte 						// und weg 
sev_sup6:
#ifdef cf_stack
		move.l	12(a7),-(a5)		// pc transferieren 
		move.l	8(a7),-(a5)			// sr,vec
		bset	#5,2(a5)			// auf super setzen
#else
		move.w	8(a7),-(a5)			// vector nr.
		move.l	12(a7),-(a5)		// pc verschieben
		move.w	10(a7),-(a5)		// sr verschieben
		bset	#5,(a5)				// auf super
#endif
		move.l	a5,usp				// usp setzen
		move.l  0xF0020000,a5		// vector holen: intack routine
		add.l	_rt_vbr,a5			// virtuelle VBR des Systems
		move.l	(a5),12(a7)			// hier gehts weiter
		movem.l	(a7),d0/a5			// register zurück
		rts
blinker:.long	0
/**************************************************/
/* pseudo dma */
/**************************************************/
acsi_dma:				// atari dma
		move.l  a1,-(a7)
		move.l  d1,-(a7)
		
	lea	MCF_PSC0_PSCTB_8BIT,a1		// ++ vr
	move.l	#'DMA ',(a1)
	move.l	#'INT!',(a1)

		lea 	0xf0020110,a5		// fifo daten
acsi_dma_start:
		move.l	-12(a5),a1			// dma adresse
		move.l	-8(a5),d0			// byt counter
     	ble		acsi_dma_end
     	btst.b	#0,-16(a5)			// write? (dma modus reg)
		bne		acsi_dma_wl			// ja->
acsi_dma_rl:
 		tst.b	-4(a5) 				// dma req?
  		bpl		acsi_dma_fertig		// nein->
		move.l	(a5),(a1)+			// read 4 bytes	 
		move.l	(a5),(a1)+			// read 4 bytes	 
		move.l	(a5),(a1)+			// read 4 bytes	 
		move.l	(a5),(a1)+			// read 4 bytes	
		
 		moveq	#'.',d1
 		move.b	d1,MCF_PSC0_PSCTB_8BIT

		sub.l	#16,d0				// byt counter -16
 		bpl		acsi_dma_rl
		bra		acsi_dma_fertig	
acsi_dma_wl:
 		tst.b	-4(a5) 				// dma req?
  		bpl		acsi_dma_fertig		// nein->
		move.l	(a1)+,(a5)			// write 4 byts
		move.l	(a1)+,(a5)			// write 4 byts
		move.l	(a1)+,(a5)			// write 4 byts
		move.l	(a1)+,(a5)			// write 4 byts
		
 		moveq	#'.',d1
 		move.b	d1,MCF_PSC0_PSCTB_8BIT

		sub.l	#16,d0				// byt counter -16
		bpl		acsi_dma_wl
acsi_dma_fertig:
		move.l	a1,-12(a5)			// adresse zurück
		move.l	d0,-8(a5)			// byt counter zurück
acsi_dma_end:
		tst.b	-4(a5) 				// dma req?
 		bmi		acsi_dma_start		// ja->
		lea		0xfffffa0b,a5
		bclr.b	#7,4(a5)			// clear int in service mfp
		bclr.b	#7,(a5)				// clear int pending mfp 0xfffffa0b
	
		move.w	#0x0d0a,d1
		move.w	d1,MCF_PSC0_PSCTB_8BIT
		
		move.l	(a7)+,d1
		move.l	(a7)+,a1
		rts
/**************************************************/
/* irq 7 = pseudo bus error */
/**************************************************/
irq7:
		lea			-12(sp),sp
		movem.l		d0/a0,(sp)

		move.l		___RAMBAR0+0x008,a0	// Real Access Error handler
		move.l		a0,8(sp)			// This will be the return address for rts

		move.w		12(sp),d0			// Format/Vector word
		andi.l		#0xf000,d0			// Keep only the Format
		ori.l		#2*4,d0				// Simulate Vector #2, no Fault
		move.w		d0,12(sp)

		// TODO: Inside an interrupt handler, 16(sp) is the return address.
		// For an Access Error, it should be the address of the fault instruction instead

		lea			MCF_EPORT_EPFR,a0
		move.b		#0x80,(a0)			// clear int7
		move.l		(sp)+,d0
		move.l		(sp)+,a0
		rts								// Forward to the Access Error handler

/**************************************************/
/* psc3 com PIC MCF */
/**************************************************/
psc3:
		move.w	#0x2700,sr			// disable interrupt
		lea		-20(a7),a7
		movem.l	d0-d2/a0/a3,(a7)	
		lea		MCF_PSC3_PSCRB_8BIT,a3
		move.b	(a3),d1
		cmp.b	#2,d1				// anforderung rtc daten?
		bne		psc3_fertig

	lea	MCF_PSC0_PSCTB_8BIT,a0		// ++ vr
	move.l	#'PIC ',(a0)
	move.l	#'INT ',(a0)
	move.l	#'RTC!',(a0)
	move.l	#0x0d0a,(a0)

		lea		0xffff8961,a0
		lea		MCF_PSC3_PSCTB_8BIT,a3
		clr.l	d1	
		moveq	#64,d2
		move.b	#0x82,(a3)			// header: rtcd mcf->pic
loop_sr2:
		move.b  d1,(a0)
		move.b	2(a0),d0
		move.b	d0,(a3)
		addq.l	#1,d1
		cmp.b	d1,d2
		bne		loop_sr2
psc3_fertig:
		movem.l	(a7),d0-d2/a0/a3	// register zurück
		lea		20(a7),a7
		RTE
/**************************************************/
/* timer 0: video change later also others
/**************************************************/
timer0:
		move	#0x2700,sr
//		halt
		lea		-28(a7),a7
		movem.l	d0-d4/a0-a1,(a7)
		mvz.b	0xffff8201,d0		// löschen und high byt
		cmp.w	#2,d0
		blt		video_chg_end
		cmp.w	#0xd0,d0			// normale addresse
		blt		sca_other			// nein->
		lea		MCF_SLT0_SCNT,a0
  		move.l	(a0),d4
		move.l	d4,_video_sbt		// time sichern
sca_other:
		lsl.l	#8,d0				
		move.b	0xffff8203,d0		// mid byt
		lsl.l	#8,d0
		move.b  0xffff820d,d0		// low byt
		move.l	d0,d3
video_chg_1page:
// test ob page schon gesetzt
		moveq	#20,d4
		move.l	d0,d2
		lsr.l	d4,d2				// neue page
		move.l	_video_tlb,d4
		bset.l	d2,d4				// setzen als geändert
		bne		video_chg_2page		// schon gesetzt gewesen? ja->weg
		move.l	d4,_video_tlb
		bsr		cpusha				// cache leeren
// daten copieren 
video_copy_data:
		move.l	d4,_video_tlb
		and.l	#0x00f00000,d0
		move.l	d0,a0
		move.l	a0,a1
		add.l	#0x60000000,a1
		move.l	#0x10000,d4			// die ganze page
video_copy_data_loop:
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		subq.l	#1,d4
		bne		video_copy_data_loop
// eintrag suchen
		move.l	d0,MCF_MMU_MMUAR	// addresse
 		move.l	#0x106,d4
 		move.l	d4,MCF_MMU_MMUOR	// suchen -> schlägt neuen vor wenn keiner
 		nop
 		move.l	MCF_MMU_MMUOR,d4
 		clr.w	d4
 		swap	d4
 		move.l	d4,MCF_MMU_MMUAR
		move.l	d0,d1
		add.l	#MCF_MMU_MMUTR_ID(sca_page_ID)|std_mmutr,d0
		add.l	#0x60000000|wt_mmudr|MCF_MMU_MMUDR_LK,d1
		mvz.w	#0x10b,d2								// MMU update 
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR					// setzen vidoe maped to 60xxx only data  
		nop
video_chg_2page:
// test ob evt. anschliessende page gesetzt werden muss
		move.l	d3,d0
		mvz.w	0xffff8210,d4		// byts pro zeile
		mvz.w	0xffff82aa,d2		// zeilen ende
		mvz.w	0xffff82a8,d1		// zeilenstart
		sub.l	d1,d2				// differenz = anzahl zeilen
		mulu	d2,d4				// maximal 480 zeilen
		add.l	d4,d0				// video grösse
		cmp.l	#0xe00000,d0		// maximale addresse
		bge		video_chg_end		// wenn gleich oder grösser -> fertig
		moveq	#20,d4
		move.l	d0,d2
		lsr.l	d4,d2				// neue page
		move.l	_video_tlb,d4
		bset.l	d2,d4				// setzen als geändert
		beq		video_copy_data		// nein nochmal
video_chg_end:
// int pending löschen
		lea		MCF_GPT0_GMS,a0
		bclr.b	#0,3(a0)
		nop
		bset.b	#0,3(a0)

		movem.l	(a7),d0-d4/a0-a1
		lea		28(a7),a7
//--------------------------------------------------------------------------------------------------------
		RTE

