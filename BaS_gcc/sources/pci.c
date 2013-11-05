/*
 * pci.c
 *
 * Purpose:		PCI configuration for the Coldfire builtin PCI bridge.
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
#include "bas_string.h"
#include "util.h"
#include "wait.h"

/*
 * PCI device class descriptions displayed during PCI bus scan
 */
static struct pci_class
{
	int classcode;
	char *description;
} pci_classes[] =
{
	{ 0x00, "device was built prior definition of the class code field" },
	{ 0x01, "Mass Storage Controller" },
	{ 0x02, "Network Controller" },
	{ 0x03, "Display Controller" },
	{ 0x04, "Multimedia Controller" },
	{ 0x05, "Memory Controller" },
	{ 0x06, "Bridge Device" },
	{ 0x07, "Simple Communication Controller" },
	{ 0x08, "Base System Peripherial" },
	{ 0x09, "Input Device" },
	{ 0x0a, "Docking Station" },
	{ 0x0b, "Processor" },
	{ 0x0c, "Serial Bus Controller" },
	{ 0x0d, "Wireless Controller" },
	{ 0x0e, "Intelligent I/O Controller" },
	{ 0x0f, "Satellite Communication Controller" },
	{ 0x10, "Encryption/Decryption Controller" },
	{ 0x11, "Data Acquisition and Signal Processing Controller" },
	{ 0xff, "Device does not fit any defined class" },
};
static int num_pci_classes = sizeof(pci_classes) / sizeof(struct pci_class);

#define NUM_CARDS		10
#define NUM_RESOURCES	6
/* holds the handle of a card at position = array index */
static uint16_t handles[NUM_CARDS];	
/* holds the card's resource descriptors; filled in pci_device_config() */
static struct pci_rd resource_descriptors[NUM_CARDS][NUM_RESOURCES]; 

static int16_t handle2index(int16_t handle)
{
	int i;

	for (i = 0; i < NUM_CARDS; i++)
	{
		if (handles[i] == handle)
		{
			return i;
		}
	}
	return -1;
}

static char *device_class(int classcode)
{
	int i;

	for (i = 0; i < num_pci_classes; i++)
	{
		if (pci_classes[i].classcode == classcode)
		{
			return pci_classes[i].description;
		}
	}
	return "not found";
}

uint32_t pci_read_config_longword(uint16_t handle, uint16_t offset)
{
	uint32_t value;
	uint16_t bus = PCI_BUS_FROM_HANDLE(handle);
	uint16_t device = PCI_DEVICE_FROM_HANDLE(handle);
	uint16_t function = PCI_FUNCTION_FROM_HANDLE(handle);

	/* clear PCI status/command register */
	MCF_PCI_PCISCR = MCF_PCI_PCISCR_PE |		/* clear parity error bit */
				MCF_PCI_PCISCR_SE |					/* clear system error */
				MCF_PCI_PCISCR_MA |					/* clear master abort */
				MCF_PCI_PCISCR_TR |					/* clear target abort */
				MCF_PCI_PCISCR_TS |					/* clear target abort signalling (as target) */
				MCF_PCI_PCISCR_DP;					/* clear parity error */
	
	//(void) MCF_PCI_PCISCR;
	//wait(100);

	//xprintf("PCISCR before config cycle: %lx\r\n", MCF_PCI_PCISCR);

	/* initiate PCI configuration access to device */

	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |			/* enable configuration access special cycle */
			MCF_PCI_PCICAR_BUSNUM(bus) |
			MCF_PCI_PCICAR_DEVNUM(device) |		/* device number, devices 0 - 9 are reserved */
			MCF_PCI_PCICAR_FUNCNUM(function) |	/* function number */
			MCF_PCI_PCICAR_DWORD(offset / 4);

	//wait(100);
	value =  * (volatile uint32_t *) PCI_IO_OFFSET;	/* access device */
	//xprintf("pci_read_config_longword(%d (bus=%d, device=%d, function=%d), %d) = %d\r\n", handle, bus, device, function, offset, swpl(value));

	return swpl(value);
}

uint16_t pci_read_config_word(uint16_t handle, uint16_t offset)
{
   uint32_t value;

   value = pci_read_config_longword(handle, offset / 2);
   return((value >> (1 - offset % 2) * 8) & 0xffff);
}

uint8_t pci_read_config_byte(uint16_t handle, uint16_t offset)
{
	uint32_t value;
	
	value = pci_read_config_longword(handle, offset / 4);
	return ((value >> (3 - offset % 4) * 8) & 0xff);
}

/*
 * pci_write_config_longword()
 *
 * write an uint32_t value to the configuration space of a PCI device
 * offset is a PCI DWORD value.
 */
