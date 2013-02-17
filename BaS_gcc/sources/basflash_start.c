/*
 * basflash_start.c
 *
 *  Created on: 16.02.2013
 *      Author: mfro
 */

#include <stdint.h>

static uint32_t ownstack[4096];
static uint32_t *stackptr = &ownstack[4095];

/*
 * setup our own stack in SDRAM to prevent clashing BaS's in SRAM (size limited).
 */
void startup(void)
{
	static uint32_t oldstack;

	void basflash(void);
	__asm__ __volatile__("move.l	sp,%0\n\t" : "=g"(oldstack) : :);
	__asm__ __volatile__("move.l	%0,sp\n\t" : : "g"(stackptr) : );
	basflash();
	__asm__ __volatile__("move.l	%0,sp\n\t" : : "g"(oldstack) :);
	(void) stackptr; /* make compiler happy about unused variables */
}
