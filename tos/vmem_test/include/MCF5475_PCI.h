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

#ifndef __MCF5475_PCI_H__
#define __MCF5475_PCI_H__


/*********************************************************************
*
* PCI Bus Controller (PCI)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_PCI_PCIIDR                       (*(volatile uint32_t*)(&_MBAR[0xB00]))
#define MCF_PCI_PCISCR                       (*(volatile uint32_t*)(&_MBAR[0xB04]))
#define MCF_PCI_PCICCRIR                     (*(volatile uint32_t*)(&_MBAR[0xB08]))
#define MCF_PCI_PCICR1                       (*(volatile uint32_t*)(&_MBAR[0xB0C]))
#define MCF_PCI_PCIBAR0                      (*(volatile uint32_t*)(&_MBAR[0xB10]))
#define MCF_PCI_PCIBAR1                      (*(volatile uint32_t*)(&_MBAR[0xB14]))
#define MCF_PCI_PCICCPR                      (*(volatile uint32_t*)(&_MBAR[0xB28]))
#define MCF_PCI_PCISID                       (*(volatile uint32_t*)(&_MBAR[0xB2C]))
#define MCF_PCI_PCICR2                       (*(volatile uint32_t*)(&_MBAR[0xB3C]))
#define MCF_PCI_PCIGSCR                      (*(volatile uint32_t*)(&_MBAR[0xB60]))
#define MCF_PCI_PCITBATR0                    (*(volatile uint32_t*)(&_MBAR[0xB64]))
#define MCF_PCI_PCITBATR1                    (*(volatile uint32_t*)(&_MBAR[0xB68]))
#define MCF_PCI_PCITCR                       (*(volatile uint32_t*)(&_MBAR[0xB6C]))
#define MCF_PCI_PCIIW0BTAR                   (*(volatile uint32_t*)(&_MBAR[0xB70]))
#define MCF_PCI_PCIIW1BTAR                   (*(volatile uint32_t*)(&_MBAR[0xB74]))
#define MCF_PCI_PCIIW2BTAR                   (*(volatile uint32_t*)(&_MBAR[0xB78]))
#define MCF_PCI_PCIIWCR                      (*(volatile uint32_t*)(&_MBAR[0xB80]))
#define MCF_PCI_PCIICR                       (*(volatile uint32_t*)(&_MBAR[0xB84]))
#define MCF_PCI_PCIISR                       (*(volatile uint32_t*)(&_MBAR[0xB88]))
#define MCF_PCI_PCICAR                       (*(volatile uint32_t*)(&_MBAR[0xBF8]))
#define MCF_PCI_PCITPSR                      (*(volatile uint32_t*)(&_MBAR[0x8400]))
#define MCF_PCI_PCITSAR                      (*(volatile uint32_t*)(&_MBAR[0x8404]))
#define MCF_PCI_PCITTCR                      (*(volatile uint32_t*)(&_MBAR[0x8408]))
#define MCF_PCI_PCITER                       (*(volatile uint32_t*)(&_MBAR[0x840C]))
#define MCF_PCI_PCITNAR                      (*(volatile uint32_t*)(&_MBAR[0x8410]))
#define MCF_PCI_PCITLWR                      (*(volatile uint32_t*)(&_MBAR[0x8414]))
#define MCF_PCI_PCITDCR                      (*(volatile uint32_t*)(&_MBAR[0x8418]))
#define MCF_PCI_PCITSR                       (*(volatile uint32_t*)(&_MBAR[0x841C]))
#define MCF_PCI_PCITFDR                      (*(volatile uint32_t*)(&_MBAR[0x8440]))
#define MCF_PCI_PCITFSR                      (*(volatile uint32_t*)(&_MBAR[0x8444]))
#define MCF_PCI_PCITFCR                      (*(volatile uint32_t*)(&_MBAR[0x8448]))
#define MCF_PCI_PCITFAR                      (*(volatile uint32_t*)(&_MBAR[0x844C]))
#define MCF_PCI_PCITFRPR                     (*(volatile uint32_t*)(&_MBAR[0x8450]))
#define MCF_PCI_PCITFWPR                     (*(volatile uint32_t*)(&_MBAR[0x8454]))
#define MCF_PCI_PCIRPSR                      (*(volatile uint32_t*)(&_MBAR[0x8480]))
#define MCF_PCI_PCIRSAR                      (*(volatile uint32_t*)(&_MBAR[0x8484]))
#define MCF_PCI_PCIRTCR                      (*(volatile uint32_t*)(&_MBAR[0x8488]))
#define MCF_PCI_PCIRER                       (*(volatile uint32_t*)(&_MBAR[0x848C]))
#define MCF_PCI_PCIRNAR                      (*(volatile uint32_t*)(&_MBAR[0x8490]))
#define MCF_PCI_PCIRDCR                      (*(volatile uint32_t*)(&_MBAR[0x8498]))
#define MCF_PCI_PCIRSR                       (*(volatile uint32_t*)(&_MBAR[0x849C]))
#define MCF_PCI_PCIRFDR                      (*(volatile uint32_t*)(&_MBAR[0x84C0]))
#define MCF_PCI_PCIRFSR                      (*(volatile uint32_t*)(&_MBAR[0x84C4]))
#define MCF_PCI_PCIRFCR                      (*(volatile uint32_t*)(&_MBAR[0x84C8]))
#define MCF_PCI_PCIRFAR                      (*(volatile uint32_t*)(&_MBAR[0x84CC]))
#define MCF_PCI_PCIRFRPR                     (*(volatile uint32_t*)(&_MBAR[0x84D0]))
#define MCF_PCI_PCIRFWPR                     (*(volatile uint32_t*)(&_MBAR[0x84D4]))


/* Bit definitions and macros for MCF_PCI_PCIIDR */
#define MCF_PCI_PCIIDR_VENDORID(x)           (((x)&0xFFFF)<<0)
#define MCF_PCI_PCIIDR_DEVICEID(x)           (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_PCI_PCISCR */
#define MCF_PCI_PCISCR_IO                    (0x1)
#define MCF_PCI_PCISCR_M                     (0x2)
#define MCF_PCI_PCISCR_B                     (0x4)
#define MCF_PCI_PCISCR_SP                    (0x8)
#define MCF_PCI_PCISCR_MW                    (0x10)
#define MCF_PCI_PCISCR_V                     (0x20)
#define MCF_PCI_PCISCR_PER                   (0x40)
#define MCF_PCI_PCISCR_ST                    (0x80)
#define MCF_PCI_PCISCR_S                     (0x100)
#define MCF_PCI_PCISCR_F                     (0x200)
#define MCF_PCI_PCISCR_C                     (0x100000)
#define MCF_PCI_PCISCR_66M                   (0x200000)
#define MCF_PCI_PCISCR_R                     (0x400000)
#define MCF_PCI_PCISCR_FC                    (0x800000)
#define MCF_PCI_PCISCR_DP                    (0x1000000)
#define MCF_PCI_PCISCR_DT(x)                 (((x)&0x3)<<0x19)
#define MCF_PCI_PCISCR_TS                    (0x8000000)
#define MCF_PCI_PCISCR_TR                    (0x10000000)
#define MCF_PCI_PCISCR_MA                    (0x20000000)
#define MCF_PCI_PCISCR_SE                    (0x40000000)
#define MCF_PCI_PCISCR_PE                    (0x80000000)

