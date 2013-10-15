#include <stdint.h>
#include <bas_types.h>
#include <sd_card.h>
#include <bas_printf.h>
#include <sysinit.h>
#include <wait.h>
#include <MCF5475.h>

/*
 * Firebee: MMCv3/SDv1/SDv2 (SPI mode) control module
 *
 *
 * Copyright (C) 2011, ChaN, all right reserved.
 *
 * This software is a free software and there is NO WARRANTY.
 * No restriction on use. You can use, modify and redistribute it for
 * personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
 * Redistributions of source code must retain the above copyright notice.
 *
 */

/* Copyright (C) 2012, mfro, all rights reserved. */


#define	CS_LOW()	{ dspi_fifo_val |= MCF_DSPI_DTFR_CS5; }
#define	CS_HIGH()	{ dspi_fifo_val &= ~MCF_DSPI_DTFR_CS5; }

/*
 * DCTAR_PBR (baud rate prescaler) and DCTAR_BR (baud rate scaler) together determine the SPI baud rate. The forumula is
 *
 * 	baud rate = system clock / DCTAR_PBR * 1 / DCTAR_BR.
 *
 * 	System clock for the Firebee is 133 MHZ.
 *
 * 	The SPICLK_FAST() example calculates as follows: baud rate = 133 MHz / 3 * 1 / 2 = 22,16 MHz
 *  SPICLK_SLOW() should be between 100 and 400 kHz: 133 MHz / 1 * 1 / 1024 = 129 kHz
 */
#define SPICLK_FAST() { MCF_DSPI_DCTAR0 = MCF_DSPI_DCTAR_TRSZ(0b0111) |	/* transfer size = 8 bit */ \
					  MCF_DSPI_DCTAR_PCSSCK(0b01) |	/* 3 clock DSPICS to DSPISCK delay prescaler */ \
					  MCF_DSPI_DCTAR_PASC_3CLK |	/* 3 clock DSPISCK to DSPICS negation prescaler */ \
					  MCF_DSPI_DCTAR_PDT_3CLK |		/* 3 clock delay between DSPICS assertions prescaler */ \
					  MCF_DSPI_DCTAR_PBR_1CLK |		/* 3 clock baudrate prescaler */ \
					  MCF_DSPI_DCTAR_CSSCK(1) |		/* delay scaler * 4 */\
					  MCF_DSPI_DCTAR_ASC(0b0001) |	/* 2 */ \
					  MCF_DSPI_DCTAR_DT(0b0010) |	/* 2 */ \
					  MCF_DSPI_DCTAR_BR(0b0001); }	/* clock / 2 */

#define SPICLK_SLOW() { \
						MCF_DSPI_DCTAR0 = MCF_DSPI_DCTAR_TRSZ(0b111) |	/* transfer size = 8 bit */ \
						MCF_DSPI_DCTAR_PCSSCK(0b01) |	/* 3 clock DSPICS to DSPISCK delay prescaler */ \
						MCF_DSPI_DCTAR_PASC_3CLK |	/* 3 clock DSPISCK to DSPICS negation prescaler */ \
						MCF_DSPI_DCTAR_PDT_3CLK |		/* 3 clock delay between DSPICS assertions prescaler */ \
						MCF_DSPI_DCTAR_PBR_3CLK |		/* 1 clock baudrate prescaler */ \
						MCF_DSPI_DCTAR_CSSCK(8) |		/* delay scaler * 512 */\
						MCF_DSPI_DCTAR_ASC(8) |	/* 2 */ \
						MCF_DSPI_DCTAR_DT(9) |	/* 2 */ \
						MCF_DSPI_DCTAR_BR(7); \
					}



/*--------------------------------------------------------------------------

   Module Private Functions

---------------------------------------------------------------------------*/

#include "diskio.h"


/* MMC/SD command */
#define CMD0	(0)				/* GO_IDLE_STATE */
#define CMD1	(1)				/* SEND_OP_COND (MMC) */
#define ACMD41	(0x80+41)		/* SEND_OP_COND (SDC) */
#define CMD8	(8)				/* SEND_IF_COND */
#define CMD9	(9)				/* SEND_CSD */
#define CMD10	(10)			/* SEND_CID */
#define CMD12	(12)			/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)		/* SD_STATUS (SDC) */
#define CMD16	(16)			/* SET_BLOCKLEN */
#define CMD17	(17)			/* READ_SINGLE_BLOCK */
#define CMD18	(18)			/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)			/* SET_BLOCK_COUNT (MMC) */
#define ACMD23	(0x80+23)		/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)			/* WRITE_BLOCK */
#define CMD25	(25)			/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)			/* ERASE_ER_BLK_START */
#define CMD33	(33)			/* ERASE_ER_BLK_END */
#define CMD38	(38)			/* ERASE */
#define CMD55	(55)			/* APP_CMD */
#define CMD58	(58)			/* READ_OCR */


