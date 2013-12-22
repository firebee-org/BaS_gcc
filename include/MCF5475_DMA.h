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

#ifndef __MCF5475_DMA_H__
#define __MCF5475_DMA_H__


/*********************************************************************
*
* Multichannel DMA (DMA)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_DMA_TASKBAR                      (*(volatile uint32_t*)(&_MBAR[0x8000]))
#define MCF_DMA_CP                           (*(volatile uint32_t*)(&_MBAR[0x8004]))
#define MCF_DMA_EP                           (*(volatile uint32_t*)(&_MBAR[0x8008]))
#define MCF_DMA_VP                           (*(volatile uint32_t*)(&_MBAR[0x800C]))
#define MCF_DMA_PTD                          (*(volatile uint32_t*)(&_MBAR[0x8010]))
#define MCF_DMA_DIPR                         (*(volatile uint32_t*)(&_MBAR[0x8014]))
#define MCF_DMA_DIMR                         (*(volatile uint32_t*)(&_MBAR[0x8018]))
#define MCF_DMA_TCR0                         (*(volatile uint16_t*)(&_MBAR[0x801C]))
#define MCF_DMA_TCR1                         (*(volatile uint16_t*)(&_MBAR[0x801E]))
#define MCF_DMA_TCR2                         (*(volatile uint16_t*)(&_MBAR[0x8020]))
#define MCF_DMA_TCR3                         (*(volatile uint16_t*)(&_MBAR[0x8022]))
#define MCF_DMA_TCR4                         (*(volatile uint16_t*)(&_MBAR[0x8024]))
#define MCF_DMA_TCR5                         (*(volatile uint16_t*)(&_MBAR[0x8026]))
#define MCF_DMA_TCR6                         (*(volatile uint16_t*)(&_MBAR[0x8028]))
#define MCF_DMA_TCR7                         (*(volatile uint16_t*)(&_MBAR[0x802A]))
#define MCF_DMA_TCR8                         (*(volatile uint16_t*)(&_MBAR[0x802C]))
#define MCF_DMA_TCR9                         (*(volatile uint16_t*)(&_MBAR[0x802E]))
#define MCF_DMA_TCR10                        (*(volatile uint16_t*)(&_MBAR[0x8030]))
#define MCF_DMA_TCR11                        (*(volatile uint16_t*)(&_MBAR[0x8032]))
#define MCF_DMA_TCR12                        (*(volatile uint16_t*)(&_MBAR[0x8034]))
#define MCF_DMA_TCR13                        (*(volatile uint16_t*)(&_MBAR[0x8036]))
#define MCF_DMA_TCR14                        (*(volatile uint16_t*)(&_MBAR[0x8038]))
#define MCF_DMA_TCR15                        (*(volatile uint16_t*)(&_MBAR[0x803A]))
#define MCF_DMA_PRIOR0                       (*(volatile uint8_t *)(&_MBAR[0x803C]))
#define MCF_DMA_PRIOR1                       (*(volatile uint8_t *)(&_MBAR[0x803D]))
#define MCF_DMA_PRIOR2                       (*(volatile uint8_t *)(&_MBAR[0x803E]))
#define MCF_DMA_PRIOR3                       (*(volatile uint8_t *)(&_MBAR[0x803F]))
#define MCF_DMA_PRIOR4                       (*(volatile uint8_t *)(&_MBAR[0x8040]))
#define MCF_DMA_PRIOR5                       (*(volatile uint8_t *)(&_MBAR[0x8041]))
#define MCF_DMA_PRIOR6                       (*(volatile uint8_t *)(&_MBAR[0x8042]))
#define MCF_DMA_PRIOR7                       (*(volatile uint8_t *)(&_MBAR[0x8043]))
#define MCF_DMA_PRIOR8                       (*(volatile uint8_t *)(&_MBAR[0x8044]))
#define MCF_DMA_PRIOR9                       (*(volatile uint8_t *)(&_MBAR[0x8045]))
#define MCF_DMA_PRIOR10                      (*(volatile uint8_t *)(&_MBAR[0x8046]))
#define MCF_DMA_PRIOR11                      (*(volatile uint8_t *)(&_MBAR[0x8047]))
#define MCF_DMA_PRIOR12                      (*(volatile uint8_t *)(&_MBAR[0x8048]))
#define MCF_DMA_PRIOR13                      (*(volatile uint8_t *)(&_MBAR[0x8049]))
#define MCF_DMA_PRIOR14                      (*(volatile uint8_t *)(&_MBAR[0x804A]))
#define MCF_DMA_PRIOR15                      (*(volatile uint8_t *)(&_MBAR[0x804B]))
#define MCF_DMA_PRIOR16                      (*(volatile uint8_t *)(&_MBAR[0x804C]))
#define MCF_DMA_PRIOR17                      (*(volatile uint8_t *)(&_MBAR[0x804D]))
#define MCF_DMA_PRIOR18                      (*(volatile uint8_t *)(&_MBAR[0x804E]))
#define MCF_DMA_PRIOR19                      (*(volatile uint8_t *)(&_MBAR[0x804F]))
#define MCF_DMA_PRIOR20                      (*(volatile uint8_t *)(&_MBAR[0x8050]))
#define MCF_DMA_PRIOR21                      (*(volatile uint8_t *)(&_MBAR[0x8051]))
#define MCF_DMA_PRIOR22                      (*(volatile uint8_t *)(&_MBAR[0x8052]))
#define MCF_DMA_PRIOR23                      (*(volatile uint8_t *)(&_MBAR[0x8053]))
#define MCF_DMA_PRIOR24                      (*(volatile uint8_t *)(&_MBAR[0x8054]))
#define MCF_DMA_PRIOR25                      (*(volatile uint8_t *)(&_MBAR[0x8055]))
#define MCF_DMA_PRIOR26                      (*(volatile uint8_t *)(&_MBAR[0x8056]))
#define MCF_DMA_PRIOR27                      (*(volatile uint8_t *)(&_MBAR[0x8057]))
#define MCF_DMA_PRIOR28                      (*(volatile uint8_t *)(&_MBAR[0x8058]))
#define MCF_DMA_PRIOR29                      (*(volatile uint8_t *)(&_MBAR[0x8059]))
#define MCF_DMA_PRIOR30                      (*(volatile uint8_t *)(&_MBAR[0x805A]))
#define MCF_DMA_PRIOR31                      (*(volatile uint8_t *)(&_MBAR[0x805B]))
#define MCF_DMA_IMCR                         (*(volatile uint32_t*)(&_MBAR[0x805C]))
#define MCF_DMA_TSKSZ0                       (*(volatile uint32_t*)(&_MBAR[0x8060]))
#define MCF_DMA_TSKSZ1                       (*(volatile uint32_t*)(&_MBAR[0x8064]))
#define MCF_DMA_DBGCOMP0                     (*(volatile uint32_t*)(&_MBAR[0x8070]))
#define MCF_DMA_DBGCOMP2                     (*(volatile uint32_t*)(&_MBAR[0x8074]))
#define MCF_DMA_DBGCTL                       (*(volatile uint32_t*)(&_MBAR[0x8078]))
#define MCF_DMA_TCR(x)                       (*(volatile uint16_t*)(&_MBAR[0x801C + ((x)*0x2)]))
#define MCF_DMA_PRIOR(x)                     (*(volatile uint8_t *)(&_MBAR[0x803C + ((x)*0x1)]))


/* Bit definitions and macros for MCF_DMA_TASKBAR */
#define MCF_DMA_TASKBAR_TASK_BASE_ADDRESS(x) (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DMA_CP */
#define MCF_DMA_CP_DESCRIPTOR_POINTER(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DMA_EP */
#define MCF_DMA_EP_DESCRIPTOR_POINTER(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DMA_VP */
#define MCF_DMA_VP_VARIABLE_POINTER(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DMA_PTD */
#define MCF_DMA_PTD_PCTL0                    (0x1)
#define MCF_DMA_PTD_PCTL1                    (0x2)
#define MCF_DMA_PTD_PCTL13                   (0x2000)
#define MCF_DMA_PTD_PCTL14                   (0x4000)
#define MCF_DMA_PTD_PCTL15                   (0x8000)

