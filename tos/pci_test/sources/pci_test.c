#include <stdio.h>
#include <mint/osbind.h>
#include <stdint.h>
#include <stdbool.h>

#include "bas_printf.h"
#include "MCF5475.h"
#include "driver_vec.h"

#define NOP() __asm__ __volatile__("nop\n\t" : : : "memory")

#define SYSCLK 132000

volatile int32_t time, start, end;
int i;

void do_tests(struct pci_native_driver_interface *pci)
{
#define PCI_READ_CONFIG_LONGWORD(a, b)  pci->pci_read_config_longword(a, b)
#define PCI_WRITE_CONFIG_LONGWORD(a, b) pci->pci_write_config_longword(a, b)

    start = MCF_SLT0_SCNT;
    hexdump((uint8_t *) 0, 64);
    end = MCF_SLT0_SCNT;
    time = (start - end) / (SYSCLK / 1000) / 1000;

    xprintf("finished (took %f seconds).\r\n", time / 1000.0);
}

struct driver_table *get_bas_drivers(void)
{
    struct driver_table *ret = NULL;

    __asm__ __volatile__(
        "           bra.s   do_trap             \n\t"
        "           .dc.l   0x5f424153          \n\t"   // '_BAS'
        "do_trap:   trap    #0                  \n\t"
        "           move.l  d0,%[ret]           \n\t"
        :   [ret] "=m" (ret)    /* output */
        :                       /* no inputs */
        :                       /* clobbered */
    );

    return ret;
}

void pci_test(void)
{
    struct driver_table *bas_drivers;
    struct generic_interface *ifc;
    bool pci_driver_found = false;
    struct pci_native_driver_interface *pci_driver = NULL;

    bas_drivers = get_bas_drivers();
    printf("BaS version: %ld.%02ld\r\n", (long) bas_drivers->bas_version, (long) bas_drivers->bas_revision);

    ifc = bas_drivers->interfaces;

    do {
        struct generic_interface *pci_driver_interface = NULL;

        printf("interface type: %ld\r\n", (long) ifc[i].type);
        printf("interface version: %ld.%02ld\r\n", (long) ifc[i].version, (long) ifc[i].revision);
        printf("interface name: %s\r\n", ifc[i].name);
        printf("interface description: %s\r\n", ifc[i].description);

        if (ifc[i].type == PCI_NATIVE_DRIVER)
        {
            pci_driver_found = true;

            if (!pci_driver_interface || (ifc[i].version > pci_driver_interface->version ||
                                          (ifc[i].version == pci_driver_interface->version && ifc[i].revision > pci_driver_interface->revision)))
            {
                /*
                 * either no PCI driver interface found yet or with lower version or with lower version and higher revision
                 *
                 * replace it
                 */
                pci_driver = ifc[i].interface.pci_native;
                pci_driver_interface = &ifc[i];
                printf("PCI native driver interface v%d.%02d found\r\n", pci_driver_interface->version, pci_driver_interface->revision);
            }
        }
    } while (ifc[++i].type != END_OF_DRIVERS);

    if (pci_driver_found)
    {
        do_tests(pci_driver);
    }
}

int main(int argc, char *argv[])
{
    printf("PCI test routines\r\n");
    printf("\xbd 2014 M. Fr\x94schle\r\n");

    Supexec(pci_test);

    return 0;       /* just to make the compiler happy, we will never return */
}

