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

#ifndef __MCF5475_SEC_H__
#define __MCF5475_SEC_H__


/*********************************************************************
*
* Integrated Security Engine (SEC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_SEC_EUACRH                       (*(volatile uint32_t*)(&__MBAR[0x21000]))
#define MCF_SEC_EUACRL                       (*(volatile uint32_t*)(&__MBAR[0x21004]))
#define MCF_SEC_SIMRH                        (*(volatile uint32_t*)(&__MBAR[0x21008]))
#define MCF_SEC_SIMRL                        (*(volatile uint32_t*)(&__MBAR[0x2100C]))
#define MCF_SEC_SISRH                        (*(volatile uint32_t*)(&__MBAR[0x21010]))
#define MCF_SEC_SISRL                        (*(volatile uint32_t*)(&__MBAR[0x21014]))
#define MCF_SEC_SICRH                        (*(volatile uint32_t*)(&__MBAR[0x21018]))
#define MCF_SEC_SICRL                        (*(volatile uint32_t*)(&__MBAR[0x2101C]))
#define MCF_SEC_SIDR                         (*(volatile uint32_t*)(&__MBAR[0x21020]))
#define MCF_SEC_EUASRH                       (*(volatile uint32_t*)(&__MBAR[0x21028]))
#define MCF_SEC_EUASRL                       (*(volatile uint32_t*)(&__MBAR[0x2102C]))
#define MCF_SEC_SMCR                         (*(volatile uint32_t*)(&__MBAR[0x21030]))
#define MCF_SEC_MEAR                         (*(volatile uint32_t*)(&__MBAR[0x21038]))
#define MCF_SEC_CCCR0                        (*(volatile uint32_t*)(&__MBAR[0x2200C]))
#define MCF_SEC_CCPSRH0                      (*(volatile uint32_t*)(&__MBAR[0x22010]))
#define MCF_SEC_CCPSRL0                      (*(volatile uint32_t*)(&__MBAR[0x22014]))
#define MCF_SEC_CDPR0                        (*(volatile uint32_t*)(&__MBAR[0x22044]))
#define MCF_SEC_FR0                          (*(volatile uint32_t*)(&__MBAR[0x2204C]))
#define MCF_SEC_CCCR1                        (*(volatile uint32_t*)(&__MBAR[0x2300C]))
#define MCF_SEC_CCPSRH1                      (*(volatile uint32_t*)(&__MBAR[0x23010]))
#define MCF_SEC_CCPSRL1                      (*(volatile uint32_t*)(&__MBAR[0x23014]))
#define MCF_SEC_CDPR1                        (*(volatile uint32_t*)(&__MBAR[0x23044]))
#define MCF_SEC_FR1                          (*(volatile uint32_t*)(&__MBAR[0x2304C]))
#define MCF_SEC_AFRCR                        (*(volatile uint32_t*)(&__MBAR[0x28018]))
#define MCF_SEC_AFSR                         (*(volatile uint32_t*)(&__MBAR[0x28028]))
#define MCF_SEC_AFISR                        (*(volatile uint32_t*)(&__MBAR[0x28030]))
#define MCF_SEC_AFIMR                        (*(volatile uint32_t*)(&__MBAR[0x28038]))
#define MCF_SEC_DRCR                         (*(volatile uint32_t*)(&__MBAR[0x2A018]))
#define MCF_SEC_DSR                          (*(volatile uint32_t*)(&__MBAR[0x2A028]))
#define MCF_SEC_DISR                         (*(volatile uint32_t*)(&__MBAR[0x2A030]))
#define MCF_SEC_DIMR                         (*(volatile uint32_t*)(&__MBAR[0x2A038]))
#define MCF_SEC_MDRCR                        (*(volatile uint32_t*)(&__MBAR[0x2C018]))
#define MCF_SEC_MDSR                         (*(volatile uint32_t*)(&__MBAR[0x2C028]))
#define MCF_SEC_MDISR                        (*(volatile uint32_t*)(&__MBAR[0x2C030]))
#define MCF_SEC_MDIMR                        (*(volatile uint32_t*)(&__MBAR[0x2C038]))
#define MCF_SEC_RNGRCR                       (*(volatile uint32_t*)(&__MBAR[0x2E018]))
#define MCF_SEC_RNGSR                        (*(volatile uint32_t*)(&__MBAR[0x2E028]))
#define MCF_SEC_RNGISR                       (*(volatile uint32_t*)(&__MBAR[0x2E030]))
#define MCF_SEC_RNGIMR                       (*(volatile uint32_t*)(&__MBAR[0x2E038]))
#define MCF_SEC_AESRCR                       (*(volatile uint32_t*)(&__MBAR[0x32018]))
#define MCF_SEC_AESSR                        (*(volatile uint32_t*)(&__MBAR[0x32028]))
#define MCF_SEC_AESISR                       (*(volatile uint32_t*)(&__MBAR[0x32030]))
#define MCF_SEC_AESIMR                       (*(volatile uint32_t*)(&__MBAR[0x32038]))
#define MCF_SEC_CCCRn(x)                     (*(volatile uint32_t*)(&__MBAR[0x2200C + ((x)*0x1000)]))
#define MCF_SEC_CCPSRHn(x)                   (*(volatile uint32_t*)(&__MBAR[0x22010 + ((x)*0x1000)]))
#define MCF_SEC_CCPSRLn(x)                   (*(volatile uint32_t*)(&__MBAR[0x22014 + ((x)*0x1000)]))
#define MCF_SEC_CDPRn(x)                     (*(volatile uint32_t*)(&__MBAR[0x22044 + ((x)*0x1000)]))
#define MCF_SEC_FRn(x)                       (*(volatile uint32_t*)(&__MBAR[0x2204C + ((x)*0x1000)]))


/* Bit definitions and macros for MCF_SEC_EUACRH */
#define MCF_SEC_EUACRH_AFEU(x)               (((x)&0xF)<<0)
#define MCF_SEC_EUACRH_AFFEU_NOASSIGN        (0)
#define MCF_SEC_EUACRH_AFFEU_CHA0            (0x1)
#define MCF_SEC_EUACRH_AFFEU_CHA1            (0x2)
#define MCF_SEC_EUACRH_MDEU(x)               (((x)&0xF)<<0x8)
#define MCF_SEC_EUACRH_MDEU_NOASSIGN         (0)
#define MCF_SEC_EUACRH_MDEU_CHA0             (0x100)
#define MCF_SEC_EUACRH_MDEU_CHA1             (0x200)
#define MCF_SEC_EUACRH_RNG(x)                (((x)&0xF)<<0x18)
#define MCF_SEC_EUACRH_RNG_NOASSIGN          (0)
#define MCF_SEC_EUACRH_RNG_CHA0              (0x1000000)
#define MCF_SEC_EUACRH_RNG_CHA1              (0x2000000)

