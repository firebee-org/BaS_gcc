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

#define pci_config_wait()	wait(10000);	/* FireBee USB not properly detected otherwise */

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
static int32_t handles[NUM_CARDS];	
/* holds the card's resource descriptors; filled in pci_device_config() */
static struct pci_rd resource_descriptors[NUM_CARDS][NUM_RESOURCES]; 

/*
 * retrieve handle for i'th device
 */
static int handle2index(int32_t handle)
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

/*
 * retrieve device class (in cleartext) for a PCI classcode
 */
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

/*
 * read an uint32_t from configuration space of card with handle and offset
 *
 * The returned value is in little endian format.
 */
uint32_t pci_read_config_longword(int32_t handle, int offset)
{
	uint32_t value;
	uint16_t bus = PCI_BUS_FROM_HANDLE(handle);
	uint16_t device = PCI_DEVICE_FROM_HANDLE(handle);
	uint16_t function = PCI_FUNCTION_FROM_HANDLE(handle);

	/* initiate PCI configuration access to device */
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |			/* enable configuration access special cycle */
			MCF_PCI_PCICAR_BUSNUM(bus) |
			MCF_PCI_PCICAR_DEVNUM(device) |		/* device number, devices 0 - 9 are reserved */
			MCF_PCI_PCICAR_FUNCNUM(function) |	/* function number */
			MCF_PCI_PCICAR_DWORD(offset / 4);
	
	pci_config_wait();

	value =  * (volatile uint32_t *) PCI_IO_OFFSET;	/* access device */

	/* finish PCI configuration access special cycle (allow regular PCI accesses) */
	MCF_PCI_PCICAR &= ~MCF_PCI_PCICAR_E;

	pci_config_wait();

	return value;
}

uint16_t pci_read_config_word(int32_t handle, int offset)
{
   uint32_t value;

   value = pci_read_config_longword(handle, offset);
   return value >> ((1 - offset % 2) * 16) & 0xffff;
}

uint8_t pci_read_config_byte(int32_t handle, int offset)
{
	uint32_t value;
	
	value = pci_read_config_longword(handle, offset);
	return value >> ((3 - offset % 4) * 8) & 0xff;
}

/*
 * pci_write_config_longword()
 *
 * write an uint32_t value (must be in little endian format) to the configuration space of a PCI device
 */
int32_t pci_write_config_longword(int32_t handle, int offset, uint32_t value)
{
	uint16_t bus = PCI_BUS_FROM_HANDLE(handle);
	uint16_t device = PCI_DEVICE_FROM_HANDLE(handle);
	uint16_t function = PCI_FUNCTION_FROM_HANDLE(handle);

	/* initiate PCI configuration access to device */
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |				/* enable configuration access special cycle */
			MCF_PCI_PCICAR_BUSNUM(bus) |
			MCF_PCI_PCICAR_DEVNUM(device) |			/* device number, devices 0 - 9 are reserved */
			MCF_PCI_PCICAR_FUNCNUM(function) |		/* function number */
			MCF_PCI_PCICAR_DWORD(offset / 4);

	pci_config_wait();

	* (volatile uint32_t *) PCI_IO_OFFSET = value;	/* access device */

	pci_config_wait();

	/* finish configuration space access cycle */
	MCF_PCI_PCICAR &= ~MCF_PCI_PCICAR_E;
	pci_config_wait();

	return PCI_SUCCESSFUL;
}

/*
 * pci_get_resource
 *
 * get resource descriptor chain for handle
 */
struct pci_rd *pci_get_resource(int32_t handle)
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
int32_t pci_find_device(uint16_t device_id, uint16_t vendor_id, int index)
{
	uint16_t bus;
	uint16_t device;
	uint16_t function = 0;
	uint16_t n = 0;
	int32_t handle;

	for (bus = 0; bus < 2; bus++)
	{
		for (device = 10; device < 31; device++)
		{
			uint32_t value;
			uint8_t htr;

			handle = PCI_HANDLE(bus, device, 0);
			value = pci_read_config_longword(handle, PCIIDR);
			if (value != 0xffffffff)	/* we have a device at this position */
			{
#ifdef _NOT_USED_
				xprintf("value=%08x, vendor_id = 0x%04x, device_id=0x%04x\r\n",
						value, PCI_VENDOR_ID(value), PCI_DEVICE_ID(value));
#endif /* _NOT_USED_ */
				if (vendor_id == 0xffff ||
					(PCI_VENDOR_ID(value) == vendor_id && PCI_DEVICE_ID(value) == device_id))
				{
					if (n == index)
					{
						return handle;
					}
					n++;
				}

				/*
				 * There is a device at this position, but not the one we are looking for.
				 * Check to see if it is a multi-function device. We need to look "behind" it
				 * for the other functions in that case.
				 */
				if ((htr = pci_read_config_byte(handle, PCIHTR)) & 0x80)
				{
					/* yes, this is a multi-function device, look for more functions */

					for (function = 1; function < 8; function++)
					{
						handle = PCI_HANDLE(bus, device, function);
						value = pci_read_config_longword(handle, PCIIDR);
						if (value != 0xFFFFFFFF)	/* device found */
						{
							if (vendor_id == 0xffff || 
								(PCI_VENDOR_ID(value) == vendor_id && PCI_DEVICE_ID(value) == device_id))
							{
								if (n == index)
								{
									return handle;
								}
								n++;
							}
						}
					}
				}
			}
		}
	}
	return PCI_DEVICE_NOT_FOUND;
}