static volatile DSTATUS Stat = 0 /* STA_NOINIT */;	/* Physical drive status */
static uint8_t CardType;			/* Card type flags */


static uint32_t dspi_fifo_val = MCF_DSPI_DTFR_CTCNT;

/*-----------------------------------------------------------------------*/
/* Send/Receive data to the MMC  (Platform dependent)                    */
/*-----------------------------------------------------------------------*/

/*
 * Exchange a byte. If last is false (0), there will be more bytes to follow (EOQ flag in DTFR left unset)
 */
static uint8_t xchg_spi(uint8_t byte, int last)
{
	uint32_t fifo;
	uint8_t res;

	fifo =  dspi_fifo_val | (byte & 0xff);							/* transfer bytes only */
	//fifo |= (last ? MCF_DSPI_DTFR_EOQ : 0);			/* mark last transfer */
	MCF_DSPI_DTFR = fifo;
	while (! (MCF_DSPI_DSR & MCF_DSPI_DSR_TCF));	/* wait until DSPI transfer complete */
	fifo = MCF_DSPI_DRFR;							/* read transferred word */

	MCF_DSPI_DSR = -1;		 						/* clear DSPI status register */

	res = fifo & 0xff;

	return res;
}


/* Receive multiple byte
 *
 * buff: pointer to data buffer
 * btr: number of bytes to receive (16, 64 or 512)
 */
static void rcvr_spi_multi(uint8_t *buff, uint32_t count)
{
	int i;

	for (i = 0; i < count - 1; i++)
		*buff++ = xchg_spi(0xff, 0);
	*buff++ = xchg_spi(0xff, 1); 		/* transfer last byte and stop transmission */
}


#if _USE_WRITE
/* Send multiple byte
 *
 * buff: pointer to data
 * btx: number of bytes to send
 */
static void xmit_spi_multi(const uint8_t *buff, uint32_t btx)
{
	int i;

	for (i = 0; i < btx - 1; i++)
		xchg_spi(*buff++, 0);
	xchg_spi(*buff++, 1);				/* transfer last byte and indicate end of transmission */
}
#endif


static bool card_ready(void)
{
	uint8_t d;

	d = xchg_spi(0xff, 1);
	return (d == 0xff);
}

/*
 * Wait for card ready
 *
 * wt: timeout in ns
 * returns 1: ready, 0: timeout
 */
static int wait_ready(uint32_t wt)
{
	return waitfor(wt * 1000, card_ready);
}



/*
 * Deselect card and release SPI
 */
static void deselect(void)
{
	CS_HIGH();
	wait_ready(50);	/* Dummy clock (force DO hi-z for multiple slave SPI) */
}



/*
 * Select card and wait for ready
 */

static int select(void)	/* 1:OK, 0:Timeout */
{
	CS_LOW();

	if (wait_ready(500))
		return 1;	/* OK */
	deselect();
	return 0;	/* Timeout */
}



/*
 * Control SPI module (Platform dependent)
 */
static void power_on(void)	/* Enable SSP module */
{
	MCF_PAD_PAR_DSPI = 0x1fff;			/* configure all DSPI GPIO pins for DSPI usage */
	dspi_fifo_val = MCF_DSPI_DTFR_CTCNT;
	/*
	 * initialize DSPI module configuration register
	 */
	MCF_DSPI_DMCR = MCF_DSPI_DMCR_MSTR |	/* FireBee is DSPI master*/
				MCF_DSPI_DMCR_CSIS5 |		/* CS5 inactive state high */
				MCF_DSPI_DMCR_CSIS3 |		/* CS3 inactive state high */
				MCF_DSPI_DMCR_CSIS2 |		/* CS2 inactive state high */
				MCF_DSPI_DMCR_CSIS0 |		/* CS0 inactive state high */
				MCF_DSPI_DMCR_DTXF |		/* disable transmit FIFO */
				MCF_DSPI_DMCR_DRXF |		/* disable receive FIFO */
				MCF_DSPI_DMCR_CTXF |		/* clear transmit FIFO */
				MCF_DSPI_DMCR_CRXF;			/* clear receive FIFO */

	/* initialize DSPI clock and transfer attributes register 0 */
	SPICLK_SLOW();

	CS_HIGH();					/* Set CS# high */

	/* card should now be initialized as MMC */

	wait(10 * 1000);	/* 10ms */
}


