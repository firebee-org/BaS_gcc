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

#ifndef __MCF5475_USB_H__
#define __MCF5475_USB_H__


/*********************************************************************
*
* Universal Serial Bus Interface (USB)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_USB_USBAISR                      (*(vuint8 *)(&__MBAR[0xB000]))
#define MCF_USB_USBAIMR                      (*(vuint8 *)(&__MBAR[0xB001]))
#define MCF_USB_EPINFO                       (*(vuint8 *)(&__MBAR[0xB003]))
#define MCF_USB_CFGR                         (*(vuint8 *)(&__MBAR[0xB004]))
#define MCF_USB_CFGAR                        (*(vuint8 *)(&__MBAR[0xB005]))
#define MCF_USB_SPEEDR                       (*(vuint8 *)(&__MBAR[0xB006]))
#define MCF_USB_FRMNUMR                      (*(vuint16*)(&__MBAR[0xB00E]))
#define MCF_USB_EPTNR                        (*(vuint16*)(&__MBAR[0xB010]))
#define MCF_USB_IFUR                         (*(vuint16*)(&__MBAR[0xB014]))
#define MCF_USB_IFR0                         (*(vuint16*)(&__MBAR[0xB040]))
#define MCF_USB_IFR1                         (*(vuint16*)(&__MBAR[0xB042]))
#define MCF_USB_IFR2                         (*(vuint16*)(&__MBAR[0xB044]))
#define MCF_USB_IFR3                         (*(vuint16*)(&__MBAR[0xB046]))
#define MCF_USB_IFR4                         (*(vuint16*)(&__MBAR[0xB048]))
#define MCF_USB_IFR5                         (*(vuint16*)(&__MBAR[0xB04A]))
#define MCF_USB_IFR6                         (*(vuint16*)(&__MBAR[0xB04C]))
#define MCF_USB_IFR7                         (*(vuint16*)(&__MBAR[0xB04E]))
#define MCF_USB_IFR8                         (*(vuint16*)(&__MBAR[0xB050]))
#define MCF_USB_IFR9                         (*(vuint16*)(&__MBAR[0xB052]))
#define MCF_USB_IFR10                        (*(vuint16*)(&__MBAR[0xB054]))
#define MCF_USB_IFR11                        (*(vuint16*)(&__MBAR[0xB056]))
#define MCF_USB_IFR12                        (*(vuint16*)(&__MBAR[0xB058]))
#define MCF_USB_IFR13                        (*(vuint16*)(&__MBAR[0xB05A]))
#define MCF_USB_IFR14                        (*(vuint16*)(&__MBAR[0xB05C]))
#define MCF_USB_IFR15                        (*(vuint16*)(&__MBAR[0xB05E]))
#define MCF_USB_IFR16                        (*(vuint16*)(&__MBAR[0xB060]))
#define MCF_USB_IFR17                        (*(vuint16*)(&__MBAR[0xB062]))
#define MCF_USB_IFR18                        (*(vuint16*)(&__MBAR[0xB064]))
#define MCF_USB_IFR19                        (*(vuint16*)(&__MBAR[0xB066]))
#define MCF_USB_IFR20                        (*(vuint16*)(&__MBAR[0xB068]))
#define MCF_USB_IFR21                        (*(vuint16*)(&__MBAR[0xB06A]))
#define MCF_USB_IFR22                        (*(vuint16*)(&__MBAR[0xB06C]))
#define MCF_USB_IFR23                        (*(vuint16*)(&__MBAR[0xB06E]))
#define MCF_USB_IFR24                        (*(vuint16*)(&__MBAR[0xB070]))
#define MCF_USB_IFR25                        (*(vuint16*)(&__MBAR[0xB072]))
#define MCF_USB_IFR26                        (*(vuint16*)(&__MBAR[0xB074]))
#define MCF_USB_IFR27                        (*(vuint16*)(&__MBAR[0xB076]))
#define MCF_USB_IFR28                        (*(vuint16*)(&__MBAR[0xB078]))
#define MCF_USB_IFR29                        (*(vuint16*)(&__MBAR[0xB07A]))
#define MCF_USB_IFR30                        (*(vuint16*)(&__MBAR[0xB07C]))
#define MCF_USB_IFR31                        (*(vuint16*)(&__MBAR[0xB07E]))
#define MCF_USB_PPCNT                        (*(vuint16*)(&__MBAR[0xB080]))
#define MCF_USB_DPCNT                        (*(vuint16*)(&__MBAR[0xB082]))
#define MCF_USB_CRCECNT                      (*(vuint16*)(&__MBAR[0xB084]))
#define MCF_USB_BSECNT                       (*(vuint16*)(&__MBAR[0xB086]))
#define MCF_USB_PIDECNT                      (*(vuint16*)(&__MBAR[0xB088]))
#define MCF_USB_FRMECNT                      (*(vuint16*)(&__MBAR[0xB08A]))
#define MCF_USB_TXPCNT                       (*(vuint16*)(&__MBAR[0xB08C]))
#define MCF_USB_CNTOVR                       (*(vuint8 *)(&__MBAR[0xB08E]))
#define MCF_USB_EP0ACR                       (*(vuint8 *)(&__MBAR[0xB101]))
#define MCF_USB_EP0MPSR                      (*(vuint16*)(&__MBAR[0xB102]))
#define MCF_USB_EP0IFR                       (*(vuint8 *)(&__MBAR[0xB104]))
#define MCF_USB_EP0SR                        (*(vuint8 *)(&__MBAR[0xB105]))
#define MCF_USB_BMRTR                        (*(vuint8 *)(&__MBAR[0xB106]))
#define MCF_USB_BRTR                         (*(vuint8 *)(&__MBAR[0xB107]))
#define MCF_USB_WVALUER                      (*(vuint16*)(&__MBAR[0xB108]))
#define MCF_USB_WINDEXR                      (*(vuint16*)(&__MBAR[0xB10A]))
#define MCF_USB_WLENGTHR                     (*(vuint16*)(&__MBAR[0xB10C]))
#define MCF_USB_EP1OUTACR                    (*(vuint8 *)(&__MBAR[0xB131]))
#define MCF_USB_EP1OUTMPSR                   (*(vuint16*)(&__MBAR[0xB132]))
#define MCF_USB_EP1OUTIFR                    (*(vuint8 *)(&__MBAR[0xB134]))
#define MCF_USB_EP1OUTSR                     (*(vuint8 *)(&__MBAR[0xB135]))
#define MCF_USB_EP1OUTSFR                    (*(vuint16*)(&__MBAR[0xB13E]))
#define MCF_USB_EP1INACR                     (*(vuint8 *)(&__MBAR[0xB149]))
#define MCF_USB_EP1INMPSR                    (*(vuint16*)(&__MBAR[0xB14A]))
#define MCF_USB_EP1INIFR                     (*(vuint8 *)(&__MBAR[0xB14C]))
#define MCF_USB_EP1INSR                      (*(vuint8 *)(&__MBAR[0xB14D]))
#define MCF_USB_EP1INSFR                     (*(vuint16*)(&__MBAR[0xB156]))
#define MCF_USB_EP2OUTACR                    (*(vuint8 *)(&__MBAR[0xB161]))
#define MCF_USB_EP2OUTMPSR                   (*(vuint16*)(&__MBAR[0xB162]))
#define MCF_USB_EP2OUTIFR                    (*(vuint8 *)(&__MBAR[0xB164]))
#define MCF_USB_EP2OUTSR                     (*(vuint8 *)(&__MBAR[0xB165]))
#define MCF_USB_EP2OUTSFR                    (*(vuint16*)(&__MBAR[0xB16E]))
#define MCF_USB_EP2INACR                     (*(vuint8 *)(&__MBAR[0xB179]))
#define MCF_USB_EP2INMPSR                    (*(vuint16*)(&__MBAR[0xB17A]))
#define MCF_USB_EP2INIFR                     (*(vuint8 *)(&__MBAR[0xB17C]))
#define MCF_USB_EP2INSR                      (*(vuint8 *)(&__MBAR[0xB17D]))
#define MCF_USB_EP2INSFR                     (*(vuint16*)(&__MBAR[0xB186]))
#define MCF_USB_EP3OUTACR                    (*(vuint8 *)(&__MBAR[0xB191]))
#define MCF_USB_EP3OUTMPSR                   (*(vuint16*)(&__MBAR[0xB192]))
#define MCF_USB_EP3OUTIFR                    (*(vuint8 *)(&__MBAR[0xB194]))
#define MCF_USB_EP3OUTSR                     (*(vuint8 *)(&__MBAR[0xB195]))
#define MCF_USB_EP3OUTSFR                    (*(vuint16*)(&__MBAR[0xB19E]))
#define MCF_USB_EP3INACR                     (*(vuint8 *)(&__MBAR[0xB1A9]))
#define MCF_USB_EP3INMPSR                    (*(vuint16*)(&__MBAR[0xB1AA]))
#define MCF_USB_EP3INIFR                     (*(vuint8 *)(&__MBAR[0xB1AC]))
#define MCF_USB_EP3INSR                      (*(vuint8 *)(&__MBAR[0xB1AD]))
#define MCF_USB_EP3INSFR                     (*(vuint16*)(&__MBAR[0xB1B6]))
#define MCF_USB_EP4OUTACR                    (*(vuint8 *)(&__MBAR[0xB1C1]))
#define MCF_USB_EP4OUTMPSR                   (*(vuint16*)(&__MBAR[0xB1C2]))
#define MCF_USB_EP4OUTIFR                    (*(vuint8 *)(&__MBAR[0xB1C4]))
#define MCF_USB_EP4OUTSR                     (*(vuint8 *)(&__MBAR[0xB1C5]))
#define MCF_USB_EP4OUTSFR                    (*(vuint16*)(&__MBAR[0xB1CE]))
#define MCF_USB_EP4INACR                     (*(vuint8 *)(&__MBAR[0xB1D9]))
#define MCF_USB_EP4INMPSR                    (*(vuint16*)(&__MBAR[0xB1DA]))
#define MCF_USB_EP4INIFR                     (*(vuint8 *)(&__MBAR[0xB1DC]))
#define MCF_USB_EP4INSR                      (*(vuint8 *)(&__MBAR[0xB1DD]))
#define MCF_USB_EP4INSFR                     (*(vuint16*)(&__MBAR[0xB1E6]))
#define MCF_USB_EP5OUTACR                    (*(vuint8 *)(&__MBAR[0xB1F1]))
#define MCF_USB_EP5OUTMPSR                   (*(vuint16*)(&__MBAR[0xB1F2]))
#define MCF_USB_EP5OUTIFR                    (*(vuint8 *)(&__MBAR[0xB1F4]))
#define MCF_USB_EP5OUTSR                     (*(vuint8 *)(&__MBAR[0xB1F5]))
#define MCF_USB_EP5OUTSFR                    (*(vuint16*)(&__MBAR[0xB1FE]))
#define MCF_USB_EP5INACR                     (*(vuint8 *)(&__MBAR[0xB209]))
#define MCF_USB_EP5INMPSR                    (*(vuint16*)(&__MBAR[0xB20A]))
#define MCF_USB_EP5INIFR                     (*(vuint8 *)(&__MBAR[0xB20C]))
#define MCF_USB_EP5INSR                      (*(vuint8 *)(&__MBAR[0xB20D]))
#define MCF_USB_EP5INSFR                     (*(vuint16*)(&__MBAR[0xB216]))
#define MCF_USB_EP6OUTACR                    (*(vuint8 *)(&__MBAR[0xB221]))
#define MCF_USB_EP6OUTMPSR                   (*(vuint16*)(&__MBAR[0xB222]))
#define MCF_USB_EP6OUTIFR                    (*(vuint8 *)(&__MBAR[0xB224]))
#define MCF_USB_EP6OUTSR                     (*(vuint8 *)(&__MBAR[0xB225]))
#define MCF_USB_EP6OUTSFR                    (*(vuint16*)(&__MBAR[0xB22E]))
#define MCF_USB_EP6INACR                     (*(vuint8 *)(&__MBAR[0xB239]))
#define MCF_USB_EP6INMPSR                    (*(vuint16*)(&__MBAR[0xB23A]))
#define MCF_USB_EP6INIFR                     (*(vuint8 *)(&__MBAR[0xB23C]))
#define MCF_USB_EP6INSR                      (*(vuint8 *)(&__MBAR[0xB23D]))
#define MCF_USB_EP6INSFR                     (*(vuint16*)(&__MBAR[0xB246]))
#define MCF_USB_USBSR                        (*(vuint32*)(&__MBAR[0xB400]))
#define MCF_USB_USBCR                        (*(vuint32*)(&__MBAR[0xB404]))
#define MCF_USB_DRAMCR                       (*(vuint32*)(&__MBAR[0xB408]))
#define MCF_USB_DRAMDR                       (*(vuint32*)(&__MBAR[0xB40C]))
#define MCF_USB_USBISR                       (*(vuint32*)(&__MBAR[0xB410]))
#define MCF_USB_USBIMR                       (*(vuint32*)(&__MBAR[0xB414]))
#define MCF_USB_EP0STAT                      (*(vuint32*)(&__MBAR[0xB440]))
#define MCF_USB_EP0ISR                       (*(vuint32*)(&__MBAR[0xB444]))
#define MCF_USB_EP0IMR                       (*(vuint32*)(&__MBAR[0xB448]))
#define MCF_USB_EP0FRCFGR                    (*(vuint32*)(&__MBAR[0xB44C]))
#define MCF_USB_EP0FDR                       (*(vuint32*)(&__MBAR[0xB450]))
#define MCF_USB_EP0FSR                       (*(vuint32*)(&__MBAR[0xB454]))
#define MCF_USB_EP0FCR                       (*(vuint32*)(&__MBAR[0xB458]))
#define MCF_USB_EP0FAR                       (*(vuint32*)(&__MBAR[0xB45C]))
#define MCF_USB_EP0FRP                       (*(vuint32*)(&__MBAR[0xB460]))
#define MCF_USB_EP0FWP                       (*(vuint32*)(&__MBAR[0xB464]))
#define MCF_USB_EP0LRFP                      (*(vuint32*)(&__MBAR[0xB468]))
#define MCF_USB_EP0LWFP                      (*(vuint32*)(&__MBAR[0xB46C]))
#define MCF_USB_EP1STAT                      (*(vuint32*)(&__MBAR[0xB470]))
#define MCF_USB_EP1ISR                       (*(vuint32*)(&__MBAR[0xB474]))
#define MCF_USB_EP1IMR                       (*(vuint32*)(&__MBAR[0xB478]))
#define MCF_USB_EP1FRCFGR                    (*(vuint32*)(&__MBAR[0xB47C]))
#define MCF_USB_EP1FDR                       (*(vuint32*)(&__MBAR[0xB480]))
#define MCF_USB_EP1FSR                       (*(vuint32*)(&__MBAR[0xB484]))
#define MCF_USB_EP1FCR                       (*(vuint32*)(&__MBAR[0xB488]))
#define MCF_USB_EP1FAR                       (*(vuint32*)(&__MBAR[0xB48C]))
#define MCF_USB_EP1FRP                       (*(vuint32*)(&__MBAR[0xB490]))
#define MCF_USB_EP1FWP                       (*(vuint32*)(&__MBAR[0xB494]))
#define MCF_USB_EP1LRFP                      (*(vuint32*)(&__MBAR[0xB498]))
#define MCF_USB_EP1LWFP                      (*(vuint32*)(&__MBAR[0xB49C]))
#define MCF_USB_EP2STAT                      (*(vuint32*)(&__MBAR[0xB4A0]))
#define MCF_USB_EP2ISR                       (*(vuint32*)(&__MBAR[0xB4A4]))
#define MCF_USB_EP2IMR                       (*(vuint32*)(&__MBAR[0xB4A8]))
#define MCF_USB_EP2FRCFGR                    (*(vuint32*)(&__MBAR[0xB4AC]))
#define MCF_USB_EP2FDR                       (*(vuint32*)(&__MBAR[0xB4B0]))
#define MCF_USB_EP2FSR                       (*(vuint32*)(&__MBAR[0xB4B4]))
#define MCF_USB_EP2FCR                       (*(vuint32*)(&__MBAR[0xB4B8]))
#define MCF_USB_EP2FAR                       (*(vuint32*)(&__MBAR[0xB4BC]))
#define MCF_USB_EP2FRP                       (*(vuint32*)(&__MBAR[0xB4C0]))
#define MCF_USB_EP2FWP                       (*(vuint32*)(&__MBAR[0xB4C4]))
#define MCF_USB_EP2LRFP                      (*(vuint32*)(&__MBAR[0xB4C8]))
#define MCF_USB_EP2LWFP                      (*(vuint32*)(&__MBAR[0xB4CC]))
#define MCF_USB_EP3STAT                      (*(vuint32*)(&__MBAR[0xB4D0]))
#define MCF_USB_EP3ISR                       (*(vuint32*)(&__MBAR[0xB4D4]))
#define MCF_USB_EP3IMR                       (*(vuint32*)(&__MBAR[0xB4D8]))
#define MCF_USB_EP3FRCFGR                    (*(vuint32*)(&__MBAR[0xB4DC]))
#define MCF_USB_EP3FDR                       (*(vuint32*)(&__MBAR[0xB4E0]))
#define MCF_USB_EP3FSR                       (*(vuint32*)(&__MBAR[0xB4E4]))
#define MCF_USB_EP3FCR                       (*(vuint32*)(&__MBAR[0xB4E8]))
#define MCF_USB_EP3FAR                       (*(vuint32*)(&__MBAR[0xB4EC]))
#define MCF_USB_EP3FRP                       (*(vuint32*)(&__MBAR[0xB4F0]))
#define MCF_USB_EP3FWP                       (*(vuint32*)(&__MBAR[0xB4F4]))
#define MCF_USB_EP3LRFP                      (*(vuint32*)(&__MBAR[0xB4F8]))
#define MCF_USB_EP3LWFP                      (*(vuint32*)(&__MBAR[0xB4FC]))
#define MCF_USB_EP4STAT                      (*(vuint32*)(&__MBAR[0xB500]))
#define MCF_USB_EP4ISR                       (*(vuint32*)(&__MBAR[0xB504]))
#define MCF_USB_EP4IMR                       (*(vuint32*)(&__MBAR[0xB508]))
#define MCF_USB_EP4FRCFGR                    (*(vuint32*)(&__MBAR[0xB50C]))
#define MCF_USB_EP4FDR                       (*(vuint32*)(&__MBAR[0xB510]))
#define MCF_USB_EP4FSR                       (*(vuint32*)(&__MBAR[0xB514]))
#define MCF_USB_EP4FCR                       (*(vuint32*)(&__MBAR[0xB518]))
#define MCF_USB_EP4FAR                       (*(vuint32*)(&__MBAR[0xB51C]))
#define MCF_USB_EP4FRP                       (*(vuint32*)(&__MBAR[0xB520]))
#define MCF_USB_EP4FWP                       (*(vuint32*)(&__MBAR[0xB524]))
#define MCF_USB_EP4LRFP                      (*(vuint32*)(&__MBAR[0xB528]))
#define MCF_USB_EP4LWFP                      (*(vuint32*)(&__MBAR[0xB52C]))
#define MCF_USB_EP5STAT                      (*(vuint32*)(&__MBAR[0xB530]))
#define MCF_USB_EP5ISR                       (*(vuint32*)(&__MBAR[0xB534]))
#define MCF_USB_EP5IMR                       (*(vuint32*)(&__MBAR[0xB538]))
#define MCF_USB_EP5FRCFGR                    (*(vuint32*)(&__MBAR[0xB53C]))
#define MCF_USB_EP5FDR                       (*(vuint32*)(&__MBAR[0xB540]))
#define MCF_USB_EP5FSR                       (*(vuint32*)(&__MBAR[0xB544]))
#define MCF_USB_EP5FCR                       (*(vuint32*)(&__MBAR[0xB548]))
#define MCF_USB_EP5FAR                       (*(vuint32*)(&__MBAR[0xB54C]))
#define MCF_USB_EP5FRP                       (*(vuint32*)(&__MBAR[0xB550]))
#define MCF_USB_EP5FWP                       (*(vuint32*)(&__MBAR[0xB554]))
#define MCF_USB_EP5LRFP                      (*(vuint32*)(&__MBAR[0xB558]))
#define MCF_USB_EP5LWFP                      (*(vuint32*)(&__MBAR[0xB55C]))
#define MCF_USB_EP6STAT                      (*(vuint32*)(&__MBAR[0xB560]))
#define MCF_USB_EP6ISR                       (*(vuint32*)(&__MBAR[0xB564]))
#define MCF_USB_EP6IMR                       (*(vuint32*)(&__MBAR[0xB568]))
#define MCF_USB_EP6FRCFGR                    (*(vuint32*)(&__MBAR[0xB56C]))
#define MCF_USB_EP6FDR                       (*(vuint32*)(&__MBAR[0xB570]))
#define MCF_USB_EP6FSR                       (*(vuint32*)(&__MBAR[0xB574]))
#define MCF_USB_EP6FCR                       (*(vuint32*)(&__MBAR[0xB578]))
#define MCF_USB_EP6FAR                       (*(vuint32*)(&__MBAR[0xB57C]))
#define MCF_USB_EP6FRP                       (*(vuint32*)(&__MBAR[0xB580]))
#define MCF_USB_EP6FWP                       (*(vuint32*)(&__MBAR[0xB584]))
#define MCF_USB_EP6LRFP                      (*(vuint32*)(&__MBAR[0xB588]))
#define MCF_USB_EP6LWFP                      (*(vuint32*)(&__MBAR[0xB58C]))
#define MCF_USB_IFR(x)                       (*(vuint16*)(&__MBAR[0xB040 + ((x)*0x2)]))
#define MCF_USB_EPOUTACR(x)                  (*(vuint8 *)(&__MBAR[0xB131 + ((x-1)*0x30)]))
#define MCF_USB_EPOUTMPSR(x)                 (*(vuint16*)(&__MBAR[0xB132 + ((x-1)*0x30)]))
#define MCF_USB_EPOUTIFR(x)                  (*(vuint8 *)(&__MBAR[0xB134 + ((x-1)*0x30)]))
#define MCF_USB_EPOUTSR(x)                   (*(vuint8 *)(&__MBAR[0xB135 + ((x-1)*0x30)]))
#define MCF_USB_EPOUTSFR(x)                  (*(vuint16*)(&__MBAR[0xB13E + ((x-1)*0x30)]))
#define MCF_USB_EPINACR(x)                   (*(vuint8 *)(&__MBAR[0xB149 + ((x-1)*0x30)]))
#define MCF_USB_EPINMPSR(x)                  (*(vuint16*)(&__MBAR[0xB14A + ((x-1)*0x30)]))
#define MCF_USB_EPINIFR(x)                   (*(vuint8 *)(&__MBAR[0xB14C + ((x-1)*0x30)]))
#define MCF_USB_EPINSR(x)                    (*(vuint8 *)(&__MBAR[0xB14D + ((x-1)*0x30)]))
#define MCF_USB_EPINSFR(x)                   (*(vuint16*)(&__MBAR[0xB156 + ((x-1)*0x30)]))
#define MCF_USB_EPSTAT(x)                    (*(vuint32*)(&__MBAR[0xB440 + ((x)*0x30)]))
#define MCF_USB_EPISR(x)                     (*(vuint32*)(&__MBAR[0xB444 + ((x)*0x30)]))
#define MCF_USB_EPIMR(x)                     (*(vuint32*)(&__MBAR[0xB448 + ((x)*0x30)]))
#define MCF_USB_EPFRCFGR(x)                  (*(vuint32*)(&__MBAR[0xB44C + ((x)*0x30)]))
#define MCF_USB_EPFDR(x)                     (*(vuint32*)(&__MBAR[0xB450 + ((x)*0x30)]))
#define MCF_USB_EPFSR(x)                     (*(vuint32*)(&__MBAR[0xB454 + ((x)*0x30)]))
#define MCF_USB_EPFCR(x)                     (*(vuint32*)(&__MBAR[0xB458 + ((x)*0x30)]))
#define MCF_USB_EPFAR(x)                     (*(vuint32*)(&__MBAR[0xB45C + ((x)*0x30)]))
#define MCF_USB_EPFRP(x)                     (*(vuint32*)(&__MBAR[0xB460 + ((x)*0x30)]))
#define MCF_USB_EPFWP(x)                     (*(vuint32*)(&__MBAR[0xB464 + ((x)*0x30)]))
#define MCF_USB_EPLRFP(x)                    (*(vuint32*)(&__MBAR[0xB468 + ((x)*0x30)]))
#define MCF_USB_EPLWFP(x)                    (*(vuint32*)(&__MBAR[0xB46C + ((x)*0x30)]))


/* Bit definitions and macros for MCF_USB_USBAISR */
#define MCF_USB_USBAISR_SETUP                (0x1)
#define MCF_USB_USBAISR_IN                   (0x2)
#define MCF_USB_USBAISR_OUT                  (0x4)
#define MCF_USB_USBAISR_EPHALT               (0x8)
#define MCF_USB_USBAISR_TRANSERR             (0x10)
#define MCF_USB_USBAISR_ACK                  (0x20)
#define MCF_USB_USBAISR_CTROVFL              (0x40)
#define MCF_USB_USBAISR_EPSTALL              (0x80)

