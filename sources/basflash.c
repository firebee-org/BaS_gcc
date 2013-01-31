/*
 * basflash.c
 *
 *  Created on: 18.12.2012
 *      Author: mfro
 */

#include <stdint.h>
#include <stdbool.h>
#include <bas_printf.h>
#include <diskio.h>
#include <ff.h>
#include <s19reader.h>

typedef enum {OK, ERR} err_t;

bool memcpy()
{
	return true;
}

bool verify()
{
	return true;
}

bool simulate()
{
	return true;
}

void srec_flash(char *flash_filename)
{
	DRESULT res;
	FRESULT fres;
	FATFS fs;
	FIL file;
	err_t err;
	void *start_address;
	uint32_t length;

	// disk_initialize(0);
	res = disk_status(0);
	if (res == RES_OK)
	{
		fres = f_mount(0, &fs);
		if (fres == FR_OK)
		{
			if ((fres = f_open(&file, flash_filename, FA_READ) != FR_OK))
			{
				xprintf("flasher file %s not present on disk\r\n", flash_filename);
			}
			else
			{
				f_close(&file);

				/* first pass: parse and check for inconsistencies */
				xprintf("check file integrity: ");
				err = read_srecords(flash_filename, &start_address, &length, simulate);
				if (err == OK)
				{
					/* next pass: copy data to destination */
					xprintf("OK.\r\ncopy/flash data: ");
					err = read_srecords(flash_filename, &start_address, &length, memcpy);
					if (err == OK)
					{
						/* next pass: verify data */
						xprintf("OK.\r\nverify data: ");
						err = read_srecords(flash_filename, &start_address, &length, verify);
						if (err == OK)
						{
							xprintf("OK.\r\n");
							typedef void void_func(void);
							void_func *func;
							xprintf("target successfully written and verified. Start address: %p\r\n", start_address);

							func = start_address;
							(*func)();
						}
						else
						{
							xprintf("failed\r\n");
						}
					}
					else
					{
						xprintf("failed\r\n");
					}
				}
				else
				{
					xprintf("failed\r\n");
				}
			}
		}
		else
		{
			// xprintf("could not mount FAT FS\r\n");
		}
		f_mount(0, 0L);
	}
	else
	{
		// xprintf("could not initialize SD card\r\n");
	}
}

void srec_load(char *flash_filename)
{
	DRESULT res;
	FRESULT fres;
	FATFS fs;
	FIL file;
	err_t err;
	void *start_address;
	uint32_t length;

	// disk_initialize(0);
	res = disk_status(0);
	if (res == RES_OK)
	{
		fres = f_mount(0, &fs);
		if (fres == FR_OK)
		{
			if ((fres = f_open(&file, flash_filename, FA_READ) != FR_OK))
			{
				xprintf("flasher file %s not present on disk\r\n", flash_filename);
			}
			else
			{
				f_close(&file);

				/* first pass: parse and check for inconsistencies */
				xprintf("check file integrity: ");
				err = read_srecords(flash_filename, &start_address, &length, simulate);
				if (err == OK)
				{
					/* next pass: copy data to destination */
					xprintf("OK.\r\ncopy/flash data: ");
					err = read_srecords(flash_filename, &start_address, &length, memcpy);
					if (err == OK)
					{
						/* next pass: verify data */
						xprintf("OK.\r\nverify data: ");
						err = read_srecords(flash_filename, &start_address, &length, verify);
						if (err == OK)
						{
							xprintf("OK.\r\n");
							typedef void void_func(void);
							void_func *func;
							xprintf("target successfully written and verified. Start address: %p\r\n", start_address);

							func = start_address;
							(*func)();
						}
						else
						{
							xprintf("failed\r\n");
						}
					}
					else
					{
						xprintf("failed\r\n");
					}
				}
				else
				{
					xprintf("failed\r\n");
				}
			}
		}
		else
		{
			// xprintf("could not mount FAT FS\r\n");
		}
		f_mount(0, 0L);
	}
	else
	{
		// xprintf("could not initialize SD card\r\n");
	}
}

void basflash(void)
{
	const char *basauto_str = "\\BASAUTO\\";
	const char *bassimu_str = "\\BASSIMU\\";

	xprintf("\r\nHello from BASFLASH.S19!\r\n\r\n");
}