static void power_off (void)		/* Disable SPI function */
{
	select();				/* Wait for card ready */
	deselect();
}


/*-----------------------------------------------------------------------*/
/* Receive a data packet from the MMC                                    */
/*-----------------------------------------------------------------------*/
static int rcvr_datablock(uint8_t *buff, uint32_t btr)
{
	uint8_t token;
	int32_t target = MCF_SLT_SCNT(0) - (20000L * 100L * 132L);

	do {								/* Wait for DataStart token in timeout of 200ms */
		token = xchg_spi(0xFF, 0);
	} while ((token == 0xFF) && MCF_SLT_SCNT(0) - target > 0);

	if (token == 0xff)
	{
		xprintf("no data start token received after 2000ms in rcvr_datablock\r\n");
		return 0;
	}

	if (token != 0xFE)
	{
		xprintf("invalid token (%x) in rcvr_datablock()!\r\n", token);
		return 0;		/* Function fails if invalid DataStart token or timeout */
	}

	rcvr_spi_multi(buff, btr);						/* Store trailing data to the buffer */

	xchg_spi(0xFF, 1);
	xchg_spi(0xFF, 1);			/* Discard CRC */

	return 1;										/* Function succeeded */
}



/*-----------------------------------------------------------------------*/
/* Send a data packet to the MMC                                         */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
static int xmit_datablock(const uint8_t *buff, uint8_t token)
{
	uint8_t resp;


	if (!wait_ready(500))
	{
		xprintf("card did not respond ready after 500 ms in xmit_datablock()\r\n");
		return 0;		/* Wait for card ready */
	}

	xchg_spi(token, 1);					/* Send token */
	if (token != 0xFD) {				/* Send data if token is other than StopTran */
		xmit_spi_multi(buff, 512);		/* Data */
		xchg_spi(0xFF, 1);
		xchg_spi(0xFF, 1);				/* Dummy CRC */

		resp = xchg_spi(0xFF, 1);		/* Receive data resp */
		if ((resp & 0x1F) != 0x05)		/* Function fails if the data packet was not accepted */
		{
			xprintf("card did not accept data packet in xmit_datablock() (resp = %x)\r\n", resp & 0x1F);
			return 0;
		}
	}

	wait_ready(3000);

	return 1;
}
#endif


/*-----------------------------------------------------------------------*/
/* Send a command packet to the MMC                                      */
/*-----------------------------------------------------------------------*/

