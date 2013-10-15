/*
 * bas_string.c
 *
 *  Created on: 26.02.2013
 *      Author: mfro
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
