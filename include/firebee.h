#ifndef _FIREBEE_H_
#define _FIREBEE_H_

/*
 * firebee.h
 *
 * preprocessor definitions for the Firebee machine. This file should contain nothing but preprocessor
 * definition that evaluate to numbers. It is intended for use in C sources as well as in linker control
 * files, so care must be taken to not break the syntax of either one.
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

#define SYSCLK			132000

#define BOOTFLASH_BASE_ADDRESS		0xE0000000
#define BOOTFLASH_SIZE				0x800000		/* FireBee has 8 MByte Flash */
#define BOOTFLASH_BAM				(BOOTFLASH_SIZE - 1)

#define SDRAM_START					0x00000000	/* start at address 0 */
#define SDRAM_SIZE					0x20000000	/* 512 MB on the Firebee */

#ifdef COMPILE_RAM
#define TARGET_ADDRESS  	(SDRAM_START + SDRAM_SIZE - 0x100000)
#else
#define TARGET_ADDRESS  	BOOTFLASH_BASE_ADDRESS

#endif /* COMPILE_RAM */

#define USB_BUFFER_SIZE   	0x80000

#define EMUTOS_BASE_ADDRESS	0xe0600000

#endif /* _FIREBEE_H_ */
