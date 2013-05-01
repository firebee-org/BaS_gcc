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

	switch(xhdi_fun)
	{
	case 0:
		return xhdi_version();
		break;

	case 1:
	{
		uint16_t major;
		uint16_t minor;
		uint32_t block_size;
		uint32_t flags;
		char *product_name;

		major = va_arg(arguments, unsigned int);
		minor = va_arg(arguments, unsigned int);
		block_size = va_arg(arguments, uint32_t);
		flags = va_arg(arguments, uint32_t);
		product_name = va_arg(arguments, char *);
		return xhdi_inquire_target((uint32_t) major, (uint32_t) minor, block_size, flags,
				product_name);
	}
		break;

#ifdef test
	case 2:

		return xhdi_reserve();
		break;

	case 3:
		return xhdi_lock();
		break;

	case 4:
		return xhdi_stop();
		break;

	case 5:
		return xhdi_eject();
		break;

	case 6:
		return xhdi_drivemap();
		break;

	case 7:
		return xhdi_inquire_device();
		break;

	case 8:
		return xhdi_inquire_driver();
		break;

	case 9:
		return xhdi_new_cookie();
		break;

	case 10:
		return xhdi_read_write();
		break;

	case 11:
		return xhdi_inquire_target2();
		break;

	case 12:
		return xhdi_inquire_device2();
		break;

	case 13:
		return xhdi_driver_special();
		break;

	case 14:
		return xhdi_get_capacity();
		break;

	case 15:
		return xhdi_medium_changed();
		break;

	case 16:
		return xhdi_mint_info();
		break;

	case 17:
		return xhdi_dos_limits();
		break;

	case 18:
		return xhdi_last_access();
		break;
#endif
	default:
		;
	}
	xprintf("unknown XHDI function %d\r\n");
	return -32;
}
