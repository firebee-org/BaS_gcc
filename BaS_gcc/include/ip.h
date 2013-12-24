/*
 * File:        ip.h
 * Purpose:     Definitions for the Internet Protocol, IP.
 *
 * Notes:       See RFC 791 "DARPA Internet Program Protocol
 *              Specification" for more details.
 */

#ifndef _IP_H
#define _IP_H

/********************************************************************/

/* 32-bit IP Addresses */
typedef uint8_t IP_ADDR[4];

/* Pointer to an IP Address */
typedef uint8_t IP_ADDR_P[];

/* Definition of an IP packet header */
typedef struct
{
	uint8_t       version_ihl;
	uint8_t       service_type;
	uint16_t      total_length;
	uint16_t      identification;
	uint16_t      flags_frag_offset;
	uint8_t       ttl;
	uint8_t       protocol;
	uint16_t      checksum;
	IP_ADDR     source_addr;
	IP_ADDR     dest_addr;
	uint8_t       options;    /* actually an array of undetermined length */
} ip_frame_hdr;

/* Macros for accessing an IP datagram.  */
#define IP_VERSION(a)   ((a->version_ihl & 0x00F0) >> 4)
#define IP_IHL(a)       ((a->version_ihl & 0x000F))
#define IP_SERVICE(a)   (a->service_type)
#define IP_LENGTH(a)    (a->total_length)
#define IP_IDENT(a)     (a->identification)
#define IP_FLAGS(a)     ((a->flags_frag_offset & 0x0000E000) >> 13)
#define IP_FRAGMENT(a)  ((a->flags_frag_offset & 0x00001FFF))
#define IP_TTL(a)       (a->ttl)
#define IP_PROTOCOL(a)  (a->protocol)
#define IP_CHKSUM(a)    (a->checksum)
#define IP_SRC(a)       (&a->source_addr[0])
#define IP_DEST(a)      (&a->dest_addr[0])
#define IP_OPTIONS(a)   (&a->options)
#define IP_DATA(a)      (&((uint8_t *)a)[IP_IHL(a) * 4])

/* Defined IP protocols */
#define IP_PROTO_ICMP   (1)
#define IP_PROTO_UDP    (17)

/* Protocol Header information */
#define IP_HDR_OFFSET   ETH_HDR_LEN
#define IP_HDR_SIZE     20  /* no options */

/********************************************************************/

typedef struct
{
	IP_ADDR         myip;
	IP_ADDR         gateway;
	IP_ADDR         netmask;
	IP_ADDR         broadcast;
	unsigned int    rx;
	unsigned int    rx_unsup;
	unsigned int    tx;
	unsigned int    err;
} IP_INFO;

/********************************************************************/

extern void ip_handler(NIF *nif, NBUF *nbf);
uint16_t ip_chksum(uint16_t *data, int num);
extern int ip_send(NIF *nif, uint8_t *dest_addr, uint8_t *src_addr, uint8_t protocol, NBUF *nbf);
extern void ip_init(IP_INFO *, IP_ADDR_P, IP_ADDR_P, IP_ADDR_P);
extern uint8_t *ip_get_myip(IP_INFO *);
extern uint8_t *ip_resolve_route(NIF *, IP_ADDR_P);

#endif  /* _IP_H */
