#define RINFO_ONLY
#include "x86debug.h"
#include "radeonfb.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "util.h"
#include "driver_mem.h"
#include "x86emu.h"
#include "pci.h"
#include "pci_ids.h"

#define USE_SDRAM
#define DIRECT_ACCESS

#define MEM_WB(where, what) wrb(where, what)
#define MEM_WW(where, what) wrw(where, what)
#define MEM_WL(where, what) wrl(where, what)

#define MEM_RB(where) rdb(where)
#define MEM_RW(where) rdw(where)
#define MEM_RL(where) rdl(where)

#define PCI_VGA_RAM_IMAGE_START 0xC0000
#define PCI_RAM_IMAGE_START     0xD0000
#define SYS_BIOS                0xF0000
#define SIZE_EMU               0x100000

typedef struct
{
	long ident;
	union
	{
		long l;
		short i[2];
		char c[4];
	} v;
} COOKIE;

struct rom_header
{
	uint16_t	signature;
	uint8_t size;
	uint8_t init[3];
	uint8_t reserved[0x12];
	uint16_t	data;
};

struct pci_data
{
	uint32_t signature;
	uint16_t vendor;
	uint16_t device;
	uint16_t reserved_1;
	uint16_t dlen;
	uint8_t drevision;
	uint8_t class_lo;
	uint16_t class_hi;
	uint16_t ilen;
	uint16_t irevision;
	uint8_t type;
	uint8_t indicator;
	uint16_t	reserved_2;
};

static struct radeonfb_info *rinfo_biosemu;
uint16_t offset_port;
uint32_t offset_mem;
static uint32_t offset_io;
static uint32_t config_address_reg;

extern int x86_pcibios_emulator();
extern COOKIE *get_cookie(long id);
extern short restart, os_magic;

X86EMU_sysEnv _X86EMU_env;

/* general software interrupt handler */
uint32_t getIntVect(int num)
{
	return MEM_RW(num << 2) + (MEM_RW((num << 2) + 2) << 4);
}

/* FixME: There is already a push_word() in the emulator */
void pushw(uint16_t val)
{
	X86_ESP -= 2;
	MEM_WW(((uint32_t) X86_SS << 4) + X86_SP, val);
}

int run_bios_int(int num)
{
	uint32_t eflags;
	eflags = X86_EFLAGS;
	pushw(eflags);
	pushw(X86_CS);
	pushw(X86_IP);
	X86_CS = MEM_RW((num << 2) + 2);
	X86_IP = MEM_RW(num << 2);
	return 1;
}

uint8_t inb(uint16_t port)
{
	uint8_t val = 0;

	if ((port >= offset_port) && (port <= offset_port + 0xFF))
	{
		val = * (uint8_t *) (offset_io + (uint32_t) port);
		//dbg("%s: inb(0x%x) = 0x%x\r\n", __FUNCTION__, port, val);
	}
	return val;
}

uint16_t inw(uint16_t port)
{
	uint16_t val = 0;

	if ((port >= offset_port) && (port <= offset_port + 0xFF))
	{
		val = swpw(*(uint16_t *)(offset_io + (uint32_t) port));
		//dbg("inw(0x%x) = 0x%x\r\n", port, val);
	}
	return val;
}

uint32_t inl(uint16_t port)
{
	uint32_t val = 0;
	if ((port >= offset_port) && (port <= offset_port + 0xFF))
	{
		val = swpl(*(uint32_t *)(offset_io + (uint32_t) port));
		//dbg("0x%x) = 0x%x\r\n", port, val);
	}
	else if (port == 0xCF8)
	{
		val = config_address_reg;
		dbg("inl(0x%x) = 0x%x\r\n", port, val);
	}
	else if ((port == 0xCFC) && ((config_address_reg & 0x80000000) != 0))
	{
		dbg("%s: PCI BIOS access to register %x\r\n", __FUNCTION__, config_address_reg);
		switch (config_address_reg & 0xFC)
		{
			case PCIIDR:
				val = ((uint32_t) rinfo_biosemu->chipset << 16) + PCI_VENDOR_ID_ATI;
				break;

			case PCIBAR1:
				val = (uint32_t) offset_port + 1;
				break;

			default: 
				val = pci_read_config_longword(rinfo_biosemu->handle, config_address_reg & 0xFC);
				break;
		}
		dbg("inl(0x%x) = 0x%x\r\n", port, val);
	}
	return val;
}