/* Bit definitions and macros for MCF_USB_USBAIMR */
#define MCF_USB_USBAIMR_SETUPEN              (0x1)
#define MCF_USB_USBAIMR_INEN                 (0x2)
#define MCF_USB_USBAIMR_OUTEN                (0x4)
#define MCF_USB_USBAIMR_EPHALTEN             (0x8)
#define MCF_USB_USBAIMR_TRANSERREN           (0x10)
#define MCF_USB_USBAIMR_ACKEN                (0x20)
#define MCF_USB_USBAIMR_CTROVFLEN            (0x40)
#define MCF_USB_USBAIMR_EPSTALLEN            (0x80)

/* Bit definitions and macros for MCF_USB_EPINFO */
#define MCF_USB_EPINFO_EPDIR                 (0x1)
#define MCF_USB_EPINFO_EPNUM(x)              (((x)&0x7)<<0x1)

/* Bit definitions and macros for MCF_USB_CFGR */
#define MCF_USB_CFGR_Configuration_Value(x)  (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_USB_CFGAR */
#define MCF_USB_CFGAR_RESERVED               (0xA0)
#define MCF_USB_CFGAR_RMTWKEUP               (0xE0)

/* Bit definitions and macros for MCF_USB_SPEEDR */
#define MCF_USB_SPEEDR_SPEED(x)              (((x)&0x3)<<0)

