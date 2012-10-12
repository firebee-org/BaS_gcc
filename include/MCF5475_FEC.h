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

#ifndef __MCF5475_FEC_H__
#define __MCF5475_FEC_H__


/*********************************************************************
*
* Fast Ethernet Controller(FEC)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_FEC0_EIR                         (*(volatile uint32_t*)(&__MBAR[0x9004]))
#define MCF_FEC0_EIMR                        (*(volatile uint32_t*)(&__MBAR[0x9008]))
#define MCF_FEC0_ECR                         (*(volatile uint32_t*)(&__MBAR[0x9024]))
#define MCF_FEC0_MMFR                        (*(volatile uint32_t*)(&__MBAR[0x9040]))
#define MCF_FEC0_MSCR                        (*(volatile uint32_t*)(&__MBAR[0x9044]))
#define MCF_FEC0_MIBC                        (*(volatile uint32_t*)(&__MBAR[0x9064]))
#define MCF_FEC0_RCR                         (*(volatile uint32_t*)(&__MBAR[0x9084]))
#define MCF_FEC0_RHR                         (*(volatile uint32_t*)(&__MBAR[0x9088]))
#define MCF_FEC0_TCR                         (*(volatile uint32_t*)(&__MBAR[0x90C4]))
#define MCF_FEC0_PALR                        (*(volatile uint32_t*)(&__MBAR[0x90E4]))
#define MCF_FEC0_PAHR                        (*(volatile uint32_t*)(&__MBAR[0x90E8]))
#define MCF_FEC0_OPD                         (*(volatile uint32_t*)(&__MBAR[0x90EC]))
#define MCF_FEC0_IAUR                        (*(volatile uint32_t*)(&__MBAR[0x9118]))
#define MCF_FEC0_IALR                        (*(volatile uint32_t*)(&__MBAR[0x911C]))
#define MCF_FEC0_GAUR                        (*(volatile uint32_t*)(&__MBAR[0x9120]))
#define MCF_FEC0_GALR                        (*(volatile uint32_t*)(&__MBAR[0x9124]))
#define MCF_FEC0_FECTFWR                     (*(volatile uint32_t*)(&__MBAR[0x9144]))
#define MCF_FEC0_FECRFDR                     (*(volatile uint32_t*)(&__MBAR[0x9184]))
#define MCF_FEC0_FECRFSR                     (*(volatile uint32_t*)(&__MBAR[0x9188]))
#define MCF_FEC0_FECRFCR                     (*(volatile uint32_t*)(&__MBAR[0x918C]))
#define MCF_FEC0_FECRLRFP                    (*(volatile uint32_t*)(&__MBAR[0x9190]))
#define MCF_FEC0_FECRLWFP                    (*(volatile uint32_t*)(&__MBAR[0x9194]))
#define MCF_FEC0_FECRFAR                     (*(volatile uint32_t*)(&__MBAR[0x9198]))
#define MCF_FEC0_FECRFRP                     (*(volatile uint32_t*)(&__MBAR[0x919C]))
#define MCF_FEC0_FECRFWP                     (*(volatile uint32_t*)(&__MBAR[0x91A0]))
#define MCF_FEC0_FECTFDR                     (*(volatile uint32_t*)(&__MBAR[0x91A4]))
#define MCF_FEC0_FECTFSR                     (*(volatile uint32_t*)(&__MBAR[0x91A8]))
#define MCF_FEC0_FECTFCR                     (*(volatile uint32_t*)(&__MBAR[0x91AC]))
#define MCF_FEC0_FECTLRFP                    (*(volatile uint32_t*)(&__MBAR[0x91B0]))
#define MCF_FEC0_FECTLWFP                    (*(volatile uint32_t*)(&__MBAR[0x91B4]))
#define MCF_FEC0_FECTFAR                     (*(volatile uint32_t*)(&__MBAR[0x91B8]))
#define MCF_FEC0_FECTFRP                     (*(volatile uint32_t*)(&__MBAR[0x91BC]))
#define MCF_FEC0_FECTFWP                     (*(volatile uint32_t*)(&__MBAR[0x91C0]))
#define MCF_FEC0_FECFRST                     (*(volatile uint32_t*)(&__MBAR[0x91C4]))
#define MCF_FEC0_FECCTCWR                    (*(volatile uint32_t*)(&__MBAR[0x91C8]))
#define MCF_FEC0_RMON_T_DROP                 (*(volatile uint32_t*)(&__MBAR[0x9200]))
#define MCF_FEC0_RMON_T_PACKETS              (*(volatile uint32_t*)(&__MBAR[0x9204]))
#define MCF_FEC0_RMON_T_BC_PKT               (*(volatile uint32_t*)(&__MBAR[0x9208]))
#define MCF_FEC0_RMON_T_MC_PKT               (*(volatile uint32_t*)(&__MBAR[0x920C]))
#define MCF_FEC0_RMON_T_CRC_ALIGN            (*(volatile uint32_t*)(&__MBAR[0x9210]))
#define MCF_FEC0_RMON_T_UNDERSIZE            (*(volatile uint32_t*)(&__MBAR[0x9214]))
#define MCF_FEC0_RMON_T_OVERSIZE             (*(volatile uint32_t*)(&__MBAR[0x9218]))
#define MCF_FEC0_RMON_T_FRAG                 (*(volatile uint32_t*)(&__MBAR[0x921C]))
#define MCF_FEC0_RMON_T_JAB                  (*(volatile uint32_t*)(&__MBAR[0x9220]))
#define MCF_FEC0_RMON_T_COL                  (*(volatile uint32_t*)(&__MBAR[0x9224]))
#define MCF_FEC0_RMON_T_P64                  (*(volatile uint32_t*)(&__MBAR[0x9228]))
#define MCF_FEC0_RMON_T_P65TO127             (*(volatile uint32_t*)(&__MBAR[0x922C]))
#define MCF_FEC0_RMON_T_P128TO255            (*(volatile uint32_t*)(&__MBAR[0x9230]))
#define MCF_FEC0_RMON_T_P256TO511            (*(volatile uint32_t*)(&__MBAR[0x9234]))
#define MCF_FEC0_RMON_T_P512TO1023           (*(volatile uint32_t*)(&__MBAR[0x9238]))
#define MCF_FEC0_RMON_T_P1024TO2047          (*(volatile uint32_t*)(&__MBAR[0x923C]))
#define MCF_FEC0_RMON_T_P_GTE2048            (*(volatile uint32_t*)(&__MBAR[0x9240]))
#define MCF_FEC0_RMON_T_OCTETS               (*(volatile uint32_t*)(&__MBAR[0x9244]))
#define MCF_FEC0_IEEE_T_DROP                 (*(volatile uint32_t*)(&__MBAR[0x9248]))
#define MCF_FEC0_IEEE_T_FRAME_OK             (*(volatile uint32_t*)(&__MBAR[0x924C]))
#define MCF_FEC0_IEEE_T_1COL                 (*(volatile uint32_t*)(&__MBAR[0x9250]))
#define MCF_FEC0_IEEE_T_MCOL                 (*(volatile uint32_t*)(&__MBAR[0x9254]))
#define MCF_FEC0_IEEE_T_DEF                  (*(volatile uint32_t*)(&__MBAR[0x9258]))
#define MCF_FEC0_IEEE_T_LCOL                 (*(volatile uint32_t*)(&__MBAR[0x925C]))
#define MCF_FEC0_IEEE_T_EXCOL                (*(volatile uint32_t*)(&__MBAR[0x9260]))
#define MCF_FEC0_IEEE_T_MACERR               (*(volatile uint32_t*)(&__MBAR[0x9264]))
#define MCF_FEC0_IEEE_T_CSERR                (*(volatile uint32_t*)(&__MBAR[0x9268]))
#define MCF_FEC0_IEEE_T_SQE                  (*(volatile uint32_t*)(&__MBAR[0x926C]))
#define MCF_FEC0_IEEE_T_FDXFC                (*(volatile uint32_t*)(&__MBAR[0x9270]))
#define MCF_FEC0_IEEE_T_OCTETS_OK            (*(volatile uint32_t*)(&__MBAR[0x9274]))
#define MCF_FEC0_RMON_R_DROP                 (*(volatile uint32_t*)(&__MBAR[0x9280]))
#define MCF_FEC0_RMON_R_PACKETS              (*(volatile uint32_t*)(&__MBAR[0x9284]))
#define MCF_FEC0_RMON_R_BC_PKT               (*(volatile uint32_t*)(&__MBAR[0x9288]))
#define MCF_FEC0_RMON_R_MC_PKT               (*(volatile uint32_t*)(&__MBAR[0x928C]))
#define MCF_FEC0_RMON_R_CRC_ALIGN            (*(volatile uint32_t*)(&__MBAR[0x9290]))
#define MCF_FEC0_RMON_R_UNDERSIZE            (*(volatile uint32_t*)(&__MBAR[0x9294]))
#define MCF_FEC0_RMON_R_OVERSIZE             (*(volatile uint32_t*)(&__MBAR[0x9298]))
#define MCF_FEC0_RMON_R_FRAG                 (*(volatile uint32_t*)(&__MBAR[0x929C]))
#define MCF_FEC0_RMON_R_JAB                  (*(volatile uint32_t*)(&__MBAR[0x92A0]))
#define MCF_FEC0_RMON_R_RESVD_0              (*(volatile uint32_t*)(&__MBAR[0x92A4]))
#define MCF_FEC0_RMON_R_P64                  (*(volatile uint32_t*)(&__MBAR[0x92A8]))
#define MCF_FEC0_RMON_R_P65TO127             (*(volatile uint32_t*)(&__MBAR[0x92AC]))
#define MCF_FEC0_RMON_R_P128TO255            (*(volatile uint32_t*)(&__MBAR[0x92B0]))
#define MCF_FEC0_RMON_R_P256TO511            (*(volatile uint32_t*)(&__MBAR[0x92B4]))
#define MCF_FEC0_RMON_R_P512TO1023           (*(volatile uint32_t*)(&__MBAR[0x92B8]))
#define MCF_FEC0_RMON_R_P1024TO2047          (*(volatile uint32_t*)(&__MBAR[0x92BC]))
#define MCF_FEC0_RMON_R_P_GTE2048            (*(volatile uint32_t*)(&__MBAR[0x92C0]))
#define MCF_FEC0_RMON_R_OCTETS               (*(volatile uint32_t*)(&__MBAR[0x92C4]))
#define MCF_FEC0_IEEE_R_DROP                 (*(volatile uint32_t*)(&__MBAR[0x92C8]))
#define MCF_FEC0_IEEE_R_FRAME_OK             (*(volatile uint32_t*)(&__MBAR[0x92CC]))
#define MCF_FEC0_IEEE_R_CRC                  (*(volatile uint32_t*)(&__MBAR[0x92D0]))
#define MCF_FEC0_IEEE_R_ALIGN                (*(volatile uint32_t*)(&__MBAR[0x92D4]))
#define MCF_FEC0_IEEE_R_MACERR               (*(volatile uint32_t*)(&__MBAR[0x92D8]))
#define MCF_FEC0_IEEE_R_FDXFC                (*(volatile uint32_t*)(&__MBAR[0x92DC]))
#define MCF_FEC0_IEEE_R_OCTETS_OK            (*(volatile uint32_t*)(&__MBAR[0x92E0]))

#define MCF_FEC1_EIR                         (*(volatile uint32_t*)(&__MBAR[0x9804]))
#define MCF_FEC1_EIMR                        (*(volatile uint32_t*)(&__MBAR[0x9808]))
#define MCF_FEC1_ECR                         (*(volatile uint32_t*)(&__MBAR[0x9824]))
#define MCF_FEC1_MMFR                        (*(volatile uint32_t*)(&__MBAR[0x9840]))
#define MCF_FEC1_MSCR                        (*(volatile uint32_t*)(&__MBAR[0x9844]))
#define MCF_FEC1_MIBC                        (*(volatile uint32_t*)(&__MBAR[0x9864]))
#define MCF_FEC1_RCR                         (*(volatile uint32_t*)(&__MBAR[0x9884]))
#define MCF_FEC1_RHR                         (*(volatile uint32_t*)(&__MBAR[0x9888]))
#define MCF_FEC1_TCR                         (*(volatile uint32_t*)(&__MBAR[0x98C4]))
#define MCF_FEC1_PALR                        (*(volatile uint32_t*)(&__MBAR[0x98E4]))
#define MCF_FEC1_PAHR                        (*(volatile uint32_t*)(&__MBAR[0x98E8]))
#define MCF_FEC1_OPD                         (*(volatile uint32_t*)(&__MBAR[0x98EC]))
#define MCF_FEC1_IAUR                        (*(volatile uint32_t*)(&__MBAR[0x9918]))
#define MCF_FEC1_IALR                        (*(volatile uint32_t*)(&__MBAR[0x991C]))
#define MCF_FEC1_GAUR                        (*(volatile uint32_t*)(&__MBAR[0x9920]))
#define MCF_FEC1_GALR                        (*(volatile uint32_t*)(&__MBAR[0x9924]))
#define MCF_FEC1_FECTFWR                     (*(volatile uint32_t*)(&__MBAR[0x9944]))
#define MCF_FEC1_FECRFDR                     (*(volatile uint32_t*)(&__MBAR[0x9984]))
#define MCF_FEC1_FECRFSR                     (*(volatile uint32_t*)(&__MBAR[0x9988]))
#define MCF_FEC1_FECRFCR                     (*(volatile uint32_t*)(&__MBAR[0x998C]))
#define MCF_FEC1_FECRLRFP                    (*(volatile uint32_t*)(&__MBAR[0x9990]))
#define MCF_FEC1_FECRLWFP                    (*(volatile uint32_t*)(&__MBAR[0x9994]))
#define MCF_FEC1_FECRFAR                     (*(volatile uint32_t*)(&__MBAR[0x9998]))
#define MCF_FEC1_FECRFRP                     (*(volatile uint32_t*)(&__MBAR[0x999C]))
#define MCF_FEC1_FECRFWP                     (*(volatile uint32_t*)(&__MBAR[0x99A0]))
#define MCF_FEC1_FECTFDR                     (*(volatile uint32_t*)(&__MBAR[0x99A4]))
#define MCF_FEC1_FECTFSR                     (*(volatile uint32_t*)(&__MBAR[0x99A8]))
#define MCF_FEC1_FECTFCR                     (*(volatile uint32_t*)(&__MBAR[0x99AC]))
#define MCF_FEC1_FECTLRFP                    (*(volatile uint32_t*)(&__MBAR[0x99B0]))
#define MCF_FEC1_FECTLWFP                    (*(volatile uint32_t*)(&__MBAR[0x99B4]))
#define MCF_FEC1_FECTFAR                     (*(volatile uint32_t*)(&__MBAR[0x99B8]))
#define MCF_FEC1_FECTFRP                     (*(volatile uint32_t*)(&__MBAR[0x99BC]))
#define MCF_FEC1_FECTFWP                     (*(volatile uint32_t*)(&__MBAR[0x99C0]))
#define MCF_FEC1_FECFRST                     (*(volatile uint32_t*)(&__MBAR[0x99C4]))
#define MCF_FEC1_FECCTCWR                    (*(volatile uint32_t*)(&__MBAR[0x99C8]))
#define MCF_FEC1_RMON_T_DROP                 (*(volatile uint32_t*)(&__MBAR[0x9A00]))
#define MCF_FEC1_RMON_T_PACKETS              (*(volatile uint32_t*)(&__MBAR[0x9A04]))
#define MCF_FEC1_RMON_T_BC_PKT               (*(volatile uint32_t*)(&__MBAR[0x9A08]))
#define MCF_FEC1_RMON_T_MC_PKT               (*(volatile uint32_t*)(&__MBAR[0x9A0C]))
#define MCF_FEC1_RMON_T_CRC_ALIGN            (*(volatile uint32_t*)(&__MBAR[0x9A10]))
#define MCF_FEC1_RMON_T_UNDERSIZE            (*(volatile uint32_t*)(&__MBAR[0x9A14]))
#define MCF_FEC1_RMON_T_OVERSIZE             (*(volatile uint32_t*)(&__MBAR[0x9A18]))
#define MCF_FEC1_RMON_T_FRAG                 (*(volatile uint32_t*)(&__MBAR[0x9A1C]))
#define MCF_FEC1_RMON_T_JAB                  (*(volatile uint32_t*)(&__MBAR[0x9A20]))
#define MCF_FEC1_RMON_T_COL                  (*(volatile uint32_t*)(&__MBAR[0x9A24]))
#define MCF_FEC1_RMON_T_P64                  (*(volatile uint32_t*)(&__MBAR[0x9A28]))
#define MCF_FEC1_RMON_T_P65TO127             (*(volatile uint32_t*)(&__MBAR[0x9A2C]))
#define MCF_FEC1_RMON_T_P128TO255            (*(volatile uint32_t*)(&__MBAR[0x9A30]))
#define MCF_FEC1_RMON_T_P256TO511            (*(volatile uint32_t*)(&__MBAR[0x9A34]))
#define MCF_FEC1_RMON_T_P512TO1023           (*(volatile uint32_t*)(&__MBAR[0x9A38]))
#define MCF_FEC1_RMON_T_P1024TO2047          (*(volatile uint32_t*)(&__MBAR[0x9A3C]))
#define MCF_FEC1_RMON_T_P_GTE2048            (*(volatile uint32_t*)(&__MBAR[0x9A40]))
#define MCF_FEC1_RMON_T_OCTETS               (*(volatile uint32_t*)(&__MBAR[0x9A44]))
#define MCF_FEC1_IEEE_T_DROP                 (*(volatile uint32_t*)(&__MBAR[0x9A48]))
#define MCF_FEC1_IEEE_T_FRAME_OK             (*(volatile uint32_t*)(&__MBAR[0x9A4C]))
#define MCF_FEC1_IEEE_T_1COL                 (*(volatile uint32_t*)(&__MBAR[0x9A50]))
#define MCF_FEC1_IEEE_T_MCOL                 (*(volatile uint32_t*)(&__MBAR[0x9A54]))
#define MCF_FEC1_IEEE_T_DEF                  (*(volatile uint32_t*)(&__MBAR[0x9A58]))
#define MCF_FEC1_IEEE_T_LCOL                 (*(volatile uint32_t*)(&__MBAR[0x9A5C]))
#define MCF_FEC1_IEEE_T_EXCOL                (*(volatile uint32_t*)(&__MBAR[0x9A60]))
#define MCF_FEC1_IEEE_T_MACERR               (*(volatile uint32_t*)(&__MBAR[0x9A64]))
#define MCF_FEC1_IEEE_T_CSERR                (*(volatile uint32_t*)(&__MBAR[0x9A68]))
#define MCF_FEC1_IEEE_T_SQE                  (*(volatile uint32_t*)(&__MBAR[0x9A6C]))
#define MCF_FEC1_IEEE_T_FDXFC                (*(volatile uint32_t*)(&__MBAR[0x9A70]))
#define MCF_FEC1_IEEE_T_OCTETS_OK            (*(volatile uint32_t*)(&__MBAR[0x9A74]))
#define MCF_FEC1_RMON_R_DROP                 (*(volatile uint32_t*)(&__MBAR[0x9A80]))
#define MCF_FEC1_RMON_R_PACKETS              (*(volatile uint32_t*)(&__MBAR[0x9A84]))
#define MCF_FEC1_RMON_R_BC_PKT               (*(volatile uint32_t*)(&__MBAR[0x9A88]))
#define MCF_FEC1_RMON_R_MC_PKT               (*(volatile uint32_t*)(&__MBAR[0x9A8C]))
#define MCF_FEC1_RMON_R_CRC_ALIGN            (*(volatile uint32_t*)(&__MBAR[0x9A90]))
#define MCF_FEC1_RMON_R_UNDERSIZE            (*(volatile uint32_t*)(&__MBAR[0x9A94]))
#define MCF_FEC1_RMON_R_OVERSIZE             (*(volatile uint32_t*)(&__MBAR[0x9A98]))
#define MCF_FEC1_RMON_R_FRAG                 (*(volatile uint32_t*)(&__MBAR[0x9A9C]))
#define MCF_FEC1_RMON_R_JAB                  (*(volatile uint32_t*)(&__MBAR[0x9AA0]))
#define MCF_FEC1_RMON_R_RESVD_0              (*(volatile uint32_t*)(&__MBAR[0x9AA4]))
#define MCF_FEC1_RMON_R_P64                  (*(volatile uint32_t*)(&__MBAR[0x9AA8]))
#define MCF_FEC1_RMON_R_P65TO127             (*(volatile uint32_t*)(&__MBAR[0x9AAC]))
#define MCF_FEC1_RMON_R_P128TO255            (*(volatile uint32_t*)(&__MBAR[0x9AB0]))
#define MCF_FEC1_RMON_R_P256TO511            (*(volatile uint32_t*)(&__MBAR[0x9AB4]))
#define MCF_FEC1_RMON_R_P512TO1023           (*(volatile uint32_t*)(&__MBAR[0x9AB8]))
#define MCF_FEC1_RMON_R_P1024TO2047          (*(volatile uint32_t*)(&__MBAR[0x9ABC]))
#define MCF_FEC1_RMON_R_P_GTE2048            (*(volatile uint32_t*)(&__MBAR[0x9AC0]))
#define MCF_FEC1_RMON_R_OCTETS               (*(volatile uint32_t*)(&__MBAR[0x9AC4]))
#define MCF_FEC1_IEEE_R_DROP                 (*(volatile uint32_t*)(&__MBAR[0x9AC8]))
#define MCF_FEC1_IEEE_R_FRAME_OK             (*(volatile uint32_t*)(&__MBAR[0x9ACC]))
#define MCF_FEC1_IEEE_R_CRC                  (*(volatile uint32_t*)(&__MBAR[0x9AD0]))
#define MCF_FEC1_IEEE_R_ALIGN                (*(volatile uint32_t*)(&__MBAR[0x9AD4]))
#define MCF_FEC1_IEEE_R_MACERR               (*(volatile uint32_t*)(&__MBAR[0x9AD8]))
#define MCF_FEC1_IEEE_R_FDXFC                (*(volatile uint32_t*)(&__MBAR[0x9ADC]))
#define MCF_FEC1_IEEE_R_OCTETS_OK            (*(volatile uint32_t*)(&__MBAR[0x9AE0]))

#define MCF_FEC_EIR(x)                       (*(volatile uint32_t*)(&__MBAR[0x9004 + ((x)*0x800)]))
#define MCF_FEC_EIMR(x)                      (*(volatile uint32_t*)(&__MBAR[0x9008 + ((x)*0x800)]))
#define MCF_FEC_ECR(x)                       (*(volatile uint32_t*)(&__MBAR[0x9024 + ((x)*0x800)]))
#define MCF_FEC_MMFR(x)                      (*(volatile uint32_t*)(&__MBAR[0x9040 + ((x)*0x800)]))
#define MCF_FEC_MSCR(x)                      (*(volatile uint32_t*)(&__MBAR[0x9044 + ((x)*0x800)]))
#define MCF_FEC_MIBC(x)                      (*(volatile uint32_t*)(&__MBAR[0x9064 + ((x)*0x800)]))
#define MCF_FEC_RCR(x)                       (*(volatile uint32_t*)(&__MBAR[0x9084 + ((x)*0x800)]))
#define MCF_FEC_RHR(x)                       (*(volatile uint32_t*)(&__MBAR[0x9088 + ((x)*0x800)]))
#define MCF_FEC_TCR(x)                       (*(volatile uint32_t*)(&__MBAR[0x90C4 + ((x)*0x800)]))
#define MCF_FEC_PALR(x)                      (*(volatile uint32_t*)(&__MBAR[0x90E4 + ((x)*0x800)]))
#define MCF_FEC_PAHR(x)                      (*(volatile uint32_t*)(&__MBAR[0x90E8 + ((x)*0x800)]))
#define MCF_FEC_OPD(x)                       (*(volatile uint32_t*)(&__MBAR[0x90EC + ((x)*0x800)]))
#define MCF_FEC_IAUR(x)                      (*(volatile uint32_t*)(&__MBAR[0x9118 + ((x)*0x800)]))
#define MCF_FEC_IALR(x)                      (*(volatile uint32_t*)(&__MBAR[0x911C + ((x)*0x800)]))
#define MCF_FEC_GAUR(x)                      (*(volatile uint32_t*)(&__MBAR[0x9120 + ((x)*0x800)]))
#define MCF_FEC_GALR(x)                      (*(volatile uint32_t*)(&__MBAR[0x9124 + ((x)*0x800)]))
#define MCF_FEC_FECTFWR(x)                   (*(volatile uint32_t*)(&__MBAR[0x9144 + ((x)*0x800)]))
#define MCF_FEC_FECRFDR(x)                   (*(volatile uint32_t*)(&__MBAR[0x9184 + ((x)*0x800)]))
#define MCF_FEC_FECRFSR(x)                   (*(volatile uint32_t*)(&__MBAR[0x9188 + ((x)*0x800)]))
#define MCF_FEC_FECRFCR(x)                   (*(volatile uint32_t*)(&__MBAR[0x918C + ((x)*0x800)]))
#define MCF_FEC_FECRLRFP(x)                  (*(volatile uint32_t*)(&__MBAR[0x9190 + ((x)*0x800)]))
#define MCF_FEC_FECRLWFP(x)                  (*(volatile uint32_t*)(&__MBAR[0x9194 + ((x)*0x800)]))
#define MCF_FEC_FECRFAR(x)                   (*(volatile uint32_t*)(&__MBAR[0x9198 + ((x)*0x800)]))
#define MCF_FEC_FECRFRP(x)                   (*(volatile uint32_t*)(&__MBAR[0x919C + ((x)*0x800)]))
#define MCF_FEC_FECRFWP(x)                   (*(volatile uint32_t*)(&__MBAR[0x91A0 + ((x)*0x800)]))
#define MCF_FEC_FECTFDR(x)                   (*(volatile uint32_t*)(&__MBAR[0x91A4 + ((x)*0x800)]))
#define MCF_FEC_FECTFSR(x)                   (*(volatile uint32_t*)(&__MBAR[0x91A8 + ((x)*0x800)]))
#define MCF_FEC_FECTFCR(x)                   (*(volatile uint32_t*)(&__MBAR[0x91AC + ((x)*0x800)]))
#define MCF_FEC_FECTLRFP(x)                  (*(volatile uint32_t*)(&__MBAR[0x91B0 + ((x)*0x800)]))
#define MCF_FEC_FECTLWFP(x)                  (*(volatile uint32_t*)(&__MBAR[0x91B4 + ((x)*0x800)]))
#define MCF_FEC_FECTFAR(x)                   (*(volatile uint32_t*)(&__MBAR[0x91B8 + ((x)*0x800)]))
#define MCF_FEC_FECTFRP(x)                   (*(volatile uint32_t*)(&__MBAR[0x91BC + ((x)*0x800)]))
#define MCF_FEC_FECTFWP(x)                   (*(volatile uint32_t*)(&__MBAR[0x91C0 + ((x)*0x800)]))
#define MCF_FEC_FECFRST(x)                   (*(volatile uint32_t*)(&__MBAR[0x91C4 + ((x)*0x800)]))
#define MCF_FEC_FECCTCWR(x)                  (*(volatile uint32_t*)(&__MBAR[0x91C8 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_DROP(x)               (*(volatile uint32_t*)(&__MBAR[0x9200 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_PACKETS(x)            (*(volatile uint32_t*)(&__MBAR[0x9204 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_BC_PKT(x)             (*(volatile uint32_t*)(&__MBAR[0x9208 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_MC_PKT(x)             (*(volatile uint32_t*)(&__MBAR[0x920C + ((x)*0x800)]))
#define MCF_FEC_RMON_T_CRC_ALIGN(x)          (*(volatile uint32_t*)(&__MBAR[0x9210 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_UNDERSIZE(x)          (*(volatile uint32_t*)(&__MBAR[0x9214 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_OVERSIZE(x)           (*(volatile uint32_t*)(&__MBAR[0x9218 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_FRAG(x)               (*(volatile uint32_t*)(&__MBAR[0x921C + ((x)*0x800)]))
#define MCF_FEC_RMON_T_JAB(x)                (*(volatile uint32_t*)(&__MBAR[0x9220 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_COL(x)                (*(volatile uint32_t*)(&__MBAR[0x9224 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_P64(x)                (*(volatile uint32_t*)(&__MBAR[0x9228 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_P65TO127(x)           (*(volatile uint32_t*)(&__MBAR[0x922C + ((x)*0x800)]))
#define MCF_FEC_RMON_T_P128TO255(x)          (*(volatile uint32_t*)(&__MBAR[0x9230 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_P256TO511(x)          (*(volatile uint32_t*)(&__MBAR[0x9234 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_P512TO1023(x)         (*(volatile uint32_t*)(&__MBAR[0x9238 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_P1024TO2047(x)        (*(volatile uint32_t*)(&__MBAR[0x923C + ((x)*0x800)]))
#define MCF_FEC_RMON_T_P_GTE2048(x)          (*(volatile uint32_t*)(&__MBAR[0x9240 + ((x)*0x800)]))
#define MCF_FEC_RMON_T_OCTETS(x)             (*(volatile uint32_t*)(&__MBAR[0x9244 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_DROP(x)               (*(volatile uint32_t*)(&__MBAR[0x9248 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_FRAME_OK(x)           (*(volatile uint32_t*)(&__MBAR[0x924C + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_1COL(x)               (*(volatile uint32_t*)(&__MBAR[0x9250 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_MCOL(x)               (*(volatile uint32_t*)(&__MBAR[0x9254 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_DEF(x)                (*(volatile uint32_t*)(&__MBAR[0x9258 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_LCOL(x)               (*(volatile uint32_t*)(&__MBAR[0x925C + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_EXCOL(x)              (*(volatile uint32_t*)(&__MBAR[0x9260 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_MACERR(x)             (*(volatile uint32_t*)(&__MBAR[0x9264 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_CSERR(x)              (*(volatile uint32_t*)(&__MBAR[0x9268 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_SQE(x)                (*(volatile uint32_t*)(&__MBAR[0x926C + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_FDXFC(x)              (*(volatile uint32_t*)(&__MBAR[0x9270 + ((x)*0x800)]))
#define MCF_FEC_IEEE_T_OCTETS_OK(x)          (*(volatile uint32_t*)(&__MBAR[0x9274 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_DROP(x)               (*(volatile uint32_t*)(&__MBAR[0x9280 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_PACKETS(x)            (*(volatile uint32_t*)(&__MBAR[0x9284 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_BC_PKT(x)             (*(volatile uint32_t*)(&__MBAR[0x9288 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_MC_PKT(x)             (*(volatile uint32_t*)(&__MBAR[0x928C + ((x)*0x800)]))
#define MCF_FEC_RMON_R_CRC_ALIGN(x)          (*(volatile uint32_t*)(&__MBAR[0x9290 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_UNDERSIZE(x)          (*(volatile uint32_t*)(&__MBAR[0x9294 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_OVERSIZE(x)           (*(volatile uint32_t*)(&__MBAR[0x9298 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_FRAG(x)               (*(volatile uint32_t*)(&__MBAR[0x929C + ((x)*0x800)]))
#define MCF_FEC_RMON_R_JAB(x)                (*(volatile uint32_t*)(&__MBAR[0x92A0 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_RESVD_0(x)            (*(volatile uint32_t*)(&__MBAR[0x92A4 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_P64(x)                (*(volatile uint32_t*)(&__MBAR[0x92A8 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_P65TO127(x)           (*(volatile uint32_t*)(&__MBAR[0x92AC + ((x)*0x800)]))
#define MCF_FEC_RMON_R_P128TO255(x)          (*(volatile uint32_t*)(&__MBAR[0x92B0 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_P256TO511(x)          (*(volatile uint32_t*)(&__MBAR[0x92B4 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_P512TO1023(x)         (*(volatile uint32_t*)(&__MBAR[0x92B8 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_P1024TO2047(x)        (*(volatile uint32_t*)(&__MBAR[0x92BC + ((x)*0x800)]))
#define MCF_FEC_RMON_R_P_GTE2048(x)          (*(volatile uint32_t*)(&__MBAR[0x92C0 + ((x)*0x800)]))
#define MCF_FEC_RMON_R_OCTETS(x)             (*(volatile uint32_t*)(&__MBAR[0x92C4 + ((x)*0x800)]))
#define MCF_FEC_IEEE_R_DROP(x)               (*(volatile uint32_t*)(&__MBAR[0x92C8 + ((x)*0x800)]))
#define MCF_FEC_IEEE_R_FRAME_OK(x)           (*(volatile uint32_t*)(&__MBAR[0x92CC + ((x)*0x800)]))
#define MCF_FEC_IEEE_R_CRC(x)                (*(volatile uint32_t*)(&__MBAR[0x92D0 + ((x)*0x800)]))
#define MCF_FEC_IEEE_R_ALIGN(x)              (*(volatile uint32_t*)(&__MBAR[0x92D4 + ((x)*0x800)]))
#define MCF_FEC_IEEE_R_MACERR(x)             (*(volatile uint32_t*)(&__MBAR[0x92D8 + ((x)*0x800)]))
#define MCF_FEC_IEEE_R_FDXFC(x)              (*(volatile uint32_t*)(&__MBAR[0x92DC + ((x)*0x800)]))
#define MCF_FEC_IEEE_R_OCTETS_OK(x)          (*(volatile uint32_t*)(&__MBAR[0x92E0 + ((x)*0x800)]))


/* Bit definitions and macros for MCF_FEC_EIR */
#define MCF_FEC_EIR_RFERR                    (0x20000)
#define MCF_FEC_EIR_XFERR                    (0x40000)
#define MCF_FEC_EIR_XFUN                     (0x80000)
#define MCF_FEC_EIR_RL                       (0x100000)
#define MCF_FEC_EIR_LC                       (0x200000)
#define MCF_FEC_EIR_MII                      (0x800000)
#define MCF_FEC_EIR_TXF                      (0x8000000)
#define MCF_FEC_EIR_GRA                      (0x10000000)
#define MCF_FEC_EIR_BABT                     (0x20000000)
#define MCF_FEC_EIR_BABR                     (0x40000000)
#define MCF_FEC_EIR_HBERR                    (0x80000000)
#define MCF_FEC_EIR_CLEAR_ALL                (0xFFFFFFFF)

