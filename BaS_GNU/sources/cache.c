/*
 * cache handling
 */
#include "cache.h"

void flush_and_invalidate_caches(void)
{
	__asm__ (
	"	clr.l		d0\n\t"
	"	clr.l		d1\n\t"
	"	move.l		d0,a0\n\t"
	"cfa_setloop:\n\t"
	"	cpushl		bc,(a0)				| flush\n\t"
	"	lea			0x10(a0),a0			| index+1\n\t"
	"	addq.l		#1,d1				| index+1\n\t"
	"	cmpi.w		#512,d1				| all sets?\n\t"
	"	bne.s		cfa_setloop			| no->\n\t"
	"	clr.l		d1\n\t"
	"	addq.l		#1,d0\n\t"
	"	move.l		d0,a0\n\t"
	"	cmpi.w		#4,d0				| all ways?\n\t"
	"	bne.s		cfa_setloop			| no->\n\t"
	/* input */	:
	/* output */	:
	/* clobber */	: "d0", "d1", "a0"
	);
}
