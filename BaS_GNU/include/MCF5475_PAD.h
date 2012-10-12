/* Coldfire C Header File
 * Copyright Freescale Semiconductor Inc
 * All rights reserved.
 *
 * 2008/05/23 Revision: 0.81
 *
 * (c) Copyright UNIS, a.s. 1997-2008
 * UNIS, a.s.
 * Jundrovska 33
 * 624 00 Brno
 * Czech Republic
 * http      : www.processorexpert.com
 * mail      : info@processorexpert.com
 */

#ifndef __MCF5475_PAD_H__
#define __MCF5475_PAD_H__


/*********************************************************************
*
* Common GPIO
*
*********************************************************************/

/* Register read/write macros */
#define MCF_PAD_PAR_FBCTL                    (*(volatile uint16_t*)(&__MBAR[0xA40]))
#define MCF_PAD_PAR_FBCS                     (*(volatile uint8_t *)(&__MBAR[0xA42]))
#define MCF_PAD_PAR_DMA                      (*(volatile uint8_t *)(&__MBAR[0xA43]))
#define MCF_PAD_PAR_FECI2CIRQ                (*(volatile uint16_t*)(&__MBAR[0xA44]))
#define MCF_PAD_PAR_PCIBG                    (*(volatile uint16_t*)(&__MBAR[0xA48]))
#define MCF_PAD_PAR_PCIBR                    (*(volatile uint16_t*)(&__MBAR[0xA4A]))
#define MCF_PAD_PAR_PSC3                     (*(volatile uint8_t *)(&__MBAR[0xA4C]))
#define MCF_PAD_PAR_PSC2                     (*(volatile uint8_t *)(&__MBAR[0xA4D]))
#define MCF_PAD_PAR_PSC1                     (*(volatile uint8_t *)(&__MBAR[0xA4E]))
#define MCF_PAD_PAR_PSC0                     (*(volatile uint8_t *)(&__MBAR[0xA4F]))
#define MCF_PAD_PAR_DSPI                     (*(volatile uint16_t*)(&__MBAR[0xA50]))
#define MCF_PAD_PAR_TIMER                    (*(volatile uint8_t *)(&__MBAR[0xA52]))


/* Bit definitions and macros for MCF_PAD_PAR_FBCTL */
#define MCF_PAD_PAR_FBCTL_PAR_ALE(x)         (((x)&0x3)<<0)
#define MCF_PAD_PAR_FBCTL_PAR_ALE_GPIO       (0)
#define MCF_PAD_PAR_FBCTL_PAR_ALE_TBST       (0x2)
#define MCF_PAD_PAR_FBCTL_PAR_ALE_ALE        (0x3)
#define MCF_PAD_PAR_FBCTL_PAR_TA             (0x4)
#define MCF_PAD_PAR_FBCTL_PAR_RWB(x)         (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_FBCTL_PAR_RWB_GPIO       (0)
#define MCF_PAD_PAR_FBCTL_PAR_RWB_TBST       (0x20)
#define MCF_PAD_PAR_FBCTL_PAR_RWB_RW         (0x30)
#define MCF_PAD_PAR_FBCTL_PAR_OE             (0x40)
#define MCF_PAD_PAR_FBCTL_PAR_BWE0           (0x100)
#define MCF_PAD_PAR_FBCTL_PAR_BWE1           (0x400)
#define MCF_PAD_PAR_FBCTL_PAR_BWE2           (0x1000)
#define MCF_PAD_PAR_FBCTL_PAR_BWE3           (0x4000)

/* Bit definitions and macros for MCF_PAD_PAR_FBCS */
#define MCF_PAD_PAR_FBCS_PAR_CS1             (0x2)
#define MCF_PAD_PAR_FBCS_PAR_CS2             (0x4)
#define MCF_PAD_PAR_FBCS_PAR_CS3             (0x8)
#define MCF_PAD_PAR_FBCS_PAR_CS4             (0x10)
#define MCF_PAD_PAR_FBCS_PAR_CS5             (0x20)

