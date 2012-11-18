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

/*
 * Write data to the DSPI TX FIFO register
 * First 16 bits are the SPI command field (basically say only HOW to transfer the second
 * half), second are the data to transfer
 */
uint32_t sd_com(uint32_t data)
{
	uint32_t ret;

	MCF_DSPI_DTFR = data;	/* write value to TX FIFO */

	while (! (MCF_DSPI_DSR & MCF_DSPI_DSR_TCF));	/* wait until DSPI transfer complete */
	ret = MCF_DSPI_DRFR;							/* read DSPI Rx FIFO register */
	MCF_DSPI_DSR = 0xffffffff;						/* clear DSPI status register */

	return ret;
}

void sd_card_init(void)
{
	uint32_t ret;
	const uint32_t DSPI_DMCR_CONF = MCF_DSPI_DMCR_MSTR |	/* FireBee is DSPI master*/	/* 8 bit CS5 on */
			MCF_DSPI_DMCR_CSIS3 |	/* CS3 inactive */
			MCF_DSPI_DMCR_CSIS2 |	/* CS2 inactive */
			MCF_DSPI_DMCR_DTXF |	/* disable transmit FIFO */
			MCF_DSPI_DMCR_DRXF |	/* disable receive FIFO */
			MCF_DSPI_DMCR_CTXF |	/* clear transmit FIFO */
			MCF_DSPI_DMCR_CRXF;		/* clear receive FIFO */
			/* 0x800d3c00 */

	int i;

	xprintf("SD-Card initialization: ");

	MCF_PAD_PAR_DSPI = 0x1fff;
	MCF_PAD_PAR_TIMER = 0xff;			/*
										 * FIXME: really necessary or just an oversight
										 * that PAD_PAR_DSPI is only 16 bit?
										 */

	MCF_DSPI_DMCR = DSPI_DMCR_CONF;

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

	MCF_DSPI_DMCR = DSPI_DMCR_CONF | MCF_DSPI_DMCR_CSCK; /* enable continuous serial comms clock */
	/* 0xc00d3c00 */

	wait(10000);

	MCF_DSPI_DMCR = DSPI_DMCR_CONF;
	for (i = 0; i < 10; i++)
	{
		ret = sd_com(MCF_DSPI_DTFR_EOQ | MCF_DSPI_DTFR_CS5 | 0x00FF);
	}

	MCF_DSPI_DMCR = DSPI_DMCR_CONF | MCF_DSPI_DMCR_CSIS5; /* CS5 inactive */
	/* 0x802d3c00; */

	for (i = 0; i < 2; i++)
	{
		ret = sd_com(MCF_DSPI_DTFR_EOQ | MCF_DSPI_DTFR_CS5);
	}

	MCF_DSPI_DMCR = DSPI_DMCR_CONF;
	for (i = 0; i < 2; i++)
	{
		ret = sd_com(MCF_DSPI_DTFR_EOQ | MCF_DSPI_DTFR_CS5 | 0x00FF);
	}

	MCF_DSPI_DMCR = DSPI_DMCR_CONF;

	wait(10000);

	xprintf("finished\r\n");
}

void sd_card_idle(void)
{
}