/* Bit definitions and macros for MCF_DMA_DIPR */
#define MCF_DMA_DIPR_TASK(x)                 (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DMA_DIMR */
#define MCF_DMA_DIMR_TASK(x)                 (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DMA_TCR */
#define MCF_DMA_TCR_ASTSKNUM(x)              (((x)&0xF)<<0)
#define MCF_DMA_TCR_HLDINITNUM               (0x20)
#define MCF_DMA_TCR_HIPRITSKEN               (0x40)
#define MCF_DMA_TCR_ASTRT                    (0x80)
#define MCF_DMA_TCR_INITNUM(x)               (((x)&0x1F)<<0x8)
#define MCF_DMA_TCR_ALWINIT                  (0x2000)
#define MCF_DMA_TCR_V                        (0x4000)
#define MCF_DMA_TCR_EN                       (0x8000)

/* Bit definitions and macros for MCF_DMA_PRIOR */
#define MCF_DMA_PRIOR_PRI(x)                 (((x)&0x7)<<0)
#define MCF_DMA_PRIOR_HLD                    (0x80)

/* Bit definitions and macros for MCF_DMA_IMCR */
#define MCF_DMA_IMCR_IMC16(x)	(((x)&0x3)<<0)
#define MCF_DMA_IMCR_IMC17(x)	(((x)&0x3)<<0x2)
#define MCF_DMA_IMCR_IMC18(x)	(((x)&0x3)<<0x4)
#define MCF_DMA_IMCR_IMC19(x)	(((x)&0x3)<<0x6)
#define MCF_DMA_IMCR_IMC20(x)	(((x)&0x3)<<0x8)
#define MCF_DMA_IMCR_IMC21(x)	(((x)&0x3)<<0xA)
#define MCF_DMA_IMCR_IMC22(x)	(((x)&0x3)<<0xC)
#define MCF_DMA_IMCR_IMC23(x)	(((x)&0x3)<<0xE)
#define MCF_DMA_IMCR_IMC24(x)	(((x)&0x3)<<0x10)
#define MCF_DMA_IMCR_IMC25(x)	(((x)&0x3)<<0x12)
#define MCF_DMA_IMCR_IMC26(x)	(((x)&0x3)<<0x14)
#define MCF_DMA_IMCR_IMC27(x)	(((x)&0x3)<<0x16)
#define MCF_DMA_IMCR_IMC28(x)	(((x)&0x3)<<0x18)
#define MCF_DMA_IMCR_IMC29(x)	(((x)&0x3)<<0x1A)
#define MCF_DMA_IMCR_IMC30(x)	(((x)&0x3)<<0x1C)
#define MCF_DMA_IMCR_IMC31(x)	(((x)&0x3)<<0x1E)


