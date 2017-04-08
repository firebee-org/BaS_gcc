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

#include "startcf.h"
#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#endif /* MACHINE_FIREBEE */

	.extern	__SUP_SP
	.extern	_rom_entry
	.extern	__RAMBAR0
	.extern	_rt_mod
	.extern	_rt_ssp
	.extern	_rt_usp
	.extern	_rt_vbr
	.extern	_mmutr_miss
	.extern	__MBAR
	.extern	__MMUBAR
	.extern	_video_tlb
	.extern	_video_sbt
	.extern	_flush_and_invalidate_caches
	.extern	_get_bas_drivers

	/* PCI interrupt handlers */
	.extern	_irq5_handler
	.extern	_irq6_handler
	.extern	_irq7_handler

	.global	_vec_init
	.global	_std_exc_vec                /* needed by driver_vec.c */

/* Register read/write equates */

	/* MMU */
	.equ	MCF_MMU_MMUCR, __MMUBAR
	.equ	MCF_MMU_MMUOR, __MMUBAR+0x04
	.equ	MCF_MMU_MMUSR, __MMUBAR+0x08
	.equ	MCF_MMU_MMUAR, __MMUBAR+0x10
	.equ	MCF_MMU_MMUTR, __MMUBAR+0x14
	.equ	MCF_MMU_MMUDR, __MMUBAR+0x18

	/* EPORT flag register */
	.equ	MCF_EPORT_EPFR, __MBAR+0xf0c

	/* FEC1 port output data direction register */
	.equ	MCF_GPIO_PODR_FEC1L, __MBAR+0xa07

	/* PSC0 transmit buffer register */
	.equ	MCF_PSC0_PSCTB_8BIT, __MBAR+0x860c

	/* GPT mode select register */
	.equ	MCF_GPT0_GMS, __MBAR+0x800

	/* Slice timer 0 count register */
	.equ	MCF_SLT0_SCNT, __MBAR+0x908

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
	.equ	INT_SOURCE_PSC3,32		// PSC3 interrupt
	.equ	INT_SOURCE_PSC2,33		// PSC2 interrupt
	.equ	INT_SOURCE_PSC1,34		// PSC1 interrupt
	.equ	INT_SOURCE_PSC0,35		// PSC0 interrupt
	.equ	INT_SOURCE_CTIMERS,36		// combined source for comm timers
	.equ	INT_SOURCE_SEC,37		// SEC interrupt
	.equ	INT_SOURCE_FEC1,38		// FEC1 interrupt
	.equ	INT_SOURCE_FEC0,39		// FEC0 interrupt
	.equ	INT_SOURCE_I2C,40		// I2C interrupt
	.equ	INT_SOURCE_PCIARB,41		// PCI arbiter interrupt
	.equ	INT_SOURCE_CBPCI,42		// COMM bus PCI interrupt
	.equ	INT_SOURCE_XLBPCI,43		// XLB PCI interrupt
	.equ	INT_SOURCE_XLBARB,47		// XLBARB to PCI interrupt
	.equ	INT_SOURCE_DMA,48		// multichannel DMA interrupt
	.equ	INT_SOURCE_CAN0_ERROR,49	// FlexCAN error interrupt
	.equ	INT_SOURCE_CAN0_BUSOFF,50	// FlexCAN bus off interrupt
	.equ	INT_SOURCE_CAN0_MBOR,51		// message buffer ORed interrupt
	.equ	INT_SOURCE_SLT1,53		// slice timer 1 interrupt
	.equ	INT_SOURCE_SLT0,54		// slice timer 0 interrupt
	.equ	INT_SOURCE_CAN1_ERROR,55	// FlexCAN error interrupt
	.equ	INT_SOURCE_CAN1_BUSOFF,56	// FlexCAN bus off interrupt
	.equ	INT_SOURCE_CAN1_MBOR,57		// message buffer ORed interrupt
	.equ	INT_SOURCE_GPT3,59		// GPT3 timer interrupt
	.equ	INT_SOURCE_GPT2,60		// GPT2 timer interrupt
	.equ	INT_SOURCE_GPT1,61		// GPT1 timer interrupt
	.equ	INT_SOURCE_GPT0,62		// GPT0 timer interrupt

