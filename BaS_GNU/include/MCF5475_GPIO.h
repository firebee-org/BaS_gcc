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

#ifndef __MCF5475_GPIO_H__
#define __MCF5475_GPIO_H__


/*********************************************************************
*
* General Purpose I/O (GPIO)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_GPIO_PODR_FBCTL                  (*(volatile uint8_t *)(&__MBAR[0xA00]))
#define MCF_GPIO_PDDR_FBCTL                  (*(volatile uint8_t *)(&__MBAR[0xA10]))
#define MCF_GPIO_PPDSDR_FBCTL                (*(volatile uint8_t *)(&__MBAR[0xA20]))
#define MCF_GPIO_PCLRR_FBCTL                 (*(volatile uint8_t *)(&__MBAR[0xA30]))

#define MCF_GPIO_PODR_FBCS                   (*(volatile uint8_t *)(&__MBAR[0xA01]))
#define MCF_GPIO_PDDR_FBCS                   (*(volatile uint8_t *)(&__MBAR[0xA11]))
#define MCF_GPIO_PPDSDR_FBCS                 (*(volatile uint8_t *)(&__MBAR[0xA21]))
#define MCF_GPIO_PCLRR_FBCS                  (*(volatile uint8_t *)(&__MBAR[0xA31]))

#define MCF_GPIO_PODR_DMA                    (*(volatile uint8_t *)(&__MBAR[0xA02]))
#define MCF_GPIO_PDDR_DMA                    (*(volatile uint8_t *)(&__MBAR[0xA12]))
#define MCF_GPIO_PPDSDR_DMA                  (*(volatile uint8_t *)(&__MBAR[0xA22]))
#define MCF_GPIO_PCLRR_DMA                   (*(volatile uint8_t *)(&__MBAR[0xA32]))

#define MCF_GPIO_PODR_FEC0H                  (*(volatile uint8_t *)(&__MBAR[0xA04]))
#define MCF_GPIO_PDDR_FEC0H                  (*(volatile uint8_t *)(&__MBAR[0xA14]))
#define MCF_GPIO_PPDSDR_FEC0H                (*(volatile uint8_t *)(&__MBAR[0xA24]))
#define MCF_GPIO_PCLRR_FEC0H                 (*(volatile uint8_t *)(&__MBAR[0xA34]))

#define MCF_GPIO_PODR_FEC0L                  (*(volatile uint8_t *)(&__MBAR[0xA05]))
#define MCF_GPIO_PDDR_FEC0L                  (*(volatile uint8_t *)(&__MBAR[0xA15]))
#define MCF_GPIO_PPDSDR_FEC0L                (*(volatile uint8_t *)(&__MBAR[0xA25]))
#define MCF_GPIO_PCLRR_FEC0L                 (*(volatile uint8_t *)(&__MBAR[0xA35]))

#define MCF_GPIO_PODR_FEC1H                  (*(volatile uint8_t *)(&__MBAR[0xA06]))
#define MCF_GPIO_PDDR_FEC1H                  (*(volatile uint8_t *)(&__MBAR[0xA16]))
#define MCF_GPIO_PPDSDR_FEC1H                (*(volatile uint8_t *)(&__MBAR[0xA26]))
#define MCF_GPIO_PCLRR_FEC1H                 (*(volatile uint8_t *)(&__MBAR[0xA36]))

#define MCF_GPIO_PODR_FEC1L                  (*(volatile uint8_t *)(&__MBAR[0xA07]))
#define MCF_GPIO_PDDR_FEC1L                  (*(volatile uint8_t *)(&__MBAR[0xA17]))
#define MCF_GPIO_PPDSDR_FEC1L                (*(volatile uint8_t *)(&__MBAR[0xA27]))
#define MCF_GPIO_PCLRR_FEC1L                 (*(volatile uint8_t *)(&__MBAR[0xA37]))

#define MCF_GPIO_PODR_FECI2C                 (*(volatile uint8_t *)(&__MBAR[0xA08]))
#define MCF_GPIO_PDDR_FECI2C                 (*(volatile uint8_t *)(&__MBAR[0xA18]))
#define MCF_GPIO_PPDSDR_FECI2C               (*(volatile uint8_t *)(&__MBAR[0xA28]))
#define MCF_GPIO_PCLRR_FECI2C                (*(volatile uint8_t *)(&__MBAR[0xA38]))

#define MCF_GPIO_PODR_PCIBG                  (*(volatile uint8_t *)(&__MBAR[0xA09]))
#define MCF_GPIO_PDDR_PCIBG                  (*(volatile uint8_t *)(&__MBAR[0xA19]))
#define MCF_GPIO_PPDSDR_PCIBG                (*(volatile uint8_t *)(&__MBAR[0xA29]))
#define MCF_GPIO_PCLRR_PCIBG                 (*(volatile uint8_t *)(&__MBAR[0xA39]))

#define MCF_GPIO_PODR_PCIBR                  (*(volatile uint8_t *)(&__MBAR[0xA0A]))
#define MCF_GPIO_PDDR_PCIBR                  (*(volatile uint8_t *)(&__MBAR[0xA1A]))
#define MCF_GPIO_PPDSDR_PCIBR                (*(volatile uint8_t *)(&__MBAR[0xA2A]))
#define MCF_GPIO_PCLRR_PCIBR                 (*(volatile uint8_t *)(&__MBAR[0xA3A]))

#define MCF_GPIO2_PODR_PSC3PSC               (*(volatile uint8_t *)(&__MBAR[0xA0C]))
#define MCF_GPIO2_PDDR_PSC3PSC               (*(volatile uint8_t *)(&__MBAR[0xA1C]))
#define MCF_GPIO2_PPDSDR_PSC3PSC             (*(volatile uint8_t *)(&__MBAR[0xA2C]))
#define MCF_GPIO2_PCLRR_PSC3PSC              (*(volatile uint8_t *)(&__MBAR[0xA3C]))

#define MCF_GPIO0_PODR_PSC1PSC               (*(volatile uint8_t *)(&__MBAR[0xA0D]))
#define MCF_GPIO0_PDDR_PSC1PSC               (*(volatile uint8_t *)(&__MBAR[0xA1D]))
#define MCF_GPIO0_PPDSDR_PSC1PSC             (*(volatile uint8_t *)(&__MBAR[0xA2D]))
#define MCF_GPIO0_PCLRR_PSC1PSC              (*(volatile uint8_t *)(&__MBAR[0xA3D]))

#define MCF_GPIO_PODR_DSPI                   (*(volatile uint8_t *)(&__MBAR[0xA0E]))
#define MCF_GPIO_PDDR_DSPI                   (*(volatile uint8_t *)(&__MBAR[0xA1E]))
#define MCF_GPIO_PPDSDR_DSPI                 (*(volatile uint8_t *)(&__MBAR[0xA2E]))
#define MCF_GPIO_PCLRR_DSPI                  (*(volatile uint8_t *)(&__MBAR[0xA3E]))



/* Bit definitions and macros for MCF_GPIO_PODR_FBCTL */
#define MCF_GPIO_PODR_FBCTL_PODR_FBCTL0      (0x1)
#define MCF_GPIO_PODR_FBCTL_PODR_FBCTL1      (0x2)
#define MCF_GPIO_PODR_FBCTL_PODR_FBCTL2      (0x4)
#define MCF_GPIO_PODR_FBCTL_PODR_FBCTL3      (0x8)
#define MCF_GPIO_PODR_FBCTL_PODR_FBCTL4      (0x10)
#define MCF_GPIO_PODR_FBCTL_PODR_FBCTL5      (0x20)
#define MCF_GPIO_PODR_FBCTL_PODR_FBCTL6      (0x40)
#define MCF_GPIO_PODR_FBCTL_PODR_FBCTL7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_FBCTL */
#define MCF_GPIO_PDDR_FBCTL_PDDR_FBCTL0      (0x1)
#define MCF_GPIO_PDDR_FBCTL_PDDR_FBCTL1      (0x2)
#define MCF_GPIO_PDDR_FBCTL_PDDR_FBCTL2      (0x4)
#define MCF_GPIO_PDDR_FBCTL_PDDR_FBCTL3      (0x8)
#define MCF_GPIO_PDDR_FBCTL_PDDR_FBCTL4      (0x10)
#define MCF_GPIO_PDDR_FBCTL_PDDR_FBCTL5      (0x20)
#define MCF_GPIO_PDDR_FBCTL_PDDR_FBCTL6      (0x40)
#define MCF_GPIO_PDDR_FBCTL_PDDR_FBCTL7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_FBCTL */
#define MCF_GPIO_PPDSDR_FBCTL_PPDSDR_FBCTL0  (0x1)
#define MCF_GPIO_PPDSDR_FBCTL_PPDSDR_FBCTL1  (0x2)
#define MCF_GPIO_PPDSDR_FBCTL_PPDSDR_FBCTL2  (0x4)
#define MCF_GPIO_PPDSDR_FBCTL_PPDSDR_FBCTL3  (0x8)
#define MCF_GPIO_PPDSDR_FBCTL_PPDSDR_FBCTL4  (0x10)
#define MCF_GPIO_PPDSDR_FBCTL_PPDSDR_FBCTL5  (0x20)
#define MCF_GPIO_PPDSDR_FBCTL_PPDSDR_FBCTL6  (0x40)
#define MCF_GPIO_PPDSDR_FBCTL_PPDSDR_FBCTL7  (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_FBCTL */
#define MCF_GPIO_PCLRR_FBCTL_PCLRR_FBCTL0    (0x1)
#define MCF_GPIO_PCLRR_FBCTL_PCLRR_FBCTL1    (0x2)
#define MCF_GPIO_PCLRR_FBCTL_PCLRR_FBCTL2    (0x4)
#define MCF_GPIO_PCLRR_FBCTL_PCLRR_FBCTL3    (0x8)
#define MCF_GPIO_PCLRR_FBCTL_PCLRR_FBCTL4    (0x10)
#define MCF_GPIO_PCLRR_FBCTL_PCLRR_FBCTL5    (0x20)
#define MCF_GPIO_PCLRR_FBCTL_PCLRR_FBCTL6    (0x40)
#define MCF_GPIO_PCLRR_FBCTL_PCLRR_FBCTL7    (0x80)

