/*
 * driver_vec.c
 *
 * Expose BaS drivers to OS
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
 *
 *  Created on: 24.10.2013
 *      Author: Markus Fröschle
 */

#include <stdint.h>
#include <stddef.h>
#include "driver_vec.h"
#include "version.h"
#include "xhdi_sd.h"
#include "dma.h"
#include "driver_mem.h"

/*
 * driver interface struct for the SD card BaS driver
 */
static struct xhdi_driver_interface xhdi_call_interface = 
{
	xhdi_call
};

/*
 * driver interface struct for the BaS multichannel DMA driver
 * This is exactly the same thing FireTOS provides and the MiNT FEC drivers expect.
 * It can be directly used in TOS to register the corresponding cookie ("DMAC").
 */
static struct dma_driver_interface dma_interface =
{
	.version = 0x0101,
	.magic = 'DMAC',
	.dma_set_initiator = &dma_set_initiator,
	.dma_get_initiator = dma_get_initiator,
	.dma_free_initiator = dma_free_initiator,
	.dma_set_channel = dma_set_channel,
	.dma_get_channel = dma_get_channel,
	.dma_free_channel = dma_free_channel,
	.dma_clear_channel = dma_clear_channel,
	.MCD_startDma = MCD_startDma,
	.MCD_dmaStatus = MCD_dmaStatus,
	.MCD_XferProgrQuery = MCD_XferProgrQuery,
	.MCD_killDma = MCD_killDma,
	.MCD_continDma = MCD_continDma,
	.MCD_pauseDma = MCD_pauseDma,
	.MCD_resumeDma = MCD_resumeDma,
	.MCD_csumQuery = MCD_csumQuery,
	.dma_malloc = driver_mem_alloc,
	.dma_free = driver_mem_free
};

static struct generic_interface interfaces[] =
{
	{
		/* BaS SD-card driver interface */

		.type = XHDI_DRIVER,
		.name = "SDCARD",
		.description = "BaS SD Card driver",
		.version = 0,
		.revision = 1,
		.interface.xhdi = &xhdi_call_interface 
	},
	{
		.type = MCD_DRIVER,
		.name = "MCDDMA",
		.description = "BaS Multichannel DMA driver",
		.version = 0,
		.revision = 1,
		.interface.dma = &dma_interface,
	},

	/* insert new drivers here */

	{
		.type = END_OF_DRIVERS
	}
};

/*
 * this is the driver table we expose to the OS
 */
static struct driver_table bas_drivers =
{
	.bas_version = MAJOR_VERSION,
	.bas_revision = MINOR_VERSION,
	.remove_handler = NULL,
	.interfaces = { interfaces }
};

void __attribute__((interrupt)) get_bas_drivers(void)
{
	__asm__ __volatile__(
		"move.l	#%[drivers],d0\n\t"
		:	                            /* no output */
		:	[drivers] "o" (bas_drivers)		/* input */
		:									/* clobber */
	);
}