int32_t pci_hook_interrupt(int32_t handle, void *handler, void *parameter)
{
	/* FIXME: implement */
	xprintf("pci_hook_interrupt() still not implemented\r\n");
	return PCI_SUCCESSFUL;
}

int32_t pci_unhook_interrupt(int32_t handle)
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
	uint32_t value;
	static uint32_t mem_address = PCI_MEMORY_OFFSET;
	static uint32_t io_address = PCI_IO_OFFSET;

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
		/*
		 * read BAR[i] value
		 */
		value = swpl(pci_read_config_longword(handle, PCIBAR0 + i));

		/*
		 * write all bits of BAR[i]
		 */
		pci_write_config_longword(handle, 0x10 + i, 0xffffffff);

		/*
		 * read back value to see which bits have been set
		 */
		address = swpl(pci_read_config_longword(handle, PCIBAR0 + i));

		if (address)	/* is bar in use? */
		{
			/*
			 * resource descriptor for this device
			 */
			struct pci_rd *rd = &descriptors[barnum]; 

			if (IS_PCI_MEM_BAR(value))
			{
				/* adjust base address to card's alignment requirements */
				int size = ~(address & 0xfffffff0) + 1;
				xprintf("device 0x%x: BAR[%d] requests %d kBytes of memory\r\n", handle, i, size / 1024);

				/* calculate a valid map adress with alignment requirements */
				mem_address = (mem_address + size - 1) & ~(size - 1);

				/* write it to the BAR */
				pci_write_config_longword(handle, PCIBAR0 + i, swpl(mem_address));

				/* read it back, just to be sure */
				value = swpl(pci_read_config_longword(handle, PCIBAR0 + i));
				
				xprintf("set PCIBAR%d on device 0x%02x to 0x%08x\r\n",
						i, handle, value);

				/* fill resource descriptor */
				rd->next = sizeof(struct pci_rd);
				rd->flags = 0 | FLG_8BIT | FLG_16BIT | FLG_32BIT | ORD_MOTOROLA;
				rd->start = mem_address;
				rd->length = size;
				rd->offset = PCI_MEMORY_OFFSET;
				rd->dmaoffset = 0;

				/* adjust memory adress for next turn */
				mem_address += size;

				/* index to next unused resource descriptor */
				barnum++;
			}
			else if (IS_PCI_IO_BAR(value)) /* same as above for I/O resources */
			{
				int size = ~(address & 0xfffffffc) + 1;
				xprintf("device 0x%x: BAR[%d] requests %d bytes of memory\r\n", handle, i, size);

				io_address = (io_address + size - 1) & ~(size - 1);
				pci_write_config_longword(handle, PCIBAR0 + i, swpl(io_address));
				value = swpl(pci_read_config_longword(handle, PCIBAR0 + i));

				xprintf("set PCIBAR%d on device 0x%02x to 0x%08x\r\n",
					i, handle, value);

				rd->next = sizeof(struct pci_rd);
				rd->flags = FLG_IO | FLG_8BIT | FLG_16BIT | FLG_32BIT | 1;
				rd->start = io_address;
				rd->offset = PCI_IO_OFFSET;
				rd->length = size;
				rd->dmaoffset = PCI_MEMORY_OFFSET;

				io_address += size;

				barnum++;
			}
		}
	}
	/* mark end of resource chain */
	if (barnum > 0)
		descriptors[barnum - 1].flags |= FLG_LAST;
	
	/*
	 * enable device finally
	 */
	value = swpl(pci_read_config_longword(handle, PCICSR));
	value |= 0xffff035f;
	pci_write_config_longword(handle, PCICSR, swpl(value));

	value = swpl(pci_read_config_longword(handle, PCICSR));
	xprintf("device 0x%02x PCICSR = 0x%08x\r\n", handle, value);
}

/*
 * scan PCI bus and display devices found. Create a handle for each device and call pci_device_config() for it
 */
