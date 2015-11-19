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
#define INT_SOURCE_EPORT_EPF1       1   // edge port flag 1
#define INT_SOURCE_EPORT_EPF2       2   // edge port flag 2
#define INT_SOURCE_EPORT_EPF3       3   // edge port flag 3
#define INT_SOURCE_EPORT_EPF4       4   // edge port flag 4
#define INT_SOURCE_EPORT_EPF5       5   // edge port flag 5
#define INT_SOURCE_EPORT_EPF6       6   // edge port flag 6
#define INT_SOURCE_EPORT_EPF7       7   // edge port flag 7
#define INT_SOURCE_USB_EP0ISR       15  // USB endpoint 0 interrupt
#define INT_SOURCE_USB_EP1ISR       16  // USB endpoint 1 interrupt
#define INT_SOURCE_USB_EP2ISR       17  // USB endpoint 2 interrupt
#define INT_SOURCE_USB_EP3ISR       18  // USB endpoint 3 interrupt
#define INT_SOURCE_USB_EP4ISR       19  // USB endpoint 4 interrupt
#define INT_SOURCE_USB_EP5ISR       20  // USB endpoint 5 interrupt
#define INT_SOURCE_USB_EP6ISR       21  // USB endpoint 6 interrupt
#define INT_SOURCE_USB_USBISR       22  // USB general interrupt
#define INT_SOURCE_USB_USBAISR      23  // USB core interrupt
#define INT_SOURCE_USB_ANY          24  // OR of all USB interrupts
#define INT_SOURCE_USB_DSPI_OVF     25  // DSPI overflow or underflow
#define INT_SOURCE_USB_DSPI_RFOF    26  // receive FIFO overflow interrupt
#define INT_SOURCE_USB_DSPI_RFDF    27  // receive FIFO drain interrupt
#define INT_SOURCE_USB_DSPI_TFUF    28  // transmit FIFO underflow interrupt
#define INT_SOURCE_USB_DSPI_TCF     29  // transfer complete interrupt
#define INT_SOURCE_USB_DSPI_TFFF    30  // transfer FIFO fill interrupt
#define INT_SOURCE_USB_DSPI_EOQF    31  // end of queue interrupt
#define INT_SOURCE_PSC3             32  // PSC3 interrupt
#define INT_SOURCE_PSC2             33  // PSC2 interrupt
#define INT_SOURCE_PSC1             34  // PSC1 interrupt
#define INT_SOURCE_PSC0             35  // PSC0 interrupt
#define INT_SOURCE_CTIMERS          36  // combined source for comm timers
#define INT_SOURCE_SEC              37  // SEC interrupt
#define INT_SOURCE_FEC1             38  // FEC1 interrupt
#define INT_SOURCE_FEC0             39  // FEC0 interrupt
#define INT_SOURCE_I2C              40  // I2C interrupt
#define INT_SOURCE_PCIARB           41  // PCI arbiter interrupt
#define INT_SOURCE_CBPCI            42  // COMM bus PCI interrupt
#define INT_SOURCE_XLBPCI           43  // XLB PCI interrupt
#define INT_SOURCE_XLBARB           47  // XLBARB to PCI interrupt
#define INT_SOURCE_DMA              48  // multichannel DMA interrupt
#define INT_SOURCE_CAN0_ERROR       49  // FlexCAN error interrupt
#define INT_SOURCE_CAN0_BUSOFF      50  // FlexCAN bus off interrupt
#define INT_SOURCE_CAN0_MBOR        51  // message buffer ORed interrupt
#define INT_SOURCE_SLT1             53  // slice timer 1 interrupt
#define INT_SOURCE_SLT0             54  // slice timer 0 interrupt
#define INT_SOURCE_CAN1_ERROR       55  // FlexCAN error interrupt
#define INT_SOURCE_CAN1_BUSOFF      56  // FlexCAN bus off interrupt
#define INT_SOURCE_CAN1_MBOR        57  // message buffer ORed interrupt
#define INT_SOURCE_GPT3             59  // GPT3 timer interrupt
#define INT_SOURCE_GPT2             60  // GPT2 timer interrupt
#define INT_SOURCE_GPT1             61  // GPT1 timer interrupt
#define INT_SOURCE_GPT0             62  // GPT0 timer interrupt


#define FEC0_INTC_LVL   6           /* interrupt level for FEC0 */
#define FEC0_INTC_PRI   7           /* interrupt priority for FEC0 */

#define FEC1_INTC_LVL   6           /* interrupt level for FEC1 */
#define FEC1_INTC_PRI   6           /* interrupt priority for FEC1 */

