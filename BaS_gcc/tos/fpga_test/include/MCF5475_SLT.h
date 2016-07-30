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

#ifndef __MCF5475_SLT_H__
#define __MCF5475_SLT_H__


/*********************************************************************
*
* Slice Timers (SLT)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SLT0_STCNT                       (*(volatile uint32_t*)(&_MBAR[0x900]))
#define MCF_SLT0_SCR                         (*(volatile uint32_t*)(&_MBAR[0x904]))
#define MCF_SLT0_SCNT                        (*(volatile uint32_t*)(&_MBAR[0x908]))
#define MCF_SLT0_SSR                         (*(volatile uint32_t*)(&_MBAR[0x90C]))

#define MCF_SLT1_STCNT                       (*(volatile uint32_t*)(&_MBAR[0x910]))
#define MCF_SLT1_SCR                         (*(volatile uint32_t*)(&_MBAR[0x914]))
#define MCF_SLT1_SCNT                        (*(volatile uint32_t*)(&_MBAR[0x918]))
#define MCF_SLT1_SSR                         (*(volatile uint32_t*)(&_MBAR[0x91C]))

#define MCF_SLT_STCNT(x)                     (*(volatile uint32_t*)(&_MBAR[0x900 + ((x)*0x10)]))
#define MCF_SLT_SCR(x)                       (*(volatile uint32_t*)(&_MBAR[0x904 + ((x)*0x10)]))
#define MCF_SLT_SCNT(x)                      (*(volatile int32_t*)(&_MBAR[0x908 + ((x)*0x10)]))
#define MCF_SLT_SSR(x)                       (*(volatile uint32_t*)(&_MBAR[0x90C + ((x)*0x10)]))


/* Bit definitions and macros for MCF_SLT_STCNT */
#define MCF_SLT_STCNT_TC(x)                  (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SLT_SCR */
#define MCF_SLT_SCR_TEN                      (0x1000000)
#define MCF_SLT_SCR_IEN                      (0x2000000)
#define MCF_SLT_SCR_RUN                      (0x4000000)

/* Bit definitions and macros for MCF_SLT_SCNT */
#define MCF_SLT_SCNT_CNT(x)                  (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SLT_SSR */
#define MCF_SLT_SSR_ST                       (0x1000000)
#define MCF_SLT_SSR_BE                       (0x2000000)


#endif /* __MCF5475_SLT_H__ */