static uint8_t send_cmd(uint8_t cmd, uint32_t arg)
{
	int n;
	int res;

	if (cmd & 0x80)
	{	/* Send a CMD55 prior to ACMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1)
			return res;
	}

	/* Select card */
	deselect();
	if (!select())
	{
		xprintf("card could not be selected in send_cmd()\r\n");
		return 0xFF;
	}

	if (!wait_ready(5000))
	{
		xprintf("card did not respond ready after 5000 ms in send_cmd()\r\n");
		return 0xff;		/* Wait for card ready */
	}

	/* Send command packet */
	xchg_spi(0x40 | cmd, 0);				/* Start + command index */
	xchg_spi((uint8_t)(arg >> 24), 0);		/* Argument[31..24] */
	xchg_spi((uint8_t)(arg >> 16), 0);		/* Argument[23..16] */
	xchg_spi((uint8_t)(arg >> 8), 0);		/* Argument[15..8] */
	xchg_spi((uint8_t)arg, 1);				/* Argument[7..0] */

	n = 0x01;								/* Dummy CRC + Stop */
	if (cmd == CMD0)
		n = 0x95;				/* Valid CRC for CMD0(0) */
	if (cmd == CMD8)
		n = 0x87;				/* Valid CRC for CMD8(0x1AA) */
	xchg_spi(n, 0);

	/* Receive command resp */
	if (cmd == CMD12)
	{
		xchg_spi(0xFF, 0);	/* Discard following one byte when CMD12 */
	}

	n = 10000000;				/* Wait for response (1000 bytes max) */
	do
		res = xchg_spi(0xFF, 1);
	while ((res & 0x80) && --n);
	return res;							/* Return received response */
}



/*--------------------------------------------------------------------------

   Public Functions

---------------------------------------------------------------------------*/


/*
 * Initialize disk drive
 *
 * drv: physical drive number (0)
 */
DSTATUS disk_initialize(uint8_t drv)
{
	uint8_t n, cmd, card_type, ocr[4];
	uint8_t buff[16];
	int res;

	if (drv)
		return STA_NOINIT;			/* Supports only drive 0 */

	power_on();							/* Initialize SPI */

	if (Stat & STA_NODISK)
		return Stat;	/* Is card existing in the socket? */

	SPICLK_SLOW();

	for (n = 10; n; n--)
		xchg_spi(0xFF, 1);	/* Send 80 dummy clocks */

	card_type = 0;
	if (send_cmd(CMD0, 0) == 1)
	{
		/* Put the card SPI/Idle state */
		int32_t target;

		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDv2? */
			for (n = 0; n < 4; n++)
				ocr[n] = xchg_spi(0xFF, 1);	/* Get 32 bit return value of R7 resp */
			if (ocr[2] == 0x01 && ocr[3] == 0xAA)
			{
				int res;

				/* Is the card supports vcc of 2.7-3.6V? */
				target = MCF_SLT_SCNT(0) - (1000L * 1000L * 132);	/* 1 sec */
				while (MCF_SLT_SCNT(0) - target > 0)
				{
					res = send_cmd(ACMD41, 1UL << 30);	/* Wait for end of initialization with ACMD41(HCS) */
					if (res != 0xff)
						break;
				}
				xprintf("res = %d\r\n", res);

				target = MCF_SLT_SCNT(0) - (1000L * 1000L * 132);	/* 1 sec */
				while (MCF_SLT_SCNT(0) - target > 0)
				{
					res = send_cmd(CMD58, 0);		/* Check CCS bit in the OCR */
					if (res != 0xff)
						break;
				}
				xprintf("res = %d\r\n", res);
				for (n = 0; n < 4; n++)
					ocr[n] = xchg_spi(0xFF, 1);
				card_type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* Card id SDv2 */
			}
		}
		else
		{	/* Not SDv2 card */
			if (send_cmd(ACMD41, 0) <= 1)
			{	/* SDv1 or MMC? */
				card_type = CT_SD1;
				cmd = ACMD41;	/* SDv1 (ACMD41(0)) */
			} else {
				card_type = CT_MMC;
				cmd = CMD1;	/* MMCv3 (CMD1(0)) */
			}
			target = MCF_SLT_SCNT(0) - (1000L * 1000L * 132);			/* 1 sec */
			while (MCF_SLT_SCNT(0) - target > 0 && send_cmd(cmd, 0));	/* Wait for end of initialization */

			if (send_cmd(CMD16, 512) != 0)								/* Set block length: 512 */
				card_type = 0;
		}
	}
	CardType = card_type;	/* Card type */

	res = disk_ioctl(0, MMC_GET_CSD, buff);
	if (res == RES_OK)
	{
		xprintf("CSD of card:\r\n");
		hexdump(buff, 16);
	}
	deselect();

	if (card_type)
	{
		/* OK */

		SPICLK_FAST();			/* Set fast clock */
		Stat &= ~STA_NOINIT;	/* Clear STA_NOINIT flag */
		xprintf("card type: %d\r\n", card_type);
		res = disk_ioctl(0, MMC_GET_CSD, buff);
		if (res == RES_OK)
		{
			xprintf("CSD of card now:\r\n");
			hexdump(buff, 16);
		}
		deselect();

	}
	else
	{
		/* Failed */
		xprintf("no card type detected in disk_initialize()\r\n");
		power_off();
		Stat = STA_NOINIT;
	}

	return Stat;
}



/*-----------------------------------------------------------------------*/
/* Get disk status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(uint8_t drv)
{
	if (drv) return STA_NOINIT;		/* Supports only drive 0 */

	return Stat;	/* Return disk status */
}

DSTATUS disk_reset(uint8_t drv)
{
	if (drv) return STA_NOINIT;

	deselect();
	disk_initialize(0);

	return 0;
}

