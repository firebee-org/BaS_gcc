/*
 * sd_card.h
 *
 * Exported sd-card access routines for the FireBee BaS
 *
 *  Created on: 19.11.2012
 *      Author: mfro
 */

#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include <MCF5475.h>

/*
 * "standard value" for DSPI module configuration register MCF_DSPC_DMCR
 */
const uint32_t DSPI_DMCR_CONF = MCF_DSPI_DMCR_MSTR |	/* FireBee is DSPI master*/	/* 8 bit CS5 on */
			MCF_DSPI_DMCR_CSIS3 |	/* CS3 inactive */
			MCF_DSPI_DMCR_CSIS2 |	/* CS2 inactive */
			MCF_DSPI_DMCR_DTXF |	/* disable transmit FIFO */
			MCF_DSPI_DMCR_DRXF |	/* disable receive FIFO */
			MCF_DSPI_DMCR_CTXF |	/* clear transmit FIFO */
			MCF_DSPI_DMCR_CRXF;		/* clear receive FIFO */
			/* 0x800d3c00 */

extern int sd_card_init(void);
extern uint32_t sd_com(uint32_t data);
extern void sd_card_idle(void);


#endif /* _SD_CARD_H_ */
