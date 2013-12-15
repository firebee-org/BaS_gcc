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
#include "interrupts.h"
#include "wait.h"

//#define DEBUG_PCI
#ifdef DEBUG_PCI
#define debug_printf(format, arg...) do { xprintf("DEBUG: " format "", ##arg); } while (0)
#else
#define debug_printf(format, arg...) do { ; } while (0)
#endif /* DEBUG_PCI */

#if MACHINE_FIREBEE
#define pci_config_wait()	wait(20000);	/* FireBee USB not properly detected otherwise */
#elif MACHINE_M5484LITE
#define pci_config_wait() do { ; } while (0)
#endif

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


__attribute__((aligned(16))) void chip_errata_135(void)
{
	/*
	 * Errata type: Silicon
	 * Affected component: PCI
	 * Description: When core PCI transactions that involve writes to configuration or I/O space
	 * are followed by a core line access to line addresses 0x4 and 0xC, core access
	 * to the XL bus can hang.
	 * Workaround: Prevent PCI configuration and I/O writes from being followed by the described
	 * line access by the core by generating a known good XL bus transaction after
	 * the PCI transaction.
	 * Create a dummy function which is called immediately after each of the affected
	 * transactions. There are three requirements for this dummy function.
	 * 1. The function must be aligned to a 16-byte boundary.
	 * 2. The function must contain a dummy write to a location on the XL bus,
	 * preferably one with no side effects.
	 * 3. The function must be longer than 32 bytes. If it is not, the function should
	 * be padded with 16- or 48-bit TPF instructions placed after the end of
	 * the function (after the RTS instruction) such that the length is longer
	 * than 32 bytes.
	 */

	 __asm__ __volatile(
		"		.extern __MBAR\n\t"
		"		clr.l	d0\n\t"
		"		move.l	d0,__MBAR+0xF0C\n\t"		/* Must use direct addressing. write to EPORT module */
													/* xlbus -> slavebus -> eport, writing '0' to register */
													/* has no effect */
		"		rts\n\t"
		"		tpf.l	#0x0\n\t"
		"		tpf.l	#0x0\n\t"
		"		tpf.l	#0x0\n\t"
		"		tpf.l	#0x0\n\t"
		"		tpf.l	#0x0\n\t"
		:::);
}



__attribute__((interrupt)) void pci_arb_interrupt(void)
{
	debug_printf("XLBARB slave error interrupt\r\n");
	MCF_XLB_XARB_SR |= ~MCF_XLB_XARB_SR_SEA;
}

__attribute__((interrupt)) void xlb_pci_interrupt(void)
{
	debug_printf("XLBPCI interrupt\r\n");
}

__attribute__((interrupt)) void pci_interrupt(void)
{
}

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

	/* initiate PCI configuration access to device */
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |			/* enable configuration access special cycle */
			MCF_PCI_PCICAR_BUSNUM(PCI_BUS_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_DEVNUM(PCI_DEVICE_FROM_HANDLE(handle)) |	/* device number, devices 0 - 9 are reserved */
			MCF_PCI_PCICAR_FUNCNUM(PCI_FUNCTION_FROM_HANDLE(handle)) |	/* function number */
			MCF_PCI_PCICAR_DWORD(offset / 4);
	
	__asm__ __volatile__("nop" ::: "memory");				/* this is what the Linux BSP does */

	pci_config_wait();
	value =  * (volatile uint32_t *) PCI_IO_OFFSET;	/* access device */

	__asm__ __volatile__("tpf" ::: "memory");				/* this is what the Linux BSP does */

	/* finish PCI configuration access special cycle (allow regular PCI accesses) */
	MCF_PCI_PCICAR &= ~MCF_PCI_PCICAR_E;

	return value;
}

