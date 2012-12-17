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

			/*
			 * let's see if we find our boot flashing executable on disk
			 */
			fres = f_open(&file, FLASHCODE_NAME, FA_READ);
			if (fres == FR_OK)
			{
				/*
				 * yes, load and execute it
				 *
				 * FIXME: we will need some kind of user confirmation here
				 * to avoid unwanted flashing or "bootsector viruses" before going productive
				 */
				uint32_t size;	/* length of code piece read */
				uint32_t total_size = 0L;
				uint32_t start_time = MCF_SLT_SCNT(0);
				uint32_t end_time;
				uint32_t time = 0;

				while ((fres = f_read(&file, (void *) FLASHCODE_ADDRESS, 1024, &size)) == FR_OK)
				{
					total_size += size / 1024;
					//xprintf("read hunk of %d bytes, total_size = %d kBytes\r\n", size, total_size);
				}
				end_time = MCF_SLT_SCNT(0);
				time = (end_time - start_time) / 132L;
				xprintf("result of f_read: %ld, %ld kbytes read\r\n", fres, total_size);
				xprintf("time to load %s: %ld s\r\n", FLASHCODE_NAME, time / 1000 / 100);
				xprintf("equals to about %ld kBytes/second\r\n", total_size / (time / 1000 / 100));

			}
			f_close(&file);
		}
		f_mount(0, 0L);	/* release work area */
	}
}
