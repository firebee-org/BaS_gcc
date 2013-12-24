/*
 * File:        bootp.h
 * Purpose:     BOOTP definitions.
 *
 * Notes:
 */

#ifndef _BOOTP_H_
#define _BOOTP_H_

/********************************************************************/

/*
 * This data definition is defined for Ethernet only!
 */
typedef struct
{
    uint8_t type;			/* bootp operation type */
    uint8_t htype;			/* hardware type */
    uint8_t hlen;			/* hardware address length */
	uint8_t hops;			/* hops */
	uint32_t xid;			/* transaction identifier */
	uint16_t secs;			/* seconds since trying to boot */
	uint16_t flags;			/* only broadcast flag in use */
	uint32_t cl_addr;		/* client ip address. Set to all 0 on request */
	uint32_t yi_addr;		/* this field contains the new IP */
	uint32_t gi_addr;		/* gateway address */
	uint8_t ch_addr[16];	/* client hw address */
	uint8_t sname[64];		/* server name */
	uint8_t file[128];		/* name of bootfile */
	uint8_t vend[64];		/* vendor specific (see below) */
} bootp_frame_hdr;

#define BOOTP_HDR_LEN		sizeof(bootp_frame_hdr)

/* possible values for type field */
#define BOOTP_TYPE_BOOTREQUEST	1
#define BOOTP_TYPE_BOOTREPLY	2

/* values for hardware type - we only use ethernet */
#define BOOTP_HTYPE_ETHERNET	1

/* values for hlen - again only ethernet defined */
#define BOOTP_HLEN_ETHERNET		6

/* values for flags - only broadcast flag in use */
#define BOOTP_FLAGS_BROADCAST	1

#define BOOTP_TIMEOUT     (1)     /* Timeout in seconds */

/* Protocol Header information */
#define BOOTP_HDR_OFFSET  ETH_HDR_LEN

extern void bootp_request(NIF *, uint8_t *);
extern void bootp_handler(NIF *, NBUF *);
//extern void bootp_init(BOOTP_INFO *);

#endif  /* _BOOTP_H_ */