/* Bit definitions and macros for MCF_FEC_EIMR */
#define MCF_FEC_EIMR_RFERR                   (0x20000)
#define MCF_FEC_EIMR_XFERR                   (0x40000)
#define MCF_FEC_EIMR_XFUN                    (0x80000)
#define MCF_FEC_EIMR_RL                      (0x100000)
#define MCF_FEC_EIMR_LC                      (0x200000)
#define MCF_FEC_EIMR_MII                     (0x800000)
#define MCF_FEC_EIMR_TXF                     (0x8000000)
#define MCF_FEC_EIMR_GRA                     (0x10000000)
#define MCF_FEC_EIMR_BABT                    (0x20000000)
#define MCF_FEC_EIMR_BABR                    (0x40000000)
#define MCF_FEC_EIMR_HBERR                   (0x80000000)
#define MCF_FEC_EIMR_MASK_ALL                (0)
#define MCF_FEC_EIMR_UNMASK_ALL              (0xFFFFFFFF)

/* Bit definitions and macros for MCF_FEC_ECR */
#define MCF_FEC_ECR_RESET                    (0x1)
#define MCF_FEC_ECR_ETHER_EN                 (0x2)

/* Bit definitions and macros for MCF_FEC_MMFR */
#define MCF_FEC_MMFR_DATA(x)                 (((x)&0xFFFF)<<0)
#define MCF_FEC_MMFR_TA(x)                   (((x)&0x3)<<0x10)
#define MCF_FEC_MMFR_TA_10                   (0x20000)
#define MCF_FEC_MMFR_RA(x)                   (((x)&0x1F)<<0x12)
#define MCF_FEC_MMFR_PA(x)                   (((x)&0x1F)<<0x17)
#define MCF_FEC_MMFR_OP(x)                   (((x)&0x3)<<0x1C)
#define MCF_FEC_MMFR_OP_READ                 (0x20000000)
#define MCF_FEC_MMFR_OP_WRITE                (0x10000000)
#define MCF_FEC_MMFR_ST(x)                   (((x)&0x3)<<0x1E)
#define MCF_FEC_MMFR_ST_01                   (0x40000000)

