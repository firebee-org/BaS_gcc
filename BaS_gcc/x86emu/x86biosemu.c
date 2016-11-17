#define RINFO_ONLY
#include "radeonfb.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "util.h"
#include "driver_mem.h"
#include "x86emu.h"
#include "x86emu_regs.h"
#include "pci.h"
#include "pci_ids.h"
#include "x86pcibios.h"

#define DEBUG
#include "debug.h"

struct rom_header
{
    uint16_t signature;
    uint8_t size;
    uint8_t init[3];
    uint8_t reserved[0x12];
    uint16_t data;
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
    uint16_t reserved_2;
};

static struct radeonfb_info *rinfo_biosemu;
uint16_t offset_port;
uint32_t offset_mem;
static uint32_t offset_io;
static uint32_t config_address_reg;


/* general software interrupt handler */
static uint32_t getIntVect(struct X86EMU *emu, int num)
{
    return MEM_RW(num << 2) + (MEM_RW((num << 2) + 2) << 4);
}

/* FixME: There is already a push_word() in the emulator */
static void pushw(struct X86EMU *emu, uint16_t val)
{
    emu->x86.R_ESP -= 2;
    MEM_WW(((uint32_t) emu->x86.R_SS << 4) + emu->x86.R_SP, val);
}

static int run_bios_int(struct X86EMU *emu, int num)
{
    uint32_t eflags;

    eflags = emu->x86.R_EFLG;
    pushw(emu, eflags);
    pushw(emu, emu->x86.R_CS);
    pushw(emu, emu->x86.R_IP);
    emu->x86.R_CS = MEM_RW((num << 2) + 2);
    emu->x86.R_IP = MEM_RW(num << 2);

    return 1;
}

static uint8_t inb(struct X86EMU *emu, uint16_t port)
{
    uint8_t val = 0;

    if ((port >= offset_port) && (port <= offset_port + 0xff))
    {
        val = * (uint8_t *) (offset_io + (uint32_t) port);
    }
    else
        dbg("illegal port 0x%x\r\n", port);

    return val;
}

static uint16_t inw(struct X86EMU *emu, uint16_t port)
{
    uint16_t val = 0;

    if ((port >= offset_port) && (port <= offset_port + 0xFF))
    {
        val = swpw(*(uint16_t *)(offset_io + (uint32_t) port));
    }
    else
        dbg("illegal port 0x%x\r\n", port);

    return val;
}

#define PC_PCI_INDEX_PORT       0xcf8
#define PC_PCI_DATA_PORT        0xcfc

static uint32_t inl(struct X86EMU *emu, uint16_t port)
{
    uint32_t val = 0;

    if ((port >= offset_port) && (port <= offset_port + 0xFF))
    {
        val = swpl(*(uint32_t *)(offset_io + (uint32_t) port));
    }
    else if (port == PC_PCI_INDEX_PORT)
    {
        val = config_address_reg;
    }
    else if ((port == PC_PCI_DATA_PORT) && ((config_address_reg & 0x80000000) != 0))
    {
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
        // dbg("PCI inl from register %x, value = 0x%08x\r\n", config_address_reg, val);
    }
    else
        dbg("illegal port 0x%x\r\n", port);

    return val;
}

static void outb(struct X86EMU *emu, uint16_t port, uint8_t val)
{
    if ((port >= offset_port) && (port <= offset_port + 0xFF))
    {
        *(uint8_t *)(offset_io + (uint32_t) port) = val;
    }
    else
        dbg("illegal port 0x%x\r\n", port);
}

static void outw(struct X86EMU *emu, uint16_t port, uint16_t val)
{
    if ((port >= offset_port) && (port <= offset_port + 0xFF))
    {
        *(uint16_t *)(offset_io + (uint32_t) port) = swpw(val);
    }
    else
        dbg("illegal port 0x%x\r\n", port);
}

static void outl(struct X86EMU *emu, uint16_t port, uint32_t val)
{
    if ((port >= offset_port) && (port <= offset_port + 0xFF))
    {
        *(uint32_t *)(offset_io + (uint32_t) port) = swpl(val);
    }
    else if (port == PC_PCI_INDEX_PORT)
    {
        config_address_reg = val;
    }
    else if ((port == PC_PCI_DATA_PORT) && ((config_address_reg & 0x80000000) !=0))
    {
        dbg("outl(0x%x, 0x%x) to PCI config space\r\n", port, val);
        if ((config_address_reg & 0xFC) == PCIBAR1)
        {
            offset_port = (uint16_t) val & 0xFFFC;
        }
        else
        {
            pci_write_config_longword(rinfo_biosemu->handle, config_address_reg & 0xFC, val);
        }
    }
    else
        dbg("illegal port 0x%x\r\n", port);
}

