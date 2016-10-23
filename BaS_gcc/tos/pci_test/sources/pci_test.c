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

#define KB  1024UL
#define MB  (KB * KB)

volatile int32_t time, start, end;
int i;

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
    return "unknown device class";
}

void hexdump(uint8_t buffer[], int size)
{
   int i;
   int line = 0;
   uint8_t *bp = buffer;

   while (bp < buffer + size) {
      uint8_t *lbp = bp;

      printf("%08lx  ", (long) buffer + line);

      for (i = 0; i < 16; i++) {
         if (bp + i > buffer + size) {
            break;
         }
         printf("%02x ", (uint8_t) *lbp++);
      }

      lbp = bp;
      for (i = 0; i < 16; i++) {
         int8_t c = *lbp++;

         if (bp + i > buffer + size) {
            break;
         }
         if (c > ' ' && c < '~') {
            printf("%c", c);
         } else {
            printf(".");
         }
      }
      printf("\r\n");

      bp += 16;
      line += 16;
   }
}

void do_tests(struct pci_native_driver_interface *pci)
{
#define PCI_READ_CONFIG_LONGWORD(a, b)  pci->pci_read_config_longword(a, b)
#define PCI_WRITE_CONFIG_LONGWORD(a, b) pci->pci_write_config_longword(a, b)


    printf("enumerate PCI devices\r\n");

    int16_t handle;
    int16_t index = 0;

    printf("\r\nPCI bus scan...\r\n\r\n");
    printf(" Bus| Dev|Func|Vndr|D-ID|Hndl|\r\n");
    printf("----+----+----+----+----+----+\r\n");

    handle = (*pci->pci_find_device)(0x0, 0xFFFF, index);

    while (handle > 0)
    {
        uint32_t value;

        value = swpl((*pci->pci_read_config_longword)(handle, PCIIDR));

        printf(" %02x | %02x | %02x |%04x|%04x|%04x| %s (0x%02x, 0x%04x)\r\n",
               PCI_BUS_FROM_HANDLE(handle),
               PCI_DEVICE_FROM_HANDLE(handle),
               PCI_FUNCTION_FROM_HANDLE(handle),
               PCI_VENDOR_ID(value), PCI_DEVICE_ID(value),
               handle,
               device_class((*pci->pci_read_config_byte)(handle, PCI_LANESWAP_B(PCICCR))),
               (*pci->pci_read_config_byte)(handle, PCI_LANESWAP_B(PCICCR)),
               (*pci->pci_read_config_word)(handle, PCI_LANESWAP_W(PCICCR)));

        handle = (*pci->pci_find_device)(0x0, 0xFFFF, ++index);
    }

    struct pci_rd *rd;
    int flags;

    /*
     * look for an ATI Radeon video card
     */

    // handle = 0xd8;
    handle = (*pci->pci_find_device)(0x5159, 0x1002, 0);
    if (handle > 0)
    {
        rd = (*pci->pci_get_resource)(handle);     /* get resource descriptor for ATI graphics card */
        if (rd != NULL)
        {
            do
            {
                flags = rd->flags;

                printf("Start address: 0x%08lx\r\n", rd->start);
                printf("Length:        0x%08lx\r\n", rd->length);
                printf("Offset:        0x%08lx\r\n", rd->offset);
                printf("DMA offset:    0x%08lx\r\n", rd->dmaoffset);
                printf("FLAGS: %s%s%s%s%s%s%s\r\n",
                       flags & FLG_IO ? "FLG_IO, " : "",
                       flags & FLG_ROM ? "FLG_ROM, " : "",
                       flags & FLG_8BIT ? "FLG_8BIT, " : "",
                       flags & FLG_16BIT ? "FLG_16BIT, " : "",
                       flags & FLG_32BIT ? "FLG_32BIT, " : "",
                       (flags & FLG_ENDMASK) == ORD_MOTOROLA ? "ORD_MOTOROLA" :
                                                               (flags & FLG_ENDMASK) == ORD_INTEL_AS ? "ORD_INTEL_AS" :
                                                                                                       (flags & FLG_ENDMASK) == ORD_INTEL_LS ? "ORD_INTEL_LS" :
                                                                                                                                               (flags & FLG_ENDMASK) == ORD_UNKNOWN ? "ORD_UNKNOWN" :
                                                                                                                                                                                      "", "");
                printf("\r\n");

                if (rd->start != 0L && rd->start == 0x80000000)
                {
                    hexdump((uint8_t *) rd->start + rd->offset, 64);

                    memset((uint8_t *) rd-> start + rd->offset, 0, 64 * MB);

                    printf("memory cleared\r\n");
                    hexdump((uint8_t *) rd->start + rd->offset, 64);

                    memset((uint8_t *) rd->start + rd->offset, 0xaa, 64 * MB);
                    hexdump((uint8_t *) rd->start + rd->offset, 64);
                }

                rd = (struct pci_rd *) (((uintptr_t) rd) + (uintptr_t) rd->next);
            } while (!(flags & FLG_LAST));
        }
        else
        {
            printf("resource descriptor for handle 0x%02x not found\r\n", handle);
        }
    }
    else
        fprintf(stderr, "card not found\r\n");

    printf("\r\n...finished\r\n");
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

void pci_test(void)
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