/* Bit definitions and macros for MCF_PAD_PAR_DMA */
#define MCF_PAD_PAR_DMA_PAR_DREQ0(x)         (((x)&0x3)<<0)
#define MCF_PAD_PAR_DMA_PAR_DREQ0_GPIO       (0)
#define MCF_PAD_PAR_DMA_PAR_DREQ0_TIN0       (0x2)
#define MCF_PAD_PAR_DMA_PAR_DREQ0_DREQ0      (0x3)
#define MCF_PAD_PAR_DMA_PAR_DREQ1(x)         (((x)&0x3)<<0x2)
#define MCF_PAD_PAR_DMA_PAR_DREQ1_GPIO       (0)
#define MCF_PAD_PAR_DMA_PAR_DREQ1_IRQ1       (0x4)
#define MCF_PAD_PAR_DMA_PAR_DREQ1_TIN1       (0x8)
#define MCF_PAD_PAR_DMA_PAR_DREQ1_DREQ1      (0xC)
#define MCF_PAD_PAR_DMA_PAR_DACK0(x)         (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_DMA_PAR_DACK0_GPIO       (0)
#define MCF_PAD_PAR_DMA_PAR_DACK0_TOUT0      (0x20)
#define MCF_PAD_PAR_DMA_PAR_DACK0_DACK0      (0x30)
#define MCF_PAD_PAR_DMA_PAR_DACK1(x)         (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_DMA_PAR_DACK1_GPIO       (0)
#define MCF_PAD_PAR_DMA_PAR_DACK1_TOUT1      (0x80)
#define MCF_PAD_PAR_DMA_PAR_DACK1_DACK1      (0xC0)

/* Bit definitions and macros for MCF_PAD_PAR_FECI2CIRQ */
#define MCF_PAD_PAR_FECI2CIRQ_PAR_IRQ5       (0x1)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_IRQ6       (0x2)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_SCL        (0x4)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_SDA        (0x8)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDC(x)   (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDC_SCL  (0x80)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDC_E1MDC (0xC0)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDIO(x)  (((x)&0x3)<<0x8)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDIO_SDA (0x200)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDIO_E1MDIO (0x300)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E1MII      (0x400)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E17        (0x800)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E0MDC      (0x1000)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E0MDIO     (0x2000)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E0MII      (0x4000)
#define MCF_PAD_PAR_FECI2CIRQ_PAR_E07        (0x8000)

