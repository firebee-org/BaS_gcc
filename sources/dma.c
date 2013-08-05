/*
 * dma.c
 *
 *  Created on: 27.07.2013
 *      Author: mfro
 */

#include "dma.h"
#include <MCD_dma.h>
#include "mcd_initiators.h"
#include "bas_printf.h"

extern char _SYS_SRAM[];
#define SYS_SRAM &_SYS_SRAM[0]

void *dma_memcpy(void *dst, void *src, size_t n)
{
	int ret;
	int speed;
	int32_t start = MCF_SLT_SCNT(0);
	int32_t end;

	xprintf("doing memory to memory DMA from %p to %p (%x bytes)\r\n", src, dst, n);

	ret = MCD_startDma(0, src, n, dst, n, n, 1, DMA_ALWAYS, 7, MCD_SINGLE_DMA|MCD_TT_FLAGS_CW|MCD_TT_FLAGS_RL|MCD_TT_FLAGS_SP, 0);
	if (ret == MCD_OK)
	{
		xprintf("DMA on channel 0 successfully started\r\n");
	}

	do
	{
		ret = MCD_dmaStatus(0);
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
			//xprintf("MCD_RUNNING: DMA active and working on this channel\r\n");
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
	} while (ret != MCD_DONE);
	xprintf("\r\n");

	end = MCF_SLT_SCNT(0);

	speed = n / (start - end) / 132 / 100;
	xprintf("took %d ms (%d.%d Mbytes/second)\r\n",
			(start - end) / 132 / 1000, speed / 10, speed % 10);
#ifdef _NOT_USED_
	__asm__ __volatile__("move.w	sr,d0\n\t"
						 "stop		#0x270\n\t"
						 "move.w	d0,sr": : :);	/* halt CPU until DMA finished */
#endif /* _NOT_USED_ */

	return dst;
}

int dma_init(void)
{
	int res;

	xprintf("MCD DMA API initialization: ");
	res = MCD_initDma((dmaRegs *) &MCF_DMA_TASKBAR, SYS_SRAM, MCD_RELOC_TASKS | MCD_COMM_PREFETCH_EN);
	if (res != MCD_OK)
	{
		xprintf("DMA API initialization failed (0x%x)\r\n", res);
		return 0;
	}
	xprintf("DMA API initialized. Tasks are at %p\r\n", SYS_SRAM);

	// test
	dma_memcpy((void *) 0x1e000000, (void *) 0xe0700000, 0x100000);	/* copy one megabyte of flash to RAM */

	xprintf("DMA finished\r\n");
	return 1;
}

