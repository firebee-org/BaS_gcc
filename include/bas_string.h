/*
 * bas_string.h
 *
 *  Created on: 26.02.2013
 *      Author: mfro
 */

#ifndef BAS_STRING_H_
#define BAS_STRING_H_

#include <stddef.h>

extern int strncmp(const char *s1, const char *s2, int max);
extern char *strcpy(char *dst, const char *src);
extern size_t strlen(const char *str);
extern int strncat(char *dst, char *src, int max);
extern int atoi(const char *c);

#define isdigit(c)	(((c) >= '0') && ((c) <= '9'))
#define isupper(c)	((c) >= 'A' && ((c) <= 'Z'))
#define islower(c)	((c) >= 'a' && ((c) <= 'z'))
#define isalpha(c)	(isupper((c)) || islower(c))
#define tolower(c)	(isupper(c) ? ((c) + 'a' - 'A') : (c))

#endif /* BAS_STRING_H_ */