/*-----------------------------------------------------------------------*/
/* Read sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(uint8_t drv, uint8_t *buff, uint32_t sector, uint8_t count)
{
	if (drv)
	{
		xprintf("wrong drive in disk_read()\r\n");
		return RES_PARERR;		/* Check parameter */
	}

	if (! count)
	{
		xprintf("wrong count in disk_read()\r\n");
		return RES_PARERR;
	}

	if (Stat & STA_NOINIT)
	{
		xprintf("drive not ready in disk_read()\r\n");
		return RES_NOTRDY;	/* Check if drive is ready */
	}

	if (!(CardType & CT_BLOCK)) sector *= 512;	/* LBA or BA conversion (byte addressing cards) */

	if (count == 1) {	/* Single sector read */
		if ((send_cmd(CMD17, sector) == 0))		/* READ_SINGLE_BLOCK */
			if (rcvr_datablock(buff, 512))
				count = 0;
	}
	else {				/* Multiple sector read */
		if (send_cmd(CMD18, sector) == 0) {	/* READ_MULTIPLE_BLOCK */
			do {
				if (!rcvr_datablock(buff, 512))
					break;
				buff += 512;
			} while (--count);
			send_cmd(CMD12, 0);				/* STOP_TRANSMISSION */
		}
	}
	deselect();

	return count ? RES_ERROR : RES_OK;	/* Return result */
}



