#include <stdio.h>
#include <string.h>
#include <mint/osbind.h>
#include <stdint.h>
#include <stdbool.h>

#include "MCF5475.h"
#include "driver_vec.h"
#include "pci.h"

#define NOP() __asm__ __volatile__("nop\n\t" : : : "memory")

#define SYSCLK 132000


int i;

void add_mem(struct pci_native_driver_interface *pci)
{
    int32_t handle;
    int16_t index = 0;

#define PCI_READ_CONFIG_LONGWORD(a, b)      (*pci->pci_read_config_longword)(a, b)
#define PCI_WRITE_CONFIG_LONGWORD(a, b)     (*pci->pci_write_config_longword)(a, b)
#define PCI_FIND_DEVICE(a, b, c)           (*pci->pci_find_device)(a, b, c)
#define PCI_GET_RESOURCE(a)                 (*pci->pci_get_resource)(a)

    printf("find ATI graphics card\r\n");

    handle = PCI_FIND_DEVICE(0x5159, 0x1002, index);
    if (handle > 0)
    {
        struct pci_rd *rd;

        printf("ATI device with handle 0x%02x found\r\n", handle);
        rd = PCI_GET_RESOURCE(handle);     /* get resource descriptor for ATI graphics card */
        printf("rd=%p\r\n", rd);
        printf("start=%lx, length=%lx, flags=%x\r\n", rd->start, rd->length, rd->flags);

        if (rd != NULL)
        {
            if (rd->start != 0L && rd->length > 0x04000000 &&
                    !(rd->flags & FLG_IO) && !(rd->flags & FLG_ROM))
            {
                int32_t err;

                if ((err = Maddalt(0x04000000 /* rd->start + 0x7C000000UL */, rd->length)) > 0)
                {
                    printf("%ld MBytes of additional memory added to system at %p\r\n",
                           rd->length / 1024 / 1024, 0x04000000 /* rd->start + 0x7C000000 */);
                    return;
                }
                else
                {
                    fprintf(stderr, "Maddalt() returned error code %ld\r\n", err);
                    exit(1);
                }
            }
        }
        else
        {
            fprintf(stderr, "no resource descriptor for handle 0x%02x found", handle);
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "no ATI graphics card found\r\n");
        exit(1);
    }

}

/*
 * temporarily replace the trap 0 handler with this so we can avoid
 * getting caught by BaS versions that don't understand the driver interface
 * exposure call.
 * If we get here, we have a BaS version that doesn't support the trap 0 interface
 */
static void __attribute__((interrupt)) trap0_catcher(void)
{
    __asm__ __volatile__(
                "       clr.l       d0              \n\t"       // return 0 to indicate not supported
                :
                :
                :
                );
}

struct driver_table *get_bas_drivers(void)
{
    struct driver_table *ret = NULL;
    void *old_vector;

    old_vector = Setexc(0x20, trap0_catcher);               /* set our own temporarily */

    __asm__ __volatile__(
                "           bra.s   do_trap             \n\t"
                "           .dc.l   0x5f424153          \n\t"   // '_BAS'
                "do_trap:   trap    #0                  \n\t"
                "           move.l  d0,%[ret]           \n\t"
                :   [ret] "=m" (ret)    /* output */
                :                       /* no inputs */
                :                       /* clobbered */
                );
    (void) Setexc(0x20, old_vector);                        /* restore original vector */

    return ret;
}

void pci_mem(void)
{
    struct driver_table *bas_drivers;
    struct generic_interface *ifc;
    bool pci_driver_found = false;
    struct pci_native_driver_interface *pci_driver = NULL;

    bas_drivers = get_bas_drivers();

    /*
     * trap0_catcher should return 0L on failure, for some reason FireTOS
     * returns -1L on a trap #0. Anyway, ...
     */
    if (bas_drivers != NULL && bas_drivers != (void *) -1L)
    {
        printf("BaS driver vector: %p\r\n", bas_drivers);
        printf("BaS version: %ld.%02ld\r\n", (long) bas_drivers->bas_version, (long) bas_drivers->bas_revision);
    }
    else
    {
        printf("BaS driver retrieval not supported\r\n");
        printf("(old BaS version or FireTOS?)\r\n");
        exit(1);
    }

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
                printf("replaced old with newer driver version\r\n");
            }
        }
    } while (ifc[++i].type != END_OF_DRIVERS);

    if (pci_driver_found)
    {
        add_mem(pci_driver);
    }
}

int main(int argc, char *argv[])
{
    printf("PCI Memory Adder\r\n");
    printf("\xbd 2016 M. Fr\x94schle\r\n");

    Supexec(pci_mem);

    return 0;       /* just to make the compiler happy, we will never return */
}

