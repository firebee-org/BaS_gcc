/*
 * s19reader.c
 *
 *  Created on: 17.12.2012
 *      Author: mfro
 *      The ACP Firebee project
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

#include <stdint.h>
#include <bas_printf.h>
#include <sd_card.h>
#include <diskio.h>
#include <ff.h>
#include <s19reader.h>

typedef enum { FALSE, TRUE } bool;
typedef enum
{
	OK, 			/* no error */
	FAIL,			/* general error aka "I don't know what went wrong" */
	FILE_OPEN,		/* file open failed */
	FILE_READ,		/* file read failed */
	SREC_CORRUPT,	/* file doesn't seem to contain valid S-records */
	MEMCPY_FAILED,	/* could not copy buffer to destination */
	CODE_OVERLAPS,	/* copying would overwrite ourself */
	VERIFY_FAILED	/* destination does not read as we've written to */
} err_t;

#define NULL (void *) 0L

/*
 * Yes, I know. The following doesn't really look like code should look like...
 *
 * I did try to map structures over the S-records with (packed) which didn't work reliably due to gcc _not_ packing them appropiate
 * and finally ended up with this. Not nice, put paid (and working).
 *
 */
#define SREC_TYPE(a) 		(a)[0]							/* type of record */
#define SREC_COUNT(a) 		(a)[1]							/* length of valid bytes to follow */
#define SREC_ADDR16(a) 		(256 * (a)[2] + (a)[3])			/* 2 byte address field */
#define SREC_ADDR32(a) 		(0x1000000 * a[2] + 0x10000 *\
							a[3] + 0x100 * (a)[4] + (a)[5])	/* 4 byte address field */
#define SREC_DATA16(a)		((uint8_t *)&((a)[4]))			/* address of first byte of data in a record */
#define SREC_DATA32(a)		((uint8_t *)&((a)[6]))			/* adress of first byte of a record with 32 bit address field */
#define SREC_DATA16_SIZE(a)	(SREC_COUNT((a)) - 3)			/* length of the data[] array without the checksum field */
#define SREC_DATA32_SIZE(a)	(SREC_COUNT((a)) - 5)			/* length of the data[] array without the checksum field */
#define SREC_CHECKSUM(a)	(a)[SREC_COUNT(a) + 2 - 1]		/* record's checksum (two's complement of the sum of all bytes) */

/*
 * convert a single hex character into byte
 */
uint8_t nibble_to_byte(uint8_t nibble)
{
	if ((nibble >= '0') && (nibble <= '9'))
		return nibble - '0';
	else if ((nibble >= 'A' && nibble <= 'F'))
		return 10 + nibble - 'A';
	else if ((nibble >= 'a' && nibble <= 'f'))
		return 10 + nibble - 'a';
	return 0;
}

/*
 * convert two hex characters into byte
 */
uint8_t hex_to_byte(uint8_t hex[2])
{
	return 16 * (nibble_to_byte(hex[0])) + (nibble_to_byte(hex[1]));
}

/*
 * convert four hex characters into a 16 bit word
 */
uint16_t hex_to_word(uint8_t hex[4])
{
	return 256 * hex_to_byte(&hex[0]) + hex_to_byte(&hex[2]);
}

/*
 * convert eight hex characters into a 32 bit word
 */
uint32_t hex_to_long(uint8_t hex[8])
{
	return 65536 * hex_to_word(&hex[0]) + hex_to_word(&hex[4]);
}

/*
 * compute the record checksum
 *
 * it consists of the one's complement of the byte sum of the data from the count field until the end
 */
uint8_t checksum(uint8_t arr[])
{
	int i;
	uint8_t checksum = SREC_COUNT(arr);

	for (i = 0; i < SREC_COUNT(arr) - 1; i++)
	{
		checksum += arr[i + 2];
	}
	return ~checksum;
}

#ifdef _NOT_USED_
void print_record(uint8_t *arr)
{
	switch (SREC_TYPE(arr))
	{
		case 0:
		{
			xprintf("type 0x%x ", SREC_TYPE(arr));
			xprintf("count 0x%x ", SREC_COUNT(arr));
			xprintf("addr 0x%x ", SREC_ADDR16(arr));
			xprintf("module %11.11s ", SREC_DATA16(arr));
			xprintf("chk 0x%x 0x%x\r\n", SREC_CHECKSUM(arr), checksum(arr));
		}
		break;

		case 3:
		case 7:
		{
			xprintf("type 0x%x ", SREC_TYPE(arr));
			xprintf("count 0x%x ", SREC_COUNT(arr));
			xprintf("addr 0x%x ", SREC_ADDR32(arr));
			xprintf("data %02x,%02x,%02x,%02x,... ",
					SREC_DATA32(arr)[0], SREC_DATA32(arr)[1], SREC_DATA32(arr)[3], SREC_DATA32(arr)[4]);
			xprintf("chk 0x%x 0x%x\r\n", SREC_CHECKSUM(arr), checksum(arr));
		}
		break;

		default:
			xprintf("unsupported report type %d in print_record\r\n", arr[0]);
			break;
	}
}
#endif /* _NOT_USED_ */

/*
 * convert an S-record line into its corresponding byte vector (ASCII->binary)
 */
void line_to_vector(uint8_t *line, uint8_t *vector)
{
	int i;
	int length;
	uint8_t *vp = vector;

	length = hex_to_byte(line + 2);

	line++;
	*vp++ = nibble_to_byte(*line); /* record type. Only one single nibble */
	line++;

	for (i = 0; i <= length; i++)
	{
		*vp++ = hex_to_byte(line);
		line += 2;
	}
}

