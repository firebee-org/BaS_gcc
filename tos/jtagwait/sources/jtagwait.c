#include <stdio.h>
#include <mint/osbind.h>
#include <stdint.h>
#include <stdbool.h>

#include "bas_printf.h"
#include "MCF5475.h"
#include "driver_vec.h"

extern bool _FPGA_JTAG_LOADED;
extern long	_FPGA_JTAG_VALID;

#define VALID_JTAG	0xaffeaffe

#define FPGA_CONFIG                     	(1 << 2)
#define FPGA_CONF_DONE                      (1 << 5)

void wait_for_jtag(void)
{
	int i;

	/* set supervisor stack to end of SRAM1 */
	__asm__ __volatile__ (
		"		move	#0x2700,sr\n\t"							/* disable interrupts */
		"		move.l	#0xff101000 + 0x1000 - 4,d0\n\t"		/* 4KB on-chip core SRAM1 */
		"		move.l	d0,sp\n\t"								/* set stack pointer */
		:
		:
		: "d0", "cc"	/* clobber */
	);

	MCF_EPORT_EPIER = 0x0;				/* disable EPORT interrupts */
	MCF_INTC_IMRL = 0xffffffff;
	MCF_INTC_IMRH = 0xffffffff;			/* disable interrupt controller */

	MCF_MMU_MMUCR &= ~MCF_MMU_MMUCR_EN;	/* disable MMU */

	xprintf("relocated supervisor stack, disabled interrupts and disabled MMU\r\n");

	/*
	 * configure FEC1L port directions to enable external JTAG configuration download to FPGA
	 */
	MCF_GPIO_PDDR_FEC1L = 0 |
						  MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L4;	/* bit 4 = LED => output */
															/* all other bits = input */

	xprintf("waiting for JTAG configuration start\r\n");
	/*
	 * now that this GPIO ports have been switched to input, we can poll for FPGA config
	 * started from the JTAG interface (CONF_DONE goes low) and finish (CONF_DONE goes high)
	 */
	while ((MCF_GPIO_PPDSDR_FEC1L & FPGA_CONF_DONE));		/* wait for JTAG config load started */

	xprintf("waiting for JTAG configuration finished\r\n");
	while (!(MCF_GPIO_PPDSDR_FEC1L & FPGA_CONF_DONE));     /* wait for JTAG config load finished */

	xprintf("JTAG configuration finished.\r\n");
	_FPGA_JTAG_LOADED = true;     /* indicate jtag loaded FPGA config to BaS */
	_FPGA_JTAG_VALID = VALID_JTAG;

	/* wait */
	xprintf("wait a little to let things settle...\r\n");
	for (i = 0; i < 1000000; i++);

	__asm__ __volatile__(
		"		jmp		0xe0000000\n\t"
		: : :
	);
}

int main(int argc, char *argv[])
{
	printf("\033E\r\nFPGA JTAG configuration support\r\n");
	printf("You may now savely load a new FPGA configuration through the JTAG interface\r\n"
		   "and your Firebee will reboot once finished using that new configuration.\r\n");
	Supexec(wait_for_jtag);

	return 0;       /* just to make the compiler happy, we will never return */
}

