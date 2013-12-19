/*
 * dspi.c
 *
 * Coldfire DSPI (DMA Serial Peripherial Interface).
 *
 * On the Coldfire, the DSPI interface supports 4 SPI output channels and one input channel.
 * On the Firebee, DSPICS5 is connected to the SD card slot.
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
 *  Created on: 16.10.2013
 *      Author: mfro
 */

#include <stdint.h>
#include <bas_types.h>
#include <MCF5475.h>

struct baudrate
{
	int br_divisor;
	int pbr_divisor;
	int	divider;
};

static const int system_clock = 133000000;	/* System clock in Hz */

struct baudrate baudrates[] =
{
	{ 0b0000, 0b00,	4 },
	{ 0b0000, 0b01,	6 },
	{ 0b0001, 0b00,	8 },
	{ 0b0000, 0b10,	10 },
	{ 0b0001, 0b01,	12 },
	{ 0b0010, 0b00,	12 },
	{ 0b0000, 0b11,	14 },
	{ 0b0011, 0b00,	16 },
	{ 0b0010, 0b01,	18 },
	{ 0b0001, 0b10,	20 },
	{ 0b0011, 0b01,	24 },
	{ 0b0001, 0b11,	28 },
	{ 0b0010, 0b10,	30 },
	{ 0b0100, 0b00,	32 },
	{ 0b0011, 0b10,	40 },
	{ 0b0010, 0b11,	42 },
	{ 0b0100, 0b01,	48 },
	{ 0b0011, 0b11,	56 },
	{ 0b0101, 0b00,	64 },
	{ 0b0100, 0b10,	80 },
	{ 0b0101, 0b01,	96 },
	{ 0b0100, 0b11,	112 },
	{ 0b0110, 0b00,	128 },
	{ 0b0101, 0b10,	160 },
	{ 0b0110, 0b01,	192 },
	{ 0b0101, 0b11,	224 },
	{ 0b0111, 0b00,	256 },
	{ 0b0110, 0b10,	320 },
	{ 0b0111, 0b01,	384 },
	{ 0b0110, 0b11,	448 },
	{ 0b1000, 0b00,	512 },
	{ 0b0111, 0b10,	640 },
	{ 0b1000, 0b01,	768 },
	{ 0b0111, 0b11,	896 },
	{ 0b1001, 0b00,	1024 },
	{ 0b1000, 0b10,	1280 },
	{ 0b1001, 0b01,	1536 },
	{ 0b1000, 0b11,	1792 },
	{ 0b1010, 0b00,	2048 },
	{ 0b1001, 0b10,	2560 },
	{ 0b1010, 0b01,	3072 },
	{ 0b1001, 0b11,	3584 },
	{ 0b1011, 0b00,	4096 },
	{ 0b1010, 0b10,	5120 },
	{ 0b1011, 0b01,	6144 },
	{ 0b1010, 0b11,	7168 },
	{ 0b1100, 0b00,	8192 },
	{ 0b1011, 0b10,	10240 },
	{ 0b1100, 0b01,	12288 },
	{ 0b1011, 0b11,	14336 },
	{ 0b1101, 0b00,	16384 },
	{ 0b1100, 0b10,	20480 },
	{ 0b1101, 0b01,	24576 },
	{ 0b1100, 0b11,	28672 },
	{ 0b1110, 0b00,	32768 },
	{ 0b1101, 0b10,	40960 },
	{ 0b1110, 0b01,	49152 },
	{ 0b1101, 0b11,	57344 },
	{ 0b1111, 0b00,	65536 },
	{ 0b1110, 0b10,	81920 },
	{ 0b1111, 0b01,	98304 },
	{ 0b1110, 0b11,	114688 },
	{ 0b1111, 0b10,	163840 },
	{ 0b1111, 0b11,	229376 },
};

/*
 * set the dspi clock to rate or - if no exact match possible - to the next lower possible baudrate
 */
int dspi_set_baudrate(int rate)
{
	int set_baudrate = 0;
	int br;
	int pbr;
	int i;

	for (i = sizeof(baudrates) / sizeof(struct baudrate) - 1; i >= 0; i--)
	{
		set_baudrate = system_clock / baudrates[i].divider;

		if (set_baudrate > rate)
		{
			continue;
		}
		br = baudrates[i].br_divisor;
		pbr = baudrates[i].pbr_divisor;

		/* TODO: set br and pbr here */

		return set_baudrate;
	}

	return 0;
}

static uint32_t dspi_fifo_val = MCF_DSPI_DTFR_CTCNT;

/*
 * Exchange a byte. If last is false (0), there will be more bytes to follow (EOQ flag in DTFR left unset)
 */
uint8_t dspi_xchg_byte(int device, uint8_t byte, int last)
{
	uint32_t fifo;
	uint8_t res;

	fifo =  dspi_fifo_val | (byte & 0xff);			/* transfer bytes only */
	fifo |= (last ? MCF_DSPI_DTFR_EOQ : 0);			/* mark last transfer */
	MCF_DSPI_DTFR = fifo;
	while (! (MCF_DSPI_DSR & MCF_DSPI_DSR_TCF));	/* wait until DSPI transfer complete */
	fifo = MCF_DSPI_DRFR;							/* read transferred word */

	MCF_DSPI_DSR = -1;		 						/* clear DSPI status register */

	res = fifo & 0xff;

	return res;
}

/* Receive multiple byte with 0xff as output
 *
 * buff: pointer to data buffer
 * btr: number of bytes to receive (16, 64 or 512)
 */
static void dspi_rcv_byte_multi(int device, uint8_t *buff, uint32_t count)
{
	int i;

	for (i = 0; i < count - 1; i++)
		*buff++ = dspi_xchg_byte(device, 0xff, 0);
	*buff++ = dspi_xchg_byte(device, 0xff, 1); 		/* transfer last byte and stop transmission */
}

/* Send multiple byte, discard input
 *
 * buff: pointer to data
 * btx: number of bytes to send
 */
static void dspi_xmt_byte_multi(int device, const uint8_t *buff, uint32_t btx)
{
	int i;

	for (i = 0; i < btx - 1; i++)
		dspi_xchg_byte(device, *buff++, 0);
	dspi_xchg_byte(device, *buff++, 1);				/* transfer last byte and indicate end of transmission */
}

