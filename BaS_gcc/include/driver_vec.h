/*
 * driver_vec.h
 *
 * Interface for exposure of BaS drivers to the OS
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
 *  Created on: 24.10.2013
 *      Author: Markus Fröschle
 */

#ifndef _DRIVER_VEC_H_
#define _DRIVER_VEC_H_

#include "xhdi_sd.h"
#include "MCD_dma.h"

enum driver_type
{
	END_OF_DRIVERS,		/* marks end of driver list */
	BLOCKDEV_DRIVER,
	CHARDEV_DRIVER,
	VIDEO_DRIVER,
	XHDI_DRIVER,
	MCD_DRIVER,
};

struct generic_driver_interface
{
	uint32_t (*init)(void);
	uint32_t (*read)(void *buf, size_t count);
	uint32_t (*write)(const void *buf, size_t count);
	uint32_t (*ioctl)(uint32_t request, ...);
};

struct dma_driver_interface
{
	int32_t version;
	int32_t magic;
	int32_t (*dma_set_initiator)(int initiator);
	uint32_t (*dma_get_initiator)(int requestor);
	void (*dma_free_initiator)(int requestor);
	int32_t (*dma_set_channel)(int requestor, void (*handler)(void));
	int (*dma_get_channel)(int requestor);
	void (*dma_free_channel)(int requestor);
	void (*dma_clear_channel)(int channel);
	int (*MCD_startDma)(int channel, int8_t *srcAddr, int16_t srcIncr, int8_t *destAddr, int16_t destIncr,
						uint32_t dmaSize, uint32_t xferSize, uint32_t initiator, int32_t priority, uint32_t flags,
						uint32_t funcDesc);
	int (*MCD_dmaStatus)(int channel);
	int (*MCD_XferProgrQuery)(int channel, MCD_XferProg *progRep);
	int (*MCD_killDma)(int channel);
	int (*MCD_continDma)(int channel);
	int (*MCD_pauseDma)(int channel);
	int (*MCD_resumeDma)(int channel);
	int (*MCD_csumQuery)(int channel, uint32_t *csum);
	void *(*dma_malloc)(long amount);
	int (*dma_free)(void *addr);
};
	
struct xhdi_driver_interface
{
	uint32_t (*xhdivec)();
};

union interface
{
	struct generic_driver_interface *gdi;
	struct xhdi_driver_interface *xhdi;
	struct dma_driver_interface *dma;
};

struct generic_interface
{
	enum driver_type type;
	char name[16];
	char description[64];
	int version;
	int revision;
	union interface interface;
};

struct driver_table
{
	uint32_t bas_version;
	uint32_t bas_revision;
	uint32_t (*remove_handler)();			/* calling this will disable the BaS' hook into trap #0 */
	struct generic_interface *interfaces[];
};


#endif /* _DRIVER_VEC_H_ */