/* Bit definitions and macros for MCF_PAD_PAR_PCIBG */
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG0(x)      (((x)&0x3)<<0)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG0_GPIO    (0)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG0_TOUT0   (0x2)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG0_PCIBG0  (0x3)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG1(x)      (((x)&0x3)<<0x2)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG1_GPIO    (0)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG1_TOUT1   (0x8)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG1_PCIBG1  (0xC)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG2(x)      (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG2_GPIO    (0)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG2_TOUT2   (0x20)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG2_PCIBG2  (0x30)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG3(x)      (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG3_GPIO    (0)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG3_TOUT3   (0x80)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG3_PCIBG3  (0xC0)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG4(x)      (((x)&0x3)<<0x8)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG4_GPIO    (0)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG4_TBST    (0x200)
#define MCF_PAD_PAR_PCIBG_PAR_PCIBG4_PCIBG4  (0x300)

/* Bit definitions and macros for MCF_PAD_PAR_PCIBR */
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR0(x)      (((x)&0x3)<<0)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR0_GPIO    (0)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR0_TIN0    (0x2)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR0_PCIBR0  (0x3)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR1(x)      (((x)&0x3)<<0x2)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR1_GPIO    (0)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR1_TIN1    (0x8)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR1_PCIBR1  (0xC)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR2(x)      (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR2_GPIO    (0)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR2_TIN2    (0x20)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR2_PCIBR2  (0x30)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR3(x)      (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR3_GPIO    (0)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR3_TIN3    (0x80)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR3_PCIBR3  (0xC0)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR4(x)      (((x)&0x3)<<0x8)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR4_GPIO    (0)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR4_IRQ4    (0x200)
#define MCF_PAD_PAR_PCIBR_PAR_PCIBR4_PCIBR4  (0x300)

/* Bit definitions and macros for MCF_PAD_PAR_PSC3 */
#define MCF_PAD_PAR_PSC3_PAR_TXD3            (0x4)
#define MCF_PAD_PAR_PSC3_PAR_RXD3            (0x8)
#define MCF_PAD_PAR_PSC3_PAR_RTS3(x)         (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_PSC3_PAR_RTS3_GPIO       (0)
#define MCF_PAD_PAR_PSC3_PAR_RTS3_FSYNC      (0x20)
#define MCF_PAD_PAR_PSC3_PAR_RTS3_RTS        (0x30)
#define MCF_PAD_PAR_PSC3_PAR_CTS3(x)         (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_PSC3_PAR_CTS3_GPIO       (0)
#define MCF_PAD_PAR_PSC3_PAR_CTS3_BCLK       (0x80)
#define MCF_PAD_PAR_PSC3_PAR_CTS3_CTS        (0xC0)

/* Bit definitions and macros for MCF_PAD_PAR_PSC2 */
#define MCF_PAD_PAR_PSC2_PAR_TXD2            (0x4)
#define MCF_PAD_PAR_PSC2_PAR_RXD2            (0x8)
#define MCF_PAD_PAR_PSC2_PAR_RTS2(x)         (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_PSC2_PAR_RTS2_GPIO       (0)
#define MCF_PAD_PAR_PSC2_PAR_RTS2_FSYNC      (0x20)
#define MCF_PAD_PAR_PSC2_PAR_RTS2_RTS        (0x30)
#define MCF_PAD_PAR_PSC2_PAR_CTS2(x)         (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_PSC2_PAR_CTS2_GPIO       (0)
#define MCF_PAD_PAR_PSC2_PAR_CTS2_BCLK       (0x80)
#define MCF_PAD_PAR_PSC2_PAR_CTS2_CTS        (0xC0)

/* Bit definitions and macros for MCF_PAD_PAR_PSC1 */
#define MCF_PAD_PAR_PSC1_PAR_TXD1            (0x4)
#define MCF_PAD_PAR_PSC1_PAR_RXD1            (0x8)
#define MCF_PAD_PAR_PSC1_PAR_RTS1(x)         (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_PSC1_PAR_RTS1_GPIO       (0)
#define MCF_PAD_PAR_PSC1_PAR_RTS1_FSYNC      (0x20)
#define MCF_PAD_PAR_PSC1_PAR_RTS1_RTS        (0x30)
#define MCF_PAD_PAR_PSC1_PAR_CTS1(x)         (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_PSC1_PAR_CTS1_GPIO       (0)
#define MCF_PAD_PAR_PSC1_PAR_CTS1_BCLK       (0x80)
#define MCF_PAD_PAR_PSC1_PAR_CTS1_CTS        (0xC0)

/* Bit definitions and macros for MCF_PAD_PAR_PSC0 */
#define MCF_PAD_PAR_PSC0_PAR_TXD0            (0x4)
#define MCF_PAD_PAR_PSC0_PAR_RXD0            (0x8)
#define MCF_PAD_PAR_PSC0_PAR_RTS0(x)         (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_PSC0_PAR_RTS0_GPIO       (0)
#define MCF_PAD_PAR_PSC0_PAR_RTS0_FSYNC      (0x20)
#define MCF_PAD_PAR_PSC0_PAR_RTS0_RTS        (0x30)
#define MCF_PAD_PAR_PSC0_PAR_CTS0(x)         (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_PSC0_PAR_CTS0_GPIO       (0)
#define MCF_PAD_PAR_PSC0_PAR_CTS0_BCLK       (0x80)
#define MCF_PAD_PAR_PSC0_PAR_CTS0_CTS        (0xC0)

/* Bit definitions and macros for MCF_PAD_PAR_DSPI */
#define MCF_PAD_PAR_DSPI_PAR_SOUT(x)         (((x)&0x3)<<0)
#define MCF_PAD_PAR_DSPI_PAR_SOUT_GPIO       (0)
#define MCF_PAD_PAR_DSPI_PAR_SOUT_TXD        (0x2)
#define MCF_PAD_PAR_DSPI_PAR_SOUT_SOUT       (0x3)
#define MCF_PAD_PAR_DSPI_PAR_SIN(x)          (((x)&0x3)<<0x2)
#define MCF_PAD_PAR_DSPI_PAR_SIN_GPIO        (0)
#define MCF_PAD_PAR_DSPI_PAR_SIN_RXD         (0x8)
#define MCF_PAD_PAR_DSPI_PAR_SIN_SIN         (0xC)
#define MCF_PAD_PAR_DSPI_PAR_SCK(x)          (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_DSPI_PAR_SCK_GPIO        (0)
#define MCF_PAD_PAR_DSPI_PAR_SCK_BCLK        (0x10)
#define MCF_PAD_PAR_DSPI_PAR_SCK_CTS         (0x20)
#define MCF_PAD_PAR_DSPI_PAR_SCK_SCK         (0x30)
#define MCF_PAD_PAR_DSPI_PAR_CS0(x)          (((x)&0x3)<<0x6)
#define MCF_PAD_PAR_DSPI_PAR_CS0_GPIO        (0)
#define MCF_PAD_PAR_DSPI_PAR_CS0_FSYNC       (0x40)
#define MCF_PAD_PAR_DSPI_PAR_CS0_RTS         (0x80)
#define MCF_PAD_PAR_DSPI_PAR_CS0_DSPICS0     (0xC0)
#define MCF_PAD_PAR_DSPI_PAR_CS2(x)          (((x)&0x3)<<0x8)
#define MCF_PAD_PAR_DSPI_PAR_CS2_GPIO        (0)
#define MCF_PAD_PAR_DSPI_PAR_CS2_TOUT2       (0x200)
#define MCF_PAD_PAR_DSPI_PAR_CS2_DSPICS2     (0x300)
#define MCF_PAD_PAR_DSPI_PAR_CS3(x)          (((x)&0x3)<<0xA)
#define MCF_PAD_PAR_DSPI_PAR_CS3_GPIO        (0)
#define MCF_PAD_PAR_DSPI_PAR_CS3_TOUT3       (0x800)
#define MCF_PAD_PAR_DSPI_PAR_CS3_DSPICS3     (0xC00)
#define MCF_PAD_PAR_DSPI_PAR_CS5             (0x1000)

/* Bit definitions and macros for MCF_PAD_PAR_TIMER */
#define MCF_PAD_PAR_TIMER_PAR_TOUT2          (0x1)
#define MCF_PAD_PAR_TIMER_PAR_TIN2(x)        (((x)&0x3)<<0x1)
#define MCF_PAD_PAR_TIMER_PAR_TIN2_IRQ2      (0x4)
#define MCF_PAD_PAR_TIMER_PAR_TIN2_TIN2      (0x6)
#define MCF_PAD_PAR_TIMER_PAR_TOUT3          (0x8)
#define MCF_PAD_PAR_TIMER_PAR_TIN3(x)        (((x)&0x3)<<0x4)
#define MCF_PAD_PAR_TIMER_PAR_TIN3_IRQ3      (0x20)
#define MCF_PAD_PAR_TIMER_PAR_TIN3_TIN3      (0x30)


#endif /* __MCF5475_PAD_H__ */
