#include <stdio.h>
#include <mint/osbind.h>
#include <stdint.h>
#include <stdbool.h>

#include "bas_printf.h"
#include "MCF5475.h"
#include "driver_vec.h"

#define FPGA_CONFIG                         (1 << 2)
#define FPGA_CONF_DONE                      (1 << 5)

#define SRAM1_START     0xff101000
#define SRAM1_END       SRAM1_START + 0x1000
#define SAFE_STACK      SRAM1_END - 4

#define NOP() __asm__ __volatile__("nop\n\t" : : : "memory")

#define SYSCLK 132000

long bas_start = 0xe0000000;

volatile uint16_t *FB_CS1 = (volatile uint16_t *) 0xfff00000; /* "classic" ATARI I/O registers */
volatile uint32_t *FB_CS2 = (volatile uint32_t *) 0xf0000000; /* FireBee 32 bit I/O registers */
volatile uint16_t *FB_CS3 = (volatile uint16_t *) 0xf8000000; /* FireBee SRAM */
volatile uint32_t *FB_CS4 = (volatile uint32_t *) 0x40000000; /* FireBee SD RAM */

bool verify_longaddr(volatile uint32_t * const addr, uint32_t value)
{
    *addr = value;

    if (value != *addr)
        return false;

    return true;
}

bool verify_long(volatile uint32_t * const addr, uint32_t low_value, uint32_t high_value)
{
    uint32_t i;

    for (i = low_value; i <= high_value; i++)
        if (verify_longaddr(addr, i) == false)
            return false;

    return true;
}

void firebee_io_test(void)
{
    volatile uint32_t *ACP_VCTR = &FB_CS2[0x100];            /* 0xf000400 */
    volatile uint32_t *CCR = &FB_CS2[0x101];                 /* 0xf000401 - FireBee mode border color */
    volatile uint32_t *ATARI_HH = &FB_CS2[0x104];            /* 0xf000410 */
    volatile uint32_t *ATARI_VH = &FB_CS2[0x105];            /* 0xf000414 */
    volatile uint32_t *ATARI_HL = &FB_CS2[0x106];            /* 0xf000418 */
    volatile uint32_t *ATARI_VL = &FB_CS2[0x107];            /* 0xf00041c */

    volatile uint32_t *VIDEO_PLL_CONFIG = &FB_CS2[0x180];    /* 0xf000600 */
    volatile uint32_t *VIDEO_PLL_RECONFIG = &FB_CS2[0x200];  /* 0xf000800 */

    verify_long(ACP_VCTR, 0, 0x7fffffff);
    verify_long(CCR, 0, 0x7fffffff);
    verify_long(ATARI_HH, 0, 0xffffffff);
    verify_long(ATARI_VH, 0, 0xffffffff);
    verify_long(ATARI_HL, 0, 0xffffffff);
    verify_long(ATARI_VL, 0, 0xffffffff);

    verify_long(VIDEO_PLL_CONFIG, 0, 0xffffffff);
    verify_long(VIDEO_PLL_RECONFIG, 0, 0xffffffff);
}

bool verify_wordaddr(volatile uint16_t * const addr, uint16_t value)
{
    *addr = value;

    if (value != *addr)
        return false;

    return true;
}

bool verify_word(volatile uint16_t * const addr, uint16_t low_value, uint16_t high_value)
{
    int16_t i;

    for (i = low_value; i <= high_value; i++)
        if (verify_wordaddr(addr, i) == false)
            return false;

    return true;
}

void atari_io_test(void)
{
    volatile uint16_t *SYS_CTR = &FB_CS1[0x7c003];           /* 0xffff8006 */
    volatile uint16_t *VDL_LOF = &FB_CS1[0x7c107];           /* 0xffff820e */
    volatile uint16_t *VDL_LWD = &FB_CS1[0x7c108];           /* 0xffff8210 */
    volatile uint16_t *VDL_HHT = &FB_CS1[0x7c141];           /* 0xffff8282 */
    volatile uint16_t *VDL_HBB = &FB_CS1[0x7c142];           /* 0xffff8284 */
    volatile uint16_t *VDL_HBE = &FB_CS1[0x7c143];           /* 0xffff8286 */
    volatile uint16_t *VDL_HDB = &FB_CS1[0x7c144];           /* 0xffff8288 */
    volatile uint16_t *VDL_HDE = &FB_CS1[0x7c145];           /* 0xffff828a */
    volatile uint16_t *VDL_HSS = &FB_CS1[0x7c146];           /* 0xffff828c */

    volatile uint16_t *VDL_VFT = &FB_CS1[0x7c151];           /* 0xffff82a2 */
    volatile uint16_t *VDL_VBB = &FB_CS1[0x7c152];           /* 0xffff82a4 */
    volatile uint16_t *VDL_VBE = &FB_CS1[0x7c153];           /* 0xffff82a6 */
    volatile uint16_t *VDL_VDB = &FB_CS1[0x7c154];           /* 0xffff82a8 */
    volatile uint16_t *VDL_VDE = &FB_CS1[0x7c155];           /* 0xffff82aa */
    volatile uint16_t *VDL_VSS = &FB_CS1[0x7c156];           /* 0xffff82ac */
    volatile uint16_t *VDL_VCT = &FB_CS1[0x7c160];           /* 0xffff82c0 */
    volatile uint16_t *VDL_VMD = &FB_CS1[0x7c161];           /* 0xffff82c2 */

    verify_word(SYS_CTR, 0, 0x7fff);
    verify_word(VDL_LOF, 0, 0x7fff);
    verify_word(VDL_LWD, 0, 0x7fff);
    verify_word(VDL_HHT, 0, 0x7fff);
    verify_word(VDL_HBB, 0, 0x7fff);
    verify_word(VDL_HBE, 0, 0x7fff);
    verify_word(VDL_HDB, 0, 0x7fff);
    verify_word(VDL_HDE, 0, 0x7fff);
    verify_word(VDL_HSS, 0, 0x7fff);

    verify_word(VDL_VFT, 0, 0x7fff);
    verify_word(VDL_VBB, 0, 0x7fff);
    verify_word(VDL_VBE, 0, 0x7fff);
    verify_word(VDL_VDB, 0, 0x7fff);
    verify_word(VDL_VDE, 0, 0x7fff);
    verify_word(VDL_VSS, 0, 0x7fff);
    verify_word(VDL_VCT, 0, 0x7fff);
    verify_word(VDL_VMD, 0, 0x7fff);
}

