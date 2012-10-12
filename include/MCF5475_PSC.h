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

#ifndef __MCF5475_PSC_H__
#define __MCF5475_PSC_H__


/*********************************************************************
*
* Programmable Serial Controller (PSC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_PSC0_PSCMR2                      (*(volatile uint8_t *)(&__MBAR[0x8600]))
#define MCF_PSC0_PSCMR1                      (*(volatile uint8_t *)(&__MBAR[0x8600]))
#define MCF_PSC0_PSCCSR                      (*(volatile uint8_t *)(&__MBAR[0x8604]))
#define MCF_PSC0_PSCSR                       (*(volatile uint16_t*)(&__MBAR[0x8604]))
#define MCF_PSC0_PSCCR                       (*(volatile uint8_t *)(&__MBAR[0x8608]))
#define MCF_PSC0_PSCRB_8BIT                  (*(volatile uint32_t*)(&__MBAR[0x860C]))
#define MCF_PSC0_PSCTB_8BIT                  (*(volatile uint32_t*)(&__MBAR[0x860C]))
#define MCF_PSC0_PSCRB_16BIT                 (*(volatile uint32_t*)(&__MBAR[0x860C]))
#define MCF_PSC0_PSCTB_16BIT                 (*(volatile uint32_t*)(&__MBAR[0x860C]))
#define MCF_PSC0_PSCRB_AC97                  (*(volatile uint32_t*)(&__MBAR[0x860C]))
#define MCF_PSC0_PSCTB_AC97                  (*(volatile uint32_t*)(&__MBAR[0x860C]))
#define MCF_PSC0_PSCIPCR                     (*(volatile uint8_t *)(&__MBAR[0x8610]))
#define MCF_PSC0_PSCACR                      (*(volatile uint8_t *)(&__MBAR[0x8610]))
#define MCF_PSC0_PSCIMR                      (*(volatile uint16_t*)(&__MBAR[0x8614]))
#define MCF_PSC0_PSCISR                      (*(volatile uint16_t*)(&__MBAR[0x8614]))
#define MCF_PSC0_PSCCTUR                     (*(volatile uint8_t *)(&__MBAR[0x8618]))
#define MCF_PSC0_PSCCTLR                     (*(volatile uint8_t *)(&__MBAR[0x861C]))
#define MCF_PSC0_PSCIP                       (*(volatile uint8_t *)(&__MBAR[0x8634]))
#define MCF_PSC0_PSCOPSET                    (*(volatile uint8_t *)(&__MBAR[0x8638]))
#define MCF_PSC0_PSCOPRESET                  (*(volatile uint8_t *)(&__MBAR[0x863C]))
#define MCF_PSC0_PSCSICR                     (*(volatile uint8_t *)(&__MBAR[0x8640]))
#define MCF_PSC0_PSCIRCR1                    (*(volatile uint8_t *)(&__MBAR[0x8644]))
#define MCF_PSC0_PSCIRCR2                    (*(volatile uint8_t *)(&__MBAR[0x8648]))
#define MCF_PSC0_PSCIRSDR                    (*(volatile uint8_t *)(&__MBAR[0x864C]))
#define MCF_PSC0_PSCIRMDR                    (*(volatile uint8_t *)(&__MBAR[0x8650]))
#define MCF_PSC0_PSCIRFDR                    (*(volatile uint8_t *)(&__MBAR[0x8654]))
#define MCF_PSC0_PSCRFCNT                    (*(volatile uint16_t*)(&__MBAR[0x8658]))
#define MCF_PSC0_PSCTFCNT                    (*(volatile uint16_t*)(&__MBAR[0x865C]))
#define MCF_PSC0_PSCRFDR                     (*(volatile uint32_t*)(&__MBAR[0x8660]))
#define MCF_PSC0_PSCRFSR                     (*(volatile uint16_t*)(&__MBAR[0x8664]))
#define MCF_PSC0_PSCRFCR                     (*(volatile uint32_t*)(&__MBAR[0x8668]))
#define MCF_PSC0_PSCRFAR                     (*(volatile uint16_t*)(&__MBAR[0x866E]))
#define MCF_PSC0_PSCRFRP                     (*(volatile uint16_t*)(&__MBAR[0x8672]))
#define MCF_PSC0_PSCRFWP                     (*(volatile uint16_t*)(&__MBAR[0x8676]))
#define MCF_PSC0_PSCRLRFP                    (*(volatile uint16_t*)(&__MBAR[0x867A]))
#define MCF_PSC0_PSCRLWFP                    (*(volatile uint16_t*)(&__MBAR[0x867E]))
#define MCF_PSC0_PSCTFDR                     (*(volatile uint32_t*)(&__MBAR[0x8680]))
#define MCF_PSC0_PSCTFSR                     (*(volatile uint16_t*)(&__MBAR[0x8684]))
#define MCF_PSC0_PSCTFCR                     (*(volatile uint32_t*)(&__MBAR[0x8688]))
#define MCF_PSC0_PSCTFAR                     (*(volatile uint16_t*)(&__MBAR[0x868E]))
#define MCF_PSC0_PSCTFRP                     (*(volatile uint16_t*)(&__MBAR[0x8692]))
#define MCF_PSC0_PSCTFWP                     (*(volatile uint16_t*)(&__MBAR[0x8696]))
#define MCF_PSC0_PSCTLRFP                    (*(volatile uint16_t*)(&__MBAR[0x869A]))
#define MCF_PSC0_PSCTLWFP                    (*(volatile uint16_t*)(&__MBAR[0x869E]))

#define MCF_PSC1_PSCMR2                      (*(volatile uint8_t *)(&__MBAR[0x8700]))
#define MCF_PSC1_PSCMR1                      (*(volatile uint8_t *)(&__MBAR[0x8700]))
#define MCF_PSC1_PSCCSR                      (*(volatile uint8_t *)(&__MBAR[0x8704]))
#define MCF_PSC1_PSCSR                       (*(volatile uint16_t*)(&__MBAR[0x8704]))
#define MCF_PSC1_PSCCR                       (*(volatile uint8_t *)(&__MBAR[0x8708]))
#define MCF_PSC1_PSCRB_8BIT                  (*(volatile uint32_t*)(&__MBAR[0x870C]))
#define MCF_PSC1_PSCTB_8BIT                  (*(volatile uint32_t*)(&__MBAR[0x870C]))
#define MCF_PSC1_PSCRB_16BIT                 (*(volatile uint32_t*)(&__MBAR[0x870C]))
#define MCF_PSC1_PSCTB_16BIT                 (*(volatile uint32_t*)(&__MBAR[0x870C]))
#define MCF_PSC1_PSCRB_AC97                  (*(volatile uint32_t*)(&__MBAR[0x870C]))
#define MCF_PSC1_PSCTB_AC97                  (*(volatile uint32_t*)(&__MBAR[0x870C]))
#define MCF_PSC1_PSCIPCR                     (*(volatile uint8_t *)(&__MBAR[0x8710]))
#define MCF_PSC1_PSCACR                      (*(volatile uint8_t *)(&__MBAR[0x8710]))
#define MCF_PSC1_PSCIMR                      (*(volatile uint16_t*)(&__MBAR[0x8714]))
#define MCF_PSC1_PSCISR                      (*(volatile uint16_t*)(&__MBAR[0x8714]))
#define MCF_PSC1_PSCCTUR                     (*(volatile uint8_t *)(&__MBAR[0x8718]))
#define MCF_PSC1_PSCCTLR                     (*(volatile uint8_t *)(&__MBAR[0x871C]))
#define MCF_PSC1_PSCIP                       (*(volatile uint8_t *)(&__MBAR[0x8734]))
#define MCF_PSC1_PSCOPSET                    (*(volatile uint8_t *)(&__MBAR[0x8738]))
#define MCF_PSC1_PSCOPRESET                  (*(volatile uint8_t *)(&__MBAR[0x873C]))
#define MCF_PSC1_PSCSICR                     (*(volatile uint8_t *)(&__MBAR[0x8740]))
#define MCF_PSC1_PSCIRCR1                    (*(volatile uint8_t *)(&__MBAR[0x8744]))
#define MCF_PSC1_PSCIRCR2                    (*(volatile uint8_t *)(&__MBAR[0x8748]))
#define MCF_PSC1_PSCIRSDR                    (*(volatile uint8_t *)(&__MBAR[0x874C]))
#define MCF_PSC1_PSCIRMDR                    (*(volatile uint8_t *)(&__MBAR[0x8750]))
#define MCF_PSC1_PSCIRFDR                    (*(volatile uint8_t *)(&__MBAR[0x8754]))
#define MCF_PSC1_PSCRFCNT                    (*(volatile uint16_t*)(&__MBAR[0x8758]))
#define MCF_PSC1_PSCTFCNT                    (*(volatile uint16_t*)(&__MBAR[0x875C]))
#define MCF_PSC1_PSCRFDR                     (*(volatile uint32_t*)(&__MBAR[0x8760]))
#define MCF_PSC1_PSCRFSR                     (*(volatile uint16_t*)(&__MBAR[0x8764]))
#define MCF_PSC1_PSCRFCR                     (*(volatile uint32_t*)(&__MBAR[0x8768]))
#define MCF_PSC1_PSCRFAR                     (*(volatile uint16_t*)(&__MBAR[0x876E]))
#define MCF_PSC1_PSCRFRP                     (*(volatile uint16_t*)(&__MBAR[0x8772]))
#define MCF_PSC1_PSCRFWP                     (*(volatile uint16_t*)(&__MBAR[0x8776]))
#define MCF_PSC1_PSCRLRFP                    (*(volatile uint16_t*)(&__MBAR[0x877A]))
#define MCF_PSC1_PSCRLWFP                    (*(volatile uint16_t*)(&__MBAR[0x877E]))
#define MCF_PSC1_PSCTFDR                     (*(volatile uint32_t*)(&__MBAR[0x8780]))
#define MCF_PSC1_PSCTFSR                     (*(volatile uint16_t*)(&__MBAR[0x8784]))
#define MCF_PSC1_PSCTFCR                     (*(volatile uint32_t*)(&__MBAR[0x8788]))
#define MCF_PSC1_PSCTFAR                     (*(volatile uint16_t*)(&__MBAR[0x878E]))
#define MCF_PSC1_PSCTFRP                     (*(volatile uint16_t*)(&__MBAR[0x8792]))
#define MCF_PSC1_PSCTFWP                     (*(volatile uint16_t*)(&__MBAR[0x8796]))
#define MCF_PSC1_PSCTLRFP                    (*(volatile uint16_t*)(&__MBAR[0x879A]))
#define MCF_PSC1_PSCTLWFP                    (*(volatile uint16_t*)(&__MBAR[0x879E]))

#define MCF_PSC2_PSCMR2                      (*(volatile uint8_t *)(&__MBAR[0x8800]))
#define MCF_PSC2_PSCMR1                      (*(volatile uint8_t *)(&__MBAR[0x8800]))
#define MCF_PSC2_PSCCSR                      (*(volatile uint8_t *)(&__MBAR[0x8804]))
#define MCF_PSC2_PSCSR                       (*(volatile uint16_t*)(&__MBAR[0x8804]))
#define MCF_PSC2_PSCCR                       (*(volatile uint8_t *)(&__MBAR[0x8808]))
#define MCF_PSC2_PSCRB_8BIT                  (*(volatile uint32_t*)(&__MBAR[0x880C]))
#define MCF_PSC2_PSCTB_8BIT                  (*(volatile uint32_t*)(&__MBAR[0x880C]))
#define MCF_PSC2_PSCRB_16BIT                 (*(volatile uint32_t*)(&__MBAR[0x880C]))
#define MCF_PSC2_PSCTB_16BIT                 (*(volatile uint32_t*)(&__MBAR[0x880C]))
#define MCF_PSC2_PSCRB_AC97                  (*(volatile uint32_t*)(&__MBAR[0x880C]))
#define MCF_PSC2_PSCTB_AC97                  (*(volatile uint32_t*)(&__MBAR[0x880C]))
#define MCF_PSC2_PSCIPCR                     (*(volatile uint8_t *)(&__MBAR[0x8810]))
#define MCF_PSC2_PSCACR                      (*(volatile uint8_t *)(&__MBAR[0x8810]))
#define MCF_PSC2_PSCIMR                      (*(volatile uint16_t*)(&__MBAR[0x8814]))
#define MCF_PSC2_PSCISR                      (*(volatile uint16_t*)(&__MBAR[0x8814]))
#define MCF_PSC2_PSCCTUR                     (*(volatile uint8_t *)(&__MBAR[0x8818]))
#define MCF_PSC2_PSCCTLR                     (*(volatile uint8_t *)(&__MBAR[0x881C]))
#define MCF_PSC2_PSCIP                       (*(volatile uint8_t *)(&__MBAR[0x8834]))
#define MCF_PSC2_PSCOPSET                    (*(volatile uint8_t *)(&__MBAR[0x8838]))
#define MCF_PSC2_PSCOPRESET                  (*(volatile uint8_t *)(&__MBAR[0x883C]))
#define MCF_PSC2_PSCSICR                     (*(volatile uint8_t *)(&__MBAR[0x8840]))
#define MCF_PSC2_PSCIRCR1                    (*(volatile uint8_t *)(&__MBAR[0x8844]))
#define MCF_PSC2_PSCIRCR2                    (*(volatile uint8_t *)(&__MBAR[0x8848]))
#define MCF_PSC2_PSCIRSDR                    (*(volatile uint8_t *)(&__MBAR[0x884C]))
#define MCF_PSC2_PSCIRMDR                    (*(volatile uint8_t *)(&__MBAR[0x8850]))
#define MCF_PSC2_PSCIRFDR                    (*(volatile uint8_t *)(&__MBAR[0x8854]))
#define MCF_PSC2_PSCRFCNT                    (*(volatile uint16_t*)(&__MBAR[0x8858]))
#define MCF_PSC2_PSCTFCNT                    (*(volatile uint16_t*)(&__MBAR[0x885C]))
#define MCF_PSC2_PSCRFDR                     (*(volatile uint32_t*)(&__MBAR[0x8860]))
#define MCF_PSC2_PSCRFSR                     (*(volatile uint16_t*)(&__MBAR[0x8864]))
#define MCF_PSC2_PSCRFCR                     (*(volatile uint32_t*)(&__MBAR[0x8868]))
#define MCF_PSC2_PSCRFAR                     (*(volatile uint16_t*)(&__MBAR[0x886E]))
#define MCF_PSC2_PSCRFRP                     (*(volatile uint16_t*)(&__MBAR[0x8872]))
#define MCF_PSC2_PSCRFWP                     (*(volatile uint16_t*)(&__MBAR[0x8876]))
#define MCF_PSC2_PSCRLRFP                    (*(volatile uint16_t*)(&__MBAR[0x887A]))
#define MCF_PSC2_PSCRLWFP                    (*(volatile uint16_t*)(&__MBAR[0x887E]))
#define MCF_PSC2_PSCTFDR                     (*(volatile uint32_t*)(&__MBAR[0x8880]))
#define MCF_PSC2_PSCTFSR                     (*(volatile uint16_t*)(&__MBAR[0x8884]))
#define MCF_PSC2_PSCTFCR                     (*(volatile uint32_t*)(&__MBAR[0x8888]))
#define MCF_PSC2_PSCTFAR                     (*(volatile uint16_t*)(&__MBAR[0x888E]))
#define MCF_PSC2_PSCTFRP                     (*(volatile uint16_t*)(&__MBAR[0x8892]))
#define MCF_PSC2_PSCTFWP                     (*(volatile uint16_t*)(&__MBAR[0x8896]))
#define MCF_PSC2_PSCTLRFP                    (*(volatile uint16_t*)(&__MBAR[0x889A]))
#define MCF_PSC2_PSCTLWFP                    (*(volatile uint16_t*)(&__MBAR[0x889E]))

#define MCF_PSC3_PSCMR2                      (*(volatile uint8_t *)(&__MBAR[0x8900]))
#define MCF_PSC3_PSCMR1                      (*(volatile uint8_t *)(&__MBAR[0x8900]))
#define MCF_PSC3_PSCCSR                      (*(volatile uint8_t *)(&__MBAR[0x8904]))
#define MCF_PSC3_PSCSR                       (*(volatile uint16_t*)(&__MBAR[0x8904]))
#define MCF_PSC3_PSCCR                       (*(volatile uint8_t *)(&__MBAR[0x8908]))
#define MCF_PSC3_PSCRB_8BIT                  (*(volatile uint32_t*)(&__MBAR[0x890C]))
#define MCF_PSC3_PSCTB_8BIT                  (*(volatile uint32_t*)(&__MBAR[0x890C]))
#define MCF_PSC3_PSCRB_16BIT                 (*(volatile uint32_t*)(&__MBAR[0x890C]))
#define MCF_PSC3_PSCTB_16BIT                 (*(volatile uint32_t*)(&__MBAR[0x890C]))
#define MCF_PSC3_PSCRB_AC97                  (*(volatile uint32_t*)(&__MBAR[0x890C]))
#define MCF_PSC3_PSCTB_AC97                  (*(volatile uint32_t*)(&__MBAR[0x890C]))
#define MCF_PSC3_PSCIPCR                     (*(volatile uint8_t *)(&__MBAR[0x8910]))
#define MCF_PSC3_PSCACR                      (*(volatile uint8_t *)(&__MBAR[0x8910]))
#define MCF_PSC3_PSCIMR                      (*(volatile uint16_t*)(&__MBAR[0x8914]))
#define MCF_PSC3_PSCISR                      (*(volatile uint16_t*)(&__MBAR[0x8914]))
#define MCF_PSC3_PSCCTUR                     (*(volatile uint8_t *)(&__MBAR[0x8918]))
#define MCF_PSC3_PSCCTLR                     (*(volatile uint8_t *)(&__MBAR[0x891C]))
#define MCF_PSC3_PSCIP                       (*(volatile uint8_t *)(&__MBAR[0x8934]))
#define MCF_PSC3_PSCOPSET                    (*(volatile uint8_t *)(&__MBAR[0x8938]))
#define MCF_PSC3_PSCOPRESET                  (*(volatile uint8_t *)(&__MBAR[0x893C]))
#define MCF_PSC3_PSCSICR                     (*(volatile uint8_t *)(&__MBAR[0x8940]))
#define MCF_PSC3_PSCIRCR1                    (*(volatile uint8_t *)(&__MBAR[0x8944]))
#define MCF_PSC3_PSCIRCR2                    (*(volatile uint8_t *)(&__MBAR[0x8948]))
#define MCF_PSC3_PSCIRSDR                    (*(volatile uint8_t *)(&__MBAR[0x894C]))
#define MCF_PSC3_PSCIRMDR                    (*(volatile uint8_t *)(&__MBAR[0x8950]))
#define MCF_PSC3_PSCIRFDR                    (*(volatile uint8_t *)(&__MBAR[0x8954]))
#define MCF_PSC3_PSCRFCNT                    (*(volatile uint16_t*)(&__MBAR[0x8958]))
#define MCF_PSC3_PSCTFCNT                    (*(volatile uint16_t*)(&__MBAR[0x895C]))
#define MCF_PSC3_PSCRFDR                     (*(volatile uint32_t*)(&__MBAR[0x8960]))
#define MCF_PSC3_PSCRFSR                     (*(volatile uint16_t*)(&__MBAR[0x8964]))
#define MCF_PSC3_PSCRFCR                     (*(volatile uint32_t*)(&__MBAR[0x8968]))
#define MCF_PSC3_PSCRFAR                     (*(volatile uint16_t*)(&__MBAR[0x896E]))
#define MCF_PSC3_PSCRFRP                     (*(volatile uint16_t*)(&__MBAR[0x8972]))
#define MCF_PSC3_PSCRFWP                     (*(volatile uint16_t*)(&__MBAR[0x8976]))
#define MCF_PSC3_PSCRLRFP                    (*(volatile uint16_t*)(&__MBAR[0x897A]))
#define MCF_PSC3_PSCRLWFP                    (*(volatile uint16_t*)(&__MBAR[0x897E]))
#define MCF_PSC3_PSCTFDR                     (*(volatile uint32_t*)(&__MBAR[0x8980]))
#define MCF_PSC3_PSCTFSR                     (*(volatile uint16_t*)(&__MBAR[0x8984]))
#define MCF_PSC3_PSCTFCR                     (*(volatile uint32_t*)(&__MBAR[0x8988]))
#define MCF_PSC3_PSCTFAR                     (*(volatile uint16_t*)(&__MBAR[0x898E]))
#define MCF_PSC3_PSCTFRP                     (*(volatile uint16_t*)(&__MBAR[0x8992]))
#define MCF_PSC3_PSCTFWP                     (*(volatile uint16_t*)(&__MBAR[0x8996]))
#define MCF_PSC3_PSCTLRFP                    (*(volatile uint16_t*)(&__MBAR[0x899A]))
#define MCF_PSC3_PSCTLWFP                    (*(volatile uint16_t*)(&__MBAR[0x899E]))

#define MCF_PSC_PSCMR(x)                     (*(volatile uint8_t *)(&__MBAR[0x8600 + ((x)*0x100)]))
#define MCF_PSC_PSCCSR(x)                    (*(volatile uint8_t *)(&__MBAR[0x8604 + ((x)*0x100)]))
#define MCF_PSC_PSCSR(x)                     (*(volatile uint16_t*)(&__MBAR[0x8604 + ((x)*0x100)]))
#define MCF_PSC_PSCCR(x)                     (*(volatile uint8_t *)(&__MBAR[0x8608 + ((x)*0x100)]))
#define MCF_PSC_PSCRB_8BIT(x)                (*(volatile uint32_t*)(&__MBAR[0x860C + ((x)*0x100)]))
#define MCF_PSC_PSCTB_8BIT(x)                (*(volatile uint32_t*)(&__MBAR[0x860C + ((x)*0x100)]))
#define MCF_PSC_PSCRB_16BIT(x)               (*(volatile uint32_t*)(&__MBAR[0x860C + ((x)*0x100)]))
#define MCF_PSC_PSCTB_16BIT(x)               (*(volatile uint32_t*)(&__MBAR[0x860C + ((x)*0x100)]))
#define MCF_PSC_PSCRB_AC97(x)                (*(volatile uint32_t*)(&__MBAR[0x860C + ((x)*0x100)]))
#define MCF_PSC_PSCTB_AC97(x)                (*(volatile uint32_t*)(&__MBAR[0x860C + ((x)*0x100)]))
#define MCF_PSC_PSCIPCR(x)                   (*(volatile uint8_t *)(&__MBAR[0x8610 + ((x)*0x100)]))
#define MCF_PSC_PSCACR(x)                    (*(volatile uint8_t *)(&__MBAR[0x8610 + ((x)*0x100)]))
#define MCF_PSC_PSCIMR(x)                    (*(volatile uint16_t*)(&__MBAR[0x8614 + ((x)*0x100)]))
#define MCF_PSC_PSCISR(x)                    (*(volatile uint16_t*)(&__MBAR[0x8614 + ((x)*0x100)]))
#define MCF_PSC_PSCCTUR(x)                   (*(volatile uint8_t *)(&__MBAR[0x8618 + ((x)*0x100)]))
#define MCF_PSC_PSCCTLR(x)                   (*(volatile uint8_t *)(&__MBAR[0x861C + ((x)*0x100)]))
#define MCF_PSC_PSCIP(x)                     (*(volatile uint8_t *)(&__MBAR[0x8634 + ((x)*0x100)]))
#define MCF_PSC_PSCOPSET(x)                  (*(volatile uint8_t *)(&__MBAR[0x8638 + ((x)*0x100)]))
#define MCF_PSC_PSCOPRESET(x)                (*(volatile uint8_t *)(&__MBAR[0x863C + ((x)*0x100)]))
#define MCF_PSC_PSCSICR(x)                   (*(volatile uint8_t *)(&__MBAR[0x8640 + ((x)*0x100)]))
#define MCF_PSC_PSCIRCR1(x)                  (*(volatile uint8_t *)(&__MBAR[0x8644 + ((x)*0x100)]))
#define MCF_PSC_PSCIRCR2(x)                  (*(volatile uint8_t *)(&__MBAR[0x8648 + ((x)*0x100)]))
#define MCF_PSC_PSCIRSDR(x)                  (*(volatile uint8_t *)(&__MBAR[0x864C + ((x)*0x100)]))
#define MCF_PSC_PSCIRMDR(x)                  (*(volatile uint8_t *)(&__MBAR[0x8650 + ((x)*0x100)]))
#define MCF_PSC_PSCIRFDR(x)                  (*(volatile uint8_t *)(&__MBAR[0x8654 + ((x)*0x100)]))
#define MCF_PSC_PSCRFCNT(x)                  (*(volatile uint16_t*)(&__MBAR[0x8658 + ((x)*0x100)]))
#define MCF_PSC_PSCTFCNT(x)                  (*(volatile uint16_t*)(&__MBAR[0x865C + ((x)*0x100)]))
#define MCF_PSC_PSCRFDR(x)                   (*(volatile uint32_t*)(&__MBAR[0x8660 + ((x)*0x100)]))
#define MCF_PSC_PSCRFSR(x)                   (*(volatile uint16_t*)(&__MBAR[0x8664 + ((x)*0x100)]))
#define MCF_PSC_PSCRFCR(x)                   (*(volatile uint32_t*)(&__MBAR[0x8668 + ((x)*0x100)]))
#define MCF_PSC_PSCRFAR(x)                   (*(volatile uint16_t*)(&__MBAR[0x866E + ((x)*0x100)]))
#define MCF_PSC_PSCRFRP(x)                   (*(volatile uint16_t*)(&__MBAR[0x8672 + ((x)*0x100)]))
#define MCF_PSC_PSCRFWP(x)                   (*(volatile uint16_t*)(&__MBAR[0x8676 + ((x)*0x100)]))
#define MCF_PSC_PSCRLRFP(x)                  (*(volatile uint16_t*)(&__MBAR[0x867A + ((x)*0x100)]))
#define MCF_PSC_PSCRLWFP(x)                  (*(volatile uint16_t*)(&__MBAR[0x867E + ((x)*0x100)]))
#define MCF_PSC_PSCTFDR(x)                   (*(volatile uint32_t*)(&__MBAR[0x8680 + ((x)*0x100)]))
#define MCF_PSC_PSCTFSR(x)                   (*(volatile uint16_t*)(&__MBAR[0x8684 + ((x)*0x100)]))
#define MCF_PSC_PSCTFCR(x)                   (*(volatile uint32_t*)(&__MBAR[0x8688 + ((x)*0x100)]))
#define MCF_PSC_PSCTFAR(x)                   (*(volatile uint16_t*)(&__MBAR[0x868E + ((x)*0x100)]))
#define MCF_PSC_PSCTFRP(x)                   (*(volatile uint16_t*)(&__MBAR[0x8692 + ((x)*0x100)]))
#define MCF_PSC_PSCTFWP(x)                   (*(volatile uint16_t*)(&__MBAR[0x8696 + ((x)*0x100)]))
#define MCF_PSC_PSCTLRFP(x)                  (*(volatile uint16_t*)(&__MBAR[0x869A + ((x)*0x100)]))
#define MCF_PSC_PSCTLWFP(x)                  (*(volatile uint16_t*)(&__MBAR[0x869E + ((x)*0x100)]))

/* Bit definitions and macros for MCF_PSC_PSCMR */
#define MCF_PSC_PSCMR_SB(x)                  (((x)&0xF)<<0)
#define MCF_PSC_PSCMR_TXCTS                  (0x10)
#define MCF_PSC_PSCMR_TXRTS                  (0x20)
#define MCF_PSC_PSCMR_CM(x)                  (((x)&0x3)<<0x6)
#define MCF_PSC_PSCMR_CM_NORMAL              (0)
#define MCF_PSC_PSCMR_CM_ECHO                (0x40)
#define MCF_PSC_PSCMR_CM_LOCAL_LOOP          (0x80)
#define MCF_PSC_PSCMR_CM_REMOTE_LOOP         (0xC0)
#define MCF_PSC_PSCMR_SB_STOP_BITS_1         (0x7)
#define MCF_PSC_PSCMR_SB_STOP_BITS_15        (0x8)
#define MCF_PSC_PSCMR_SB_STOP_BITS_2         (0xF)
#define MCF_PSC_PSCMR_PM_MULTI_ADDR          (0x1C)
#define MCF_PSC_PSCMR_PM_MULTI_DATA          (0x18)
#define MCF_PSC_PSCMR_PM_NONE                (0x10)
#define MCF_PSC_PSCMR_PM_FORCE_HI            (0xC)
#define MCF_PSC_PSCMR_PM_FORCE_LO            (0x8)
#define MCF_PSC_PSCMR_PM_ODD                 (0x4)
#define MCF_PSC_PSCMR_PM_EVEN                (0)
#define MCF_PSC_PSCMR_BC(x)                  (((x)&0x3)<<0)
#define MCF_PSC_PSCMR_BC_5                   (0)
#define MCF_PSC_PSCMR_BC_6                   (0x1)
#define MCF_PSC_PSCMR_BC_7                   (0x2)
#define MCF_PSC_PSCMR_BC_8                   (0x3)
#define MCF_PSC_PSCMR_PT                     (0x4)
#define MCF_PSC_PSCMR_PM(x)                  (((x)&0x3)<<0x3)
#define MCF_PSC_PSCMR_ERR                    (0x20)
#define MCF_PSC_PSCMR_RXIRQ_FU               (0x40)
#define MCF_PSC_PSCMR_RXRTS                  (0x80)