uint16_t pci_read_config_word(int32_t handle, int offset)
{
	uint16_t value;

	/*
	 * initiate PCI configuration space access to device
	 */
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |		/* enable configuration space special cycle */
			MCF_PCI_PCICAR_BUSNUM(PCI_BUS_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_DEVNUM(PCI_DEVICE_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_FUNCNUM(PCI_FUNCTION_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_DWORD(offset / 4);
	
	__asm__ __volatile("nop" ::: "memory");			/* this is what Linux BSP does */

	value = * (volatile uint16_t *) PCI_IO_OFFSET + (offset & 2);

	__asm__ __volatile("tpf" ::: "memory");

	/* finish PCI configuration access special cycle */
	MCF_PCI_PCICAR &= ~MCF_PCI_PCICAR_E;

	return value;
}

uint8_t pci_read_config_byte(int32_t handle, int offset)
{
	uint8_t value;

	/* initiate PCI configuration access to device */
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |				/* enable configuration access special cycle */
		MCF_PCI_PCICAR_BUSNUM(PCI_BUS_FROM_HANDLE(handle)) |
		MCF_PCI_PCICAR_DEVNUM(PCI_DEVICE_FROM_HANDLE(handle)) |		/* device number, devices 0 - 9 are reserved */
		MCF_PCI_PCICAR_FUNCNUM(PCI_FUNCTION_FROM_HANDLE(handle)) |	/* function number */
		MCF_PCI_PCICAR_DWORD(offset / 4);
	
	__asm__ __volatile__("nop" ::: "memory");

	value = * (volatile uint8_t *) (PCI_IO_OFFSET + (offset & 3));

	__asm__ __volatile__("tpf" ::: "memory");

	MCF_PCI_PCICAR &= ~MCF_PCI_PCICAR_E;

	return value;
}

/*
 * pci_write_config_longword()
 *
 * write an uint32_t value (must be in little endian format) to the configuration space of a PCI device
 */
int32_t pci_write_config_longword(int32_t handle, int offset, uint32_t value)
{
	/* initiate PCI configuration access to device */
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |				/* enable configuration access special cycle */
		MCF_PCI_PCICAR_BUSNUM(PCI_BUS_FROM_HANDLE(handle)) |
		MCF_PCI_PCICAR_DEVNUM(PCI_DEVICE_FROM_HANDLE(handle)) |		/* device number, devices 0 - 9 are reserved */
		MCF_PCI_PCICAR_FUNCNUM(PCI_FUNCTION_FROM_HANDLE(handle)) |	/* function number */
		MCF_PCI_PCICAR_DWORD(offset / 4);

	__asm__ __volatile__("nop" ::: "memory");

	* (volatile uint32_t *) PCI_IO_OFFSET = value;	/* access device */

	__asm__ __volatile__("tpf" ::: "memory");

	/* finish configuration space access cycle */
	MCF_PCI_PCICAR &= ~MCF_PCI_PCICAR_E;

	chip_errata_135();

	return PCI_SUCCESSFUL;
}

/*
 * write a 16-bit value to config space. Must be in little-endian format
 */
int32_t pci_write_config_word(int32_t handle, int offset, uint16_t value)
{
	/* initiate PCI configuration access to device */
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |				/* enable configuration access special cycle */
			MCF_PCI_PCICAR_BUSNUM(PCI_BUS_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_DEVNUM(PCI_DEVICE_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_FUNCNUM(PCI_FUNCTION_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_DWORD(offset / 4);
	
	__asm__ __volatile__("tpf" ::: "memory");

	* (volatile uint16_t *) (PCI_IO_OFFSET + (offset & 2)) = value;
	
	__asm__ __volatile__("tpf" ::: "memory");

	/* finish configuration space access cycle */
	MCF_PCI_PCICAR &= ~MCF_PCI_PCICAR_E;

	chip_errata_135();

	return PCI_SUCCESSFUL;
}

/*
 * write a single byte to config space
 */
int32_t pci_write_config_byte(int32_t handle, int offset, uint8_t value)
{
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |
			MCF_PCI_PCICAR_BUSNUM(PCI_BUS_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_DEVNUM(PCI_DEVICE_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_FUNCNUM(PCI_FUNCTION_FROM_HANDLE(handle)) |
			MCF_PCI_PCICAR_DWORD(offset / 4);
	
	__asm__ __volatile__("tpf" ::: "memory");

	* (volatile uint8_t *) (PCI_IO_OFFSET + (offset & 3)) = value;

	__asm__ __volatile__("tpf" ::: "memory");

	/* finish configuration space access cycle */
	MCF_PCI_PCICAR &= ~MCF_PCI_PCICAR_E;

	chip_errata_135();

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
	struct pci_rd *ret;

	index = handle2index(handle);
	if (index == -1)
		ret = NULL;
	else
		ret = &resource_descriptors[index][0];

	debug_printf("pci_get_resource: resource struct for handle %lx (index %d) is at %p\r\n", handle, index, ret);

	return ret;
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
	debug_printf("pci_hook_interrupt() still not implemented\r\n");
	return PCI_SUCCESSFUL;
}

int32_t pci_unhook_interrupt(int32_t handle)
{
	/* FIXME: implement */

	debug_printf("pci_unhook_interrupt() still not implemented\r\n");
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
	int32_t handle;
	int16_t index = - 1;
	struct pci_rd *descriptors;
	int i;
	uint32_t value;
	static uint32_t mem_address = PCI_MEMORY_OFFSET;
	static uint32_t io_address = PCI_IO_OFFSET;
	uint16_t cr;

	/* determine pci handle from bus, device + function number */
	handle = PCI_HANDLE(bus, device, function);

	/* find index into resource descriptor table for handle */
	index = handle2index(handle);

	if (index == -1)
	{
		debug_printf("cannot find index for handle %d\r\n", handle);
		return;
	}

	/*
	 * disable device
	 */
	
	cr = swpw(pci_read_config_word(handle, PCICSR));
	cr &= ~3;	/* disable device response to address */
	pci_write_config_word(handle, PCICSR, swpw(cr));

	int barnum = 0;

	descriptors = resource_descriptors[index];
	for (i = 0; i < 6 * 4; i += 4)		/* for all bars */
	{
		/*
		 * read BAR[i] value
		 */
		value = swpl(pci_read_config_longword(handle, PCIBAR0 + i));

		/*
		 * write all bits of BAR[i]
		 */
		pci_write_config_longword(handle, PCIBAR0 + i, 0xffffffff);

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
				debug_printf("device 0x%x: BAR[%d] requests %d bytes of memory\r\n", handle, i / 4, size);

				/* calculate a valid map adress with alignment requirements */
				address = (mem_address + size - 1) & ~(size - 1);

				/* write it to the BAR */
				pci_write_config_longword(handle, PCIBAR0 + i, swpl(address));

				/* read it back, just to be sure */
				value = swpl(pci_read_config_longword(handle, PCIBAR0 + i)) & ~1;
				
				debug_printf("set PCIBAR%d on device 0x%02x to 0x%08x\r\n",
						i / 4, handle, value);

				/* fill resource descriptor */
				rd->next = sizeof(struct pci_rd);
				rd->flags = 0 | FLG_32BIT | FLG_16BIT | FLG_8BIT | 2; /* little endian, lane swapped */
				rd->start = address;
				rd->length = size;
				rd->offset = 0; 
				rd->dmaoffset = 0;

				/* adjust memory adress for next turn */
				mem_address += size;

				cr |= 2;

				/* index to next unused resource descriptor */
				barnum++;
			}
			else if (IS_PCI_IO_BAR(value)) /* same as above for I/O resources */
			{
				int size = ~(address & 0xfffffffc) + 1;
				debug_printf("device 0x%x: BAR[%d] requests %d bytes of I/O space\r\n", handle, i, size);

				address = (io_address + size - 1) & ~(size - 1);
				pci_write_config_longword(handle, PCIBAR0 + i, swpl(address | 1));
				value = swpl(pci_read_config_longword(handle, PCIBAR0 + i));

				debug_printf("set PCIBAR%d on device 0x%02x to 0x%08x\r\n",
					i / 4, handle, value);

				rd->next = sizeof(struct pci_rd);
				rd->flags = FLG_IO | FLG_8BIT | FLG_16BIT | FLG_32BIT | 2;
				rd->start = address;
				rd->offset = 0;
				rd->length = size;
				rd->dmaoffset = 0;

				io_address += size;

				cr |= 1;

				barnum++;
			}
		}
	}
	/* mark end of resource chain */
	if (barnum > 0)
		descriptors[barnum - 1].flags |= FLG_LAST;

	/*
	 * enable device memory or I/O access
	 */
	pci_write_config_word(handle, PCICSR, swpw(cr));
}

static void pci_bridge_config(uint16_t bus, uint16_t device, uint16_t function)
{
	int32_t handle;

	if (function != 0)
	{
		debug_printf("trying to configure a multi-function bridge. Cancelled\r\n");
		return;
	}
	handle = PCI_HANDLE(bus, device, function);

	pci_write_config_longword(handle, PCIBAR0, 0x40000000);
	pci_write_config_longword(handle, PCIBAR1, 0x0);
	pci_write_config_longword(handle, PCICSR, 0x146);
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

		/* save handle to index value so that we'll be able to later find our resources */
		handles[index] = handle;
		if (PCI_VENDOR_ID(value) != 0x1057 && PCI_DEVICE_ID(value) != 0x5806) /* do not configure bridge */
		{
			/* configure memory and I/O for card */
			pci_device_config(PCI_BUS_FROM_HANDLE(handle),
						PCI_DEVICE_FROM_HANDLE(handle),
						PCI_FUNCTION_FROM_HANDLE(handle));
		}
		else
		{
			pci_bridge_config(PCI_BUS_FROM_HANDLE(handle),
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
	/* configure IRQ1-7 pins on EPORT falling edge triggered */
	MCF_EPORT_EPPAR = MCF_EPORT_EPPAR_EPPA7(MCF_EPORT_EPPAR_FALLING) |
			MCF_EPORT_EPPAR_EPPA6(MCF_EPORT_EPPAR_FALLING) |
#if MACHINE_FIREBEE		/* irq5 level triggered on FireBee */
			MCF_EPORT_EPPAR_EPPA5(MCF_EPORT_EPPAR_LEVEL) |
#elif MACHINE_M5484LITE
			MCF_EPORT_EPPAR_EPPA5(MCF_EPORT_EPPAR_FALLING) |
#endif /* MACHINE_FIREBEE */
			MCF_EPORT_EPPAR_EPPA4(MCF_EPORT_EPPAR_FALLING) |
			MCF_EPORT_EPPAR_EPPA3(MCF_EPORT_EPPAR_FALLING) |
			MCF_EPORT_EPPAR_EPPA2(MCF_EPORT_EPPAR_FALLING) |
			MCF_EPORT_EPPAR_EPPA1(MCF_EPORT_EPPAR_FALLING);
	MCF_EPORT_EPDDR = 0;	/* clear data direction register. All pins as input */
	MCF_EPORT_EPFR = -1;	/* clear all EPORT interrupt flags */
	MCF_EPORT_EPIER = 0xfe;	/* enable all EPORT interrupts (for now) */
}

void init_xlbus_arbiter(void)
{
	uint8_t clock_ratio;

	/* setup XL bus arbiter */
	clock_ratio = (MCF_PCI_PCIGSCR >> 24) & 0x07;
	if (clock_ratio == 4)
	{
		MCF_XLB_XARB_CFG = MCF_XLB_XARB_CFG_BA |
			MCF_XLB_XARB_CFG_DT |
			MCF_XLB_XARB_CFG_AT |
			MCF_XLB_XARB_CFG_PLDIS;
	}
	else
	{
		MCF_XLB_XARB_CFG = MCF_XLB_XARB_CFG_BA |
			MCF_XLB_XARB_CFG_DT |
			MCF_XLB_XARB_CFG_AT; 
	}

	MCF_XLB_XARB_ADRTO = 0x1fffff;
	MCF_XLB_XARB_DATTO = 0x1fffff;
	MCF_XLB_XARB_BUSTO = 0xffffff;

	/*
	 * set arbitration priorities for XL bus masters
	 *
	 * M0 = ColdFire core
	 * M2 = Multichannel DMA
	 * M3 = PCI target interface
	 */
	
	MCF_XLB_XARB_PRIEN = MCF_XLB_XARB_PRIEN_M0 |	/* activate programmed priority for Coldfire core */
					MCF_XLB_XARB_PRIEN_M2 |			/* activate programmed priority for Multichannel DMA */
					MCF_XLB_XARB_PRIEN_M3;			/* activate programmed priority for PCI target interface */
	MCF_XLB_XARB_PRI = MCF_XLB_XARB_PRI_M0P(7) |	/* Coldfire core gets lowest */
					MCF_XLB_XARB_PRI_M2P(5) |		/* Multichannel DMA mid priority */
					MCF_XLB_XARB_PRI_M3P(3);		/* PCI target interface is highest priority */
}

void init_pci(void)
{
	int res;

	xprintf("initializing PCI bridge:\r\n");

	(void) res; /* for now */
	res = register_interrupt_handler(0, INT_SOURCE_PCIARB, pci_arb_interrupt);
	debug_printf("registered interrupt handler for PCI arbiter: %s\r\n",
			(res < 0 ? "failed" : "succeeded"));
	register_interrupt_handler(0, INT_SOURCE_XLBPCI, xlb_pci_interrupt);
	debug_printf("registered interrupt handler for XLB PCI: %s\r\n",
			(res < 0 ? "failed" : "succeeded"));

	init_eport();
	init_xlbus_arbiter();

	MCF_PCI_PCIGSCR = 1;	/* reset PCI */

	/*
	 * setup the PCI arbiter
	 */
	MCF_PCIARB_PACR = MCF_PCIARB_PACR_INTMPRI	/* internal master priority: high */
       | MCF_PCIARB_PACR_EXTMPRI(0xf)			/* external master priority: high */
       | MCF_PCIARB_PACR_INTMINTEN				/* enable "internal master broken" interrupt */
       | MCF_PCIARB_PACR_EXTMINTEN(0x0f);		/* enable "external master broken" interrupt */

#ifdef _NOT_USED_	/* since this is already done in sysinit.c */
#if MACHINE_FIREBEE
	//MCF_PAD_PAR_PCIBG = 0x3f; // FIXME: MiNT initialization hangs if this is enabled ???
	//MCF_PAD_PAR_PCIBR = 0x3f;
#elif MACHINE_M5484LITE
	MCF_PAD_PAR_PCIBG = 0x3ff; /* enable all PCI bus grant and bus requests on the LITE board */
	MCF_PAD_PAR_PCIBR = 0x3ff;
#endif /* MACHINE_FIREBEE */
#endif /* _NOT_USED_ */

	MCF_PCI_PCISCR =  MCF_PCI_PCISCR_M | /* memory access control enabled */
		MCF_PCI_PCISCR_B |		/* bus master enabled */
		MCF_PCI_PCISCR_M |		/* mem access enable */
		MCF_PCI_PCISCR_MA |		/* clear master abort error */
		MCF_PCI_PCISCR_MW;		/* memory write and invalidate enabled */
		

	/* Setup burst parameters */
	MCF_PCI_PCICR1 = MCF_PCI_PCICR1_CACHELINESIZE(8) |
						MCF_PCI_PCICR1_LATTIMER(0xff); /* TODO: test increased latency timer */
#ifdef _NOT_USED_
	MCF_PCI_PCICR2 = MCF_PCI_PCICR2_MINGNT(1) |
					MCF_PCI_PCICR2_MAXLAT(32);
#endif /* _NOT_USED_ */
	MCF_PCI_PCICR2 = 0;		/* this is what Linux does */

	/* error signaling */
#ifdef NOT_USED
	MCF_PCI_PCIICR = MCF_PCI_PCIICR_TAE |	/* target abort enable */
		MCF_PCI_PCIICR_IAE; 				/* initiator abort enable */
#endif /* NOT_USED */
	MCF_PCI_PCIICR = 0;						/* this is what Linux does */
	
	MCF_PCI_PCIGSCR |= MCF_PCI_PCIGSCR_SEE;	/* system error interrupt enable */

	/* Configure Initiator Windows */

	/* initiator window 0 base / translation adress register */
	MCF_PCI_PCIIW0BTAR = (PCI_MEMORY_OFFSET | (((PCI_MEMORY_SIZE - 1) >> 8) & 0xffff0000))
						  | ((PCI_MEMORY_OFFSET >> 16) & 0xff00); 

	debug_printf("PCIIW0BTAR=0x%08x\r\n", MCF_PCI_PCIIW0BTAR);

	/* initiator window 1 base / translation adress register */
	MCF_PCI_PCIIW1BTAR = (PCI_IO_OFFSET | ((PCI_IO_SIZE - 1) >> 8)) & 0xffff0000;

	/* initiator window 2 base / translation address register */
	MCF_PCI_PCIIW2BTAR = 0L;   /* not used */

	/* initiator window configuration register */
	MCF_PCI_PCIIWCR = MCF_PCI_PCIIWCR_WINCTRL0_MEMRDLINE |
					MCF_PCI_PCIIWCR_WINCTRL1_IO |
					MCF_PCI_PCIIWCR_WINCTRL0_E |
					MCF_PCI_PCIIWCR_WINCTRL1_E;

	/*
	 * Initialize target control register.
	 * Used when an external bus master accesses the Coldfire PCI as target
	 */
	MCF_PCI_PCIBAR0 = 0x40000000;	/* 256 kB window */
	MCF_PCI_PCITBATR0 = (uint32_t) &_MBAR[0] |  MCF_PCI_PCITBATR0_EN;	/* target base address translation register 0 */
	MCF_PCI_PCIBAR1 = 0;			/* 1GB window */
	MCF_PCI_PCITBATR1 = MCF_PCI_PCITBATR1_EN;

	/* reset PCI devices */
	MCF_PCI_PCIGSCR &= ~MCF_PCI_PCIGSCR_PR;
	do {;} while (MCF_PCI_PCIGSCR & MCF_PCI_PCIGSCR_PR); /* wait until reset finished */
	xprintf("finished\r\n");

	/* initialize/clear resource descriptor table */
	memset(&resource_descriptors, 0, NUM_CARDS * NUM_RESOURCES * sizeof(struct pci_rd));
	/* initialize/clear handles array */
	memset(handles, 0, NUM_CARDS * sizeof(int32_t));

	/*
	 * do normal initialization
	 */
	pci_scan();


#ifdef DEBUG_PCI

	int index = 0;
	int handle;
	handle = pci_find_device(0x0, 0xFFFF, ++index);
	while (handle > 0)
	{
		uint32_t value;
		uint32_t addr;

		value = pci_read_config_longword(handle, PCIIDR);
		xprintf(" %02x | %02x | %02x |%04x|%04x|%04x| %s (0x%02x)\r\n",
				PCI_BUS_FROM_HANDLE(handle),
				PCI_DEVICE_FROM_HANDLE(handle),
				PCI_FUNCTION_FROM_HANDLE(handle),
				PCI_VENDOR_ID(value), PCI_DEVICE_ID(value),
				handle,
				device_class(pci_read_config_byte(handle, PCICCR)),
				pci_read_config_byte(handle, PCICCR));

		pci_print_device_abilities(handle);
		pci_print_device_config(handle);

		/* read some value from PCIBAR0 */
		addr = swpl(pci_read_config_longword(handle, PCIBAR0)) & ~0x1f;
		xprintf("%p = %08x\r\n", addr, * (uint32_t *) addr);

		pci_print_device_abilities(handle);
		pci_print_device_config(handle);

		handle = pci_find_device(0x0, 0xFFFF, ++index);
	}
#endif /* DEBUG_PCI */
}


#ifdef DEBUG_PCI
void pci_print_device_abilities(int32_t handle)
{
	uint16_t value;
	uint16_t saved_value;

	saved_value = pci_read_config_word(handle, PCICSR);
	pci_write_config_word(handle, PCICSR, 0xffff);
	value = swpw(pci_read_config_word(handle, PCICSR));
	debug_printf("IO: %1d MEM: %1d MSTR:%1d SPCC: %1d MEMW: %1d VGAS: %1d PERR: %1d STEP: %1d SERR: %1d FBTB: %1d\r\n",
			value & PCICSR_IO ? 1 : 0,
			value & PCICSR_MEMORY ? 1 : 0,
			value & PCICSR_MASTER ? 1 : 0,
			value & PCICSR_SPECIAL ? 1 : 0,
			value & PCICSR_MEMWI ? 1 : 0,
			value & PCICSR_VGA_SNOOP ? 1 : 0,
			value & PCICSR_PERR ? 1 : 0,
			value & PCICSR_STEPPING ? 1 : 0,
			value & PCICSR_SERR ? 1 : 0,
			value & PCICSR_FAST_BTOB_E ? 1 : 0);
	pci_write_config_word(handle, PCICSR, saved_value);
}


void pci_print_device_config(int32_t handle)
{
	uint16_t value;

	value = swpw(pci_read_config_word(handle, PCICSR + 2));
	debug_printf("66M: %1d UDF: %1d FB2B:%1d PERR: %1d TABR: %1d DABR: %1d SERR: %1d PPER: %1d\r\n",
			value & PCICSR_66MHZ ? 1 : 0,
			value & PCICSR_UDF ? 1 : 0,
			value & PCICSR_FAST_BTOB ? 1 : 0,
			value & PCICSR_DPARITY_ERROR ? 1 : 0,
			value & PCICSR_T_ABORT_S ? 1 : 0,
			value & PCICSR_T_ABORT_R ? 1 : 0,
			value & PCICSR_M_ABORT_R ? 1 : 0,
			value & PCICSR_S_ERROR_S ? 1 : 0,
			value & PCICSR_PARITY_ERR ? 1 : 0);
}
#endif /* DEBUG_PCI */

	
