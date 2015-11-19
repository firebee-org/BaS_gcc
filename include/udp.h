/*
 * File:        udp.h
 * Purpose:     User Datagram Protocol, UDP, data definitions
 *
 * Notes:
 */

#ifndef _UDP_H
#define _UDP_H

/********************************************************************/

typedef struct
{
    uint16_t      src_port;
    uint16_t      dest_port;
    uint16_t      length;
    uint16_t      chksum;
} udp_frame_hdr;

#define UDP_SOURCE(a)       (a->src_port)
#define UDP_DEST(a)         (a->dest_port)
#define UDP_LENGTH(a)       (a->length)
#define UDP_CHKSUM(a)       (a->chksum)

#define DEFAULT_UDP_PORT    (0x4321)
#define UDP_PORT_TELNET     (23)
#define UDP_PORT_FTP        (21)
#define UDP_PORT_TFTP       (69)

/* Protocol Header information */
#define UDP_HDR_OFFSET      (ETH_HDR_LEN + IP_HDR_SIZE)
#define UDP_HDR_SIZE        8


/********************************************************************/

extern void udp_init(void);
extern void udp_prime_port(uint16_t);
extern uint16_t udp_obtain_free_port(void);
extern void udp_bind_port( uint16_t, void (*)(NIF *,NBUF *));
extern void udp_free_port(uint16_t);
extern int udp_send(NIF *, uint8_t *, int, int, NBUF *);
extern void udp_handler(NIF *, NBUF *);

/********************************************************************/

#endif  /* _UDP_H */