/* Bit definitions and macros for MCF_PSC_PSCCSR */
#define MCF_PSC_PSCCSR_TCSEL(x)              (((x)&0xF)<<0)
#define MCF_PSC_PSCCSR_RCSEL(x)              (((x)&0xF)<<0x4)
#define MCF_PSC_PSCCSR_TCSEL_SYS_CLK         (0x0D)
#define MCF_PSC_PSCCSR_TCSEL_CTM16           (0x0E)
#define MCF_PSC_PSCCSR_TCSEL_CTM             (0x0F)
#define MCF_PSC_PSCCSR_RCSEL_SYS_CLK         (0xD0)
#define MCF_PSC_PSCCSR_RCSEL_CTM16           (0xE0)
#define MCF_PSC_PSCCSR_RCSEL_CTM             (0xF0)

/* Bit definitions and macros for MCF_PSC_PSCSR */
#define MCF_PSC_PSCSR_ERR                    (0x40)
#define MCF_PSC_PSCSR_CDE_DEOF               (0x80)
#define MCF_PSC_PSCSR_RXRDY                  (0x100)
#define MCF_PSC_PSCSR_FU                     (0x200)
#define MCF_PSC_PSCSR_TXRDY                  (0x400)
#define MCF_PSC_PSCSR_TXEMP_URERR            (0x800)
#define MCF_PSC_PSCSR_OE                     (0x1000)
#define MCF_PSC_PSCSR_PE_CRCERR              (0x2000)
#define MCF_PSC_PSCSR_FE_PHYERR              (0x4000)
#define MCF_PSC_PSCSR_RB_NEOF                (0x8000)

