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

#ifndef __MCF5475_PCIARB_H__
#define __MCF5475_PCIARB_H__


/*********************************************************************
*
* PCI Bus Arbiter Module (PCIARB)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_PCIARB_PACR                      (*(volatile uint32_t*)(&_MBAR[0xC00]))
#define MCF_PCIARB_PASR                      (*(volatile uint32_t*)(&_MBAR[0xC04]))


/* Bit definitions and macros for MCF_PCIARB_PACR */
#define MCF_PCIARB_PACR_INTMPRI              (0x1)
#define MCF_PCIARB_PACR_EXTMPRI(x)           (((x)&0x1F)<<0x1)
#define MCF_PCIARB_PACR_INTMINTEN            (0x10000)
#define MCF_PCIARB_PACR_EXTMINTEN(x)         (((x)&0x1F)<<0x11)
#define MCF_PCIARB_PACR_DS                   (0x80000000)

/* Bit definitions and macros for MCF_PCIARB_PASR */
#define MCF_PCIARB_PASR_ITLMBK               (0x10000)
#define MCF_PCIARB_PASR_EXTMBK(x)            (((x)&0x1F)<<0x11)


#endif /* __MCF5475_PCIARB_H__ */
