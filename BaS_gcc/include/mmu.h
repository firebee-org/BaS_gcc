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

#define STD_MMUTR	(MCF_MMU_MMUTR_SG | MCF_MMU_MMUTR_V)
#define MMUORD_D	(MCF_MMU_MMUOR_ACC | MCF_MMU_MMUOR_UAA)
#define MMUORD_I	(MCF_MMU_MMUOR_ITLB | MCF_MMU_MMUOR_ACC | MCF_MMU_MMUOR_UAA)

#define WRITETHROUGH_MMUDR		(MCF_MMU_MMUDR_SZ(00) | MCF_MMU_MMUDR_CM(00) | MCF_MMU_MMUDR_R | MCF_MMU_MMUDR_W | MCF_MMU_MMUDR_X)
#define COPYBACK_MMUDR			(MCF_MMU_MMUDR_SZ(00) | MCF_MMU_MMUDR_CM(01) | MCF_MMU_MMUDR_R | MCF_MMU_MMUDR_W | MCF_MMU_MMUDR_X)
#define NOCACHE_PRECISE_MMUDR	(MCF_MMU_MMUDR_SZ(00) | MCF_MMU_MMUDR_CM(10) | MCF_MMU_MMUDR_R | MCF_MMU_MMUDR_W | MCF_MMU_MMUDR_X)

#define SCA_PAGE_ID		6

/*
 * global variables from linker script
 */
extern long video_tlb;
extern long video_sbt;

extern void mmu_init(void);
extern void mmutr_miss(void) __attribute__((interrupt));

#endif /* _MMU_H_ */