/* Bit definitions and macros for MCF_USB_FRMNUMR */
#define MCF_USB_FRMNUMR_FRMNUM(x)            (((x)&0xFFF)<<0)

/* Bit definitions and macros for MCF_USB_EPTNR */
#define MCF_USB_EPTNR_EP1T(x)                (((x)&0x3)<<0)
#define MCF_USB_EPTNR_EP2T(x)                (((x)&0x3)<<0x2)
#define MCF_USB_EPTNR_EP3T(x)                (((x)&0x3)<<0x4)
#define MCF_USB_EPTNR_EP4T(x)                (((x)&0x3)<<0x6)
#define MCF_USB_EPTNR_EP5T(x)                (((x)&0x3)<<0x8)
#define MCF_USB_EPTNR_EP6T(x)                (((x)&0x3)<<0xA)
#define MCF_USB_EPTNR_EPnT1                  (0)
#define MCF_USB_EPTNR_EPnT2                  (0x1)
#define MCF_USB_EPTNR_EPnT3                  (0x2)

/* Bit definitions and macros for MCF_USB_IFUR */
#define MCF_USB_IFUR_ALTSET(x)               (((x)&0xFF)<<0)
#define MCF_USB_IFUR_IFNUM(x)                (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_USB_IFR */
#define MCF_USB_IFR_ALTSET(x)                (((x)&0xFF)<<0)
#define MCF_USB_IFR_IFNUM(x)                 (((x)&0xFF)<<0x8)

