/*
 * dma.c
 *
 *
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */

#include "dma.h"
#include <MCD_dma.h>
#include "mcd_initiators.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "cache.h"
#include "exceptions.h"

#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error "unknown machine!"
#endif /* MACHINE_FIREBEE */

#define DBG_DMA
#ifdef DBG_DMA
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DBG_DMA */
#define err(format, arg...) do { xprintf("ERROR: %s(): " format, __FUNCTION__, ##arg); } while (0)

extern char _SYS_SRAM[];
#define SYS_SRAM &_SYS_SRAM[0]

struct dma_channel
{
    int req;
    void (*handler)(void);
};

static char used_reqs[32] =
{
    DMA_ALWAYS,  DMA_DSPI_RXFIFO, DMA_DSPI_TXFIFO, DMA_DREQ0,
    DMA_PSC0_RX, DMA_PSC0_TX, DMA_USB_EP0,  DMA_USB_EP1,
    DMA_USB_EP2,  DMA_USB_EP3,  DMA_PCI_TX,  DMA_PCI_RX,
    DMA_PSC1_RX, DMA_PSC1_TX, DMA_I2C_RX,  DMA_I2C_TX,
    0,           0,           0,           0,
    0,           0,           0,           0,
    0,           0,           0,           0,
    0,           0,           0,           0
};

static struct dma_channel dma_channel[NCHANNELS] =
{
    {-1, NULL}, {-1, NULL}, {-1, NULL}, {-1, NULL},
    {-1, NULL}, {-1, NULL}, {-1, NULL}, {-1, NULL},
    {-1, NULL}, {-1, NULL}, {-1, NULL}, {-1, NULL},
    {-1, NULL}, {-1, NULL}, {-1, NULL}, {-1, NULL},
};

/*
 * Enable all DMA interrupts
 *
 * Parameters:
 *  pri     Interrupt Priority
 *  lvl     Interrupt Level
 */
void dma_irq_enable(uint8_t lvl, uint8_t pri)
{
    /* Setup the DMA ICR (#48) */
    MCF_INTC_ICR48 = MCF_INTC_ICR_IP(pri) |
                     MCF_INTC_ICR_IL(lvl);
    dbg("DMA irq assigned level %d, priority %d\r\n", lvl, pri);

    /* Unmask all task interrupts */
    MCF_DMA_DIMR = 0;

    /* Clear the interrupt pending register */
    MCF_DMA_DIPR = 0;

    /* Unmask the DMA interrupt in the interrupt controller */
    MCF_INTC_IMRH &= ~MCF_INTC_IMRH_INT_MASK48;

    dbg("DMA task interrupts unmasked, pending interrupts cleared, interrupt controller active\r\n");
}

/*
 * Disable all DMA interrupts
 */
void dma_irq_disable(void)
{
    /* Mask all task interrupts */
    MCF_DMA_DIMR = (uint32_t) ~0;

    /* Clear any pending task interrupts */
    MCF_DMA_DIPR = (uint32_t) ~0;

    /* Mask the DMA interrupt in the interrupt controller */
    MCF_INTC_IMRH |= MCF_INTC_IMRH_INT_MASK48;

    dbg("DMA interrupts masked and disabled\r\n");
}