#define MCF_DMA_IMCR_IMC16_FEC0RX    (0x00000000)
#define MCF_DMA_IMCR_IMC17_FEC0TX    (0x00000000)
#define MCF_DMA_IMCR_IMC18_FEC0RX    (0x00000020)
#define MCF_DMA_IMCR_IMC19_FEC0TX    (0x00000080)
#define MCF_DMA_IMCR_IMC20_FEC1RX    (0x00000100)
#define MCF_DMA_IMCR_IMC21_DREQ1     (0x00000000)
#define MCF_DMA_IMCR_IMC21_FEC1TX    (0x00000400)
#define MCF_DMA_IMCR_IMC22_FEC0RX    (0x00001000)
#define MCF_DMA_IMCR_IMC23_FEC0TX    (0x00004000)
#define MCF_DMA_IMCR_IMC24_CTM0      (0x00010000)
#define MCF_DMA_IMCR_IMC24_FEC1RX    (0x00020000)
#define MCF_DMA_IMCR_IMC25_CTM1      (0x00040000)
#define MCF_DMA_IMCR_IMC25_FEC1TX    (0x00080000)
#define MCF_DMA_IMCR_IMC26_USBEP4    (0x00000000)
#define MCF_DMA_IMCR_IMC26_CTM2      (0x00200000)
#define MCF_DMA_IMCR_IMC27_USBEP5    (0x00000000)
#define MCF_DMA_IMCR_IMC27_CTM3      (0x00800000)
#define MCF_DMA_IMCR_IMC28_USBEP6    (0x00000000)
#define MCF_DMA_IMCR_IMC28_CTM4      (0x01000000)
#define MCF_DMA_IMCR_IMC28_DREQ1     (0x02000000)
#define MCF_DMA_IMCR_IMC28_PSC2RX    (0x03000000)
#define MCF_DMA_IMCR_IMC29_DREQ1     (0x04000000)
#define MCF_DMA_IMCR_IMC29_CTM5      (0x08000000)
#define MCF_DMA_IMCR_IMC29_PSC2TX    (0x0C000000)
#define MCF_DMA_IMCR_IMC30_FEC1RX    (0x00000000)
#define MCF_DMA_IMCR_IMC30_CTM6      (0x10000000)
#define MCF_DMA_IMCR_IMC30_PSC3RX    (0x30000000)
#define MCF_DMA_IMCR_IMC31_FEC1TX    (0x00000000)
#define MCF_DMA_IMCR_IMC31_CTM7      (0x80000000)
#define MCF_DMA_IMCR_IMC31_PSC3TX    (0xC0000000)

