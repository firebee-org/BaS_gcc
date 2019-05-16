#ifndef _M5475E_H_
#define _M5475E_H_

/*
 * m5475e.h
 *
 * preprocessor definitions for the M5475EVB machine. This file should contain nothing but preprocessor
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

#define SYSCLK                      13333333UL

#define BOOTFLASH_BASE_ADDRESS      0xe0000000
#define BOOTFLASH_SIZE              0x200000	/* 2MB Boot flash size (16MB code flash) */
#define BOOTFLASH_BAM               (BOOTFLASH_SIZE - 1)

#define SDRAM_START                 0x00000000	/* start at address 0 */
#define SDRAM_SIZE                  0x4000000  	/* 64 MB */

#ifdef COMPILE_RAM
#define TARGET_ADDRESS              (SDRAM_START + SDRAM_SIZE - 0x200000)
#else
#define TARGET_ADDRESS              BOOTFLASH_BASE_ADDRESS
#endif /* COMPILE_RAM */
#define BFL_TARGET_ADDRESS          0x0100000           /* load address for basflash */

#define DRIVER_MEM_BUFFER_SIZE      0x100000

#define EMUTOS_BASE_ADDRESS         0xe0100000

#endif /* _M5475E_H_ */