#define FEC_INTC_LVL(x)     ((x == 0) ? FEC0_INTC_LVL : FEC1_INTC_LVL)
#define FEC_INTC_PRI(x)     ((x == 0) ? FEC0_INTC_PRI : FEC1_INTC_PRI)

#define FEC0RX_DMA_PRI      5
#define FEC1RX_DMA_PRI      4
#define FECRX_DMA_PRI(x)    ((x == 0) ? FEC0RX_DMA_PRI : FEC1RX_DMA_PRI)
#define FEC0TX_DMA_PRI      2
#define FEC1TX_DMA_PRI      1
#define FECTX_DMA_PRI(x)    ((x == 0) ? FEC0TX_DMA_PRI : FEC1TX_DMA_PRI)

#if defined(MACHINE_FIREBEE)

/* Firebee FPGA interrupt controller */
#define FBEE_INTR_CONTROL   * ((volatile uint32_t *) 0xf0010000)
#define FBEE_INTR_ENABLE    * ((volatile uint32_t *) 0xf0010004)
#define FBEE_INTR_CLEAR     * ((volatile uint32_t *) 0xf0010008)
#define FBEE_INTR_PENDING   * ((volatile uint32_t *) 0xff01000c)

/* register bits for Firebee FPGA-based interrupt controller */
#define FBEE_INTR_PIC               (1 << 0)        /* PIC interrupt enable/pending/clear bit */
#define FBEE_INTR_ETHERNET          (1 << 1)        /* ethernet PHY interrupt enable/pending/clear bit */
#define FBEE_INTR_DVI               (1 << 2)        /* TFP410 monitor sense interrupt enable/pending/clear bit */
#define FBEE_INTR_PCI_INTA          (1 << 3)        /* /PCIINTA enable/pending clear bit */
#define FBEE_INTR_PCI_INTB          (1 << 4)        /* /PCIINTB enable/pending clear bit */
#define FBEE_INTR_PCI_INTC          (1 << 5)        /* /PCIINTC enable/pending clear bit */
#define FBEE_INTR_PCI_INTD          (1 << 6)        /* /PCIINTD enable/pending clear bit */
#define FBEE_INTR_DSP               (1 << 7)        /* DSP interrupt enable/pending/clear bit */
#define FBEE_INTR_VSYNC             (1 << 8)        /* VSYNC interrupt enable/pending/clear bit */
#define FBEE_INTR_HSYNC             (1 << 9)        /* HSYNC interrupt enable/pending/clear bit */

#define FBEE_INTR_INT_HSYNC_IRQ2    (1 << 26)       /* these bits are only meaningful for the FBEE_INTR_ENABLE register */
#define FBEE_INTR_INT_CTR0_IRQ3     (1 << 27)
#define FBEE_INTR_INT_VSYNC_IRQ4    (1 << 28)
#define FBEE_INTR_INT_FPGA_IRQ5     (1 << 29)
#define FBEE_INTR_INT_MFP_IRQ6      (1 << 30)
#define FBEE_INTR_INT_IRQ7          (1 << 31)

/*
 * Atari MFP interrupt registers.
 */

#define FALCON_MFP_IERA     *((volatile uint8_t *) 0xfffffa07)
#define FALCON_MFP_IERB     *((volatile uint8_t *) 0xfffffa09)
#define FALCON_MFP_IPRA     *((volatile uint8_t *) 0xfffffa0b)
#define FALCON_MFP_IPRB     *((volatile uint8_t *) 0xfffffa0d)
#define FALCON_MFP_IMRA     *((volatile uint8_t *) 0xfffffa13)
#define FALCON_MFP_IMRB     *((volatile uint8_t *) 0xfffffa15)

#endif /* MACHINE_FIREBEE */

extern void isr_init(void);
extern bool isr_set_prio_and_level(int int_source, int priority, int level);
extern bool isr_enable_int_source(int int_source);
extern bool isr_register_handler(int vector, int level, int priority, bool (*handler)(void *, void *), void *hdev, void *harg);
extern void isr_remove_handler(bool (*handler)(void *, void *));
extern bool isr_execute_handler(int vector);
extern bool pic_interrupt_handler(void *arg1, void *arg2);
extern bool xlbpci_interrupt_handler(void *arg1, void *arg2);
extern bool pciarb_interrupt_handler(void *arg1, void *arg2);
extern bool gpt0_interrupt_handler(void *arg1, void *arg2);
extern bool irq5_handler(void *arg1, void *arg2);
#endif /* _INTERRUPTS_H_ */
