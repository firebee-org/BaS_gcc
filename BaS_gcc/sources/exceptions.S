/*
 * initialize exception vectors
 */
#include "startcf.h"

		.extern __Bas_base
		.extern __SUP_SP
		.extern _rom_entry
		.extern __RAMBAR0
		.extern _rt_cacr
		.extern _rt_mod
		.extern _rt_ssp
		.extern _rt_usp
		.extern _rt_vbr
		.extern _illegal_instruction
		.extern _privileg_violation
		.extern _mmutr_miss
		.extern __MBAR
		.extern __MMUBAR
		.extern _video_tlb
		.extern _video_sbt
		.extern cpusha
		.extern _xhdi_sd_install	/* trap #0 exception vector for installation of xhdi SD card driver */

/* Register read/write macros */
#define MCF_MMU_MMUCR            			__MMUBAR
#define MCF_MMU_MMUOR                       __MMUBAR+0x04
#define MCF_MMU_MMUSR                       __MMUBAR+0x08
#define MCF_MMU_MMUAR                       __MMUBAR+0x10
#define MCF_MMU_MMUTR                       __MMUBAR+0x14
#define MCF_MMU_MMUDR                       __MMUBAR+0x18

#define MCF_EPORT_EPPAR                     __MBAR+0xF00
#define MCF_EPORT_EPDDR                     __MBAR+0xF04
#define MCF_EPORT_EPIER                     __MBAR+0xF05
#define MCF_EPORT_EPDR                      __MBAR+0xF08
#define MCF_EPORT_EPPDR                     __MBAR+0xF09
#define MCF_EPORT_EPFR                      __MBAR+0xF0C

#define MCF_GPIO_PODR_FEC1L                 __MBAR+0xA07

#define MCF_PSC0_PSCTB_8BIT                 __MBAR+0x860C

