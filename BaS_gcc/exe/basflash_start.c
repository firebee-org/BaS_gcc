/*
 * basflash_start.c
 *
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
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */

#include <bas_types.h>

#define STACKSIZE   16384
static uint32_t ownstack[STACKSIZE];
static uint32_t *stackptr = &ownstack[STACKSIZE - 1];

/*
 * setup our own stack in SDRAM to prevent clashing BaS's in SRAM (size limited).
 */
void startup(void)
{
	static uint32_t oldstack;

	void basflash(void);
	__asm__ __volatile__("move.l	sp,%0\n\t" : "=g"(oldstack) : :);
	__asm__ __volatile__("move.l	%0,sp\n\t" : : "g"(stackptr) : );
	basflash();
	__asm__ __volatile__("move.l	%0,sp\n\t" : : "g"(oldstack) : "sp");
	(void) stackptr; /* make compiler happy about unused variables */
}
