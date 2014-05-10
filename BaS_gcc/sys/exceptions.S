/*
 * initialize exception vectors
 *
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */

#define DBG_EXC

#include "startcf.h"
#if MACHINE_FIREBEE
#include "firebee.h"
#elif MACHINE_M5484LITE
#include "m5484l.h"
#endif /* MACHINE_FIREBEE */

		.extern __SUP_SP
		.extern _rom_entry
		.extern __RAMBAR0
		.extern _rt_mod
		.extern _rt_ssp
		.extern _rt_usp
		.extern _rt_vbr
		.extern _mmutr_miss
		.extern __MBAR
		.extern __MMUBAR
		.extern _video_tlb
		.extern _video_sbt
		.extern _flush_and_invalidate_caches

		/* PCI interrupt handlers */
		.extern _irq5_handler
		.extern _irq7_handler

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

// Atari register equates (provided by FPGA)
		.equ	vbasehi, 0xffff8201

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
#define	 writethrough_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(00)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
#define	 copyback_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(01)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)

		// equates for (experimental) video page copying via Coldfire DMA
		.equ	MCD_SINGLE_DMA, 0x100
		.equ	MCD_TT_FLAGS_CW, 0x2
		.equ	MCD_TT_FLAGS_RL, 0x1
		.equ	MCD_TT_FLAGS_SP, 0x4
		.equ	DMA_ALWAYS, 0

/* Register read/write macros */
#define MCF_GPT0_GMS                         __MBAR+0x800
#define MCF_SLT0_SCNT                        __MBAR+0x908

		.altmacro
		.macro	irq	vector,int_mask,clr_int
		move.w	#0x2700,sr			// disable interrupt
		subq.l	#8,a7
		movem.l	d0/a5,(a7)			// save registers

		lea		MCF_EPORT_EPFR,a5
		move.b	#\clr_int,(a5)		// clear int pending

		movem.l	(a7),d0/a5			// restore registers
		addq.l	#8,a7
		move.l	\vector,-(a7)
		move	#0x2\int_mask\()00,sr
		rts
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

		move.l	#__SUP_SP,(a0)		// set initial stack pointer at start of exception vector table

		lea		reset_vector(pc),a1	// set reset vector
		move.l	a1,0x04(a0)

		lea		access_exception(pc),a1		// set illegal access exception handler
		move.l	a1,0x08(a0)

		.extern	_get_bas_drivers
// trap #0 (without any parameters for now) is used to provide BaS' driver addresses to the OS
		lea		_get_bas_drivers(pc),a1
		move.l	a1,0x80(a0)				// trap #0 exception vector

#ifdef MACHINE_FIREBEE
// ACP interrupts 1-7 (user-defined, generated by FPGA on the FireBee, M5484LITE has irq7 and irq5 for PCI)
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

// install PSC vectors (used for PIC communication on the FireBee)
		lea		handler_psc3(pc),a1
		// PSC3 interrupt source = 32
		move.l	a1,(INT_SOURCE_PSC3 + 64) * 4(a0)

// timer vectors (triggers when vbashi gets changed, used for video page copy)
		lea		handler_gpt0(pc),a1
		// GPT0 interrupt source = 62
		move.l	a1,(INT_SOURCE_GPT0 + 64) * 4(a0)
#endif /* MACHINE_FIREBEE */

// install lowlevel_isr_handler for the three GPT timers
		lea		_lowlevel_isr_handler(pc),a1
		move.l	a1,(INT_SOURCE_GPT1 + 64) * 4(a0)
		move.l	a1,(INT_SOURCE_GPT2 + 64) * 4(a0)
		move.l	a1,(INT_SOURCE_GPT3 + 64) * 4(a0)

// install lowlevel_isr_handler for the FEC0  interrupt
		move.l	a1,(INT_SOURCE_FEC0 + 64) * 4(a0)

#ifndef MACHINE_FIREBEE
// FEC1 not wired on the FireBee
		move.l	a1,(INT_SOURCE_FEC1 + 64) * 4(a0)
