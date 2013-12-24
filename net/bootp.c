/*
 * File:        arp.c
 * Purpose:     Address Resolution Protocol routines.
 *
 * Notes:
 */

#include "net.h"
#include "bootp.h"
#include <stdbool.h>
#include <stddef.h>

#define TIMER_NETWORK 0

void bootp_request(NIF *nif, uint8_t *pa)
{
	/*
	 * This function broadcasts a BOOTP request for the protocol
	 * address "pa"
	 */
	uint8_t *addr;
	NBUF *pNbuf;
	bootp_frame_hdr *bootpframe;
	int i, result;

	pNbuf = nbuf_alloc();
	if (pNbuf == NULL)
	{
	    #if defined(DEBUG_PRINT)
	        xprintf("%s: arp_request couldn't allocate Tx buffer\r\n", __FUNCTION__);
	    #endif
	    return;
	}

	bootpframe = (bootp_frame_hdr *) &pNbuf->data[BOOTP_HDR_OFFSET];

	/* Build the BOOTP request packet */
	bootpframe->type = BOOTP_TYPE_BOOTREQUEST;
	bootpframe->htype = BOOTP_HTYPE_ETHERNET;
	bootpframe->hlen = BOOTP_HLEN_ETHERNET;
	bootpframe->hops = 0;
	bootpframe->xid = 0x1234;
	bootpframe->secs = 1;
	bootpframe->flags = BOOTP_FLAGS_BROADCAST;
	bootpframe->cl_addr = 0x0;
	bootpframe->yi_addr = 0x0;
	bootpframe->gi_addr = 0x0;

	addr = &nif->hwa[0];
	for (i = 0; i < 6; i++)
	    bootpframe->ch_addr[i] = addr[i];

	pNbuf->length = BOOTP_HDR_LEN;

	/* Send the BOOTP request */
	result = nif->send(nif, nif->broadcast, nif->hwa, ETH_FRM_IP, pNbuf);

	if (result == 0)
	    nbuf_free(pNbuf);
}

void bootp_handler(NIF *nif, NBUF *pNbuf)
{
	/* 
	 * ARP protocol handler 
	 */
	uint8_t *addr;
	bootp_frame_hdr *rx_bootpframe, *tx_bootpframe;

	rx_bootpframe = (bootp_frame_hdr *) &pNbuf->data[pNbuf->offset];

#ifdef _NOT_USED_
	/* 
	 * Check for an appropriate ARP packet 
	 */
	if ((pNbuf->length < ARP_HDR_LEN)       ||  
	    (rx_arpframe->ar_hrd != ETHERNET)   ||
	    (rx_arpframe->ar_hln != 6)          ||
	    (rx_arpframe->ar_pro != ETH_FRM_IP) ||
	    (rx_arpframe->ar_pln != 4))
	{
	    nbuf_free(pNbuf);
	    return;
	}

	/* 
	 * Check to see if it was addressed to me - if it was, keep this
	 * ARP entry in the table permanently; if not, mark it so that it
	 * can be displaced later if necessary
	 */
	addr = ip_get_myip(nif_get_protocol_info(nif,ETH_FRM_IP));
	if ((rx_arpframe->ar_tpa[0] == addr[0]) &&
	    (rx_arpframe->ar_tpa[1] == addr[1]) &&
	    (rx_arpframe->ar_tpa[2] == addr[2]) &&
	    (rx_arpframe->ar_tpa[3] == addr[3]) )
	{
	    longevity = ARP_ENTRY_PERM;
	}
	else 
	    longevity = ARP_ENTRY_TEMP;

	/* 
	 * Add ARP info into the table
	 */
	arp_merge(arptab,
	          rx_arpframe->ar_pro,
	          rx_arpframe->ar_hln,
	          &rx_arpframe->ar_sha[0],
	          rx_arpframe->ar_pln,
	          &rx_arpframe->ar_spa[0],
	          longevity
	          );

	switch (rx_arpframe->opcode)
	{
	    case ARP_REQUEST:
	        /* 
	         * Check to see if request is directed to me
	         */
	        if ((rx_arpframe->ar_tpa[0] == addr[0]) &&
	            (rx_arpframe->ar_tpa[1] == addr[1]) &&
	            (rx_arpframe->ar_tpa[2] == addr[2]) &&
	            (rx_arpframe->ar_tpa[3] == addr[3]) )
	        {
	            /*
	             * Reuse the current network buffer to assemble an ARP reply
	             */
	            tx_arpframe = (arp_frame_hdr *)&pNbuf->data[ARP_HDR_OFFSET];

	            /*
	             * Build new ARP frame from the received data
	             */
	            tx_arpframe->ar_hrd = ETHERNET;
	            tx_arpframe->ar_pro = ETH_FRM_IP;
	            tx_arpframe->ar_hln = 6;
	            tx_arpframe->ar_pln = 4;
	            tx_arpframe->opcode = ARP_REPLY;
	            tx_arpframe->ar_tha[0] = rx_arpframe->ar_sha[0];
	            tx_arpframe->ar_tha[1] = rx_arpframe->ar_sha[1];
	            tx_arpframe->ar_tha[2] = rx_arpframe->ar_sha[2];
	            tx_arpframe->ar_tha[3] = rx_arpframe->ar_sha[3];
	            tx_arpframe->ar_tha[4] = rx_arpframe->ar_sha[4];
	            tx_arpframe->ar_tha[5] = rx_arpframe->ar_sha[5];
	            tx_arpframe->ar_tpa[0] = rx_arpframe->ar_spa[0];
	            tx_arpframe->ar_tpa[1] = rx_arpframe->ar_spa[1];
	            tx_arpframe->ar_tpa[2] = rx_arpframe->ar_spa[2];
	            tx_arpframe->ar_tpa[3] = rx_arpframe->ar_spa[3];

	            /* 
	             * Now copy in the new information 
	             */
	            addr = &nif->hwa[0];
	            tx_arpframe->ar_sha[0] = addr[0];
	            tx_arpframe->ar_sha[1] = addr[1];
	            tx_arpframe->ar_sha[2] = addr[2];
	            tx_arpframe->ar_sha[3] = addr[3];
	            tx_arpframe->ar_sha[4] = addr[4];
	            tx_arpframe->ar_sha[5] = addr[5];

	            addr = ip_get_myip(nif_get_protocol_info(nif,ETH_FRM_IP));
	            tx_arpframe->ar_spa[0] = addr[0];
	            tx_arpframe->ar_spa[1] = addr[1];
	            tx_arpframe->ar_spa[2] = addr[2];
	            tx_arpframe->ar_spa[3] = addr[3];

	            /* 
	             * Save the length of my packet in the buffer structure 
	             */
	            pNbuf->length = ARP_HDR_LEN;
	            
	            nif->send(nif, 
	                      &tx_arpframe->ar_tha[0], 
	                      &tx_arpframe->ar_sha[0], 
	                      ETH_FRM_ARP, 
	                      pNbuf);
	        }
	        else 
	            nbuf_free(pNbuf);
	        break;
	    case ARP_REPLY:
	        /*
	         * The ARP Reply case is already taken care of
	         */
	    default:
	        nbuf_free(pNbuf);
	        break;
	}
#endif /* _NOT_USED_ */
	return;
}
