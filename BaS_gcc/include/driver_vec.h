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

enum driver_type
{
	END_OF_DRIVERS,		/* marks end of driver list */
	BLOCKDEV_DRIVER,
	CHARDEV_DRIVER,
	VIDEO_DRIVER,
	XHDI_DRIVER,
};

struct generic_driver_interface
{
	uint32_t (*init)();
	uint32_t (*read)();
	uint32_t (*write)();
	uint32_t (*ioctl)();
};

struct xhdi_driver_interface
{
	uint32_t (*xhdivec)();
};

union driver_interface
{
	struct generic_driver_interface gdi;
	struct xhdi_driver_interface xhdi;
};

struct interface
{
	enum driver_type type;
	char name[16];
	char description[64];
	int version;
	int revision;
	union driver_interface interface;
};


#endif /* _DRIVER_VEC_H_ */
