/*
 * util.h
 *
 * Byteswapping macros lend from EmuTOS sources
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
 *  Created on: 27.10.2013
 *      Author: mfro
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <bas_types.h>

#define NOP() __asm__ __volatile__("nop\n\t" : : : "memory")

/*
 * uint16_t swpw(uint16_t val);
 *   swap endianess of val, 16 bits only.
 */
static inline uint16_t swpw(uint16_t w)
{
    return (w << 8) | (w >> 8);
}

/*
 * uint32_t swpl(uint32_t val);
 *   swap endianess of val, 32 bits only.
 *   e.g. ABCD => DCBA
 */
static inline uint32_t swpl(uint32_t l)
{
    return ((l & 0xff000000) >> 24) | ((l & 0x00ff0000) >> 8) |
           ((l & 0x0000ff00) << 8) | (l << 24);
}


/*
 * WORD swpw2(ULONG val);
 *   swap endianness of val, treated as two 16-bit words.
 *   e.g. ABCD => BADC
 */

#define swpw2(a)                          \
  __extension__                           \
  ({unsigned long _tmp;                   \
	__asm__ __volatile__                  \
	("move.b  (%1),%0\n\t"                \
	 "move.b  1(%1),(%1)\n\t"             \
	 "move.b  %0,1(%1)\n\t"               \
	 "move.b  2(%1),%0\n\t"               \
	 "move.b  3(%1),2(%1)\n\t"            \
	 "move.b  %0,3(%1)"                   \
	: "=d"(_tmp)     /* outputs */        \
	: "a"(&a)        /* inputs  */        \
	: "cc", "memory" /* clobbered */      \
	);                                    \
  })

/*
 * WORD set_sr(WORD new);
 *   sets sr to the new value, and return the old sr value
 */

#define set_sr(a)                         \
__extension__                             \
({short _r, _a = (a);                     \
  __asm__ __volatile__                    \
  ("move.w sr,%0\n\t"                     \
   "move.w %1,sr"                         \
  : "=&d"(_r)       /* outputs */        \
  : "nd"(_a)        /* inputs  */        \
  : "cc", "memory"   /* clobbered */      \
  );                                      \
  _r;                                     \
})


/*
 * WORD get_sr(void);
 *   returns the current value of sr.
 */

#define get_sr()                          \
__extension__                             \
({short _r;                               \
  __asm__ volatile                        \
  ("move.w sr,%0"                         \
  : "=dm"(_r)        /* outputs */        \
  :                  /* inputs  */        \
  : "cc", "memory"   /* clobbered */      \
  );                                      \
  _r;                                     \
})



/*
 * void regsafe_call(void *addr)
 *   Saves all registers to the stack, calls the function
 *   that addr points to, and restores the registers afterwards.
 */
#define regsafe_call(addr)                         \
__extension__                                      \
({__asm__ volatile ("lea     -60(sp),sp\n\t"       \
                    "movem.l d0-d7/a0-a6,(sp)");   \
  ((void (*) (void)) addr)();                      \
  __asm__ volatile ("movem.l (sp),d0-d7/a0-a6\n\t" \
                    "lea     60(sp),sp");          \
})


#endif /* UTIL_H_ */