void outb(uint8_t val, uint16_t port)
{
	if ((port >= offset_port) && (port <= offset_port + 0xFF))
	{
		//dbg("outb(0x%x, 0x%x)\r\n", port, val);
		*(uint8_t *)(offset_io + (uint32_t) port) = val;
	}
}

void outw(uint16_t val, uint16_t port)
{
	if ((port >= offset_port) && (port <= offset_port + 0xFF))
	{
		//dbg("outw(0x%x, 0x%x)\r\n", port, val);
		*(uint16_t *)(offset_io + (uint32_t) port) = swpw(val);
	}
}

void outl(uint32_t val, uint16_t port)
{
	if ((port >= offset_port) && (port <= offset_port + 0xFF))
	{
		//dbg("outl(0x%x, 0x%x)\r\n", port, val);
		*(uint32_t *)(offset_io + (uint32_t) port) = swpl(val);
	}
	else if (port == 0xCF8)
	{
		dbg("outl(0x%x, 0x%x)\r\n", port, val);
		config_address_reg = val;
	}
	else if ((port == 0xCFC) && ((config_address_reg & 0x80000000) !=0))
	{
		if ((config_address_reg & 0xFC) == PCIBAR1)
			offset_port = (uint16_t)val & 0xFFFC;
		else
		{
			dbg("outl(0x%x, 0x%x) to PCI config space\r\n", port, val);
			pci_write_config_longword(rinfo_biosemu->handle, config_address_reg & 0xFC, val);
		}
	}
}

/* Interrupt multiplexer */

void do_int(int num)
{
	int ret = 0;

	switch (num)
	{
#ifndef _PC
	case 0x10:
	case 0x42:
	case 0x6D:
		if (getIntVect(num) == 0x0000)
			dbg("uninitialised int vector\r\n");
		if (getIntVect(num) == 0xFF065)
		{
			//ret = int42_handler();
			ret = 1;
		}
		break;
#endif
	case 0x15:
		//ret = int15_handler();
		ret = 1;
		break;
	case 0x16:
		//ret = int16_handler();
		ret = 0;
		break;
	case 0x1A:
		ret = x86_pcibios_emulator();
		ret = 1;
		break;
	case 0xe6:
		//ret = intE6_handler();
		ret = 0;
		break;
	default:
		break;
	}
	if (!ret)
		ret = run_bios_int(num);
}

static int setup_system_bios(void *base_addr)
{
	char *base = (char *) base_addr;
	int i;
	/*
	 * we trap the "industry standard entry points" to the BIOS
	 * and all other locations by filling them with "hlt"
	 * TODO: implement hlt-handler for these
	 */
	for(i = 0; i < SIZE_EMU + 4; base[i++] = 0xF4);

	return(1);
}