/* Bit definitions and macros for MCF_SEC_EUACRL */
#define MCF_SEC_EUACRL_AESU(x)               (((x)&0xF)<<0x10)
#define MCF_SEC_EUACRL_AESU_NOASSIGN         (0)
#define MCF_SEC_EUACRL_AESU_CHA0             (0x10000)
#define MCF_SEC_EUACRL_AESU_CHA1             (0x20000)
#define MCF_SEC_EUACRL_DEU(x)                (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_SEC_SIMRH */
#define MCF_SEC_SIMRH_AERR                   (0x8000000)
#define MCF_SEC_SIMRH_CHA_0_DN               (0x10000000)
#define MCF_SEC_SIMRH_CHA_0_ERR              (0x20000000)
#define MCF_SEC_SIMRH_CHA_1_DN               (0x40000000)
#define MCF_SEC_SIMRH_CHA_1_ERR              (0x80000000)

/* Bit definitions and macros for MCF_SEC_SIMRL */
#define MCF_SEC_SIMRL_TEA                    (0x40)
#define MCF_SEC_SIMRL_DEU_DN                 (0x100)
#define MCF_SEC_SIMRL_DEU_ERR                (0x200)
#define MCF_SEC_SIMRL_AESU_DN                (0x1000)
#define MCF_SEC_SIMRL_AESU_ERR               (0x2000)
#define MCF_SEC_SIMRL_MDEU_DN                (0x10000)
#define MCF_SEC_SIMRL_MDEU_ERR               (0x20000)
#define MCF_SEC_SIMRL_AFEU_DN                (0x100000)
#define MCF_SEC_SIMRL_AFEU_ERR               (0x200000)
#define MCF_SEC_SIMRL_RNG_DN                 (0x1000000)
#define MCF_SEC_SIMRL_RNG_ERR                (0x2000000)

