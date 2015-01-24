/*
 * File:        nif.c
 * Purpose:     Network InterFace routines
 *
 * Notes:
 *
 * Modifications:
 *
 */
#include "net.h"
#include "bas_types.h"
#include "bas_printf.h"

#define DBG_NIF
#ifdef DBG_NIF
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DBG_NIF */

int nif_protocol_exist(NIF *nif, uint16_t protocol)
{
	/*
	 * This function searches the list of supported protocols
	 * on the particular NIF and if a protocol handler exists,
	 * true is returned.  This function is useful for network cards
	 * that needn't read in the entire frame but can discard frames
	 * arbitrarily.
	 */
	int index;

	for (index = 0; index < nif->num_protocol; ++index)
	{
		if (nif->protocol[index].protocol == protocol)
		{
			return true;
		}
	}
	return false;
}

void nif_protocol_handler(NIF *nif, uint16_t protocol, NBUF *pNbuf)
{
	/*
	 * This function searches the list of supported protocols
	 * on the particular NIF and if a protocol handler exists,
	 * the protocol handler is invoked.  This routine called by
	 * network device driver after receiving a frame.
	 */
	int index;

	for (index = 0; index < nif->num_protocol; ++index)
	{
		if (nif->protocol[index].protocol == protocol)
		{
			dbg("call protocol handler for protocol %d at %p\r\n", protocol,
						nif->protocol[index].handler);
			nif->protocol[index].handler(nif,pNbuf);
			return;
		}
	}
	dbg("no protocol handler found for protocol %d\r\n", protocol);
}

void *nif_get_protocol_info(NIF *nif, uint16_t protocol)
{
	/*
	 * This function searches the list of supported protocols
	 * on the particular NIF and returns a pointer to the
	 * config info for 'protocol', otherwise NULL is returned.
	 */
	int index;

	for (index = 0; index < nif->num_protocol; ++index)
	{
		if (nif->protocol[index].protocol == protocol)
			return (void *)nif->protocol[index].info;
	}
	return (void *)0;
}

int nif_bind_protocol(NIF *nif, uint16_t protocol, void (*handler)(NIF *,NBUF *),
					void *info)
{
	/*
	 * This function registers 'protocol' as a supported
	 * protocol in 'nif'.
	 */
	if (nif->num_protocol < (MAX_SUP_PROTO - 1))
	{
		nif->protocol[nif->num_protocol].protocol = protocol;
		nif->protocol[nif->num_protocol].handler = (void(*)(NIF *, NBUF *)) handler;
		nif->protocol[nif->num_protocol].info = info;
		++nif->num_protocol;

		return true;
	}
	return false;
}

NIF *nif_init (NIF *nif)
{
	int i;

	for (i = 0; i < ETH_ADDR_LEN; ++i)
	{
		nif->hwa[i] = 0;
		nif->broadcast[i] = 0xFF;
	}

	for (i = 0; i < MAX_SUP_PROTO; ++i)
	{
		nif->protocol[i].protocol = 0;
		nif->protocol[i].handler = 0;
		nif->protocol[i].info = 0;
	}
	nif->num_protocol = 0;

	nif->mtu = 0;
	nif->ch = 0;
	nif->send = 0;

	nif->f_rx = 0;
	nif->f_tx = 0;
	nif->f_rx_err = 0;
	nif->f_tx_err = 0;
	nif->f_err = 0;

	return nif;
}
