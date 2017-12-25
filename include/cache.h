#ifndef _CACHE_H_
#define _CACHE_H_

/*
 * cache.h
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

#include <bas_types.h>

/*
 * CACR Cache Control Register
 */
#define CF_CACR_DEC         (0x80000000) /* Data Cache Enable                */
#define CF_CACR_DW          (0x40000000) /* Data default Write-protect       */
#define CF_CACR_DESB        (0x20000000) /* Data Enable Store Buffer         */
#define CF_CACR_DPI         (0x10000000) /* Data Disable CPUSHL Invalidate   */
#define CF_CACR_DHLCK       (0x08000000) /* 1/2 Data Cache Lock Mode         */
#define CF_CACR_DDCM_00     (0x00000000) /* Cacheable writethrough imprecise */
#define CF_CACR_DDCM_01     (0x02000000) /* Cacheable copyback               */
#define CF_CACR_DDCM_10     (0x04000000) /* Noncacheable precise             */
#define CF_CACR_DDCM_11     (0x06000000) /* Noncacheable imprecise           */
#define CF_CACR_DCINVA      (0x01000000) /* Data Cache Invalidate All        */
#define CF_CACR_DDSP        (0x00800000) /* Data default supervisor-protect  */
#define CF_CACR_IVO         (0x00100000) /* Invalidate only                  */
#define CF_CACR_BEC         (0x00080000) /* Branch Cache Enable              */
#define CF_CACR_BCINVA      (0x00040000) /* Branch Cache Invalidate All      */
#define CF_CACR_IEC         (0x00008000) /* Instruction Cache Enable         */
#define CF_CACR_SPA         (0x00004000) /* Search by Physical Address       */
#define CF_CACR_DNFB        (0x00002000) /* Default cache-inhibited fill buf */
#define CF_CACR_IDPI        (0x00001000) /* Instr Disable CPUSHL Invalidate  */
#define CF_CACR_IHLCK       (0x00000800) /* 1/2 Instruction Cache Lock Mode  */
#define CF_CACR_IDCM        (0x00000400) /* Noncacheable Instr default mode  */
#define CF_CACR_ICINVA      (0x00000100) /* Instr Cache Invalidate All       */
#define CF_CACR_IDSP        (0x00000080) /* Ins default supervisor-protect   */
#define CF_CACR_EUSP        (0x00000020) /* Switch stacks in user mode       */
#define CF_CACR_DF          (0x00000010) /* Disable FPU                      */

#define _DCACHE_SET_MASK ((DCACHE_SIZE / 64 - 1) << CACHE_WAYS)
#define _ICACHE_SET_MASK ((ICACHE_SIZE / 64 - 1) << CACHE_WAYS)
#define LAST_DCACHE_ADDR _DCACHE_SET_MASK
#define LAST_ICACHE_ADDR _ICACHE_SET_MASK

#define ICACHE_SIZE 0x8000              /* instruction - 32k */
#define DCACHE_SIZE 0x8000              /* data - 32k */

#define CACHE_LINE_SIZE 0x0010          /* 16 bytes */
#define CACHE_SETS 0x0200               /* 512 sets */
#define CACHE_WAYS 0x0004               /* 4 way */


#define CACHE_DISABLE_MODE      (CF_CACR_DCINVA +        \
                                 CF_CACR_BCINVA +        \
                                 CF_CACR_ICINVA)

#define CACHE_INITIAL_MODE      (CF_CACR_DEC +           \
                                 CF_CACR_BEC +           \
                                 CF_CACR_IEC +           \
                                 CF_CACR_DESB +          \
                                 CF_CACR_EUSP)

extern void flush_and_invalidate_caches(void);
extern uint32_t cacr_get(void);
extern void cacr_set(uint32_t);
extern void flush_icache_range(void *address, size_t size);
extern void flush_dcache_range(void *address, size_t size);
extern void flush_cache_range(void *address, size_t size);


#endif /* _CACHE_H_ */
