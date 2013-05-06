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
 * Created on: 01.05.2013
 * Copyright 2012: M. Fröschle
 */

#include <stdint.h>
#include <stdarg.h>

#include "xhdi_sd.h"
#include "bas_printf.h"


uint32_t xhdi_call(uint16_t *stack)
{
	uint16_t opcode = *stack;

	xprintf("xhdi_call(): opcode %d\r\n", opcode);

	switch (opcode)
	{
	case XHDI_VERSION:
		return xhdi_version();
		break;

	case XHDI_INQUIRE_TARGET:
	{
		struct XHINQTARGET_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint32_t *blocksize;
			uint32_t *deviceflags;
			char *productname;
		}*args = (struct XHINQTARGET_args *) stack;

		return xhdi_inquire_target(args->major, args->minor, args->blocksize,
				args->deviceflags, args->productname);
	}
		break;

	case XHDI_RESERVE:
	{
		struct XHRESERVE_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint16_t do_reserve;
			uint16_t key;
		}*args = (struct XHRESERVE_args *) stack;

		return xhdi_reserve(args->major, args->minor, args->do_reserve, args->key);
	}

	case XHDI_LOCK:
	{
		struct XHLOCK_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint16_t do_lock;
			uint16_t key;
		}*args = (struct XHLOCK_args *) stack;

		return xhdi_lock(args->major, args->minor, args->do_lock, args->key);
	}

	case XHDI_STOP:
	{
		struct XHSTOP_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint16_t do_stop;
			uint16_t key;
		}*args = (struct XHSTOP_args *) stack;

		return xhdi_stop(args->major, args->minor, args->do_stop, args->key);
	}

	case XHDI_EJECT:
	{
		struct XHEJECT_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint16_t do_eject;
			uint16_t key;
		}*args = (struct XHEJECT_args *) stack;

		return xhdi_eject(args->major, args->minor, args->do_eject, args->key);
	}

	case XHDI_DRIVEMAP:
	{
		return xhdi_drivemap();
	}

	case XHDI_INQUIRE_DEVICE:
	{
		struct XHINQDEV_args
		{
			uint16_t opcode;
			uint16_t drv;
			uint16_t *major;
			uint16_t *minor;
			uint32_t *start;
			BPB *bpb;
		}*args = (struct XHINQDEV_args *) stack;

		return xhdi_inquire_device(args->drv, args->major, args->minor, args->start,
				args->bpb);
	}

	case XHDI_INQUIRE_DRIVER:
	{
		struct XHINQDRIVER_args
		{
			uint16_t opcode;
			uint16_t dev;
			char *name;
			char *version;
			char *company;
			uint16_t *ahdi_version;
			uint16_t *maxIPL;
		}*args = (struct XHINQDRIVER_args *) stack;

		return xhdi_inquire_driver(args->dev, args->name, args->version, args->company,
				args->ahdi_version, args->maxIPL);
	}

	case XHDI_NEW_COOKIE:
	{
		struct XHNEWCOOKIE_args
		{
			uint16_t opcode;
			uint32_t newcookie;
		}*args = (struct XHNEWCOOKIE_args *) stack;

		return xhdi_new_cookie(args->newcookie);
	}

	case XHDI_READ_WRITE:
	{
		struct XHREADWRITE_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint16_t rw;
			uint32_t sector;
			uint16_t count;
			void *buf;
		}*args = (struct XHREADWRITE_args *) stack;

		return xhdi_read_write(args->major, args->minor, args->rw, args->sector,
				args->count, args->buf);
	}

	case XHDI_INQUIRE_TARGET2:
	{
		struct XHINQTARGET2_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint32_t *blocksize;
			uint32_t *deviceflags;
			char *productname;
			uint16_t stringlen;
		}*args = (struct XHINQTARGET2_args *) stack;

		return xhdi_inquire_target2(args->major, args->minor, args->blocksize,
				args->deviceflags, args->productname, args->stringlen);
	}

	case XHDI_INQUIRE_DEVICE2:
	{
		struct XHINQDEV2_args
		{
			uint16_t opcode;
			uint16_t drv;
			uint16_t *major;
			uint16_t *minor;
			uint32_t *start;
			BPB *bpb;
			uint32_t *blocks;
			char *partid;
		}*args = (struct XHINQDEV2_args *) stack;

		return xhdi_inquire_device2(args->drv, args->major, args->minor, args->start,
				args->bpb, args->blocks, args->partid);
	}

	case XHDI_DRIVER_SPECIAL:
	{
		struct XHDRIVERSPECIAL_args
		{
			uint16_t opcode;
			uint32_t key1;
			uint32_t key2;
			uint16_t subopcode;
			void *data;
		}*args = (struct XHDRIVERSPECIAL_args *) stack;

		return xhdi_driver_special(args->key1, args->key2, args->subopcode,
				args->data);
	}

	case XHDI_GET_CAPACITY:
	{
		struct XHGETCAPACITY_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint32_t *blocks;
			uint32_t *blocksize;
		}*args = (struct XHGETCAPACITY_args *) stack;

		return xhdi_get_capacity(args->major, args->minor, args->blocks,
				args->blocksize);
	}

	case XHDI_MEDIUM_CHANGED:
	{
		struct XHMEDIUMCHANGED_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
		}*args = (struct XHMEDIUMCHANGED_args *) stack;

		return xhdi_medium_changed(args->major, args->minor);
	}

	case XHDI_MINT_INFO:
	{
		struct XHMINTINFO_args
		{
			uint16_t opcode;
			uint16_t subopcode;
			void *data;
		}*args = (struct XHMINTINFO_args *) stack;

		return xhdi_mint_info(args->subopcode, args->data);
	}

	case XHDI_DOS_LIMITS:
	{
		struct XHDOSLIMITS_args
		{
			uint16_t opcode;
			uint16_t which;
			uint32_t limit;
		}*args = (struct XHDOSLIMITS_args *) stack;

		return xhdi_dos_limits(args->which, args->limit);
	}

	case XHDI_LAST_ACCESS:
	{
		struct XHLASTACCESS_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
			uint32_t *ms;
		}*args = (struct XHLASTACCESS_args *) stack;

		return xhdi_last_access(args->major, args->minor, args->ms);
	}

	case XHDI_REACCESS:
	{
		struct XHREACCESS_args
		{
			uint16_t opcode;
			uint16_t major;
			uint16_t minor;
		}*args = (struct XHREACCESS_args *) stack;

		return xhdi_reaccess(args->major, args->minor);
	}

	default:
	{
		xprintf("unknown XHDI function 0x%x\r\n", opcode);
		return EINVFN;
		break;
	}
	}
}

