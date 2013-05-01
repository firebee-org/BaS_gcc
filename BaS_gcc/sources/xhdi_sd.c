/*
 * xhdi_sd.c
 *
 *  Created on: 01.05.2013
 *      Author: mfro
 */

#include <stdint.h>
#include <stddef.h>

#include "xhdi_sd.h"

#define DRIVER_VERSION	0x130

static xhdi_call_fun old_vector = NULL;

__attribute__((__interrupt__)) xhdi_call_fun xhdi_sd_install(xhdi_call_fun ov)
{
	old_vector = ov;
	long *_drvbits = 0x4c2;

	/* THIS does not work: return (xhdi_call_fun) &xhdi_call; */
	__asm__ __volatile__ (
			"move.l		%[xhdi_call],d1\n\t"
			"move.l		d1,(sp)\n\t"
			: /* output */
			: [xhdi_call]"g"(xhdi_call)
			: "d1","memory");

	*_drvbits |= (1 << ('O' - 'A'));

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
	long map = (1 << ('O' - 'A'));

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
