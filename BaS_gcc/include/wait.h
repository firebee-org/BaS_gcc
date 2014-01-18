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

#if MACHINE_FIREBEE
#include "firebee.h"
#elif MACHINE_M5484LITE
#include "m5484l.h"
#endif /* MACHINE_FIREBEE */

#include "MCF5475.h"

typedef bool (*checker_func)(void);

extern void wait(uint32_t);
extern bool waitfor(uint32_t us, checker_func condition);
extern uint32_t get_timer(void);
extern void wait_ms(uint32_t ms);

#endif /* _WAIT_H_ */