/* Bit definitions and macros for MCF_GPIO_PODR_FBCS */
#define MCF_GPIO_PODR_FBCS_PODR_FBCS1        (0x2)
#define MCF_GPIO_PODR_FBCS_PODR_FBCS2        (0x4)
#define MCF_GPIO_PODR_FBCS_PODR_FBCS3        (0x8)
#define MCF_GPIO_PODR_FBCS_PODR_FBCS4        (0x10)
#define MCF_GPIO_PODR_FBCS_PODR_FBCS5        (0x20)

/* Bit definitions and macros for MCF_GPIO_PDDR_FBCS */
#define MCF_GPIO_PDDR_FBCS_PDDR_FBCS1        (0x2)
#define MCF_GPIO_PDDR_FBCS_PDDR_FBCS2        (0x4)
#define MCF_GPIO_PDDR_FBCS_PDDR_FBCS3        (0x8)
#define MCF_GPIO_PDDR_FBCS_PDDR_FBCS4        (0x10)
#define MCF_GPIO_PDDR_FBCS_PDDR_FBCS5        (0x20)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_FBCS */
#define MCF_GPIO_PPDSDR_FBCS_PPDSDR_FBCS1    (0x2)
#define MCF_GPIO_PPDSDR_FBCS_PPDSDR_FBCS2    (0x4)
#define MCF_GPIO_PPDSDR_FBCS_PPDSDR_FBCS3    (0x8)
#define MCF_GPIO_PPDSDR_FBCS_PPDSDR_FBCS4    (0x10)
#define MCF_GPIO_PPDSDR_FBCS_PPDSDR_FBCS5    (0x20)

