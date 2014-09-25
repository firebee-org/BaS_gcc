/*
 * File:        ip.c
 * Purpose:     Internet Protcol device driver
 *
 * Notes:
 *
 * Modifications:
 */
#include <bas_types.h>
#include "net.h"
#include "bas_printf.h"
#include "bas_string.h"


#define IP_DEBUG
#if defined(IP_DEBUG)
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif

void ip_init(IP_INFO *info, IP_ADDR_P myip, IP_ADDR_P gateway, IP_ADDR_P netmask)
{
	int index;

	for (index = 0; index < sizeof(IP_ADDR); index++)
	{
		info->myip[index] = myip[index];
		info->gateway[index] = gateway[index];
		info->netmask[index] = netmask[index];
		info->broadcast[index] = 0xFF;
	}

	info->rx = 0;
	info->rx_unsup = 0;
	info->tx = 0;
	info->err = 0;
}

uint8_t *ip_get_myip(IP_INFO *info)
{
	if (info != 0)
	{
		return (uint8_t *) &info->myip[0];
	}
	dbg("info is NULL!\n\t");
	return 0;
}

int ip_addr_compare(IP_ADDR_P addr1, IP_ADDR_P addr2)
{
	int i;

	for (i = 0; i < sizeof(IP_ADDR); i++)
	{
		if (addr1[i] != addr2[i])
			return 0;
	}
	return 1;
}

uint8_t *ip_resolve_route(NIF *nif, IP_ADDR_P destip)
{
	/*
	 * This function determines whether or not an outgoing IP
	 * packet needs to be transmitted on the local net or sent
	 * to the router for transmission.
	 */
	IP_INFO *info;
	IP_ADDR mask, result;
	IP_ADDR bc = { 255, 255, 255, 255 };
	int i;

	info = nif_get_protocol_info(nif, ETH_FRM_IP);

	if (memcmp(destip, bc, 4) == 0)
	{
		dbg("destip is broadcast address, no gateway needed\r\n");
		return destip;
	}

	/* create mask for local IP */
	for (i = 0; i < sizeof(IP_ADDR); i++)
	{
		mask[i] = info->myip[i] & info->netmask[i];
	}

	/* apply mask to the destination IP */
	for (i = 0; i < sizeof(IP_ADDR); i++)
	{
		result[i] = mask[i] & destip[i];
	}

	/* See if destination IP is local or not */
	if (ip_addr_compare(mask, result))
	{
		/* The destination IP is on the local net */
		return arp_resolve(nif, ETH_FRM_IP, destip);
	}
	else
	{
		/* The destination IP is not on the local net */
		return arp_resolve(nif, ETH_FRM_IP, info->gateway);
	}
}

int ip_send(NIF *nif, uint8_t *dest, uint8_t *src, uint8_t protocol, NBUF *pNbuf)
{
	/*
	 * This function assembles an IP datagram and passes it
	 * onto the hardware to be sent over the network.
	 */
	uint8_t *route;
	ip_frame_hdr *ipframe;

	/*
	 * Construct the IP header
	 */
	ipframe = (ip_frame_hdr*) &pNbuf->data[IP_HDR_OFFSET];

	/* IP version 4, Internet Header Length of 5 32-bit words */
	ipframe->version_ihl = 0x45;

	/* Type of Service == 0, normal and routine */
	ipframe->service_type = 0x00;

	/* Total length of data */
	ipframe->total_length = (uint16_t) (pNbuf->length + IP_HDR_SIZE);

	/* User defined identification */
	ipframe->identification = 0x0000;

	/* Fragment Flags and Offset -- Don't fragment, last frag */
	ipframe->flags_frag_offset = 0x0000;

	/* Time To Live */
	ipframe->ttl = 0xFF;

	/* Protocol */
	ipframe->protocol = protocol;

	/* Checksum, computed later, zeroed for computation */
	ipframe->checksum = 0x0000;

	/* source IP address */
	ipframe->source_addr[0] = src[0];
	ipframe->source_addr[1] = src[1];
	ipframe->source_addr[2] = src[2];
	ipframe->source_addr[3] = src[3];

	/* dest IP address */
	ipframe->dest_addr[0] = dest[0];
	ipframe->dest_addr[1] = dest[1];
	ipframe->dest_addr[2] = dest[2];
	ipframe->dest_addr[3] = dest[3];

	/* Compute checksum */
	ipframe->checksum = ip_chksum((uint16_t *) ipframe, IP_HDR_SIZE);

	/* Increment the packet length by the size of the IP header */
	pNbuf->length += IP_HDR_SIZE;

	/*
	 * Determine the hardware address of the recipient
	 */
	IP_ADDR bc = { 255, 255, 255, 255};
	if (memcmp(bc, dest, 4) != 0)
	{
		route = ip_resolve_route(nif, dest);
		if (route == NULL)
		{
			dbg("Unable to locate %d.%d.%d.%d\r\n",
					dest[0], dest[1], dest[2], dest[3]);
			return 0;
		}
	}
	else
	{
		route = bc;
		dbg("route = broadcast\r\n");
		dbg("nif = %p\r\n", nif);
		dbg("nif->send = %p\r\n", nif->send);
	}

	return nif->send(nif, route, &nif->hwa[0], ETH_FRM_IP, pNbuf);
}