/* Bit definitions and macros for MCF_DMA_TSKSZ0 */
#define MCF_DMA_TSKSZ0_DSTSZ7(x)             (((x)&0x3)<<0)
#define MCF_DMA_TSKSZ0_SRCSZ7(x)             (((x)&0x3)<<0x2)
#define MCF_DMA_TSKSZ0_DSTSZ6(x)             (((x)&0x3)<<0x4)
#define MCF_DMA_TSKSZ0_SRCSZ6(x)             (((x)&0x3)<<0x6)
#define MCF_DMA_TSKSZ0_DSTSZ5(x)             (((x)&0x3)<<0x8)
#define MCF_DMA_TSKSZ0_SRCSZ5(x)             (((x)&0x3)<<0xA)
#define MCF_DMA_TSKSZ0_DSTSZ4(x)             (((x)&0x3)<<0xC)
#define MCF_DMA_TSKSZ0_SRCSZ4(x)             (((x)&0x3)<<0xE)
#define MCF_DMA_TSKSZ0_DSTSZ3(x)             (((x)&0x3)<<0x10)
#define MCF_DMA_TSKSZ0_SRCSZ3(x)             (((x)&0x3)<<0x12)
#define MCF_DMA_TSKSZ0_DSTSZ2(x)             (((x)&0x3)<<0x14)
#define MCF_DMA_TSKSZ0_SRCSZ2(x)             (((x)&0x3)<<0x16)
#define MCF_DMA_TSKSZ0_DSTSZ1(x)             (((x)&0x3)<<0x18)
#define MCF_DMA_TSKSZ0_SRCSZ1(x)             (((x)&0x3)<<0x1A)
#define MCF_DMA_TSKSZ0_DSTSZ0(x)             (((x)&0x3)<<0x1C)
#define MCF_DMA_TSKSZ0_SRCSZ0(x)             (((x)&0x3)<<0x1E)

/* Bit definitions and macros for MCF_DMA_TSKSZ1 */
#define MCF_DMA_TSKSZ1_DSTSZ15(x)            (((x)&0x3)<<0)
#define MCF_DMA_TSKSZ1_SRCSZ15(x)            (((x)&0x3)<<0x2)
#define MCF_DMA_TSKSZ1_DSTSZ14(x)            (((x)&0x3)<<0x4)
#define MCF_DMA_TSKSZ1_SRCSZ14(x)            (((x)&0x3)<<0x6)
#define MCF_DMA_TSKSZ1_DSTSZ13(x)            (((x)&0x3)<<0x8)
#define MCF_DMA_TSKSZ1_SRCSZ13(x)            (((x)&0x3)<<0xA)
#define MCF_DMA_TSKSZ1_DSTSZ12(x)            (((x)&0x3)<<0xC)
#define MCF_DMA_TSKSZ1_SRCSZ12(x)            (((x)&0x3)<<0xE)
#define MCF_DMA_TSKSZ1_DSTSZ11(x)            (((x)&0x3)<<0x10)
#define MCF_DMA_TSKSZ1_SRCSZ11(x)            (((x)&0x3)<<0x12)
#define MCF_DMA_TSKSZ1_DSTSZ10(x)            (((x)&0x3)<<0x14)
#define MCF_DMA_TSKSZ1_SRCSZ10(x)            (((x)&0x3)<<0x16)
#define MCF_DMA_TSKSZ1_DSTSZ9(x)             (((x)&0x3)<<0x18)
#define MCF_DMA_TSKSZ1_SRCSZ9(x)             (((x)&0x3)<<0x1A)
#define MCF_DMA_TSKSZ1_DSTSZ8(x)             (((x)&0x3)<<0x1C)
#define MCF_DMA_TSKSZ1_SRCSZ8(x)             (((x)&0x3)<<0x1E)

/* Bit definitions and macros for MCF_DMA_DBGCOMP0 */
#define MCF_DMA_DBGCOMP0_COMPARATOR_VALUE(x) (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DMA_DBGCOMP2 */
#define MCF_DMA_DBGCOMP2_COMPARATOR_VALUE(x) (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_DMA_DBGCTL */
#define MCF_DMA_DBGCTL_I                     (0x2)
#define MCF_DMA_DBGCTL_E                     (0x4)
#define MCF_DMA_DBGCTL_AND_OR                (0x80)
#define MCF_DMA_DBGCTL_COMPARATOR_TYPE_2(x)  (((x)&0x7)<<0x8)
#define MCF_DMA_DBGCTL_COMPARATOR_TYPE_1(x)  (((x)&0x7)<<0xB)
#define MCF_DMA_DBGCTL_B                     (0x4000)
#define MCF_DMA_DBGCTL_AA                    (0x8000)
#define MCF_DMA_DBGCTL_BLOCK_TASKS(x)        (((x)&0xFFFF)<<0x10)


#endif /* __MCF5475_DMA_H__ */