/* Interrupt multiplexer */

static void do_int(struct X86EMU *emu, int num)
{
    int ret = 0;

    switch (num)
    {
        case 0x10:
            /* video interrupt */
            /* fall through intentional */

        case 0x42:
            /* video interrupt */
            /* fall through intentional */

        case 0x6d:
            /* VGA internal interrupt */

            dbg("int %02xh, AH=0x%02x, AL=0x%02x\r\n", num,
                emu->x86.register_a.I8_reg.h_reg,
                emu->x86.register_a.I8_reg.l_reg);

            if (emu->x86.register_a.I8_reg.h_reg == 0x13)       /* VGA write string */
            {
                int num_chars = emu->x86.register_c.I16_reg.x_reg;
                int seg = emu->x86.register_es;
                int off = emu->x86.register_bp.I16_reg.x_reg;
                int str = (seg << 4) + off;
                int i;

                dbg("string to output at 0x%04x:0x%04x length=0x%04x\r\n", seg, off, num_chars);

                for (i = 0; i < num_chars; i++)
                    xprintf("%c", * (char *)(BIOS_MEM + str + i));
            }

            if (getIntVect(emu, num) == 0x0000)
                err("uninitialised int vector\r\n");

            if (getIntVect(emu, num) == 0xFF065)
            {
                //ret = int42_handler();
                ret = 1;
            }
            break;

        case 0x15:
            //ret = int15_handler();
            ret = 1;
            break;

        case 0x16:
            //ret = int16_handler();
            ret = 0;
            break;

        case 0x1a:
            ret = x86_pcibios_handler(emu);
            ret = 1;
            break;

        case 0xe6:
            //ret = intE6_handler();
            ret = 0;
            break;

        default:
            dbg("unhandled interrupt 0x%x\r\n", num);
            break;
    }

    if (!ret)
    {
        ret = run_bios_int(emu, num);
    }
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

    for (i = 0; i < SIZE_EMU + 4; base[i++] = 0xF4);

    return 1;
}

