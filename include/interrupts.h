/*
 * interrupts.h
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
 *
 *  Created on: 08.08.2013
 *      Author: Markus Fröschle
 */

#ifndef _INTERRUPTS_H_
#define _INTERRUPTS_H_

#include <stdbool.h>

/* interrupt sources */
#define INT_SOURCE_EPORT_EPF1		1	// edge port flag 1
#define	INT_SOURCE_EPORT_EPF2		2	// edge port flag 2
#define	INT_SOURCE_EPORT_EPF3		3	// edge port flag 3
#define	INT_SOURCE_EPORT_EPF4		4	// edge port flag 4
#define	INT_SOURCE_EPORT_EPF5		5	// edge port flag 5
#define	INT_SOURCE_EPORT_EPF6		6	// edge port flag 6
#define	INT_SOURCE_EPORT_EPF7		7	// edge port flag 7
#define	INT_SOURCE_USB_EP0ISR		15	// USB endpoint 0 interrupt
#define	INT_SOURCE_USB_EP1ISR		16	// USB endpoint 1 interrupt
#define	INT_SOURCE_USB_EP2ISR		17	// USB endpoint 2 interrupt
#define	INT_SOURCE_USB_EP3ISR		18	// USB endpoint 3 interrupt
#define	INT_SOURCE_USB_EP4ISR		19	// USB endpoint 4 interrupt
#define	INT_SOURCE_USB_EP5ISR		20	// USB endpoint 5 interrupt
#define	INT_SOURCE_USB_EP6ISR		21	// USB endpoint 6 interrupt
#define	INT_SOURCE_USB_USBISR		22	// USB general interrupt
#define	INT_SOURCE_USB_USBAISR		23	// USB core interrupt
#define	INT_SOURCE_USB_ANY			24	// OR of all USB interrupts
#define	INT_SOURCE_USB_DSPI_OVF		25	// DSPI overflow or underflow
#define	INT_SOURCE_USB_DSPI_RFOF	26	// receive FIFO overflow interrupt
#define	INT_SOURCE_USB_DSPI_RFDF	27	// receive FIFO drain interrupt
#define	INT_SOURCE_USB_DSPI_TFUF	28	// transmit FIFO underflow interrupt
#define	INT_SOURCE_USB_DSPI_TCF		29	// transfer complete interrupt
#define	INT_SOURCE_USB_DSPI_TFFF	30	// transfer FIFO fill interrupt
#define	INT_SOURCE_USB_DSPI_EOQF	31	// end of queue interrupt
#define	INT_SOURCE_PSC3				32	// PSC3 interrupt
#define	INT_SOURCE_PSC2				33	// PSC2 interrupt
#define	INT_SOURCE_PSC1				34	// PSC1 interrupt
#define	INT_SOURCE_PSC0				35	// PSC0 interrupt
#define	INT_SOURCE_CTIMERS			36	// combined source for comm timers
#define	INT_SOURCE_SEC				37	// SEC interrupt
#define	INT_SOURCE_FEC1				38	// FEC1 interrupt
#define	INT_SOURCE_FEC0				39	// FEC0 interrupt
#define	INT_SOURCE_I2C				40	// I2C interrupt
#define	INT_SOURCE_PCIARB			41	// PCI arbiter interrupt
#define	INT_SOURCE_CBPCI			42	// COMM bus PCI interrupt
#define	INT_SOURCE_XLBPCI			43	// XLB PCI interrupt
#define	INT_SOURCE_XLBARB			47	// XLBARB to PCI interrupt
#define	INT_SOURCE_DMA				48	// multichannel DMA interrupt
#define	INT_SOURCE_CAN0_ERROR		49	// FlexCAN error interrupt
#define	INT_SOURCE_CAN0_BUSOFF		50	// FlexCAN bus off interrupt
#define	INT_SOURCE_CAN0_MBOR		51	// message buffer ORed interrupt
#define	INT_SOURCE_SLT1				53	// slice timer 1 interrupt
#define	INT_SOURCE_SLT0				54	// slice timer 0 interrupt
#define	INT_SOURCE_CAN1_ERROR		55	// FlexCAN error interrupt
#define	INT_SOURCE_CAN1_BUSOFF		56	// FlexCAN bus off interrupt
#define	INT_SOURCE_CAN1_MBOR		57	// message buffer ORed interrupt
#define	INT_SOURCE_GPT3				59	// GPT3 timer interrupt
#define	INT_SOURCE_GPT2				60	// GPT2 timer interrupt
#define	INT_SOURCE_GPT1				61	// GPT1 timer interrupt
#define	INT_SOURCE_GPT0				62	// GPT0 timer interrupt

#define DMA_INTC_LVL	6			/* interrupt level for DMA interrupt */
#define DMA_INTC_PRI	0			/* interrupt priority for DMA interrupt */

#define FEC0_INTC_LVL	5			/* interrupt level for FEC0 */
#define FEC0_INTC_PRI	0			/* interrupt priority for FEC0 */

#define FEC1_INTC_LVL	5			/* interrupt level for FEC1 */
#define FEC1_INTC_PRI	1			/* interrupt priority for FEC1 */

#define FEC_INTC_LVL(x)		((x == 0) ? FEC0_INTC_LVL : FEC1_INTC_LVL)
#define FEC_INTC_PRI(x)		((x == 0) ? FEC0_INTC_PRI : FEC1_INTC_PRI)

#define FEC0RX_DMA_PRI      5
#define FEC1RX_DMA_PRI      5
#define FECRX_DMA_PRI(x)    ((x == 0) ? FEC0RX_DMA_PRI : FEC1RX_DMA_PRI)
#define FEC0TX_DMA_PRI      6
#define FEC1TX_DMA_PRI      6
#define FECTX_DMA_PRI(x)    ((x == 0) ? FEC0TX_DMA_PRI : FEC1TX_DMA_PRI)

extern int register_interrupt_handler(uint8_t source, uint8_t level, uint8_t priority, uint8_t intr, void (*handler)(void));

#define ISR_DBUG_ISR	0x01
#define ISR_USER_ISR 	0x02

extern void isr_init(void);
extern int isr_register_handler(int type, int vector, int (*handler)(void *, void *), void *hdev, void *harg);
extern void isr_remove_handler(int type ,int (*handler)(void *, void *));
extern bool isr_execute_handler(int vector);
#endif /* _INTERRUPTS_H_ */
