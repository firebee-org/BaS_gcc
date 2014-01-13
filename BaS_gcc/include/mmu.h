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

#define SCA_PAGE_ID		6	/* indicates video memory page */

/*
 * global variables from linker script
 */
extern long video_tlb;
extern long video_sbt;

extern void mmu_init(void);
extern void mmu_map_page(uint32_t virt, uint32_t phys);

#endif /* _MMU_H_ */