/* Bit definitions and macros for MCF_USB_PPCNT */
#define MCF_USB_PPCNT_PPCNT(x)               (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_DPCNT */
#define MCF_USB_DPCNT_DPCNT(x)               (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_CRCECNT */
#define MCF_USB_CRCECNT_CRCECNT(x)           (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_BSECNT */
#define MCF_USB_BSECNT_BSECNT(x)             (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_PIDECNT */
#define MCF_USB_PIDECNT_PIDECNT(x)           (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_FRMECNT */
#define MCF_USB_FRMECNT_FRMECNT(x)           (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_TXPCNT */
#define MCF_USB_TXPCNT_TXPCNT(x)             (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_CNTOVR */
#define MCF_USB_CNTOVR_PPCNT                 (0x1)
#define MCF_USB_CNTOVR_DPCNT                 (0x2)
#define MCF_USB_CNTOVR_CRCECNT               (0x4)
#define MCF_USB_CNTOVR_BSECNT                (0x8)
#define MCF_USB_CNTOVR_PIDECNT               (0x10)
#define MCF_USB_CNTOVR_FRMECNT               (0x20)
#define MCF_USB_CNTOVR_TXPCNT                (0x40)

/* Bit definitions and macros for MCF_USB_EP0ACR */
#define MCF_USB_EP0ACR_TTYPE(x)              (((x)&0x3)<<0)
#define MCF_USB_EP0ACR_TTYPE_CTRL            (0)
#define MCF_USB_EP0ACR_TTYPE_ISOC            (0x1)
#define MCF_USB_EP0ACR_TTYPE_BULK            (0x2)
#define MCF_USB_EP0ACR_TTYPE_INT             (0x3)

