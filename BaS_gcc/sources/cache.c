/*
 * cache handling
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

#include "cache.h"

void flush_and_invalidate_caches(void)
{
	__asm__ (
	"	clr.l		d0\n\t"
	"	clr.l		d1\n\t"
	"	move.l		d0,a0\n\t"
	"cfa_setloop:\n\t"
	"	cpushl		bc,(a0)				| flush\n\t"
	"	lea			0x10(a0),a0			| index+1\n\t"
	"	addq.l		#1,d1				| index+1\n\t"
	"	cmpi.w		#512,d1				| all sets?\n\t"
	"	bne.s		cfa_setloop			| no->\n\t"
	"	clr.l		d1\n\t"
	"	addq.l		#1,d0\n\t"
	"	move.l		d0,a0\n\t"
	"	cmpi.w		#4,d0				| all ways?\n\t"
	"	bne.s		cfa_setloop			| no->\n\t"
	/* input */	:
	/* output */	:
	/* clobber */	: "d0", "d1", "a0"
	);
}