/* Bit definitions and macros for MCF_GPIO_PCLRR_FBCS */
#define MCF_GPIO_PCLRR_FBCS_PCLRR_FBCS1      (0x2)
#define MCF_GPIO_PCLRR_FBCS_PCLRR_FBCS2      (0x4)
#define MCF_GPIO_PCLRR_FBCS_PCLRR_FBCS3      (0x8)
#define MCF_GPIO_PCLRR_FBCS_PCLRR_FBCS4      (0x10)
#define MCF_GPIO_PCLRR_FBCS_PCLRR_FBCS5      (0x20)

/* Bit definitions and macros for MCF_GPIO_PODR_DMA */
#define MCF_GPIO_PODR_DMA_PODR_DMA0          (0x1)
#define MCF_GPIO_PODR_DMA_PODR_DMA1          (0x2)
#define MCF_GPIO_PODR_DMA_PODR_DMA2          (0x4)
#define MCF_GPIO_PODR_DMA_PODR_DMA3          (0x8)

/* Bit definitions and macros for MCF_GPIO_PDDR_DMA */
#define MCF_GPIO_PDDR_DMA_PDDR_DMA0          (0x1)
#define MCF_GPIO_PDDR_DMA_PDDR_DMA1          (0x2)
#define MCF_GPIO_PDDR_DMA_PDDR_DMA2          (0x4)
#define MCF_GPIO_PDDR_DMA_PDDR_DMA3          (0x8)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_DMA */
#define MCF_GPIO_PPDSDR_DMA_PPDSDR_DMA0      (0x1)
#define MCF_GPIO_PPDSDR_DMA_PPDSDR_DMA1      (0x2)
#define MCF_GPIO_PPDSDR_DMA_PPDSDR_DMA2      (0x4)
#define MCF_GPIO_PPDSDR_DMA_PPDSDR_DMA3      (0x8)

/* Bit definitions and macros for MCF_GPIO_PCLRR_DMA */
#define MCF_GPIO_PCLRR_DMA_PCLRR_DMA0        (0x1)
#define MCF_GPIO_PCLRR_DMA_PCLRR_DMA1        (0x2)
#define MCF_GPIO_PCLRR_DMA_PCLRR_DMA2        (0x4)
#define MCF_GPIO_PCLRR_DMA_PCLRR_DMA3        (0x8)

/* Bit definitions and macros for MCF_GPIO_PODR_FEC0H */
#define MCF_GPIO_PODR_FEC0H_PODR_FEC0H0      (0x1)
#define MCF_GPIO_PODR_FEC0H_PODR_FEC0H1      (0x2)
#define MCF_GPIO_PODR_FEC0H_PODR_FEC0H2      (0x4)
#define MCF_GPIO_PODR_FEC0H_PODR_FEC0H3      (0x8)
#define MCF_GPIO_PODR_FEC0H_PODR_FEC0H4      (0x10)
#define MCF_GPIO_PODR_FEC0H_PODR_FEC0H5      (0x20)
#define MCF_GPIO_PODR_FEC0H_PODR_FEC0H6      (0x40)
#define MCF_GPIO_PODR_FEC0H_PODR_FEC0H7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_FEC0H */
#define MCF_GPIO_PDDR_FEC0H_PDDR_FEC0H0      (0x1)
#define MCF_GPIO_PDDR_FEC0H_PDDR_FEC0H1      (0x2)
#define MCF_GPIO_PDDR_FEC0H_PDDR_FEC0H2      (0x4)
#define MCF_GPIO_PDDR_FEC0H_PDDR_FEC0H3      (0x8)
#define MCF_GPIO_PDDR_FEC0H_PDDR_FEC0H4      (0x10)
#define MCF_GPIO_PDDR_FEC0H_PDDR_FEC0H5      (0x20)
#define MCF_GPIO_PDDR_FEC0H_PDDR_FEC0H6      (0x40)
#define MCF_GPIO_PDDR_FEC0H_PDDR_FEC0H7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_FEC0H */
#define MCF_GPIO_PPDSDR_FEC0H_PPDSDR_FEC0H0  (0x1)
#define MCF_GPIO_PPDSDR_FEC0H_PPDSDR_FEC0H1  (0x2)
#define MCF_GPIO_PPDSDR_FEC0H_PPDSDR_FEC0H2  (0x4)
#define MCF_GPIO_PPDSDR_FEC0H_PPDSDR_FEC0H3  (0x8)
#define MCF_GPIO_PPDSDR_FEC0H_PPDSDR_FEC0H4  (0x10)
#define MCF_GPIO_PPDSDR_FEC0H_PPDSDR_FEC0H5  (0x20)
#define MCF_GPIO_PPDSDR_FEC0H_PPDSDR_FEC0H6  (0x40)
#define MCF_GPIO_PPDSDR_FEC0H_PPDSDR_FEC0H7  (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_FEC0H */
#define MCF_GPIO_PCLRR_FEC0H_PCLRR_FEC0H0    (0x1)
#define MCF_GPIO_PCLRR_FEC0H_PCLRR_FEC0H1    (0x2)
#define MCF_GPIO_PCLRR_FEC0H_PCLRR_FEC0H2    (0x4)
#define MCF_GPIO_PCLRR_FEC0H_PCLRR_FEC0H3    (0x8)
#define MCF_GPIO_PCLRR_FEC0H_PCLRR_FEC0H4    (0x10)
#define MCF_GPIO_PCLRR_FEC0H_PCLRR_FEC0H5    (0x20)
#define MCF_GPIO_PCLRR_FEC0H_PCLRR_FEC0H6    (0x40)
#define MCF_GPIO_PCLRR_FEC0H_PCLRR_FEC0H7    (0x80)

