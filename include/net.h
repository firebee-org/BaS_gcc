/*
 * File:        net.h
 * Purpose:     Network definitions and prototypes for dBUG.
 *
 * Notes:
 */

#ifndef _NET_H
#define _NET_H

/********************************************************************/

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

/********************************************************************/

int net_init(void);

/********************************************************************/

#endif  /* _NET_H */