#define MCF_PSC3_PSCRB_8BIT                 __MBAR+0x890C
#define MCF_PSC3_PSCTB_8BIT                 __MBAR+0x890C

		.global	_vec_init

		// interrupt sources
		.equ	INT_SOURCE_EPORT_EPF1,1		// edge port flag 1
		.equ	INT_SOURCE_EPORT_EPF2,2		// edge port flag 2
		.equ	INT_SOURCE_EPORT_EPF3,3		// edge port flag 3
		.equ	INT_SOURCE_EPORT_EPF4,4		// edge port flag 4
		.equ	INT_SOURCE_EPORT_EPF5,5		// edge port flag 5
		.equ	INT_SOURCE_EPORT_EPF6,6		// edge port flag 6
		.equ	INT_SOURCE_EPORT_EPF7,7		// edge port flag 7
		.equ	INT_SOURCE_USB_EP0ISR,15	// USB endpoint 0 interrupt
		.equ	INT_SOURCE_USB_EP1ISR,16	// USB endpoint 1 interrupt
		.equ	INT_SOURCE_USB_EP2ISR,17	// USB endpoint 2 interrupt
		.equ	INT_SOURCE_USB_EP3ISR,18	// USB endpoint 3 interrupt
		.equ	INT_SOURCE_USB_EP4ISR,19	// USB endpoint 4 interrupt
		.equ	INT_SOURCE_USB_EP5ISR,20	// USB endpoint 5 interrupt
		.equ	INT_SOURCE_USB_EP6ISR,21	// USB endpoint 6 interrupt
		.equ	INT_SOURCE_USB_USBISR,22	// USB general interrupt
		.equ	INT_SOURCE_USB_USBAISR,23	// USB core interrupt
		.equ	INT_SOURCE_USB_ANY,24		// OR of all USB interrupts
		.equ	INT_SOURCE_USB_DSPI_OVF,25	// DSPI overflow or underflow
		.equ	INT_SOURCE_USB_DSPI_RFOF,26	// receive FIFO overflow interrupt
		.equ	INT_SOURCE_USB_DSPI_RFDF,27	// receive FIFO drain interrupt
		.equ	INT_SOURCE_USB_DSPI_TFUF,28	// transmit FIFO underflow interrupt
		.equ	INT_SOURCE_USB_DSPI_TCF,29	// transfer complete interrupt
		.equ	INT_SOURCE_USB_DSPI_TFFF,30	// transfer FIFO fill interrupt
		.equ	INT_SOURCE_USB_DSPI_EOQF,31	// end of queue interrupt
		.equ	INT_SOURCE_PSC3,32			// PSC3 interrupt
		.equ	INT_SOURCE_PSC2,33			// PSC2 interrupt
		.equ	INT_SOURCE_PSC1,34			// PSC1 interrupt
		.equ	INT_SOURCE_PSC0,35			// PSC0 interrupt
		.equ	INT_SOURCE_CTIMERS,36		// combined source for comm timers
		.equ	INT_SOURCE_SEC,37			// SEC interrupt
		.equ	INT_SOURCE_FEC1,38			// FEC1 interrupt
		.equ	INT_SOURCE_FEC0,39			// FEC0 interrupt
		.equ	INT_SOURCE_I2C,40			// I2C interrupt
		.equ	INT_SOURCE_PCIARB,41		// PCI arbiter interrupt
		.equ	INT_SOURCE_CBPCI,42			// COMM bus PCI interrupt
		.equ	INT_SOURCE_XLBPCI,43		// XLB PCI interrupt
		.equ	INT_SOURCE_XLBARB,47		// XLBARB to PCI interrupt
		.equ	INT_SOURCE_DMA,48			// multichannel DMA interrupt
		.equ	INT_SOURCE_CAN0_ERROR,49	// FlexCAN error interrupt
		.equ	INT_SOURCE_CAN0_BUSOFF,50	// FlexCAN bus off interrupt
		.equ	INT_SOURCE_CAN0_MBOR,51		// message buffer ORed interrupt
		.equ	INT_SOURCE_SLT1,53			// slice timer 1 interrupt
		.equ	INT_SOURCE_SLT0,54			// slice timer 0 interrupt
		.equ	INT_SOURCE_CAN1_ERROR,55	// FlexCAN error interrupt
		.equ	INT_SOURCE_CAN1_BUSOFF,56	// FlexCAN bus off interrupt
		.equ	INT_SOURCE_CAN1_MBOR,57		// message buffer ORed interrupt
		.equ	INT_SOURCE_GPT3,59			// GPT3 timer interrupt
		.equ	INT_SOURCE_GPT2,60			// GPT2 timer interrupt
		.equ	INT_SOURCE_GPT1,61			// GPT1 timer interrupt
		.equ	INT_SOURCE_GPT0,62			// GPT0 timer interrupt


//mmu ---------------------------------------------------
/* Register read/write macros */
#define MCF_MMU_MMUCR            			__MMUBAR
#define MCF_MMU_MMUOR                       __MMUBAR+0x04
#define MCF_MMU_MMUSR                       __MMUBAR+0x08
#define MCF_MMU_MMUAR                       __MMUBAR+0x10
#define MCF_MMU_MMUTR                       __MMUBAR+0x14
#define MCF_MMU_MMUDR                       __MMUBAR+0x18


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
#define	 writethrough_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(00)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
#define	 copyback_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(01)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
#define	 nocache_precise_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(10)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
//---------------------------------------------------
/*********************************************************************
*
* General Purpose Timers (GPT)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_GPT0_GMS                         __MBAR+0x800

/*********************************************************************
*
* Slice Timers (SLT)
*
*********************************************************************/

#define MCF_SLT0_SCNT                        __MBAR+0x908

/**********************************************************/
// macros
/**********************************************************/
		.altmacro
		.macro	irq	vector,int_mask,clr_int
		local	irq_protect
		local 	sev_supint
		local	irq_end

		move.w	#0x2700,sr			// disable interrupt
		subq.l	#8,a7
		movem.l	d0/a5,(a7)			// save registers
		lea		MCF_EPORT_EPFR,a5
		move.b	#\clr_int,(a5)		// clear int pending
