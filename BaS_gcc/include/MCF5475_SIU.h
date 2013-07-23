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

#ifndef __MCF5475_SIU_H__
#define __MCF5475_SIU_H__


/*********************************************************************
*
* System Integration Unit (SIU)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SIU_SBCR                         (*(volatile uint32_t*)(&_MBAR[0x10]))
#define MCF_SIU_SECSACR                      (*(volatile uint32_t*)(&_MBAR[0x38]))
#define MCF_SIU_RSR                          (*(volatile uint32_t*)(&_MBAR[0x44]))
#define MCF_SIU_JTAGID                       (*(volatile uint32_t*)(&_MBAR[0x50]))


/* Bit definitions and macros for MCF_SIU_SBCR */
#define MCF_SIU_SBCR_PIN2DSPI                (0x8000000)
#define MCF_SIU_SBCR_DMA2CPU                 (0x10000000)
#define MCF_SIU_SBCR_CPU2DMA                 (0x20000000)
#define MCF_SIU_SBCR_PIN2DMA                 (0x40000000)
#define MCF_SIU_SBCR_PIN2CPU                 (0x80000000)

/* Bit definitions and macros for MCF_SIU_SECSACR */
#define MCF_SIU_SECSACR_SEQEN                (0x1)

/* Bit definitions and macros for MCF_SIU_RSR */
#define MCF_SIU_RSR_RST                      (0x1)
#define MCF_SIU_RSR_RSTWD                    (0x2)
#define MCF_SIU_RSR_RSTJTG                   (0x8)

/* Bit definitions and macros for MCF_SIU_JTAGID */
#define MCF_SIU_JTAGID_JTAGID(x)             (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SIU_JTAGID */
#define MCF_SIU_JTAGID_REV          (0xF0000000)
#define MCF_SIU_JTAGID_PROCESSOR    (0x0FFFFFFF)
#define MCF_SIU_JTAGID_MCF5485      (0x0800C01D)
#define MCF_SIU_JTAGID_MCF5484      (0x0800D01D)
#define MCF_SIU_JTAGID_MCF5483      (0x0800E01D)
#define MCF_SIU_JTAGID_MCF5482      (0x0800F01D)
#define MCF_SIU_JTAGID_MCF5481      (0x0801001D)
#define MCF_SIU_JTAGID_MCF5480      (0x0801101D)
#define MCF_SIU_JTAGID_MCF5475      (0x0801201D)
#define MCF_SIU_JTAGID_MCF5474      (0x0801301D)
#define MCF_SIU_JTAGID_MCF5473      (0x0801401D)
#define MCF_SIU_JTAGID_MCF5472      (0x0801501D)
#define MCF_SIU_JTAGID_MCF5471      (0x0801601D)
#define MCF_SIU_JTAGID_MCF5470      (0x0801701D)

#endif /* __MCF5475_SIU_H__ */
