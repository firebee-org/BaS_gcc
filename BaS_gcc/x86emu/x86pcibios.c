#include "radeonfb.h"
#include "pci.h"
#include "x86emu.h"
#include "x86pcibios.h"

extern unsigned short offset_port;

int x86_pcibios_emulator()
{
	int ret = 0;
	unsigned long dev;

	switch (X86_AX)
	{
		case PCI_BIOS_PRESENT:
			dbg("%s: PCI_BIOS_PRESENT\r\n", __FUNCTION__);
			X86_AH	= 0x00;			/* no config space/special cycle support */
			X86_AL	= 0x01;			/* config mechanism 1 */
			X86_EDX = 'P' | 'C' << 8 | 'I' << 16 | ' ' << 24;
			X86_EBX = 0x0210;		/* Version 2.10 */
			X86_ECX = 0xFF00;		/* FixME: Max bus number */
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;

		case FIND_PCI_DEVICE:
			dbg("%s: FIND_PCI_DEVICE vendor = %04x, device = %04x\r\n", __FUNCTION__, X86_DX, X86_CX);
			dev = pci_find_device((unsigned long) X86_DX, ((unsigned long) X86_CX), 0);

			if (dev != 0)
			{
				dbg("%s:  ... OK\r\n", __FUNCTION__);		
				X86_BH = PCI_BUS_FROM_HANDLE(dev);
				//X86_BH = (char)(dev >> 16) / PCI_MAX_FUNCTION); // dev->bus->secondary;
				X86_BL = PCI_DEVICE_FROM_HANDLE(dev) << 3 | PCI_FUNCTION_FROM_HANDLE(dev);
				//X86_BL = (char)dev; // dev->path.u.pci.devfn;
				X86_AH = SUCCESSFUL;
				X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
				ret = 1;
			} else {
				dbg("%s: ... error\r\n", __FUNCTION__);		
				X86_AH = DEVICE_NOT_FOUND;
				X86_EFLAGS |= FB_CF;	/* set carry flag */
				ret = 0;
			}
			break;

		case FIND_PCI_CLASS_CODE:
			/* FixME: support SI != 0 */
			dbg("%s: FIND_PCI_CLASS_CODE %x\r\n", __FUNCTION__, X86_ECX);
			dev = pci_find_classcode(X86_ECX, 0);
			if (dev != 0) {
				dbg("%s: ... OK\r\n", __FUNCTION__);		
				X86_BH = PCI_BUS_FROM_HANDLE(dev);
				X86_BL = PCI_DEVICE_FROM_HANDLE(dev) << 3 | PCI_FUNCTION_FROM_HANDLE(dev);
				X86_AH = SUCCESSFUL;
				X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
				ret = 1;
			}
			else
			{
				dbg("%s: ... error\r\n", __FUNCTION__);		
				X86_AH = DEVICE_NOT_FOUND;
				X86_EFLAGS |= FB_CF;	/* set carry flag */
				ret = 0;
			}
			break;

		case READ_CONFIG_BYTE:
			// bus, devfn
			dbg("%s: READ_CONFIG_BYTE bus = %x, devfn = %x, reg = %x\r\n", __FUNCTION__, X86_BH, X86_BL, X86_DI);
			dev = PCI_HANDLE(X86_BH, X86_BL >> 3, X86_BL & 3); 
			X86_CL = pci_read_config_byte(dev, X86_DI);
			dbg("%s: value = %x\r\n", __FUNCTION__, X86_CL);
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;

		case READ_CONFIG_WORD:
			// bus, devfn
			dbg("%s: READ_CONFIG_WORD bus = %x, devfn = %x, reg = %x\r\n", __FUNCTION__, X86_BH, X86_BL, X86_DI);
			dev = PCI_HANDLE(X86_BH, X86_BL >> 3, X86_BL & 3); 
			if(X86_DI == PCIBAR1)
				X86_CX = offset_port + 1;
			else
				X86_CX = pci_read_config_word(dev, X86_DI);
			dbg("%s: value = %x\r\n", __FUNCTION__, X86_CX);
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;

		case READ_CONFIG_DWORD:
			// bus, devfn
			dbg("%s: READ_CONFIG_DWORD bus = %x, devfn = %x, reg = %x\r\n", __FUNCTION__, X86_BH, X86_BL, X86_DI);
			dev = PCI_HANDLE(X86_BH, X86_BL >> 3, X86_BL & 3); 
			if (X86_DI == PCIBAR1)
				X86_CX = (unsigned long) offset_port + 1;
			else
				X86_ECX = pci_read_config_longword(dev, X86_DI);
			dbg("%s: value = %x\r\n", __FUNCTION__, X86_ECX);
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;

		case WRITE_CONFIG_BYTE:
			// bus, devfn
			dbg("%s: READ_CONFIG_BYTE bus = %x, devfn = %x, reg = %x, value = %x\r\n", __FUNCTION__,
						X86_BH, X86_BL, X86_DI, X86_CL);
			dev = PCI_HANDLE(X86_BH, X86_BL >> 3, X86_BL & 3); 
			pci_write_config_byte(dev, X86_DI, X86_CL);
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;

		case WRITE_CONFIG_WORD:
			// bus, devfn
			dev = PCI_HANDLE(X86_BH, X86_BL >> 3, X86_BL & 3); 
			dbg("%s: WRITE_CONFIG_WORD bus = %x, devfn = %x, reg = %x, value = %x\r\n", X86_BH, X86_BL, X86_DI, X86_CX);
			if (X86_DI == PCIBAR1)
			{
				offset_port = X86_CX;
				X86_AH = SUCCESSFUL;
				X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
				ret = 1;
				break;
			}
			pci_write_config_word(dev, X86_DI, X86_CX);
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;

		case WRITE_CONFIG_DWORD:
			// bus, devfn
			dev = PCI_HANDLE(X86_BH, X86_BL >> 3, X86_BL & 3); 
			dbg("%s: WRITE_CONFIG_DWORD bus = %x, devfn = %x, value = %x\r\n", __FUNCTION__,
					X86_BH, X86_BL, X86_DI, X86_ECX);
			if (X86_DI == PCIBAR1)
			{
				offset_port = (unsigned short) X86_ECX & 0xFFFC;
				X86_AH = SUCCESSFUL;
				X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
				ret = 1;
				break;
			}
			pci_write_config_longword(dev, X86_DI, X86_ECX);
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;

		default:
			dbg("%s: PCI_BIOS FUNC_NOT_SUPPORTED\r\n", __FUNCTION__);
			X86_AH = FUNC_NOT_SUPPORTED;
			X86_EFLAGS |= FB_CF; 
			break;
	}

	return ret;
}
