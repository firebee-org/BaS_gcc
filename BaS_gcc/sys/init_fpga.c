/*
 * init_fpga.c
 *
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 - 2012 F. Aschwanden
 * Copyright 2011 - 2012 V. Riviere
 * Copyright 2012        M. Froeschle
 *
 */

#include "MCF5475.h"
#include "sysinit.h"
#include "bas_printf.h"
#include "wait.h"

// #define FPGA_DEBUG
#if defined(FPGA_DEBUG)
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif

#define FPGA_STATUS     (1 << 0)
#define FPGA_CLOCK      (1 << 1)
#define FPGA_CONFIG     (1 << 2)
#define FPGA_DATA0      (1 << 3)
#define FPGA_CONF_DONE  (1 << 5)

extern uint8_t _FPGA_CONFIG[];
#define FPGA_FLASH_DATA &_FPGA_CONFIG[0]
extern uint8_t _FPGA_CONFIG_SIZE[];
#define FPGA_FLASH_DATA_SIZE    ((uint32_t) &_FPGA_CONFIG_SIZE[0])

/*
 * flag located in processor SRAM1 that indicates that the FPGA configuration has
 * been loaded through the onboard JTAG interface.
 * init_fpga() will honour this and not overwrite config.
 */
extern uint32_t _FPGA_JTAG_LOADED;
extern uint32_t _FPGA_JTAG_VALID;
#define VALID_JTAG 0xaffeaffe

void config_gpio_for_fpga_config(void)
{
#if defined(MACHINE_FIREBEE)
    /*
     * Configure GPIO FEC1L port directions (needed to load FPGA configuration)
     */
    MCF_GPIO_PDDR_FEC1L = 0 |                   /* bit 7 = input */
            0   |                               /* bit 6 = input */
            0 |                                 /* bit 5 = input */
            MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L4 |   /* bit 4 = LED => output */
            MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L3 |   /* bit 3 = PRG_DQ0 => output */
            MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L2 |   /* bit 2 = FPGA_CONFIG => output */
            MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L1 |   /* bit 1 = PRG_CLK (FPGA) => output */
            0;                                  /* bit 0 => input */
#endif /* MACHINE_FIREBEE */
}

void config_gpio_for_jtag_config(void)
{
    /*
     * configure FEC1L port directions to enable external JTAG configuration download to FPGA
     */
    MCF_GPIO_PDDR_FEC1L = 0 |
                          MCF_GPIO_PDDR_FEC1L_PDDR_FEC1L4;  /* bit 4 = LED => output */
                                                            /* all other bits = input */
    /*
     * unfortunately, the GPIO module cannot trigger interrupts. That means CONF_DONE needs to be polled to detect
     * external FPGA (re)configuration and reset the system in that case. Could be done from the OS as well...
     */
}

/*
 * load FPGA
 */
bool init_fpga(void)
{
    uint8_t *fpga_data;
    volatile int32_t time, start, end;
    int i;

    xprintf("FPGA load config...\r\n");
    if (_FPGA_JTAG_LOADED == 1 && _FPGA_JTAG_VALID == VALID_JTAG)
    {
        xprintf("detected _FPGA_JTAG_LOADED flag. FPGA config skipped.\r\n");

        /* reset the flag so that next boot will load config again from flash */
        _FPGA_JTAG_LOADED = 0;
        _FPGA_JTAG_VALID = 0;

        return true;
    }
    start = MCF_SLT0_SCNT;

    config_gpio_for_fpga_config();
    MCF_GPIO_PODR_FEC1L &= ~FPGA_CLOCK;                 /* FPGA clock => low */

    /* pulling FPGA_CONFIG to low resets the FPGA */
    MCF_GPIO_PODR_FEC1L &= ~FPGA_CONFIG;                /* FPGA config => low */
    wait(10);                                           /* give it some time to do its reset stuff */

    while ((MCF_GPIO_PPDSDR_FEC1L & FPGA_STATUS) && (MCF_GPIO_PPDSDR_FEC1L & FPGA_CONF_DONE));

    MCF_GPIO_PODR_FEC1L |= FPGA_CONFIG;                 /* pull FPGA_CONFIG high to start config cycle */
    while (!(MCF_GPIO_PPDSDR_FEC1L & FPGA_STATUS))
        ;                                               /* wait until status becomes high */

    /*
     * excerpt from an Altera configuration manual:
     *
     * The low-to-high transition of nCONFIG on the FPGA begins the configuration cycle. The
     * configuration cycle consists of 3 stages�reset, configuration, and initialization.
     * While nCONFIG is low, the device is in reset. When the device comes out of reset,
     * nCONFIG must be at a logic high level in order for the device to release the open-drain
     * nSTATUS pin. After nSTATUS is released, it is pulled high by a pull-up resistor and the FPGA
     * is ready to receive configuration data. Before and during configuration, all user I/O pins
     * are tri-stated. Stratix series, Arria series, and Cyclone series have weak pull-up resistors
     * on the I/O pins which are on, before and during configuration.
     *
     * To begin configuration, nCONFIG and nSTATUS must be at a logic high level. You can delay
     * configuration by holding the nCONFIG low. The device receives configuration data on its
     * DATA0 pins. Configuration data is latched into the FPGA on the rising edge of DCLK. After
     * the FPGA has received all configuration data successfully, it releases the CONF_DONE pin,
     * which is pulled high by a pull-up resistor. A low to high transition on CONF_DONE indicates
     * configuration is complete and initialization of the device can begin.
     */

    const uint8_t *fpga_flash_data_end = FPGA_FLASH_DATA + FPGA_FLASH_DATA_SIZE;

    fpga_data = (uint8_t *) FPGA_FLASH_DATA;
    do
    {
        uint8_t value = *fpga_data++;
        for (i = 0; i < 8; i++, value >>= 1)
        {

            if (value & 1)
            {
                /* bit set -> toggle DATA0 to high */
                MCF_GPIO_PODR_FEC1L |= FPGA_DATA0;
            }
            else
            {
                /* bit is cleared -> toggle DATA0 to low */
                MCF_GPIO_PODR_FEC1L &= ~FPGA_DATA0;
            }
            /* toggle DCLK -> FPGA reads the bit */
            MCF_GPIO_PODR_FEC1L |= FPGA_CLOCK;
            MCF_GPIO_PODR_FEC1L &= ~FPGA_CLOCK;
        }
    } while ((!(MCF_GPIO_PPDSDR_FEC1L & FPGA_CONF_DONE)) && (fpga_data < fpga_flash_data_end));

    if (fpga_data < fpga_flash_data_end)
    {
#ifdef _NOT_USED_
        while (fpga_data++ < fpga_flash_data_end)
        {
            /* toggle a little more since it's fun ;) */
            MCF_GPIO_PODR_FEC1L |= FPGA_CLOCK;
            MCF_GPIO_PODR_FEC1L &= ~FPGA_CLOCK;
        }
#endif /* _NOT_USED_ */
        end = MCF_SLT0_SCNT;
        time = (start - end) / (SYSCLK / 1000) / 1000;

        xprintf("finished (took %f seconds).\r\n", time / 1000.0);
        config_gpio_for_jtag_config();
        return true;
    }
    xprintf("FAILED!\r\n");
    config_gpio_for_jtag_config();

    return false;
}
