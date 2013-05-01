/*
 * xhdi_sd.c
 *
 *  Created on: 01.05.2013
 *      Author: mfro
 */

#include <stdint.h>
#include <stdarg.h>

#include "xhdi_sd.h"
#include "bas_printf.h"

uint32_t xhdi_call(int xhdi_fun, ...)
{
	va_list arguments;
	va_start(arguments, xhdi_fun);

	switch (xhdi_fun)
	{
	case XHDI_VERSION:
		return xhdi_version();
		break;

	case XHDI_INQUIRE_TARGET:
	{
		uint16_t major;
		uint16_t minor;
		uint32_t *block_size;
		uint32_t *flags;
		char *product_name;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		block_size = va_arg(arguments, uint32_t *);
		flags = va_arg(arguments, uint32_t *);
		product_name = va_arg(arguments, char *);
		return xhdi_inquire_target(major, minor,
				block_size, flags, product_name);
	}
		break;

	case XHDI_RESERVE:
	{
		uint16_t major;
		uint16_t minor;
		uint16_t do_reserve;
		uint16_t key;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		do_reserve = va_arg(arguments, unsigned int);
		key = va_arg(arguments, unsigned int);

		return xhdi_reserve(major, minor, do_reserve, key);
	}
		break;

	case XHDI_LOCK:
	{
		uint16_t major;
		uint16_t minor;
		uint16_t do_lock;
		uint16_t key;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		do_lock = va_arg(arguments, unsigned int);
		key = va_arg(arguments, unsigned int);

		return xhdi_lock(major, minor, do_lock, key);
	}
		break;

	case XHDI_STOP:
	{
		uint16_t major;
		uint16_t minor;
		uint16_t do_stop;
		uint16_t key;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		do_stop = va_arg(arguments, unsigned int);
		key = va_arg(arguments, unsigned int);

		return xhdi_stop(major, minor, do_stop, key);
	}
		break;

	case XHDI_EJECT:
	{
		uint16_t major;
		uint16_t minor;
		uint16_t do_eject;
		uint16_t key;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		do_eject = va_arg(arguments, unsigned int);
		key = va_arg(arguments, unsigned int);

		return xhdi_eject(major, minor, do_eject, key);
	}
		break;

	case XHDI_DRIVEMAP:
		return xhdi_drivemap();
		break;

	case XHDI_INQUIRE_DEVICE:
	{
		uint16_t bios_device;
		uint16_t *major;
		uint16_t *minor;
		uint32_t *start_sector;
		void *bpb;

		bios_device = va_arg(arguments, unsigned int);
		major = va_arg(arguments, uint16_t *);
		minor = va_arg(arguments, uint16_t *);
		start_sector = va_arg(arguments, uint32_t *);
		bpb = va_arg(arguments, void *);

		return xhdi_inquire_device(bios_device, major, minor, start_sector, bpb);
	}
		break;

	case XHDI_INQUIRE_DRIVER:
	{
		uint16_t bios_device;
		char *name;
		char *version;
		char *company;
		uint16_t *ahdi_version;
		uint16_t *maxIPL;

		bios_device = va_arg(arguments, unsigned int);
		name = va_arg(arguments, char *);
		version = va_arg(arguments, char *);
		company = va_arg(arguments, char *);
		ahdi_version = va_arg(arguments, uint16_t *);
		maxIPL = va_arg(arguments, uint16_t *);

		return xhdi_inquire_driver(bios_device, name, version, company,
				ahdi_version, maxIPL);
	}
		break;

	case XHDI_NEW_COOKIE:
	{
		void *new_cookie;

		new_cookie = va_arg(arguments, void *);

		return xhdi_new_cookie(new_cookie);
	}
		break;

	case XHDI_READ_WRITE:
	{
		uint16_t major;
		uint16_t minor;
		uint16_t rwflag;
		uint32_t recno;
		uint16_t count;
		void *buf;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		rwflag = va_arg(arguments, unsigned int);
		recno = va_arg(arguments, uint32_t);
		count = va_arg(arguments, unsigned int);
		buf = va_arg(arguments, void *);

		return xhdi_read_write(major, minor, rwflag, recno, count, buf);
	}
		break;

	case XHDI_INQUIRE_TARGET2:
	{
		uint16_t major;
		uint16_t minor;
		uint32_t *block_size;
		uint32_t *device_flags;
		char *product_name;
		uint16_t strlen;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		block_size = va_arg(arguments, uint32_t *);
		device_flags = va_arg(arguments, uint32_t *);
		product_name = va_arg(arguments, char *);
		strlen = va_arg(arguments, unsigned int);

		return xhdi_inquire_target2(major, minor, block_size, device_flags,
				product_name, strlen);
	}
		break;

	case XHDI_INQUIRE_DEVICE2:
	{
		uint16_t bios_device;
		uint16_t *major;
		uint16_t *minor;
		uint16_t *start_sector;
		void *bpb;
		uint32_t *blocks;
		char *partid;

		bios_device = va_arg(arguments, unsigned int);
		major = va_arg(arguments, uint16_t *);
		minor = va_arg(arguments, uint16_t *);
		start_sector = va_arg(arguments, uint16_t *);
		bpb = va_arg(arguments, void *);
		blocks = va_arg(arguments, uint32_t *);
		partid = va_arg(arguments, char *);

		return xhdi_inquire_device2(bios_device, major, minor, start_sector,
				bpb, blocks, partid);
	}
		break;

	case XHDI_DRIVER_SPECIAL:
	{
		uint32_t key1;
		uint32_t key2;
		uint16_t subopcode;
		void *data;

		key1 = va_arg(arguments, uint32_t);
		key2 = va_arg(arguments, uint32_t);
		subopcode = va_arg(arguments, unsigned int);
		data = va_arg(arguments, void *);

		return xhdi_driver_special(key1, key2, subopcode, data);
	}
		break;

	case XHDI_GET_CAPACITY:
	{
		uint16_t major;
		uint16_t minor;
		uint32_t *blocks;
		uint32_t *bs;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		blocks = va_arg(arguments, uint32_t *);
		bs = va_arg(arguments, uint32_t *);

		return xhdi_get_capacity(major, minor, blocks, bs);
	}
		break;

	case XHDI_MEDIUM_CHANGED:
	{
		uint16_t major;
		uint16_t minor;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		return xhdi_medium_changed(major, minor);
	}
		break;

	case XHDI_MINT_INFO:
	{
		uint16_t opcode;
		void *data;

		opcode = va_arg(arguments, unsigned int);
		data = va_arg(arguments, void *);

		return xhdi_mint_info(opcode, data);
	}
		break;

	case XHDI_DOS_LIMITS:
	{
		uint16_t which;
		uint32_t limit;

		which = va_arg(arguments, unsigned int);
		limit = va_arg(arguments, uint32_t);

		return xhdi_dos_limits(which, limit);
	}
		break;

	case XHDI_LAST_ACCESS:
	{
		uint16_t major;
		uint16_t minor;
		uint32_t *ms;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		ms = va_arg(arguments, uint32_t *);

		return xhdi_last_access(major, minor, ms);
	}
		break;

	default:
		;
	}
	xprintf("unknown XHDI function %d\r\n");
	return EINVFN;
}