/* Bit definitions and macros for MCF_PSC_PSCCR */
#define MCF_PSC_PSCCR_RXC(x)                 (((x)&0x3)<<0)
#define MCF_PSC_PSCCR_RX_ENABLED             (0x1)
#define MCF_PSC_PSCCR_RX_DISABLED            (0x2)
#define MCF_PSC_PSCCR_TXC(x)                 (((x)&0x3)<<0x2)
#define MCF_PSC_PSCCR_TX_ENABLED             (0x4)
#define MCF_PSC_PSCCR_TX_DISABLED            (0x8)
#define MCF_PSC_PSCCR_MISC(x)                (((x)&0x7)<<0x4)
#define MCF_PSC_PSCCR_NONE                   (0)
#define MCF_PSC_PSCCR_RESET_MR               (0x10)
#define MCF_PSC_PSCCR_RESET_RX               (0x20)
#define MCF_PSC_PSCCR_RESET_TX               (0x30)
#define MCF_PSC_PSCCR_RESET_ERROR            (0x40)
#define MCF_PSC_PSCCR_RESET_BKCHGINT         (0x50)
#define MCF_PSC_PSCCR_START_BREAK            (0x60)
#define MCF_PSC_PSCCR_STOP_BREAK             (0x70)

/* Bit definitions and macros for MCF_PSC_PSCRB_8BIT */
#define MCF_PSC_PSCRB_8BIT_RB3(x)            (((x)&0xFF)<<0)
#define MCF_PSC_PSCRB_8BIT_RB2(x)            (((x)&0xFF)<<0x8)
#define MCF_PSC_PSCRB_8BIT_RB1(x)            (((x)&0xFF)<<0x10)
#define MCF_PSC_PSCRB_8BIT_RB0(x)            (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_PSC_PSCTB_8BIT */
#define MCF_PSC_PSCTB_8BIT_TB3(x)            (((x)&0xFF)<<0)
#define MCF_PSC_PSCTB_8BIT_TB2(x)            (((x)&0xFF)<<0x8)
#define MCF_PSC_PSCTB_8BIT_TB1(x)            (((x)&0xFF)<<0x10)
#define MCF_PSC_PSCTB_8BIT_TB0(x)            (((x)&0xFF)<<0x18)