/* Bit definitions and macros for MCF_SEC_SISRH */
#define MCF_SEC_SISRH_AERR                   (0x8000000)
#define MCF_SEC_SISRH_CHA_0_DN               (0x10000000)
#define MCF_SEC_SISRH_CHA_0_ERR              (0x20000000)
#define MCF_SEC_SISRH_CHA_1_DN               (0x40000000)
#define MCF_SEC_SISRH_CHA_1_ERR              (0x80000000)

/* Bit definitions and macros for MCF_SEC_SISRL */
#define MCF_SEC_SISRL_TEA                    (0x40)
#define MCF_SEC_SISRL_DEU_DN                 (0x100)
#define MCF_SEC_SISRL_DEU_ERR                (0x200)
#define MCF_SEC_SISRL_AESU_DN                (0x1000)
#define MCF_SEC_SISRL_AESU_ERR               (0x2000)
#define MCF_SEC_SISRL_MDEU_DN                (0x10000)
#define MCF_SEC_SISRL_MDEU_ERR               (0x20000)
#define MCF_SEC_SISRL_AFEU_DN                (0x100000)
#define MCF_SEC_SISRL_AFEU_ERR               (0x200000)
#define MCF_SEC_SISRL_RNG_DN                 (0x1000000)
#define MCF_SEC_SISRL_RNG_ERR                (0x2000000)

/* Bit definitions and macros for MCF_SEC_SICRH */
#define MCF_SEC_SICRH_AERR                   (0x8000000)
#define MCF_SEC_SICRH_CHA_0_DN               (0x10000000)
#define MCF_SEC_SICRH_CHA_0_ERR              (0x20000000)
#define MCF_SEC_SICRH_CHA_1_DN               (0x40000000)
#define MCF_SEC_SICRH_CHA_1_ERR              (0x80000000)

/* Bit definitions and macros for MCF_SEC_SICRL */
#define MCF_SEC_SICRL_TEA                    (0x40)
#define MCF_SEC_SICRL_DEU_DN                 (0x100)
#define MCF_SEC_SICRL_DEU_ERR                (0x200)
#define MCF_SEC_SICRL_AESU_DN                (0x1000)
#define MCF_SEC_SICRL_AESU_ERR               (0x2000)
#define MCF_SEC_SICRL_MDEU_DN                (0x10000)
#define MCF_SEC_SICRL_MDEU_ERR               (0x20000)
#define MCF_SEC_SICRL_AFEU_DN                (0x100000)
#define MCF_SEC_SICRL_AFEU_ERR               (0x200000)
#define MCF_SEC_SICRL_RNG_DN                 (0x1000000)
#define MCF_SEC_SICRL_RNG_ERR                (0x2000000)

/* Bit definitions and macros for MCF_SEC_SIDR */
#define MCF_SEC_SIDR_VERSION(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SEC_EUASRH */
#define MCF_SEC_EUASRH_AFEU(x)               (((x)&0xF)<<0)
#define MCF_SEC_EUASRH_MDEU(x)               (((x)&0xF)<<0x8)
#define MCF_SEC_EUASRH_RNG(x)                (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_SEC_EUASRL */
#define MCF_SEC_EUASRL_AESU(x)               (((x)&0xF)<<0x10)
#define MCF_SEC_EUASRL_DEU(x)                (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_SEC_SMCR */
#define MCF_SEC_SMCR_CURR_CHAN(x)            (((x)&0xF)<<0x4)
#define MCF_SEC_SMCR_CURR_CHAN_1             (0x10)
#define MCF_SEC_SMCR_CURR_CHAN_2             (0x20)
#define MCF_SEC_SMCR_SWR                     (0x1000000)