/* Bit definitions and macros for MCF_FEC_MSCR */
#define MCF_FEC_MSCR_MII_SPEED(x)            (((x)&0x3F)<<0x1)
#define MCF_FEC_MSCR_DIS_PREAMBLE            (0x80)
#define MCF_FEC_MSCR_MII_SPEED_133           (0x1B<<0x1)
#define MCF_FEC_MSCR_MII_SPEED_120           (0x18<<0x1)
#define MCF_FEC_MSCR_MII_SPEED_66            (0xE<<0x1)
#define MCF_FEC_MSCR_MII_SPEED_60            (0xC<<0x1)

/* Bit definitions and macros for MCF_FEC_MIBC */
#define MCF_FEC_MIBC_MIB_IDLE                (0x40000000)
#define MCF_FEC_MIBC_MIB_DISABLE             (0x80000000)

/* Bit definitions and macros for MCF_FEC_RCR */
#define MCF_FEC_RCR_LOOP                     (0x1)
#define MCF_FEC_RCR_DRT                      (0x2)
#define MCF_FEC_RCR_MII_MODE                 (0x4)
#define MCF_FEC_RCR_PROM                     (0x8)
#define MCF_FEC_RCR_BC_REJ                   (0x10)
#define MCF_FEC_RCR_FCE                      (0x20)
#define MCF_FEC_RCR_MAX_FL(x)                (((x)&0x7FF)<<0x10)