int dma_set_initiator(int initiator)
{
    switch (initiator)
    {
        /* these initiators are always active */
        case DMA_ALWAYS:
        case DMA_DSPI_RXFIFO:
        case DMA_DSPI_TXFIFO:
        case DMA_DREQ0:
        case DMA_PSC0_RX:
        case DMA_PSC0_TX:
        case DMA_USB_EP0:
        case DMA_USB_EP1:
        case DMA_USB_EP2:
        case DMA_USB_EP3:
        case DMA_PCI_TX:
        case DMA_PCI_RX:
        case DMA_PSC1_RX:
        case DMA_I2C_RX:
        case DMA_I2C_TX:
            break;

        case DMA_FEC0_RX:
            MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC16(3)) | MCF_DMA_IMCR_IMC16_FEC0RX;
            used_reqs[16] = DMA_FEC0_RX;
            break;

        case DMA_FEC0_TX:
            MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC17(3)) | MCF_DMA_IMCR_IMC17_FEC0TX;
            used_reqs[17] = DMA_FEC0_TX;
            break;

        case DMA_FEC1_RX:
            MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC20(3)) | MCF_DMA_IMCR_IMC20_FEC1RX;
            used_reqs[20] = DMA_FEC1_RX;
            break;

        case DMA_FEC1_TX:
            if (used_reqs[21] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC21(3)) | MCF_DMA_IMCR_IMC21_FEC1TX;
                used_reqs[21] = DMA_FEC1_TX;
            }
            else if (used_reqs[25] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC25(3)) | MCF_DMA_IMCR_IMC25_FEC1TX;
                used_reqs[25] = DMA_FEC1_TX;
            }
            else if (used_reqs[31] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC31(3)) | MCF_DMA_IMCR_IMC31_FEC1TX;
                used_reqs[31] = DMA_FEC1_TX;
            }
            else /* No empty slots */
            {
                err("no free slot found\r\n");

                return 1;
            }
            break;

        case DMA_DREQ1:
            if (used_reqs[29] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC29(3)) | MCF_DMA_IMCR_IMC29_DREQ1;
                used_reqs[29] = DMA_DREQ1;
            }
            else if (used_reqs[21] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC21(3)) | MCF_DMA_IMCR_IMC21_DREQ1;
                used_reqs[21] = DMA_DREQ1;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_CTM0:
            if (used_reqs[24] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC24(3)) | MCF_DMA_IMCR_IMC24_CTM0;
                used_reqs[24] = DMA_CTM0;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_CTM1:
            if (used_reqs[25] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC25(3)) | MCF_DMA_IMCR_IMC25_CTM1;
                used_reqs[25] = DMA_CTM1;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_CTM2:
            if (used_reqs[26] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC26(3)) | MCF_DMA_IMCR_IMC26_CTM2;
                used_reqs[26] = DMA_CTM2;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_CTM3:
            if (used_reqs[27] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC27(3)) | MCF_DMA_IMCR_IMC27_CTM3;
                used_reqs[27] = DMA_CTM3;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_CTM4:
            if (used_reqs[28] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC28(3)) | MCF_DMA_IMCR_IMC28_CTM4;
                used_reqs[28] = DMA_CTM4;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_CTM5:
            if (used_reqs[29] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC29(3)) | MCF_DMA_IMCR_IMC29_CTM5;
                used_reqs[29] = DMA_CTM5;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_CTM6:
            if (used_reqs[30] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC30(3)) | MCF_DMA_IMCR_IMC30_CTM6;
                used_reqs[30] = DMA_CTM6;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_CTM7:
            if (used_reqs[31] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC31(3)) | MCF_DMA_IMCR_IMC31_CTM7;
                used_reqs[31] = DMA_CTM7;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_USBEP4:
            if (used_reqs[26] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC26(3)) | MCF_DMA_IMCR_IMC26_USBEP4;
                used_reqs[26] = DMA_USBEP4;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_USBEP5:
            if (used_reqs[27] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC27(3)) | MCF_DMA_IMCR_IMC27_USBEP5;
                used_reqs[27] = DMA_USBEP5;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_USBEP6:
            if (used_reqs[28] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC28(3)) | MCF_DMA_IMCR_IMC28_USBEP6;
                used_reqs[28] = DMA_USBEP6;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");
                return 1;
            }
            break;

        case DMA_PSC2_RX:
            if (used_reqs[28] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC28(3)) | MCF_DMA_IMCR_IMC28_PSC2RX;
                used_reqs[28] = DMA_PSC2_RX;            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_PSC2_TX:
            if (used_reqs[29] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC29(3)) | MCF_DMA_IMCR_IMC29_PSC2TX;
                used_reqs[29] = DMA_PSC2_TX;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_PSC3_RX:
            if (used_reqs[30] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC30(3)) | MCF_DMA_IMCR_IMC30_PSC3RX;
                used_reqs[30] = DMA_PSC3_RX;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        case DMA_PSC3_TX:
            if (used_reqs[31] == 0)
            {
                MCF_DMA_IMCR = (MCF_DMA_IMCR & ~MCF_DMA_IMCR_IMC31(3)) | MCF_DMA_IMCR_IMC31_PSC3TX;
                used_reqs[31] = DMA_PSC3_TX;
            }
            else /* No empty slots */
            {
                err("no free slot\r\n");

                return 1;
            }
            break;

        default:
            {
                err("don't know what to do\r\n");

                return 1;
            }
    }
    return 0;
}

