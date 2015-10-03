/*
 * printf_helper.S
 *
 * assembler trampoline to let printf (compiled -mpcrel) indirectly reference __MBAR
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
 * Copyright 2012 - 2025 M. Froeschle
 */


	.globl	printf_helper

printf_helper:
	.extern	__MBAR

.wait_txready:
	move.w	__MBAR+0x8604,d2		// PSCSCR0 status register
	btst	#10,d2				// space left in TX fifo?
	beq.s	.wait_txready			// no, loop
	lea	__MBAR+0x860C,a0		// PSCSTB0 transmitter buffer register
	move.b	d0,(a0)				// send byte
	rts
