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

typedef enum driver_type
{
	blockdev,
	chardev,
	video,
	xhdi
} DRIVER_TYPE;

typedef struct generic_driver_interface
{
	int (*read)();
	int (*write)();
	int (*ioctl)();
} GENERIC_DRIVER_INTERFACE;

typedef struct xhdi_driver_interface
{
	long xhdivec;
} XHDI_DRIVER_INTERFACE;

typedef union driver_interface
{
	enum driver_type type;
	struct generic_driver_interface gdi;
	struct xhdi_driver_interface xhdi;
} DRIVER_INTERFACE;

#endif /* _DRIVER_VEC_H_ */
