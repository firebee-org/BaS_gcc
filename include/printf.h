#ifndef _PRINTF_H_
#define _PRINTF_H_

extern void xvsnprintf(char *str, size_t size, const char *fmt, va_list va);
extern void xvprintf(const char *fmt, va_list va);
extern void xprintf(const char *fmt, ...);
extern void xsnprintf(char *str, size_t size, const char *fmt, ...);

#endif /* _PRINTF_H_ */
