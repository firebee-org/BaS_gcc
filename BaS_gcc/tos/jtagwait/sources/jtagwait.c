#include <stdio.h>
#include <mint/osbind.h>
#include <stdint.h>
#include <stdbool.h>

#include "driver_vec.h"

#define FPGA_JTAG_LOADED_FLAG               ((volatile int32_t *) 0xFF101000)

#define _MBAR                               ((volatile uint8_t *) 0xFF000000)
#define MCF_GPIO_PDDR_FEC1L                 ((volatile uint8_t *)(&_MBAR[0xA17]))
#define MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L4     (0x10)
#define MCF_GPIO_PPDSDR_FEC1L               ((volatile uint8_t *)(&_MBAR[0xA27]))
#define FPGA_CONFIG                     	(1 << 2)
#define FPGA_CONF_DONE                      (1 << 5)

static inline uint32_t set_ipl(uint32_t ipl)
{
    uint32_t ret;

    __asm__ __volatile__(
        "		move.w	sr,%[ret]\r\n"			/* retrieve status register */
        "		andi.l	#0x07,%[ipl]\n\t"		/* mask out ipl bits on new value */
        "		lsl.l	#8,%[ipl]\n\t"			/* shift them to position */
        "		move.l	%[ret],d0\n\t"			/* retrieve original value */
        "		andi.l	#0x0000f8ff,d0\n\t"		/* clear ipl part */
        "		or.l	%[ipl],d0\n\t"			/* or in new value */
        "		move.w	d0,sr\n\t"				/* put it in place */
        "		andi.l	#0x0700,%[ret]\r\n"		/* mask out ipl bits */
        "		lsr.l	#8,%[ret]\r\n"			/* shift them to position */
        : [ret] "=&d" (ret)		/* output */
        : [ipl] "d" (ipl)		/* input */
        : "d0"	/* clobber */
    );

    return ret;
}

void wait_for_jtag(void)
{
    set_ipl(7);                     /* disable interrupts */
    /*
     * configure FEC1L port directions to enable external JTAG configuration download to FPGA
     */
    *MCF_GPIO_PDDR_FEC1L = 0 |
                          MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L4;	/* bit 4 = LED => output */
                                                            /* all other bits = input */
    /*
     * now that this GPIO ports have been switched to input, we can poll for FPGA config
     * started from the JTAG interface (CONFIGn goes high) and finish (CONF_DONE goes high)
     */
    while (*MCF_GPIO_PPDSDR_FEC1L & FPGA_CONFIG);           /* wait for JTAG reset */
    while (!(*MCF_GPIO_PPDSDR_FEC1L & FPGA_CONFIG));        /* wait for JTAG config load starting */
    while (!(*MCF_GPIO_PPDSDR_FEC1L & FPGA_CONF_DONE));     /* wait for JTAG config load finished */

    *FPGA_JTAG_LOADED_FLAG = 1;     /* indicate jtag loaded FPGA config to BaS */

    __asm__ __volatile__(
        "jmp    0xE0000000\n\t"
    );
}

int main(int argc, char *argv[])
{
    printf("\033E\r\nFPGA JTAG configuration support\r\n");
    printf("You may now savely load a new FPGA configuration through the JTAG interface\r\n"
           "and your Firebee will reboot once finished using that new configuration.\r\n");
    Supexec(wait_for_jtag);

    return 0;       /* just to make the compiler happy, we will never return */
}

