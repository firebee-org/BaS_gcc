/*
 * File:        udp.c
 * Purpose:     User Datagram Protocol driver
 *
 * Notes:
 *
 * Modifications:
 *
 */
#include "bas_types.h"
#include "bas_printf.h"
#include "net.h"
#include <stddef.h>

//#define DBG_UDP
#if defined(DBG_UDP)
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DBG_UDP */

typedef struct
{
    uint16_t port;
    void (*handler)(NIF *, NBUF *);
} UDP_BOUND_PORT;

#define UDP_MAX_PORTS   (5)     /* plenty for this implementation */


static UDP_BOUND_PORT udp_port_table[UDP_MAX_PORTS];

static uint16_t udp_port;

void udp_init(void)
{
    int index;

    for (index = 0; index < UDP_MAX_PORTS; ++index)
    {
        udp_port_table[index].port = 0;
        udp_port_table[index].handler = 0;
    }

    udp_port = DEFAULT_UDP_PORT;    /* next free port */
}

void udp_prime_port(uint16_t init_port)
{
    udp_port = init_port;
}

void udp_bind_port(uint16_t port, void (*handler)(NIF *, NBUF *))
{
    int index;

    for (index = 0; index < UDP_MAX_PORTS; ++index)
    {
        if (udp_port_table[index].port == 0)
        {
            udp_port_table[index].port = port;
            udp_port_table[index].handler = handler;

            return;
        }
    }
}

void udp_free_port(uint16_t port)
{
    int index;

    for (index = 0; index < UDP_MAX_PORTS; ++index)
    {
        if (udp_port_table[index].port == port)
        {
            udp_port_table[index].port = 0;

            return;
        }
    }
}

static void *udp_port_handler(uint16_t port)
{
    int index;

    for (index = 0; index < UDP_MAX_PORTS; ++index)
    {
        if (udp_port_table[index].port == port)
        {
            return (void *) udp_port_table[index].handler;
        }
    }
    return NULL;
}

uint16_t udp_obtain_free_port(void)
{
    uint16_t port;

    port = udp_port;
    if (--udp_port <= 255)
        udp_port = DEFAULT_UDP_PORT;

    return port;
}

int udp_send(NIF *nif, uint8_t *dest, int sport, int dport, NBUF *pNbuf)
{
    uint8_t *myip;

    if (nif == NULL)
    {
        dbg("nif is NULL\r\n");
        return 0;
    }

    /*
     * This function takes data, creates a UDP frame from it and
     * passes it onto the IP layer
     */
    udp_frame_hdr *udpframe;

    udpframe = (udp_frame_hdr *) &pNbuf->data[UDP_HDR_OFFSET];

    /* Set UDP source port */
    udpframe->src_port = (uint16_t) sport;

    /* Set UDP destination port */
    udpframe->dest_port = (uint16_t) dport;

    /* Set length */
    udpframe->length = (uint16_t) (pNbuf->length + UDP_HDR_SIZE);

    /* No checksum calcualation needed */
    udpframe->chksum = (uint16_t) 0;

    /* Add the length of the UDP packet to the total length of the packet */
    pNbuf->length += 8;

    myip = ip_get_myip(nif_get_protocol_info(nif, ETH_FRM_IP));

    dbg("sent UDP request to %d.%d.%d.%d from %d.%d.%d.%d\r\n",
            dest[0], dest[1], dest[2], dest[3],
            myip[0], myip[1], myip[2], myip[3]);

    return (ip_send(nif, dest, myip, IP_PROTO_UDP, pNbuf));

}

void udp_handler(NIF *nif, NBUF *pNbuf)
{
    /*
     * This function handles incoming UDP packets
     */
    udp_frame_hdr *udpframe;
    void (*handler)(NIF *, NBUF *);

    udpframe = (udp_frame_hdr *) &pNbuf->data[pNbuf->offset];

    dbg("packet received\r\n",);

    /*
     * Adjust the length and valid data offset of the packet we are
     * passing on
     */
    pNbuf->length -= UDP_HDR_SIZE;
    pNbuf->offset += UDP_HDR_SIZE;

    /*
     * Traverse the list of bound ports to see if there is a higher
     * level protocol to pass the packet on to
     */
    if ((handler = (void(*)(NIF*, NBUF*)) udp_port_handler(UDP_DEST(udpframe))) != NULL)
        handler(nif, pNbuf);
    else
    {
        dbg("received UDP packet for non-supported port\n");
        nbuf_free(pNbuf);
    }

    return;
}
