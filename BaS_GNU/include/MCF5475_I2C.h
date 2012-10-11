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

#ifndef __MCF5475_I2C_H__
#define __MCF5475_I2C_H__


/*********************************************************************
*
* I2C Module (I2C)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_I2C_I2ADR                        (*(vuint8 *)(&__MBAR[0x8F00]))
#define MCF_I2C_I2FDR                        (*(vuint8 *)(&__MBAR[0x8F04]))
#define MCF_I2C_I2CR                         (*(vuint8 *)(&__MBAR[0x8F08]))
#define MCF_I2C_I2SR                         (*(vuint8 *)(&__MBAR[0x8F0C]))
#define MCF_I2C_I2DR                         (*(vuint8 *)(&__MBAR[0x8F10]))
#define MCF_I2C_I2ICR                        (*(vuint8 *)(&__MBAR[0x8F20]))



/* Bit definitions and macros for MCF_I2C_I2ADR */
#define MCF_I2C_I2ADR_ADR(x)                 (((x)&0x7F)<<0x1)

/* Bit definitions and macros for MCF_I2C_I2FDR */
#define MCF_I2C_I2FDR_IC(x)                  (((x)&0x3F)<<0)

/* Bit definitions and macros for MCF_I2C_I2CR */
#define MCF_I2C_I2CR_RSTA                    (0x4)
#define MCF_I2C_I2CR_TXAK                    (0x8)
#define MCF_I2C_I2CR_MTX                     (0x10)
#define MCF_I2C_I2CR_MSTA                    (0x20)
#define MCF_I2C_I2CR_IIEN                    (0x40)
#define MCF_I2C_I2CR_IEN                     (0x80)

/* Bit definitions and macros for MCF_I2C_I2SR */
#define MCF_I2C_I2SR_RXAK                    (0x1)
#define MCF_I2C_I2SR_IIF                     (0x2)
#define MCF_I2C_I2SR_SRW                     (0x4)
#define MCF_I2C_I2SR_IAL                     (0x10)
#define MCF_I2C_I2SR_IBB                     (0x20)
#define MCF_I2C_I2SR_IAAS                    (0x40)
#define MCF_I2C_I2SR_ICF                     (0x80)

/* Bit definitions and macros for MCF_I2C_I2DR */
#define MCF_I2C_I2DR_DATA(x)                 (((x)&0xFF)<<0)

/* Bit definitions and macros for MCF_I2C_I2ICR */
#define MCF_I2C_I2ICR_IE                     (0x1)
#define MCF_I2C_I2ICR_RE                     (0x2)
#define MCF_I2C_I2ICR_TE                     (0x4)
#define MCF_I2C_I2ICR_BNBE                   (0x8)


#endif /* __MCF5475_I2C_H__ */
