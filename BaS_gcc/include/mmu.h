/*
 * mmu.h
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
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */


#ifndef _MMU_H_
#define _MMU_H_

#include <stddef.h>
#include "bas_types.h"

/*
 * ACR register handling macros
 */
#define ACR_BA(x)                   ((x) & 0xffff0000)
#define ACR_ADMSK(x)                (((x) & 0xffff) << 16)
#define ACR_E(x)                    (((x) & 1) << 15)

#define ACR_S(x)                    (((x) & 3) << 13)
#define ACR_S_USERMODE              0
#define ACR_S_SUPERVISOR_MODE       1
#define ACR_S_ALL                   2

#define ACR_ADDRESS_MASK_MODE(x)	(((x) & 1) << 10)

#define ACR_CACHE_MODE(x)			(((x) & 3) << 5)
#define ACR_SUPERVISOR_PROTECT(x)	(((x) & 1) << 3)
#define ACR_WRITE_PROTECT(x)		(((x) & 1) << 2)


/*
 * MMU register handling macros
 */

#define SCA_PAGE_ID		6	/* indicates video memory page */

/*
 * MMU page sizes
 */

enum mmu_page_size
{
    MMU_PAGE_SIZE_1M	= 0,
    MMU_PAGE_SIZE_4K	= 1,
    MMU_PAGE_SIZE_8K	= 2,
    MMU_PAGE_SIZE_1K	= 3
};

#define MMU_PAGE_SIZE_DEFAULT   MMU_PAGE_SIZE_1M    /* note: if this changes, SIZE_DEFAULT below _must_ also change */

#define SIZE_1M             0x100000        /* 1 Megabyte */
#define SIZE_4K             0x1000          /* 4 KB */
#define SIZE_8K             0x2000          /* 8 KB */
#define SIZE_1K             0x400           /* 1 KB */

#define SIZE_DEFAULT        SIZE_1M
/*
 * cache modes
 */
#define CACHE_WRITETHROUGH		0
#define CACHE_COPYBACK			1
#define CACHE_NOCACHE_PRECISE	2
#define CACHE_NOCACHE_IMPRECISE	3


/*
 * page flags
 */
#define SV_PROTECT				1
#define SV_USER					0

#define ACCESS_READ		(1 << 0)
#define	ACCESS_WRITE	(1 << 1)
#define ACCESS_EXECUTE	(1 << 2)


/*
 * global variables from linker script
 */
extern long video_tlb;
extern long video_sbt;

struct mmu_page_descriptor
{
    uint8_t cache_mode          : 2;
    uint8_t supervisor_protect  : 1;
    uint8_t read                : 1;
    uint8_t write               : 1;
    uint8_t execute             : 1;
    uint8_t global              : 1;
    uint8_t locked              : 1;
};

extern void mmu_init(void);
extern uint32_t mmu_map_page(uint32_t virt, uint32_t phys, enum mmu_page_size sz, uint8_t page_id, const struct mmu_page_descriptor *flags);

/*
 * API functions for the BaS driver interface
 */
extern uint32_t mmu_map_data_page_locked(uint32_t address, uint32_t length, int asid);
extern uint32_t mmu_unlock_data_page(uint32_t address, uint32_t length, int asid);
extern uint32_t mmu_report_locked_pages(uint32_t *num_itlb, uint32_t *num_dtlb);
extern uint32_t mmu_report_pagesize(void);
#endif /* _MMU_H_ */
