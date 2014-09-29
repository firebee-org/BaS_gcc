#include <stdio.h>
#include <mint/osbind.h>
#include <stdint.h>
#include <stdbool.h>

#include "driver_vec.h"

struct driver_table *get_bas_drivers(void)
{
	struct driver_table *ret = NULL;

	__asm__ __volatile__(
		"			bra.s	 do_trap			\n\t"
		"			.dc.l	0x5f424153			\n\t"	// '_BAS'
		"do_trap:	trap	#0					\n\t"
		"			move.l	d0,%[ret]			\n\t"
		:	[ret] "=m" (ret)	/* output */
		:						/* no inputs */
		:						/* clobbered */
	);

	return ret;
}

/*
 * temporarily replace the trap 0 handler with this so we can avoid
 * getting caught by BaS versions that don't understand the driver interface
 * exposure call.
 * If we get here, we have a BaS version that doesn't support the trap 0 interface
 */
static void __attribute__((interrupt)) my_own_trap0_handler(void)
{
	__asm__ __volatile__(
		"		clr.l		d0				\n\t"		// return 0 to indicate not supported
		:
		:
		:
	);
}

static uint32_t cookieptr(void)
{
	return * (uint32_t *) 0x5a0L;
}

void setcookie(uint32_t cookie, uint32_t value)
{
	uint32_t *cookiejar = (uint32_t *) Supexec(cookieptr);
	int num_slots;
	int max_slots;

	num_slots = max_slots = 0;
	do
	{
		if (cookiejar[0] == cookie)
		{
			cookiejar[1] = value;
			return;
		}
		cookiejar = &(cookiejar[2]);
		num_slots++;
	} while (cookiejar[-2]);

	/*
	 * Here we are at the end of the list and did not find our cookie.
	 * Let's check if there is any space left and append our value to the
	 * list if so. If not, we are lost (extending the cookie jar does only
	 * work from TSRs)
	 */
	if (cookiejar[-1])
		max_slots = cookiejar[-1];

	if (max_slots > num_slots)
	{
		 /* relief, there is space left, extend the list */
		cookiejar[0] = cookiejar[-2];
		cookiejar[1] = cookiejar[-1];
		/* add the new element */
		cookiejar[-2] = cookie;
		cookiejar[-1] = value;
	}
	else
		printf("cannot set cookie, cookie jar is full!\r\n");
}

# define COOKIE_DMAC	0x444D4143L /* FireTOS DMA API */

static char *dt_to_str(enum driver_type dt)
{
	switch (dt)
	{
		case BLOCKDEV_DRIVER: return "generic block device driver";
		case CHARDEV_DRIVER:  return "generic character device driver";
		case VIDEO_DRIVER:    return "video/framebuffer driver";
		case XHDI_DRIVER:     return "XHDI compatible hard disk driver";
		case MCD_DRIVER:	  return "multichannel DMA driver";
		case PCI_DRIVER:	  return "PCI interface driver";
		case MMU_DRIVER:	  return "MMU lock/unlock pages driver";
		default:              return "unknown driver type";
	}
}

int main(int argc, char *argv[])
{
	struct driver_table *dt;
	void *ssp;
	void *old_vector;
	char *sysbase = (char *) 0x4f2;

	(void) Cconws("retrieve BaS driver interface\r\n");

	ssp = (void *) Super(0L);								/* go to supervisor mode, we are doing dirty tricks */

	/*
	 * first check if we are on EmuTOS, FireTOS want's to do everything itself
	 */
	if (* (long *)(*sysbase + 0x2c) == 0x45544f53)
	{
		old_vector = Setexc(0x20, my_own_trap0_handler);		/* set our own temporarily */
		dt = get_bas_drivers();									/* trap #1 */
		(void) Setexc(0x20, old_vector);						/* restore original vector */

		if (dt)
		{
			struct generic_interface *ifc = &dt->interfaces[0];

			printf("BaS driver table found at %p, BaS version is %d.%d\r\n", dt,
					dt->bas_version, dt->bas_revision);

			while (ifc->type != END_OF_DRIVERS)
			{
				printf("driver \"%s (%s)\" found,\r\n"
					   "interface type is %d (%s),\r\n"
					   "version %d.%d\r\n\r\n",
						ifc->name, ifc->description, ifc->type, dt_to_str(ifc->type),
						ifc->version, ifc->revision);
				if (ifc->type == MCD_DRIVER)
				{
					setcookie(COOKIE_DMAC, (uint32_t) ifc->interface.dma);
					printf("\r\nDMAC cookie set to %p\r\n", ifc->interface.dma);
				}
				ifc++;
			}
		}
	}
	else
	{
		printf("not running on EmuTOS, exiting\r\n");
	}
	Super(ssp);

	while (Cconis()) Cconin();	/* eat keys */
	// printf("press any key to continue\n\r");
	// while (! Cconis());
	return 0;
}

