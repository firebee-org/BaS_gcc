/*
 * tc.printf.c: A public-domain, minimal printf/sprintf routine that prints
 *	       through the putchar() routine.  Feel free to use for
 *	       anything...  -- 7/17/87 Paul Placeway
 */
/*-
 * Copyright (c) 1980, 1991 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdarg.h>
#include "MCF5475.h"
#include "bas_printf.h"
#include "bas_string.h"

/*
 * Lexical definitions.
 *
 * All lexical space is allocated dynamically.
 * The eighth/sixteenth bit of characters is used to prevent recognition,
 * and eventually stripped.
 */
#define	META		0200
#define	ASCII		0177
#define	QUOTE		((char)	0200)	/* Eighth char bit used for 'ing */
#define	TRIM		0177	/* Mask to strip quote bit */
#define	UNDER		0000000	/* No extra bits to do both */
#define	BOLD		0000000	/* Bold flag */
#define	STANDOUT	META	/* Standout flag */
#define	LITERAL		0000000	/* Literal character flag */
#define	ATTRIBUTES	0200	/* The bits used for attributes */
#define	CHAR		0000177	/* Mask to mask out the character */

#define INF	32766		/* should be bigger than any field to print */

static char snil[] = "(nil)";

void xputchar(int c)
{
	__asm__ __volatile__
	(
			".extern		printf_helper\n\t"
			"move.b			%0,d0\n\t"
			"bsr			printf_helper\n\t"
			/* output */:
			/* input */: "r" (c)
			/* clobber */: "d0","d2","a0","memory"
	);
}

static void doprnt(void (*addchar)(int), const char *sfmt, va_list ap)
{
	char buf[128];
	char *bp;
	const char *f;
	long l;
	unsigned long u;
	int i;
	int fmt;
	unsigned char pad = ' ';
	int flush_left = 0;
	int f_width = 0;
	int prec = INF;
	int hash = 0;
	int do_long = 0;
	int sign = 0;
	int attributes = 0;

	f = sfmt;
	for (; *f; f++)
	{
		if (*f != '%')
		{
			/* then just out the char */
			(*addchar)((int) (((unsigned char) *f) | attributes));
		}
		else
		{
			f++; /* skip the % */

			if (*f == '-')
			{ /* minus: flush left */
				flush_left = 1;
				f++;
			}

			if (*f == '0' || *f == '.')
			{
				/* padding with 0 rather than blank */
				pad = '0';
				f++;
			}
			if (*f == '*')
			{
				/* field width */
				f_width = va_arg(ap, int);
				f++;
			}
			else if (isdigit((unsigned char)*f))
			{
				f_width = atoi(f);
				while (isdigit((unsigned char)*f))
					f++; /* skip the digits */
			}

			if (*f == '.')
			{ /* precision */
				f++;
				if (*f == '*')
				{
					prec = va_arg(ap, int);
					f++;
				}
				else if (isdigit((unsigned char)*f))
				{
					prec = atoi(f);
					while (isdigit((unsigned char)*f))
						f++; /* skip the digits */
				}
			}

			if (*f == '#')
			{ /* alternate form */
				hash = 1;
				f++;
			}

			if (*f == 'l')
			{ /* long format */
				do_long++;
				f++;
				if (*f == 'l')
				{
					do_long++;
					f++;
				}
			}

			fmt = (unsigned char) *f;
			if (fmt != 'S' && fmt != 'Q' && isupper(fmt))
			{
				do_long = 1;
				fmt = tolower(fmt);
			}
			bp = buf;
			switch (fmt)
			{ /* do the format */
			case 'd':
				switch (do_long)
				{
				case 0:
					l = (long) (va_arg(ap, int));
					break;
				case 1:
				default:
					l = va_arg(ap, long);
					break;
				}

				if (l < 0)
				{
					sign = 1;
					l = -l;
				}
				do
				{
					*bp++ = (char) (l % 10) + '0';
				} while ((l /= 10) > 0);
				if (sign)
					*bp++ = '-';
				f_width = f_width - (int) (bp - buf);
				if (!flush_left)
					while (f_width-- > 0)
						(*addchar)((int) (pad | attributes));
				for (bp--; bp >= buf; bp--)
					(*addchar)((int) (((unsigned char) *bp) | attributes));
				if (flush_left)
					while (f_width-- > 0)
						(*addchar)((int) (' ' | attributes));
				break;

			case 'p':
				do_long = 1;
				hash = 1;
				fmt = 'x';
				/* no break */
			case 'o':
			case 'x':
			case 'u':
				switch (do_long)
				{
				case 0:
					u = (unsigned long) (va_arg(ap, unsigned int));
					break;
				case 1:
				default:
					u = va_arg(ap, unsigned long);
					break;
				}
				if (fmt == 'u')
				{ /* unsigned decimal */
					do
					{
						*bp++ = (char) (u % 10) + '0';
					} while ((u /= 10) > 0);
				}
				else if (fmt == 'o')
				{ /* octal */
					do
					{
						*bp++ = (char) (u % 8) + '0';
					} while ((u /= 8) > 0);
					if (hash)
						*bp++ = '0';
				}
				else if (fmt == 'x')
				{ /* hex */
					do
					{
						i = (int) (u % 16);
						if (i < 10)
							*bp++ = i + '0';
						else
							*bp++ = i - 10 + 'a';
					} while ((u /= 16) > 0);
					if (hash)
					{
						*bp++ = 'x';
						*bp++ = '0';
					}
				}
				i = f_width - (int) (bp - buf);
				if (!flush_left)
					while (i-- > 0)
						(*addchar)((int) (pad | attributes));
				for (bp--; bp >= buf; bp--)
					(*addchar)((int) (((unsigned char) *bp) | attributes));
				if (flush_left)
					while (i-- > 0)
						(*addchar)((int) (' ' | attributes));
				break;

			case 'c':
				i = va_arg(ap, int);
				(*addchar)((int) (i | attributes));
				break;

			case 'S':
			case 'Q':
			case 's':
			case 'q':
				bp = va_arg(ap, char *);
				if (!bp)
					bp = snil;
				f_width = f_width - strlen((char *) bp);
				if (!flush_left)
					while (f_width-- > 0)
						(*addchar)((int) (pad | attributes));
				for (i = 0; *bp && i < prec; i++)
				{
					if (fmt == 'q' && (*bp & QUOTE))
						(*addchar)((int) ('\\' | attributes));
					(*addchar)(
							(int) (((unsigned char) *bp & TRIM) | attributes));
					bp++;
				}
				if (flush_left)
					while (f_width-- > 0)
						(*addchar)((int) (' ' | attributes));
				break;

			case 'a':
				attributes = va_arg(ap, int);
				break;

			case '%':
				(*addchar)((int) ('%' | attributes));
				break;

			default:
				break;
			}
			flush_left = 0, f_width = 0, prec = INF, hash = 0, do_long = 0;
			sign = 0;
			pad = ' ';
		}
	}
}