/* Bit definitions and macros for MCF_SEC_MEAR */
#define MCF_SEC_MEAR_ADDRESS(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SEC_CCCRn */
#define MCF_SEC_CCCRn_RST                    (0x1)
#define MCF_SEC_CCCRn_CDIE                   (0x2)
#define MCF_SEC_CCCRn_NT                     (0x4)
#define MCF_SEC_CCCRn_NE                     (0x8)
#define MCF_SEC_CCCRn_WE                     (0x10)
#define MCF_SEC_CCCRn_BURST_SIZE(x)          (((x)&0x7)<<0x8)
#define MCF_SEC_CCCRn_BURST_SIZE_2           (0)
#define MCF_SEC_CCCRn_BURST_SIZE_8           (0x100)
#define MCF_SEC_CCCRn_BURST_SIZE_16          (0x200)
#define MCF_SEC_CCCRn_BURST_SIZE_24          (0x300)
#define MCF_SEC_CCCRn_BURST_SIZE_32          (0x400)
#define MCF_SEC_CCCRn_BURST_SIZE_40          (0x500)
#define MCF_SEC_CCCRn_BURST_SIZE_48          (0x600)
#define MCF_SEC_CCCRn_BURST_SIZE_56          (0x700)

/* Bit definitions and macros for MCF_SEC_CCPSRHn */
#define MCF_SEC_CCPSRHn_STATE(x)             (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_SEC_CCPSRLn */
#define MCF_SEC_CCPSRLn_PAIR_PTR(x)          (((x)&0xFF)<<0)
#define MCF_SEC_CCPSRLn_EUERR                (0x100)
#define MCF_SEC_CCPSRLn_SERR                 (0x200)
#define MCF_SEC_CCPSRLn_DERR                 (0x400)
#define MCF_SEC_CCPSRLn_PERR                 (0x1000)
#define MCF_SEC_CCPSRLn_TEA                  (0x2000)
#define MCF_SEC_CCPSRLn_SD                   (0x10000)
#define MCF_SEC_CCPSRLn_PD                   (0x20000)
#define MCF_SEC_CCPSRLn_SRD                  (0x40000)
#define MCF_SEC_CCPSRLn_PRD                  (0x80000)
#define MCF_SEC_CCPSRLn_SG                   (0x100000)
#define MCF_SEC_CCPSRLn_PG                   (0x200000)
#define MCF_SEC_CCPSRLn_SR                   (0x400000)
#define MCF_SEC_CCPSRLn_PR                   (0x800000)
#define MCF_SEC_CCPSRLn_MO                   (0x1000000)
#define MCF_SEC_CCPSRLn_MI                   (0x2000000)
#define MCF_SEC_CCPSRLn_STAT                 (0x4000000)

/* Bit definitions and macros for MCF_SEC_CDPRn */
#define MCF_SEC_CDPRn_CDP(x)                 (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SEC_FRn */
#define MCF_SEC_FRn_FETCH_ADDR(x)            (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_SEC_AFRCR */
#define MCF_SEC_AFRCR_SR                     (0x1000000)
#define MCF_SEC_AFRCR_MI                     (0x2000000)
#define MCF_SEC_AFRCR_RI                     (0x4000000)

/* Bit definitions and macros for MCF_SEC_AFSR */
#define MCF_SEC_AFSR_RD                      (0x1000000)
#define MCF_SEC_AFSR_ID                      (0x2000000)
#define MCF_SEC_AFSR_IE                      (0x4000000)
#define MCF_SEC_AFSR_OFR                     (0x8000000)
#define MCF_SEC_AFSR_IFW                     (0x10000000)
#define MCF_SEC_AFSR_HALT                    (0x20000000)

/* Bit definitions and macros for MCF_SEC_AFISR */
#define MCF_SEC_AFISR_DSE                    (0x10000)
#define MCF_SEC_AFISR_KSE                    (0x20000)
#define MCF_SEC_AFISR_CE                     (0x40000)
#define MCF_SEC_AFISR_ERE                    (0x80000)
#define MCF_SEC_AFISR_IE                     (0x100000)
#define MCF_SEC_AFISR_OFU                    (0x2000000)
#define MCF_SEC_AFISR_IFO                    (0x4000000)
#define MCF_SEC_AFISR_IFE                    (0x10000000)
#define MCF_SEC_AFISR_OFE                    (0x20000000)
#define MCF_SEC_AFISR_AE                     (0x40000000)
#define MCF_SEC_AFISR_ME                     (0x80000000)

