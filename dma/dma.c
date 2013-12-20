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

#if MACHINE_FIREBEE
#include "firebee.h"
#elif MACHINE_M5484LITE
#include "m5484l.h"
#endif /* MACHINE_FIREBEE */

extern char _SYS_SRAM[];
#define SYS_SRAM &_SYS_SRAM[0]

struct dma_channel
{
	int req;
	void (*handler)(void);
};

static struct dma_channel dma_channel[NCHANNELS] =
{
	{-1,NULL}, {-1,NULL}, {-1,NULL}, {-1,NULL},
	{-1,NULL}, {-1,NULL}, {-1,NULL}, {-1,NULL},
	{-1,NULL}, {-1,NULL}, {-1,NULL}, {-1,NULL},
	{-1,NULL}, {-1,NULL}, {-1,NULL}, {-1,NULL},
};

/*
 * return the channel being initiated by the given requestor
 */
int dma_get_channel(int requestor)
{
	int i;

	for (i = 0; i < NCHANNELS; i++)
	{
		if (dma_channel[i].req == requestor)
		{
			return i;
		}
	}
	return -1;
}

int dma_set_channel(int requestor, void (*handler)(void))
{
	int i;

	/* check to see if requestor is already assigned to a channel */
	if ((i = dma_get_channel(requestor)) != -1)
	{
		return i;
	}

	for (i = 0; i < NCHANNELS; i++)
	{
		if (dma_channel[i].req == -1)
		{
			dma_channel[i].req = requestor;
			dma_channel[i].handler = handler;

			return i;
		}
	}

	/* all channels taken */
	return -1;
}

void *dma_memcpy(void *dst, void *src, size_t n)
{
	int ret;
	volatile int32_t time;
	volatile int32_t start;
	volatile int32_t end;

	start = MCF_SLT0_SCNT;
	ret = MCD_startDma(1, src, 4, dst, 4, n, 4, DMA_ALWAYS, 0, MCD_SINGLE_DMA, 0);
	if (ret == MCD_OK)
	{
		xprintf("DMA on channel 1 successfully started\r\n");
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

	end = MCF_SLT0_SCNT;
	time = (start - end) / (SYSCLK / 1000) / 1000;
	xprintf("took %d ms (%f Mbytes/second)\r\n",	time, n / (float) time / 1000.0);

	return dst;
}

int dma_init(void)
{
	int res;

	xprintf("MCD DMA API initialization: ");
	res = MCD_initDma((dmaRegs *) &_MBAR[0x8000], SYS_SRAM, MCD_RELOC_TASKS | MCD_COMM_PREFETCH_EN);
	if (res != MCD_OK)
	{
		xprintf("DMA API initialization failed (0x%x)\r\n", res);
		return 0;
	}

	// test
	dma_memcpy((void *) 0x10000, (void *) 0x03e00000, 0x00100000);	/* copy one megabyte of flash to RAM */

	xprintf("DMA finished\r\n");
	return 0;
}

