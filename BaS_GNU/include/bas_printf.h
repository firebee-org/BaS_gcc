#ifndef _BAS_PRINTF_H_
#define _BAS_PRINTF_H_
#include <stdarg.h>
typedef uint32_t size_t;

extern void xvsnprintf(char *str, size_t size, const char *fmt, va_list va);
extern void xvprintf(const char *fmt, va_list va);
extern void xprintf(const char *fmt, ...);
extern void xsnprintf(char *str, size_t size, const char *fmt, ...);

#endif /* _BAS_PRINTF_H_ */