/* Bit definitions and macros for MCF_GPIO_PODR_FEC0L */
#define MCF_GPIO_PODR_FEC0L_PODR_FEC0L0      (0x1)
#define MCF_GPIO_PODR_FEC0L_PODR_FEC0L1      (0x2)
#define MCF_GPIO_PODR_FEC0L_PODR_FEC0L2      (0x4)
#define MCF_GPIO_PODR_FEC0L_PODR_FEC0L3      (0x8)
#define MCF_GPIO_PODR_FEC0L_PODR_FEC0L4      (0x10)
#define MCF_GPIO_PODR_FEC0L_PODR_FEC0L5      (0x20)
#define MCF_GPIO_PODR_FEC0L_PODR_FEC0L6      (0x40)
#define MCF_GPIO_PODR_FEC0L_PODR_FEC0L7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_FEC0L */
#define MCF_GPIO_PDDR_FEC0L_PDDR_FEC0L0      (0x1)
#define MCF_GPIO_PDDR_FEC0L_PDDR_FEC0L1      (0x2)
#define MCF_GPIO_PDDR_FEC0L_PDDR_FEC0L2      (0x4)
#define MCF_GPIO_PDDR_FEC0L_PDDR_FEC0L3      (0x8)
#define MCF_GPIO_PDDR_FEC0L_PDDR_FEC0L4      (0x10)
#define MCF_GPIO_PDDR_FEC0L_PDDR_FEC0L5      (0x20)
#define MCF_GPIO_PDDR_FEC0L_PDDR_FEC0L6      (0x40)
#define MCF_GPIO_PDDR_FEC0L_PDDR_FEC0L7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_FEC0L */
#define MCF_GPIO_PPDSDR_FEC0L_PPDSDR_FEC0L0  (0x1)
#define MCF_GPIO_PPDSDR_FEC0L_PPDSDR_FEC0L1  (0x2)
#define MCF_GPIO_PPDSDR_FEC0L_PPDSDR_FEC0L2  (0x4)
#define MCF_GPIO_PPDSDR_FEC0L_PPDSDR_FEC0L3  (0x8)
#define MCF_GPIO_PPDSDR_FEC0L_PPDSDR_FEC0L4  (0x10)
#define MCF_GPIO_PPDSDR_FEC0L_PPDSDR_FEC0L5  (0x20)
#define MCF_GPIO_PPDSDR_FEC0L_PPDSDR_FEC0L6  (0x40)
#define MCF_GPIO_PPDSDR_FEC0L_PPDSDR_FEC0L7  (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_FEC0L */
#define MCF_GPIO_PCLRR_FEC0L_PCLRR_FEC0L0    (0x1)
#define MCF_GPIO_PCLRR_FEC0L_PCLRR_FEC0L1    (0x2)
#define MCF_GPIO_PCLRR_FEC0L_PCLRR_FEC0L2    (0x4)
#define MCF_GPIO_PCLRR_FEC0L_PCLRR_FEC0L3    (0x8)
#define MCF_GPIO_PCLRR_FEC0L_PCLRR_FEC0L4    (0x10)
#define MCF_GPIO_PCLRR_FEC0L_PCLRR_FEC0L5    (0x20)
#define MCF_GPIO_PCLRR_FEC0L_PCLRR_FEC0L6    (0x40)
#define MCF_GPIO_PCLRR_FEC0L_PCLRR_FEC0L7    (0x80)