/* Bit definitions and macros for MCF_FEC_RHR */
#define MCF_FEC_RHR_HASH(x)                  (((x)&0x3F)<<0x18)
#define MCF_FEC_RHR_MULTCAST                 (0x40000000)
#define MCF_FEC_RHR_FCE                      (0x80000000)

/* Bit definitions and macros for MCF_FEC_TCR */
#define MCF_FEC_TCR_GTS                      (0x1)
#define MCF_FEC_TCR_HBC                      (0x2)
#define MCF_FEC_TCR_FDEN                     (0x4)
#define MCF_FEC_TCR_TFC_PAUSE                (0x8)
#define MCF_FEC_TCR_RFC_PAUSE                (0x10)

/* Bit definitions and macros for MCF_FEC_PALR */
#define MCF_FEC_PALR_PADDR1(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_PAHR */
#define MCF_FEC_PAHR_TYPE(x)                 (((x)&0xFFFF)<<0)
#define MCF_FEC_PAHR_PADDR2(x)               (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_FEC_OPD */
#define MCF_FEC_OPD_PAUSE_DUR(x)             (((x)&0xFFFF)<<0)
#define MCF_FEC_OPD_OPCODE(x)                (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_FEC_IAUR */
#define MCF_FEC_IAUR_IADDR1(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IALR */
#define MCF_FEC_IALR_IADDR2(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_GAUR */
#define MCF_FEC_GAUR_GADDR1(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_GALR */
#define MCF_FEC_GALR_GADDR2(x)               (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_FECTFWR */
#define MCF_FEC_FECTFWR_X_WMRK(x)            (((x)&0xF)<<0)
#define MCF_FEC_FECTFWR_X_WMRK_64            (0)
#define MCF_FEC_FECTFWR_X_WMRK_128           (0x1)
#define MCF_FEC_FECTFWR_X_WMRK_192           (0x2)
#define MCF_FEC_FECTFWR_X_WMRK_256           (0x3)
#define MCF_FEC_FECTFWR_X_WMRK_320           (0x4)
#define MCF_FEC_FECTFWR_X_WMRK_384           (0x5)
#define MCF_FEC_FECTFWR_X_WMRK_448           (0x6)
#define MCF_FEC_FECTFWR_X_WMRK_512           (0x7)
#define MCF_FEC_FECTFWR_X_WMRK_576           (0x8)
#define MCF_FEC_FECTFWR_X_WMRK_640           (0x9)
#define MCF_FEC_FECTFWR_X_WMRK_704           (0xA)
#define MCF_FEC_FECTFWR_X_WMRK_768           (0xB)
#define MCF_FEC_FECTFWR_X_WMRK_832           (0xC)
#define MCF_FEC_FECTFWR_X_WMRK_896           (0xD)
#define MCF_FEC_FECTFWR_X_WMRK_960           (0xE)
#define MCF_FEC_FECTFWR_X_WMRK_1024          (0xF)

