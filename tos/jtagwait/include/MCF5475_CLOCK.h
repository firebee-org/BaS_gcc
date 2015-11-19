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

#ifndef __MCF5475_CLOCK_H__
#define __MCF5475_CLOCK_H__


/*********************************************************************
*
* Clock Module (CLOCK)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_CLOCK_SPCR                       (*(volatile uint32_t*)(&_MBAR[0x300]))


/* Bit definitions and macros for MCF_CLOCK_SPCR */
#define MCF_CLOCK_SPCR_MEMEN                 (0x1)
#define MCF_CLOCK_SPCR_PCIEN                 (0x2)
#define MCF_CLOCK_SPCR_FBEN                  (0x4)
#define MCF_CLOCK_SPCR_CAN0EN                (0x8)
#define MCF_CLOCK_SPCR_DMAEN                 (0x10)
#define MCF_CLOCK_SPCR_FEC0EN                (0x20)
#define MCF_CLOCK_SPCR_FEC1EN                (0x40)
#define MCF_CLOCK_SPCR_USBEN                 (0x80)
#define MCF_CLOCK_SPCR_PSCEN                 (0x200)
#define MCF_CLOCK_SPCR_CAN1EN                (0x800)
#define MCF_CLOCK_SPCR_CRYENA                (0x1000)
#define MCF_CLOCK_SPCR_CRYENB                (0x2000)
#define MCF_CLOCK_SPCR_COREN                 (0x4000)
#define MCF_CLOCK_SPCR_PLLK                  (0x80000000)


#endif /* __MCF5475_CLOCK_H__ */
