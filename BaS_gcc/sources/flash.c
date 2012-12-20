/*
 * flash.c
 *
 * flashing routines for BaS_gcc
 *
 *  Created on: 19.12.2012
 *      Author: mfro
 *      The ACP Firebee project
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
 * Copyright 2012        M. Froeschle
 */

#include <stdint.h>

void chip_erase(uint8_t *address)
{
	* (volatile uint8_t *) (address + 0xaaa) = 0xaa;
	* (volatile uint8_t *) (address + 0x555) = 0x55;
	* (volatile uint8_t *) (address + 0xaaa) = 0x80;
	* (volatile uint8_t *) (address + 0xaaa) = 0xaa;
	* (volatile uint8_t *) (address + 0x555) = 0x55;
	* (volatile uint8_t *) (address + 0xaaa) = 0x10;
}

void sector_erase(uint8_t *address, uint16_t sector)
{
	* (volatile uint8_t *) (address + 0xaaa) = 0xaa;
	* (volatile uint8_t *) (address + 0x555) = 0x55;
	* (volatile uint8_t *) (address + 0xaaa) = 0x80;
	* (volatile uint8_t *) (address + 0xaaa) = 0xaa;
	* (volatile uint8_t *) (address + 0x555) = 0x55;
	* (volatile uint8_t *) (address + sector) = 0x30;

	do {
		;
	} while (* (volatile uint32_t *) (address + sector) != 0xffffffff);
}

#ifdef _NOT_USED_
* MX28LV640DB.alg
	bra	lab0x5a
	move.w	#0xaa,d0
	move.w	d0,0xaaa(a5)
	move.w	#0xff,d0
	move.w	d0,0x554(a5)
	move.w	#0x20,d0
	move.w	d0,0xaaa(a5)
lab0x1c:
	move.w	#0xa0,d0
	move.w	d0,(a5)
	moveq 	#0,d0
	move.w	(a3),d0
	move.w	d0,(a4)
lab0x28:
	move.w	(a4),d1
	andi.l	#0xffff,d1
	cmp.l	d1,d0
	bne.s	lab0x28
	adda.l	#2,a3
	adda.l	#2,a4
	cmpa.l	a2,a3
	blt.s	lab0x1c
	move.w	#0x90,d0
	move.w	d0,(a5)
	move.w	#0,d0
	move.w	d0,(a5)
lab0x50:
	nop
	nop
	halt
	nop
	bra.s	lab0x50
lab0x5a:
	move.w	#0xaa,d0
	move.w	d0,0xaaa(a5)
	move.w	#0x55,d0
	move.w	d0,0x554(a5)
	move.w	#0x80,d0
	move.w	d0,0xaaa(a5)
	move.w	#0xaa,d0
	move.w	d0,0xaaa(a5)
	move.w	#0x55,d0
	move.w	d0,0x554(a5)
	move.w	#0x30,d0
	move.w	d0,(a4)
lab0x88:
	move.l	(a4),d1
	move.l	#-1,d0
	cmp.l	d1,d0
	bne.s	lab0x88
	nop
	nop
	halt
	nop
#endif /* _NOT_USED_ */