#endif

// install lowlevel_isr_handler for DMA interrupts
		move.l	a1,(INT_SOURCE_DMA + 64) * 4(a0)

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
		move.w	8(sp),d0			// fetch vector
		and.l	#0x3fc,d0			// mask out vector number

#ifdef DBG_EXC
		// printout vector number of exception

		lea		-4 * 4(sp),sp		// reserve stack space
		movem.l	d0-d1/a0-a1,(sp)	// save gcc scratch registers

		lsr.l	#2,d0				// shift vector number in place
		cmp.l	#33,d0
		beq		noprint
		cmp.l	#34,d0
		beq		noprint
		cmp.l	#45,d0
		beq		noprint
		cmp.l	#46,d0
		beq		noprint
		move.l	4 * 4 + 8 + 4(sp),-(sp)	// pc at exception
		move.l	d0,-(sp)			// provide it to xprintf()
		pea		exception_text
		jsr		_xprintf			// call xprintf()
		add.l	#3*4,sp				// adjust stack
noprint:

		movem.l	(sp),d0-d1/a0-a1	// restore registers
		lea		4 * 4(sp),sp
#endif /* DBG_EXC */

		add.l	_rt_vbr,d0			// + VBR
		move.l	d0,a5
		move.l	(a5),d0				// fetch exception routine address

		move.l	4(sp),a5			// restore a5
		move.l	d0,4(a7)			// store exception routine address

		move.w	10(a7),d0			// restore original SR
		bset	#13,d0				// set supervisor bit
		move.w	d0,sr				//
		move.l	(a7)+,d0			// restore d0
		rts							// jump to exception routine

exception_text:
		.ascii	"DEBUG: EXCEPTION %d caught at %p"
		.byte	13, 10, 0
		.align	4

reset_vector:
		move.w	#0x2700,sr			// disable interrupt
		move.l	#0x31415926,d0
		cmp.l	0x426,d0			// _resvalid: reset vector valid?
		beq		std_exc_vec			// yes->
		jmp		_rom_entry	 		// no, cold start machine

//
// Triggered when code tries to access a memory area that is not known to the MMU yet.
// This is either a "classic" bus error or the MMU hit a "legal" page not yet mapped.
//
access_exception:
		move.w	#0x2700,sr			// avoid us being interrupted by the video handler
									// (this would probably overwrite the MMUAR register)

		// save gcc scratch registers, others will be handled by called function
		lea		-4*4(sp),sp
		movem.l	d0-d1/a0-a1,(sp)


		lea		4*4(sp),a0			// original stack pointer

		move.l	(a0),-(sp)			// format status word
		move.l	4(a0),-(sp)			// program counter at access error

		jsr		_access_exception	// note the underscore
		lea		2*4(sp),sp			// adjust stack

		tst.l	d0					// handled?

		movem.l	(sp),d0-d1/a0-a1	// restore scratch registers
		lea		4*4(sp),sp

		beq		bus_error			// no

		rte

bus_error:
		bra		std_exc_vec

zero_divide:
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

#if MACHINE_M5484LITE				// handlers for M5484LITE

irq5:								// irq5 is tied to PCI INTC# and PCI INTD# on the M5484LITE
		move.w	#0x2700,sr			// disable interrupts

		lea		-4*4(sp),sp			// save gcc scratch registers
		movem.l	d0-d1/a0-a1,(sp)

		jsr		_irq5_handler		// call C handler routine

		movem.l	(sp),d0-d1/a0-a1	// restore registers
		lea		4*4(sp),sp

		rte							// return from exception

irq5text:
		.ascii	"IRQ5!"
		.dc.b	13,10,0

irq6:
		irq		0x74,5,0x20

irq7:								// irq7 is tied to PCI INTA# and PCI INTB# on the M5484LITE

		move.w	#0x2700,sr			// disable interrupts

		lea		-4*4(sp),sp			// save gcc scratch registers
		movem.l	d0-d1/a0-a1,(sp)

		jsr		_irq7_handler		// call C handler routine

		movem.l	(sp),d0-d1/a0-a1	// restore registers
		lea		4*4(sp),sp

		rte							// return from exception

