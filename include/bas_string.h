/*
 * bas_string.h
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

#ifndef BAS_STRING_H_
#define BAS_STRING_H_

#include <stddef.h>

extern int strncmp(const char *s1, const char *s2, size_t max);
extern char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, size_t max);
extern int strcmp(const char *s1, const char *s2);
extern size_t strlen(const char *str);
extern char *strcat(char *dst, const char *src);
extern char *strncat(char *dst, const char *src, size_t max);
extern int atoi(const char *c);
extern void *memcpy(void *dst, const void *src, size_t n);
extern void *memmove(void *dst, const void *src, size_t n);
extern void *memset(void *s, int c, size_t n);
extern int memcmp(const void *s1, const void *s2, size_t max);
extern void bzero(void *s, size_t n);

#define isdigit(c)	(((c) >= '0') && ((c) <= '9'))
#define isupper(c)	((c) >= 'A' && ((c) <= 'Z'))
#define islower(c)	((c) >= 'a' && ((c) <= 'z'))
#define isalpha(c)	(isupper((c)) || islower(c))
#define tolower(c)	(isupper(c) ? ((c) + 'a' - 'A') : (c))

#endif /* BAS_STRING_H_ */
