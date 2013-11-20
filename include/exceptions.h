#ifndef _EXCEPTIONS_H_
#define _EXCEPTIONS_H_

#include <stdint.h>

inline uint32_t set_ipl(uint32_t ipl)
{
	uint32_t ret;

	__asm__ __volatile__(
		"		clr.l	%[ret]\r\n"			/* clear result (mvz.w sr is no valid statement) */
		"		move.w	sr,%[ret]\r\n"			/* retrieve status register */
		"		andi.l	#0x07,%[ipl]\n\t"		/* mask out ipl bits on new value */
		"		lsl.l	#8,%[ipl]\n\t"			/* shift them to position */
		"		move.l	%[ret],d0\n\t"			/* retrieve original value */
		"		andi.l	#0x0000f8ff,d0\n\t"		/* clear ipl part */
		"		or.l	%[ipl],d0\n\t"			/* or in new value */
		"		move.w	d0,sr\n\t"				/* put it in place */
		"		andi.l	#0x0700,%[ret]\r\n"		/* mask out ipl bits */
		"		lsr.l	#8,%[ret]\r\n"			/* shift them to position */
		: [ret] "=d" (ret)		/* output */
		: [ipl] "r" (ipl)		/* input */
		: "d0", "memory"	/* clobber */
	);

	return ret;
}

#endif /* _EXCEPTIONS_H_ */
