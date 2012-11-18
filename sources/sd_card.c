/*
 * sd card
 */

#include <stdint.h>
#include <MCF5475.h>
#include "bas_printf.h"

extern void wait(volatile uint32_t value);

#ifdef _NOT_USED_	/* disabled assembler routines */

void sd_card_idle(void)
{
	__asm__ __volatile__ (
			".extern	sd_idle\n\t"
			"bsr		sd_idle\n\t"
			/* output */:
			/* input */ :
			/* clobber */: "a0","a1","a2","a3","a4","a5",
			              "d0","d1","d2","d3","d4","d5","d6","d7","memory"
	);
}



int sd_card_init(void)
{
	register int ret __asm__("d0");

	__asm__ __volatile__ (
		".extern		sd_init\n\t"
		"bsr.l			sd_init\n\t"
		/* output */: "=r" (ret)
		/* input */ :
		/* clobber */: "a0","a1","a2","a3","a4","a5",
		              "d1","d2","d3","d4","d5","d6","d7","memory"
	);

	return ret;
}
#endif /* _NOT_USED_ */

uint32_t sd_com(uint32_t command)
{
	uint32_t ret;

	MCF_DSPI_DTFR = command;

	while (! (MCF_DSPI_DSR & MCF_DSPI_DSR_TCF));	/* wait until DSPI transfer complete */
	ret = MCF_DSPI_DRFR;							/* read DSPI Rx FIFO register */
	MCF_DSPI_DSR = 0xffffffff;						/* clear DSPI status register */

	return ret;
}

void sd_card_init(void)
{
	uint32_t ret;
	int i;

	xprintf("SD-Card initialization: ");

	MCF_PAD_PAR_DSPI = 0x1fff;
	MCF_PAD_PAR_TIMER = 0xff;			/*
										 * FIXME: really necessary or just an oversight
										 * that PAD_PAR_DSPI is only 16 bit?
										 */

	MCF_DSPI_DMCR = MCF_DSPI_DMCR_MSTR |	/* 8 bit CS5 on */
					MCF_DSPI_DMCR_CSIS3 |
					MCF_DSPI_DMCR_CSIS2 |
					MCF_DSPI_DMCR_DTXF |
					MCF_DSPI_DMCR_DRXF |
					MCF_DSPI_DMCR_CTXF |
					MCF_DSPI_DMCR_CRXF;		/* 0x800d3c00 */

	MCF_DSPI_DCTAR0 = MCF_DSPI_DCTAR_TRSZ(0b111) |	/* transfer size = 8 bit */
					  MCF_DSPI_DCTAR_PCSSCK(0b01) |	/* 3 clock DSPICS to DSPISCK delay prescaler */
					  MCF_DSPI_DCTAR_PASC_3CLK |	/* 3 clock DSPISCK to DSPICS negation prescaler */
					  MCF_DSPI_DCTAR_PDT_3CLK |		/* 3 clock delay between DSPICS assertions prescaler */
					  MCF_DSPI_DCTAR_PBR_3CLK |		/* 3 clock prescaler */
					  MCF_DSPI_DCTAR_ASC(0b1001) |	/* 1024 */
					  MCF_DSPI_DCTAR_DT(0b1001) |	/* 1024 */
					  MCF_DSPI_DCTAR_BR(0b0111);
													/* 0x38558897 */

	MCF_DSPI_DSR = 0xffffffff; 	/* clear DSPI status register */
	wait(1000);					/* wait 1ms */

	MCF_DSPI_DMCR = 0xc00d3c00;
	wait(10000);

	MCF_DSPI_DMCR = 0x800d3c00;
	for (i = 0; i < 10; i++)
	{
		ret = sd_com(0x082000ff);
	}

	MCF_DSPI_DMCR = 0x802d3c00;
	for (i = 0; i < 2; i++)
	{
		ret = sd_com(0x08200000);
	}

	MCF_DSPI_DMCR = 0x800d3c00;
	for (i = 0; i < 2; i++)
	{
		ret = sd_com(0x082000ff);
	}

	MCF_DSPI_DMCR = 0x800d3c00;

	wait(10000);

	xprintf("finished\r\n");
}

void sd_card_idle(void)
{
}