void run_bios(struct radeonfb_info *rinfo)
{
	long i, j;
	unsigned char *ptr;
	struct rom_header *rom_header;
	struct pci_data *rom_data;
	unsigned long rom_size=0;
	unsigned long image_size=0;
	void *biosmem = (void *) 0x0100000; /* when run_bios() is called, SDRAM is valid but not added to the system */
	unsigned long addr;
	unsigned short initialcs;
	unsigned short initialip;
	unsigned short devfn = (unsigned short) rinfo->handle;
	X86EMU_intrFuncs intFuncs[256];

	if ((rinfo->mmio_base == NULL) || (rinfo->io_base == NULL))
	{
		dbg("%s: rinfo->mmio_base = %p, rinfo->io_base = %p\r\n",
				__FUNCTION__, rinfo->mmio_base, rinfo->io_base);
		return;
	}

	rinfo_biosemu = rinfo;
	config_address_reg = 0;
	offset_port = 0x300;
	offset_io = (uint32_t) rinfo->io_base - (uint32_t) offset_port;
	offset_mem = (uint32_t) rinfo->fb_base - 0xA0000;

	rom_header = NULL;
	do
	{
		rom_header = (struct rom_header *) ((unsigned long) rom_header + image_size); // get next image
		rom_data = (struct pci_data *) ((unsigned long)rom_header + (unsigned long) BIOS_IN16((long) &rom_header->data));
		image_size = (unsigned long) BIOS_IN16((long) &rom_data->ilen) * 512;
	} while ((BIOS_IN8((long) &rom_data->type) != 0) && (BIOS_IN8((long) &rom_data->indicator) != 0));  // make sure we got x86 version

	if (BIOS_IN8((long) &rom_data->type) != 0)
	{
		dbg("%s: ROM data type = 0x%x\r\n", __FUNCTION__, BIOS_IN8((long) &rom_data->type));
		return;
	}

	rom_size = (unsigned long) BIOS_IN8((long) &rom_header->size) * 512;
	if (PCI_CLASS_DISPLAY_VGA == BIOS_IN16((long) &rom_data->class_hi))
	{
		memset((char *) biosmem, 0, SIZE_EMU);
		
		setup_system_bios((char *) biosmem);

		dbg("%s: Copying VGA ROM Image from %p to %p (0x%lx bytes)\r\n",
				__FUNCTION__, (long) rinfo->bios_seg + (long) rom_header,
				biosmem + PCI_VGA_RAM_IMAGE_START, rom_size);
		{
			long bytes_align = (long) rom_header & 3;

			ptr = (unsigned char *) biosmem;
			i = (long) rom_header;
			j = PCI_VGA_RAM_IMAGE_START;

			if (bytes_align)
				for(; i < 4 - bytes_align; ptr[j++] = BIOS_IN8(i++));

			for(; i < (long) rom_header + rom_size; i += 4, j += 4)
				*((unsigned long *) &ptr[j]) = swpl(BIOS_IN32(i));
		}
		addr = PCI_VGA_RAM_IMAGE_START;	
	}
	else
	{
		setup_system_bios((char *) biosmem);

		memset((char *) biosmem, 0, SIZE_EMU);

		dbg("%s: Copying non-VGA ROM Image from %p to %p (0x%lx bytes)\r\n", __FUNCTION__,
				(long) rinfo->bios_seg + (long) rom_header,
				biosmem + PCI_RAM_IMAGE_START,
				rom_size);
		ptr = (unsigned char *) biosmem;
		for (i = (long) rom_header, j = PCI_RAM_IMAGE_START; i < (long) rom_header+rom_size; ptr[j++] = BIOS_IN8(i++));
		addr = PCI_RAM_IMAGE_START;
	}

	initialcs = (addr & 0xF0000) >> 4;
	initialip = (addr + 3) & 0xFFFF;	
	X86EMU_setMemBase((void *) biosmem, SIZE_EMU);

	for (i = 0; i < 256; i++)
		intFuncs[i] = do_int;

	X86EMU_setupIntrFuncs(intFuncs);
	{
		char *date = "01/01/99";
		for (i = 0; date[i]; i++)
			wrb(0xffff5 + i, date[i]);
		wrb(0xffff7, '/');
		wrb(0xffffa, '/');
	}
	{
    	/* FixME: move PIT init to its own file */
    	outb(0x36, 0x43);
    	outb(0x00, 0x40);
    	outb(0x00, 0x40);
	}
//	setup_int_vect();

	/* cpu setup */
	X86_AX = devfn ? devfn : 0xff;
	X86_DX = 0x80;
	X86_EIP = initialip;
	X86_CS = initialcs;

	/* Initialize stack and data segment */
	X86_SS = initialcs;
	X86_SP = 0xfffe;
	X86_DS = 0x0040;
	X86_ES = 0x0000;

	/*
	 * We need a sane way to return from bios
	 * execution. A hlt instruction and a pointer
	 * to it, both kept on the stack, will do.
	 */
	pushw(0xf4f4);    /* hlt; hlt */
//	pushw(0x10cd);    /* int #0x10 */
//	pushw(0x0013);    /* 320 x 200 x 256 colors */
// //	pushw(0x000F);    /* 640 x 350 x mono */
//	pushw(0xb890);    /* nop, mov ax,#0x13 */
	pushw(X86_SS);
	pushw(X86_SP + 2);
#ifdef DBG_X86EMU
	X86EMU_trace_on();
	X86EMU_set_debug(DEBUG_DECODE_F | DEBUG_TRACE_F);
#endif

	dbg("%s: X86EMU entering emulator\r\n", __FUNCTION__);
	//*vblsem = 0;
	X86EMU_exec();
	//*vblsem = 1;
	dbg("%s: X86EMU halted\r\n", __FUNCTION__);
//	biosfn_set_video_mode(0x13); /* 320 x 200 x 256 colors */
	memset((char *) biosmem, 0, SIZE_EMU);
}