/* Bit definitions and macros for MCF_PSC_PSCRB_16BIT */
#define MCF_PSC_PSCRB_16BIT_RB1(x)           (((x)&0xFFFF)<<0)
#define MCF_PSC_PSCRB_16BIT_RB0(x)           (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_PSC_PSCTB_16BIT */
#define MCF_PSC_PSCTB_16BIT_TB1(x)           (((x)&0xFFFF)<<0)
#define MCF_PSC_PSCTB_16BIT_TB0(x)           (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_PSC_PSCRB_AC97 */
#define MCF_PSC_PSCRB_AC97_SOF               (0x800)
#define MCF_PSC_PSCRB_AC97_RB(x)             (((x)&0xFFFFF)<<0xC)

/* Bit definitions and macros for MCF_PSC_PSCTB_AC97 */
#define MCF_PSC_PSCTB_AC97_TB(x)             (((x)&0xFFFFF)<<0xC)

/* Bit definitions and macros for MCF_PSC_PSCIPCR */
#define MCF_PSC_PSCIPCR_RESERVED             (0xC)
#define MCF_PSC_PSCIPCR_CTS                  (0xD)
#define MCF_PSC_PSCIPCR_D_CTS                (0x1C)
#define MCF_PSC_PSCIPCR_SYNC                 (0x8C)

