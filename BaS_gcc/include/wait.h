/*
 * wait.h
 *
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

#ifndef _WAIT_H_
#define _WAIT_H_

#include <bas_types.h>

typedef uint32_t (*checker_func)(void);

extern __inline__ void wait(uint32_t) __attribute__((always_inline));
extern __inline__ uint32_t waitfor(uint32_t us, checker_func condition) __attribute__((always_inline));
/*
 * wait for the specified number of us on slice timer 0. Replaces the original routines that had
 * the number of useconds to wait for hardcoded in their name.
 */
extern __inline__ void wait(uint32_t us)
{
	uint32_t target = MCF_SLT_SCNT(0) - (us * 132);

	while (MCF_SLT_SCNT(0) > target);
}

/*
 * the same as above, with a checker function which gets called while
 * busy waiting and allows for an early return if it returns true
 */
extern __inline__ uint32_t waitfor(uint32_t us, checker_func condition)
{
	uint32_t target = MCF_SLT_SCNT(0) - (us * 132);
	uint32_t res;

	do
	{
		if ((res = (*condition)()))
			return res;
	} while (MCF_SLT_SCNT(0) > target);
	return 0;
}
#endif /* _WAIT_H_ */
