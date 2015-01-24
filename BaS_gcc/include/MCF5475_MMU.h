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

#ifndef __MCF5475_MMU_H__
#define __MCF5475_MMU_H__


/*********************************************************************
*
* Memory Management Unit (MMU)
*
*********************************************************************/

/* Register read/write macros */

/* note the uint32_t_a - this is to avoid gcc warnings about pointer aliasing */
#define MCF_MMU_MMUCR                        (*(volatile uint32_t_a*)(&_MMUBAR[0]))
#define MCF_MMU_MMUOR                        (*(volatile uint32_t_a*)(&_MMUBAR[0x4]))
#define MCF_MMU_MMUSR                        (*(volatile uint32_t_a*)(&_MMUBAR[0x8]))
#define MCF_MMU_MMUAR                        (*(volatile uint32_t_a*)(&_MMUBAR[0x10]))
#define MCF_MMU_MMUTR                        (*(volatile uint32_t_a*)(&_MMUBAR[0x14]))
#define MCF_MMU_MMUDR                        (*(volatile uint32_t_a*)(&_MMUBAR[0x18]))


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
#define MCF_MMU_MMUTR_ID(x)                  (((x) & 0xFF) << 0x2)
#define MCF_MMU_MMUTR_VA(x)                  (((x)&0x3FFFFF)<<0xA)

/* Bit definitions and macros for MCF_MMU_MMUDR */
#define MCF_MMU_MMUDR_LK                     (0x2)
#define MCF_MMU_MMUDR_X                      (0x4)
#define MCF_MMU_MMUDR_W                      (0x8)
#define MCF_MMU_MMUDR_R                      (0x10)
#define MCF_MMU_MMUDR_SP                     (0x20)
#define MCF_MMU_MMUDR_CM(x)                  (((x) & 0x3) << 0x6)
#define MCF_MMU_MMUDR_SZ(x)                  (((x) & 0x3) << 0x8)
#define MCF_MMU_MMUDR_PA(x)                  (((x) & 0x3FFFFF) << 0xA)


#endif /* __MCF5475_MMU_H__ */
