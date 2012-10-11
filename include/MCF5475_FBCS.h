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

#ifndef __MCF5475_FBCS_H__
#define __MCF5475_FBCS_H__


/*********************************************************************
*
* FlexBus Chip Select Module (FBCS)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_FBCS0_CSAR                       (*(vuint32*)(&__MBAR[0x500]))
#define MCF_FBCS0_CSMR                       (*(vuint32*)(&__MBAR[0x504]))
#define MCF_FBCS0_CSCR                       (*(vuint32*)(&__MBAR[0x508]))

#define MCF_FBCS1_CSAR                       (*(vuint32*)(&__MBAR[0x50C]))
#define MCF_FBCS1_CSMR                       (*(vuint32*)(&__MBAR[0x510]))
#define MCF_FBCS1_CSCR                       (*(vuint32*)(&__MBAR[0x514]))

#define MCF_FBCS2_CSAR                       (*(vuint32*)(&__MBAR[0x518]))
#define MCF_FBCS2_CSMR                       (*(vuint32*)(&__MBAR[0x51C]))
#define MCF_FBCS2_CSCR                       (*(vuint32*)(&__MBAR[0x520]))

#define MCF_FBCS3_CSAR                       (*(vuint32*)(&__MBAR[0x524]))
#define MCF_FBCS3_CSMR                       (*(vuint32*)(&__MBAR[0x528]))
#define MCF_FBCS3_CSCR                       (*(vuint32*)(&__MBAR[0x52C]))

#define MCF_FBCS4_CSAR                       (*(vuint32*)(&__MBAR[0x530]))
#define MCF_FBCS4_CSMR                       (*(vuint32*)(&__MBAR[0x534]))
#define MCF_FBCS4_CSCR                       (*(vuint32*)(&__MBAR[0x538]))

#define MCF_FBCS5_CSAR                       (*(vuint32*)(&__MBAR[0x53C]))
#define MCF_FBCS5_CSMR                       (*(vuint32*)(&__MBAR[0x540]))
#define MCF_FBCS5_CSCR                       (*(vuint32*)(&__MBAR[0x544]))

#define MCF_FBCS_CSAR(x)                     (*(vuint32*)(&__MBAR[0x500 + ((x)*0xC)]))
#define MCF_FBCS_CSMR(x)                     (*(vuint32*)(&__MBAR[0x504 + ((x)*0xC)]))
#define MCF_FBCS_CSCR(x)                     (*(vuint32*)(&__MBAR[0x508 + ((x)*0xC)]))


/* Bit definitions and macros for MCF_FBCS_CSAR */
#define MCF_FBCS_CSAR_BA(x)                  ((x)&0xFFFF0000)

/* Bit definitions and macros for MCF_FBCS_CSMR */
#define MCF_FBCS_CSMR_V                      (0x1)
#define MCF_FBCS_CSMR_WP                     (0x100)
#define MCF_FBCS_CSMR_BAM(x)                 (((x)&0xFFFF)<<0x10)
#define MCF_FBCS_CSMR_BAM_4G                 (0xFFFF0000)
#define MCF_FBCS_CSMR_BAM_2G                 (0x7FFF0000)
#define MCF_FBCS_CSMR_BAM_1G                 (0x3FFF0000)
#define MCF_FBCS_CSMR_BAM_1024M              (0x3FFF0000)
#define MCF_FBCS_CSMR_BAM_512M               (0x1FFF0000)
#define MCF_FBCS_CSMR_BAM_256M               (0xFFF0000)
#define MCF_FBCS_CSMR_BAM_128M               (0x7FF0000)
#define MCF_FBCS_CSMR_BAM_64M                (0x3FF0000)
#define MCF_FBCS_CSMR_BAM_32M                (0x1FF0000)
#define MCF_FBCS_CSMR_BAM_16M                (0xFF0000)
#define MCF_FBCS_CSMR_BAM_8M                 (0x7F0000)
#define MCF_FBCS_CSMR_BAM_4M                 (0x3F0000)
#define MCF_FBCS_CSMR_BAM_2M                 (0x1F0000)
#define MCF_FBCS_CSMR_BAM_1M                 (0xF0000)
#define MCF_FBCS_CSMR_BAM_1024K              (0xF0000)
#define MCF_FBCS_CSMR_BAM_512K               (0x70000)
#define MCF_FBCS_CSMR_BAM_256K               (0x30000)
#define MCF_FBCS_CSMR_BAM_128K               (0x10000)
#define MCF_FBCS_CSMR_BAM_64K                (0)

/* Bit definitions and macros for MCF_FBCS_CSCR */
#define MCF_FBCS_CSCR_BSTW                   (0x8)
#define MCF_FBCS_CSCR_BSTR                   (0x10)
#define MCF_FBCS_CSCR_BEM                    (0x20)
#define MCF_FBCS_CSCR_PS(x)                  (((x)&0x3)<<0x6)
#define MCF_FBCS_CSCR_PS_32                  (0)
#define MCF_FBCS_CSCR_PS_8                   (0x40)
#define MCF_FBCS_CSCR_PS_16                  (0x80)
#define MCF_FBCS_CSCR_AA                     (0x100)
#define MCF_FBCS_CSCR_WS(x)                  (((x)&0x3F)<<0xA)
#define MCF_FBCS_CSCR_WRAH(x)                (((x)&0x3)<<0x10)
#define MCF_FBCS_CSCR_RDAH(x)                (((x)&0x3)<<0x12)
#define MCF_FBCS_CSCR_ASET(x)                (((x)&0x3)<<0x14)
#define MCF_FBCS_CSCR_SWSEN                  (0x800000)
#define MCF_FBCS_CSCR_SWS(x)                 (((x)&0x3F)<<0x1A)


#endif /* __MCF5475_FBCS_H__ */