/*-----------------------------------------------------------------------*/
/* Write sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write(uint8_t drv,	const uint8_t *buff, uint32_t sector, uint8_t count)
{
	int res;

	if (drv || !count) return RES_PARERR;		/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check drive status */
	if (Stat & STA_PROTECT) return RES_WRPRT;	/* Check write protect */

	if (!(CardType & CT_BLOCK))
	{
		sector *= 512;	/* LBA ==> BA conversion (byte addressing cards) */
	}

	if (count == 1)
	{	/* Single sector write */
		res = send_cmd(CMD24, sector);
		if (res == 0)
		{
			count = 0;
		}
		else
			xprintf("send_cmd(CMD24, ...) failed in disk_write()\r\n");

		if (xmit_datablock(buff, 0xFE))
		{
			count = 0;
		}
		else
		{
			xprintf("xmit_datablock(buff, ...) failed in disk_write()\r\n");
		}
	}
	else {				/* Multiple sector write */
		if (CardType & CT_SDC) send_cmd(ACMD23, count);	/* Predefine number of sectors */
		if (send_cmd(CMD25, sector) == 0)
		{	/* WRITE_MULTIPLE_BLOCK */
			do
			{
				if (!xmit_datablock(buff, 0xFC)) break;
				buff += 512;
			} while (--count);

			if (!xmit_datablock(0, 0xFD))	/* STOP_TRAN token */
			{
				count = 1;
			}
		}
	}
	deselect();

	if (count)	/* we had an error, try a reinit */
	{
		xprintf("disk_write() failed (count=%d)\r\n", count);
	}

	return count ? RES_ERROR : RES_OK;	/* Return result */
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous drive controls other than data read/write               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl(uint8_t drv,	uint8_t ctrl, void *buff)
{
	DRESULT res;
	uint8_t n, csd[16], *ptr = buff;
	uint32_t *dp, st, ed, csize;


	if (drv) return RES_PARERR;					/* Check parameter */
	if (Stat & STA_NOINIT) return RES_NOTRDY;	/* Check if drive is ready */

	res = RES_ERROR;

	switch (ctrl) {
	case CTRL_SYNC :		/* Wait for end of internal write process of the drive */
		if (select()) {
			deselect();
			res = RES_OK;
		}
		break;

	case GET_SECTOR_COUNT :	/* Get drive capacity in unit of sector (DWORD) */
		if ((send_cmd(CMD9, 0) == 0))
		{
			if (rcvr_datablock(csd, 16))
			{
				if ((csd[0] >> 6) == 1)
				{	/* SDC ver 2.00 */
					csize = csd[9] + ((uint16_t)csd[8] << 8) + ((uint32_t)(csd[7] & 63) << 16) + 1;
					* (uint32_t*) buff = csize << 10;
				}
				else
				{
					/* SDC ver 1.XX or MMC ver 3 */
					n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
					csize = (csd[8] >> 6) + ((uint16_t)csd[7] << 2) + ((uint16_t)(csd[6] & 3) << 10) + 1;
					* (uint32_t*) buff = csize << (n - 9);
				}
			}
			res = RES_OK;
		}
		break;

	case GET_SECTOR_SIZE :	/* Get sector size in unit of byte (WORD) */
		* (uint32_t*) buff = 512;
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
		if (CardType & CT_SD2) {	/* SDC ver 2.00 */
			if (send_cmd(ACMD13, 0) == 0) {	/* Read SD status */
				xchg_spi(0xFF, 1);
				if (rcvr_datablock(csd, 16)) {				/* Read partial block */
					for (n = 64 - 16; n; n--) xchg_spi(0xFF, 1);	/* Purge trailing data */
					*(uint32_t*)buff = 16UL << (csd[10] >> 4);
					res = RES_OK;
				}
			}
		} else {					/* SDC ver 1.XX or MMC */
			if ((send_cmd(CMD9, 0) == 0))
			{
				if (rcvr_datablock(csd, 16))
				{
					/* Read CSD */
					if (CardType & CT_SD1)
					{	/* SDC ver 1.XX */
						* (uint32_t*) buff = (((csd[10] & 63) << 1) + ((uint16_t)(csd[11] & 128) >> 7) + 1) << ((csd[13] >> 6) - 1);
					}
					else
					{
						/* MMC */
						*(uint32_t*)buff = ((uint16_t)((csd[10] & 124) >> 2) + 1) * (((csd[11] & 3) << 3) + ((csd[11] & 224) >> 5) + 1);
					}
				}
				res = RES_OK;
			}
		}
		break;

	case CTRL_ERASE_SECTOR :	/* Erase a block of sectors (used when _USE_ERASE == 1) */
		if (!(CardType & CT_SDC)) break;				/* Check if the card is SDC */
		if (disk_ioctl(drv, MMC_GET_CSD, csd)) break;	/* Get CSD */
		if (!(csd[0] >> 6) && !(csd[10] & 0x40)) break;	/* Check if sector erase can be applied to the card */
		dp = buff; st = dp[0]; ed = dp[1];				/* Load sector block */
		if (!(CardType & CT_BLOCK)) {
			st *= 512; ed *= 512;
		}
		if (send_cmd(CMD32, st) == 0)
		{
			if (send_cmd(CMD33, ed) == 0)
				if (send_cmd(CMD38, 0) == 0)
					if (wait_ready(30))
						; /* Erase sector block */
		}
		res = RES_OK;	/* FatFs does not check result of this command */
		break;

	/* Following command are not used by FatFs module */

	case MMC_GET_TYPE :		/* Get MMC/SDC type (BYTE) */
		*ptr = CardType;
		res = RES_OK;
		break;

	case MMC_GET_CSD :		/* Read CSD (16 bytes) */
		if (send_cmd(CMD9, 0) == 0		/* READ_CSD */
			&& rcvr_datablock(ptr, 16))
			res = RES_OK;
		break;

	case MMC_GET_CID :		/* Read CID (16 bytes) */
		if (send_cmd(CMD10, 0) == 0		/* READ_CID */
			&& rcvr_datablock(ptr, 16))
			res = RES_OK;
		break;

	case MMC_GET_OCR :		/* Read OCR (4 bytes) */
		if (send_cmd(CMD58, 0) == 0) {	/* READ_OCR */
			for (n = 4; n; n--) *ptr++ = xchg_spi(0xFF, 1);
			res = RES_OK;
		}
		break;

	case MMC_GET_SDSTAT :	/* Read SD status (64 bytes) */
		if (send_cmd(ACMD13, 0) == 0) {	/* SD_STATUS */
			xchg_spi(0xFF, 1);
			if (rcvr_datablock(ptr, 64))
				res = RES_OK;
		}
		break;

	default:
		res = RES_PARERR;
		break;
	}

	deselect();

	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* Device timer function                                                 */
/*-----------------------------------------------------------------------*/
/* This function must be called from timer interrupt routine in period
/  of 1 ms to generate card control timing.
*/

#ifdef _NOT_USED_
void disk_timerproc (void)
{
	uint8_t s;

	s = Stat;
	if (WP)		/* Write protected */
		s |= STA_PROTECT;
	else		/* Write enabled */
		s &= ~STA_PROTECT;
	//if (INS)	/* Card is in socket */
		s &= ~STA_NODISK;
	//else		/* Socket empty */
	//	s |= (STA_NODISK | STA_NOINIT);
	Stat = s;
}
#endif /* _NOT_USED_ */