// test auf protect mode ---------------------
		move.b	DIP_SWITCHa,d0
		btst	#7,d0 
		bne		irq_protect			// ja->
// -------------------------------------------		
		movem.l	(a7),d0/a5			// restore registers
		addq.l	#8,a7
		move.l	\vector,-(a7)
		move	#0x2\int_mask\()00,sr
		rts
irq_protect:
		move.l	usp,a5				// get usp
		tst.b	_rt_mod				// supervisor mode active?
		bne		sev_supint			// yes ->
		mov3q.l	#-1,_rt_mod			// enable supervisor mode
		move.l	a5,_rt_usp			// save rt_usp
		move.l	_rt_ssp,a5			// get rt_ssp
#ifdef cf_stack
		move.l	12(a7),-(a5)		// transfer pc
		move.l	8(a7),-(a5)			// sr,vec
#else
		move.w	8(a7),-(a5)			// vector no
		move.l	12(a7),-(a5)		// pc verschieben
		move.w	10(a7),-(a5)		// sr verschieben
#endif
		bra		irq_end
sev_supint:
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
irq_end:
		move.l	a5,usp				// usp setzen
		lea		\vector,a5
		adda.l	_rt_vbr,a5
		move.l	(a5),12(a7)			// vectoradresse eintragen
		move.b	#\int_mask,10(a7)	// intmaske setzen
		movem.l	(a7),d0/a5			// register zur�ck
		addq.l	#8,a7
		rte							// und weg 
		.endm

/*
 * FIXME: this is a GNU gas kludge. Ugly, but I just can't come up with any smarter solution
 *
 * GNU as does not support multi-character constants. At least I don't know of any way it would.
 * The following might look more than strange, but I considered the statement
 *
 * mchar	move.l, 'T,'E,'S,'T,-(SP)
 *
 * somewhat more readable than
 *
 * move.l #1413829460,-(SP)
 *
 * If anybody knows of any better way on how to do this - please do!
 *
 */
    .macro mchar  st,a,b,c,d,tgt
    \st #\a << 24|\b<<16|\c<<8|\d,\tgt
    .endm

.text
_vec_init:
		move.l	a2,-(sp)			// Backup registers

 		mov3q.l	#-1,_rt_mod			// rt_mod auf super
 		clr.l	_rt_ssp
 		clr.l	_rt_usp
 		clr.l	_rt_vbr
		move.l	#__RAMBAR0,d0		// exception vectors reside in rambar0
		movec	d0,VBR
		move.l	d0,a0
		move.l	a0,a2
init_vec:
		move.l	#256,d0
		lea		std_exc_vec(pc),a1	// standard vector
init_vec_loop:
		move.l	a1,(a2)+			// set standard vector for all exceptions
		subq.l	#1,d0
		bne		init_vec_loop

		move.l	#__SUP_SP,(a0)
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
		
		// trap #0 (without any parameters for now) is used to provide BaS' XHDI
		// routine address to EmuTOS.
		lea		_xhdi_sd_install,a1
		move.l	a1,0x80(a0)				// trap #0 exception vector

// ACP interrupts 1-7 (user-defined, generated by FPGA)
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
// PSC vectors
		lea		handler_psc3(pc),a1
		// PSC3 interrupt source = 32
		move.l	a1,(INT_SOURCE_PSC3 + 64) * 4(a0)
// timer vectors
		lea		handler_gpt0(pc),a1
		// GPT0 interrupt source = 62
		move.l	a1,(INT_SOURCE_GPT0 + 64) * 4(a0)

		move.l	(sp)+,a2			// Restore registers
		rts
/*
 * exception vector routines
 */
