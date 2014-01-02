/*
 * File:    fecbd.c
 * Purpose: Provide a simple buffer management driver
 *
 * Notes:       
 */
#include "MCD_dma.h"
#include "fecbd.h"
#include "nbuf.h"
#include "eth.h"
#include "bas_printf.h"
#include <stddef.h>

#define DBG_FECBD
#ifdef DBG_FECBD
#define dbg(format, arg...) do { xprintf("DEBUG: " format, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DBG_FECBD */

/*
 * This implements a simple static buffer descriptor
 * ring for each channel and each direction
 *
 * FEC Buffer Descriptors need to be aligned to a 4-byte boundary.
 * In order to accomplish this, data is over-allocated and manually
 * aligned at runtime
 * 
 * Enough space is allocated for each of the two FEC channels to have 
 * NRXBD Rx BDs and NTXBD Tx BDs
 * 
 */

FECBD unaligned_bds[(2 * NRXBD) + (2 * NTXBD) + 1];

/*
 * These pointers are used to reference into the chunck of data set 
 * aside for buffer descriptors
 */
FECBD *RxBD;
FECBD *TxBD;

/*
 * Macros to easier access to the BD ring
 */
#define RxBD(ch,i)      RxBD[(ch * NRXBD) + i]
#define TxBD(ch,i)      TxBD[(ch * NTXBD) + i]

/* 
 * Buffer descriptor indexes 
 */
static int iTxbd_new;
static int iTxbd_old;
static int iRxbd;

/*
 * Initialize the FEC Buffer Descriptor ring 
 * Buffer Descriptor format is defined by the MCDAPI
 *
 * Parameters:
 *  ch      FEC channel
 */
void fecbd_init(uint8_t ch)
{
	NBUF *nbuf;
	int i;

	dbg("%s:\r\n", __FUNCTION__);

	/* 
	 * Align Buffer Descriptors to 4-byte boundary 
	 */
	RxBD = (FECBD *)(((int) unaligned_bds + 3) & 0xFFFFFFFC);
	TxBD = (FECBD *)((int) RxBD + (sizeof(FECBD) * 2 * NRXBD));

	dbg("%s: initialise RX buffer descriptor ring\r\n", __FUNCTION__);

	/* 
	 * Initialize the Rx Buffer Descriptor ring 
	 */
	for (i = 0; i < NRXBD; ++i)
	{
		/* Grab a network buffer from the free list */
		nbuf = nbuf_alloc();
		if (nbuf == NULL)
		{
			dbg("%s: could not allocate network buffer\r\n", __FUNCTION__);
			return;
		}

		/* Initialize the BD */
		RxBD(ch,i).status = RX_BD_E | RX_BD_INTERRUPT;
		RxBD(ch,i).length = RX_BUF_SZ;
		RxBD(ch,i).data =  nbuf->data;

		/* Add the network buffer to the Rx queue */
		nbuf_add(NBUF_RX_RING, nbuf);
	}

	/*
	 * Set the WRAP bit on the last one
	 */
	RxBD(ch, i - 1).status |= RX_BD_W;

	dbg("%s: initialise TX buffer descriptor ring\r\n", __FUNCTION__);

	/* 
	 * Initialize the Tx Buffer Descriptor ring 
	 */
	for (i = 0; i < NTXBD; ++i)
	{
		TxBD(ch, i).status = TX_BD_INTERRUPT;
		TxBD(ch, i).length = 0;
		TxBD(ch, i).data = NULL;
	}

	/*
	 * Set the WRAP bit on the last one
	 */
	TxBD(ch, i - 1).status |= TX_BD_W;

	/* 
	 * Initialize the buffer descriptor indexes 
	 */
	iTxbd_new = iTxbd_old = iRxbd = 0;
}

void fecbd_dump(uint8_t ch)
{
#ifdef DBG_FECBD
	int i;

	xprintf("\n------------ FEC%d BDs -----------\n",ch);
	xprintf("RxBD Ring\n");
	for (i = 0; i < NRXBD; i++)
	{
		xprintf("%02d: BD Addr=0x%08x, Ctrl=0x%04x, Lgth=%04d, DataPtr=0x%08x\n",
				i, &RxBD(ch, i), 
				RxBD(ch, i).status, 
				RxBD(ch, i).length, 
				RxBD(ch, i).data);
	}
	xprintf("TxBD Ring\n");
	for (i = 0; i < NTXBD; i++)
	{
		xprintf("%02d: BD Addr=0x%08x, Ctrl=0x%04x, Lgth=%04d, DataPtr=0x%08x\n",
				i, &TxBD(ch, i), 
				TxBD(ch, i).status, 
				TxBD(ch, i).length, 
				TxBD(ch, i).data);
	}
	xprintf("--------------------------------\n\n");
#endif /* DBG_FECBD */
}

/* 
 * Return the address of the first buffer descriptor in the ring.
 *
 * Parameters:
 *  ch          FEC channel
 *  direction   Rx or Tx Macro
 *
 * Return Value:
 *  The start address of the selected Buffer Descriptor ring
 */
uint32_t fecbd_get_start(uint8_t ch, uint8_t direction)
{
	switch (direction)
	{
		case Rx:
			return (uint32_t)((int)RxBD + (ch * sizeof(FECBD) * NRXBD));
		case Tx:
		default:
			return (uint32_t)((int)TxBD + (ch * sizeof(FECBD) * NTXBD));
	}
}

FECBD *fecbd_rx_alloc(uint8_t ch)
{
	int i = iRxbd;

	/* Check to see if the ring of BDs is full */
	if (RxBD(ch, i).status & RX_BD_E)
		return NULL;

	/* Increment the circular index */
	iRxbd = (uint8_t)((iRxbd + 1) % NRXBD);

	return &RxBD(ch, i);
}

/*
 * This function keeps track of the next available Tx BD in the ring
 *
 * Parameters:
 *  ch  FEC channel
 *
 * Return Value:
 *  Pointer to next available buffer descriptor.
 *  NULL if the BD ring is full
 */
FECBD *fecbd_tx_alloc(uint8_t ch)
{
	int i = iTxbd_new;

	/* Check to see if the ring of BDs is full */
	if (TxBD(ch, i).status & TX_BD_R)
		return NULL;

	/* Increment the circular index */
	iTxbd_new = (uint8_t)((iTxbd_new + 1) % NTXBD);

	return &TxBD(ch, i);
}

/*
 * This function keeps track of the Tx BDs that have already been
 * processed by the FEC
 *
 * Parameters:
 *  ch  FEC channel
 *
 * Return Value:
 *  Pointer to the oldest buffer descriptor that has already been sent
 *  by the FEC, NULL if the BD ring is empty
 */
FECBD *fecbd_tx_free(uint8_t ch)
{
	int i = iTxbd_old;

	/* Check to see if the ring of BDs is empty */
	if ((TxBD(ch, i).data == NULL) || (TxBD(ch, i).status & TX_BD_R))
		return NULL;

	/* Increment the circular index */
	iTxbd_old = (uint8_t)((iTxbd_old + 1) % NTXBD);

	return &TxBD(ch, i);
}