// Atari register equates (provided by FPGA)
	.equ	vbasehi, 0xffff8201

/*
 * macros
 */

/*
 * used for "forwarding" interrupt handlers. This just clears the "pending interrupt"
 * flag from the EDGE PORT flag register, set the status register to the appropriate interrupt
 * mask an jump through the corresponging vector
 */
	.macro	irq vector,int_mask,clr_int
	move.w	#0x2700,sr			// disable interrupts
	subq.l	#8,sp
	movem.l	d0/a5,(sp)			// save registers

	lea	MCF_EPORT_EPFR,a5
	move.b	#\clr_int,(a5)			// clear int pending

	movem.l	(sp),d0/a5			// restore registers
	addq.l	#8,sp
	move.l	\vector,-(sp)
	move	#0x2\int_mask\()00,sr
	rts
	.endm

	.text
_vec_init:
	move.l	a2,-(sp)			// Backup registers

	mov3q.l	#-1,_rt_mod			// rt_mod auf super
	clr.l	_rt_ssp
	clr.l	_rt_usp
	clr.l	_rt_vbr
	move.l	#__RAMBAR0,d0			// exception vectors reside in rambar0
	movec	d0,VBR
	move.l	d0,a0
	move.l	a0,a2

/*
 * first, set standard vector for all exceptions
 */
init_vec:
	move.l	#256,d0
	lea	std_exc_vec(pc),a1		// standard vector
init_vec_loop:
	move.l	a1,(a2)+			// set standard vector for all exceptions
	subq.l	#1,d0
	bne	init_vec_loop

// set individual interrupt handler assignments

	move.l	#__SUP_SP,(a0)			// set initial stack pointer at start of exception vector table

	lea	reset_vector(pc),a1		// set reset vector
	move.l	a1,0x04(a0)

	lea	access(pc),a1			// set illegal access exception handler
	move.l	a1,0x08(a0)

// install spurious interrupt handler
	lea	_lowlevel_isr_handler,a1
	move.l	a1,0x60(a0)

// trap #0 (without any parameters for now) is used to provide BaS' driver addresses to the OS
	lea	_get_bas_drivers(pc),a1
	move.l	a1,0x80(a0)			// trap #0 exception vector

// MFP non-autovector interrupt handlers. Those are just rerouted to their autovector counterparts

	lea	irq1(pc),a1
	move.l	a1,0x104(a0)

	lea	irq2(pc),a1
	move.l	a1,0x108(a0)

	lea	irq3(pc),a1
	move.l	a1,0x10c(a0)

	lea	irq4(pc),a1
	move.l	a1,0x110(a0)

	lea	irq5(pc),a1
	move.l	a1,0x114(a0)

	lea	irq6(pc),a1
	move.l	a1,0x118(a0)

	lea	irq7(pc),a1
	move.l	a1,0x11c(a0)



// install lowlevel_isr_handler for the three GPT timers
	lea	_lowlevel_isr_handler(pc),a1
	move.l	a1,(INT_SOURCE_GPT1 + 64) * 4(a0)
	move.l	a1,(INT_SOURCE_GPT2 + 64) * 4(a0)
	move.l	a1,(INT_SOURCE_GPT3 + 64) * 4(a0)

// install lowlevel_isr_handler for the PSC3 interrupt
	move.l	a1,(INT_SOURCE_PSC3 + 64) * 4(a0)

// install lowlevel_isr_handler for Coldfire DMA interrupts
	move.l	a1,(INT_SOURCE_DMA + 64) * 4(a0)

// install lowlevel_isr_handler for the XLBPCI interrupt
	move.l	a1,(INT_SOURCE_XLBPCI + 64) * 4(a0)

