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

#ifndef __MCF5475_GPT_H__
#define __MCF5475_GPT_H__


/*********************************************************************
*
* General Purpose Timers (GPT)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_GPT0_GMS                         (*(volatile uint32_t*)(&__MBAR[0x800]))
#define MCF_GPT0_GCIR                        (*(volatile uint32_t*)(&__MBAR[0x804]))
#define MCF_GPT0_GPWM                        (*(volatile uint32_t*)(&__MBAR[0x808]))
#define MCF_GPT0_GSR                         (*(volatile uint32_t*)(&__MBAR[0x80C]))

#define MCF_GPT1_GMS                         (*(volatile uint32_t*)(&__MBAR[0x810]))
#define MCF_GPT1_GCIR                        (*(volatile uint32_t*)(&__MBAR[0x814]))
#define MCF_GPT1_GPWM                        (*(volatile uint32_t*)(&__MBAR[0x818]))
#define MCF_GPT1_GSR                         (*(volatile uint32_t*)(&__MBAR[0x81C]))

#define MCF_GPT2_GMS                         (*(volatile uint32_t*)(&__MBAR[0x820]))
#define MCF_GPT2_GCIR                        (*(volatile uint32_t*)(&__MBAR[0x824]))
#define MCF_GPT2_GPWM                        (*(volatile uint32_t*)(&__MBAR[0x828]))
#define MCF_GPT2_GSR                         (*(volatile uint32_t*)(&__MBAR[0x82C]))

#define MCF_GPT3_GMS                         (*(volatile uint32_t*)(&__MBAR[0x830]))
#define MCF_GPT3_GCIR                        (*(volatile uint32_t*)(&__MBAR[0x834]))
#define MCF_GPT3_GPWM                        (*(volatile uint32_t*)(&__MBAR[0x838]))
#define MCF_GPT3_GSR                         (*(volatile uint32_t*)(&__MBAR[0x83C]))

#define MCF_GPT_GMS(x)                       (*(volatile uint32_t*)(&__MBAR[0x800 + ((x)*0x10)]))
#define MCF_GPT_GCIR(x)                      (*(volatile uint32_t*)(&__MBAR[0x804 + ((x)*0x10)]))
#define MCF_GPT_GPWM(x)                      (*(volatile uint32_t*)(&__MBAR[0x808 + ((x)*0x10)]))
#define MCF_GPT_GSR(x)                       (*(volatile uint32_t*)(&__MBAR[0x80C + ((x)*0x10)]))


/* Bit definitions and macros for MCF_GPT_GMS */
#define MCF_GPT_GMS_TMS(x)                   (((x)&0x7)<<0)
#define MCF_GPT_GMS_TMS_DISABLE              (0)
#define MCF_GPT_GMS_TMS_INCAPT               (0x1)
#define MCF_GPT_GMS_TMS_OUTCAPT              (0x2)
#define MCF_GPT_GMS_TMS_PWM                  (0x3)
#define MCF_GPT_GMS_TMS_GPIO                 (0x4)
#define MCF_GPT_GMS_GPIO(x)                  (((x)&0x3)<<0x4)
#define MCF_GPT_GMS_GPIO_INPUT               (0)
#define MCF_GPT_GMS_GPIO_OUTLO               (0x20)
#define MCF_GPT_GMS_GPIO_OUTHI               (0x30)
#define MCF_GPT_GMS_IEN                      (0x100)
#define MCF_GPT_GMS_OD                       (0x200)
#define MCF_GPT_GMS_SC                       (0x400)
#define MCF_GPT_GMS_CE                       (0x1000)
#define MCF_GPT_GMS_WDEN                     (0x8000)
#define MCF_GPT_GMS_ICT(x)                   (((x)&0x3)<<0x10)
#define MCF_GPT_GMS_ICT_ANY                  (0)
#define MCF_GPT_GMS_ICT_RISE                 (0x10000)
#define MCF_GPT_GMS_ICT_FALL                 (0x20000)
#define MCF_GPT_GMS_ICT_PULSE                (0x30000)
#define MCF_GPT_GMS_OCT(x)                   (((x)&0x3)<<0x14)
#define MCF_GPT_GMS_OCT_FRCLOW               (0)
#define MCF_GPT_GMS_OCT_PULSEHI              (0x100000)
#define MCF_GPT_GMS_OCT_PULSELO              (0x200000)
#define MCF_GPT_GMS_OCT_TOGGLE               (0x300000)
#define MCF_GPT_GMS_OCPW(x)                  (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_GPT_GCIR */
#define MCF_GPT_GCIR_CNT(x)                  (((x)&0xFFFF)<<0)
#define MCF_GPT_GCIR_PRE(x)                  (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_GPT_GPWM */
#define MCF_GPT_GPWM_LOAD                    (0x1)
#define MCF_GPT_GPWM_PWMOP                   (0x100)
#define MCF_GPT_GPWM_WIDTH(x)                (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_GPT_GSR */
#define MCF_GPT_GSR_CAPT                     (0x1)
#define MCF_GPT_GSR_COMP                     (0x2)
#define MCF_GPT_GSR_PWMP                     (0x4)
#define MCF_GPT_GSR_TEXP                     (0x8)
#define MCF_GPT_GSR_PIN                      (0x100)
#define MCF_GPT_GSR_OVF(x)                   (((x)&0x7)<<0xC)
#define MCF_GPT_GSR_CAPTURE(x)               (((x)&0xFFFF)<<0x10)


#endif /* __MCF5475_GPT_H__ */
