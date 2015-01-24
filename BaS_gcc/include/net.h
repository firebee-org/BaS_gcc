/*
 * File:        net.h
 * Purpose:     Network definitions and prototypes for BaS.
 *
 * Notes:
 */

#ifndef _NET_H
#define _NET_H


/*
 * Include information and prototypes for all protocols
 */
#include "eth.h"
#include "nbuf.h"
#include "nif.h"
#include "ip.h"
#include "icmp.h"
#include "arp.h"
#include "udp.h"
#include "tftp.h"

#define TIMER_NETWORK	3		/* use GPT3 for network timers */
#define TMR_INTC_LVL	3		/* interrupt level for network timer */
#define TMR_INTC_PRI	0		/* interrupt priority for network timer */

extern int net_init(void);


#endif  /* _NET_H */