void pci_scan(void)
{
	int16_t handle;
	int16_t index = 0;

	xprintf("\r\nPCI bus scan...\r\n\r\n");
	xprintf(" Bus| Dev|Func|Vndr|D-ID|Hndl|\r\n");
	xprintf("----+----+----+----+----+----+\r\n");

	handle = pci_find_device(0x0, 0xFFFF, index);
	while (handle > 0)
	{
		uint32_t value;

		value = pci_read_config_longword(handle, PCIIDR);
		xprintf(" %02x | %02x | %02x |%04x|%04x|%04x| %s (0x%02x)\r\n",
				PCI_BUS_FROM_HANDLE(handle),
				PCI_DEVICE_FROM_HANDLE(handle),
				PCI_FUNCTION_FROM_HANDLE(handle),
				PCI_VENDOR_ID(value), PCI_DEVICE_ID(value),
				handle,
				device_class(pci_read_config_byte(handle, PCICCR)),
				pci_read_config_byte(handle, PCICCR));

		if (PCI_VENDOR_ID(value) != 0x1057 && PCI_DEVICE_ID(value) != 0x5806) /* do not configure bridge */
		{
			/* save handle to index value so that we'll be able to later find our resources */
			handles[index] = handle;

			/* configure memory and I/O for card */
			pci_device_config(PCI_BUS_FROM_HANDLE(handle),
						PCI_DEVICE_FROM_HANDLE(handle),
						PCI_FUNCTION_FROM_HANDLE(handle));
		}

		handle = pci_find_device(0x0, 0xFFFF, ++index);
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
	MCF_EPORT_EPIER = 0xfe;	/* enable all EPORT interrupts (for now) */
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

	xprintf("PCIGSCR = %08x\r\n");
	MCF_PCI_PCIGSCR |= 0x60000000;	/* clear PERR and SERR in global status/command register */
	xprintf("PCIGSCR = %08x\r\n");

	/* FIXME: Firetos (boot2.S, l. 719) looks pretty strange at this place - is this a typo? */
}

void init_pci(void)
{
	uint32_t value;
	uint32_t new_value;

	xprintf("initializing PCI bridge:");

	init_eport();
	init_xlbus_arbiter();

	MCF_PCIARB_PACR = MCF_PCIARB_PACR_INTMPRI
       + MCF_PCIARB_PACR_EXTMPRI(0x1F)
       + MCF_PCIARB_PACR_INTMINTEN
       + MCF_PCIARB_PACR_EXTMINTEN(0x1F);

	/* Setup burst parameters */
	MCF_PCI_PCICR1 = MCF_PCI_PCICR1_CACHELINESIZE(4) + MCF_PCI_PCICR1_LATTIMER(32); /* TODO: test increased latency timer */
	MCF_PCI_PCICR2 = MCF_PCI_PCICR2_MINGNT(16) + MCF_PCI_PCICR2_MAXLAT(16);

	/* Turn on error signaling, 32 write retries on failure */
	MCF_PCI_PCIICR = MCF_PCI_PCIICR_REE + MCF_PCI_PCIICR_IAE + MCF_PCI_PCIICR_TAE + 32;
	MCF_PCI_PCIGSCR |= MCF_PCI_PCIGSCR_SEE;

	/* Configure Initiator Windows */

	/*
	 * Window starts at PCI_MEMORY_OFFSET, ends at PCI_MEMORY_OFFSET + PCI_MEMORY_SIZE - 1 (2 GB)
	 * There is no translation from M54xx address space to PCI address space (same addresses)
	 */

	/* initiator window 0 base / translation adress register */
	MCF_PCI_PCIIW0BTAR = PCI_MEMORY_OFFSET | (((PCI_MEMORY_SIZE - 1) >> 8) & 0xffff0000);
	/* | PCI_MEMORY_OFFSET >> 16; */

	/* initiator window 1 base / translation adress register */
	MCF_PCI_PCIIW1BTAR = (PCI_IO_OFFSET | ((PCI_IO_SIZE - 1) >> 8)) & 0xffff0000;

	/* initiator window 2 base / translation address register */
	MCF_PCI_PCIIW2BTAR = 0L;   /* not used */

	/* initiator window configuration register */
	MCF_PCI_PCIIWCR = MCF_PCI_PCIIWCR_WINCTRL0_MEMRDLINE | MCF_PCI_PCIIWCR_WINCTRL1_IO;

	/* initialize target control register */
	MCF_PCI_PCITCR = 0;
	
	value =  MCF_PCI_PCISCR_M | /* memory access control enabled */
		MCF_PCI_PCISCR_B |		/* bus master enabled */
		MCF_PCI_PCISCR_MW |		/* memory write and invalidate enabled */
		MCF_PCI_PCISCR_PER |	/* parity errors enabled, PERR# will be asserted */
		MCF_PCI_PCISCR_S;		/* SERR enabbled */

	MCF_PCI_PCISCR = value;

	new_value = MCF_PCI_PCISCR;

	if (new_value != value)
		xprintf("MCF_PCI_PCISCR wanted: %08x, got %08x\r\n", value, new_value);

	/* reset PCI devices */
	MCF_PCI_PCIGSCR &= ~MCF_PCI_PCIGSCR_PR;
	do ; while (MCF_PCI_PCIGSCR & 1); /* wait until reset finished */

	xprintf("finished\r\n");

	/* initialize resource descriptor table */
	memset(&resource_descriptors, 0, NUM_CARDS * NUM_RESOURCES * sizeof(struct pci_rd));
	/* initialize handles array */
	memset(handles, 0, NUM_CARDS * sizeof(int32_t));

	/*
	 * do normal initialization
	 */
	pci_scan();
}