irq7text:
		.ascii	"IRQ7!"
		.dc.b	13,10,0

#elif MACHINE_FIREBEE			/* these handlers are only meaningful for the Firebee */
irq5:
		irq		0x74,5,0x20

		.extern	_irq6_interrupt_handler	// highlevel C handler

irq6:	// MFP interrupt from FPGA
		move.w	#0x2700,sr			// disable interrupts

		lea		-4 * 4(sp),sp		// save gcc scratch registers
		movem.l	d0-d1/a0-a1,(sp)

		move.l	4 * 4(sp),-(sp)		// push original exception stack frame
		move.l	5 * 4(sp),-(sp)
		jsr		_irq6_interrupt_handler		// call highlevel C handler
		lea.l	2 * 4(sp),sp

		tst.l	d0					// completely handled?

		movem.l	(sp),d0-d1/a0-a1	// restore registers saved above
		lea		4 * 4(sp),sp		// adjust stack

		bra		irq6_os				// call OS handler
		rte

irq6_os:	// call native OS irq6 handler
		move.l	a5,-(sp)			// save registers: TODO: this could be done more effective
		move.l	d0,-(sp)
		move.l	0xf0020000,a5		// fetch vector
		add.l	_rt_vbr,a5			// add vector base
		move.l	(a5),d0				// fetch handler
		move.l	4(sp),a5			// restore a5
		move.l	d0,4(sp)			// prepare indirect return
		move.l	(sp)+,d0			// restore d0
		move.w	#0x2600,sr			// set interrupt mask
		rts

#ifdef _NOT_USED_
		subq.l	#8,a7
		movem.l	d0/a5,(a7)			// save registers

		lea		MCF_EPORT_EPFR,a5	// clear int6 from edge port
		bset	#6,(a5)

		// there was a potential bug here before: would also clear all other edge port interrupts that might have happened...
		// move.b	#0x40,(a5)			// clear int6 from edge port

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
        jsr		_flush_and_invalidate_caches

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
		add.l	#copyback_mmudr,d1	// sonst page cb
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
		move.l	#0x10000,d4			// one whole page (1 MB)

#define _DO_CPU_COPY
#ifndef _DO_CPU_COPY

		// experiment: do video page copy using Coldfire DMA

		lea 	-15 * 4(sp),sp
		movem.l	d0-d1/a0-a1,(sp)	// save gcc scratch registers
		clr.l	-(sp)				// no special functions
		move.l	#MCD_SINGLE_DMA|MCD_TT_FLAGS_CW|MCD_TT_FLAGS_RL|MCD_TT_FLAGS_SP,-(sp)
		mov3q	#7,-(sp)			// highest DMA priority
		move.l	#DMA_ALWAYS,-(sp)	// do memory to memory DMA
		move.l	#1,-(sp)			// copy 4 bytes at a time
		move.l	#0x100000,-(sp)		// copy 1 Megabyte
		move.l	#4,-(sp)			// destination increment
		move.l	a1,-(sp)			// destination adress
		move.l	#4,-(sp)			// source increment
		move.l	a0,-(sp)			// source adress
		move.l	#1,-(sp)			// channel 1
		jsr		_MCD_startDma

		movem.l	(sp),d0-d1/a0-a1	// restore gcc scratch registers
		lea		15 * 4(sp),sp		// adjust stack

wait_dma_finished:
		clr.l	-(sp)
		jsr		_MCD_dmaStatus
		addq.l	#4,sp
		tst.l	d0
		cmp.l	#6,d0
		bne		wait_dma_finished
#else
irq6_vcd0_loop:
		move.l	(a0)+,(a1)+			// page copy
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		subq.l	#1,d4
		bne		irq6_vcd0_loop
#endif /* _DO_CPU_COPY */

