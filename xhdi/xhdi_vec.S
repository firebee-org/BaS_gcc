/*
 * xhdi_vec.S
 *
 * XHDI entry point for EmuTOS into BaS' SD-card driver
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

//
// XHDI entry point
//
	.extern	_xhdi_call

	.globl	_xhdi_vec
	.globl	_xhdi_sd_install

//
// this is where the XHDI cookie points to:
//

	.text
	.byte	'B','A','S','_'
_xhdi_vec:
	lea	-12(sp),sp		// save all used registers according to XHDI spec
	movem.l	d1/a0-a1,(sp)

	pea	16(sp)			// forward address of parameters on stack
	jsr	_xhdi_call		// to internal routine
	addq.l	#4,sp			// correct stack

	movem.l	(sp),d1/a0-a1		// restore registers
	lea	12(sp),sp
	rts

	.bss
_old_vector:
	.ds.l   1

	.text

//
// trap #0 handler to bring the address of the disk routines into TOS
//
_xhdi_sd_install:
	move.l	4(sp),d0		// address of the old XHDI vector
	move.l	d0,_old_vector		// save it - just in case we need it later
	move.l	#_xhdi_vec,d0		// return our BaS vector to TOS
	move.l	d0,a0			//
	rte
