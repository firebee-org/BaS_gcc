/*
 * s19header.c
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
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "s19reader.h"

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
#define SREC_ADDR24(a)		(0x10000 * (a)[2] + 0x100 *  \
							(a)[3] + (a)[4])				/* 3 byte address field */
#define SREC_ADDR32(a) 		(0x1000000 * a[2] + 0x10000 * \
							a[3] + 0x100 * (a)[4] + (a)[5])	/* 4 byte address field */
#define SREC_DATA16(a)		((uint8_t *)&((a)[4]))			/* address of first byte of data in a record */
#define SREC_DATA24(a)		((uint8_t *)&((a)[5]))			/* address of first data byte in 24 bit record */
#define SREC_DATA32(a)		((uint8_t *)&((a)[6]))			/* adress of first byte of a record with 32 bit address field */
#define SREC_DATA16_SIZE(a)	(SREC_COUNT((a)) - 3)			/* length of the data[] array without the checksum field */
#define SREC_DATA24_SIZE(a)	(SREC_COUNT((a)) - 4)			/* length of the data[] array without the checksum field */
#define SREC_DATA32_SIZE(a)	(SREC_COUNT((a)) - 5)			/* length of the data[] array without the checksum field */
#define SREC_CHECKSUM(a)	(a)[SREC_COUNT(a) + 2 - 1]		/* record's checksum (two's complement of the sum of all bytes) */

#define SREC_MODULENAME(a)  	&((a)[4])					/* module name in an S0 record */
#define SREC_MODULENAME_LENGTH	20
#define SREC_VERSION(a)			&((a)[24])					/* version info in an S0 record */
#define SREC_VERSION_LENGTH		2
#define SREC_REVISION(a)		&((a)[26])					/* revision info in an S0 record */
#define SREC_REVISIION_LENGTH	2
#define SREC_DESCRIPTION(a)		&((a)[28])					/* description field in an S0 record */
#define SREC_DESCRIPTION_LENGTH	36

/*
 * convert a single hex character into byte
 */
static uint8_t nibble_to_byte(uint8_t nibble)
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
static uint8_t hex_to_byte(uint8_t hex[2])
{
	return 16 * (nibble_to_byte(hex[0])) + (nibble_to_byte(hex[1]));
}

#ifdef _NOT_USED_
/*
 * convert four hex characters into a 16 bit word
 */
static uint16_t hex_to_word(uint8_t hex[4])
{
	return 256 * hex_to_byte(&hex[0]) + hex_to_byte(&hex[2]);
}

/*
 * convert eight hex characters into a 32 bit word
 */
static uint32_t hex_to_long(uint8_t hex[8])
{
	return 65536 * hex_to_word(&hex[0]) + hex_to_word(&hex[4]);
}
#endif /* _NOT_USED_ */

/*
 * compute the record checksum
 *
 * it consists of the one's complement of the byte sum of the data from the count field until the end
 */
static uint8_t checksum(uint8_t arr[])
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
			printf("type 0x%x ", SREC_TYPE(arr));
			printf("count 0x%x ", SREC_COUNT(arr));
			printf("addr 0x%x ", SREC_ADDR16(arr));
			printf("module %11.11s ", SREC_DATA16(arr));
			printf("chk 0x%x 0x%x\r\n", SREC_CHECKSUM(arr), checksum(arr));
		}
		break;

		case 3:
		case 7:
		{
			printf("type 0x%x ", SREC_TYPE(arr));
			printf("count 0x%x ", SREC_COUNT(arr));
			printf("addr 0x%x ", SREC_ADDR32(arr));
			printf("data %02x,%02x,%02x,%02x,... ",
					SREC_DATA32(arr)[0], SREC_DATA32(arr)[1], SREC_DATA32(arr)[3], SREC_DATA32(arr)[4]);
			printf("chk 0x%x 0x%x\r\n", SREC_CHECKSUM(arr), checksum(arr));
		}
		break;

		default:
			printf("unsupported report type %d in print_record\r\n", arr[0]);
			break;
	}
}
#endif /* _NOT_USED_ */

/*
 * convert an S-record line into its corresponding byte vector (ASCII->binary)
 */
