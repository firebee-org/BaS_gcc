/*
 * File:        bootp.c
 * Purpose:     Address Resolution Protocol routines.
 *
 * Notes:
 */

#include "net.h"
#include "bootp.h"
#include <stdbool.h>
#include <stddef.h>
#include "bas_printf.h"

#define DBG_BOOTP
#ifdef DBG_BOOTP
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DBG_BOOTP */

#define TIMER_NETWORK 3			/* defines GPT3 as timer for this function */

static struct bootp_connection connection;
#define XID			0x1234		/* this is arbitrary */
#define	MAX_TRIES	5			/* since UDP can fail */

void bootp_request(NIF *nif, uint8_t *pa)
{
	/*
	 * This function broadcasts a BOOTP request for the protocol
	 * address "pa"
	 */
	uint8_t *addr;
	IP_ADDR broadcast = {255, 255, 255, 255};
	NBUF *nbuf;
	struct bootp_packet *p;
	int i, result;

	nbuf = nbuf_alloc();
	if (nbuf == NULL)
	{
		xprintf("%s: couldn't allocate Tx buffer\r\n", __FUNCTION__);
		return;
	}

	p = (struct bootp_packet *) &nbuf->data[BOOTP_HDR_OFFSET];

	/* Build the BOOTP request packet */
	p->type = BOOTP_TYPE_BOOTREQUEST;
	p->htype = BOOTP_HTYPE_ETHERNET;
	p->hlen = BOOTP_HLEN_ETHERNET;
	p->hops = 0;
	p->xid = XID;
	p->secs = 1;
	p->flags = BOOTP_FLAGS_BROADCAST;
	p->cl_addr = 0x0;
	p->yi_addr = 0x0;
	p->gi_addr = 0x0;

	connection.nif = nif;
	addr = &nif->hwa[0];

	for (i = 0; i < 6; i++)
		p->ch_addr[i] = addr[i];

	nbuf->length = BOOTP_PACKET_LEN;

	/* setup reply handler */
	udp_bind_port(BOOTP_CLIENT_PORT, bootp_handler);

	for (i = 0; i < MAX_TRIES; i++)
	{
		/* Send the BOOTP request */
		result = udp_send(connection.nif, broadcast, BOOTP_CLIENT_PORT,
							BOOTP_SERVER_PORT, nbuf);
		dbg("sent bootp request\r\n");
		if (result == true)
			break;
	}

	/* release handler */
	udp_free_port(BOOTP_CLIENT_PORT);

	if (result == 0)
		nbuf_free(nbuf);
}

void bootp_handler(NIF *nif, NBUF *nbuf)
{
	/* 
	 * BOOTP protocol handler 
	 */
	uint8_t *addr;
	struct bootp_packet *rx_p;
	udp_frame_hdr *udpframe;

    dbg("\r\n");

	rx_p = (struct bootp_packet *) &nbuf->data[nbuf->offset];
	udpframe = (udp_frame_hdr *) &nbuf->data[nbuf->offset - UDP_HDR_SIZE];

	/* check packet if it is valid and if it is really intended for us */

	if (rx_p->type == BOOTP_TYPE_BOOTREPLY && rx_p->xid == XID)
	{
		dbg("received bootp reply\r\n");
		/* seems to be valid */
	
	}
	else
	{
		/* not valid */
		return;
	}
}