/* Bit definitions and macros for MCF_USB_EP0MPSR */
#define MCF_USB_EP0MPSR_MAXPKTSZ(x)          (((x)&0x7FF)<<0)
#define MCF_USB_EP0MPSR_ADDTRANS(x)          (((x)&0x3)<<0xB)

/* Bit definitions and macros for MCF_USB_EP0IFR */
#define MCF_USB_EP0IFR_IFNUM(x)              (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_USB_EP0SR */
#define MCF_USB_EP0SR_HALT                   (0x1)
#define MCF_USB_EP0SR_ACTIVE                 (0x2)
#define MCF_USB_EP0SR_PSTALL                 (0x4)
#define MCF_USB_EP0SR_CCOMP                  (0x8)
#define MCF_USB_EP0SR_TXZERO                 (0x20)
#define MCF_USB_EP0SR_INT                    (0x80)

/* Bit definitions and macros for MCF_USB_BMRTR */
#define MCF_USB_BMRTR_REC(x)                 (((x)&0x1F)<<0)
#define MCF_USB_BMRTR_REC_DEVICE             (0)
#define MCF_USB_BMRTR_REC_INTERFACE          (0x1)
#define MCF_USB_BMRTR_REC_ENDPOINT           (0x2)
#define MCF_USB_BMRTR_REC_OTHER              (0x3)
#define MCF_USB_BMRTR_TYPE(x)                (((x)&0x3)<<0x5)
#define MCF_USB_BMRTR_TYPE_STANDARD          (0)
#define MCF_USB_BMRTR_TYPE_CLASS             (0x20)
#define MCF_USB_BMRTR_TYPE_VENDOR            (0x40)
#define MCF_USB_BMRTR_DIR                    (0x80)

