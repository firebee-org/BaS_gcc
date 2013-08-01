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
	/*
	 * set ASID and shadow register
	 */
	__asm__ __volatile__("clr.l d0\n\t"
						 "movec d0,ASID\n\t"
						 "move.l d0,_rt_asid\n\t");

}


__attribute__((interrupt)) void mmutr_miss(void)
{
}
