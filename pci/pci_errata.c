#include "pci_errata.h"
#include "pci.h"
#include <MCF5475.h>

#include "debug.h"

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
        "       .extern __MBAR          \n\t"
        "       clr.l   d0              \n\t"
        "       move.l  d0,__MBAR+0xF0C \n\t"       /* Must use direct addressing. write to EPORT module */
                                                    /* xlbus -> slavebus -> eport, writing '0' to register */
                                                    /* has no effect */
        "       rts                     \n\t"
        "       tpf.l   #0x0            \n\t"
        "       tpf.l   #0x0            \n\t"
        "       tpf.l   #0x0            \n\t"
        "       tpf.l   #0x0            \n\t"
        "       tpf.l   #0x0            \n\t"
        ::: "d0", "memory");
}

void chip_errata_055(int32_t handle)
{
    uint32_t dummy;

    return;     /* test */

    /* initiate PCI configuration access to device */
    MCF_PCI_PCICAR = MCF_PCI_PCICAR_E |         /* enable configuration access special cycle */
            MCF_PCI_PCICAR_BUSNUM(3) |          /* note: invalid bus number */
            MCF_PCI_PCICAR_DEVNUM(PCI_DEVICE_FROM_HANDLE(handle)) | /* device number, devices 0 - 9 are reserved */
            MCF_PCI_PCICAR_FUNCNUM(PCI_FUNCTION_FROM_HANDLE(handle)) |  /* function number */
            MCF_PCI_PCICAR_DWORD(0);

    /* issue a dummy read to an unsupported bus number (will fail) */
    dummy =  * (volatile uint32_t *) PCI_IO_OFFSET; /* access device */

    /* silently clear the PCI errors we produced just now */
    MCF_PCI_PCIISR = 0xffffffff;     /* clear all errors */
    MCF_PCI_PCIGSCR = MCF_PCI_PCIGSCR_PE | MCF_PCI_PCIGSCR_SE;

    (void) dummy;
}