/* Bit definitions and macros for MCF_USB_BRTR */
#define MCF_USB_BRTR_BREQ(x)                 (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_USB_WVALUER */
#define MCF_USB_WVALUER_WVALUE(x)            (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_WINDEXR */
#define MCF_USB_WINDEXR_WINDEX(x)            (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_WLENGTHR */
#define MCF_USB_WLENGTHR_WLENGTH(x)          (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_USB_EPOUTACR */
#define MCF_USB_EPOUTACR_TTYPE(x)            (((x)&0x3)<<0)
#define MCF_USB_EPOUTACR_TTYPE_ISOC          (0x1)
#define MCF_USB_EPOUTACR_TTYPE_BULK          (0x2)
#define MCF_USB_EPOUTACR_TTYPE_INT           (0x3)

/* Bit definitions and macros for MCF_USB_EPOUTMPSR */
#define MCF_USB_EPOUTMPSR_MAXPKTSZ(x)        (((x)&0x7FF)<<0)
#define MCF_USB_EPOUTMPSR_ADDTRANS(x)        (((x)&0x3)<<0xB)

/* Bit definitions and macros for MCF_USB_EPOUTIFR */
#define MCF_USB_EPOUTIFR_IFNUM(x)            (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_USB_EPOUTSR */
#define MCF_USB_EPOUTSR_HALT                 (0x1)
#define MCF_USB_EPOUTSR_ACTIVE               (0x2)
#define MCF_USB_EPOUTSR_PSTALL               (0x4)
#define MCF_USB_EPOUTSR_CCOMP                (0x8)
#define MCF_USB_EPOUTSR_TXZERO               (0x20)
#define MCF_USB_EPOUTSR_INT                  (0x80)

