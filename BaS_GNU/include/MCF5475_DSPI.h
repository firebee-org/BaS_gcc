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

#ifndef __MCF5475_DSPI_H__
#define __MCF5475_DSPI_H__


/*********************************************************************
*
* DMA Serial Peripheral Interface (DSPI)
*
*********************************************************************/

/* Register read/write macros */
#define MCF_DSPI_DMCR                        (*(vuint32*)(&__MBAR[0x8A00]))
#define MCF_DSPI_DTCR                        (*(vuint32*)(&__MBAR[0x8A08]))
#define MCF_DSPI_DCTAR0                      (*(vuint32*)(&__MBAR[0x8A0C]))
#define MCF_DSPI_DCTAR1                      (*(vuint32*)(&__MBAR[0x8A10]))
#define MCF_DSPI_DCTAR2                      (*(vuint32*)(&__MBAR[0x8A14]))
#define MCF_DSPI_DCTAR3                      (*(vuint32*)(&__MBAR[0x8A18]))
#define MCF_DSPI_DCTAR4                      (*(vuint32*)(&__MBAR[0x8A1C]))
#define MCF_DSPI_DCTAR5                      (*(vuint32*)(&__MBAR[0x8A20]))
#define MCF_DSPI_DCTAR6                      (*(vuint32*)(&__MBAR[0x8A24]))
#define MCF_DSPI_DCTAR7                      (*(vuint32*)(&__MBAR[0x8A28]))
#define MCF_DSPI_DSR                         (*(vuint32*)(&__MBAR[0x8A2C]))
#define MCF_DSPI_DIRSR                       (*(vuint32*)(&__MBAR[0x8A30]))
#define MCF_DSPI_DTFR                        (*(vuint32*)(&__MBAR[0x8A34]))
#define MCF_DSPI_DRFR                        (*(vuint32*)(&__MBAR[0x8A38]))
#define MCF_DSPI_DTFDR0                      (*(vuint32*)(&__MBAR[0x8A3C]))
#define MCF_DSPI_DTFDR1                      (*(vuint32*)(&__MBAR[0x8A40]))
#define MCF_DSPI_DTFDR2                      (*(vuint32*)(&__MBAR[0x8A44]))
#define MCF_DSPI_DTFDR3                      (*(vuint32*)(&__MBAR[0x8A48]))
#define MCF_DSPI_DRFDR0                      (*(vuint32*)(&__MBAR[0x8A7C]))
#define MCF_DSPI_DRFDR1                      (*(vuint32*)(&__MBAR[0x8A80]))
#define MCF_DSPI_DRFDR2                      (*(vuint32*)(&__MBAR[0x8A84]))
#define MCF_DSPI_DRFDR3                      (*(vuint32*)(&__MBAR[0x8A88]))
#define MCF_DSPI_DCTAR(x)                    (*(vuint32*)(&__MBAR[0x8A0C + ((x)*0x4)]))
#define MCF_DSPI_DTFDR(x)                    (*(vuint32*)(&__MBAR[0x8A3C + ((x)*0x4)]))
#define MCF_DSPI_DRFDR(x)                    (*(vuint32*)(&__MBAR[0x8A7C + ((x)*0x4)]))


/* Bit definitions and macros for MCF_DSPI_DMCR */
#define MCF_DSPI_DMCR_HALT                   (0x1)
#define MCF_DSPI_DMCR_SMPL_PT(x)             (((x)&0x3)<<0x8)
#define MCF_DSPI_DMCR_SMPL_PT_0CLK           (0)
#define MCF_DSPI_DMCR_SMPL_PT_1CLK           (0x100)
#define MCF_DSPI_DMCR_SMPL_PT_2CLK           (0x200)
#define MCF_DSPI_DMCR_CRXF                   (0x400)
#define MCF_DSPI_DMCR_CTXF                   (0x800)
#define MCF_DSPI_DMCR_DRXF                   (0x1000)
#define MCF_DSPI_DMCR_DTXF                   (0x2000)
#define MCF_DSPI_DMCR_CSIS0                  (0x10000)
#define MCF_DSPI_DMCR_CSIS2                  (0x40000)
#define MCF_DSPI_DMCR_CSIS3                  (0x80000)
#define MCF_DSPI_DMCR_CSIS5                  (0x200000)
#define MCF_DSPI_DMCR_ROOE                   (0x1000000)
#define MCF_DSPI_DMCR_PCSSE                  (0x2000000)
#define MCF_DSPI_DMCR_MTFE                   (0x4000000)
#define MCF_DSPI_DMCR_FRZ                    (0x8000000)
#define MCF_DSPI_DMCR_DCONF(x)               (((x)&0x3)<<0x1C)
#define MCF_DSPI_DMCR_CSCK                   (0x40000000)
#define MCF_DSPI_DMCR_MSTR                   (0x80000000)