static char *xstring, *xestring;

static void xaddchar(int c)
{
	if (xestring == xstring)
		*xstring = '\0';
	else
		*xstring++ = (char) c;
}

void xsnprintf(char *str, size_t size, const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);

	xstring = str;
	xestring = str + size - 1;
	doprnt(xaddchar, fmt, va);
	va_end(va);
	*xstring++ = '\0';
}

void xprintf(const char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	doprnt(xputchar, fmt, va);
	va_end(va);
}

void xvprintf(const char *fmt, va_list va)
{
	doprnt(xputchar, fmt, va);
}

void xvsnprintf(char *str, size_t size, const char *fmt, va_list va)
{
	xstring = str;
	xestring = str + size - 1;
	doprnt(xaddchar, fmt, va);
	*xstring++ = '\0';
}


void display_progress()
{
	static int _progress_index;
	char progress_char[] = "|/-\\";

	xputchar(progress_char[_progress_index++ % strlen(progress_char)]);
	xputchar('\r');
}

void hexdump(uint8_t buffer[], int size)
{
   int i;
   int line = 0;
   uint8_t *bp = buffer;

   while (bp < buffer + size) {
      uint8_t *lbp = bp;

      xprintf("%08x  ", line);

      for (i = 0; i < 16; i++) {
         if (bp + i > buffer + size) {
            break;
         }
         xprintf("%02x ", (uint8_t) *lbp++);
      }

      lbp = bp;
      for (i = 0; i < 16; i++) {
         int8_t c = *lbp++;

         if (bp + i > buffer + size) {
            break;
         }
         if (c > ' ' && c < '~') {
            xprintf("%c", c);
         } else {
            xprintf(".");
         }
      }
      xprintf("\r\n");

      bp += 16;
      line += 16;
   }
}