/* Bit definitions and macros for MCF_PCI_PCICCRIR */
#define MCF_PCI_PCICCRIR_REVISIONID(x)       (((x)&0xFF)<<0)
#define MCF_PCI_PCICCRIR_CLASSCODE(x)        (((x)&0xFFFFFF)<<0x8)

/* Bit definitions and macros for MCF_PCI_PCICR1 */
#define MCF_PCI_PCICR1_CACHELINESIZE(x)      (((x)&0xFF)<<0)
#define MCF_PCI_PCICR1_LATTIMER(x)           (((x)&0xFF)<<0x8)
#define MCF_PCI_PCICR1_HEADERTYPE(x)         (((x)&0xFF)<<0x10)
#define MCF_PCI_PCICR1_BIST(x)               (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_PCI_PCIBAR0 */
#define MCF_PCI_PCIBAR0_IOM                  (0x1)
#define MCF_PCI_PCIBAR0_RANGE(x)             (((x)&0x3)<<0x1)
#define MCF_PCI_PCIBAR0_PREF                 (0x8)
#define MCF_PCI_PCIBAR0_BAR0(x)              (((x)&0x3FFF)<<0x12)

/* Bit definitions and macros for MCF_PCI_PCIBAR1 */
#define MCF_PCI_PCIBAR1_IOM                  (0x1)
#define MCF_PCI_PCIBAR1_RANGE(x)             (((x)&0x3)<<0x1)
#define MCF_PCI_PCIBAR1_PREF                 (0x8)
#define MCF_PCI_PCIBAR1_BAR1(x)              (((x)&0x3)<<0x1E)