/* Bit definitions and macros for MCF_GPIO_PODR_FEC1H */
#define MCF_GPIO_PODR_FEC1H_PODR_FEC1H0      (0x1)
#define MCF_GPIO_PODR_FEC1H_PODR_FEC1H1      (0x2)
#define MCF_GPIO_PODR_FEC1H_PODR_FEC1H2      (0x4)
#define MCF_GPIO_PODR_FEC1H_PODR_FEC1H3      (0x8)
#define MCF_GPIO_PODR_FEC1H_PODR_FEC1H4      (0x10)
#define MCF_GPIO_PODR_FEC1H_PODR_FEC1H5      (0x20)
#define MCF_GPIO_PODR_FEC1H_PODR_FEC1H6      (0x40)
#define MCF_GPIO_PODR_FEC1H_PODR_FEC1H7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_FEC1H */
#define MCF_GPIO_PDDR_FEC1H_PDDR_FEC1H0      (0x1)
#define MCF_GPIO_PDDR_FEC1H_PDDR_FEC1H1      (0x2)
#define MCF_GPIO_PDDR_FEC1H_PDDR_FEC1H2      (0x4)
#define MCF_GPIO_PDDR_FEC1H_PDDR_FEC1H3      (0x8)
#define MCF_GPIO_PDDR_FEC1H_PDDR_FEC1H4      (0x10)
#define MCF_GPIO_PDDR_FEC1H_PDDR_FEC1H5      (0x20)
#define MCF_GPIO_PDDR_FEC1H_PDDR_FEC1H6      (0x40)
#define MCF_GPIO_PDDR_FEC1H_PDDR_FEC1H7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_FEC1H */
#define MCF_GPIO_PPDSDR_FEC1H_PPDSDR_FEC1H0  (0x1)
#define MCF_GPIO_PPDSDR_FEC1H_PPDSDR_FEC1H1  (0x2)
#define MCF_GPIO_PPDSDR_FEC1H_PPDSDR_FEC1H2  (0x4)
#define MCF_GPIO_PPDSDR_FEC1H_PPDSDR_FEC1H3  (0x8)
#define MCF_GPIO_PPDSDR_FEC1H_PPDSDR_FEC1H4  (0x10)
#define MCF_GPIO_PPDSDR_FEC1H_PPDSDR_FEC1H5  (0x20)
#define MCF_GPIO_PPDSDR_FEC1H_PPDSDR_FEC1H6  (0x40)
#define MCF_GPIO_PPDSDR_FEC1H_PPDSDR_FEC1H7  (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_FEC1H */
#define MCF_GPIO_PCLRR_FEC1H_PCLRR_FEC1H0    (0x1)
#define MCF_GPIO_PCLRR_FEC1H_PCLRR_FEC1H1    (0x2)
#define MCF_GPIO_PCLRR_FEC1H_PCLRR_FEC1H2    (0x4)
#define MCF_GPIO_PCLRR_FEC1H_PCLRR_FEC1H3    (0x8)
#define MCF_GPIO_PCLRR_FEC1H_PCLRR_FEC1H4    (0x10)
#define MCF_GPIO_PCLRR_FEC1H_PCLRR_FEC1H5    (0x20)
#define MCF_GPIO_PCLRR_FEC1H_PCLRR_FEC1H6    (0x40)
#define MCF_GPIO_PCLRR_FEC1H_PCLRR_FEC1H7    (0x80)

