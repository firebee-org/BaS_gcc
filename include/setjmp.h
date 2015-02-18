#ifndef _SETJMP_H_
#define _SETJMP_H_

#include "bas_types.h"

typedef uint32_t jmp_buf[18];

extern int setjmp(jmp_buf env);
extern void longjmp(jmp_buf env, int val);

#endif /* _SETJMP_H_ */
