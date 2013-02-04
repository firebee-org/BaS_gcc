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

uint32_t mx29lv640d_sector_groups[] =
{
	0xe0000000,	0xe0008000,	0xe0010000,	0xe0018000,	0xe0020000,	0xe0028000,	0xe0030000,	0xe0038000,
	0xe0040000,	0xe0048000,	0xe0050000,	0xe0058000,	0xe0060000,	0xe0068000,	0xe0070000,	0xe0078000,
	0xe0080000,	0xe0088000,	0xe0090000,	0xe0098000,	0xe00a0000,	0xe00a8000,	0xe00b0000,	0xe00b8000,
	0xe00c0000,	0xe00c8000,	0xe00d0000,	0xe00d8000,	0xe00e0000,	0xe00e8000,	0xe00f0000,	0xe00f8000,
	0xe0100000,	0xe0108000,	0xe0110000,	0xe0118000,	0xe0120000,	0xe0128000,	0xe0130000,	0xe0138000,
};

extern err_t simulate();
extern err_t memcpy();
extern err_t verify();

err_t erase_flash_sector(const void *start_address)
{
	return OK;
}

err_t erase_flash_region(void *start_address)
{
	const uint32_t flash_region_size = 0x400000;	/* 4 MByte */
	const uint32_t flash_sector_size = 0x2000;		/* 8 KByte */
	err_t err;

	const void *p = start_address;

	do {
		err = erase_flash_sector(p);
		p += flash_sector_size;
	} while ((p < start_address + flash_region_size) && err == OK);

	return err;
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
					xprintf("OK.\r\nerase flash area (from %p): ", start_address);
					err = erase_flash_region(start_address);

					/* next pass: copy data to destination */
					xprintf("OK.\r\flash data: ");
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
