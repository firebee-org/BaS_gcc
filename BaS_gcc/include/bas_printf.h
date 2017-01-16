/*
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
 */

#ifndef _BAS_PRINTF_H_
#define _BAS_PRINTF_H_
#include <stdarg.h>
#include <stddef.h>
#include "MCF5475.h"

extern void xvsnprintf(char *str, size_t size, const char *fmt, va_list va);
extern void xvprintf(const char *fmt, va_list va);
extern void xprintf(const char *fmt, ...);
extern void xsnprintf(char *str, size_t size, const char *fmt, ...);
extern int sprintf(char *str, const char *format, ...);

extern bool conoutstat(void);
extern bool coninstat(void);
extern void xputchar(int c);
extern char xgetchar(void);


extern void display_progress(void);
extern void hexdump(uint8_t buffer[], int size);


#endif /* _BAS_PRINTF_H_ */
