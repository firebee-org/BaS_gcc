/*
 * pci.c
 *
 * * Purpose:		PCI configuration for the Coldfire builtin PCI bridge.
 *
 * Notes:
 *
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Created on: 08.01.2013
 *      Author: Markus Froeschle
 */

#include <MCF5475.h>
#include "pci.h"
#include "stdint.h"
#include "bas_printf.h"

uint32_t pci_read_config_longword(uint16_t slot, uint16_t function, uint16_t offset)
{
	return 0;
}


uint16_t pci_read_config_word(uint16_t slot, uint16_t function, uint16_t offset)
{
	return 0;
}

/* start of PCI initialization code */
void init_eport(void)
{
	/* concigure IRQ1-7 pins on EPORT falling edge triggered */
	MCF_EPORT_EPPAR = MCF_EPORT_EPPAR_EPPA7(MCF_EPORT_EPPAR_FALLING) +
			MCF_EPORT_EPPAR_EPPA6(MCF_EPORT_EPPAR_FALLING) +
			MCF_EPORT_EPPAR_EPPA5(MCF_EPORT_EPPAR_FALLING) +
			MCF_EPORT_EPPAR_EPPA4(MCF_EPORT_EPPAR_FALLING) +
			MCF_EPORT_EPPAR_EPPA3(MCF_EPORT_EPPAR_FALLING) +
			MCF_EPORT_EPPAR_EPPA2(MCF_EPORT_EPPAR_FALLING) +
			MCF_EPORT_EPPAR_EPPA1(MCF_EPORT_EPPAR_FALLING);
	MCF_EPORT_EPDDR = 0;	/* clear data direction register. All pins as input */
	MCF_EPORT_EPFR = 0;		/* clear all EPORT interrupt flags */
	MCF_EPORT_EPIER = 0;	/* disable all EPORT interrupts (for now) */
}

void init_xlbus_arbiter(void)
{
	uint8_t clock_ratio;

	/* setup XL bus arbiter */
	clock_ratio = (MCF_PCI_PCIGSCR >> 24) & 0x07;

	if (clock_ratio == 4)
	{
		/* device errata 26: Flexbus hang up in 4:1 clock ratio */
		MCF_PCI_PCIGSCR |= 0x80000000; /* disable pipeline */
	}
	/* FIXME: Firetos (boot2.S, l. 719) looks pretty strange at this place - is this a typo? */
}

void init_pci(void)
{
	xprintf("initializing PCI bridge:");

   MCF_PCIARB_PACR = MCF_PCIARB_PACR_INTMPRI
       + MCF_PCIARB_PACR_EXTMPRI(0x1F)
       + MCF_PCIARB_PACR_INTMINTEN
       + MCF_PCIARB_PACR_EXTMINTEN(0x1F);

   // Setup burst parameters
   MCF_PCI_PCICR1 = MCF_PCI_PCICR1_CACHELINESIZE(4) + MCF_PCI_PCICR1_LATTIMER(32);
   MCF_PCI_PCICR2 = MCF_PCI_PCICR2_MINGNT(16) + MCF_PCI_PCICR2_MAXLAT(16);

   // Turn on error signaling
   MCF_PCI_PCIICR = MCF_PCI_PCIICR_TAE + MCF_PCI_PCIICR_TAE + MCF_PCI_PCIICR_REE + 32;
   MCF_PCI_PCIGSCR |= MCF_PCI_PCIGSCR_SEE;

   /* Configure Initiator Windows */
   /* initiator window 0 base / translation adress register */
   MCF_PCI_PCIIW0BTAR = (PCI_MEMORY_OFFSET + ((PCI_MEMORY_SIZE -1) >> 8)) & 0xffff0000;

   /* initiator window 1 base / translation adress register */
   MCF_PCI_PCIIW1BTAR = (PCI_IO_OFFSET + ((PCI_IO_SIZE - 1) >> 8)) & 0xffff0000;

   /* initiator window 2 base / translation address register */
   MCF_PCI_PCIIW2BTAR = 0L;   /* not used */

   /* initiator window configuration register */
   MCF_PCI_PCIIWCR = MCF_PCI_PCIIWCR_WINCTRL0_MEMRDLINE + MCF_PCI_PCIIWCR_WINCTRL1_IO;

   /* reset PCI devices */
   MCF_PCI_PCIGSCR &= ~MCF_PCI_PCIGSCR_PR;

   xprintf("finished\r\n");
}

