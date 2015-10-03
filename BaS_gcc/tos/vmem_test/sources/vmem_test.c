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
extern volatile uint32_t _VRAM[];

volatile int32_t time, start, end;
int i;

static void wait_pll(void)
{
    int32_t trgt = MCF_SLT0_SCNT - 100000;
    do
    {
        ;
    } while ((* (volatile int16_t *) 0xf0000800 < 0) && MCF_SLT0_SCNT > trgt);
}

static volatile uint8_t *pll_base = (volatile uint8_t *) 0xf0000600;

static void init_pll(void)
{
    xprintf("FPGA PLL initialization: ");

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x48) = 27;     /* loopfilter  r */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x08) = 1;      /* charge pump 1 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x00) = 12;     /* N counter high = 12 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x40) = 12;     /* N counter low = 12 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x114) = 1;     /* ck1 bypass */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x118) = 1;     /* ck2 bypass */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x11c) = 1;     /* ck3 bypass */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x10) = 1;      /* ck0 high  = 1 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x50) = 1;      /* ck0 low = 1 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x144) = 1;     /* M odd division */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x44) = 1;      /* M low = 1 */

    wait_pll();
    * (volatile uint16_t *) (pll_base + 0x04) = 145;    /* M high = 145 = 146 MHz */

    wait_pll();

    * (volatile uint8_t *) 0xf0000800 = 0;              /* set */

    xprintf("finished\r\n");
}

/*
 * INIT VIDEO DDR RAM
 */
static void init_video_ddr(void) {
    xprintf("init video RAM: ");

    * (volatile uint16_t *) 0xf0000400 = 0xb;   /* set cke = 1, cs=1, config = 1 */
    NOP();

    _VRAM[0] = 0x00050400; /* IPALL */
    NOP();

    _VRAM[0] = 0x00072000; /* load EMR pll on */
    NOP();

    _VRAM[0] = 0x00070122; /* load MR: reset pll, cl=2, burst=4lw */
    NOP();

    _VRAM[0] = 0x00050400; /* IPALL */
    NOP();

    _VRAM[0] = 0x00060000; /* auto refresh */
    NOP();

    _VRAM[0] = 0x00060000; /* auto refresh */
    NOP();

    /* FIXME: what's this? */
    _VRAM[0] = 0000070022; /* load MR dll on */
    NOP();

    * (uint32_t *) 0xf0000400 = 0x01070002; /* fifo on, refresh on, ddrcs und cke on, video dac on */

    xprintf("finished\r\n");
}

void do_tests(void)
{
    uint32_t version;

    xprintf("try to read Configware version (only works on later configs)\r\n");
    version = * (uint32_t *) 0xffffffff;

    xprintf("version = 0x%lx\r\n", version);

    xprintf("try to access Firebee FPGA memory\r\n");

    xprintf("read\r\n");
    start = MCF_SLT0_SCNT;
    hexdump((uint8_t *) _VRAM, 64);
    end = MCF_SLT0_SCNT;
    time = (start - end) / (SYSCLK / 1000) / 1000;

    xprintf("finished (took %f seconds).\r\n", time / 1000.0);

    xprintf("write\r\n");
    start = MCF_SLT0_SCNT;
    for (i = 0; i < 64; i++)
    {
       ((uint8_t *) _VRAM)[i] = (uint32_t) i;
    }
    end = MCF_SLT0_SCNT;
    time = (start - end) / (SYSCLK / 1000) / 1000;

    xprintf("finished (took %f seconds).\r\n", time / 1000.0);

    xprintf("read\r\n");
    start = MCF_SLT0_SCNT;
    hexdump((uint8_t *) _VRAM, 64);
    end = MCF_SLT0_SCNT;
    time = (start - end) / (SYSCLK / 1000) / 1000;

    xprintf("finished (took %f seconds).\r\n", time / 1000.0);
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

    xprintf("init FPGA PLLs\r\n");
    init_pll();

    xprintf("init video DDR RAM\r\n");
    init_video_ddr();

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
    printf("\033E\r\nFPGA JTAG configuration support\r\n");
    printf("test FPGA DDR RAM controller\r\n");
    printf("<C> 2014 M. F\f6schle\r\n");

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

