/*
 * init_fpga.c
 *
 *  Created on: 16.10.2012
 *      Author: Markus Fröschle
 */
#include <MCF5475.h>
#include "sysinit.h"

/*
 * load FPGA
 */
void init_fpga(void)
{
	register uint8_t *fpga_data;
	register int i;

	uart_out_word('FPGA');

	MCF_GPIO_PODR_FEC1L &= ~(1 << 1);	/* FPGA clock => low */
	MCF_GPIO_PODR_FEC1L &= ~(1 << 2);	/* FPGA config => low */

	while (((MCF_GPIO_PPDSDR_FEC1L & (1 << 0))) || ((MCF_GPIO_PPDSDR_FEC1L & (1 << 5))));

	wait_10us();
	MCF_GPIO_PODR_FEC1L |= (1 << 2);
	wait_10us();

	while (!MCF_GPIO_PPDSDR_FEC1L & (1 << 0))
	{
		wait_10us();
	}

	/*
	 * excerpt from an Altera configuration manual:
	 *
	 * The low-to-high transition of nCONFIG on the FPGA begins the configuration cycle. The
	 * configuration cycle consists of 3 stages�reset, configuration, and initialization.
	 * While nCONFIG is low, the device is in reset. When the device comes out of reset,
	 * nCONFIG must be at a logic high level in order for the device to release the open-drain
 	 * nSTATUS pin. After nSTATUS is released, it is pulled high by a pull-up resistor and the FPGA
	 * is ready to receive configuration data. Before and during configuration, all user I/O pins
	 * are tri-stated. Stratix series, Arria series, and Cyclone series have weak pull-up resistors
	 * on the I/O pins which are on, before and during configuration.
	 *
	 * To begin configuration, nCONFIG and nSTATUS must be at a logic high level. You can delay
	 * configuration by holding the nCONFIG low. The device receives configuration data on its
	 * DATA0 pins. Configuration data is latched into the FPGA on the rising edge of DCLK. After
	 * the FPGA has received all configuration data successfully, it releases the CONF_DONE pin,
	 * which is pulled high by a pull-up resistor. A low to high transition on CONF_DONE indicates
	 * configuration is complete and initialization of the device can begin.
	 */
	fpga_data = (uint8_t *) FPGA_FLASH_DATA;
	do
	{
		uint8_t value = *fpga_data++;
		for (i = 0; i < 8; i++)
		{

			if ((value << i) & 0b10000000)
			{
				/* bit set -> toggle DATA0 to high */
				MCF_GPIO_PODR_FEC1L |= (1 << 3);
			}
			else
			{
				/* bit is cleared -> toggle DATA0 to low */
				MCF_GPIO_PODR_FEC1L &= ~(1 << 3);
			}
			/* toggle DCLK -> FPGA reads the bit */
			MCF_GPIO_PODR_FEC1L |= 1;
			MCF_GPIO_PODR_FEC1L &= ~1;
		}
	} while (!(MCF_GPIO_PPDSDR_FEC1L & (1 << 5)) && (fpga_data < FPGA_FLASH_DATA_END));

	if (fpga_data < FPGA_FLASH_DATA_END)
	{
		for (i = 0; i < 4000; i++)
		{
			/* toggle a little more since it's fun ;) */
			MCF_GPIO_PODR_FEC1L |= 1;
			MCF_GPIO_PODR_FEC1L &= ~1;
		}
	}
	else
	{
		uart_out_word(' NOT');
	}
	uart_out_word(' OK!');
	uart_out_word(0x0d0a);
}
