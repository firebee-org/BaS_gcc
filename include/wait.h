/*
 * wait.h
 *
 *      Author: mfro
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
 * Copyright 2010 - 2012 F. Aschwanden
 * Copyright 2011 - 2012 V. Riviere
 * Copyright 2012        M. Froeschle
 *
 */

#ifndef _WAIT_H_
#define _WAIT_H_

#include <bas_types.h>

#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#elif defined(MACHINE_M5475EVB)
#include "m5475e.h"
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error "unknown machine"
#endif /* MACHINE_FIREBEE */

#include "MCF5475.h"

typedef bool (*checker_func)(void);

extern void wait(uint32_t);
extern void wait_us(uint32_t);  /* this is just an alias to the above */

inline static void udelay(long us)
{
    wait((uint32_t) us);
}

extern bool waitfor(uint32_t us, checker_func condition);
extern uint32_t get_timer(void);
extern void wait_ms(uint32_t ms);

#define US_TO_TIMER(a) ((a) * SYSCLK) / 1000000UL
#define TIMER_TO_US(a) ((a) * 1000000UL) / SYSCLK)

#endif /* _WAIT_H_ */