void pci_write_config_longword(uint16_t handle, uint16_t offset, uint32_t value)
{
	uint16_t bus = PCI_BUS_FROM_HANDLE(handle);
	uint16_t device = PCI_DEVICE_FROM_HANDLE(handle);
	uint16_t function = PCI_FUNCTION_FROM_HANDLE(handle);

	/* clear PCI status/command register */
	MCF_PCI_PCISCR = MCF_PCI_PCISCR_PE |		/* clear parity error bit */
				MCF_PCI_PCISCR_SE |					/* clear system error */
				MCF_PCI_PCISCR_MA |					/* clear master abort */
				MCF_PCI_PCISCR_TR |					/* clear target abort */
				MCF_PCI_PCISCR_TS |					/* clear target abort signalling (as target) */
				MCF_PCI_PCISCR_DP;					/* clear parity error */

	//(void) MCF_PCI_PCISCR;
	//wait(10);

	//xprintf("PCISCR before config cycle: %lx\r\n", MCF_PCI_PCISCR);

	/* initiate PCI configuration access to device */

	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |				/* enable configuration access special cycle */
			MCF_PCI_PCICAR_BUSNUM(bus) |
			MCF_PCI_PCICAR_DEVNUM(device) |			/* device number, devices 0 - 9 are reserved */
			MCF_PCI_PCICAR_FUNCNUM(function) |		/* function number */
			MCF_PCI_PCICAR_DWORD(offset / 4);

	//wait(10);
	* (volatile uint32_t *) PCI_IO_OFFSET = swpl(value);	/* access device */
}

/*
 * pci_get_resource
 *
 * get resource descriptor chain for handle
 */
struct pci_rd *pci_get_resource(uint16_t handle)
{
	int index = -1;

	index = handle2index(handle);
	if (index == -1)
		return NULL;
	return resource_descriptors[index];
}

/*
 * pci_find_device()
 *
 * find index'th device by device_id and vendor_id. Special case: vendor id -1 (0xffff)
 * matches all devices. You can search the whole bus by repeatedly calling this function
 */
int16_t pci_find_device(uint16_t device_id, uint16_t vendor_id, int index)
{
	uint16_t bus;
	uint16_t device;
	uint16_t function;
	uint16_t pos = 0;
	int handle;

	for (bus = 0; bus < 255; bus++)	/* FireBee USB is on DEVSEL(17) ??? */
	{
		for (device = 0; device < 32; device++)
		{
			for (function = 0; function < 8; function++)
			{
				uint32_t value;

				handle = PCI_HANDLE(bus, device, function);
				value = pci_read_config_longword(handle, 0);
				if (value != 0xffffffff)	/* we have a device at this position */
				{
					if (vendor_id == 0xffff && pos == index)	/* ignore device id */
					{
						return handle;
					}
					else
					{
						/* we found a match, but at wrong position */
						pos++;
						continue;
					}
					if (PCI_VENDOR_ID(value) == vendor_id && PCI_DEVICE_ID(value) == device_id)
					{
						if (pos == index)
							return handle;
					}
					else
						pos++;
				}
			}
		}
	}
	return PCI_DEVICE_NOT_FOUND;
}

int16_t pci_hook_interrupt(uint16_t handle, void *handler, void *parameter)
{
	/* FIXME: implement */
	xprintf("pci_hook_interrupt() still not implemented\r\n");
	return PCI_SUCCESSFUL;
}

int16_t pci_unhook_interrupt(uint16_t handle)
{
	/* FIXME: implement */

	xprintf("pci_unhook_interrupt() still not implemented\r\n");
	return PCI_SUCCESSFUL;
}



/*
 * pci_device_config()
 *
 * Map card resources, adjust BARs and fill resource descriptors
 */