static void line_to_vector(uint8_t *buff, uint8_t *vector)
{
	int i;
	int length;
	uint8_t *vp = vector;

	length = hex_to_byte(buff + 2);

	buff++;
	*vp++ = nibble_to_byte(*buff); /* record type. Only one single nibble */
	buff++;

	for (i = 0; i <= length; i++)
	{
		*vp++ = hex_to_byte(buff);
		buff += 2;
	}
}

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
int main(int argc, char *argv[])
{
	int fres;
	int set;
	char *filename = NULL;
	FILE *file;
	int ret = OK;
	int i;


	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			/* option */
			if (strcmp(argv[i], "-s") == 0)
			{
				set = 1;
			}
		}
		else
		{
			filename = argv[i];
		}
	}

	if (filename == NULL)
	{
		fprintf(stderr, "no filename given\n");
		exit(1);
	}

	if ((file = fopen(filename, "r")) != NULL)
	{
		uint8_t line[80];
		int lineno = 0;
		int data_records = 0;
		bool found_block_header = false;
		bool found_block_end = false;
		bool found_block_data = false;

		while (ret == OK && (uint8_t *) fgets((char *) line, sizeof(line), file) != NULL)
		{
			lineno++;
			uint8_t vector[80];



			line_to_vector(line, vector);	/* vector now contains the decoded contents of line, from line[1] on */

			if (line[0] == 'S')
			{
				char header[256];

				if (SREC_CHECKSUM(vector) != checksum(vector))
				{
					printf("invalid checksum 0x%x (should be 0x%x) in line %d\r\n",
							SREC_CHECKSUM(vector), checksum(vector), lineno);
					ret = FAIL;
				}

				switch (vector[0])
				{
				case 0:	/* block header */
					found_block_header = true;
					if (found_block_data || found_block_end)
					{
						printf("S7 or S3 record found before S0: S-records corrupt?\r\n");
						ret = FAIL;
					}
					printf("length of record: %d\n", SREC_COUNT(vector));
					printf("comment: %s\n", &vector[4]);

					break;

				case 2: /* three byte address field data record */
					if (!found_block_header || found_block_end)
					{
						printf("S3 record found before S0 or after S7: S-records corrupt?\r\n");
						ret = FAIL;
					}
					// ret = callback((uint8_t *) SREC_ADDR24(vector), SREC_DATA24(vector), SREC_DATA24_SIZE(vector));
					data_records++;
					break;

				case 3: /* four byte address field data record */
					if (!found_block_header || found_block_end)
					{
						printf("S3 record found before S0 or after S7: S-records corrupt?\r\n");
						ret = FAIL;
					}
					// ret = callback((uint8_t *) SREC_ADDR32(vector), SREC_DATA32(vector), SREC_DATA32_SIZE(vector));
					data_records++;
					break;

				case 7: /* four byte address field end record */
					if (!found_block_header || found_block_end)
					{
						printf("S7 record found before S0 or after S7: S-records corrupt?\r\n");
					}
					else
					{
						// printf("S7 record (end) found after %d valid data blocks\r\n", data_records);
						//*start_address = (void *) SREC_ADDR32(vector);
					}
					break;

				case 8: /* three byte address field end record */
					if (!found_block_header || found_block_end)
					{
						printf("S8 record found before S0 or after S8: S-records corrupt?\r\n");
					}
					else
					{
						// printf("S7 record (end) found after %d valid data blocks\r\n", data_records);
						//*start_address = (void *) SREC_ADDR24(vector);
					}
					break;

				default:
					printf("unsupported record type (%d) found in line %d\r\n", vector[0], lineno);
					printf("offending line: \r\n");
					printf("%s\r\n", line);
					ret = FAIL;
					break;
				}
			}
			else
			{
				printf("illegal character ('%c') found on line %d: S-records corrupt?\r\n", line[0], lineno);
				ret = FAIL;
				break;
			}
		}
		fclose(file);
	}
	else
	{
		printf("could not open file %s\r\n", filename);
		ret = FILE_OPEN;
	}
	return ret;
}

/*
 * this callback just does nothing besides returning OK. Meant to do a dry run over the file to check its integrity
 */
static err_t simulate()
{
	err_t ret = OK;

	return ret;
}


#ifdef _NOT_USED_
static err_t flash(uint8_t *dst, uint8_t *src, uint32_t length)
{
	err_t ret = OK;

	/* TODO: do the actual flash */
	amd_flash_program(dst, src, length, false, NULL, xputchar);

	return ret;
}
#endif /* _NOT_USED_ */


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

void srec_execute(char *flasher_filename)
{
}

