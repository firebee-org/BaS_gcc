/*
 * sd_card.c
 *
 *  Created on: 16.12.2012
 *      Author: mfro
 */
#include <sd_card.h>
#include <diskio.h>
#include <ff.h>
#include <bas_printf.h>

#define WELCOME_NAME	"WELCOME.MSG"
#define FLASHCODE_NAME	"BASFLASH.BIN"

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

			}
			f_close(&file);
		}
		f_mount(0, 0L);	/* release work area */
	}
}