/* Bit definitions and macros for MCF_PSC_PSCACR */
#define MCF_PSC_PSCACR_IEC0                  (0x1)

/* Bit definitions and macros for MCF_PSC_PSCIMR */
#define MCF_PSC_PSCIMR_ERR                   (0x40)
#define MCF_PSC_PSCIMR_DEOF                  (0x80)
#define MCF_PSC_PSCIMR_TXRDY                 (0x100)
#define MCF_PSC_PSCIMR_RXRDY_FU              (0x200)
#define MCF_PSC_PSCIMR_DB                    (0x400)
#define MCF_PSC_PSCIMR_IPC                   (0x8000)

/* Bit definitions and macros for MCF_PSC_PSCISR */
#define MCF_PSC_PSCISR_ERR                   (0x40)
#define MCF_PSC_PSCISR_DEOF                  (0x80)
#define MCF_PSC_PSCISR_TXRDY                 (0x100)
#define MCF_PSC_PSCISR_RXRDY_FU              (0x200)
#define MCF_PSC_PSCISR_DB                    (0x400)
#define MCF_PSC_PSCISR_IPC                   (0x8000)

/* Bit definitions and macros for MCF_PSC_PSCCTUR */
#define MCF_PSC_PSCCTUR_CT(x)                (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCCTLR */
#define MCF_PSC_PSCCTLR_CT(x)                (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCIP */
#define MCF_PSC_PSCIP_CTS                    (0x1)
#define MCF_PSC_PSCIP_TGL                    (0x40)
#define MCF_PSC_PSCIP_LPWR_B                 (0x80)