/* Bit definitions and macros for MCF_USB_EPOUTSFR */
#define MCF_USB_EPOUTSFR_FRMNUM(x)           (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_USB_EPINACR */
#define MCF_USB_EPINACR_TTYPE(x)             (((x)&0x3)<<0)
#define MCF_USB_EPINACR_TTYPE_ISOC           (0x1)
#define MCF_USB_EPINACR_TTYPE_BULK           (0x2)
#define MCF_USB_EPINACR_TTYPE_INT            (0x3)

/* Bit definitions and macros for MCF_USB_EPINMPSR */
#define MCF_USB_EPINMPSR_MAXPKTSZ(x)         (((x)&0x7FF)<<0)
#define MCF_USB_EPINMPSR_ADDTRANS(x)         (((x)&0x3)<<0xB)

/* Bit definitions and macros for MCF_USB_EPINIFR */
#define MCF_USB_EPINIFR_IFNUM(x)             (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_USB_EPINSR */
#define MCF_USB_EPINSR_HALT                  (0x1)
#define MCF_USB_EPINSR_ACTIVE                (0x2)
#define MCF_USB_EPINSR_PSTALL                (0x4)
#define MCF_USB_EPINSR_CCOMP                 (0x8)
#define MCF_USB_EPINSR_TXZERO                (0x20)
#define MCF_USB_EPINSR_INT                   (0x80)

/* Bit definitions and macros for MCF_USB_EPINSFR */
#define MCF_USB_EPINSFR_FRMNUM(x)            (((x)&0x7FF)<<0)

/* Bit definitions and macros for MCF_USB_USBSR */
#define MCF_USB_USBSR_ISOERREP(x)            (((x)&0xF)<<0)
#define MCF_USB_USBSR_SUSP                   (0x80)

/* Bit definitions and macros for MCF_USB_USBCR */
#define MCF_USB_USBCR_RESUME                 (0x1)
#define MCF_USB_USBCR_APPLOCK                (0x2)
#define MCF_USB_USBCR_RST                    (0x4)
#define MCF_USB_USBCR_RAMEN                  (0x8)
#define MCF_USB_USBCR_RAMSPLIT               (0x20)

/* Bit definitions and macros for MCF_USB_DRAMCR */
#define MCF_USB_DRAMCR_DADR(x)               (((x)&0x3FF)<<0)
#define MCF_USB_DRAMCR_DSIZE(x)              (((x)&0x7FF)<<0x10)
#define MCF_USB_DRAMCR_BSY                   (0x40000000)
#define MCF_USB_DRAMCR_START                 (0x80000000)

/* Bit definitions and macros for MCF_USB_DRAMDR */
#define MCF_USB_DRAMDR_DDAT(x)               (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_USB_USBISR */
#define MCF_USB_USBISR_ISOERR                (0x1)
#define MCF_USB_USBISR_FTUNLCK               (0x2)
#define MCF_USB_USBISR_SUSP                  (0x4)
#define MCF_USB_USBISR_RES                   (0x8)
#define MCF_USB_USBISR_UPDSOF                (0x10)
#define MCF_USB_USBISR_RSTSTOP               (0x20)
#define MCF_USB_USBISR_SOF                   (0x40)
#define MCF_USB_USBISR_MSOF                  (0x80)