/* Bit definitions and macros for MCF_PCI_PCICCPR */
#define MCF_PCI_PCICCPR_PCICCP(x)            (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCISID */
#define MCF_PCI_PCISID_VENDORID(x)           (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCICR2 */
#define MCF_PCI_PCICR2_INTLINE(x)            (((x)&0xFF)<<0)
#define MCF_PCI_PCICR2_INTPIN(x)             (((x)&0xFF)<<0x8)
#define MCF_PCI_PCICR2_MINGNT(x)             (((x)&0xFF)<<0x10)
#define MCF_PCI_PCICR2_MAXLAT(x)             (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_PCI_PCIGSCR */
#define MCF_PCI_PCIGSCR_PR                   (0x1)
#define MCF_PCI_PCIGSCR_SEE                  (0x1000)
#define MCF_PCI_PCIGSCR_PEE                  (0x2000)
#define MCF_PCI_PCIGSCR_CLKINRESERVED(x)     (((x)&0x7)<<0x10)
#define MCF_PCI_PCIGSCR_XLB2CLKIN(x)         (((x)&0x7)<<0x18)
#define MCF_PCI_PCIGSCR_SE                   (0x10000000)
#define MCF_PCI_PCIGSCR_PE                   (0x20000000)

/* Bit definitions and macros for MCF_PCI_PCITBATR0 */
#define MCF_PCI_PCITBATR0_EN                 (0x1)
#define MCF_PCI_PCITBATR0_BAT0(x)            (((x)&0x3FFF)<<0x12)

/* Bit definitions and macros for MCF_PCI_PCITBATR1 */
#define MCF_PCI_PCITBATR1_EN                 (0x1)
#define MCF_PCI_PCITBATR1_BAT1(x)            (((x)&0x3)<<0x1E)

/* Bit definitions and macros for MCF_PCI_PCITCR */
#define MCF_PCI_PCITCR_P                     (0x10000)
#define MCF_PCI_PCITCR_LD                    (0x1000000)

