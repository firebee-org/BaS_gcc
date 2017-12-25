/*
 * s19reader.h
 *
 *  Created on: 17.11.2012
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
 *
 */

#ifndef _S19READER_H_
#define _S19READER_H_

typedef enum
{
	OK, 			/* no error */
	FAIL,			/* general error aka "I don't know what went wrong" */
	FILE_OPEN,		/* file open failed */
	FILE_READ,		/* file read failed */
	SREC_CORRUPT,	/* file doesn't seem to contain valid S-records */
	MEMCPY_FAILED,	/* could not copy buffer to destination */
	CODE_OVERLAPS,	/* copying would overwrite ourself */
	VERIFY_FAILED,	/* destination does not read as we've written to */
	ILLEGAL_SECTOR	/* flash sector number invalid */
} err_t;

typedef err_t (*memcpy_callback_t)(uint8_t *dst, uint8_t *src, size_t length);

extern void srec_execute(char *filename);
extern err_t read_srecords(char *filename, void **start_address, uint32_t *actual_length, memcpy_callback_t callback);
extern err_t srec_memcpy(uint8_t *dst, uint8_t *src, size_t n);

#endif /* _S19READER_H_ */
