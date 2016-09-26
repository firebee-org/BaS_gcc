/*
 * cache handling
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

#include "cache.h"

void cacr_set(uint32_t value)
{
    extern uint32_t rt_cacr;

    rt_cacr = value;
    __asm__ __volatile__(
        "           movec	%0, cacr\n\t"
        : /* output */
        : "r" (rt_cacr)
        : "memory" /* clobbers */);
}

uint32_t cacr_get(void)
{
    extern uint32_t rt_cacr;

    return rt_cacr;
}

void disable_data_cache(void)
{
    flush_and_invalidate_caches();
    cacr_set((cacr_get() | CF_CACR_DCINVA) & ~CF_CACR_DEC);
}

void disable_instruction_cache(void)
{
    flush_and_invalidate_caches();
    cacr_set((cacr_get() | CF_CACR_ICINVA) & ~CF_CACR_IEC);
}

void enable_data_cache(void)
{
    cacr_set(cacr_get() & ~CF_CACR_DCINVA);
}

void flush_and_invalidate_caches(void)
{
    __asm__ __volatile__(
            "	clr.l		d0                  \n\t"
            "	clr.l		d1                  \n\t"
            "	move.l		d0,a0               \n\t"
            "1:                                 \n\t"
            "	cpushl		bc,(a0)				| flush\n\t"
            "	lea			0x10(a0),a0			| index+1\n\t"
            "	addq.l		#1,d1				| index+1\n\t"
            "	cmpi.w		#512,d1				| all sets?\n\t"
            "	bne.s		1b					| no->\n\t"
            "	clr.l		d1                  \n\t"
            "	addq.l		#1,d0               \n\t"
            "	move.l		d0,a0               \n\t"
            "	cmpi.w		#4,d0				| all ways?\n\t"
            "	bne.s		1b					| no->\n\t"
            /* input */	:
            /* output */	:
            /* clobber */	: "cc", "d0", "d1", "a0"
    );
}

/*
 * flush and invalidate a specific memory region from the instruction cache
 */
void flush_icache_range(void *address, size_t size)
{
    uint32_t set;
    uint32_t start_set;
    uint32_t end_set;
    void *endaddr = address + size;

    start_set = (uint32_t) address & _ICACHE_SET_MASK;
    end_set = (uint32_t) endaddr & _ICACHE_SET_MASK;

    if (start_set > end_set) {
        /* from the begining to the lowest address */
        for (set = 0; set <= end_set; set += (0x10 - 3))
        {
            __asm__ __volatile__(
                        "       cpushl  ic,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  ic,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  ic,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  ic,(%[set])         \n\t"
                 : /* output parameters  */
                 : [set] "a" (set)  /* input parameters */
                 : "cc"             /* clobbered registers */
            );
        }
        /* next loop will finish the cache ie pass the hole */
        end_set = LAST_ICACHE_ADDR;
    }
    for (set = start_set; set <= end_set; set += (0x10 - 3)) {
        __asm__ __volatile__(
                    "       cpushl  ic,(%[set])             \n\t"
                    "       addq.l  #1,%[set]               \n\t"
                    "       cpushl  ic,(%[set])             \n\t"
                    "       addq.l  #1,%[set]               \n\t"
                    "       cpushl  ic,(%[set])             \n\t"
                    "       addq.l  #1,%[set]               \n\t"
                    "       cpushl  ic,(%[set])"
                    : /* output parameters */
                    : [set] "a" (set)
                    : "cc"
         );
    }
}



/*
 * flush and invalidate a specific region from the data cache
 */
void flush_dcache_range(void *address, size_t size)
{
    unsigned long set;
    unsigned long start_set;
    unsigned long end_set;
    void *endaddr;

    endaddr = address + size;
    start_set = (uint32_t) address & _DCACHE_SET_MASK;
    end_set = (uint32_t) endaddr & _DCACHE_SET_MASK;

    if (start_set > end_set) {
        /* from the begining to the lowest address */
        for (set = 0; set <= end_set; set += (0x10 - 3))
        {
            __asm__ __volatile__(
                        "       cpushl  dc,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  dc,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  dc,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  dc,(%[set])         \n\t"
                        : /* output parameters */
                        : [set] "a" (set)
                        : "cc" /* clobbered registers */
            );
        }
        /* next loop will finish the cache ie pass the hole */
        end_set = LAST_DCACHE_ADDR;
    }
    for (set = start_set; set <= end_set; set += (0x10 - 3))
    {
        __asm__ __volatile__(
                    "       cpushl      dc,(%[set])     \n\t"
                    "       addq.l      #1,%[set]       \n\t"
                    "       cpushl      dc,(%[set])     \n\t"
                    "       addq%.l     #1,%[set]       \n\t"
                    "       cpushl      dc,(%[set])     \n\t"
                    "       addq.l      #1,%[set]       \n\t"
                    "       cpushl      dc,(%[set])     \n\t"
                    : /* output parameters */
                    : [set] "a" (set)
                    : "cc" /* clobbered registers */
        );
    }
}


/*
 * flush and invalidate a specific region from the both caches. We do not know if the area is cached
 * at all, we do not know in which of the four ways it is cached, but we know the index where they
 * would be cached if they are, so we only need to flush and invalidate only a subset of the 512 index
 * entries, but all four ways.
 */
void flush_cache_range(void *address, size_t size)
{
    unsigned long set;
    unsigned long start_set;
    unsigned long end_set;
    void *endaddr;

    endaddr = address + size;
    start_set = (uint32_t) address & _DCACHE_SET_MASK;
    end_set = (uint32_t) endaddr & _DCACHE_SET_MASK;

    if (start_set > end_set) {
        /* from the begining to the lowest address */
        for (set = 0; set <= end_set; set += (0x10 - 3))
        {
            __asm__ __volatile__(
                        "       cpushl  bc,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  bc,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  bc,(%[set])         \n\t"
                        "       addq.l  #1,%[set]           \n\t"
                        "       cpushl  bc,(%[set])         \n\t"
                        : /* output parameters */
                        : [set] "a" (set)
                        : "cc" /* clobbered registers */
            );
        }
        /* next loop will finish the cache ie pass the hole */
        end_set = LAST_DCACHE_ADDR;
    }
    for (set = start_set; set <= end_set; set += (0x10 - 3))
    {
        __asm__ __volatile__(
                    "       cpushl      bc,(%[set])     \n\t"
                    "       addq.l      #1,%[set]       \n\t"
                    "       cpushl      bc,(%[set])     \n\t"
                    "       addq%.l     #1,%[set]       \n\t"
                    "       cpushl      bc,(%[set])     \n\t"
                    "       addq.l      #1,%[set]       \n\t"
                    "       cpushl      bc,(%[set])     \n\t"
                    : /* output parameters */
                    : [set] "a" (set)
                    : "cc" /* clobbered registers */
        );
    }
}
