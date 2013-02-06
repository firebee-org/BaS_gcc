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

static uint32_t mx29lv640d_sector_groups[] =
{
	0xe0000000,	0xe0002000, 0xe0004000, 0xe0006000, 0xe0008000,	0xe000a000, 0xe000c000, 0xe000e000,
	0xe0010000,	0xe0020000, 0xe0030000, 0xe0040000,	0xe0050000, 0xe0060000, 0xe0070000, 0xe0080000,
	0xe0090000, 0xe00a0000, 0xe00b0000, 0xe00c0000, 0xe00d0000, 0xe00e0000, 0xe00f0000, 0xe0100000,
	0xe0110000,	0xe0120000, 0xe0130000, 0xe0140000, 0xe0150000, 0xe0160000, 0xe0170000, 0xe0180000,
	0xe0190000,	0xe01a0000, 0xe01b0000, 0xe01c0000, 0xe01d0000, 0xe01e0000, 0xe01f0000, 0xe0200000,
	0xe0210000,	0xe0220000, 0xe0230000, 0xe0240000, 0xe0250000, 0xe0260000, 0xe0270000, 0xe0280000,
	0xe0290000,	0xe02a0000, 0xe02b0000, 0xe02c0000, 0xe02d0000, 0xe02e0000, 0xe02f0000, 0xe0300000,
	0xe0310000,	0xe0320000, 0xe0330000, 0xe0340000, 0xe0350000, 0xe0360000, 0xe0370000, 0xe0380000,
	0xe0390000,	0xe03a0000, 0xe03b0000, 0xe03c0000, 0xe03d0000, 0xe03e0000, 0xe03f0000, 0xe0400000,
	0xe0410000, 0xe0420000, 0xe0430000, 0xe0440000, 0xe0450000, 0xe0460000, 0xe0470000, 0xe0480000,
	0xe0490000, 0xe04a0000, 0xe04b0000, 0xe04c0000, 0xe04d0000, 0xe04e0000, 0xe04f0000, 0xe0500000,
	0xe0510000, 0xe0520000, 0xe0530000, 0xe0540000, 0xe0550000, 0xe0560000, 0xe0570000, 0xe0580000,
	0xe0590000, 0xe05a0000, 0xe05b0000, 0xe05c0000, 0xe05d0000, 0xe05e0000, 0xe05f0000, 0xe0600000,
	0xe0610000, 0xe0620000, 0xe0630000, 0xe0640000, 0xe0650000, 0xe0660000, 0xe0670000, 0xe0680000,
	0xe0690000, 0xe06a0000, 0xe06b0000, 0xe06c0000, 0xe06d0000, 0xe06e0000, 0xe06f0000, 0xe0700000,
	0xe0710000, 0xe0720000, 0xe0730000, 0xe0740000, 0xe0750000, 0xe0760000, 0xe0770000, 0xe0780000,
	0xe0790000, 0xe07a0000, 0xe07b0000, 0xe07c0000, 0xe07d0000, 0xe07e0000, 0xe07f0000, 0xe8000000
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
