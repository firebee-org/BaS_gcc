/*
 * basflash.c
 *
 *  Created on: 18.12.2012
 *      Author: mfro
 */

#include <stdint.h>
#include <stdbool.h>
#include "bas_string.h"
#include "bas_printf.h"
#include "diskio.h"
#include "ff.h"
#include "s19reader.h"



static uint32_t mx29lv640d_flash_sectors[] =
{
	0xe0000000, 0xe0002000, 0xe0004000, 0xe0006000, 0xe0008000,	0xe000a000, 0xe000c000, 0xe000e000,
	0xe0010000, 0xe0020000, 0xe0030000, 0xe0040000,	0xe0050000, 0xe0060000, 0xe0070000, 0xe0080000,
	0xe0090000, 0xe00a0000, 0xe00b0000, 0xe00c0000, 0xe00d0000, 0xe00e0000, 0xe00f0000, 0xe0100000,
	0xe0110000, 0xe0120000, 0xe0130000, 0xe0140000, 0xe0150000, 0xe0160000, 0xe0170000, 0xe0180000,
	0xe0190000, 0xe01a0000, 0xe01b0000, 0xe01c0000, 0xe01d0000, 0xe01e0000, 0xe01f0000, 0xe0200000,
	0xe0210000, 0xe0220000, 0xe0230000, 0xe0240000, 0xe0250000, 0xe0260000, 0xe0270000, 0xe0280000,
	0xe0290000, 0xe02a0000, 0xe02b0000, 0xe02c0000, 0xe02d0000, 0xe02e0000, 0xe02f0000, 0xe0300000,
	0xe0310000, 0xe0320000, 0xe0330000, 0xe0340000, 0xe0350000, 0xe0360000, 0xe0370000, 0xe0380000,
	0xe0390000, 0xe03a0000, 0xe03b0000, 0xe03c0000, 0xe03d0000, 0xe03e0000, 0xe03f0000, 0xe0400000,
	0xe0410000, 0xe0420000, 0xe0430000, 0xe0440000, 0xe0450000, 0xe0460000, 0xe0470000, 0xe0480000,
	0xe0490000, 0xe04a0000, 0xe04b0000, 0xe04c0000, 0xe04d0000, 0xe04e0000, 0xe04f0000, 0xe0500000,
	0xe0510000, 0xe0520000, 0xe0530000, 0xe0540000, 0xe0550000, 0xe0560000, 0xe0570000, 0xe0580000,
	0xe0590000, 0xe05a0000, 0xe05b0000, 0xe05c0000, 0xe05d0000, 0xe05e0000, 0xe05f0000, 0xe0600000,
	0xe0610000, 0xe0620000, 0xe0630000, 0xe0640000, 0xe0650000, 0xe0660000, 0xe0670000, 0xe0680000,
	0xe0690000, 0xe06a0000, 0xe06b0000, 0xe06c0000, 0xe06d0000, 0xe06e0000, 0xe06f0000, 0xe0700000,
	0xe0710000, 0xe0720000, 0xe0730000, 0xe0740000, 0xe0750000, 0xe0760000, 0xe0770000, 0xe0780000,
	0xe0790000, 0xe07a0000, 0xe07b0000, 0xe07c0000, 0xe07d0000, 0xe07e0000, 0xe07f0000, 0xe0800000
};
static const int num_flash_sectors = sizeof(mx29lv640d_flash_sectors) / sizeof(uint32_t);

typedef struct romram
{
	uint32_t flash_address;
	uint32_t ram_address;
	char *name;
} ROMRAM;

static const struct romram flash_areas[] =
{
		{ 0xe0600000, 0x00e00000, "EmuTOS" },	/* EmuTOS */
		{ 0xe0400000, 0x00e00000, "FireTOS" },	/* FireTOS */
		{ 0xe0700000, 0x00e00000, "FPGA" },  /* FPGA config */
};
static const int num_flash_areas = sizeof(flash_areas) / sizeof(struct romram);

#define FLASH_ADDRESS 0xe0000000
static volatile uint16_t *flash_unlock1 = (volatile uint16_t *) FLASH_ADDRESS + 0xaaa;
static volatile uint16_t *flash_unlock2 = (volatile uint16_t *) FLASH_ADDRESS + 0x554;
static const uint16_t cmd_unlock1 = 0xaa;
static const uint16_t cmd_unlock2 = 0x55;
static const uint16_t cmd_sector_erase1 = 0x80;
static const uint16_t cmd_sector_erase2 = 0x30;
static const uint16_t cmd_sector_erase_suspend = 0xb0;
static const uint16_t cmd_sector_erase_resume = 0x30;
static const uint16_t cmd_program = 0xa0;
static const uint16_t cmd_autoselect = 0x90;
static const uint16_t cmd_read = 0xf0;

/*
 * this callback just does nothing besides returning OK. Meant to do a dry run over the file to check its integrity
 */
static err_t simulate()
{
	err_t ret = OK;

	return ret;
}

static err_t memcpy(uint8_t *dst, uint8_t *src, uint32_t length)
{
	uint8_t *end = src + length;

	do
	{
		*dst++ = *src++;
	} while (src < end);

	return OK;
}

static err_t flash(uint8_t *dst, uint8_t *src, uint32_t length)
{
	err_t ret = OK;

	/* TODO: do the actual flash */

	return ret;
}

/*
 * this callback verifies the data against the S-record file contents after a write to destination
 */
static err_t verify(uint8_t *dst, uint8_t *src, uint32_t length)
{
	uint8_t *end = src + length;

	do
	{
		if (*src++ != *dst++)
			return FAIL;
	} while (src < end);

	return OK;
}

/*
 * unlock a flash sector
 */

