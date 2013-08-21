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
#include "bas_string.h"
#include "cache.h"

extern char _SYS_SRAM[];
#define SYS_SRAM &_SYS_SRAM[0]

void print_sp(void)
{
	register char *sp asm("sp");
	xprintf("sp=%x\r\n", sp);
}


void *dma_memcpy(void *dst, void *src, size_t n)
{
	int ret;
	volatile int32_t time;
	volatile int32_t start;
	volatile int32_t end;
	uint8_t *d;
	uint8_t *s;

	xprintf("compare memcpy() to memory to memory DMA from %p to %p (0x%x bytes ending at 0x%x)\r\n",
				src, dst, n, dst + n);
	d = dst;
	s = src;

//#ifdef _NOT_USED_
	start = MCF_SLT0_SCNT;
	/* first check if we can do a "traditional" memcpy() to the destination and measure speed */

	memcpy(d, s, n);

	end = MCF_SLT0_SCNT;

	time = (start - end) / 132;
	xprintf("memcpy() took %d ms (%d.%d Mbytes/second)\r\n",
			time, n / time / 1000, n / time % 1000);
	flush_and_invalidate_caches();
//#endif

	xprintf("clear target area after memcpy():");
	bzero(dst, n);
	flush_and_invalidate_caches();
	xprintf(" finished, flush caches: ");
	flush_and_invalidate_caches();
	xprintf("finished\r\n");

	start = MCF_SLT0_SCNT;
	ret = MCD_startDma(0, src, 4, dst, 4, n, 4, DMA_ALWAYS, 7, MCD_SINGLE_DMA|MCD_TT_FLAGS_CW|MCD_TT_FLAGS_RL|MCD_TT_FLAGS_SP, 0);
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
	} while (ret != MCD_DONE);
	xprintf("\r\n");

	end = MCF_SLT0_SCNT;
	time = (start - end) / 132;
	xprintf("start = %d, end = %d, time = %d\r\n", start, end, time);
	xprintf("took %d ms (%d.%d Mbytes/second)\r\n",	time, n / time / 1000, n / time % 1000);

	/*
	 * verify if copy succeeded
	 */
	xprintf("verify if DMA copy succeeded:\r\n");

	xprintf("flush caches first:");
	flush_and_invalidate_caches();
	xprintf(" done.\r\n");

	s = src;
	d = dst;

	start = MCF_SLT0_SCNT;
	do
	{
		if (*d != *s)
		{
			xprintf("copy verification failed: %d (%p) != %d (%p) !\r\n", *d, d, *s, s);
			break;
		}
		d++; s++;
	} while (d < (uint8_t *) dst + n);
	if (d >= (uint8_t *) dst + n)
	{
		xprintf("DMA copy verification successful!\r\n");
		end = MCF_SLT0_SCNT;

		time = (start - end) / 132;
		xprintf("took %d ms (%d.%d Mbytes/second)\r\n",	time, n / time / 1000, n / time % 1000);

	}

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
	int version;
	char *long_version;

	xprintf("MCD DMA API initialization: ");
	res = MCD_initDma((dmaRegs *) &MCF_DMA_TASKBAR, SYS_SRAM, MCD_RELOC_TASKS | MCD_COMM_PREFETCH_EN);
	if (res != MCD_OK)
	{
		xprintf("DMA API initialization failed (0x%x)\r\n", res);
		return 0;
	}
	version = MCD_getVersion(&long_version);
	xprintf("DMA API version %d.%d initialized. Tasks are at %p\r\n", version / 0xff, version % 0xff, SYS_SRAM);

	// test
	dma_memcpy((void *) 0x01000000, (void *) 0xe0000000, 0x00100000);	/* copy one megabyte of flash to RAM */

	xprintf("DMA finished\r\n");
	return 0;
}