// install lowlevel_isr_handler for the XLBARB interrupt
	move.l	a1,(INT_SOURCE_XLBARB + 64) * 4(a0)

// install lowlevel_isr_handler for the FEC0  interrupt
	move.l	a1,(INT_SOURCE_FEC0 + 64) * 4(a0)

#ifndef MACHINE_FIREBEE
// FEC1 not wired on the FireBee (used for FPGA as GPIO), but available on other machines
	move.l	a1,(INT_SOURCE_FEC1 + 64) * 4(a0)
#endif

#ifdef MACHINE_FIREBEE

// timer vectors (triggers when vbashi gets changed, used for video page copy)
	// move.l  a1,(INT_SOURCE_GPT0 + 64) * 4(a0)
#endif /* MACHINE_FIREBEE */

	move.l	(sp)+,a2			// Restore registers
	rts


/*
 * exception vector routines
 */
vector_table_start:
std_exc_vec:
_std_exc_vec:
	move.w	#0x2700,sr			// disable interrupt
	subq.l	#8,sp
	movem.l	d0/a5,(sp)			// save registers
	move.w	8(sp),d0			// fetch vector
	and.l	#0x3fc,d0			// mask out vector number
// #define DBG_EXC
#ifdef DBG_EXC
	// printout vector number of exception

	lea	-4 * 4(sp),sp			// reserve stack space
	movem.l	d0-d1/a0-a1,(sp)		// save gcc scratch registers

	lsr.l	#2,d0				// shift vector number in place
	cmp.l	#33,d0
	beq	noprint
	cmp.l	#34,d0
	beq	noprint
	cmp.l	#45,d0
	beq	noprint
	cmp.l	#46,d0
	beq	noprint
	move.l	4 * 4 + 8 + 4(sp),-(sp)		// pc at exception
	move.l	d0,-(sp)			// provide it to xprintf()
	pea	exception_text
	jsr	_xprintf			// call xprintf()
	add.l	#3*4,sp				// adjust stack
noprint:

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	lea	4 * 4(sp),sp
#endif /* DBG_EXC */

	add.l	_rt_vbr,d0			// + VBR
	move.l	d0,a5
	move.l	(a5),d0				// fetch exception routine address

	move.l	4(sp),a5			// restore a5
	move.l	d0,4(sp)			// store exception routine address

	move.w	10(sp),d0			// restore original SR (irq mask)
	bset	#13,d0				// set supervisor bit
	move.w	d0,sr				//
	move.l	(sp)+,d0			// restore d0
	rts					// jump to exception handler

exception_text:
	.ascii	"DEBUG: EXCEPTION %d caught at %p"
	.byte	13, 10, 0
	.align	4

reset_vector:
	move.w	#0x2700,sr			// disable interrupts
	move.l	#0x31415926,d0
	cmp.l	0x426,d0			// _resvalid: reset vector valid?
	beq	std_exc_vec			// yes->
	jmp	_rom_entry			// no, cold start machine

access:
	move.w	#0x2700,sr			// disable interrupts

	link	a6,#-4 * 4			// make room for gcc scratch registers
	movem.l	d0-d1/a0-a1,(sp)		// save them

	move.l	4(a6),-(sp)			// push format_status
	move.l	8(a6),-(sp)			// pc at exception
	move.l	MCF_MMU_MMUAR,-(sp)		// MMU fault address
	move.l	MCF_MMU_MMUSR,-(sp)		// MMU status register
	// probably doesn't make sense since we still have a potential unmapped MMU page
	// move.w  #0x2300,sr			// can lower interrupt mask now that MMU status is safe
	jsr	_mmutr_miss			// call C routine
	lea	4 * 4(sp),sp			// adjust stack

	tst.l	d0				// exception handler signals bus error
	bne	bus_error

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6
	rte

bus_error:
	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6
	move.l	0x08,-(sp)
	rts
	// bra		std_exc_vec		// FIXME: this seems to be bogous...

