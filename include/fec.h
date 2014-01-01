/*
 * File:    fec.h
 * Purpose: Driver for the Fast Ethernet Controller (FEC)
 *
 * Notes:       
 */

#ifndef _FEC_H_
#define _FEC_H_

/********************************************************************/
/* MII Speed Settings */
#define FEC_MII_10BASE_T        0
#define FEC_MII_100BASE_TX      1

/* MII Duplex Settings */
#define FEC_MII_HALF_DUPLEX     0
#define FEC_MII_FULL_DUPLEX     1

/* Timeout for MII communications */
#define FEC_MII_TIMEOUT         0x10000

/* External Interface Modes */
#define FEC_MODE_7WIRE          0
#define FEC_MODE_MII            1
#define FEC_MODE_LOOPBACK       2   /* Internal Loopback */

/*
 * FEC Event Log
 */
typedef struct
{
	int total;      /* total count of errors   */
	int hberr;      /* heartbeat error         */
	int babr;       /* babbling receiver       */
	int babt;       /* babbling transmitter    */
	int gra;        /* graceful stop complete  */
	int txf;        /* transmit frame          */
	int mii;        /* MII                     */
	int lc;         /* late collision          */
	int rl;         /* collision retry limit   */
	int xfun;       /* transmit FIFO underrrun */
	int xferr;      /* transmit FIFO error     */
	int rferr;      /* receive FIFO error      */
	int dtxf;       /* DMA transmit frame      */
	int drxf;       /* DMA receive frame       */
	int rfsw_inv;   /* Invalid bit in RFSW     */
	int rfsw_l;     /* RFSW Last in Frame      */
	int rfsw_m;     /* RFSW Miss               */
	int rfsw_bc;    /* RFSW Broadcast          */
	int rfsw_mc;    /* RFSW Multicast          */
	int rfsw_lg;    /* RFSW Length Violation   */
	int rfsw_no;    /* RFSW Non-octet          */
	int rfsw_cr;    /* RFSW Bad CRC            */
	int rfsw_ov;    /* RFSW Overflow           */
	int rfsw_tr;    /* RFSW Truncated          */
} FEC_EVENT_LOG;


extern int fec_mii_write(uint8_t , uint8_t , uint8_t , uint16_t );
extern int fec_mii_read(uint8_t , uint8_t , uint8_t , uint16_t *);
extern void fec_mii_init(uint8_t, uint32_t);
extern void fec_mib_init(uint8_t);
extern void fec_mib_dump(uint8_t);
extern void fec_log_init(uint8_t);
extern void fec_log_dump(uint8_t);
extern void fec_debug_dump(uint8_t);
extern void fec_duplex (uint8_t, uint8_t);
extern uint8_t fec_hash_address(const uint8_t *);
extern void fec_set_address (uint8_t ch, const uint8_t *);
extern void fec_reset (uint8_t);
extern void fec_init (uint8_t, uint8_t, const uint8_t *);
extern void fec_rx_start(uint8_t, int8_t *);
extern void fec_rx_restart(uint8_t);
extern void fec_rx_stop (uint8_t);
extern void fec_rx_frame(uint8_t, NIF *);
extern void fec0_rx_frame(void);
extern void fec1_rx_frame(void);
extern void fec_tx_start(uint8_t, int8_t *);
extern void fec_tx_restart(uint8_t);
extern void fec_tx_stop (uint8_t);
extern void fec0_tx_frame(void);
extern void fec1_tx_frame(void);
extern int fec_send(uint8_t, NIF *, uint8_t *, uint8_t *, uint16_t , NBUF *);
extern int fec0_send(NIF *, uint8_t *, uint8_t *, uint16_t , NBUF *);
extern int fec1_send(NIF *, uint8_t *, uint8_t *, uint16_t , NBUF *);
extern void fec_irq_enable(uint8_t, uint8_t, uint8_t);
extern void fec_irq_disable(uint8_t);
extern void fec_interrupt_handler(uint8_t);
extern int fec0_interrupt_handler(void *, void *);
extern int fec1_interrupt_handler(void *, void *);
extern void fec_eth_setup(uint8_t, uint8_t, uint8_t, uint8_t, const uint8_t *);
extern void fec_eth_reset(uint8_t);
extern void fec_eth_stop(uint8_t);

#endif /* _FEC_H_ */