/*
 * Return the initiator number for the given requestor
 *
 * Parameters:
 *  requestor   Initiator/Requestor identifier
 *
 * Return Value:
 *  The initiator number (0-31) if initiator has been assigned
 *  0 (always initiator) otherwise
 */
uint32_t dma_get_initiator(int requestor)
{
    uint32_t i;

    for (i = 0; i < sizeof(used_reqs); ++i)
    {
        if (used_reqs[i] == requestor)
            return i;
    }
    err("no initiator found for requestor %d\r\n", requestor);

    return 0;
}

/*
 * Remove the given initiator from the active list
 *
 * Parameters:
 *  requestor   Initiator/Requestor identifier
 */
void dma_free_initiator(int requestor)
{
    uint32_t i;

    for (i = 16; i < sizeof(used_reqs); ++i)
    {
        if (used_reqs[i] == requestor)
        {
            used_reqs[i] = 0;
            break;
        }
    }
    dbg("DMA requestor %d freed\r\n", requestor);
}

/*
 * Attempt to find an available channel and mark it as used
 *
 * Parameters:
 *  requestor   Initiator/Requestor identifier
 *
 * Return Value:
 *  First available channel or -1 if they are all occupied
 */
int dma_set_channel(int requestor, void (*handler)(void))
{
    int i;

    /* Check to see if this requestor is already assigned to a channel */
    dbg("check if requestor %d is already assigned to a channel\r\n", requestor);
    if ((i = dma_get_channel(requestor)) != -1)
    {
        return i;
    }

    for (i = 0; i < NCHANNELS; ++i)
    {
        if (dma_channel[i].req == -1)
        {
            dma_channel[i].req = requestor;
            dma_channel[i].handler = handler;
            dbg("assigned channel %d to requestor %d\r\n", i, requestor);
            return i;
        }
    }
    err("no free DMA channel found for requestor %d\r\n", requestor);

    /* All channels taken */
    return -1;
}

void dma_clear_channel(int channel)
{
    if(channel >= 0 && channel < NCHANNELS)
    {
        dma_channel[channel].req = -1;
        dma_channel[channel].handler = NULL;
        dbg("cleared DMA channel %d\r\n", channel);
    }
}

/*
 * Return the channel being initiated by the given requestor
 *
 * Parameters:
 *  requestor   Initiator/Requestor identifier
 *
 * Return Value:
 *  Channel that the requestor is controlling or -1 if hasn't been
 *  activated
 */
int dma_get_channel(int requestor)
{
    uint32_t i;

    for (i = 0; i < NCHANNELS; ++i)
    {
        if (dma_channel[i].req == requestor)
            return i;
    }
    dbg("no channel occupied by requestor %d\r\n", requestor);
    return -1;
}

/*
 * Remove the channel being initiated by the given requestor from
 * the active list
 *
 * Parameters:
 *  requestor   Initiator/Requestor identifier
 */
void dma_free_channel(int requestor)
{
    uint32_t i;

    for (i = 0; i < NCHANNELS; ++i)
    {
        if (dma_channel[i].req == requestor)
        {
            dma_channel[i].req = -1;
            dma_channel[i].handler = NULL;
            break;
        }
    }
}

/*
 * This is the catch-all interrupt handler for the mult-channel DMA
 */
