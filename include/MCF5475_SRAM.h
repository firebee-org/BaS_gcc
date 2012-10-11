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

#ifndef __MCF5475_SRAM_H__
#define __MCF5475_SRAM_H__


/*********************************************************************
*
* System SRAM Module (SRAM)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SRAM_SSCR                        (*(vuint32*)(&__MBAR[0x1FFC0]))
#define MCF_SRAM_TCCR                        (*(vuint32*)(&__MBAR[0x1FFC4]))
#define MCF_SRAM_TCCRDR                      (*(vuint32*)(&__MBAR[0x1FFC8]))
#define MCF_SRAM_TCCRDW                      (*(vuint32*)(&__MBAR[0x1FFCC]))
#define MCF_SRAM_TCCRSEC                     (*(vuint32*)(&__MBAR[0x1FFD0]))


/* Bit definitions and macros for MCF_SRAM_SSCR */
#define MCF_SRAM_SSCR_INLV                   (0x10000)

/* Bit definitions and macros for MCF_SRAM_TCCR */
#define MCF_SRAM_TCCR_BANK0_TC(x)            (((x)&0xF)<<0)
#define MCF_SRAM_TCCR_BANK1_TC(x)            (((x)&0xF)<<0x8)
#define MCF_SRAM_TCCR_BANK2_TC(x)            (((x)&0xF)<<0x10)
#define MCF_SRAM_TCCR_BANK3_TC(x)            (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_SRAM_TCCRDR */
#define MCF_SRAM_TCCRDR_BANK0_TC(x)          (((x)&0xF)<<0)
#define MCF_SRAM_TCCRDR_BANK1_TC(x)          (((x)&0xF)<<0x8)
#define MCF_SRAM_TCCRDR_BANK2_TC(x)          (((x)&0xF)<<0x10)
#define MCF_SRAM_TCCRDR_BANK3_TC(x)          (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_SRAM_TCCRDW */
#define MCF_SRAM_TCCRDW_BANK0_TC(x)          (((x)&0xF)<<0)
#define MCF_SRAM_TCCRDW_BANK1_TC(x)          (((x)&0xF)<<0x8)
#define MCF_SRAM_TCCRDW_BANK2_TC(x)          (((x)&0xF)<<0x10)
#define MCF_SRAM_TCCRDW_BANK3_TC(x)          (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_SRAM_TCCRSEC */
#define MCF_SRAM_TCCRSEC_BANK0_TC(x)         (((x)&0xF)<<0)
#define MCF_SRAM_TCCRSEC_BANK1_TC(x)         (((x)&0xF)<<0x8)
#define MCF_SRAM_TCCRSEC_BANK2_TC(x)         (((x)&0xF)<<0x10)
#define MCF_SRAM_TCCRSEC_BANK3_TC(x)         (((x)&0xF)<<0x18)


#endif /* __MCF5475_SRAM_H__ */
