/*
 * wait.c
 *
 *  Created on: 10.12.2012
 *      Author: mfro
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
 * Copyright 2010 - 2012 F. Aschwanden
 * Copyright 2011 - 2012 V. Riviere
 * Copyright 2012        M. Froeschle
 *
 */

#include <stdint.h>
#include <MCF5475.h>
#include <wait.h>

uint32_t get_timer(void)
{
	return MCF_SLT_SCNT(0);
}

/*
 * wait for the specified number of us on slice timer 0. Replaces the original routines that had
 * the number of useconds to wait for hardcoded in their name.
 */
void wait(uint32_t us)
{
	int32_t target = MCF_SLT_SCNT(0) - (us * (SYSCLK / 1000));

	while (MCF_SLT_SCNT(0) - target > 0);
}

/*
 * same as above, but with milliseconds wait time
 */
void wait_ms(uint32_t ms)
{
	wait(ms * 1000);
}
/*
 * wait for the specified number of us (same as above), but with a checker function
 * which gets called while busy waiting and allows for an early return if it returns true
 */
bool waitfor(uint32_t us, checker_func condition)
{
	int32_t target = MCF_SLT_SCNT(0) - (us * (SYSCLK / 1000));
	bool res;

	do
	{
		if ((res = (*condition)()))
			return res;
	} while (MCF_SLT_SCNT(0) - target > 0);

	return false;
}