/* Bit definitions and macros for MCF_DSPI_DTCR */
#define MCF_DSPI_DTCR_SPI_TCNT(x)            (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DSPI_DCTAR */
#define MCF_DSPI_DCTAR_BR(x)                 (((x)&0xF)<<0)
#define MCF_DSPI_DCTAR_DT(x)                 (((x)&0xF)<<0x4)
#define MCF_DSPI_DCTAR_ASC(x)                (((x)&0xF)<<0x8)
#define MCF_DSPI_DCTAR_CSSCK(x)              (((x)&0xF)<<0xC)
#define MCF_DSPI_DCTAR_PBR(x)                (((x)&0x3)<<0x10)
#define MCF_DSPI_DCTAR_PBR_1CLK              (0)
#define MCF_DSPI_DCTAR_PBR_3CLK              (0x10000)
#define MCF_DSPI_DCTAR_PBR_5CLK              (0x20000)
#define MCF_DSPI_DCTAR_PBR_7CLK              (0x30000)
#define MCF_DSPI_DCTAR_PDT(x)                (((x)&0x3)<<0x12)
#define MCF_DSPI_DCTAR_PDT_1CLK              (0)
#define MCF_DSPI_DCTAR_PDT_3CLK              (0x40000)
#define MCF_DSPI_DCTAR_PDT_5CLK              (0x80000)
#define MCF_DSPI_DCTAR_PDT_7CLK              (0xC0000)
#define MCF_DSPI_DCTAR_PASC(x)               (((x)&0x3)<<0x14)
#define MCF_DSPI_DCTAR_PASC_1CLK             (0)
#define MCF_DSPI_DCTAR_PASC_3CLK             (0x100000)
#define MCF_DSPI_DCTAR_PASC_5CLK             (0x200000)
#define MCF_DSPI_DCTAR_PASC_7CLK             (0x300000)
#define MCF_DSPI_DCTAR_PCSSCK(x)             (((x)&0x3)<<0x16)
#define MCF_DSPI_DCTAR_LSBFE                 (0x1000000)
#define MCF_DSPI_DCTAR_CPHA                  (0x2000000)
#define MCF_DSPI_DCTAR_CPOL                  (0x4000000)
#define MCF_DSPI_DCTAR_TRSZ(x)               (((x)&0xF)<<0x1B)

/* Bit definitions and macros for MCF_DSPI_DSR */
#define MCF_DSPI_DSR_RXPTR(x)                (((x)&0xF)<<0)
#define MCF_DSPI_DSR_RXCTR(x)                (((x)&0xF)<<0x4)
#define MCF_DSPI_DSR_TXPTR(x)                (((x)&0xF)<<0x8)
#define MCF_DSPI_DSR_TXCTR(x)                (((x)&0xF)<<0xC)
#define MCF_DSPI_DSR_RFDF                    (0x20000)
#define MCF_DSPI_DSR_RFOF                    (0x80000)
#define MCF_DSPI_DSR_TFFF                    (0x2000000)
#define MCF_DSPI_DSR_TFUF                    (0x8000000)
#define MCF_DSPI_DSR_EOQF                    (0x10000000)
#define MCF_DSPI_DSR_TXRXS                   (0x40000000)
#define MCF_DSPI_DSR_TCF                     (0x80000000)

/* Bit definitions and macros for MCF_DSPI_DIRSR */
#define MCF_DSPI_DIRSR_RFDFS                 (0x10000)
#define MCF_DSPI_DIRSR_RFDFE                 (0x20000)
#define MCF_DSPI_DIRSR_RFOFE                 (0x80000)
#define MCF_DSPI_DIRSR_TFFFS                 (0x1000000)
#define MCF_DSPI_DIRSR_TFFFE                 (0x2000000)
#define MCF_DSPI_DIRSR_TFUFE                 (0x8000000)
#define MCF_DSPI_DIRSR_EOQFE                 (0x10000000)
#define MCF_DSPI_DIRSR_TCFE                  (0x80000000)

/* Bit definitions and macros for MCF_DSPI_DTFR */
#define MCF_DSPI_DTFR_TXDATA(x)              (((x)&0xFFFF)<<0)
#define MCF_DSPI_DTFR_CS0                    (0x10000)
#define MCF_DSPI_DTFR_CS2                    (0x40000)
#define MCF_DSPI_DTFR_CS3                    (0x80000)
#define MCF_DSPI_DTFR_CS5                    (0x200000)
#define MCF_DSPI_DTFR_CTCNT                  (0x4000000)
#define MCF_DSPI_DTFR_EOQ                    (0x8000000)
#define MCF_DSPI_DTFR_CTAS(x)                (((x)&0x7)<<0x1C)
#define MCF_DSPI_DTFR_CONT                   (0x80000000)

/* Bit definitions and macros for MCF_DSPI_DRFR */
#define MCF_DSPI_DRFR_RXDATA(x)              (((x)&0xFFFF)<<0)

/* Bit definitions and macros for MCF_DSPI_DTFDR */
#define MCF_DSPI_DTFDR_TXDATA(x)             (((x)&0xFFFF)<<0)
#define MCF_DSPI_DTFDR_TXCMD(x)              (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_DSPI_DRFDR */
#define MCF_DSPI_DRFDR_RXDATA(x)             (((x)&0xFFFF)<<0)


#endif /* __MCF5475_DSPI_H__ */