irq6_sca_pn:
		add.l	#0x00100000,d3		// next
		cmp.l	#0x00d00000,d3		// ende?
		blt		irq6_next_sca		// nein->

		move.l	#0x2000,d0
		move.l	d0,_video_tlb		// anfangszustand wieder herstellen
		clr.l	_video_sbt			// zeit löschen

        movem.l	(sp),d0-d4/a0-a1	// restore registers
        lea		7 * 4(sp),sp

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
		move.l	0xf0020000,a5		// vector holen
		add.l	_rt_vbr,a5			// basis
		move.l	(a5),d0				// vector holen
		move.l	4(a7),a5			// a5 zurück
		move.l	d0,4(a7)			// vector eintragen
		move.l	(a7)+,d0			// d0 zurück
		move	#0x2600,sr
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
        bpl		acsi_dma_finished	// nein->
        move.l	(a5),(a1)+			// read 4 bytes
        move.l	(a5),(a1)+			// read 4 bytes
        move.l	(a5),(a1)+			// read 4 bytes
        move.l	(a5),(a1)+			// read 4 bytes

        moveq	#'.',d1
        move.b	d1,MCF_PSC0_PSCTB_8BIT

        sub.l	#16,d0				// byt counter -16
        bpl		acsi_dma_rl
        bra		acsi_dma_finished
acsi_dma_wl:
        tst.b	-4(a5) 				// dma req?
        bpl		acsi_dma_finished	// nein->
        move.l	(a1)+,(a5)			// write 4 byts
        move.l	(a1)+,(a5)			// write 4 byts
        move.l	(a1)+,(a5)			// write 4 byts
        move.l	(a1)+,(a5)			// write 4 byts

        moveq	#'.',d1
        move.b	d1,MCF_PSC0_PSCTB_8BIT

		sub.l	#16,d0				// byt counter -16
		bpl		acsi_dma_wl
acsi_dma_finished:
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

#endif /* _NOT_USED_ */
/*
 * irq 7 = pseudo bus error
 */
irq7:
		lea			-12(sp),sp
		movem.l		d0/a0,(sp)

		move.l		__RAMBAR0+0x008,a0	// real access error handler
		move.l		a0,8(sp)			// this will be the return address for rts

		move.w		12(sp),d0			// format/vector word
		andi.l		#0xf000,d0			// keep only the format
		ori.l		#2*4,d0				// simulate vector #2, no fault
		move.w		d0,12(sp)

		// TODO: Inside an interrupt handler, 16(sp) is the return address.
		// For an Access Error, it should be the address of the fault instruction instead

		lea			MCF_EPORT_EPFR,a0
		bset		#7,(a0)				// clear int 7

		move.l		(sp)+,d0			// restore registers
		move.l		(sp)+,a0
		rts								// Forward to the Access Error handler

/*
 * psc3 com PIC MCF
 */
handler_psc3:
        .extern _pic_interrupt_handler

		move.w	#0x2700,sr			// disable interrupt
		lea		-4 * 4(sp),sp		// save gcc scratch registers
		movem.l	d0-d1/a0-a1,(sp)

		jsr		_pic_interrupt_handler	// call high level interrupt handler

		movem.l	(sp),d0-d1/a0-a1	// restore registers
		rte

/*
 * general purpose timer 0 (GPT0): video change, later also others. GPT0 is used as
 * input trigger. It is connected to the TIN0 signal of the FPGA which triggers it everytime
 * vbasehi is written to, i.e. when the video base address gets changed
 */
handler_gpt0:
        move	#0x2700,sr			// disable interrupts

		lea		-28(a7),a7			// save registers
		movem.l	d0-d4/a0-a1,(a7)

		mvz.b	vbasehi,d0			// screen base address high
		cmp.w	#2,d0				// screen base lower than 0x20000?
		blt		video_chg_end		// yes, do nothing
		cmp.w	#0xd0,d0			// lower than 0xd00000? - normal Falcon video area, mapped
									// to 60d00000 (FPGA video memory)
		blt		sca_other			//

        lea		MCF_SLT0_SCNT,a0
        move.l	(a0),_video_sbt		// save time

		bra		video_chg_end
		// FIXME: don't we need to get out here?