vector_table_start:
std_exc_vec:
		move.w	#0x2700,sr			// disable interrupt
		subq.l	#8,a7
		movem.l	d0/a5,(sp)			// save registers

		move.b	DIP_SWITCHa,d0		// "protect mode"?
		btst	#7,d0 
		bne		stv_protect			// yes

		move.w	8(sp),d0			// fetch vector
		and.l	#0x3fc,d0			// mask out vector number
		add.l	_rt_vbr,d0			// + OS VBR
		move.l	d0,a5				
		move.l	(a5),d0				// fetch exception routine address

		move.l	4(sp),a5			// restore a5
		move.l	d0,4(a7)			// store exception routine address

		move.w	10(a7),d0			// restore original SR
		bset	#13,d0				// set supervisor bit
		move.w	d0,sr				//
		move.l	(a7)+,d0			// restore d0
		rts							// jump to exception routine

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
		movem.l	(a7),d0/a5			// register zur�ck
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
		cmp.l	0x426,d0			// _resvalid: reset vector valid?
		beq		std_exc_vec			// yes->
		jmp		_rom_entry	 		// no, cold start machine

acess:
		move.w	#0x2700,sr			// disable interrupt
		move.l	d0,-(sp)			// ++ vr
		move.w	4(sp),d0
		andi.l	#0x0c03,d0			// mask out fault status bits
		cmpi.l	#0x0401,d0			// TLB miss on opword of instruction fetch?
		beq		access_mmu			// yes
		cmpi.l	#0x0402,d0			// TLB miss on extension word of instruction fetch?
		beq		access_mmu			// yes
		cmpi.l	#0x0802,d0			// TLB miss on data write?
		beq		access_mmu			// yes
		cmpi.l	#0x0c02,d0			// TLB miss on data read, or read-modify-write?
		beq		access_mmu			// yes
		bra		bus_error			// everything else

access_mmu:
		move.l	MCF_MMU_MMUSR,d0	// did the last fault hit in TLB?
		btst	#1,d0				// no
		bne		bus_error			// bus error handler
		move.l	MCF_MMU_MMUAR,d0
		cmp.l	#__FASTRAM_END,d0	// above max User RAM area?
		bge		bus_error			// -> bus error
		bra		_mmutr_miss			// else we have an MMU TLB miss

bus_error:
		move.l	(sp)+,d0			// restore register
		bra		std_exc_vec
		
zero_divide:
		move.w	#0x2700,sr			// disable interrupt
		move.l	a0,-(a7)
		move.l	d0,-(a7)
		move.l	12(a7),a0			// pc
		move.w	(a0)+,d0			// command word
		btst	#7,d0				// long?
		beq		zd_word				// nein->
		addq.l	#2,a0

zd_word:
		and.l	0x3f,d0				// mask out ea field
		cmp.w	#0x08,d0			// -(ax) or less?
		ble		zd_end
		addq.l	#2,a0
		cmp.w	#0x39,d0			// xxx.L
		bne		zd_nal
		addq.l	#2,a0
		bra		zd_end

zd_nal:	cmp.w	#0x3c,d0			// immediate?
		bne		zd_end				// no->
		btst	#7,d0				// long?
		beq		zd_end				// no
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
		movem.l	d0/a5,(a7)			// save registers
		lea		MCF_EPORT_EPFR,a5
		move.b	#0x40,(a5)			// clear int6

// screen adr change timed out?
		move.l	_video_sbt,d0
  		beq		irq6_non_sca		// nothing to do if 0
  		sub.l	#0x70000000,d0		// substract 14 seconds
		lea		MCF_SLT0_SCNT,a5
  		cmp.l	(a5),d0				// time reached?
  		ble		irq6_non_sca		// not yet

  		lea		-28(a7),a7			// save more registers
  		movem.l	d0-d4/a0-a1,(a7)	//
  		clr.l	d3					// beginn mit 0
		bsr		cpusha				// clear caches

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
// eintrag �ndern
		add.l	#std_mmutr,d0
		move.l	d3,d1				// page 0?
		beq		irq6_sca_pn0		// ja ->
		add.l	#copyback_mmudr,d1		// sonst page cb
		bra		irq6_sca_pn1c