/* Bit definitions and macros for MCF_SEC_AFIMR */
#define MCF_SEC_AFIMR_DSE                    (0x10000)
#define MCF_SEC_AFIMR_KSE                    (0x20000)
#define MCF_SEC_AFIMR_CE                     (0x40000)
#define MCF_SEC_AFIMR_ERE                    (0x80000)
#define MCF_SEC_AFIMR_IE                     (0x100000)
#define MCF_SEC_AFIMR_OFU                    (0x2000000)
#define MCF_SEC_AFIMR_IFO                    (0x4000000)
#define MCF_SEC_AFIMR_IFE                    (0x10000000)
#define MCF_SEC_AFIMR_OFE                    (0x20000000)
#define MCF_SEC_AFIMR_AE                     (0x40000000)
#define MCF_SEC_AFIMR_ME                     (0x80000000)

/* Bit definitions and macros for MCF_SEC_DRCR */
#define MCF_SEC_DRCR_SR                      (0x1000000)
#define MCF_SEC_DRCR_MI                      (0x2000000)
#define MCF_SEC_DRCR_RI                      (0x4000000)

/* Bit definitions and macros for MCF_SEC_DSR */
#define MCF_SEC_DSR_RD                       (0x1000000)
#define MCF_SEC_DSR_ID                       (0x2000000)
#define MCF_SEC_DSR_IE                       (0x4000000)
#define MCF_SEC_DSR_OFR                      (0x8000000)
#define MCF_SEC_DSR_IFW                      (0x10000000)
#define MCF_SEC_DSR_HALT                     (0x20000000)

/* Bit definitions and macros for MCF_SEC_DISR */
#define MCF_SEC_DISR_DSE                     (0x10000)
#define MCF_SEC_DISR_KSE                     (0x20000)
#define MCF_SEC_DISR_CE                      (0x40000)
#define MCF_SEC_DISR_ERE                     (0x80000)
#define MCF_SEC_DISR_IE                      (0x100000)
#define MCF_SEC_DISR_KPE                     (0x200000)
#define MCF_SEC_DISR_OFU                     (0x2000000)
#define MCF_SEC_DISR_IFO                     (0x4000000)
#define MCF_SEC_DISR_IFE                     (0x10000000)
#define MCF_SEC_DISR_OFE                     (0x20000000)
#define MCF_SEC_DISR_AE                      (0x40000000)
#define MCF_SEC_DISR_ME                      (0x80000000)

/* Bit definitions and macros for MCF_SEC_DIMR */
#define MCF_SEC_DIMR_DSE                     (0x10000)
#define MCF_SEC_DIMR_KSE                     (0x20000)
#define MCF_SEC_DIMR_CE                      (0x40000)
#define MCF_SEC_DIMR_ERE                     (0x80000)
#define MCF_SEC_DIMR_IE                      (0x100000)
#define MCF_SEC_DIMR_KPE                     (0x200000)
#define MCF_SEC_DIMR_OFU                     (0x2000000)
#define MCF_SEC_DIMR_IFO                     (0x4000000)
#define MCF_SEC_DIMR_IFE                     (0x10000000)
#define MCF_SEC_DIMR_OFE                     (0x20000000)
#define MCF_SEC_DIMR_AE                      (0x40000000)
#define MCF_SEC_DIMR_ME                      (0x80000000)

/* Bit definitions and macros for MCF_SEC_MDRCR */
#define MCF_SEC_MDRCR_SR                     (0x1000000)
#define MCF_SEC_MDRCR_MI                     (0x2000000)
#define MCF_SEC_MDRCR_RI                     (0x4000000)

/* Bit definitions and macros for MCF_SEC_MDSR */
#define MCF_SEC_MDSR_RD                      (0x1000000)
#define MCF_SEC_MDSR_ID                      (0x2000000)
#define MCF_SEC_MDSR_IE                      (0x4000000)
#define MCF_SEC_MDSR_IFW                     (0x10000000)
#define MCF_SEC_MDSR_HALT                    (0x20000000)

/* Bit definitions and macros for MCF_SEC_MDISR */
#define MCF_SEC_MDISR_DSE                    (0x10000)
#define MCF_SEC_MDISR_KSE                    (0x20000)
#define MCF_SEC_MDISR_CE                     (0x40000)
#define MCF_SEC_MDISR_ERE                    (0x80000)
#define MCF_SEC_MDISR_IE                     (0x100000)
#define MCF_SEC_MDISR_IFO                    (0x4000000)
#define MCF_SEC_MDISR_AE                     (0x40000000)
#define MCF_SEC_MDISR_ME                     (0x80000000)

