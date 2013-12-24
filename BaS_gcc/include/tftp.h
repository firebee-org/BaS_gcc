/*
 * File:        tftp.h
 * Purpose:     Data definitions for TFTP
 *
 * Notes:
 */

#ifndef _TFTP_H_
#define _TFTP_H_

/********************************************************************/

#define TFTP_RRQ            (1)
#define TFTP_WRQ            (2)
#define TFTP_DATA           (3)
#define TFTP_ACK            (4)
#define TFTP_ERROR          (5)

#define TFTP_ERR_FNF        1
#define TFTP_ERR_AV         2
#define TFTP_ERR_DF         3
#define TFTP_ERR_ILL        4
#define TFTP_ERR_TID        5
#define TFTP_FE             6
#define TFTP_NSU            7
#define TFTP_ERR_UD         0

#define OCTET               "octet"
#define NETASCII            "netascii"

/* Protocol Header information */
#define TFTP_HDR_OFFSET     (ETH_HDR_LEN + IP_HDR_SIZE + UDP_HDR_SIZE)

/* Timeout in seconds */
#define TFTP_TIMEOUT        2

/* Maximum TFTP Packet Size (payload only - no header) */
#define TFTP_PKTSIZE        512

/* Number of TFTP Data Buffers */
#define NUM_TFTPBD          6


/* Data Buffer Pointer Structure */
typedef struct
{
	uint8_t   data[TFTP_PKTSIZE];
	uint16_t  bytes;
} DATA_BUF;

/* TFTP RRQ/WRQ Packet */
typedef struct
{
	uint16_t  opcode;
	char    filename_mode[TFTP_PKTSIZE - 2];
} RWRQ;

/* TFTP DATA Packet */
typedef struct 
{
	uint16_t  opcode;
	uint16_t  blocknum;
	uint8_t   data[TFTP_PKTSIZE - 4];
} DATA;

/* TFTP Acknowledge Packet */
typedef struct
{
	uint16_t  opcode;
	uint16_t  blocknum;
} ACK;

/* TFTP Error Packet */
typedef struct
{
	uint16_t  opcode;
	uint16_t  code;
	char    msg[TFTP_PKTSIZE - 4];
} ERROR;

/* TFTP Generic Packet */
typedef struct
{
	uint16_t opcode;
} GEN;

union TFTPpacket
{
	RWRQ    rwrq;
	DATA    data;
	ACK     ack;
	ERROR   error;
	GEN     generic;
};

/* TFTP Connection Status */
typedef struct
{
	/* Pointer to next character in buffer ring */
	uint8_t *next_char;

	/* Direction of current connection, read or write */
	uint8_t dir;

	/* Connection established flag */
	uint8_t open;

	/* Pointer to our Network InterFace */
	NIF *nif;

	/* File being transferred */
	char *file;

	/* Server IP address */
	IP_ADDR server_ip;

	/* Queue to hold the TFTP packets */
	QUEUE queue;

	/* Bytes received counter */
	uint32_t bytes_recv;

	/* Bytes sent counter */
	uint32_t bytes_sent;

	/* Bytes remaining in current Rx buffer */
	uint32_t rem_bytes;

	/* Server UDP port */
	uint16_t  server_port;

	/* My UDP port */
	uint16_t  my_port;

	/* Expected TFTP block number */
	uint16_t exp_blocknum;

	/* Keep track of the last packet acknowledged */
	uint16_t last_ack;

	/* Error Flag */
	uint8_t error;

} TFTP_Connection;


extern void tftp_handler(NIF *, NBUF *) ;
extern int tftp_write (NIF *, char *, IP_ADDR_P, uint32_t, uint32_t);
extern int tftp_read(NIF *, char *, IP_ADDR_P);
extern void tftp_end(int);
extern int tftp_in_char(void);


#endif  /* _TFTP_H_ */