/* Bit definitions and macros for MCF_PSC_PSCOPSET */
#define MCF_PSC_PSCOPSET_RTS                 (0x1)

/* Bit definitions and macros for MCF_PSC_PSCOPRESET */
#define MCF_PSC_PSCOPRESET_RTS               (0x1)

/* Bit definitions and macros for MCF_PSC_PSCSICR */
#define MCF_PSC_PSCSICR_SIM(x)               (((x)&0x7)<<0)
#define MCF_PSC_PSCSICR_SIM_UART             (0)
#define MCF_PSC_PSCSICR_SIM_MODEM8           (0x1)
#define MCF_PSC_PSCSICR_SIM_MODEM16          (0x2)
#define MCF_PSC_PSCSICR_SIM_AC97             (0x3)
#define MCF_PSC_PSCSICR_SIM_SIR              (0x4)
#define MCF_PSC_PSCSICR_SIM_MIR              (0x5)
#define MCF_PSC_PSCSICR_SIM_FIR              (0x6)
#define MCF_PSC_PSCSICR_SHDIR                (0x10)
#define MCF_PSC_PSCSICR_DTS1                 (0x20)
#define MCF_PSC_PSCSICR_AWR                  (0x40)
#define MCF_PSC_PSCSICR_ACRB                 (0x80)