/* Bit definitions and macros for MCF_SEC_MDIMR */
#define MCF_SEC_MDIMR_DSE                    (0x10000)
#define MCF_SEC_MDIMR_KSE                    (0x20000)
#define MCF_SEC_MDIMR_CE                     (0x40000)
#define MCF_SEC_MDIMR_ERE                    (0x80000)
#define MCF_SEC_MDIMR_IE                     (0x100000)
#define MCF_SEC_MDIMR_IFO                    (0x4000000)
#define MCF_SEC_MDIMR_AE                     (0x40000000)
#define MCF_SEC_MDIMR_ME                     (0x80000000)

/* Bit definitions and macros for MCF_SEC_RNGRCR */
#define MCF_SEC_RNGRCR_SR                    (0x1000000)
#define MCF_SEC_RNGRCR_MI                    (0x2000000)
#define MCF_SEC_RNGRCR_RI                    (0x4000000)

/* Bit definitions and macros for MCF_SEC_RNGSR */
#define MCF_SEC_RNGSR_RD                     (0x1000000)
#define MCF_SEC_RNGSR_IE                     (0x4000000)
#define MCF_SEC_RNGSR_OFR                    (0x8000000)
#define MCF_SEC_RNGSR_HALT                   (0x20000000)

/* Bit definitions and macros for MCF_SEC_RNGISR */
#define MCF_SEC_RNGISR_IE                    (0x100000)
#define MCF_SEC_RNGISR_OFU                   (0x2000000)
#define MCF_SEC_RNGISR_AE                    (0x40000000)
#define MCF_SEC_RNGISR_ME                    (0x80000000)

/* Bit definitions and macros for MCF_SEC_RNGIMR */
#define MCF_SEC_RNGIMR_IE                    (0x100000)
#define MCF_SEC_RNGIMR_OFU                   (0x2000000)
#define MCF_SEC_RNGIMR_AE                    (0x40000000)
#define MCF_SEC_RNGIMR_ME                    (0x80000000)

/* Bit definitions and macros for MCF_SEC_AESRCR */
#define MCF_SEC_AESRCR_SR                    (0x1000000)
#define MCF_SEC_AESRCR_MI                    (0x2000000)
#define MCF_SEC_AESRCR_RI                    (0x4000000)

/* Bit definitions and macros for MCF_SEC_AESSR */
#define MCF_SEC_AESSR_RD                     (0x1000000)
#define MCF_SEC_AESSR_ID                     (0x2000000)
#define MCF_SEC_AESSR_IE                     (0x4000000)
#define MCF_SEC_AESSR_OFR                    (0x8000000)
#define MCF_SEC_AESSR_IFW                    (0x10000000)
#define MCF_SEC_AESSR_HALT                   (0x20000000)

/* Bit definitions and macros for MCF_SEC_AESISR */
#define MCF_SEC_AESISR_DSE                   (0x10000)
#define MCF_SEC_AESISR_KSE                   (0x20000)
#define MCF_SEC_AESISR_CE                    (0x40000)
#define MCF_SEC_AESISR_ERE                   (0x80000)
#define MCF_SEC_AESISR_IE                    (0x100000)
#define MCF_SEC_AESISR_OFU                   (0x2000000)
#define MCF_SEC_AESISR_IFO                   (0x4000000)
#define MCF_SEC_AESISR_IFE                   (0x10000000)
#define MCF_SEC_AESISR_OFE                   (0x20000000)
#define MCF_SEC_AESISR_AE                    (0x40000000)
#define MCF_SEC_AESISR_ME                    (0x80000000)

/* Bit definitions and macros for MCF_SEC_AESIMR */
#define MCF_SEC_AESIMR_DSE                   (0x10000)
#define MCF_SEC_AESIMR_KSE                   (0x20000)
#define MCF_SEC_AESIMR_CE                    (0x40000)
#define MCF_SEC_AESIMR_ERE                   (0x80000)
#define MCF_SEC_AESIMR_IE                    (0x100000)
#define MCF_SEC_AESIMR_OFU                   (0x2000000)
#define MCF_SEC_AESIMR_IFO                   (0x4000000)
#define MCF_SEC_AESIMR_IFE                   (0x10000000)
#define MCF_SEC_AESIMR_OFE                   (0x20000000)
#define MCF_SEC_AESIMR_AE                    (0x40000000)
#define MCF_SEC_AESIMR_ME                    (0x80000000)


#endif /* __MCF5475_SEC_H__ */
