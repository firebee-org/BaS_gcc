/*
 * pci.c
 *
 *  Created on: 08.01.2013
 *      Author: Markus Froeschle
 */

#include <MCF5475.h>
#include "pci.h"
#include "stdint.h"

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
	/*
	 * assert /PCIRESET (reset cards on bus). FIXME: According to documentation,
	 * this should be done last during PCI initialization
	 */
	MCF_PCI_PCIGSCR = MCF_PCI_PCIGSCR_PR;

	/*
	 * setup the PCI arbiter.
	 */

	/* PCI Arbiter Control Register (PACR) */
	MCF_PCIARB_PACR = MCF_PCIARB_PACR_INTMPRI		/* internal master priority level high */
					+ MCF_PCIARB_PACR_EXTMPRI(0x1f) /* external master priority levels all high */
					+ MCF_PCIARB_PACR_INTMINTEN		/* generate interrupt if internal master timeout */
					+ MCF_PCIARB_PACR_EXTMINTEN(0x1f);	/* generate interrupt if external master timeout */
	/* configure all 5 PCI Bus Grant pins for PCI */
	MCF_PAD_PAR_PCIBG = 0x3ff;
	/* configure all 5 PCI Bus Request pins for PCI */
	MCF_PAD_PAR_PCIBR = 0x3ff;

	/* PCI Status/Command Register PCISCR */
	MCF_PCI_PCISCR = MCF_PCI_PCISCR_M	/* recognize memory accesses */
				+ MCF_PCI_PCISCR_B			/* bus master enable for controller */
				+ MCF_PCI_PCISCR_MW;		/* controller can generate memory write and invalidate command */

	/* Configuration 1 Register PCICR1 */
	MCF_PCI_PCICR1 = MCF_PCI_PCICR1_CACHELINESIZE(8)	/* cache line size in units of DWORDs */
				+ MCF_PCI_PCICR1_LATTIMER(32);			/* 256 PCI clocks (?) latency */
	/* Configuration 2 Register PCICR2 */
	MCF_PCI_PCICR2 = MCF_PCI_PCICR2_MINGNT(1)	/* specifies how long the Coldfire processor retains bus ownership as master */
			+ MCF_PCI_PCICR2_MAXLAT(1);			/* specifies (in units of 1/4 microseconds) how often the Coldfire */
												/* processor needs access to the bus as PCI master */
	/* Initiator Control Register: turn on error signaling */
	MCF_PCI_PCIICR = MCF_PCI_PCIICR_TAE +				/* target abort enable: CPU generates an interrupt if target terminates a transaction */
			MCF_PCI_PCIICR_IAE 							/* the same for the initiator */
		/* + MCF_PCI_PCIICR_REE */ ;					/* the same for retry errors */
	/* Global Status/Control Register */
	MCF_PCI_PCIGSCR |= MCF_PCI_PCIGSCR_SEE;				/* generate interrupt if /PCISERR is asserted (PCI system error) */

	/* configure initiator windows */


	/* Initiator Window 0 Base / Translation Address Register */
#ifdef SAME_CPU_PCI_MEM_ADDR
	MCF_PCI_PCIIW0BTAR = (PCI_MEMORY_OFFSET + ((PCI_MEMORY_SIZE - 1) >> 8) & 0xFFFF0000) | (PCI_MEMORY_OFFSET >> 16 & 0xFFFF)
#else
	MCF_PCI_PCIIW0BTAR = PCI_MEMORY_OFFSET + ((PCI_MEMORY_SIZE - 1) >> 8)	& 0xFFFF0000;
#endif /* SAME_CPU_PCI_MEM_ADDR */

	/* Initiator Window 1 Base / Translation Address Register */
	MCF_PCI_PCIIW1BTAR = (PCI_IO_OFFSET + ((PCI_IO_SIZE - 1) >> 8)) & 0xFFFF0000;

	/* Initiator Window 2 Base / Translation Address Register */
	MCF_PCI_PCIIW2BTAR = 0L;	/* not used */

	MCF_PCI_PCIIWCR = MCF_PCI_PCIIWCR_WINCTRL0_MEMRDLINE
			+ MCF_PCI_PCIIWCR_WINCTRL1_IO;

	/* target zones */
}

void init(void)
{
	init_eport();
	init_xlbus_arbiter();
	init_pci();
}