/* Bit definitions and macros for MCF_PSC_PSCIRCR1 */
#define MCF_PSC_PSCIRCR1_SPUL                (0x1)
#define MCF_PSC_PSCIRCR1_SIPEN               (0x2)
#define MCF_PSC_PSCIRCR1_FD                  (0x4)

/* Bit definitions and macros for MCF_PSC_PSCIRCR2 */
#define MCF_PSC_PSCIRCR2_NXTEOF              (0x1)
#define MCF_PSC_PSCIRCR2_ABORT               (0x2)
#define MCF_PSC_PSCIRCR2_SIPREQ              (0x4)

/* Bit definitions and macros for MCF_PSC_PSCIRSDR */
#define MCF_PSC_PSCIRSDR_IRSTIM(x)           (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCIRMDR */
#define MCF_PSC_PSCIRMDR_M_FDIV(x)           (((x)&0x7F)<<0)
#define MCF_PSC_PSCIRMDR_FREQ                (0x80)

/* Bit definitions and macros for MCF_PSC_PSCIRFDR */
#define MCF_PSC_PSCIRFDR_F_FDIV(x)           (((x)&0xF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCRFCNT */
#define MCF_PSC_PSCRFCNT_CNT(x)              (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCTFCNT */
#define MCF_PSC_PSCTFCNT_CNT(x)              (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCRFDR */
#define MCF_PSC_PSCRFDR_DATA(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCRFSR */
#define MCF_PSC_PSCRFSR_EMT                  (0x1)
#define MCF_PSC_PSCRFSR_ALARM                (0x2)
#define MCF_PSC_PSCRFSR_FU                   (0x4)
#define MCF_PSC_PSCRFSR_FRMRDY               (0x8)
#define MCF_PSC_PSCRFSR_OF                   (0x10)
#define MCF_PSC_PSCRFSR_UF                   (0x20)
#define MCF_PSC_PSCRFSR_RXW                  (0x40)
#define MCF_PSC_PSCRFSR_FAE                  (0x80)
#define MCF_PSC_PSCRFSR_FRM(x)               (((x)&0xF)<<0x8)
#define MCF_PSC_PSCRFSR_FRM_BYTE0            (0x800)
#define MCF_PSC_PSCRFSR_FRM_BYTE1            (0x400)
#define MCF_PSC_PSCRFSR_FRM_BYTE2            (0x200)
#define MCF_PSC_PSCRFSR_FRM_BYTE3            (0x100)
#define MCF_PSC_PSCRFSR_TAG(x)               (((x)&0x3)<<0xC)
#define MCF_PSC_PSCRFSR_TXW                  (0x4000)
#define MCF_PSC_PSCRFSR_IP                   (0x8000)