/* Bit definitions and macros for MCF_GPIO_PODR_FEC1L */
#define MCF_GPIO_PODR_FEC1L_PODR_FEC1L0      (0x1)
#define MCF_GPIO_PODR_FEC1L_PODR_FEC1L1      (0x2)
#define MCF_GPIO_PODR_FEC1L_PODR_FEC1L2      (0x4)
#define MCF_GPIO_PODR_FEC1L_PODR_FEC1L3      (0x8)
#define MCF_GPIO_PODR_FEC1L_PODR_FEC1L4      (0x10)
#define MCF_GPIO_PODR_FEC1L_PODR_FEC1L5      (0x20)
#define MCF_GPIO_PODR_FEC1L_PODR_FEC1L6      (0x40)
#define MCF_GPIO_PODR_FEC1L_PODR_FEC1L7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_FEC1L */
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L0      (0x1)
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L1      (0x2)
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L2      (0x4)
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L3      (0x8)
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L4      (0x10)
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L5      (0x20)
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L6      (0x40)
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L7      (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_FEC1L */
#define MCF_GPIO_PPDSDR_FEC1L_PPDSDR_FEC1L0  (0x1)
#define MCF_GPIO_PPDSDR_FEC1L_PPDSDR_FEC1L1  (0x2)
#define MCF_GPIO_PPDSDR_FEC1L_PPDSDR_FEC1L2  (0x4)
#define MCF_GPIO_PPDSDR_FEC1L_PPDSDR_FEC1L3  (0x8)
#define MCF_GPIO_PPDSDR_FEC1L_PPDSDR_FEC1L4  (0x10)
#define MCF_GPIO_PPDSDR_FEC1L_PPDSDR_FEC1L5  (0x20)
#define MCF_GPIO_PPDSDR_FEC1L_PPDSDR_FEC1L6  (0x40)
#define MCF_GPIO_PPDSDR_FEC1L_PPDSDR_FEC1L7  (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_FEC1L */
#define MCF_GPIO_PCLRR_FEC1L_PCLRR_FEC1L0    (0x1)
#define MCF_GPIO_PCLRR_FEC1L_PCLRR_FEC1L1    (0x2)
#define MCF_GPIO_PCLRR_FEC1L_PCLRR_FEC1L2    (0x4)
#define MCF_GPIO_PCLRR_FEC1L_PCLRR_FEC1L3    (0x8)
#define MCF_GPIO_PCLRR_FEC1L_PCLRR_FEC1L4    (0x10)
#define MCF_GPIO_PCLRR_FEC1L_PCLRR_FEC1L5    (0x20)
#define MCF_GPIO_PCLRR_FEC1L_PCLRR_FEC1L6    (0x40)
#define MCF_GPIO_PCLRR_FEC1L_PCLRR_FEC1L7    (0x80)

/* Bit definitions and macros for MCF_GPIO_PODR_FECI2C */
#define MCF_GPIO_PODR_FECI2C_PODR_FECI2C0    (0x1)
#define MCF_GPIO_PODR_FECI2C_PODR_FECI2C1    (0x2)
#define MCF_GPIO_PODR_FECI2C_PODR_FECI2C2    (0x4)
#define MCF_GPIO_PODR_FECI2C_PODR_FECI2C3    (0x8)

/* Bit definitions and macros for MCF_GPIO_PDDR_FECI2C */
#define MCF_GPIO_PDDR_FECI2C_PDDR_FECI2C0    (0x1)
#define MCF_GPIO_PDDR_FECI2C_PDDR_FECI2C1    (0x2)
#define MCF_GPIO_PDDR_FECI2C_PDDR_FECI2C2    (0x4)
#define MCF_GPIO_PDDR_FECI2C_PDDR_FECI2C3    (0x8)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_FECI2C */
#define MCF_GPIO_PPDSDR_FECI2C_PPDSDR_FECI2C0 (0x1)
#define MCF_GPIO_PPDSDR_FECI2C_PPDSDR_FECI2C1 (0x2)
#define MCF_GPIO_PPDSDR_FECI2C_PPDSDR_FECI2C2 (0x4)
#define MCF_GPIO_PPDSDR_FECI2C_PPDSDR_FECI2C3 (0x8)

/* Bit definitions and macros for MCF_GPIO_PCLRR_FECI2C */
#define MCF_GPIO_PCLRR_FECI2C_PCLRR_FECI2C0  (0x1)
#define MCF_GPIO_PCLRR_FECI2C_PCLRR_FECI2C1  (0x2)
#define MCF_GPIO_PCLRR_FECI2C_PCLRR_FECI2C2  (0x4)
#define MCF_GPIO_PCLRR_FECI2C_PCLRR_FECI2C3  (0x8)

/* Bit definitions and macros for MCF_GPIO_PODR_PCIBG */
#define MCF_GPIO_PODR_PCIBG_PODR_PCIBG0      (0x1)
#define MCF_GPIO_PODR_PCIBG_PODR_PCIBG1      (0x2)
#define MCF_GPIO_PODR_PCIBG_PODR_PCIBG2      (0x4)
#define MCF_GPIO_PODR_PCIBG_PODR_PCIBG3      (0x8)
#define MCF_GPIO_PODR_PCIBG_PODR_PCIBG4      (0x10)

/* Bit definitions and macros for MCF_GPIO_PDDR_PCIBG */
#define MCF_GPIO_PDDR_PCIBG_PDDR_PCIBG0      (0x1)
#define MCF_GPIO_PDDR_PCIBG_PDDR_PCIBG1      (0x2)
#define MCF_GPIO_PDDR_PCIBG_PDDR_PCIBG2      (0x4)
#define MCF_GPIO_PDDR_PCIBG_PDDR_PCIBG3      (0x8)
#define MCF_GPIO_PDDR_PCIBG_PDDR_PCIBG4      (0x10)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_PCIBG */
#define MCF_GPIO_PPDSDR_PCIBG_PPDSDR_PCIBG0  (0x1)
#define MCF_GPIO_PPDSDR_PCIBG_PPDSDR_PCIBG1  (0x2)
#define MCF_GPIO_PPDSDR_PCIBG_PPDSDR_PCIBG2  (0x4)
#define MCF_GPIO_PPDSDR_PCIBG_PPDSDR_PCIBG3  (0x8)
#define MCF_GPIO_PPDSDR_PCIBG_PPDSDR_PCIBG4  (0x10)

/* Bit definitions and macros for MCF_GPIO_PCLRR_PCIBG */
#define MCF_GPIO_PCLRR_PCIBG_PCLRR_PCIBG0    (0x1)
#define MCF_GPIO_PCLRR_PCIBG_PCLRR_PCIBG1    (0x2)
#define MCF_GPIO_PCLRR_PCIBG_PCLRR_PCIBG2    (0x4)
#define MCF_GPIO_PCLRR_PCIBG_PCLRR_PCIBG3    (0x8)
#define MCF_GPIO_PCLRR_PCIBG_PCLRR_PCIBG4    (0x10)

/* Bit definitions and macros for MCF_GPIO_PODR_PCIBR */
#define MCF_GPIO_PODR_PCIBR_PODR_PCIBR0      (0x1)
#define MCF_GPIO_PODR_PCIBR_PODR_PCIBR1      (0x2)
#define MCF_GPIO_PODR_PCIBR_PODR_PCIBR2      (0x4)
#define MCF_GPIO_PODR_PCIBR_PODR_PCIBR3      (0x8)
#define MCF_GPIO_PODR_PCIBR_PODR_PCIBR4      (0x10)

/* Bit definitions and macros for MCF_GPIO_PDDR_PCIBR */
#define MCF_GPIO_PDDR_PCIBR_PDDR_PCIBR0      (0x1)
#define MCF_GPIO_PDDR_PCIBR_PDDR_PCIBR1      (0x2)
#define MCF_GPIO_PDDR_PCIBR_PDDR_PCIBR2      (0x4)
#define MCF_GPIO_PDDR_PCIBR_PDDR_PCIBR3      (0x8)
#define MCF_GPIO_PDDR_PCIBR_PDDR_PCIBR4      (0x10)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_PCIBR */
#define MCF_GPIO_PPDSDR_PCIBR_PPDSDR_PCIBR0  (0x1)
#define MCF_GPIO_PPDSDR_PCIBR_PPDSDR_PCIBR1  (0x2)
#define MCF_GPIO_PPDSDR_PCIBR_PPDSDR_PCIBR2  (0x4)
#define MCF_GPIO_PPDSDR_PCIBR_PPDSDR_PCIBR3  (0x8)
#define MCF_GPIO_PPDSDR_PCIBR_PPDSDR_PCIBR4  (0x10)

/* Bit definitions and macros for MCF_GPIO_PCLRR_PCIBR */
#define MCF_GPIO_PCLRR_PCIBR_PCLRR_PCIBR0    (0x1)
#define MCF_GPIO_PCLRR_PCIBR_PCLRR_PCIBR1    (0x2)
#define MCF_GPIO_PCLRR_PCIBR_PCLRR_PCIBR2    (0x4)
#define MCF_GPIO_PCLRR_PCIBR_PCLRR_PCIBR3    (0x8)
#define MCF_GPIO_PCLRR_PCIBR_PCLRR_PCIBR4    (0x10)

/* Bit definitions and macros for MCF_GPIO_PODR_PSC3PSC */
#define MCF_GPIO_PODR_PSC3PSC_PODR_PSC3PSC20 (0x1)
#define MCF_GPIO_PODR_PSC3PSC_PODR_PSC3PSC21 (0x2)
#define MCF_GPIO_PODR_PSC3PSC_PODR_PSC3PSC22 (0x4)
#define MCF_GPIO_PODR_PSC3PSC_PODR_PSC3PSC23 (0x8)
#define MCF_GPIO_PODR_PSC3PSC_PODR_PSC3PSC24 (0x10)
#define MCF_GPIO_PODR_PSC3PSC_PODR_PSC3PSC25 (0x20)
#define MCF_GPIO_PODR_PSC3PSC_PODR_PSC3PSC26 (0x40)
#define MCF_GPIO_PODR_PSC3PSC_PODR_PSC3PSC27 (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_PSC3PSC */
#define MCF_GPIO_PDDR_PSC3PSC_PDDR_PSC3PSC20 (0x1)
#define MCF_GPIO_PDDR_PSC3PSC_PDDR_PSC3PSC21 (0x2)
#define MCF_GPIO_PDDR_PSC3PSC_PDDR_PSC3PSC22 (0x4)
#define MCF_GPIO_PDDR_PSC3PSC_PDDR_PSC3PSC23 (0x8)
#define MCF_GPIO_PDDR_PSC3PSC_PDDR_PSC3PSC24 (0x10)
#define MCF_GPIO_PDDR_PSC3PSC_PDDR_PSC3PSC25 (0x20)
#define MCF_GPIO_PDDR_PSC3PSC_PDDR_PSC3PSC26 (0x40)
#define MCF_GPIO_PDDR_PSC3PSC_PDDR_PSC3PSC27 (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_PSC3PSC */
#define MCF_GPIO_PPDSDR_PSC3PSC_PPDSDR_PSC3PSC20 (0x1)
#define MCF_GPIO_PPDSDR_PSC3PSC_PPDSDR_PSC3PSC21 (0x2)
#define MCF_GPIO_PPDSDR_PSC3PSC_PPDSDR_PSC3PSC22 (0x4)
#define MCF_GPIO_PPDSDR_PSC3PSC_PPDSDR_PSC3PSC23 (0x8)
#define MCF_GPIO_PPDSDR_PSC3PSC_PPDSDR_PSC3PSC24 (0x10)
#define MCF_GPIO_PPDSDR_PSC3PSC_PPDSDR_PSC3PSC25 (0x20)
#define MCF_GPIO_PPDSDR_PSC3PSC_PPDSDR_PSC3PSC26 (0x40)
#define MCF_GPIO_PPDSDR_PSC3PSC_PPDSDR_PSC3PSC27 (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_PSC3PSC */
#define MCF_GPIO_PCLRR_PSC3PSC_PCLRR_PSC3PSC20 (0x1)
#define MCF_GPIO_PCLRR_PSC3PSC_PCLRR_PSC3PSC21 (0x2)
#define MCF_GPIO_PCLRR_PSC3PSC_PCLRR_PSC3PSC22 (0x4)
#define MCF_GPIO_PCLRR_PSC3PSC_PCLRR_PSC3PSC23 (0x8)
#define MCF_GPIO_PCLRR_PSC3PSC_PCLRR_PSC3PSC24 (0x10)
#define MCF_GPIO_PCLRR_PSC3PSC_PCLRR_PSC3PSC25 (0x20)
#define MCF_GPIO_PCLRR_PSC3PSC_PCLRR_PSC3PSC26 (0x40)
#define MCF_GPIO_PCLRR_PSC3PSC_PCLRR_PSC3PSC27 (0x80)

/* Bit definitions and macros for MCF_GPIO_PODR_PSC1PSC */
#define MCF_GPIO_PODR_PSC1PSC_PODR_PSC1PSC00 (0x1)
#define MCF_GPIO_PODR_PSC1PSC_PODR_PSC1PSC01 (0x2)
#define MCF_GPIO_PODR_PSC1PSC_PODR_PSC1PSC02 (0x4)
#define MCF_GPIO_PODR_PSC1PSC_PODR_PSC1PSC03 (0x8)
#define MCF_GPIO_PODR_PSC1PSC_PODR_PSC1PSC04 (0x10)
#define MCF_GPIO_PODR_PSC1PSC_PODR_PSC1PSC05 (0x20)
#define MCF_GPIO_PODR_PSC1PSC_PODR_PSC1PSC06 (0x40)
#define MCF_GPIO_PODR_PSC1PSC_PODR_PSC1PSC07 (0x80)

/* Bit definitions and macros for MCF_GPIO_PDDR_PSC1PSC */
#define MCF_GPIO_PDDR_PSC1PSC_PDDR_PSC1PSC00 (0x1)
#define MCF_GPIO_PDDR_PSC1PSC_PDDR_PSC1PSC01 (0x2)
#define MCF_GPIO_PDDR_PSC1PSC_PDDR_PSC1PSC02 (0x4)
#define MCF_GPIO_PDDR_PSC1PSC_PDDR_PSC1PSC03 (0x8)
#define MCF_GPIO_PDDR_PSC1PSC_PDDR_PSC1PSC04 (0x10)
#define MCF_GPIO_PDDR_PSC1PSC_PDDR_PSC1PSC05 (0x20)
#define MCF_GPIO_PDDR_PSC1PSC_PDDR_PSC1PSC06 (0x40)
#define MCF_GPIO_PDDR_PSC1PSC_PDDR_PSC1PSC07 (0x80)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_PSC1PSC */
#define MCF_GPIO_PPDSDR_PSC1PSC_PPDSDR_PSC1PSC00 (0x1)
#define MCF_GPIO_PPDSDR_PSC1PSC_PPDSDR_PSC1PSC01 (0x2)
#define MCF_GPIO_PPDSDR_PSC1PSC_PPDSDR_PSC1PSC02 (0x4)
#define MCF_GPIO_PPDSDR_PSC1PSC_PPDSDR_PSC1PSC03 (0x8)
#define MCF_GPIO_PPDSDR_PSC1PSC_PPDSDR_PSC1PSC04 (0x10)
#define MCF_GPIO_PPDSDR_PSC1PSC_PPDSDR_PSC1PSC05 (0x20)
#define MCF_GPIO_PPDSDR_PSC1PSC_PPDSDR_PSC1PSC06 (0x40)
#define MCF_GPIO_PPDSDR_PSC1PSC_PPDSDR_PSC1PSC07 (0x80)

/* Bit definitions and macros for MCF_GPIO_PCLRR_PSC1PSC */
#define MCF_GPIO_PCLRR_PSC1PSC_PCLRR_PSC1PSC00 (0x1)
#define MCF_GPIO_PCLRR_PSC1PSC_PCLRR_PSC1PSC01 (0x2)
#define MCF_GPIO_PCLRR_PSC1PSC_PCLRR_PSC1PSC02 (0x4)
#define MCF_GPIO_PCLRR_PSC1PSC_PCLRR_PSC1PSC03 (0x8)
#define MCF_GPIO_PCLRR_PSC1PSC_PCLRR_PSC1PSC04 (0x10)
#define MCF_GPIO_PCLRR_PSC1PSC_PCLRR_PSC1PSC05 (0x20)
#define MCF_GPIO_PCLRR_PSC1PSC_PCLRR_PSC1PSC06 (0x40)
#define MCF_GPIO_PCLRR_PSC1PSC_PCLRR_PSC1PSC07 (0x80)

/* Bit definitions and macros for MCF_GPIO_PODR_DSPI */
#define MCF_GPIO_PODR_DSPI_PODR_DSPI0        (0x1)
#define MCF_GPIO_PODR_DSPI_PODR_DSPI1        (0x2)
#define MCF_GPIO_PODR_DSPI_PODR_DSPI2        (0x4)
#define MCF_GPIO_PODR_DSPI_PODR_DSPI3        (0x8)
#define MCF_GPIO_PODR_DSPI_PODR_DSPI4        (0x10)
#define MCF_GPIO_PODR_DSPI_PODR_DSPI5        (0x20)
#define MCF_GPIO_PODR_DSPI_PODR_DSPI6        (0x40)

/* Bit definitions and macros for MCF_GPIO_PDDR_DSPI */
#define MCF_GPIO_PDDR_DSPI_PDDR_DSPI0        (0x1)
#define MCF_GPIO_PDDR_DSPI_PDDR_DSPI1        (0x2)
#define MCF_GPIO_PDDR_DSPI_PDDR_DSPI2        (0x4)
#define MCF_GPIO_PDDR_DSPI_PDDR_DSPI3        (0x8)
#define MCF_GPIO_PDDR_DSPI_PDDR_DSPI4        (0x10)
#define MCF_GPIO_PDDR_DSPI_PDDR_DSPI5        (0x20)
#define MCF_GPIO_PDDR_DSPI_PDDR_DSPI6        (0x40)

/* Bit definitions and macros for MCF_GPIO_PPDSDR_DSPI */
#define MCF_GPIO_PPDSDR_DSPI_PPDSDR_DSPI0    (0x1)
#define MCF_GPIO_PPDSDR_DSPI_PPDSDR_DSPI1    (0x2)
#define MCF_GPIO_PPDSDR_DSPI_PPDSDR_DSPI2    (0x4)
#define MCF_GPIO_PPDSDR_DSPI_PPDSDR_DSPI3    (0x8)
#define MCF_GPIO_PPDSDR_DSPI_PPDSDR_DSPI4    (0x10)
#define MCF_GPIO_PPDSDR_DSPI_PPDSDR_DSPI5    (0x20)
#define MCF_GPIO_PPDSDR_DSPI_PPDSDR_DSPI6    (0x40)

/* Bit definitions and macros for MCF_GPIO_PCLRR_DSPI */
#define MCF_GPIO_PCLRR_DSPI_PCLRR_DSPI0      (0x1)
#define MCF_GPIO_PCLRR_DSPI_PCLRR_DSPI1      (0x2)
#define MCF_GPIO_PCLRR_DSPI_PCLRR_DSPI2      (0x4)
#define MCF_GPIO_PCLRR_DSPI_PCLRR_DSPI3      (0x8)
#define MCF_GPIO_PCLRR_DSPI_PCLRR_DSPI4      (0x10)
#define MCF_GPIO_PCLRR_DSPI_PCLRR_DSPI5      (0x20)
#define MCF_GPIO_PCLRR_DSPI_PCLRR_DSPI6      (0x40)


#endif /* __MCF5475_GPIO_H__ */