/* Bit definitions and macros for MCF_FEC_FECRFDR */
#define MCF_FEC_FECRFDR_FIFO_DATA(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_FECRFSR */
#define MCF_FEC_FECRFSR_EMT                  (0x10000)
#define MCF_FEC_FECRFSR_ALARM                (0x20000)
#define MCF_FEC_FECRFSR_FU                   (0x40000)
#define MCF_FEC_FECRFSR_FRMRDY               (0x80000)
#define MCF_FEC_FECRFSR_OF                   (0x100000)
#define MCF_FEC_FECRFSR_UF                   (0x200000)
#define MCF_FEC_FECRFSR_RXW                  (0x400000)
#define MCF_FEC_FECRFSR_FAE                  (0x800000)
#define MCF_FEC_FECRFSR_FRM(x)               (((x)&0xF)<<0x18)
#define MCF_FEC_FECRFSR_IP                   (0x80000000)

/* Bit definitions and macros for MCF_FEC_FECRFCR */
#define MCF_FEC_FECRFCR_COUNTER(x)           (((x)&0xFFFF)<<0)
#define MCF_FEC_FECRFCR_OF_MSK               (0x80000)
#define MCF_FEC_FECRFCR_UF_MSK               (0x100000)
#define MCF_FEC_FECRFCR_RXW_MSK              (0x200000)
#define MCF_FEC_FECRFCR_FAE_MSK              (0x400000)
#define MCF_FEC_FECRFCR_IP_MSK               (0x800000)
#define MCF_FEC_FECRFCR_GR(x)                (((x)&0x7)<<0x18)
#define MCF_FEC_FECRFCR_FRMEN                (0x8000000)
#define MCF_FEC_FECRFCR_TIMER                (0x10000000)

