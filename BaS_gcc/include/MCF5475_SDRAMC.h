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

#ifndef __MCF5475_SDRAMC_H__
#define __MCF5475_SDRAMC_H__


/*********************************************************************
*
* Synchronous DRAM Controller (SDRAMC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SDRAMC_SDRAMDS                   (*(volatile uint32_t*)(&_MBAR[0x4]))
#define MCF_SDRAMC_CS0CFG                    (*(volatile uint32_t*)(&_MBAR[0x20]))
#define MCF_SDRAMC_CS1CFG                    (*(volatile uint32_t*)(&_MBAR[0x24]))
#define MCF_SDRAMC_CS2CFG                    (*(volatile uint32_t*)(&_MBAR[0x28]))
#define MCF_SDRAMC_CS3CFG                    (*(volatile uint32_t*)(&_MBAR[0x2C]))
#define MCF_SDRAMC_SDMR                      (*(volatile uint32_t*)(&_MBAR[0x100]))
#define MCF_SDRAMC_SDCR                      (*(volatile uint32_t*)(&_MBAR[0x104]))
#define MCF_SDRAMC_SDCFG1                    (*(volatile uint32_t*)(&_MBAR[0x108]))
#define MCF_SDRAMC_SDCFG2                    (*(volatile uint32_t*)(&_MBAR[0x10C]))
#define MCF_SDRAMC_CSCFG(x)                  (*(volatile uint32_t*)(&_MBAR[0x20 + ((x)*0x4)]))


/* Bit definitions and macros for MCF_SDRAMC_SDRAMDS */
#define MCF_SDRAMC_SDRAMDS_SB_D(x)           (((x)&0x3)<<0)
#define MCF_SDRAMC_SDRAMDS_SB_S(x)           (((x)&0x3)<<0x2)
#define MCF_SDRAMC_SDRAMDS_SB_A(x)           (((x)&0x3)<<0x4)
#define MCF_SDRAMC_SDRAMDS_SB_C(x)           (((x)&0x3)<<0x6)
#define MCF_SDRAMC_SDRAMDS_SB_E(x)           (((x)&0x3)<<0x8)
#define MCF_SDRAMC_SDRAMDS_DRIVE_24MA        (0)
#define MCF_SDRAMC_SDRAMDS_DRIVE_16MA        (0x1)
#define MCF_SDRAMC_SDRAMDS_DRIVE_8MA         (0x2)
#define MCF_SDRAMC_SDRAMDS_DRIVE_NONE        (0x3)

/* Bit definitions and macros for MCF_SDRAMC_CSCFG */
#define MCF_SDRAMC_CSCFG_CSSZ(x)             (((x)&0x1F)<<0)
#define MCF_SDRAMC_CSCFG_CSSZ_DISABLED       (0)
#define MCF_SDRAMC_CSCFG_CSSZ_1MBYTE         (0x13)
#define MCF_SDRAMC_CSCFG_CSSZ_2MBYTE         (0x14)
#define MCF_SDRAMC_CSCFG_CSSZ_4MBYTE         (0x15)
#define MCF_SDRAMC_CSCFG_CSSZ_8MBYTE         (0x16)
#define MCF_SDRAMC_CSCFG_CSSZ_16MBYTE        (0x17)
#define MCF_SDRAMC_CSCFG_CSSZ_32MBYTE        (0x18)
#define MCF_SDRAMC_CSCFG_CSSZ_64MBYTE        (0x19)
#define MCF_SDRAMC_CSCFG_CSSZ_128MBYTE       (0x1A)
#define MCF_SDRAMC_CSCFG_CSSZ_256MBYTE       (0x1B)
#define MCF_SDRAMC_CSCFG_CSSZ_512MBYTE       (0x1C)
#define MCF_SDRAMC_CSCFG_CSSZ_1GBYTE         (0x1D)
#define MCF_SDRAMC_CSCFG_CSSZ_2GBYTE         (0x1E)
#define MCF_SDRAMC_CSCFG_CSSZ_4GBYTE         (0x1F)
#define MCF_SDRAMC_CSCFG_CSBA(x)             (((x)&0xFFF)<<0x14)
#define MCF_SDRAMC_CSCFG_BA(x)               ((x)&0xFFF00000)

/* Bit definitions and macros for MCF_SDRAMC_SDMR */
#define MCF_SDRAMC_SDMR_CMD                  (0x10000)
#define MCF_SDRAMC_SDMR_AD(x)                (((x)&0xFFF)<<0x12)
#define MCF_SDRAMC_SDMR_BNKAD(x)             (((x)&0x3)<<0x1E)
#define MCF_SDRAMC_SDMR_BK_LMR               (0)
#define MCF_SDRAMC_SDMR_BK_LEMR              (0x40000000)

/* Bit definitions and macros for MCF_SDRAMC_SDCR */
#define MCF_SDRAMC_SDCR_IPALL                (0x2)
#define MCF_SDRAMC_SDCR_IREF                 (0x4)
#define MCF_SDRAMC_SDCR_BUFF                 (0x10)
#define MCF_SDRAMC_SDCR_DQS_OE(x)            (((x)&0xF)<<0x8)
#define MCF_SDRAMC_SDCR_RCNT(x)              (((x)&0x3F)<<0x10)
#define MCF_SDRAMC_SDCR_DRIVE                (0x400000)
#define MCF_SDRAMC_SDCR_AP                   (0x800000)
#define MCF_SDRAMC_SDCR_MUX(x)               (((x)&0x3)<<0x18)
#define MCF_SDRAMC_SDCR_REF                  (0x10000000)
#define MCF_SDRAMC_SDCR_DDR                  (0x20000000)
#define MCF_SDRAMC_SDCR_CKE                  (0x40000000)
#define MCF_SDRAMC_SDCR_MODE_EN              (0x80000000)

/* Bit definitions and macros for MCF_SDRAMC_SDCFG1 */
#define MCF_SDRAMC_SDCFG1_WTLAT(x)           (((x)&0x7)<<0x4)
#define MCF_SDRAMC_SDCFG1_REF2ACT(x)         (((x)&0xF)<<0x8)
#define MCF_SDRAMC_SDCFG1_PRE2ACT(x)         (((x)&0x7)<<0xC)
#define MCF_SDRAMC_SDCFG1_ACT2RW(x)          (((x)&0x7)<<0x10)
#define MCF_SDRAMC_SDCFG1_RDLAT(x)           (((x)&0xF)<<0x14)
#define MCF_SDRAMC_SDCFG1_SWT2RD(x)          (((x)&0x7)<<0x18)
#define MCF_SDRAMC_SDCFG1_SRD2RW(x)          (((x)&0xF)<<0x1C)

/* Bit definitions and macros for MCF_SDRAMC_SDCFG2 */
#define MCF_SDRAMC_SDCFG2_BL(x)              (((x)&0xF)<<0x10)
#define MCF_SDRAMC_SDCFG2_BRD2WT(x)          (((x)&0xF)<<0x14)
#define MCF_SDRAMC_SDCFG2_BWT2RW(x)          (((x)&0xF)<<0x18)
#define MCF_SDRAMC_SDCFG2_BRD2PRE(x)         (((x)&0xF)<<0x1C)


#endif /* __MCF5475_SDRAMC_H__ */
