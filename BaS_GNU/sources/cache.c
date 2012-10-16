/*
 * cache handling
 */
#include "cache.h"


void flushDataCacheRegion(void *adr, uint32_t length)
{
	__asm__
	(
		"		move.l		%0,d0		| start address\n\t"
		"		move.l		d0,a1\n\t"
		"		add.l		%1,a1		| add length\n\t"
		"		clr.l		d1			| way counter\n\t"
		"		and.l		#0xfffffff0,d0	| align start address\n\t"
		".flush_dregion_way_loop:\n\t"
		"		move.l		d0,a0		| initialize a0\n\t"
		"		add.l		d1,a0		| set way index\n\t"
		".flush_dregion_loop:\n\t"
		"		cpushl		DC,(a0)		| flush and invalidate the cache line\n\t"
		"		add.l		#0x10,a0	| increment to next cache line\n\t"
		"		cmp.l		a0,a1		| done with region?\n\t"
		"		bgt			.flush_dregion_loop\n\t"
		"		addq.l		#1,d1		| increment way counter\n\t"
		"		addq.l		#1,a1		| update stop address to reflect new way value\n\t"
		"		cmp.l		#4,d1		| cache way\n\t"
		"		bne			.flush_dregion_way_loop\n\t"
	/* output */ :
	/* input */  : "g" (adr),
				   "g" (length)
	/* clobber */: "d0", "d1", "a0", "a1"
	);
}

void flushInstructionCacheRegion(void *adr, uint32_t length)
{
	__asm__
	(
		"		move.l		%0,d0		| start address\n\t"
		"		move.l		d0,a1\n\t"
		"		add.l		%1,a1		| add length\n\t"
		"		clr.l		d1			| way counter\n\t"
		"		and.l		#0xfffffff0,d0	| align start address\n\t"
		".flush_iregion_way_loop:\n\t"
		"		move.l		d0,a0		| initialize a0\n\t"
		"		add.l		d1,a0		| set way index\n\t"
		".flush_iregion_loop:\n\t"
		"		cpushl		IC,(a0)		| flush and invalidate the cache line\n\t"
		"		add.l		#0x10,a0	| increment to next cache line\n\t"
		"		cmp.l		a0,a1		| done with region?\n\t"
		"		bgt			.flush_iregion_loop\n\t"
		"		addq.l		#1,d1		| increment way counter\n\t"
		"		addq.l		#1,a1		| update stop address to reflect new way value\n\t"
		"		cmp.l		#4,d1		| cache way\n\t"
		"		bne			.flush_iregion_way_loop\n\t"
	/* output */ :
	/* input */  : "g" (adr),
				   "g" (length)
	/* clobber */: "d0", "d1", "a0", "a1"
	);
}

void clear_caches(void)
{
	__asm__ (
	"cpusha:\n\t"
	"	move		sr,d2\n\t"
	"	move.l		d2,-(sp)\n\t"
	"	move		#0x2700,sr			| no interrupts\n\t"
	"	clr.l		d0\n\t"
	"	clr.l		d1\n\t"
	"	move.l		d0,a0\n\t"
	"cfa_setloop:\n\t"
	"	cpushl		bc,(a0)				| flush\n\t"
	"	lea			0x10(a0),a0			| index+1\n\t"
	"	addq.l		#1,d1				| index+1\n\t"
	"	cmpi.w		#512,d1				| all sets?\n\t"
	"	bne			cfa_setloop			| no->\n\t"
	"	clr.l		d1\n\t"
	"	addq.l		#1,d0\n\t"
	"	move.l		d0,a0\n\t"
	"	cmpi.w		#4,d0				| all ways?\n\t"
	"	bne			cfa_setloop			| no->\n\t"
	"	nop\n\t"
	"	move.l		_rt_cacr,d0			| get cacr shadow register\n\t"
	"	movec		d0,cacr				| set old value\n\t"
	"	move.l		(sp)+,d2\n\t"
	"	move.w		d2,sr				| restore previous interrupt mask\n\t"
	/* input */	:
	/* output */	:
	/* clobber */	: "d0", "d1", "d2", "a0"
	);
}
