/*
 * File:        arp.h
 * Purpose:     ARP definitions.
 *
 * Notes:
 */

#ifndef _ARP_H
#define _ARP_H

/********************************************************************/

/*
 * This data definition is defined for Ethernet only!
 */
typedef struct
{
    uint16_t      ar_hrd;
    uint16_t      ar_pro;
    uint8_t       ar_hln;
    uint8_t       ar_pln;
    uint16_t      opcode;
    uint8_t       ar_sha[6];  /* ethernet hw address */
    uint8_t       ar_spa[4];  /* ip address */
    uint8_t       ar_tha[6];  /* ethernet hw address */
    uint8_t       ar_tpa[4];  /* ip address */
} arp_frame_hdr;

#define ARP_HDR_LEN     sizeof(arp_frame_hdr)

/*
 * ARP table entry definition.  Note that this table only designed
 * with Ethernet and IP in mind.
 */
#define MAX_HWA_SIZE    (6) /* 6 is enough for Ethernet address */
#define MAX_PA_SIZE     (4) /* 4 is enough for Protocol address */
typedef struct
{
    uint16_t  protocol;
    uint8_t   hwa_size;
    uint8_t   hwa[MAX_HWA_SIZE];
    uint8_t   pa_size;
    uint8_t   pa[MAX_PA_SIZE];
    int     longevity;
} ARPENTRY;
#define MAX_ARP_ENTRY   (10)

typedef struct
{
    unsigned int    tab_size;
    ARPENTRY        table[MAX_ARP_ENTRY];
} ARP_INFO;

#define ARP_ENTRY_EMPTY (0)
#define ARP_ENTRY_PERM  (1)
#define ARP_ENTRY_TEMP  (2)


#define ETHERNET        (1)
#define ARP_REQUEST     (1)
#define ARP_REPLY       (2)

#define ARP_TIMEOUT     (1)     /* Timeout in seconds */

/* Protocol Header information */
#define ARP_HDR_OFFSET  ETH_HDR_LEN

/********************************************************************/

uint8_t *
arp_get_mypa (void);

uint8_t *
arp_get_myha (void);

uint8_t *
arp_get_broadcast (void);

void
arp_merge (ARP_INFO *, uint16_t, int, uint8_t *, int, uint8_t *, int);

void
arp_remove (ARP_INFO *, uint16_t, uint8_t *, uint8_t *);

void
arp_request (NIF *, uint8_t *);

void
arp_handler (NIF *, NBUF *);

uint8_t *
arp_resolve (NIF *, uint16_t, uint8_t *);

void
arp_init (ARP_INFO *);

/********************************************************************/

#endif  /* _ARP_H */
