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

#ifndef __MCF5475_CTM_H__
#define __MCF5475_CTM_H__


/*********************************************************************
*
* Comm Timer Module (CTM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_CTM_CTCR0                        (*(volatile uint32_t*)(&__MBAR[0x7F00]))
#define MCF_CTM_CTCR1                        (*(volatile uint32_t*)(&__MBAR[0x7F04]))
#define MCF_CTM_CTCR2                        (*(volatile uint32_t*)(&__MBAR[0x7F08]))
#define MCF_CTM_CTCR3                        (*(volatile uint32_t*)(&__MBAR[0x7F0C]))
#define MCF_CTM_CTCR4                        (*(volatile uint32_t*)(&__MBAR[0x7F10]))
#define MCF_CTM_CTCR5                        (*(volatile uint32_t*)(&__MBAR[0x7F14]))
#define MCF_CTM_CTCR6                        (*(volatile uint32_t*)(&__MBAR[0x7F18]))
#define MCF_CTM_CTCR7                        (*(volatile uint32_t*)(&__MBAR[0x7F1C]))
#define MCF_CTM_CTCRF(x)                     (*(volatile uint32_t*)(&__MBAR[0x7F00 + ((x)*0x4)]))
#define MCF_CTM_CTCRV(x)                     (*(volatile uint32_t*)(&__MBAR[0x7F10 + ((x-4)*0x4)]))


/* Bit definitions and macros for MCF_CTM_CTCRF */
#define MCF_CTM_CTCRF_CRV(x)                 (((x)&0xFFFF)<<0)
#define MCF_CTM_CTCRF_S(x)                   (((x)&0xF)<<0x10)
#define MCF_CTM_CTCRF_S_CLK_1                (0)
#define MCF_CTM_CTCRF_S_CLK_2                (0x10000)
#define MCF_CTM_CTCRF_S_CLK_4                (0x20000)
#define MCF_CTM_CTCRF_S_CLK_8                (0x30000)
#define MCF_CTM_CTCRF_S_CLK_16               (0x40000)
#define MCF_CTM_CTCRF_S_CLK_32               (0x50000)
#define MCF_CTM_CTCRF_S_CLK_64               (0x60000)
#define MCF_CTM_CTCRF_S_CLK_128              (0x70000)
#define MCF_CTM_CTCRF_S_CLK_256              (0x80000)
#define MCF_CTM_CTCRF_S_CLK_EXT              (0x90000)
#define MCF_CTM_CTCRF_PCT(x)                 (((x)&0x7)<<0x14)
#define MCF_CTM_CTCRF_PCT_100                (0)
#define MCF_CTM_CTCRF_PCT_50                 (0x100000)
#define MCF_CTM_CTCRF_PCT_25                 (0x200000)
#define MCF_CTM_CTCRF_PCT_12p5               (0x300000)
#define MCF_CTM_CTCRF_PCT_6p25               (0x400000)
#define MCF_CTM_CTCRF_PCT_OFF                (0x500000)
#define MCF_CTM_CTCRF_M                      (0x800000)
#define MCF_CTM_CTCRF_IM                     (0x1000000)
#define MCF_CTM_CTCRF_I                      (0x80000000)

/* Bit definitions and macros for MCF_CTM_CTCRV */
#define MCF_CTM_CTCRV_CRV(x)                 (((x)&0xFFFFFF)<<0)
#define MCF_CTM_CTCRV_PCT(x)                 (((x)&0x7)<<0x18)
#define MCF_CTM_CTCRV_PCT_100                (0)
#define MCF_CTM_CTCRV_PCT_50                 (0x1000000)
#define MCF_CTM_CTCRV_PCT_25                 (0x2000000)
#define MCF_CTM_CTCRV_PCT_12p5               (0x3000000)
#define MCF_CTM_CTCRV_PCT_6p25               (0x4000000)
#define MCF_CTM_CTCRV_PCT_OFF                (0x5000000)
#define MCF_CTM_CTCRV_M                      (0x8000000)
#define MCF_CTM_CTCRV_S                      (0x10000000)


#endif /* __MCF5475_CTM_H__ */
