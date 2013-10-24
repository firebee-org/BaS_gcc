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

static struct generic_interface interfaces[] =
{
	{
		/* BaS SD-card driver interface */

		.type = XHDI_DRIVER,
		.name = "SDCARD",
		.description = "BaS SD Card driver",
		.version = 0,
		.revision = 1,
		.interface.xhdi = { xhdi_call }
	},
	/* insert new drivers here */
	{
		.type = END_OF_DRIVERS
	}
};

/*
 * this is the driver table we expose to the OS
 */
static struct driver_table drivers =
{
	.bas_version = MAJOR_VERSION,
	.bas_revision = MINOR_VERSION,
	.remove_handler = NULL,
	.interfaces = { interfaces }
};