err_t unlock_flash_sector(int sector_num)
{
	volatile uint32_t rd;
	uint32_t size = (sector_num < num_flash_sectors ?
						mx29lv640d_flash_sectors[sector_num + 1] - mx29lv640d_flash_sectors[sector_num] :
						0);

	*flash_unlock1 = cmd_unlock1;
	*flash_unlock2 = cmd_unlock2;
	*flash_unlock1 = cmd_autoselect;
	rd = * (volatile uint32_t *) FLASH_ADDRESS;
	* (volatile uint32_t *) FLASH_ADDRESS = size;
	(void) rd;	/* get rid of "unused variable" compiler warning */
	return OK;
}

/*
 * erase a flash sector
 *
 * sector_num is the index into the sector table above.
 *
 * FIXME: need to disable data cache to ensure proper operation
 */
err_t erase_flash_sector(int sector_num)
{
	volatile uint32_t rd;
	uint32_t size = (sector_num < num_flash_sectors ?
						mx29lv640d_flash_sectors[sector_num + 1] - mx29lv640d_flash_sectors[sector_num] :
						0);

	if (unlock_flash_sector(sector_num) == OK)
	{
		*flash_unlock1 = cmd_unlock1;
		*flash_unlock2 = cmd_unlock2;
		*flash_unlock1 = cmd_sector_erase1;
		*flash_unlock1 = cmd_unlock1;
		*flash_unlock2 = cmd_unlock2;
		*flash_unlock1 = cmd_sector_erase1;
		rd = * (volatile uint32_t *) FLASH_ADDRESS;
		* (volatile uint32_t *) FLASH_ADDRESS = size;
		(void) rd;	/* get rid of "unused variable" compiler warning */
		return OK;
	}
	return ILLEGAL_SECTOR;
}

err_t erase_flash_region(void *start_address, uint32_t length)
{
	err_t err;
	int sector = -1;
	int i;

	/*
	 * determine first sector to erase
	 *
	 * FIXME: if the start address of the .s19 file does not fall on a sector boundary, we
	 * will probably erase vital code in the previous flash sector. This should not happen on the Firebee
	 * where we have fixed areas for the different flash codes, but we should probably take care anyway
	 */
	for (i = 0; i < num_flash_sectors; i++)
	{
		if (start_address >= (void *) mx29lv640d_flash_sectors[i] && start_address <= (void *) mx29lv640d_flash_sectors[i])
			sector = i;
	}

	if (sector >= 0 && sector <= num_flash_sectors)
	{
		/*
		 * erase sectors until free space equals length
		 *
		 * FIXME: same as above. Currently, there is no prevention against overlapping flash areas.
		 */
		do {
			err = erase_flash_sector(sector);
			sector++;
		} while ((void *) mx29lv640d_flash_sectors[sector] < start_address + length && ! err);
	}
	else
	{
		err = ILLEGAL_SECTOR;
	}
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
					xprintf("OK.\r\nerase flash area (from %p, length 0x%lx): ", start_address, length);
					err = erase_flash_region(start_address, length);

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

err_t srec_load(char *flash_filename)
{
	FRESULT fres;
	FIL file;
	err_t err;
	void *start_address;
	uint32_t length;

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
	return OK;
}

void basflash(void)
{
	const char *basflash_str = "\\BASFLASH";
	const char *bastest_str = "\\BASTEST";
	DRESULT res;
	FRESULT fres;
	FATFS fs;

	xprintf("\r\nHello from BASFLASH.S19!\r\n\r\n");

	/*
	 * read \BASTEST\ folder contents (search for .S19-files). If found load them to their final destination
	 * (after BaS has copied them, not their flash location) and return.
	 *
	 * Files located in the BASTEST-folder thus override those in flash. Useful for testing before flashing
	 */
	res = disk_status(0);
	xprintf("disk_status(0) = %d\r\n", res);
	if (res == RES_OK)
	{
		fres = f_mount(0, &fs);
		xprintf("f_mount() = %d\r\n", fres);
		if (fres == FR_OK)
		{
			DIR directory;

			fres = f_opendir(&directory, bastest_str);
			xprintf("f_opendir() = %d\r\n", fres);
			if (fres == FR_OK)
			{
				FILINFO fileinfo;

				fres = f_readdir(&directory, &fileinfo);
				xprintf("f_readdir() = %d\r\n", fres);
				while (fres == FR_OK)
				{
					const char *srec_ext = ".S19";
					char path[30];

					if (fileinfo.fname[0] != '\0')	/* found a file */
					{
						xprintf("check file %s (%s == %s ?)\r\n", fileinfo.fname, &fileinfo.fname[strlen(fileinfo.fname) - 4], srec_ext);
						if (strlen(fileinfo.fname) >= 4 && strncmp(&fileinfo.fname[strlen(fileinfo.fname) - 4], srec_ext, 4) == 0)	/* we have a .S19 file */
						{
							/*
							 * build path + filename
							 */
							strcpy(path, bastest_str);
							strcat(path, "\\");
							strncat(path, fileinfo.fname, 13);

							xprintf("loading file %s\r\n", path);
							/*
							 * load file
							 */
							if (srec_load(path) != OK)
							{
								xprintf("failed to load file %s\r\n", path);
								// error handling
							}
						}
					}
					else
						break;
					fres = f_readdir(&directory, &fileinfo);
					xprintf("f_readdir() = %d\r\n", fres);
				}
			}
			else
			{
				xprintf("f_opendir %s failed with error code %d\r\n", bastest_str, fres);
			}
		}
		else
		{
			// xprintf("could not mount FAT FS\r\n");
		}
		f_mount(0, 0L);		/* unmount SD card */
	}
}