/* Bit definitions and macros for MCF_USB_USBIMR */
#define MCF_USB_USBIMR_ISOERR                (0x1)
#define MCF_USB_USBIMR_FTUNLCK               (0x2)
#define MCF_USB_USBIMR_SUSP                  (0x4)
#define MCF_USB_USBIMR_RES                   (0x8)
#define MCF_USB_USBIMR_UPDSOF                (0x10)
#define MCF_USB_USBIMR_RSTSTOP               (0x20)
#define MCF_USB_USBIMR_SOF                   (0x40)
#define MCF_USB_USBIMR_MSOF                  (0x80)

/* Bit definitions and macros for MCF_USB_EPSTAT */
#define MCF_USB_EPSTAT_RST                   (0x1)
#define MCF_USB_EPSTAT_FLUSH                 (0x2)
#define MCF_USB_EPSTAT_DIR                   (0x80)
#define MCF_USB_EPSTAT_BYTECNT(x)            (((x)&0xFFF)<<0x10)

/* Bit definitions and macros for MCF_USB_EPISR */
#define MCF_USB_EPISR_EOF                    (0x1)
#define MCF_USB_EPISR_EOT                    (0x4)
#define MCF_USB_EPISR_FIFOLO                 (0x10)
#define MCF_USB_EPISR_FIFOHI                 (0x20)
#define MCF_USB_EPISR_ERR                    (0x40)
#define MCF_USB_EPISR_EMT                    (0x80)
#define MCF_USB_EPISR_FU                     (0x100)

/* Bit definitions and macros for MCF_USB_EPIMR */
#define MCF_USB_EPIMR_EOF                    (0x1)
#define MCF_USB_EPIMR_EOT                    (0x4)
#define MCF_USB_EPIMR_FIFOLO                 (0x10)
#define MCF_USB_EPIMR_FIFOHI                 (0x20)
#define MCF_USB_EPIMR_ERR                    (0x40)
#define MCF_USB_EPIMR_EMT                    (0x80)
#define MCF_USB_EPIMR_FU                     (0x100)

/* Bit definitions and macros for MCF_USB_EPFRCFGR */
#define MCF_USB_EPFRCFGR_DEPTH(x)            (((x)&0x1FFF)<<0)
#define MCF_USB_EPFRCFGR_BASE(x)             (((x)&0xFFF)<<0x10)

/* Bit definitions and macros for MCF_USB_EPFDR */
#define MCF_USB_EPFDR_RX_TXDATA(x)           (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_USB_EPFSR */
#define MCF_USB_EPFSR_EMT                    (0x10000)
#define MCF_USB_EPFSR_ALRM                   (0x20000)
#define MCF_USB_EPFSR_FU                     (0x40000)
#define MCF_USB_EPFSR_FR                     (0x80000)
#define MCF_USB_EPFSR_OF                     (0x100000)
#define MCF_USB_EPFSR_UF                     (0x200000)
#define MCF_USB_EPFSR_RXW                    (0x400000)
#define MCF_USB_EPFSR_FAE                    (0x800000)
#define MCF_USB_EPFSR_FRM(x)                 (((x)&0xF)<<0x18)
#define MCF_USB_EPFSR_TXW                    (0x40000000)
#define MCF_USB_EPFSR_IP                     (0x80000000)

/* Bit definitions and macros for MCF_USB_EPFCR */
#define MCF_USB_EPFCR_COUNTER(x)             (((x)&0xFFFF)<<0)
#define MCF_USB_EPFCR_TXWMSK                 (0x40000)
#define MCF_USB_EPFCR_OFMSK                  (0x80000)
#define MCF_USB_EPFCR_UFMSK                  (0x100000)
#define MCF_USB_EPFCR_RXWMSK                 (0x200000)
#define MCF_USB_EPFCR_FAEMSK                 (0x400000)
#define MCF_USB_EPFCR_IPMSK                  (0x800000)
#define MCF_USB_EPFCR_GR(x)                  (((x)&0x7)<<0x18)
#define MCF_USB_EPFCR_FRM                    (0x8000000)
#define MCF_USB_EPFCR_TMR                    (0x10000000)
#define MCF_USB_EPFCR_WFR                    (0x20000000)
#define MCF_USB_EPFCR_SHAD                   (0x80000000)

/* Bit definitions and macros for MCF_USB_EPFAR */
#define MCF_USB_EPFAR_ALRMP(x)               (((x)&0xFFF)<<0)

/* Bit definitions and macros for MCF_USB_EPFRP */
#define MCF_USB_EPFRP_RP(x)                  (((x)&0xFFF)<<0)

/* Bit definitions and macros for MCF_USB_EPFWP */
#define MCF_USB_EPFWP_WP(x)                  (((x)&0xFFF)<<0)

/* Bit definitions and macros for MCF_USB_EPLRFP */
#define MCF_USB_EPLRFP_LRFP(x)               (((x)&0xFFF)<<0)

/* Bit definitions and macros for MCF_USB_EPLWFP */
#define MCF_USB_EPLWFP_LWFP(x)               (((x)&0xFFF)<<0)


#endif /* __MCF5475_USB_H__ */
