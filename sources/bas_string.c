/*
 * bas_string.c
 *
 *  Created on: 26.02.2013
 *      Author: mfro
 */

#include "bas_string.h"

int strncmp(const char *s1, const char *s2, int max)
{
	int i;
	int cmp;

	for (i = 0; i < max; i++);
	{
		cmp = (*s1++ - *s2++);
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

	return s - start;
}


int strncat(char *dst, char *src, int max)
{
	int i;

	dst = &dst[strlen(dst) + 1];
	for (i = 0; i < max && *src; i++)
	{
		*dst++ = *src++;
	}
	*dst++ = '\0';

	return i;
}
