#include "radeonfb.h"
#include "pci.h"
#include "x86emu.h"
#include "x86pcibios.h"

extern unsigned short offset_port;

int x86_pcibios_emulator()
{
	int ret = 0;
	unsigned long dev;

	switch (X86_AX) {
	case PCI_BIOS_PRESENT:
#ifdef DEBUG_X86EMU_PCI
		DPRINT("PCI_BIOS_PRESENT\r\n");
#endif
		X86_AH	= 0x00;		/* no config space/special cycle support */
		X86_AL	= 0x01;		/* config mechanism 1 */
		X86_EDX = 'P' | 'C' << 8 | 'I' << 16 | ' ' << 24;
		X86_EBX = 0x0210;	/* Version 2.10 */
		X86_ECX = 0xFF00;	/* FixME: Max bus number */
		X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
		ret = 1;
		break;
	case FIND_PCI_DEVICE:
		/* FixME: support SI != 0 */
		// vendor, device
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX("FIND_PCI_DEVICE vendor ", X86_DX);
		DPRINTVALHEX(" device ", X86_CX);		
#endif
		dev = pci_find_device((unsigned long) X86_DX, ((unsigned long)X86_CX), 0);

		if (dev != 0) {
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... OK\r\n");		
#endif
			X86_BH = PCI_BUS_FROM_HANDLE(dev);
			//X86_BH = (char)(dev >> 16) / PCI_MAX_FUNCTION); // dev->bus->secondary;
			X86_BL = PCI_DEVICE_FROM_HANDLE(dev) << 3 | PCI_FUNCTION_FROM_HANDLE(dev);
			//X86_BL = (char)dev; // dev->path.u.pci.devfn;
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
		} else {
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... error\r\n");		
#endif
			X86_AH = DEVICE_NOT_FOUND;
			X86_EFLAGS |= FB_CF;	/* set carry flag */
			ret = 0;
		}
		break;
	case FIND_PCI_CLASS_CODE:
		/* FixME: support SI != 0 */
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX("FIND_PCI_CLASS_CODE ", X86_ECX);
#endif
		dev = pci_find_classcode(X86_ECX, 0);
		if (dev != 0) {
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... OK\r\n");		
#endif
			X86_BH = PCI_BUS_FROM_HANDLE(dev);
			X86_BL = PCI_DEVICE_FROM_HANDLE(dev) << 3 | PCI_FUNCTION_FROM_HANDLE(dev);
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
		} else {
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... error\r\n");		
#endif
			X86_AH = DEVICE_NOT_FOUND;
			X86_EFLAGS |= FB_CF;	/* set carry flag */
			ret = 0;
		}
		break;
	case READ_CONFIG_BYTE:
		// bus, devfn
#ifdef DEBUG_X86EMU_PCI
		DPRINTVAL("READ_CONFIG_BYTE bus ", X86_BH);
		DPRINTVAL(" devfn ", X86_BL);
		DPRINTVALHEX(" reg ", X86_DI);
#endif
		X86_CL = pci_read_config_byte(dev, X86_DI);
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX(" value ", X86_CL);
		DPRINT("\r\n");
#endif
		X86_AH = SUCCESSFUL;
		X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
		ret = 1;
		break;
	case READ_CONFIG_WORD:
		// bus, devfn
#ifdef DEBUG_X86EMU_PCI
		DPRINTVAL("READ_CONFIG_WORD bus ", X86_BH);
		DPRINTVAL(" devfn ", X86_BL);
		DPRINTVALHEX(" reg ", X86_DI);
#endif
		if(X86_DI == PCIBAR1)
			X86_CX = offset_port + 1;
		else
			X86_CX = pci_read_config_word(dev, X86_DI);
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX(" value ", X86_CX);
		DPRINT("\r\n");
#endif
		X86_AH = SUCCESSFUL;
		X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
		ret = 1;
		break;
	case READ_CONFIG_DWORD:
		// bus, devfn
#ifdef DEBUG_X86EMU_PCI
		DPRINTVAL("READ_CONFIG_DWORD bus ", X86_BH);
		DPRINTVAL(" devfn ", X86_BL);
		DPRINTVALHEX(" reg ", X86_DI);
#endif
		if(X86_DI == PCIBAR1)
			X86_CX = (unsigned long)offset_port+1;
		else
			X86_ECX = pci_read_config_longword(dev, X86_DI);
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX(" value ", X86_ECX);
		DPRINT("\r\n");
#endif
		X86_AH = SUCCESSFUL;
		X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
		ret = 1;
		break;
	case WRITE_CONFIG_BYTE:
		// bus, devfn
#ifdef DEBUG_X86EMU_PCI
		DPRINTVAL("READ_CONFIG_BYTE bus ", X86_BH);
		DPRINTVAL(" devfn ", X86_BL);
		DPRINTVALHEX(" reg ", X86_DI);
		DPRINTVALHEX(" value ", X86_CL);
#endif
		if ((ret = pci_write_config_byte(dev, X86_DI, X86_CL)) == 0)
		{
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... OK\r\n");
#endif
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
		} else {
#ifdef DEBUG_X86EMU_PCI
			DPRINTVAL(" ... error ", ret);
			DPRINT("\r\n");
#endif
			X86_AH = DEVICE_NOT_FOUND;
			X86_EFLAGS |= FB_CF;	/* set carry flag */	
			ret = 0;
		}
		break;
	case WRITE_CONFIG_WORD:
		// bus, devfn
#ifdef DEBUG_X86EMU_PCI
		DPRINTVAL("WRITE_CONFIG_WORD bus ", X86_BH);
		DPRINTVAL(" devfn ", X86_BL);
		DPRINTVALHEX(" reg ", X86_DI);
		DPRINTVALHEX(" value ", X86_CX);
#endif
		if(X86_DI == PCIBAR1) {
			offset_port = X86_CX;
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... OK\r\n");
#endif
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;
		}
		if ((ret = pci_write_config_word(dev, X86_DI, X86_CX)) == 0)
		{
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... OK\r\n");
#endif
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
		} else {
#ifdef DEBUG_X86EMU_PCI
			DPRINTVAL(" ... error ", ret);
			DPRINT("\r\n");
#endif
			X86_AH = DEVICE_NOT_FOUND;
			X86_EFLAGS |= FB_CF;	/* set carry flag */	
			ret = 0;
		}
		break;
	case WRITE_CONFIG_DWORD:
		// bus, devfn
#ifdef DEBUG_X86EMU_PCI
		DPRINTVAL("WRITE_CONFIG_DWORD bus ", X86_BH);
		DPRINTVAL(" devfn ", X86_BL);
		DPRINTVALHEX(" reg ", X86_DI);
		DPRINTVALHEX(" value ", X86_ECX);
#endif
		if(X86_DI == PCIBAR1) {
			offset_port = (unsigned short)X86_ECX & 0xFFFC;
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... OK\r\n");		
#endif
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
			break;
		}
		if ((ret = pci_write_config_longword(dev, X86_DI, X86_ECX)) == 0)
		{
#ifdef DEBUG_X86EMU_PCI
			DPRINT(" ... OK\r\n");		
#endif
			X86_AH = SUCCESSFUL;
			X86_EFLAGS &= ~FB_CF;	/* clear carry flag */
			ret = 1;
		}
		else
		{
#ifdef DEBUG_X86EMU_PCI
			DPRINTVAL(" ... error ", ret);
			DPRINT("\r\n");
#endif
			X86_AH = DEVICE_NOT_FOUND;
			X86_EFLAGS |= FB_CF;	/* set carry flag */	
			ret = 0;
		}
		break;
	default:
#ifdef DEBUG_X86EMU_PCI
		DPRINT("PCI_BIOS FUNC_NOT_SUPPORTED\r\n");
#endif
		X86_AH = FUNC_NOT_SUPPORTED;
		X86_EFLAGS |= FB_CF; 
		break;
	}

	return ret;
}
