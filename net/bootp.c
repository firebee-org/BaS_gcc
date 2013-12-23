#include "bas_types.h"
#include <stdint.h>
#include <stdbool.h>

static bool bootp_initialized = false;

/*
 * bootp client state
 */

struct bootp_client
{
	uint8_t state;
	uint8_t mode;
	uint8_t socket_handle;
	uint16_t timer_handle;
	uint16_t boot_secs;
};

static struct bootp_client client;

int bootpc_init(int mode)
{
	if (bootp_initialized)
	{
		return true;
	}
	
	/*
	 * get socket handle
	 */
	client.socket_handle = udp_getsocket

