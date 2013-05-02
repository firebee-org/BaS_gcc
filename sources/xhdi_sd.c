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

#define DRIVER_VERSION	0x130

typedef struct
{
  uint16_t recsiz;	/* bytes per sector                    */
  uint16_t clsiz;	/* sectors per cluster                 */
  uint16_t clsizb;	/* bytes per cluster                   */
  uint16_t rdlen;	/* directory length                    */
  uint16_t fsiz;	/* length of FAT                       */
  uint16_t fatrec;	/* start of 2nd FAT                    */
  uint16_t datrec;	/* 1st free sector                     */
  uint16_t numcl;	/* overall number of clusters          */
  uint16_t bflags;	/* flags                               */
  	  	  	  	  	/* Bit 0: 0 (12-Bit-FAT), 1 16-Bit-FAT */
  	  	  	  	    /* Bit 1: 0 (two FATs), 1 (one FAT)    */
} __attribute__((__packed__)) BPB;

static BPB sd_bpb[4];	/* space for four partitions on SD card */

static xhdi_call_fun old_vector = NULL;

__attribute__((__interrupt__)) xhdi_call_fun xhdi_sd_install(xhdi_call_fun ov)
{
	old_vector = ov;

	/* THIS does not work: return (xhdi_call_fun) &xhdi_call; */
	__asm__ __volatile__ (
			"move.l		%[xhdi_call],d1\n\t"
			"move.l		d1,(sp)\n\t"	/* FIXME: dirty overwrite of saved register on stack with return value */
			: /* output */
			: [xhdi_call]"g"(xhdi_call)
			: "d1","memory");

	return (xhdi_call_fun) xhdi_call;
}

uint32_t xhdi_version(void)
{
	return DRIVER_VERSION;
}

uint32_t xhdi_inquire_target(uint16_t major, uint16_t minor, uint32_t *block_size, uint32_t *flags,
		char *product_name)
{
	if (block_size != NULL)
	{
		*block_size = 512;
	}
	*flags = XH_TARGET_REMOVABLE;	/* indicate that SD card can be removed */

	return E_OK;
}

uint32_t xhdi_reserve(UINT16_T major, UINT16_T minor, UINT16_T do_reserve, UINT16_T key)
{
	return ERROR;	/* device cannot be reserved */
}

uint32_t xhdi_lock(UINT16_T major, UINT16_T minor, UINT16_T do_lock, UINT16_T key)
{
	return ERROR;	/* device cannot be locked */
}

uint32_t xhdi_stop(UINT16_T major, UINT16_T minor, UINT16_T do_stop, UINT16_T key)
{
	return ERROR;	/* device cannot be locked */
}

uint32_t xhdi_eject(UINT16_T major, UINT16_T minor, UINT16_T do_eject, UINT16_T key)
{
	return ERROR;	/* device cannot be ejected */
}

uint32_t xhdi_drivemap(void)
{
	uint32_t map = (1 << ('S' - 'A'));

	return map;
}

uint32_t xhdi_inquire_device(UINT16_T bios_device, UINT16_T *major, UINT16_T *minor,
        uint32_t *start_sector, /* BPB */ void *bpb)
{
	return ERROR;
}

uint32_t xhdi_inquire_driver(UINT16_T bios_device, char *name, char *version,
		char *company, UINT16_T *ahdi_version, UINT16_T *maxIPL)
{
	return ERROR;
}

uint32_t xhdi_new_cookie(void *newcookie)
{
	return ERROR;
}

uint32_t xhdi_read_write(UINT16_T major, UINT16_T minor, UINT16_T rwflag,
        uint32_t recno, UINT16_T count, void *buf)
{
	return ERROR;
}

uint32_t xhdi_inquire_target2(UINT16_T major, UINT16_T minor, uint32_t *block_size,
        uint32_t *device_flags, char *product_name, UINT16_T stringlen)
{
	return ERROR;
}

uint32_t xhdi_inquire_device2(UINT16_T bios_device, UINT16_T *major, UINT16_T *minor,
        UINT16_T *start_sector, /* BPB */ void *bpb, uint32_t *blocks, char *partid)
{
	return ERROR;
}

uint32_t xhdi_driver_special(uint32_t key1, uint32_t key2, UINT16_T subopcode, void *data)
{
	return ERROR;
}

uint32_t xhdi_get_capacity(UINT16_T major, UINT16_T minor, uint32_t *blocks, uint32_t *bs)
{
	return ERROR;
}

uint32_t xhdi_medium_changed(UINT16_T major, UINT16_T minor)
{
	return ERROR;
}

uint32_t xhdi_mint_info(UINT16_T opcode, void *data)
{
	return ERROR;
}

uint32_t xhdi_dos_limits(UINT16_T which, uint32_t limit)
{
	return ERROR;
}

uint32_t xhdi_last_access(UINT16_T major, UINT16_T minor, uint32_t *ms)
{
	return ERROR;
}

uint32_t xhdi_reaccess(UINT16_T major, UINT16_T minor)
{
	return ERROR;
}