/* Bit definitions and macros for MCF_FEC_FECRLRFP */
#define MCF_FEC_FECRLRFP_LRFP(x)             (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECRLWFP */
#define MCF_FEC_FECRLWFP_LWFP(x)             (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECRFAR */
#define MCF_FEC_FECRFAR_ALARM(x)             (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECRFRP */
#define MCF_FEC_FECRFRP_READ(x)              (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECRFWP */
#define MCF_FEC_FECRFWP_WRITE(x)             (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECTFDR */
#define MCF_FEC_FECTFDR_FIFO_DATA(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_FECTFSR */
#define MCF_FEC_FECTFSR_EMT                  (0x10000)
#define MCF_FEC_FECTFSR_ALARM                (0x20000)
#define MCF_FEC_FECTFSR_FU                   (0x40000)
#define MCF_FEC_FECTFSR_FRMRDY               (0x80000)
#define MCF_FEC_FECTFSR_OF                   (0x100000)
#define MCF_FEC_FECTFSR_UF                   (0x200000)
#define MCF_FEC_FECTFSR_FAE                  (0x800000)
#define MCF_FEC_FECTFSR_FRM(x)               (((x)&0xF)<<0x18)
#define MCF_FEC_FECTFSR_TXW                  (0x40000000)
#define MCF_FEC_FECTFSR_IP                   (0x80000000)

