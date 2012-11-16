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

#ifndef __MCF5475_XLB_H__
#define __MCF5475_XLB_H__


/*********************************************************************
*
* XL Bus Arbiter (XLB)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_XLB_XARB_CFG                     (*(volatile uint32_t*)(&__MBAR[0x240]))
#define MCF_XLB_XARB_VER                     (*(volatile uint32_t*)(&__MBAR[0x244]))
#define MCF_XLB_XARB_SR                      (*(volatile uint32_t*)(&__MBAR[0x248]))
#define MCF_XLB_XARB_IMR                     (*(volatile uint32_t*)(&__MBAR[0x24C]))
#define MCF_XLB_XARB_ADRCAP                  (*(volatile uint32_t*)(&__MBAR[0x250]))
#define MCF_XLB_XARB_SIGCAP                  (*(volatile uint32_t*)(&__MBAR[0x254]))
#define MCF_XLB_XARB_ADRTO                   (*(volatile uint32_t*)(&__MBAR[0x258]))
#define MCF_XLB_XARB_DATTO                   (*(volatile uint32_t*)(&__MBAR[0x25C]))
#define MCF_XLB_XARB_BUSTO                   (*(volatile uint32_t*)(&__MBAR[0x260]))
#define MCF_XLB_XARB_PRIEN                   (*(volatile uint32_t*)(&__MBAR[0x264]))
#define MCF_XLB_XARB_PRI                     (*(volatile uint32_t*)(&__MBAR[0x268]))


/* Bit definitions and macros for MCF_XLB_XARB_CFG */
#define MCF_XLB_XARB_CFG_AT                  (0x2)
#define MCF_XLB_XARB_CFG_DT                  (0x4)
#define MCF_XLB_XARB_CFG_BA                  (0x8)
#define MCF_XLB_XARB_CFG_PM(x)               (((x)&0x3)<<0x5)
#define MCF_XLB_XARB_CFG_SP(x)               (((x)&0x7)<<0x8)
#define MCF_XLB_XARB_CFG_PLDIS               (0x80000000)

/* Bit definitions and macros for MCF_XLB_XARB_VER */
#define MCF_XLB_XARB_VER_VER(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_XLB_XARB_SR */
#define MCF_XLB_XARB_SR_AT                   (0x1)
#define MCF_XLB_XARB_SR_DT                   (0x2)
#define MCF_XLB_XARB_SR_BA                   (0x4)
#define MCF_XLB_XARB_SR_TTM                  (0x8)
#define MCF_XLB_XARB_SR_ECW                  (0x10)
#define MCF_XLB_XARB_SR_TTR                  (0x20)
#define MCF_XLB_XARB_SR_TTA                  (0x40)
#define MCF_XLB_XARB_SR_MM                   (0x80)
#define MCF_XLB_XARB_SR_SEA                  (0x100)

/* Bit definitions and macros for MCF_XLB_XARB_IMR */
#define MCF_XLB_XARB_IMR_ATE                 (0x1)
#define MCF_XLB_XARB_IMR_DTE                 (0x2)
#define MCF_XLB_XARB_IMR_BAE                 (0x4)
#define MCF_XLB_XARB_IMR_TTME                (0x8)
#define MCF_XLB_XARB_IMR_ECWE                (0x10)
#define MCF_XLB_XARB_IMR_TTRE                (0x20)
#define MCF_XLB_XARB_IMR_TTAE                (0x40)
#define MCF_XLB_XARB_IMR_MME                 (0x80)
#define MCF_XLB_XARB_IMR_SEAE                (0x100)

/* Bit definitions and macros for MCF_XLB_XARB_ADRCAP */
#define MCF_XLB_XARB_ADRCAP_ADRCAP(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_XLB_XARB_SIGCAP */
#define MCF_XLB_XARB_SIGCAP_TT(x)            (((x)&0x1F)<<0)
#define MCF_XLB_XARB_SIGCAP_TBST             (0x20)
#define MCF_XLB_XARB_SIGCAP_TSIZ(x)          (((x)&0x7)<<0x7)

/* Bit definitions and macros for MCF_XLB_XARB_ADRTO */
#define MCF_XLB_XARB_ADRTO_ADRTO(x)          (((x)&0xFFFFFFF)<<0)

/* Bit definitions and macros for MCF_XLB_XARB_DATTO */
#define MCF_XLB_XARB_DATTO_DATTO(x)          (((x)&0xFFFFFFF)<<0)

/* Bit definitions and macros for MCF_XLB_XARB_BUSTO */
#define MCF_XLB_XARB_BUSTO_BUSTO(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_XLB_XARB_PRIEN */
#define MCF_XLB_XARB_PRIEN_M0                (0x1)
#define MCF_XLB_XARB_PRIEN_M2                (0x4)
#define MCF_XLB_XARB_PRIEN_M3                (0x8)

/* Bit definitions and macros for MCF_XLB_XARB_PRI */
#define MCF_XLB_XARB_PRI_M0P(x)              (((x)&0x7)<<0)
#define MCF_XLB_XARB_PRI_M2P(x)              (((x)&0x7)<<0x8)
#define MCF_XLB_XARB_PRI_M3P(x)              (((x)&0x7)<<0xC)


#endif /* __MCF5475_XLB_H__ */
