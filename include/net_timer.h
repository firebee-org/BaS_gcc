/*
 * File:        net_timer.h
 * Purpose:     Provide a timer use by the BaS network as a timeout
 *              indicator
 *
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <bas_types.h>

typedef struct
{
	uint8_t   ch;         /* which channel is this structure for? */
	uint8_t   lvl;        /* Interrupt level for this channel */
	uint8_t   pri;        /* Interrupt priority for this channel */
	uint8_t   reference;  /* timeout indicator */
	uint32_t  gms;        /* mode select register value */
	uint16_t  pre;        /* prescale value */
	uint16_t  cnt;        /* prescaled clocks for timeout */
} NET_TIMER;


extern bool timer_init(uint8_t, uint8_t, uint8_t);
extern bool timer_set_secs(uint8_t ch, uint32_t secs);
extern uint32_t timer_get_reference(uint8_t ch);

/* Vector numbers for all the timer channels */
#define TIMER_VECTOR(x)     (126-x)

#endif /* _TIMER_H_ */