void run_bios(struct radeonfb_info *rinfo)
{
    long i;
    long j;
    unsigned char *ptr;
    struct rom_header *rom_header;
    struct pci_data *rom_data;
    unsigned long rom_size = 0;
    unsigned long image_size = 0;
    unsigned long addr;
    unsigned short initialcs;
    unsigned short initialip;
    unsigned short devfn = (unsigned short) rinfo->handle;

    struct X86EMU emu = { 0 };

    X86EMU_init_default(&emu);
    emu.emu_inb = inb;
    emu.emu_inw = inw;
    emu.emu_inl = inl;

    emu.emu_outb = outb;
    emu.emu_outw = outw;
    emu.emu_outl = outl;


    if ((rinfo->mmio_base == NULL) || (rinfo->io_base == NULL))
    {
        dbg("rinfo->mmio_base = %p, rinfo->io_base = %p\r\n", rinfo->mmio_base, rinfo->io_base);
        return;
    }

    rinfo_biosemu = rinfo;
    config_address_reg = 0;
    offset_port = 0x300;
    offset_io = (uint32_t) rinfo->io_base - (uint32_t) offset_port;
    offset_mem = (uint32_t) rinfo->fb_base - 0xa0000;

    rom_header = NULL;

    do
    {
        rom_header = (struct rom_header *) ((uintptr_t) rom_header + image_size); // get next image
        rom_data = (struct pci_data *) ((uintptr_t) rom_header + (uintptr_t) BIOS_IN16((long) &rom_header->data));
        image_size = (size_t) BIOS_IN16((long) &rom_data->ilen) * 512;
    } while ((BIOS_IN8((long) &rom_data->type) != 0) && (BIOS_IN8((long) &rom_data->indicator) != 0));  // make sure we got x86 version

    if (BIOS_IN8((long) &rom_data->type) != 0)
    {
        dbg("unknown ROM data type = 0x%x\r\n", BIOS_IN8((long) &rom_data->type));
        return;
    }

    rom_size = (size_t) BIOS_IN8((uintptr_t) &rom_header->size) * 512;
    dbg("ROM size = 0x%lx\r\n", rom_size);

    if (PCI_CLASS_DISPLAY_VGA == BIOS_IN16((long) &rom_data->class_hi))
    {
        memset((char *) BIOS_MEM, 0, SIZE_EMU);
        setup_system_bios((char *) BIOS_MEM);

        dbg("Copyg VGA ROM Image from %p to %p (0x%lx bytes)\r\n",
            (uintptr_t) rinfo->bios_seg + (uintptr_t) rom_header,
            BIOS_MEM + PCI_VGA_RAM_IMAGE_START, rom_size);
        {
            long bytes_align = (uintptr_t) rom_header & 3;

            ptr = (uint8_t *) BIOS_MEM;
            i = (long) rom_header;
            j = PCI_VGA_RAM_IMAGE_START;

            if (bytes_align)
            {
                for (; i < 4 - bytes_align; ptr[j++] = BIOS_IN8(i++));
            }

            for (; i < (long) rom_header + rom_size; i += 4, j += 4)
            {
                *((uintptr_t *) &ptr[j]) = swpl(BIOS_IN32(i));
            }
        }
        addr = PCI_VGA_RAM_IMAGE_START;
    }
    else
    {
        memset((uint8_t *) BIOS_MEM, 0, SIZE_EMU);
        setup_system_bios((char *) BIOS_MEM);

        dbg("Copy non-VGA ROM Image from %p to %p (0x%lx bytes)\r\n",
            (uintptr_t) rinfo->bios_seg + (uintptr_t) rom_header,
            BIOS_MEM + PCI_RAM_IMAGE_START,
            rom_size);
        ptr = (uint8_t *) BIOS_MEM;
        for (i = (long) rom_header, j = PCI_RAM_IMAGE_START; i < (long) rom_header + rom_size; ptr[j++] = BIOS_IN8(i++));
        addr = PCI_RAM_IMAGE_START;
    }

    initialcs = (addr & 0xf0000) >> 4;
    initialip = (addr + 3) & 0xffff;
    dbg("initial CS=0x%x, initial IP=0x%x\r\n", initialcs, initialip);

    /*
     * set emulator memory
     */
    emu.mem_base = (void *) BIOS_MEM;
    emu.mem_size = SIZE_EMU;

    for (i = 0; i < 256; i++)
    {
        emu._X86EMU_intrTab[i] = do_int;
    }

    char *date = "01/01/99";

    for (i = 0; date[i]; i++)
    {
        emu.emu_wrb(&emu, 0xffff5 + i, date[i]);
    }
    emu.emu_wrb(&emu, 0xffff7, '/');
    emu.emu_wrb(&emu, 0xffffa, '/');

    /*
     * FixME: move PIT (programmable interval timer) init to its own file
     * do we really need this? - don't think it does anything sensible ...
     */
    outb(&emu, 0x36, 0x43);
    outb(&emu, 0x00, 0x40);
    outb(&emu, 0x00, 0x40);

    //	setup_int_vect();

    /* cpu setup */
    emu.x86.R_AX = devfn ? devfn : 0xff;
    emu.x86.R_DX = 0x80;
    emu.x86.R_IP = initialip;
    emu.x86.R_CS = initialcs;

    /* Initialize stack and data segment */
    emu.x86.R_SS = initialcs;
    emu.x86.R_SP = 0xfffe;
    emu.x86.R_DS = 0x0040;
    emu.x86.R_ES = 0x0000;

    /*
     * We need a sane way to return from bios
     * execution. A hlt instruction and a pointer
     * to it, both kept on the stack, will do.
     */
    pushw(&emu, 0xf4f4);    /* hlt; hlt */

    //	pushw(0x10cd);    /* int #0x10 */
    //	pushw(0x0013);    /* 320 x 200 x 256 colors */
    // //	pushw(0x000F);    /* 640 x 350 x mono */
    //	pushw(0xb890);    /* nop, mov ax,#0x13 */

    pushw(&emu, emu.x86.R_SS);
    pushw(&emu, emu.x86.R_SP + 2);

    dbg("X86EMU entering emulator\r\n");

    X86EMU_exec(&emu);

    dbg("X86EMU halted\r\n");
    //	biosfn_set_video_mode(0x13); /* 320 x 200 x 256 colors */

    /*
     * clear emulator memory once we are finished
     */
    memset((char *) BIOS_MEM, 0, SIZE_EMU);
}