static void pci_device_config(uint16_t bus, uint16_t device, uint16_t function)
{
	uint32_t address;
	uint16_t handle;
	int16_t index = - 1;
	struct pci_rd *descriptors;
	int i;
	uint32_t mem_address = PCI_MEMORY_OFFSET;
	uint32_t io_address = PCI_IO_OFFSET;

	/* determine pci handle from bus, device + function number */
	handle = PCI_HANDLE(bus, device, function);

	/* find index into resource descriptor table for handle */
	index = handle2index(handle);

	if (index == -1)
	{
		xprintf("cannot find index for handle %d\r\n", handle);
		return;
	}

	int barnum = 0;
	descriptors = resource_descriptors[index];
	for (i = 0; i < 6; i++)		/* for all bars */
	{
		uint32_t value;
		

		value = pci_read_config_longword(handle, 0x10 + i);			/* read BAR value */
		pci_write_config_longword(handle, 0x10 + i, 0xffffffff);	/* write all bits */
		address = pci_read_config_longword(handle, 0x10 + i);		/* read back value */

		if (address)	/* is bar in use? */
		{
			//xprintf("%s region found with base address %08x, size = %x\r\n",
				//(IS_PCI_MEM_BAR(value) ? "Memory" : "I/O"),
				//(IS_PCI_MEM_BAR(value) ? PCI_MEMBAR_ADR(value) : PCI_IOBAR_ADR(value)),
				//(IS_PCI_MEM_BAR(value) ? ~(address & 0xfffffff0) + 1 : ~(address & 0xfffffffc) + 1));

			struct pci_rd *rd = &descriptors[barnum]; 

			/* adjust base address to card's alignment requirements */
			if (IS_PCI_MEM_BAR(value))
			{
				int size = ~(address & 0xfffffff0) + 1;

				/* calculate start adress with alignment */
				mem_address = (mem_address + size - 1) & ~(size - 1);

				/* write it to the BAR */
				pci_write_config_longword(handle, 0x10 + i, mem_address);

				/* read it back, just to be sure */
				value = pci_read_config_longword(handle, 0x10 + i);

				//xprintf("BAR[%d] configured to %08x, size %x\r\n", i, value, size);

				/* fill resource descriptor */
				rd->next = sizeof(struct pci_rd);
				rd->flags = 0 | FLG_8BIT | FLG_16BIT | FLG_32BIT | ORD_MOTOROLA;
				rd->start = mem_address;
				rd->length = size;
				rd->offset = 0;
				rd->dmaoffset = 0;

				/* adjust memory adress for next turn */
				mem_address += size;

				/* index to next unused resource descriptor */
				barnum++;
			}
			else if (IS_PCI_IO_BAR(value))
			{
				int size = ~(address & 0xfffffffc) + 1;

				/* calculate start adress with alignment */
				io_address = (io_address + size - 1) & ~(size - 1);

				/* write it to the BAR */
				pci_write_config_longword(handle, 0x10 + i, io_address);

				/* read it back, just to be sure */
				value = pci_read_config_longword(handle, 0x10 + i);

				//xprintf("BAR[%d] mapped to %08x, size %x\r\n", i, value, size);

				/* fill resource descriptor */
				rd->next = sizeof(struct pci_rd);
				rd->flags = FLG_IO | FLG_8BIT | FLG_16BIT | FLG_32BIT | 1;
				rd->start = io_address;
				rd->offset = PCI_MEMORY_OFFSET;
				rd->length = size;
				rd->dmaoffset = PCI_MEMORY_OFFSET;

				/* adjust I/O adress for next turn */
				io_address += size;

				/* index to next unused resource descriptor */
				barnum++;
			}
		}
	}
	/* mark end of resource chain */
	if (barnum > 0)
		descriptors[barnum - 1].flags |= FLG_LAST;
}

void pci_scan(void)
{
	uint16_t bus;
	uint16_t device;
	uint16_t function;
	int16_t index = 0;

	xprintf("\r\nPCI bus scan...\r\n\r\n");
	xprintf(" Bus| Dev|Func|Vndr|D-ID|\r\n");
	xprintf("----+----+----|----+----|\r\n");
	for (bus = 0; bus < 255; bus++)	/* scan two busses. FireBee USB is on DEVSEL(17) */
	{
		for (device = 0; device < 32; device++)
		{
			for (function = 0; function < 8; function++)
			{
				uint32_t value;
				uint16_t handle = PCI_HANDLE(bus, device, function);

				value = pci_read_config_longword(handle, 0);
				if (value != 0xffffffff)
				{
					xprintf(" %02x | %02x | %02x |%04x|%04x| %s\r\n", bus, device, function,
							PCI_VENDOR_ID(value),
							PCI_DEVICE_ID(value),
							device_class(pci_read_config_longword(handle, 0x08) >> 24 & 0xff));

					if (PCI_VENDOR_ID(value) != 0x1057 && PCI_DEVICE_ID(value) != 0x5806) /* do not configure bridge */
					{
						/* save handle to index value so that we later find our resources again */
						handles[index++] = PCI_HANDLE(bus, device, function);

						/* configure memory and I/O for card */
						pci_device_config(bus, device, function);
					}

					/* test for multi-function device to avoid ghost device detects */
					value = pci_read_config_longword(handle, 0x0c);	
					if (function == 0 && !(PCI_HEADER_TYPE(value) & 0x80))	/* no multi function device */
						function = 8;
				}
			}
		}
	}
	xprintf("\r\n...finished\r\n");
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

	/* Setup burst parameters */
	MCF_PCI_PCICR1 = MCF_PCI_PCICR1_CACHELINESIZE(4) + MCF_PCI_PCICR1_LATTIMER(64); /* TODO: test increased latency timer */
	MCF_PCI_PCICR2 = MCF_PCI_PCICR2_MINGNT(16) + MCF_PCI_PCICR2_MAXLAT(16);

	/* Turn on error signaling, 32 write retries on failure */
	MCF_PCI_PCIICR = MCF_PCI_PCIICR_REE + MCF_PCI_PCIICR_IAE + MCF_PCI_PCIICR_TAE + 32;
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

	/* initialize target control register */
	MCF_PCI_PCITCR = 0;
	
	/* reset PCI devices */
	MCF_PCI_PCIGSCR &= ~MCF_PCI_PCIGSCR_PR;

	xprintf("finished\r\n");

	/* initialize resource descriptor table */
	memset(&resource_descriptors, 0, NUM_CARDS * NUM_RESOURCES * sizeof(struct pci_rd));
	/* initialize handles array */
	memset(handles, 0, NUM_CARDS * sizeof(uint16_t));

	pci_scan();
}