zero_divide:
	move.w	#0x2700,sr			// disable interrupt
	move.l	a0,-(sp)
	move.l	d0,-(sp)
	move.l	12(sp),a0			// pc
	move.w	(a0)+,d0			// command word
	btst	#7,d0				// long?
	beq	zd_word				// no->
	addq.l	#2,a0

zd_word:
	and.l	0x3f,d0				// mask out ea field
	cmp.w	#0x08,d0			// -(ax) or less?
	ble	zd_end
	addq.l	#2,a0
	cmp.w	#0x39,d0			// xxx.L
	bne	zd_nal
	addq.l	#2,a0
	bra	zd_end

zd_nal:	cmp.w	#0x3c,d0			// immediate?
	bne	zd_end				// no->
	btst	#7,d0				// long?
	beq	zd_end				// no
	addq.l	#2,a0
zd_end:
	move.l	a0,12(sp)
	move.l	(sp)+,d0
	move.l	(sp)+,a0
	rte

#ifdef _NOT_USED_
linea:
	move.w	#0x2700,sr			// disable interrupt
	halt
	nop
	nop
linef:
	move.w  #0x2700,sr			// disable interrupt
	halt
	nop
	nop
format:
	move.w  #0x2700,sr			// disable interrupt
	halt
	nop
	nop

//floating point
flpoow:
	move.w  #0x2700,sr			// disable interrupt
	halt
	nop
	nop
#endif /* _NOT_USED */


irq1:	irq	0x64, 1, 0x02			// Level 1 autovector interrupt (unused)
irq2:	irq	0x68, 2, 0x04			// Level 2 autovector interrupt (horizontal blank)
irq3:	irq	0x6c, 3, 0x08			// Level 3 autovector interrupt (unused)
irq4:	irq	0x70, 4, 0x10			// Level 4 autovector interrupt (vertical blank)



#if defined(MACHINE_FIREBEE)			// these handlers are only meaningful for the Firebee
irq5:	//move.w  #0x2700,sr			// disable interrupts
	subq.l	#4,sp				// extra space

	link	a6,#-4 * 4			// save gcc scratch registers
	movem.l	d0-d1/a0-a1,(sp)

	jsr	_irq5_handler			// call C handler routine

	tst.b	d0				// handled?
	beq	irq5_forward

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6
	addq.l	#4,sp

	rte					// return from exception

irq5_forward:   move.l	0x74,a0			// fetch OS irq5 vector
	add.l	_rt_vbr,a0			// add runtime vbr
	move.l	a0,4(a6)			// put on stack

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6				//
	move.w	#0x2500,sr			// set interrupt level
	rts					// jump through vector

/*
 * irq6 needs special treatment since - because the Coldfire only supports autovector interrupts
 * - the exception vector is provided by the simulated MFP from the FPGA
 */
irq6:	move.w	#0x2700,sr			// disable interrupt
	subq.l	#4,sp				// extra space

	link	a6,#-4 * 4			// save gcc scratch registers
	movem.l d0-d1/a0-a1,(sp)

	move.l	8(a6),-(sp)			// format status word
	move.l	12(a6),-(sp)			// pc at exception
	jsr	_irq6_handler			// call C handler
	lea	8(sp),sp			// fix stack

	tst.b	d0				// interrupt handled?
	beq	irq6_forward			// no, forward to TOS

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6
	addq.l	#4,sp				// "extra space" not needed in this case
	rte

irq6_forward:
	move.l	0xf0020000,a0			// fetch "MFP interrupt vector" from FPGA
	add.l	_rt_vbr,a0			// add runtime VBR
	move.l  (a0),4(a6)			// fetch handler address and put it on "extra space"

	movem.l (sp),d0-d1/a0-a1
	unlk	a6
	move.w	#0x2600,sr			// set interrupt mask to MFP level

	rts					// jump through vector

/*
 * irq 7 = pseudo bus error
 */
