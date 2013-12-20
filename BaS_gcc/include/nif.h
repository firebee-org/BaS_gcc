/*
 * File:        nif.h
 * Purpose:     Definition of a Network InterFace.
 *
 * Notes:
 */

#ifndef _NIF_H
#define _NIF_H

/********************************************************************/

/* 
 * Maximum number of supported protoocls: IP, ARP, RARP 
 */
#define MAX_SUP_PROTO   (3)

typedef struct NIF_t
{
    ETH_ADDR    hwa;        /* ethernet card hardware address */
    ETH_ADDR    broadcast;  /* broadcast address */
    int         mtu;        /* hardware maximum transmission unit */
    int         ch;         /* ethernet channel associated with this NIF */

    struct SUP_PROTO_t
    {
        uint16_t  protocol;
        void    (*handler)(struct NIF_t *, NBUF *);
        void    *info;
    } protocol[MAX_SUP_PROTO];

    unsigned short  num_protocol;

    int     (*send)(struct NIF_t *, uint8_t *, uint8_t *, uint16_t, NBUF *);

    unsigned int    f_rx;
    unsigned int    f_tx;
    unsigned int    f_rx_err;
    unsigned int    f_tx_err;
    unsigned int    f_err;
} NIF;

/********************************************************************/

NIF *
nif_init (NIF *);

int
nif_protocol_exist (NIF *, uint16_t);

void
nif_protocol_handler (NIF *, uint16_t, NBUF *);

void *
nif_get_protocol_info (NIF *, uint16_t);

int
nif_bind_protocol (NIF *, uint16_t, void (*)(NIF *, NBUF *), void *);

/********************************************************************/

#endif /* _NIF_H */
