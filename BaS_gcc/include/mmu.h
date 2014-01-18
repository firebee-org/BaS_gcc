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

#define ACR_AMM(x)                  (((x) & 1) << 10)

#define ACR_CM(x)                   (((x) & 3) << 5)
#define ACR_CM_CACHEABLE_WT         0x0
#define ACR_CM_CACHEABLE_CB         0x1
#define ACR_CM_CACHE_INH_PRECISE    0x2
#define ACR_CM_CACHE_INH_IMPRECISE  0x3

#define ACR_SP(x)           (((x) & 1) << 3)
#define ACR_W(x)            (((x) & 1) << 2)


/*
 * MMU register handling macros
 */

#define SCA_PAGE_ID		6	/* indicates video memory page */

/*
 * MMU page sizes
 */
#define MMU_PAGE_SIZE_1M	0
#define MMU_PAGE_SIZE_4K	1
#define MMU_PAGE_SIZE_8K	2
#define MMU_PAGE_SIZE_1K	3

/*
 * MMU cache modes
 */
#define MMU_CACHE_WRITETHROUGH		0
#define MMU_CACHE_COPYBACK			1
#define MMU_CACHE_NOCACHE_PRECISE	2
#define MMU_CACHE_NOCACHE_IMPRECISE	3

/*
 * global variables from linker script
 */
extern long video_tlb;
extern long video_sbt;

extern void mmu_init(void);
extern void mmu_map_page(uint32_t virt, uint32_t phys, uint32_t map_size, uint32_t map_flags);

#endif /* _MMU_H_ */