/* Bit definitions and macros for MCF_PCI_PCIIW0BTAR */
#define MCF_PCI_PCIIW0BTAR_WTA0(x)           (((x)&0xFF)<<0x8)
#define MCF_PCI_PCIIW0BTAR_WAM0(x)           (((x)&0xFF)<<0x10)
#define MCF_PCI_PCIIW0BTAR_WBA0(x)           (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_PCI_PCIIW1BTAR */
#define MCF_PCI_PCIIW1BTAR_WTA1(x)           (((x)&0xFF)<<0x8)
#define MCF_PCI_PCIIW1BTAR_WAM1(x)           (((x)&0xFF)<<0x10)
#define MCF_PCI_PCIIW1BTAR_WBA1(x)           (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_PCI_PCIIW2BTAR */
#define MCF_PCI_PCIIW2BTAR_WTA2(x)           (((x)&0xFF)<<0x8)
#define MCF_PCI_PCIIW2BTAR_WAM2(x)           (((x)&0xFF)<<0x10)
#define MCF_PCI_PCIIW2BTAR_WBA2(x)           (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_PCI_PCIIWCR */
#define MCF_PCI_PCIIWCR_WINCTRL2_E           (0x100)
#define MCF_PCI_PCIIWCR_WINCTRL2_PRC(x)      (((x)&0x3)<<0x9)
#define MCF_PCI_PCIIWCR_WINCTRL2_IOM         (0x800)
#define MCF_PCI_PCIIWCR_WINCTRL1_E           (0x10000)
#define MCF_PCI_PCIIWCR_WINCTRL1_PRC(x)      (((x)&0x3)<<0x11)
#define MCF_PCI_PCIIWCR_WINCTRL1_IOM         (0x80000)
#define MCF_PCI_PCIIWCR_WINCTRL0_E           (0x1000000)
#define MCF_PCI_PCIIWCR_WINCTRL0_PRC(x)      (((x)&0x3)<<0x19)
#define MCF_PCI_PCIIWCR_WINCTRL0_IOM         (0x8000000)
#define MCF_PCI_PCIIWCR_WINCTRL2_MEMREAD     (0x100)
#define MCF_PCI_PCIIWCR_WINCTRL2_MEMRDLINE   (0x300)
#define MCF_PCI_PCIIWCR_WINCTRL2_MEMRDMUL    (0x500)
#define MCF_PCI_PCIIWCR_WINCTRL2_IO          (0x900)
#define MCF_PCI_PCIIWCR_WINCTRL1_MEMREAD     (0x10000)
#define MCF_PCI_PCIIWCR_WINCTRL1_MEMRDLINE   (0x30000)
#define MCF_PCI_PCIIWCR_WINCTRL1_MEMRDMUL    (0x50000)
#define MCF_PCI_PCIIWCR_WINCTRL1_IO          (0x90000)
#define MCF_PCI_PCIIWCR_WINCTRL0_MEMREAD     (0x1000000)
#define MCF_PCI_PCIIWCR_WINCTRL0_MEMRDLINE   (0x3000000)
#define MCF_PCI_PCIIWCR_WINCTRL0_MEMRDMUL    (0x5000000)
#define MCF_PCI_PCIIWCR_WINCTRL0_IO          (0x9000000)

/* Bit definitions and macros for MCF_PCI_PCIICR */
#define MCF_PCI_PCIICR_MAXRETRY(x)           (((x)&0xFF)<<0)
#define MCF_PCI_PCIICR_TAE                   (0x1000000)
#define MCF_PCI_PCIICR_IAE                   (0x2000000)
#define MCF_PCI_PCIICR_REE                   (0x4000000)

/* Bit definitions and macros for MCF_PCI_PCIISR */
#define MCF_PCI_PCIISR_TA                    (0x1000000)
#define MCF_PCI_PCIISR_IA                    (0x2000000)
#define MCF_PCI_PCIISR_RE                    (0x4000000)

/* Bit definitions and macros for MCF_PCI_PCICAR */
#define MCF_PCI_PCICAR_DWORD(x)              (((x)&0x3F)<<0x2)
#define MCF_PCI_PCICAR_FUNCNUM(x)            (((x)&0x7)<<0x8)
#define MCF_PCI_PCICAR_DEVNUM(x)             (((x)&0x1F)<<0xB)
#define MCF_PCI_PCICAR_BUSNUM(x)             (((x)&0xFF)<<0x10)
#define MCF_PCI_PCICAR_E                     (0x80000000)

/* Bit definitions and macros for MCF_PCI_PCITPSR */
#define MCF_PCI_PCITPSR_PKTSIZE(x)           (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_PCI_PCITSAR */
#define MCF_PCI_PCITSAR_STARTADD(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCITTCR */
#define MCF_PCI_PCITTCR_DI                   (0x1)
#define MCF_PCI_PCITTCR_W                    (0x10)
#define MCF_PCI_PCITTCR_MAXBEATS(x)          (((x)&0x7)<<0x8)
#define MCF_PCI_PCITTCR_MAXRETRY(x)          (((x)&0xFF)<<0x10)
#define MCF_PCI_PCITTCR_PCICMD(x)            (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_PCI_PCITER */
#define MCF_PCI_PCITER_NE                    (0x10000)
#define MCF_PCI_PCITER_IAE                   (0x20000)
#define MCF_PCI_PCITER_TAE                   (0x40000)
#define MCF_PCI_PCITER_RE                    (0x80000)
#define MCF_PCI_PCITER_SE                    (0x100000)
#define MCF_PCI_PCITER_FEE                   (0x200000)
#define MCF_PCI_PCITER_ME                    (0x1000000)
#define MCF_PCI_PCITER_BE                    (0x8000000)
#define MCF_PCI_PCITER_CM                    (0x10000000)
#define MCF_PCI_PCITER_RF                    (0x40000000)
#define MCF_PCI_PCITER_RC                    (0x80000000)

