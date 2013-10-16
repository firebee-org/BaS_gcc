/*
 * xhdi_sd.c
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
 *  Created on: 01.05.2013
 *  Copyright 2012 M. Fröschle
 */

#include <stdint.h>
#include <stddef.h>

#include "xhdi_sd.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "diskio.h"

#define DRIVER_VERSION	0x130

#define MY_MAJOR	7
#define MY_MINOR 	0

uint16_t xhdi_version(void)
{
	return DRIVER_VERSION;
}

uint32_t xhdi_inquire_target(uint16_t major, uint16_t minor, uint32_t *block_size, uint32_t *flags,
		char *product_name)
{
	return xhdi_inquire_target2(major, minor, block_size, flags, product_name, 33);
}

uint32_t xhdi_reserve(uint16_t major, uint16_t minor, uint16_t do_reserve, uint16_t key)
{
	if (major == MY_MAJOR)
		return ERROR;	/* device cannot be reserved */

	return EUNDEV;
}

uint32_t xhdi_lock(uint16_t major, uint16_t minor, uint16_t do_lock, uint16_t key)
{
	if (major == MY_MAJOR)
		return ERROR;	/* device cannot be locked */

	return EUNDEV;
}

uint32_t xhdi_stop(uint16_t major, uint16_t minor, uint16_t do_stop, uint16_t key)
{
	if (major == MY_MAJOR)
		return ERROR;	/* device cannot be locked */

	return EUNDEV;
}

uint32_t xhdi_eject(uint16_t major, uint16_t minor, uint16_t do_eject, uint16_t key)
{
	if (major == MY_MAJOR)
		return ERROR;	/* device cannot be ejected */

	return EUNDEV;
}

uint32_t xhdi_drivemap(void)
{
	long map = (1 << ('S' - 'A'));

	return map;
}

uint32_t xhdi_inquire_device(uint16_t bios_device, uint16_t *major, uint16_t *minor,
        uint32_t *start_sector, /* BPB */ void *bpb)
{
	if (major != NULL) *major = MY_MAJOR;
	if (minor != NULL) *minor = MY_MINOR;
	if (start_sector != NULL) *start_sector = 0;

	return E_OK;
}

uint32_t xhdi_inquire_driver(uint16_t bios_device, char *name, char *version,
		char *company, uint16_t *ahdi_version, uint16_t *maxIPL)
{
	if (bios_device == 'S' - 'A')
	{
		if (name != NULL) strcpy(name, "BaS SD-card driver");
		if (version != NULL) strcpy(version, "0.1");
		if (company != NULL) strcpy(company, "Markus Fröschle");
		if (ahdi_version != NULL) *ahdi_version = 300;
		if (maxIPL != NULL) *maxIPL = 7;

		return E_OK;
	}
	return EUNDEV;
}

uint32_t xhdi_new_cookie(uint32_t newcookie)
{
	return EUNDEV;
}

uint32_t xhdi_read_write(uint16_t major, uint16_t minor, uint16_t rwflag,
        uint32_t recno, uint16_t count, void *buf)
{
	int ret;
	uint16_t num_sectors;
	int16_t s_count = count;
	uint16_t retries;
	const uint16_t max_retries = 3;

	if (major == MY_MAJOR)
	{
		do {
			num_sectors = ((s_count > 63) ? 63 : s_count);

			retries = 0;
			do {
				ret = ((rwflag & 1) ? disk_write(0, buf, recno, num_sectors) : disk_read(0, buf, recno, num_sectors));
				if (ret != RES_OK)
				{
					disk_reset(0);
					retries++;
					if (retries < max_retries) continue;

					xprintf("SD card %s error at sector %lx: %d\r\n", (rwflag & 1) ? "write" : "read", recno, ret);
					return ERROR;
				}
			} while (retries < max_retries && ret != RES_OK);

			buf += num_sectors * 512;
			recno += num_sectors;
			s_count -= num_sectors;
		} while (s_count > 0);

		return E_OK;
	}
	return EUNDEV;
}

uint32_t xhdi_inquire_target2(uint16_t major, uint16_t minor, uint32_t *block_size,
        uint32_t *device_flags, char *product_name, uint16_t stringlen)
{
	if (major == MY_MAJOR)
	{
		if (block_size != NULL) *block_size = 512;
		if (device_flags != NULL) *device_flags = XH_TARGET_REMOVABLE;
		if (product_name != NULL) strncpy(product_name, "BaS SD driver", stringlen);

		return E_OK;

	}
	return EUNDEV;
}

uint32_t xhdi_inquire_device2(uint16_t bios_device, uint16_t *major, uint16_t *minor,
        uint32_t *start_sector, BPB *bpb, uint32_t *blocks, char *partid)
{

	if (bios_device == 'S' - 'A')
	{
		return E_OK;
	}
	return EUNDEV;
}

uint32_t xhdi_driver_special(uint32_t key1, uint32_t key2, uint16_t subopcode, void *data)
{
	return EUNDEV;
}

uint32_t xhdi_get_capacity(uint16_t major, uint16_t minor, uint32_t *blocks, uint32_t *bs)
{
	if (major == MY_MAJOR)
	{
		if (blocks != 0)
		{
			if (disk_ioctl(0, GET_SECTOR_COUNT, blocks) != RES_OK)
			{
				xprintf("disk_ioctl(0, GET_SECTOR_COUNT, %p) failed at %s:%d\r\n", blocks, __FILE__, __LINE__);

				return ERROR;
			}
			xprintf("%s:%d: determined sector count to %ld\r\n", __FILE__, __LINE__, *blocks);
		}

		if (bs != 0)
		{
			if (disk_ioctl(0, GET_SECTOR_SIZE, bs) != RES_OK)
			{
				xprintf("disk_ioctl(0, GET_SECTOR_SIZE, %p) failed at %s:%dr\n", bs, __FILE__, __LINE__);

				return ERROR;
			}
			xprintf("%s:%d: determined sector size to %ld\r\n", __FILE__, __LINE__, *bs);
		}
		return E_OK;
	}
	return EUNDEV;
}

uint32_t xhdi_medium_changed(uint16_t major, uint16_t minor)
{
	if (major == MY_MAJOR)
		return 1;	/* may have changed */

	return EUNDEV;
}

uint32_t xhdi_mint_info(uint16_t opcode, void *data)
{
	return EUNDEV;
}

uint32_t xhdi_dos_limits(uint16_t which, uint32_t limit)
{
	return EUNDEV;
}

uint32_t xhdi_last_access(uint16_t major, uint16_t minor, uint32_t *ms)
{
	return EUNDEV;
}

uint32_t xhdi_reaccess(uint16_t major, uint16_t minor)
{
	return EUNDEV;
}