typedef err_t (*memcpy_callback_t)(uint8_t *dst, uint8_t *src, uint32_t length);

/*
 * read and parse a Motorola S-record file and copy contents to dst. The theory of operation is to read and parse the S-record file
 * and to use the supplied callback routine to copy the buffer to the destination once the S-record line is converted.
 * The memcpy callback can be anything (as long as it conforms parameter-wise) - a basically empty function to just let
 * read_srecords validate the file, a standard memcpy() to copy file contents to destination RAM or a more sophisticated
 * routine that does write/erase flash
 *
 * FIXME: Currently only records that the gcc toolchain emits are supported.
 *
 * Parameters:
 *   IN
 *   	filename - the filename that contains the S-records
 *   	callback - the memcpy() routine discussed above
 *   OUT
 *   	start_address - the execution address of the code as read from the file. Can be used to jump into and execute it
 *   	actual_length - the overall length of the binary code read from the file
 *   returns
 *   	OK or an err_t error code if anything failed
 */
err_t read_srecords(char *filename, void **start_address, uint32_t *actual_length, memcpy_callback_t callback)
{
	FRESULT fres;
	FIL file;
	err_t ret = OK;

	if ((fres = f_open(&file, filename, FA_READ) == FR_OK))
	{
		uint8_t line[80];
		int lineno = 0;
		int data_records = 0;
		bool found_block_header = FALSE;
		bool found_block_end = FALSE;
		bool found_block_data = FALSE;

		while (ret == OK && (uint8_t *) f_gets((char *) line, sizeof(line), &file) != NULL)
		{
			lineno++;
			uint8_t vector[64];

			line_to_vector(line, vector);	/* vector now contains the decoded contents of line, from line[1] on */

			if (line[0] == 'S')
			{
				if (SREC_CHECKSUM(vector) != checksum(vector))
				{
					xprintf("invalid checksum 0x%x (should be 0x%x) in line %d\r\n",
							SREC_CHECKSUM(vector), checksum(vector), lineno);
					ret = FAIL;
				}

				switch (vector[0])
				{
				case 0:	/* block header */
					found_block_header = TRUE;
					if (found_block_data || found_block_end)
					{
						xprintf("S7 or S3 record found before S0: S-records corrupt?\r\n");
						ret = FAIL;
					}

					break;

				case 3: /* four byte address field data record */
					if (!found_block_header || found_block_end)
					{
						xprintf("S3 record found before S0 or after S7: S-records corrupt?\r\n");
						ret = FAIL;
					}
					ret = callback((uint8_t *) SREC_ADDR32(vector), SREC_DATA32(vector), SREC_DATA32_SIZE(vector));
					data_records++;
					break;

				case 7: /* four byte address field end record */
					if (!found_block_header || found_block_end)
					{
						xprintf("S7 record found before S0 or after S7: S-records corrupt?\r\n");
					}
					else
					{
						// xprintf("S7 record (end) found after %d valid data blocks\r\n", data_records);
						*start_address = (void *) SREC_ADDR32(vector);
					}
					break;

				default:
					xprintf("unsupported record type (%d) found in line %d\r\n", vector[0], lineno);
					ret = FAIL;
					break;
				}
			}
			else
			{
				xprintf("illegal character ('%c') found on line %d: S-records corrupt?\r\n", line[0], lineno);
				ret = FAIL;
				break;
			}
		}
		f_close(&file);
	}
	else
	{
		xprintf("could not open file %s\r\n", filename);
	}
	return ret;
}

/*
 * this callback just does nothing besides returning OK. Meant to do a dry run over the file to check its integrity
 */
err_t simulate()
{
	err_t ret = OK;

	return ret;
}

err_t memcpy(uint8_t *dst, uint8_t *src, uint32_t length)
{
	uint8_t *end = src + length;

	do
	{
		*dst++ = *src++;
	} while (src < end);

	return OK;
}

err_t flash(uint8_t *dst, uint8_t *src, uint32_t length)
{
	err_t ret = OK;

	/* TODO: do the actual flash */

	return ret;
}

/*
 * this callback verifies the data against the S-record file contents after a write to destination
 */
err_t verify(uint8_t *dst, uint8_t *src, uint32_t length)
{
	uint8_t *end = src + length;

	do
	{
		if (*src++ != *dst++)
			return FAIL;
	} while (src < end);

	return OK;
}

void srec_execute(char *flasher_filename)
{
	DRESULT res;
	FRESULT fres;
	FATFS fs;
	FIL file;
	err_t err;
	void *start_address;
	uint32_t length;

	disk_initialize(0);
	res = disk_status(0);
	if (res == RES_OK)
	{
		fres = f_mount(0, &fs);
		if (fres == FR_OK)
		{
			if ((fres = f_open(&file, flasher_filename, FA_READ) != FR_OK))
			{
				xprintf("flasher file %s not present on disk\r\n", flasher_filename);
			}
			else
			{
				f_close(&file);

				/* first pass: parse and check for inconsistencies */
				xprintf("check file integrity: ");
				err = read_srecords(flasher_filename, &start_address, &length, simulate);
				if (err == OK)
				{
					/* next pass: copy data to destination */
					xprintf("OK.\r\ncopy/flash data: ");
					err = read_srecords(flasher_filename, &start_address, &length, memcpy);
					if (err == OK)
					{
						/* next pass: verify data */
						xprintf("OK.\r\nverify data: ");
						err = read_srecords(flasher_filename, &start_address, &length, verify);
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
		f_mount(0, NULL);
	}
	else
	{
		// xprintf("could not initialize SD card\r\n");
	}
}