irq7:
	lea	-12(sp),sp
	movem.l d0/a0,(sp)

	move.l	__RAMBAR0+0x008,a0		// real access error handler
	move.l	a0,8(sp)			// this will be the return address for rts

	move.w	12(sp),d0			// format/vector word
	andi.l	#0xf000,d0			// keep only the format
	ori.l	#2*4,d0				// simulate vector #2, no fault
	move.w	d0,12(sp)

	// TODO: Inside an interrupt handler, 16(sp) is the return address.
	// For an Access Error, it should be the address of the fault instruction instead

	lea	MCF_EPORT_EPFR,a0
	bset	#7,(a0)				// clear int 7

	move.l	(sp)+,d0			// restore registers
	move.l	(sp)+,a0
	rts					// Forward to the Access Error handler

#else						// handlers for M5484LITE

irq5:
	move.w	#0x2700,sr			// disable interrupts
	subq.l	#4,sp				// extra space

	link	a6,#-4 * 4			// save gcc scratch registers
	movem.l	d0-d1/a0-a1,(sp)

	jsr	_irq5_handler			// call C handler routine

	tst.b	d0				// handled?
	beq     irq5_forward

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6
	addq.l	#4,sp

	rte					// return from exception

irq5_forward:
	move.l	0x74,a0				// fetch OS irq5 vector
	add.l	_rt_vbr,a0			// add runtime vbr
	move.l	a0,4(a6)			// put on stack

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6				//
	move.w	#0x2500,sr			// set interrupt level
	rts					// jump through vector

irq6:
	irq	0x74,5,0x20

irq7:						// irq7 is tied to PCI INTA# and PCI INTB# on the M5484LITE

	move.w	#0x2700,sr			// disable interrupts

	lea	-4*4(sp),sp			// save gcc scratch registers
	movem.l	d0-d1/a0-a1,(sp)

	jsr	_irq7_handler			// call C handler routine

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	lea	4 * 4(sp),sp

	rte					// return from exception

irq7text:
	.data
	.ascii	"IRQ7!"
	.byte	13,10,0
	.align	4
	.text
#endif /* MACHINE_FIREBEE */

/*
 * low-level interrupt service routine for routines registered with
 * isr_register_handler(int vector). If the higlevel routine (isr_execute_handler())
 * returns != true, the call is forwarded to the OS (through its own vector base).
 */
	.global _lowlevel_isr_handler
	.extern _isr_execute_handler


/*
 * stack format (after link instruction) is like this:
 *
 * +12      program counter (return address)
 * +8       format_status
 * +4       save area for rts (if we need to jump through the OS vector)
 * (a6) ->  saved a6 (from link)
 * -4
 * -8
 * -12
 * (sp) ->  gcc scratch registers save area
 */
_lowlevel_isr_handler:
	subq.l	#4,sp				// extra space
	link	a6,#-4 * 4			// make room for
	movem.l	d0-d1/a0-a1,(sp)		// gcc scratch registers and save them,
						// other registers will be taken care of by gcc itself

	move.w	8(a6),d0			// fetch vector number from stack
	lsr.l	#2,d0				// move it in place
	andi.l	#0xff,d0			// mask it out
	move.l	d0,-(sp)			// push it
	jsr	_isr_execute_handler		// call the C handler
	addq.l	#4,sp				// adjust stack
	tst.b	d0				// handled?
	beq	lowlevel_forward		// no, forward it to TOS

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6
	addq.l	#4,sp				// eliminate extra space

	rte

lowlevel_forward:
	move.l	8(a6),d0			// fetch OS irq vector
	lsr.l	#2,d0				// move it in place
	andi.l	#0xff,d0			// mask out vector number
	add.l	_rt_vbr,d0			// add runtime vbr
	move.l	d0,4(a6)			// put on stack as return address

	movem.l	(sp),d0-d1/a0-a1		// restore registers
	unlk	a6				//
	rts					// jump through vector
