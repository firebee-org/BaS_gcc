/*
 * sd_card.c
 *
 *  Created on: 16.12.2012
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
 * Copyright 2012        M. Froeschle
 */

#include <sd_card.h>
#include <diskio.h>
#include <ff.h>
#include <bas_printf.h>

#define WELCOME_NAME	"WELCOME.MSG"
#define FLASHCODE_NAME	"BASFLASH.BIN"

#define FLASHCODE_ADDRESS	0x01000000L

/*
 * initialize SD-card and FF FAT filesystem routines. Harness to load a file during boot.
 *
 * This is currently more like a proof of concept,
 * but will be extended to load and execute a bootstrap flasher to be able to flash the Bee directly
 * from card.
 */
void sd_card_init(void)
{
	DRESULT res;
	FATFS fs;
	FRESULT fres;

	disk_initialize(0);
	res = disk_status(0);
	xprintf("disk status of SD card is %d\r\n", res);
	if (res == RES_OK)
	{
		fres = f_mount(0, &fs);
		xprintf("mount status of SD card fs is %d\r\n", fres);
		if (fres == FR_OK)
		{
			DIR directory;
			FIL file;

			fres = f_opendir(&directory, "\\");
			if (fres == FR_OK)
			{
				FILINFO fi;

				while (((fres = f_readdir(&directory, &fi)) == FR_OK) && fi.fname[0])
				{
					xprintf("%13.13s %d\r\n", fi.fname, fi.fsize);
				}
			}
			else
			{
				xprintf("could not open directory \"\\\" on SD-card! Error code: %d\r\n", fres);
			}

			fres = f_open(&file, WELCOME_NAME, FA_READ);
			if (fres == FR_OK)
			{
				char line[128];

				while (f_gets(line, sizeof(line), &file))
				{
					xprintf("%s", line);
				}
			}
			f_close(&file);
		}
		f_mount(0, 0L);	/* release work area */
	}
}
