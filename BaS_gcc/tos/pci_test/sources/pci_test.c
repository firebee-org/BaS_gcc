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

void do_tests(void)
{
    start = MCF_SLT0_SCNT;
    hexdump((uint8_t *) 0, 64);
    end = MCF_SLT0_SCNT;
    time = (start - end) / (SYSCLK / 1000) / 1000;

    xprintf("finished (took %f seconds).\r\n", time / 1000.0);
}



void wait_for_jtag(void)
{
    long i;

    do_tests();

    xprintf("wait a little to let things settle...\r\n");
    for (i = 0; i < 100000L; i++);

    xprintf("INFO: endless loop now. Press reset to reboot\r\n");
    while (1)
        ;
}

int main(int argc, char *argv[])
{
    printf("FPGA JTAG configuration support\r\n");
    printf("test FPGA DDR RAM controller\r\n");
    printf("\xbd 2014 M. F\x94schle\r\n");

    printf("You may now savely load a new FPGA configuration through the JTAG interface\r\n"
           "and your Firebee will reboot once finished using that new configuration.\r\n");
    if (argc == 2)
    {
        /*
         * we got an argument. This is supposed to be the address that we need to jump to after JTAG
         * configuration has been finished. Meant to support BaS in RAM testing
         */
        char *addr_str = argv[1];
        char *addr = NULL;
        char *end = NULL;

        addr = (char *) strtol(addr_str, &end, 16);
        if (addr != NULL && addr <= (char *) 0xe0000000 && addr >= (char *) 0x10000000)
        {
            /*
             * seems to be a valid address
             */
            // bas_start = (long) addr;

            // printf("BaS start address set to %p\r\n", (void *) bas_start);
        }
        else
        {
            // printf("\r\nNote: BaS start address %p not valid. Stick to %p.\r\n", addr, (void *) bas_start);
        }
    }
    Supexec(wait_for_jtag);

    return 0;       /* just to make the compiler happy, we will never return */
}