/* Bit definitions and macros for MCF_PSC_PSCRFCR */
#define MCF_PSC_PSCRFCR_CNTR(x)              (((x)&0xFFFF)<<0)
#define MCF_PSC_PSCRFCR_TXW_MSK              (0x40000)
#define MCF_PSC_PSCRFCR_OF_MSK               (0x80000)
#define MCF_PSC_PSCRFCR_UF_MSK               (0x100000)
#define MCF_PSC_PSCRFCR_RXW_MSK              (0x200000)
#define MCF_PSC_PSCRFCR_FAE_MSK              (0x400000)
#define MCF_PSC_PSCRFCR_IP_MSK               (0x800000)
#define MCF_PSC_PSCRFCR_GR(x)                (((x)&0x7)<<0x18)
#define MCF_PSC_PSCRFCR_FRMEN                (0x8000000)
#define MCF_PSC_PSCRFCR_TIMER                (0x10000000)

/* Bit definitions and macros for MCF_PSC_PSCRFAR */
#define MCF_PSC_PSCRFAR_ALARM(x)             (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCRFRP */
#define MCF_PSC_PSCRFRP_READ(x)              (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCRFWP */
#define MCF_PSC_PSCRFWP_WRITE(x)             (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCRLRFP */
#define MCF_PSC_PSCRLRFP_LRFP(x)             (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCRLWFP */
#define MCF_PSC_PSCRLWFP_LWFP(x)             (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCTFDR */
#define MCF_PSC_PSCTFDR_DATA(x)              (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCTFSR */
#define MCF_PSC_PSCTFSR_EMT                  (0x1)
#define MCF_PSC_PSCTFSR_ALARM                (0x2)
#define MCF_PSC_PSCTFSR_FU                   (0x4)
#define MCF_PSC_PSCTFSR_FRMRDY               (0x8)
#define MCF_PSC_PSCTFSR_OF                   (0x10)
#define MCF_PSC_PSCTFSR_UF                   (0x20)
#define MCF_PSC_PSCTFSR_RXW                  (0x40)
#define MCF_PSC_PSCTFSR_FAE                  (0x80)
#define MCF_PSC_PSCTFSR_FRM(x)               (((x)&0xF)<<0x8)
#define MCF_PSC_PSCTFSR_FRM_BYTE0            (0x800)
#define MCF_PSC_PSCTFSR_FRM_BYTE1            (0x400)
#define MCF_PSC_PSCTFSR_FRM_BYTE2            (0x200)
#define MCF_PSC_PSCTFSR_FRM_BYTE3            (0x100)
#define MCF_PSC_PSCTFSR_TAG(x)               (((x)&0x3)<<0xC)
#define MCF_PSC_PSCTFSR_TXW                  (0x4000)
#define MCF_PSC_PSCTFSR_IP                   (0x8000)

/* Bit definitions and macros for MCF_PSC_PSCTFCR */
#define MCF_PSC_PSCTFCR_CNTR(x)              (((x)&0xFFFF)<<0)
#define MCF_PSC_PSCTFCR_TXW_MSK              (0x40000)
#define MCF_PSC_PSCTFCR_OF_MSK               (0x80000)
#define MCF_PSC_PSCTFCR_UF_MSK               (0x100000)
#define MCF_PSC_PSCTFCR_RXW_MSK              (0x200000)
#define MCF_PSC_PSCTFCR_FAE_MSK              (0x400000)
#define MCF_PSC_PSCTFCR_IP_MSK               (0x800000)
#define MCF_PSC_PSCTFCR_GR(x)                (((x)&0x7)<<0x18)
#define MCF_PSC_PSCTFCR_FRMEN                (0x8000000)
#define MCF_PSC_PSCTFCR_TIMER                (0x10000000)
#define MCF_PSC_PSCTFCR_WFR                  (0x20000000)

/* Bit definitions and macros for MCF_PSC_PSCTFAR */
#define MCF_PSC_PSCTFAR_ALARM(x)             (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCTFRP */
#define MCF_PSC_PSCTFRP_READ(x)              (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCTFWP */
#define MCF_PSC_PSCTFWP_WRITE(x)             (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCTLRFP */
#define MCF_PSC_PSCTLRFP_LRFP(x)             (((x)&0x1FF)<<0)

/* Bit definitions and macros for MCF_PSC_PSCTLWFP */
#define MCF_PSC_PSCTLWFP_LWFP(x)             (((x)&0x1FF)<<0)


#endif /* __MCF5475_PSC_H__ */
