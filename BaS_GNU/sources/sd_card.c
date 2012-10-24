/*
 * sd card
 */

#include <stdint.h>
#include <MCF5475.h>

void sd_card_idle(void)
{
	__asm__ __volatile__ (
			".extern	sd_idle\n\t"
			"bsr		sd_idle\n\t"
			/* input */ :
			/* output */:
			/* clobber */: "a0","a1","a2","a3","a4","a5","a6",
			              "d0","d1","d2","d3","d4","d5","d6","d7","memory"
	);
}



int sd_card_init(void)
{

}