/* Bit definitions and macros for MCF_PCI_PCITNAR */
#define MCF_PCI_PCITNAR_NEXTADDRESS(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCITLWR */
#define MCF_PCI_PCITLWR_LASTWORD(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCITDCR */
#define MCF_PCI_PCITDCR_PKTSDONE(x)          (((x)&0xFFFF)<<0)
#define MCF_PCI_PCITDCR_BYTESDONE(x)         (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_PCI_PCITSR */
#define MCF_PCI_PCITSR_IA                    (0x10000)
#define MCF_PCI_PCITSR_TA                    (0x20000)
#define MCF_PCI_PCITSR_RE                    (0x40000)
#define MCF_PCI_PCITSR_SE                    (0x80000)
#define MCF_PCI_PCITSR_FE                    (0x100000)
#define MCF_PCI_PCITSR_BE1                   (0x200000)
#define MCF_PCI_PCITSR_BE2                   (0x400000)
#define MCF_PCI_PCITSR_BE3                   (0x800000)
#define MCF_PCI_PCITSR_NT                    (0x1000000)

/* Bit definitions and macros for MCF_PCI_PCITFDR */
#define MCF_PCI_PCITFDR_FIFODATAWORD(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCITFSR */
#define MCF_PCI_PCITFSR_EMPTY                (0x10000)
#define MCF_PCI_PCITFSR_ALARM                (0x20000)
#define MCF_PCI_PCITFSR_FULL                 (0x40000)
#define MCF_PCI_PCITFSR_FR                   (0x80000)
#define MCF_PCI_PCITFSR_OF                   (0x100000)
#define MCF_PCI_PCITFSR_UF                   (0x200000)
#define MCF_PCI_PCITFSR_RXW                  (0x400000)
#define MCF_PCI_PCITFSR_FAE                  (0x800000)
#define MCF_PCI_PCITFSR_TXW                  (0x40000000)
#define MCF_PCI_PCITFSR_IP                   (0x80000000)

/* Bit definitions and macros for MCF_PCI_PCITFCR */
#define MCF_PCI_PCITFCR_TXW_MASK             (0x40000)
#define MCF_PCI_PCITFCR_OF_MASK              (0x80000)
#define MCF_PCI_PCITFCR_UF_MASK              (0x100000)
#define MCF_PCI_PCITFCR_RXW_MASK             (0x200000)
#define MCF_PCI_PCITFCR_FAE_MASK             (0x400000)
#define MCF_PCI_PCITFCR_IP_MASK              (0x800000)
#define MCF_PCI_PCITFCR_GR(x)                (((x)&0x7)<<0x18)
#define MCF_PCI_PCITFCR_WFR                  (0x20000000)

/* Bit definitions and macros for MCF_PCI_PCITFAR */
#define MCF_PCI_PCITFAR_ALARM(x)             (((x)&0xFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCITFRPR */
#define MCF_PCI_PCITFRPR_READPTR(x)          (((x)&0x7F)<<0)

/* Bit definitions and macros for MCF_PCI_PCITFWPR */
#define MCF_PCI_PCITFWPR_WRITEPTR(x)         (((x)&0x7F)<<0)

/* Bit definitions and macros for MCF_PCI_PCIRPSR */
#define MCF_PCI_PCIRPSR_PKTSIZE(x)           (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_PCI_PCIRSAR */
#define MCF_PCI_PCIRSAR_STARTADD(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCIRTCR */
#define MCF_PCI_PCIRTCR_DI                   (0x1)
#define MCF_PCI_PCIRTCR_W                    (0x10)
#define MCF_PCI_PCIRTCR_MAXBEATS(x)          (((x)&0x7)<<0x8)
#define MCF_PCI_PCIRTCR_FB                   (0x1000)
#define MCF_PCI_PCIRTCR_MAXRETRY(x)          (((x)&0xFF)<<0x10)
#define MCF_PCI_PCIRTCR_PCICMD(x)            (((x)&0xF)<<0x18)