void do_tests(void)
{
    volatile unsigned long *t1 = (volatile unsigned long *) 0xf0000000;
    volatile unsigned short *t2 = (volatile unsigned short *) 0xf0000000;
    volatile unsigned short *t3 = (volatile unsigned short *) 0xf0000002;
    long value = 0;

    while (1)
    {
        *t1 = value;
        *t2 = (short) value;
        *t3 = (short) (value >> 16);
        xprintf("W: 0x%lx R: 0x%lx W: 0x%04x R: 0x%04x W: 0x%04x R: 0x%04x\r\n", value, *t1,
                 (unsigned short) value, *t2, (unsigned short)(value >> 16), *t3);
        value++;
    }
}


void wait_for_jtag(void)
{
    long i;

    /* set supervisor stack to end of SRAM1 */
    __asm__ __volatile__ (
        "       move    #0x2700,sr\n\t"         /* disable interrupts */
        "       move.l  %[stack],d0\n\t"        /* 4KB on-chip core SRAM1 */
        "       move.l  d0,sp\n\t"              /* set stack pointer */
        :
        : [stack] "i" (SAFE_STACK)
        : "d0", "cc"    /* clobber */
    );

    MCF_EPORT_EPIER = 0x0;              /* disable EPORT interrupts */
    MCF_INTC_IMRL = 0xffffffff;
    MCF_INTC_IMRH = 0xffffffff;         /* disable interrupt controller */

    MCF_MMU_MMUCR &= ~MCF_MMU_MMUCR_EN; /* disable MMU */

    xprintf("relocated supervisor stack, disabled interrupts and disabled MMU\r\n");

    /*
     * configure FEC1L port directions to enable external JTAG configuration download to FPGA
     */
    MCF_GPIO_PDDR_FEC1L = 0 |
                          MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L4;  /* bit 4 = LED => output */
                                                            /* all other bits = input */

    /*
     * configure DSPI_CS3 as GPIO input to avoid the MCU driving against the FPGA blink
     */
    MCF_PAD_PAR_DSPI &= ~MCF_PAD_PAR_DSPI_PAR_CS3(MCF_PAD_PAR_DSPI_PAR_CS3_DSPICS3);
    /*
     * now that GPIO ports have been switched to input, we can poll for FPGA config
     * started from the JTAG interface (CONF_DONE goes low) and finish (CONF_DONE goes high)
     */
    xprintf("waiting for JTAG configuration start\r\n");
    while ((MCF_GPIO_PPDSDR_FEC1L & FPGA_CONF_DONE));       /* wait for JTAG config load started */

    xprintf("waiting for JTAG configuration to finish\r\n");
    while (!(MCF_GPIO_PPDSDR_FEC1L & FPGA_CONF_DONE));     /* wait for JTAG config load finished */

    xprintf("JTAG configuration finished.\r\n");

    /* wait */
    xprintf("wait a little to let things settle...\r\n");
    for (i = 0; i < 100000L; i++);

    xprintf("disable caches\r\n");
    __asm__ __volatile(
                "move.l     #0x01000000,d0      \r\n"
                "movec      d0,CACR             \r\n"
                : /* no output */
                : /* no input */
                : "d0", "memory");

    /* begin of tests */

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
            bas_start = (long) addr;

            printf("BaS start address set to %p\r\n", (void *) bas_start);
        }
        else
        {
            printf("\r\nNote: BaS start address %p not valid. Stick to %p.\r\n", addr, (void *) bas_start);
        }
    }
    Supexec(wait_for_jtag);

    return 0;       /* just to make the compiler happy, we will never return */
}

