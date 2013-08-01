/*
 * mmu.c
 *
 *  Created on: 01.08.2013
 *      Author: froesm1
 */


#include <MCF5475.h>
#include "mmu.h"

void mmu_init(void)
{
	uint32_t regval;
	/*
	 * set ASID and shadow register
	 */
	__asm__ __volatile__("clr.l d0\n\t"
						 "movec d0,ASID\n\t"
						 "move.l d0,_rt_asid\n\t");
	/*
	 * set ACR0 and shadow register
	 */
	__asm__ __volatile__("move.l #0xc03fc040,d0\n\t"
			             "movec  d0,ACR0\n\t"
			             "move.l d0,_rt_acr0\n\t");

	/*
	 * set ACR1 and shadow register
	 */
	__asm__ __volatile__("move.l #0x601fc000, d0\n\t"
			             "movec  d0,ACR1\n\t"
			             "move.l d0,_rt_acr1\n\t");

	/*
	 * set ACR2 and shadow register
	 */
	__asm__ __volatile__("move.l #0xe007c400,d0\n\t"
			             "movec  d0,ACR2\n\t"
			             "move.l d0,_rt_acr2\n\t");

	/*
	 * set ACR3 and shadow register (currently unused)
	 */
	__asm__ __volatile__("clr.l   d0\n\t"
			             "movec   d0,ACR3\n\t"
			             "move.l  d0,_rt_acr3\n\t");

	/*
	 * set MMUBAR
	 */
	__asm__ __volatile__("movel.  #__MMUBAR+1,d0\n\t"
			             "movec   d0,MMUBAR\n\t"
			             "move.l  d0,_rt_mmubar\n\t"
			             "nop\n\t");

	MCF_MMU_MMUOR = MCF_MMU_MMUOR;	/* clear all entries */

	/*
	 * 0x00000000 locked
	 */
	MCF_MMU_MMUTR = 0x0L | STD_MMUTR;
	MCF_MMU_MMUDR = 0x0L | COPYBACK_MMUDR | MCF_MMU_MMUDR_LK;
	MCF_MMU_MMUOR = MMUORD_D;
	MCF_MMU_MMUOR = MMUORD_I;

	/*
	 * 0x00d00000 locked ID=6
	 * video RAM: read write execute normal write true
	 */
	MCF_MMU_MMUTR = 0x00d00000 | MCF_MMU_MMUTR_ID(sca_page_ID) | STD_MMUTR;
	MCF_MMU_MMUDR = 0x60d00000 | WRITETHROUGH_MMUDR | MCF_MMU_MMUDR_LK;
	MCF_MMU_MMUOR = MMUORD_D;
	MCF_MMU_MMUOR = 0x00d00000 | std_mmutr;

	video_tlb = 0x2000;
	video_sbt = 0;
}


__attribute__((interrupt)) void mmutr_miss(void)
{
}