/* Bit definitions and macros for MCF_PCI_PCIRER */
#define MCF_PCI_PCIRER_NE                    (0x10000)
#define MCF_PCI_PCIRER_IAE                   (0x20000)
#define MCF_PCI_PCIRER_TAE                   (0x40000)
#define MCF_PCI_PCIRER_RE                    (0x80000)
#define MCF_PCI_PCIRER_SE                    (0x100000)
#define MCF_PCI_PCIRER_FEE                   (0x200000)
#define MCF_PCI_PCIRER_ME                    (0x1000000)
#define MCF_PCI_PCIRER_BE                    (0x8000000)
#define MCF_PCI_PCIRER_CM                    (0x10000000)
#define MCF_PCI_PCIRER_FE                    (0x20000000)
#define MCF_PCI_PCIRER_RF                    (0x40000000)
#define MCF_PCI_PCIRER_RC                    (0x80000000)

/* Bit definitions and macros for MCF_PCI_PCIRNAR */
#define MCF_PCI_PCIRNAR_NEXTADDRESS(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCIRDCR */
#define MCF_PCI_PCIRDCR_PKTSDONE(x)          (((x)&0xFFFF)<<0)
#define MCF_PCI_PCIRDCR_BYTESDONE(x)         (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_PCI_PCIRSR */
#define MCF_PCI_PCIRSR_IA                    (0x10000)
#define MCF_PCI_PCIRSR_TA                    (0x20000)
#define MCF_PCI_PCIRSR_RE                    (0x40000)
#define MCF_PCI_PCIRSR_SE                    (0x80000)
#define MCF_PCI_PCIRSR_FE                    (0x100000)
#define MCF_PCI_PCIRSR_BE1                   (0x200000)
#define MCF_PCI_PCIRSR_BE2                   (0x400000)
#define MCF_PCI_PCIRSR_BE3                   (0x800000)
#define MCF_PCI_PCIRSR_NT                    (0x1000000)

/* Bit definitions and macros for MCF_PCI_PCIRFDR */
#define MCF_PCI_PCIRFDR_FIFODATAWORD(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PCI_PCIRFSR */
#define MCF_PCI_PCIRFSR_EMPTY                (0x10000)
#define MCF_PCI_PCIRFSR_ALARM                (0x20000)
#define MCF_PCI_PCIRFSR_FULL                 (0x40000)
#define MCF_PCI_PCIRFSR_FR                   (0x80000)
#define MCF_PCI_PCIRFSR_OF                   (0x100000)
#define MCF_PCI_PCIRFSR_UF                   (0x200000)
#define MCF_PCI_PCIRFSR_RXW                  (0x400000)
#define MCF_PCI_PCIRFSR_FAE                  (0x800000)
#define MCF_PCI_PCIRFSR_TXW                  (0x40000000)
#define MCF_PCI_PCIRFSR_IP                   (0x80000000)

/* Bit definitions and macros for MCF_PCI_PCIRFCR */
#define MCF_PCI_PCIRFCR_TXW_MASK             (0x40000)
#define MCF_PCI_PCIRFCR_OF_MASK              (0x80000)
#define MCF_PCI_PCIRFCR_UF_MASK              (0x100000)
#define MCF_PCI_PCIRFCR_RXW_MASK             (0x200000)
#define MCF_PCI_PCIRFCR_FAE_MASK             (0x400000)
#define MCF_PCI_PCIRFCR_IP_MASK              (0x800000)
#define MCF_PCI_PCIRFCR_GR(x)                (((x)&0x7)<<0x18)
#define MCF_PCI_PCIRFCR_WFR                  (0x20000000)

/* Bit definitions and macros for MCF_PCI_PCIRFAR */
#define MCF_PCI_PCIRFAR_ALARM(x)             (((x)&0x7F)<<0)

/* Bit definitions and macros for MCF_PCI_PCIRFRPR */
#define MCF_PCI_PCIRFRPR_READPTR(x)          (((x)&0x7F)<<0)

/* Bit definitions and macros for MCF_PCI_PCIRFWPR */
#define MCF_PCI_PCIRFWPR_WRITEPTR(x)         (((x)&0x7F)<<0)


#endif /* __MCF5475_PCI_H__ */
