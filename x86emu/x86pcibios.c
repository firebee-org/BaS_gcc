#include "radeonfb.h"
#include "pci.h"
#include "x86emu.h"
#include "x86pcibios.h"
#include "x86emu_regs.h"
#include "bas_printf.h"

extern unsigned short offset_port;

#define DEBUG
#include "debug.h"

int x86_pcibios_handler(struct X86EMU *emu)
{
    int ret = 0;
    unsigned long dev;

    switch (emu->x86.R_AX)
    {
        case PCI_BIOS_PRESENT:
            dbg("PCI_BIOS_PRESENT\r\n");
            emu->x86.R_AH	= 0x00;         /* no config space/special cycle support */
            emu->x86.R_AL	= 0x01;         /* config mechanism 1 */
            emu->x86.R_EDX = 'P' | 'C' << 8 | 'I' << 16 | ' ' << 24;
            emu->x86.R_EBX = 0x0210;        /* Version 2.10 */
            emu->x86.R_ECX = 0xFF00;        /* FixME: Max bus number */
            emu->x86.R_EFLG &= ~FB_CF;      /* clear carry flag */
            ret = 1;
            break;

        case FIND_PCI_DEVICE:
            dbg("FIND_PCI_DEVICE vendor = %04x, device = %04x\r\n", emu->x86.R_DX, emu->x86.R_CX);
            dev = pci_find_device((uintptr_t) emu->x86.R_DX, ((unsigned long) emu->x86.R_CX), 0);

            if (dev != 0)
            {
                dbg("dev = %d\r\n", dev);
                emu->x86.R_BH = PCI_BUS_FROM_HANDLE(dev);
                //X86_BH = (char)(dev >> 16) / PCI_MAX_FUNCTION); // dev->bus->secondary;
                emu->x86.R_BL = PCI_DEVICE_FROM_HANDLE(dev) << 3 | PCI_FUNCTION_FROM_HANDLE(dev);
                //X86_BL = (char)dev; // dev->path.u.pci.devfn;
                emu->x86.R_AH = SUCCESSFUL;
                emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
                ret = 1;
            }
            else
            {
                dbg("device not found\r\n");
                emu->x86.R_AH = DEVICE_NOT_FOUND;
                emu->x86.R_EFLG |= FB_CF;	/* set carry flag */
                ret = 0;
            }
            break;

        case FIND_PCI_CLASS_CODE:
            /* FixME: support SI != 0 */
            dbg("FIND_PCI_CLASS_CODE %x", emu->x86.R_ECX);
            dev = pci_find_classcode(emu->x86.R_ECX, 0);
            if (dev != 0) {
                dbg(" ...OK\r\n");
                emu->x86.R_BH = PCI_BUS_FROM_HANDLE(dev);
                emu->x86.R_BL = PCI_DEVICE_FROM_HANDLE(dev) << 3 | PCI_FUNCTION_FROM_HANDLE(dev);
                emu->x86.R_AH = SUCCESSFUL;
                emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
                ret = 1;
            }
            else
            {
                dbg(" ... error\r\n");
                emu->x86.R_AH = DEVICE_NOT_FOUND;
                emu->x86.R_EFLG |= FB_CF;	/* set carry flag */
                ret = 0;
            }
            break;

        case READ_CONFIG_BYTE:
            // bus, devfn
            dbg("READ_CONFIG_BYTE bus = %x, devfn = %x, reg = %x\r\n", emu->x86.R_BH, emu->x86.R_BL, emu->x86.R_DI);
            dev = PCI_HANDLE(emu->x86.R_BH, emu->x86.R_BL >> 3, emu->x86.R_BL & 7);
            emu->x86.R_CL = pci_read_config_byte(dev, emu->x86.R_DI);
            dbg("dev=0x%04x value = 0x%04x\r\n", emu->x86.R_CL);
            emu->x86.R_AH = SUCCESSFUL;
            emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
            ret = 1;
            break;

        case READ_CONFIG_WORD:
            // bus, devfn
            dbg("READ_CONFIG_WORD bus = %x, devfn = %x, reg = %x\r\n", emu->x86.R_BH, emu->x86.R_BL, emu->x86.R_DI);
            dev = PCI_HANDLE(emu->x86.R_BH, emu->x86.R_BL >> 3, emu->x86.R_BL & 7);
            if (emu->x86.R_DI == PCIBAR1)
                emu->x86.R_CX = offset_port + 1;
            else
                emu->x86.R_CX = pci_read_config_word(dev, emu->x86.R_DI);
            dbg("offset_port=0x%04x dev=0x%04x, value = %x\r\n", offset_port, dev, emu->x86.R_CX);
            emu->x86.R_AH = SUCCESSFUL;
            emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
            ret = 1;
            break;

        case READ_CONFIG_DWORD:
            // bus, devfn
            dbg("READ_CONFIG_DWORD bus = %x, devfn = %x, reg = %x\r\n", emu->x86.R_BH, emu->x86.R_BL, emu->x86.R_DI);
            dev = PCI_HANDLE(emu->x86.R_BH, emu->x86.R_BL >> 3, emu->x86.R_BL & 7);
            if (emu->x86.R_DI == PCIBAR1)
                emu->x86.R_CX = (unsigned long) offset_port + 1;
            else
                emu->x86.R_ECX = pci_read_config_longword(dev, emu->x86.R_DI);
            dbg("value = %x\r\n", emu->x86.R_ECX);
            emu->x86.R_AH = SUCCESSFUL;
            emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
            ret = 1;
            break;

        case WRITE_CONFIG_BYTE:
            // bus, devfn
            dbg("READ_CONFIG_BYTE bus = %x, devfn = %x, reg = %x, value = %x\r\n",
                emu->x86.R_BH, emu->x86.R_BL, emu->x86.R_DI, emu->x86.R_CL);
            dev = PCI_HANDLE(emu->x86.R_BH, emu->x86.R_BL >> 3, emu->x86.R_BL & 7);
            pci_write_config_byte(dev, emu->x86.R_DI, emu->x86.R_CL);
            emu->x86.R_AH = SUCCESSFUL;
            emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
            ret = 1;
            break;

        case WRITE_CONFIG_WORD:
            // bus, devfn
            dev = PCI_HANDLE(emu->x86.R_BH, emu->x86.R_BL >> 3, emu->x86.R_BL & 7);
            dbg("WRITE_CONFIG_WORD bus = %x, devfn = %x, reg = %x, value = %x\r\n", emu->x86.R_BH, emu->x86.R_BL, emu->x86.R_DI, emu->x86.R_CX);
            if (emu->x86.R_DI == PCIBAR1)
            {
                offset_port = emu->x86.R_CX;
                emu->x86.R_AH = SUCCESSFUL;
                emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
                ret = 1;
                break;
            }
            pci_write_config_word(dev, emu->x86.R_DI, emu->x86.R_CX);
            emu->x86.R_AH = SUCCESSFUL;
            emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
            ret = 1;
            break;

        case WRITE_CONFIG_DWORD:
            // bus, devfn
            dev = PCI_HANDLE(emu->x86.R_BH, emu->x86.R_BL >> 3, emu->x86.R_BL & 7);
            dbg("WRITE_CONFIG_DWORD bus = %x, devfn = %x, value = %x\r\n",
                emu->x86.R_BH, emu->x86.R_BL, emu->x86.R_DI, emu->x86.R_ECX);
            if (emu->x86.R_DI == PCIBAR1)
            {
                offset_port = (unsigned short) emu->x86.R_ECX & 0xFFFC;
                emu->x86.R_AH = SUCCESSFUL;
                emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
                ret = 1;
                break;
            }
            pci_write_config_longword(dev, emu->x86.R_DI, emu->x86.R_ECX);
            emu->x86.R_AH = SUCCESSFUL;
            emu->x86.R_EFLG &= ~FB_CF;	/* clear carry flag */
            ret = 1;
            break;

        default:
            dbg("PCI_BIOS FUNC_NOT_SUPPORTED\r\n");
            emu->x86.R_AH = FUNC_NOT_SUPPORTED;
            emu->x86.R_EFLG |= FB_CF;
            break;
    }

    return ret;
}
