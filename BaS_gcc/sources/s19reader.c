/*
 * s19reader.c
 *
 *  Created on: 17.12.2012
 *      Author: mfro
 *      (c) The ACP Firebee project
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

typedef enum { FALSE, TRUE } bool;
typedef enum { OK, FAIL } err_t;
#define NULL (void *) 0L

typedef struct srec2	/* two byte address field */
{
	uint8_t record_type;	/* [0 + 1] for S0, S1, S9 */
	uint8_t length;			/* [1 + 1] number of valid bytes following */
	uint16_t address;		/* [2 + 2] */
	uint8_t data[64];		/* [4 + length] */
} __attribute__((packed)) SREC2;
#define SREC2_DATALEN(a) ((a)->length - 2)
#define SREC2_CHECKSUM(a) ((a)->data[SREC2_DATALEN(a) - 1])

typedef struct srec4	/* four byte address field */
{
	uint8_t record_type;	/* [0 + 1] for S3, S7 */
	uint8_t length;			/* [1 + 1] */
	uint32_t address;		/* [2 + 4] */
	uint8_t data[64];		/* [6 + length] */
} __attribute__((packed)) SREC4;
#define SREC4_DATALEN(a) ((a)->length - 4)
#define SREC4_CHECKSUM(a) ((a)->data[SREC4_DATALEN(a) - 1])


uint8_t nibble_to_byte(uint8_t nibble)
{
	if ((nibble >= '0') && (nibble <= '9'))
		return nibble - '0';
	else if ((nibble >= 'A' && nibble <= 'F'))
		return 10 + nibble - 'A';
	else if ((nibble >= 'a' && nibble <= 'f'))
		return 10 + nibble - 'a';
	else
	{
		/* FIXME: do a clean error exit */
	}
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

void print_record(uint8_t *arr)
{
	xprintf("S%d record:\r\n", arr[0]);
	switch (arr[0]) {
		case 0:
		{
			SREC2 *header = (SREC2 *) arr;

			xprintf("type:           0x%x\r\n", header->record_type);
			xprintf("byte count:     0x%x\r\n", header->length);
			xprintf("address:        0x%x\r\n", header->address);
			xprintf("module name:    %11.11s\r\n", header->data);
			xprintf("checksum:       0x%x\r\n", SREC2_CHECKSUM(header));
		}
		break;

		case 3:
		{
			SREC4 *header = (SREC4 *) arr;
			xprintf("type:           0x%x\r\n", header->record_type);
			xprintf("byte count:     0x%x\r\n", header->length);
			xprintf("address:        0x08%x\r\n", header->address);
			xprintf("data:           %02x, %0sx, %02x, %02x, ...\r\n", header->data[0], header->data[1], header->data[2], header->data[3]);
			xprintf("checksum:       0x%x\r\n", SREC4_CHECKSUM(header));
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

	xprintf("  ");
	for (i = 0; i <= length; i++)
	{
		xprintf("%c%c", *line, *(line + 1));
		*vp++ = hex_to_byte(line);
		line += 2;
	}
	xprintf("\r\r\n");
	for (i = 0; i < length + 2; i++)
		xprintf("%02x", vector[i]);
	xprintf("\r\r\n");
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

		xprintf("succesfully opened file \"%s\"\r\n", filename);

		while (ret == OK && (uint8_t *) f_gets((char *) line, sizeof(line), &file) != NULL)
		{
			lineno++;
			uint8_t vector[64];

			line_to_vector(line, vector);	/* vector now contains the decoded contents of line, from line[1] on */

			if (line[0] == 'S')
			{
				print_record(vector);
				switch (line[1])
				{
				case '0':	/* block header */
					xprintf("S0 record (block header found)\r\n");
					found_block_header = TRUE;
					break;

				case '1':
					xprintf("S1 record (two byte address field) found\r\n");
					break;
				case '2':
					xprintf("S2 record (three byte address field) found\r\n");
					break;
				case '3':
					// xprintf("S3 record (four byte address field) found\r\n");
					if (found_block_header)
					{
						data_records++;
					}
					else
					{
						xprintf("found a data record without a block header before. Data is probably invalid\r\n");
						ret = FAIL;
					}
					break;

				case '5':
					xprintf("S5 record (record count record) found\r\n");
					break;
				case '7':
					xprintf("S7 record (end of block) found after %d valid data blocks\r\n", data_records);
					break;
				case '8':
					xprintf("S8 record (end of block) found\r\n");
					break;
				case '9':
					xprintf("S9 record (end of block) found\r\n");
					break;
				default:
					xprintf("unsupported record type (%c) found in line %d\r\n", line[1], lineno);
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

void flasher_load(void)
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
			err = read_srecords("../BaS_gcc_trunk/ram.elf.s19", &start_address, &length, buffer, sizeof(buffer));
		}
		f_mount(0, NULL);
	}

}