/* Bit definitions and macros for MCF_FEC_FECTFCR */
#define MCF_FEC_FECTFCR_RESERVED             (0x200000)
#define MCF_FEC_FECTFCR_COUNTER(x)           (((x)&0xFFFF)<<0|0x200000)
#define MCF_FEC_FECTFCR_TXW_MASK             (0x240000)
#define MCF_FEC_FECTFCR_OF_MSK               (0x280000)
#define MCF_FEC_FECTFCR_UF_MSK               (0x300000)
#define MCF_FEC_FECTFCR_FAE_MSK              (0x600000)
#define MCF_FEC_FECTFCR_IP_MSK               (0xA00000)
#define MCF_FEC_FECTFCR_GR(x)                (((x)&0x7)<<0x18|0x200000)
#define MCF_FEC_FECTFCR_FRMEN                (0x8200000)
#define MCF_FEC_FECTFCR_TIMER                (0x10200000)
#define MCF_FEC_FECTFCR_WFR                  (0x20200000)
#define MCF_FEC_FECTFCR_WCTL                 (0x40200000)

/* Bit definitions and macros for MCF_FEC_FECTLRFP */
#define MCF_FEC_FECTLRFP_LRFP(x)             (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECTLWFP */
#define MCF_FEC_FECTLWFP_LWFP(x)             (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECTFAR */
#define MCF_FEC_FECTFAR_ALARM(x)             (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECTFRP */
#define MCF_FEC_FECTFRP_READ(x)              (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECTFWP */
#define MCF_FEC_FECTFWP_WRITE(x)             (((x)&0x3FF)<<0)