#if defined(DEBUG_PRINT)
void dump_ip_frame(ip_frame_hdr *ipframe)
{
	xprintf("Version:  %02X\n", ((ipframe->version_ihl & 0x00f0) >> 4));
	xprintf("IHL:      %02X\n", ipframe->version_ihl & 0x000f);
	xprintf("Service:  %02X\n", ipframe->service_type);
	xprintf("Length:   %04X\n", ipframe->total_length);
	xprintf("Ident:    %04X\n", ipframe->identification);
	xprintf("Flags:    %02X\n", ((ipframe->flags_frag_offset & 0xC000) >> 14));
	xprintf("Frag:     %04X\n", ipframe->flags_frag_offset & 0x3FFF);
	xprintf("TTL:      %02X\n", ipframe->ttl);
	xprintf("Protocol: %02X\n", ipframe->protocol);
	xprintf("Chksum:   %04X\n", ipframe->checksum);
	xprintf("Source  : %d.%d.%d.%d\n",
			ipframe->source_addr[0],
			ipframe->source_addr[1],
			ipframe->source_addr[2],
			ipframe->source_addr[3]);
	xprintf("Dest    : %d.%d.%d.%d\n",
			ipframe->dest_addr[0],
			ipframe->dest_addr[1],
			ipframe->dest_addr[2],
			ipframe->dest_addr[3]);
	xprintf("Options: %08X\n", ipframe->options);
}
#endif


uint16_t ip_chksum(uint16_t *data, int num)
{
	int chksum, ichksum;
	uint16_t temp;

	chksum = 0;
	num = num >> 1; /* from bytes to words */
	for (; num; num--, data++)
	{
		temp = *data;
		ichksum = chksum + temp;
		ichksum = ichksum & 0x0000FFFF;
		if ((ichksum < temp) || (ichksum < chksum))
		{
			ichksum += 1;
			ichksum = ichksum & 0x0000FFFF;
		}
		chksum = ichksum;
	}
	return (uint16_t) ~chksum;
}

static int validate_ip_hdr(NIF *nif, ip_frame_hdr *ipframe)
{
	int index, chksum;
	IP_INFO *info;

	/*
	 * Check the IP Version
	 */
	if (IP_VERSION(ipframe) != 4)
		return 0;

	/*
	 * Check Internet Header Length
	 */
	if (IP_IHL(ipframe) < 5)
		return 0;

	/*
	 * Check the destination IP address
	 */
	info = nif_get_protocol_info(nif,ETH_FRM_IP);
	for (index = 0; index < sizeof(IP_ADDR); index++)
		if (info->myip[index] != ipframe->dest_addr[index])
			return 0;

	/*
	 * Check the checksum
	 */
	chksum = (int)((uint16_t) IP_CHKSUM(ipframe));
	IP_CHKSUM(ipframe) = 0;

	if (ip_chksum((uint16_t *) ipframe, IP_IHL(ipframe) * 4) != chksum)
		return 0;

	IP_CHKSUM(ipframe) = (uint16_t) chksum;

	return 1;
}

void ip_handler(NIF *nif, NBUF *pNbuf)
{
	/*
	 * IP packet handler
	 */
	ip_frame_hdr *ipframe;

	dbg("packet received\r\n");

	ipframe = (ip_frame_hdr *) &pNbuf->data[pNbuf->offset];

	/*
	 * Verify valid IP header and destination IP
	 */
	if (!validate_ip_hdr(nif, ipframe))
	{
		dbg("not a valid IP packet!\r\n");

		nbuf_free(pNbuf);
		return;
	}

	pNbuf->offset += (IP_IHL(ipframe) * 4);
	pNbuf->length = (uint16_t)(IP_LENGTH(ipframe) - (IP_IHL(ipframe) * 4));

	/*
	 * Call the appriopriate handler
	 */
	switch (IP_PROTOCOL(ipframe))
	{
		case IP_PROTO_ICMP:
			// FIXME: icmp_handler(nif, pNbuf);
			break;
		case IP_PROTO_UDP:
			udp_handler(nif,pNbuf);
			break;
		default:
			dbg("no protocol handler registered for protocol %d\r\n",
					__FUNCTION__, IP_PROTOCOL(ipframe));
			nbuf_free(pNbuf);
			break;
	}
	return;
}
