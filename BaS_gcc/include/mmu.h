/*
 * mmu.h
 *
 *  Created on: 01.08.2013
 *      Author: froesm1
 */

#ifndef _MMU_H_
#define _MMU_H_

#define STD_MMUTR	(MCF_MMU_MMUTR_SG | MCF_MMU_MMUTR_V)
#define MMUORD_D	(MCF_MMU_MMUOR_ACC | MCF_MMU_MMUOR_UAA)
#define MMUORD_I	(MCF_MMU_MMUOR_ITLB | MCF_MMU_MUOR_ACC | MCF_MMU_MMUOR_UAA)

#define WRITETHROUGH_MMUDR		(MCF_MMU_MMUDR_SZ(00) | MCF_MMU_MMUDR_CM(00) | MCF_MMU_MMUDR_R | MCF_MMU_MMUDR_W | MCF_MMU_MMUDR_X)
#define COPYBACK_MMUDR			(MCF_MMU_MMUDR_SZ(00) | MCF_MMU_MMUDR_CM(01) | MCF_MMU_MMUDR_R | MCF_MMU_MMUDR_W | MCF_MMU_MMUDR_X)
#define NOCACHE_PRECISE_MMUDR	(MCF_MMU_MMUDR_SZ(00) | MCF_MMU_MMUDR_CM(10) | MCF_MMU_MMUDR_R | MCF_MMU_MMUDR_W | MCF_MMU_MMUDR_X)

extern void mmu_init(void);
extern void mmutr_miss(void) __attribute__((interrupt));

#endif /* _MMU_H_ */