/* Bit definitions and macros for MCF_FEC_FECFRST */
#define MCF_FEC_FECFRST_RST_CTL              (0x1000000)
#define MCF_FEC_FECFRST_SW_RST               (0x2000000)

/* Bit definitions and macros for MCF_FEC_FECCTCWR */
#define MCF_FEC_FECCTCWR_TFCW                (0x1000000)
#define MCF_FEC_FECCTCWR_CRC                 (0x2000000)

/* Bit definitions and macros for MCF_FEC_RMON_T_DROP */
#define MCF_FEC_RMON_T_DROP_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_PACKETS */
#define MCF_FEC_RMON_T_PACKETS_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_BC_PKT */
#define MCF_FEC_RMON_T_BC_PKT_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_MC_PKT */
#define MCF_FEC_RMON_T_MC_PKT_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_CRC_ALIGN */
#define MCF_FEC_RMON_T_CRC_ALIGN_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_UNDERSIZE */
#define MCF_FEC_RMON_T_UNDERSIZE_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_OVERSIZE */
#define MCF_FEC_RMON_T_OVERSIZE_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_FRAG */
#define MCF_FEC_RMON_T_FRAG_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_JAB */
#define MCF_FEC_RMON_T_JAB_Value(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_COL */
#define MCF_FEC_RMON_T_COL_Value(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_P64 */
#define MCF_FEC_RMON_T_P64_Value(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_P65TO127 */
#define MCF_FEC_RMON_T_P65TO127_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_P128TO255 */
#define MCF_FEC_RMON_T_P128TO255_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_P256TO511 */
#define MCF_FEC_RMON_T_P256TO511_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_P512TO1023 */
#define MCF_FEC_RMON_T_P512TO1023_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_P1024TO2047 */
#define MCF_FEC_RMON_T_P1024TO2047_Value(x)  (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_P_GTE2048 */
#define MCF_FEC_RMON_T_P_GTE2048_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_T_OCTETS */
#define MCF_FEC_RMON_T_OCTETS_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_DROP */
#define MCF_FEC_IEEE_T_DROP_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_FRAME_OK */
#define MCF_FEC_IEEE_T_FRAME_OK_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_1COL */
#define MCF_FEC_IEEE_T_1COL_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_MCOL */
#define MCF_FEC_IEEE_T_MCOL_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_DEF */
#define MCF_FEC_IEEE_T_DEF_Value(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_LCOL */
#define MCF_FEC_IEEE_T_LCOL_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_EXCOL */
#define MCF_FEC_IEEE_T_EXCOL_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_MACERR */
#define MCF_FEC_IEEE_T_MACERR_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_CSERR */
#define MCF_FEC_IEEE_T_CSERR_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_SQE */
#define MCF_FEC_IEEE_T_SQE_Value(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_FDXFC */
#define MCF_FEC_IEEE_T_FDXFC_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_T_OCTETS_OK */
#define MCF_FEC_IEEE_T_OCTETS_OK_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_DROP */
#define MCF_FEC_RMON_R_DROP_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_PACKETS */
#define MCF_FEC_RMON_R_PACKETS_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_BC_PKT */
#define MCF_FEC_RMON_R_BC_PKT_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_MC_PKT */
#define MCF_FEC_RMON_R_MC_PKT_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_CRC_ALIGN */
#define MCF_FEC_RMON_R_CRC_ALIGN_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_UNDERSIZE */
#define MCF_FEC_RMON_R_UNDERSIZE_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_OVERSIZE */
#define MCF_FEC_RMON_R_OVERSIZE_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_FRAG */
#define MCF_FEC_RMON_R_FRAG_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_JAB */
#define MCF_FEC_RMON_R_JAB_Value(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_RESVD_0 */
#define MCF_FEC_RMON_R_RESVD_0_Value(x)      (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_P64 */
#define MCF_FEC_RMON_R_P64_Value(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_P65TO127 */
#define MCF_FEC_RMON_R_P65TO127_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_P128TO255 */
#define MCF_FEC_RMON_R_P128TO255_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_P256TO511 */
#define MCF_FEC_RMON_R_P256TO511_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_P512TO1023 */
#define MCF_FEC_RMON_R_P512TO1023_Value(x)   (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_P1024TO2047 */
#define MCF_FEC_RMON_R_P1024TO2047_Value(x)  (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_P_GTE2048 */
#define MCF_FEC_RMON_R_P_GTE2048_Value(x)    (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_RMON_R_OCTETS */
#define MCF_FEC_RMON_R_OCTETS_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_R_DROP */
#define MCF_FEC_IEEE_R_DROP_Value(x)         (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_R_FRAME_OK */
#define MCF_FEC_IEEE_R_FRAME_OK_Value(x)     (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_R_CRC */
#define MCF_FEC_IEEE_R_CRC_Value(x)          (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_R_ALIGN */
#define MCF_FEC_IEEE_R_ALIGN_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_R_MACERR */
#define MCF_FEC_IEEE_R_MACERR_Value(x)       (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_R_FDXFC */
#define MCF_FEC_IEEE_R_FDXFC_Value(x)        (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_FEC_IEEE_R_OCTETS_OK */
#define MCF_FEC_IEEE_R_OCTETS_OK_Value(x)    (((x)&0xFFFFFFFF)<<0)


#endif /* __MCF5475_FEC_H__ */
