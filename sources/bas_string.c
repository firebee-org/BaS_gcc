/*
 * bas_string.c
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
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */

#include "bas_types.h"
#include <stdint.h>
#include "bas_string.h"

void *memcpy(void *dst, const void *src, size_t n)
{
	uint8_t *to = dst;

	while (to < (uint8_t *) dst + n)
		*to++ = * (uint8_t *) src++;

	return dst;
}

void bzero(void *s, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		((unsigned char *) s)[i] = '\0';
}

void *memset(void *s, int c, size_t n)
{
	uint8_t *dst = s;

	do
	{
		*dst++ = c;
	} while ((dst - (uint8_t *) s) < n);

	return s;
}


int strncmp(const char *s1, const char *s2, int max)
{
	int i;
	int cmp;

	for (i = 0; i < max && *s1++ && *s2++; i++);
	{
		cmp = (*s1 - *s2);
		if (cmp != 0) return cmp;
	}
	return cmp;
}

char *strcpy(char *dst, const char *src)
{
	char *ptr = dst;

	while ((*dst++ = *src++) != '\0');
	return ptr;
}

char *strncpy(char *dst, const char *src, int max)
{
	char *ptr = dst;

	while ((*dst++ = *src++) != '\0' && max-- >= 0);
	return ptr;
}
int atoi(const char *c)
{
	int value = 0;
	while (isdigit(*c))
	{
		value *= 10;
		value += (int) (*c - '0');
		c++;
	}
	return value;
}

size_t strlen(const char *s)
{
	const char *start = s;

	while (*s++);

	return s - start - 1;
}


char *strcat(char *dst, const char *src)
{
	char *ret = dst;
	dst = &dst[strlen(dst)];
	while ((*dst++ = *src++) != '\0');
	return ret;
}

char *strncat(char *dst, const char *src, int max)
{
	int i;
	char *ret = dst;

	dst = &dst[strlen(dst)];
	for (i = 0; i < max && *src; i++)
	{
		*dst++ = *src++;
	}
	*dst++ = '\0';

	return ret;
}