irq6_sca_pn0:
		add.l	#writethrough_mmudr|MCF_MMU_MMUDR_LK,d1	// page wt and locked
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
		lea		blinker,a5
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

		.data
blinker:.long	0


		.text

/*
 * pseudo dma
 */
acsi_dma:							// atari dma
		move.l  a1,-(a7)
		move.l  d1,-(a7)
		
		lea	MCF_PSC0_PSCTB_8BIT,a1	// ++ vr
		mchar move.l, 'D,'M','A,'\ ,(a1)
		//move.l	#"DMA ",(a1)
		mchar	move.l,'I,'N,'T,'!,(a1)
		// move.l	#'INT!',(a1)

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
		move.l	a1,-12(a5)			// adresse zur�ck
		move.l	d0,-8(a5)			// byt counter zur�ck
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
/*
 * irq 7 = pseudo bus error
 */
irq7:
		lea			-12(sp),sp
		movem.l		d0/a0,(sp)

		move.l		__RAMBAR0+0x008,a0	// Real Access Error handler
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

/*
 * psc3 com PIC MCF
 */
handler_psc3:
		move.w	#0x2700,sr			// disable interrupt
		lea		-20(a7),a7
		movem.l	d0-d2/a0/a3,(a7)	
		lea		MCF_PSC3_PSCRB_8BIT,a3
		move.b	(a3),d1
		cmp.b	#2,d1				// anforderung rtc daten?
		bne		psc3_fertig

		lea	MCF_PSC0_PSCTB_8BIT,a0		// ++ vr
		mchar	move.l,'\P,'\I,'C,' ,(a0)
		// move.l	#'PIC ',(a0)
		mchar	move.l,'I,'N,'T,'\ ,(a0)
		// move.l	#'INT ',(a0)
		mchar	move.l,'R,'T,'C,'!,(a0)
		// move.l	#'RTC!',(a0)
		mchar 	move.l,0x0d,0x0a,0,0,(a0)
		//move.l	#0x0d0a,(a0)

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
		movem.l	(a7),d0-d2/a0/a3	// restore saved registers
		lea		20(a7),a7
		RTE

/*
 * general purpose timer 0 (GPT0): video change, later also others
 */
handler_gpt0:
		move	#0x2700,sr
//		halt
		lea		-28(a7),a7
		movem.l	d0-d4/a0-a1,(a7)
		mvz.b	0xffff8201,d0		// l�schen und high byt
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
		bset.l	d2,d4				// setzen als ge�ndert
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
 		move.l	d4,MCF_MMU_MMUOR	// suchen -> schl�gt neuen vor wenn keiner
 		nop
 		move.l	MCF_MMU_MMUOR,d4
 		clr.w	d4
 		swap	d4
 		move.l	d4,MCF_MMU_MMUAR
		move.l	d0,d1
		add.l	#MCF_MMU_MMUTR_ID(sca_page_ID)|std_mmutr,d0
		add.l	#0x60000000|writethrough_mmudr|MCF_MMU_MMUDR_LK,d1
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
		add.l	d4,d0				// video gr�sse
		cmp.l	#__STRAM_END,d0		// maximale addresse
		bge		video_chg_end		// wenn gleich oder gr�sser -> fertig
		moveq	#20,d4
		move.l	d0,d2
		lsr.l	d4,d2				// neue page
		move.l	_video_tlb,d4
		bset.l	d2,d4				// setzen als ge�ndert
		beq		video_copy_data		// nein nochmal
video_chg_end:
// int pending l�schen
		lea		MCF_GPT0_GMS,a0
		bclr.b	#0,3(a0)
		nop
		bset.b	#0,3(a0)

		movem.l	(a7),d0-d4/a0-a1
		lea		28(a7),a7
//--------------------------------------------------------------------------------------------------------
		RTE

