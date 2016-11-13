#ifndef PCI_BIOS_H
#define PCI_BIOS_H

enum {
    PCI_BIOS_PRESENT        = 0xB101,
    FIND_PCI_DEVICE         = 0xB102,
    FIND_PCI_CLASS_CODE     = 0xB103,
	GENERATE_SPECIAL_CYCLE	= 0xB106,
    READ_CONFIG_BYTE        = 0xB108,
    READ_CONFIG_WORD        = 0xB109,
    READ_CONFIG_DWORD       = 0xB10A,
    WRITE_CONFIG_BYTE       = 0xB10B,
    WRITE_CONFIG_WORD       = 0xB10C,
    WRITE_CONFIG_DWORD      = 0xB10D,
	GET_IRQ_ROUTING_OPTIONS	= 0xB10E,
    SET_PCI_IRQ             = 0xB10F
};

enum {
    SUCCESSFUL          = 0x00,
	FUNC_NOT_SUPPORTED	= 0x81,
	BAD_VENDOR_ID		= 0x83,
	DEVICE_NOT_FOUND	= 0x86,
	BAD_REGISTER_NUMBER	= 0x87,
    SET_FAILED          = 0x88,
	BUFFER_TOO_SMALL	= 0x89
};

extern int x86_pcibios_handler(struct X86EMU *emu);


#define USE_SDRAM
#define DIRECT_ACCESS

#define MEM_WB(where, what) emu->emu_wrb(emu, where, what)
#define MEM_WW(where, what) emu->emu_wrw(emu, where, what)
#define MEM_WL(where, what) emu->emu_wrl(emu, where, what)

#define MEM_RB(where) emu->emu_rdb(emu, where)
#define MEM_RW(where) emu->emu_rdw(emu, where)
#define MEM_RL(where) emu->emu_rdl(emu, where)

#define PCI_VGA_RAM_IMAGE_START     0xC0000
#define PCI_RAM_IMAGE_START         0xD0000
#define SYS_BIOS                    0xF0000
#define SIZE_EMU                    0x100000
#define BIOS_MEM                    0x0UL


#endif /* PCI_BIOS_H */