sca_other:
		lsl.l	#8,d0				// build new screen start address from Atari register contents
		move.b	0xffff8203,d0		// mid byte
		lsl.l	#8,d0
		move.b  0xffff820d,d0		// low byte
		move.l	d0,d3

video_chg_1page:
// check if page is already marked as video page
		moveq	#20,d4
		move.l	d0,d2
		lsr.l	d4,d2				// new page
		move.l	_video_tlb,d4
		bset.l	d2,d4				// set as changed
		bne		video_chg_2page		// was it set already?
		move.l	d4,_video_tlb
		jsr		_flush_and_invalidate_caches

video_copy_data:
		move.l	d4,_video_tlb
		and.l	#0x00f00000,d0
		move.l	d0,a0
		move.l	a0,a1
		add.l	#0x60000000,a1
		move.l	#0x10000,d4			// whole page

#define _DO_CPU_COPY
#ifndef _DO_CPU_COPY

		// experiment: do video page copy using Coldfire DMA

		lea 	-4 * 4(sp),sp
		movem.l	d0-d1/a0-a1,(sp)	// save gcc scratch registers

		clr.l	-(sp)				// no special functions
		move.l	#MCD_SINGLE_DMA|MCD_TT_FLAGS_CW|MCD_TT_FLAGS_RL|MCD_TT_FLAGS_SP,-(sp)
		mov3q	#7,-(sp)			// highest DMA priority
		move.l	#DMA_ALWAYS,-(sp)	// do memory to memory DMA
		move.l	#1,-(sp)			// copy 4 bytes at a time
		move.l	#0x100000,-(sp)		// copy 1 Megabyte
		move.l	#4,-(sp)			// destination increment
		move.l	a1,-(sp)			// destination adress
		move.l	#4,-(sp)			// source increment
		move.l	a0,-(sp)			// source adress
		move.l	#1,-(sp)			// channel 1
		jsr		_MCD_startDma

.wait_dma_finished:
		clr.l	-(sp)
		jsr		_MCD_dmaStatus
		addq.l	#4,sp
		tst.l	d0
		cmp.l	#6,d0
		bne		.wait_dma_finished

		movem.l	(sp),d0-d1/a0-a1	// restore gcc scratch registers
		lea		4 * 4(sp),sp		// adjust stack

#else

video_copy_data_loop:
		move.l	(a0)+,(a1)+			// copy video page contents to real screen
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		move.l	(a0)+,(a1)+
		subq.l	#1,d4
		bne		video_copy_data_loop
#endif

// eintrag suchen
        move.l	d0,MCF_MMU_MMUAR	// adress
        move.l	#0x106,d4
        move.l	d4,MCF_MMU_MMUOR	// search -> new one will be offered if not found
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
// test of adjacent page is needed also
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
		lea		MCF_GPT0_GMS,a0		// clear interrupt
		bclr.b	#0,3(a0)
		nop
		bset.b	#0,3(a0)

		movem.l	(a7),d0-d4/a0-a1	// restore saved registers
		lea		7 * 4(sp),a7
		rte
#endif /* MACHINE_FIREBEE */

/*
 * low-level interrupt service routine for routines registered with
 * isr_register_handler()
 */
        .global	_lowlevel_isr_handler
        .extern	_isr_execute_handler

_lowlevel_isr_handler:
		move.w	#0x2700,sr			// disable interrupts
		link	a6,#-4*4			// make room for
		movem.l	d0-d1/a0-a1,(sp)	// gcc scratch registers and save them

		move.w	4(a6),d0			// fetch vector number from stack
		lsr.l	#2,d0				// move it in place
		andi.l	#0x000000ff,d0		// mask it out
		move.l	d0,-(sp)			// push it
		jsr		_isr_execute_handler	// call the C handler
		lea		4(sp),sp			// adjust stack
		movem.l	(sp),d0-d1/a0-a1	// restore registers
		unlk	a6					// cleanup stack
		rte

// vim: set syntax=asm68k :
