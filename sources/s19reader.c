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
typedef enum { OK, FAIL } err_t;
#define NULL (void *) 0L

#define SREC_TYPE(a) 	(a)[0]		/* type of record */
#define SREC_COUNT(a) 	(a)[1]	/* length of valid bytes to follow */
#define SREC_ADDR16(a) 	(256 * (a)[2] + (a)[3])	/* 2 byte address field */
#define SREC_ADDR32(a) 	(0x1000000 * a[2] + 0x10000 * a[3] + 0x100 * (a)[4] + (a)[5])	/* 4 byte address field */
#define SREC_DATA16(a)	((uint8_t *)&((a)[4]))
#define SREC_DATA32(a)	((uint8_t *)&((a)[6]))
#define SREC_CHECKSUM(a)	(a)[SREC_COUNT(a) + 2 - 1]

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

uint8_t hex_to_byte(uint8_t hex[2])
{
	return 16 * (nibble_to_byte(hex[0])) + (nibble_to_byte(hex[1]));
}

uint16_t hex_to_word(uint8_t hex[4])
{
	return 256 * hex_to_byte(&hex[0]) + hex_to_byte(&hex[2]);
}

uint32_t hex_to_long(uint8_t hex[8])
{
	return 65536 * hex_to_word(&hex[0]) + hex_to_word(&hex[4]);
}

/*
 * compute the checksum
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

err_t read_srecords(char *filename, uint32_t *start_address, uint32_t *actual_length, uint8_t *buffer, uint32_t buffer_length)
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

		xprintf("succesfully opened file \"%s\"\r\n", filename);

		while (ret == OK && (uint8_t *) f_gets((char *) line, sizeof(line), &file) != NULL)
		{
			lineno++;
			uint8_t vector[64];

			line_to_vector(line, vector);	/* vector now contains the decoded contents of line, from line[1] on */

			if (line[0] == 'S')
			{
				//print_record(vector);
				if (SREC_CHECKSUM(vector) != checksum(vector))
				{
					xprintf("invalid checksum in line %d\r\n", lineno);
					ret = FAIL;
				}

				switch (vector[0])
				{
				case 0:	/* block header */
					xprintf("S0 record (block header) found\r\n");
					found_block_header = TRUE;
					if (found_block_data || found_block_end)
					{
						xprintf("S7 or S3 record found before S0: S-records corrupt?\r\n");
						ret = FAIL;
					}
					break;

				case 1:
					xprintf("S1 record (two byte address field) found\r\n");
					break;
				case 2:
					xprintf("S2 record (three byte address field) found\r\n");
					break;
				case 3:
					// xprintf("S3 record (four byte address field) found\r\n");
					if (!found_block_header || found_block_end)
					{
						xprintf("S3 record found before S0 or after S7: S-records corrupt?\r\n");
						ret = FAIL;
					}
					data_records++;
					break;

				case 5:
					xprintf("S5 record (record count record) found\r\n");
					break;
				case 7:
					if (!found_block_header || found_block_end)
					{
						xprintf("S7 record found before S0 or after S7: S-records corrupt?\r\n");
					}
					else
					{
						xprintf("S7 record (end) found after %d valid data blocks\r\n", data_records);
					}
					break;
				case 8:
					xprintf("S8 record (end of block) found\r\n");
					break;
				case 9:
					xprintf("S9 record (end of block) found\r\n");
					break;
				default:
					xprintf("unsupported record type (%d) found in line %d\r\n", vector[0], lineno);
					ret = FAIL;
					break;
				}
			}
			else
			{
				xprintf("illegal character ('%c') found on line %d. Probably not an S-Record file\r\n", line[0], lineno);
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

void flasher_load(char *flasher_filename)
{
	DRESULT res;
	FRESULT fres;
	FATFS fs;
	err_t err;
	uint32_t start_address;
	uint32_t length;
	uint8_t buffer[2048];

	xprintf("S-record reader\r\n");

	disk_initialize(0);
	res = disk_status(0);
	xprintf("disk status of SD card is %d\r\r\n", res);
	if (res == RES_OK)
	{
		fres = f_mount(0, &fs);
		xprintf("mount status of SD card fs is %d\r\n", fres);
		if (fres == FR_OK)
		{
			err = read_srecords(flasher_filename, &start_address, &length, buffer, sizeof(buffer));
			if (err == OK)
			{

			}
		}
		f_mount(0, NULL);
	}

}