bool dma_interrupt_handler(void *arg1, void *arg2)
{
    int i, interrupts;
    uint32_t ipl;

    ipl = set_ipl(7);       /* do not disturb */

    /*
     * Determine which interrupt(s) triggered by AND'ing the
     * pending interrupts with those that aren't masked.
     */
    interrupts = MCF_DMA_DIPR & ~MCF_DMA_DIMR;

    /* Make sure we are here for a reason */
    if (interrupts == 0)
    {
        err("not DMA interrupt!\r\n");
        return 0;
    }

    dbg("");
    /* Clear the interrupt in the pending register */
    MCF_DMA_DIPR = interrupts;

    for (i = 0; i < 16; ++i, interrupts >>= 1)
    {
        if (interrupts & 0x1)
        {
            /* If there is a handler, call it */
            if (dma_channel[i].handler != NULL)
            {
                dbg("call handler for DMA channel %d (%p)\r\n", i, dma_channel[i].handler);
                dma_channel[i].handler();
            }
        }
    }

    //set_ipl(ipl);

    return true;   /* handled */
}
/********************************************************************/

void *dma_memcpy(void *dst, void *src, size_t n)
{
    int ret;

#ifdef DBG_DMA
    int32_t time;
    int32_t start;
    int32_t end;

    start = MCF_SLT0_SCNT;
#endif /* DBG_DMA */

    ret = MCD_startDma(1, src, 4, dst, 4, n, 4, DMA_ALWAYS, 0, MCD_SINGLE_DMA, 0);
    if (ret == MCD_OK)
    {
        dbg("DMA on channel 1 successfully started\r\n");
    }

    do
    {
        ret = MCD_dmaStatus(1);
#ifdef _NOT_USED_ /* suppress annoying printout for now */
        switch (ret)
        {
            case MCD_NO_DMA:
                xprintf("MCD_NO_DMA: no DMA active on this channel\r\n");
                return NULL;
                break;
            case MCD_IDLE:
                xprintf("MCD_IDLE: DMA defined but not active (initiator not ready)\r\n");
                break;
            case MCD_RUNNING:
                xprintf("MCD_RUNNING: DMA active and working on this channel\r\n");
                break;
            case MCD_PAUSED:
                xprintf("MCD_PAUSED: DMA defined and enabled, but currently paused\r\n");
                break;
            case MCD_HALTED:
                xprintf("MCD_HALTED: DMA killed\r\n");
                return NULL;
                break;
            case MCD_DONE:
                xprintf("MCD_DONE: DMA finished\r\n");
                break;
            case MCD_CHANNEL_INVALID:
                xprintf("MCD_CHANNEL_INVALID: invalid DMA channel\r\n");
                return NULL;
                break;
            default:
                xprintf("unknown DMA status %d\r\n", ret);
                break;
        }
#endif
    } while (ret != MCD_DONE);

#ifdef DBG_DMA
    end = MCF_SLT0_SCNT;
    time = (start - end) / (SYSCLK / 1000) / 1000;
#endif /* DBG_DMA */
    dbg("took %d ms (%f Mbytes/second)\r\n", time, n / (float) time / 1000.0);

    return dst;
}

int dma_init(void)
{
    int i;
    int res;

    dbg("MCD DMA API initialization: ");
    res = MCD_initDma((dmaRegs *) &_MBAR[0x8000], SYS_SRAM, MCD_RELOC_TASKS | MCD_COMM_PREFETCH_EN);
    if (res != MCD_OK)
    {
        err("DMA API initialization failed (0x%x)\r\n", res);
        return 0;
    }

    /*
     * make sure dma_channel array is properly initialized
     */
    for (i = 0; i < NCHANNELS; i++)
    {
        dma_channel[i].req = -1;
        dma_channel[i].handler = NULL;
    }

    // test
    dma_memcpy((void *) 0x10000, (void *) 0x03e00000, 0x00100000);  /* copy one megabyte of flash to RAM */

    return 0;
}

