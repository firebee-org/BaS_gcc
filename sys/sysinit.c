/*
 * File:        sysinit.c
 * Purpose:     Power-on Reset configuration of the Firebee board.
 *
 * Notes:
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

#include <stdbool.h>
#include "MCF5475.h"
#include "startcf.h"
#include "cache.h"
#include "sysinit.h"
#include "pci.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "bas_types.h"
#include "wait.h"
#include "util.h"
#include "version.h"
#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error "unknown machine"
#endif /* MACHINE_M5484LITE */
#include "dma.h"
#include "mod_devicetable.h"
#include "pci_ids.h"
#include "driver_mem.h"
#include "usb.h"
#include "video.h"

// #define DEBUG
#include "debug.h"

#define UNUSED(x) (void)(x)             /* Unused variable         */

bool fpga_configured = false;           /* for FPGA JTAG configuration */

extern volatile long _VRAM; /* start address of video ram from linker script */

/*
 * init SLICE TIMER 0
 * all  = 32.538 sec = 30.736mHz
 * BYT0 = 127.1ms/tick = 7.876Hz   offset 0
 * BYT1 = 496.5us/tick = 2.014kHz  offset 1
 * BYT2 = 1.939us/tick = 515.6kHz  offset 2
 * BYT3 = 7.576ns/tick = 132.00MHz offset 3
 * count down!!! 132MHz!!!
 */
static void init_slt(void)
{
    xprintf("slice timer initialization: ");
    MCF_SLT0_STCNT = 0xffffffff;
    MCF_SLT0_SCR = MCF_SLT_SCR_TEN | MCF_SLT_SCR_RUN;   /* enable and run continuously */
    xprintf("finished\r\n");
}

/*
 * init GPIO general purpose I/O module
 */
static void init_gpio(void)
{
    /*
     * pad register P.S.:FBCTL and FBCS set correctly at reset
     */

    /*
     * configure all four 547x GPIO module DMA pins:
     *
     * /DACK1 - DMA acknowledge 1
     * /DACK0 - DMA acknowledge 0
     * /DREQ1 - DMA request 1
     * /DREQ0 - DMA request 0
     *
     * for DMA operation
     */
    MCF_PAD_PAR_DMA = MCF_PAD_PAR_DMA_PAR_DACK0(0x3) |
            MCF_PAD_PAR_DMA_PAR_DACK1(0x3) |
            MCF_PAD_PAR_DMA_PAR_DREQ1(0x3) |
            MCF_PAD_PAR_DMA_PAR_DREQ0(0x3);

    /*
     * configure FEC0 pin assignment on GPIO module as FEC0
     * configure FEC1 pin assignment (PAR_E17, PAR_E1MII) as GPIO,
     * /IRQ5 and /IRQ6  from GPIO (needs to be disabled on EPORT module, which also can
     * use those INTs).
     */
    MCF_PAD_PAR_FECI2CIRQ = MCF_PAD_PAR_FECI2CIRQ_PAR_E07 |
            MCF_PAD_PAR_FECI2CIRQ_PAR_E0MII |
            MCF_PAD_PAR_FECI2CIRQ_PAR_E0MDIO |
            MCF_PAD_PAR_FECI2CIRQ_PAR_E0MDC |
            MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDIO_E1MDIO |
            MCF_PAD_PAR_FECI2CIRQ_PAR_E1MDC_E1MDC |
            MCF_PAD_PAR_FECI2CIRQ_PAR_SDA |
            MCF_PAD_PAR_FECI2CIRQ_PAR_SCL |
            MCF_PAD_PAR_FECI2CIRQ_PAR_IRQ6 |
            MCF_PAD_PAR_FECI2CIRQ_PAR_IRQ5;

    /*
     * configure PCI Grant pin assignment on GPIO module:
     *
     * /PCIBG4 used as FlexBus /TBST
     * /PCIBG3 used as general purpose I/O
     * /PCIBG2 used as /PCIBG2
     * /PCIBG1 used as /PCIBG1
     * /PCIBG0 used as /PCIBG0
     */
#if defined(MACHINE_FIREBEE)
    MCF_PAD_PAR_PCIBG = MCF_PAD_PAR_PCIBG_PAR_PCIBG4_TBST |
            MCF_PAD_PAR_PCIBG_PAR_PCIBG3_GPIO |
            MCF_PAD_PAR_PCIBG_PAR_PCIBG2_PCIBG2 |
            MCF_PAD_PAR_PCIBG_PAR_PCIBG1_PCIBG1 |
            MCF_PAD_PAR_PCIBG_PAR_PCIBG0_PCIBG0;
#elif defined(MACHINE_M5484LITE)
    MCF_PAD_PAR_PCIBG = MCF_PAD_PAR_PCIBG_PAR_PCIBG4_PCIBG4 |
            MCF_PAD_PAR_PCIBG_PAR_PCIBG3_PCIBG3 |
            MCF_PAD_PAR_PCIBG_PAR_PCIBG2_PCIBG2 |
            MCF_PAD_PAR_PCIBG_PAR_PCIBG1_PCIBG1 |
            MCF_PAD_PAR_PCIBG_PAR_PCIBG0_PCIBG0;
#endif /* MACHINE_FIREBEE */

    /*
     * configure PCI request pin assignment on GPIO module:
     * /PCIBR4 as /IRQ4
     * /PCIBR3 as GPIO (PIC)
     * /PCIBR2 as /PCIBR2
     * /PCIBR1 as /PCIBR1
     * /PCIBR0 as /PCIBR0
     */
#if defined(MACHINE_FIREBEE)
    MCF_PAD_PAR_PCIBR = MCF_PAD_PAR_PCIBR_PAR_PCIBR4_IRQ4 |
            MCF_PAD_PAR_PCIBR_PAR_PCIBR3_GPIO |
            MCF_PAD_PAR_PCIBR_PAR_PCIBR2_PCIBR2 |
            MCF_PAD_PAR_PCIBR_PAR_PCIBR1_PCIBR1 |
            MCF_PAD_PAR_PCIBR_PAR_PCIBR0_PCIBR0;
#elif defined(MACHINE_M5484LITE)
    MCF_PAD_PAR_PCIBR = MCF_PAD_PAR_PCIBR_PAR_PCIBR4_PCIBR4 |
            MCF_PAD_PAR_PCIBR_PAR_PCIBR3_PCIBR3 |
            MCF_PAD_PAR_PCIBR_PAR_PCIBR2_PCIBR2 |
            MCF_PAD_PAR_PCIBR_PAR_PCIBR1_PCIBR1 |
            MCF_PAD_PAR_PCIBR_PAR_PCIBR0_PCIBR0;
#endif /* MACHINE_FIREBEE */

    /*
     * configure PSC3 pin assignment on GPIO module:
     * /PSC3CTS as /PSC3PTS
     * /PSC3RTS as /PSC3RTS
     * PSC3RXD as PSC3RXD
     * PSC3TXD as PSC3TXD
     */

    MCF_PAD_PAR_PSC3 = MCF_PAD_PAR_PSC3_PAR_TXD3 | MCF_PAD_PAR_PSC3_PAR_RXD3;

    /*
     * Configure PSC1 pin assignment on GPIO module:
     * - all pins configured for serial interface operation
     */

    MCF_PAD_PAR_PSC1 = MCF_PAD_PAR_PSC1_PAR_CTS1_CTS |
            MCF_PAD_PAR_PSC1_PAR_RTS1_RTS |
            MCF_PAD_PAR_PSC1_PAR_RXD1 |
            MCF_PAD_PAR_PSC1_PAR_TXD1;

    /*
     * Configure PSC0 Pin Assignment on GPIO module:
     * - all pins configured for serial interface operation
     */

    MCF_PAD_PAR_PSC0 = MCF_PAD_PAR_PSC0_PAR_CTS0_CTS |
            MCF_PAD_PAR_PSC0_PAR_RTS0_RTS |
            MCF_PAD_PAR_PSC0_PAR_RXD0 |
            MCF_PAD_PAR_PSC0_PAR_TXD0;

    /*
     * Configure all DSPI pins on the GPIO module for there primary function
     */
    MCF_PAD_PAR_DSPI = MCF_PAD_PAR_DSPI_PAR_SOUT(MCF_PAD_PAR_DSPI_PAR_SOUT_SOUT) |
                        MCF_PAD_PAR_DSPI_PAR_SIN(MCF_PAD_PAR_DSPI_PAR_SIN_SIN) |
                        MCF_PAD_PAR_DSPI_PAR_SCK(MCF_PAD_PAR_DSPI_PAR_SCK_SCK) |
                        MCF_PAD_PAR_DSPI_PAR_CS0(MCF_PAD_PAR_DSPI_PAR_CS0_DSPICS0) |
                        MCF_PAD_PAR_DSPI_PAR_CS2(MCF_PAD_PAR_DSPI_PAR_CS2_DSPICS2) |
                        MCF_PAD_PAR_DSPI_PAR_CS3(MCF_PAD_PAR_DSPI_PAR_CS3_DSPICS3) |
                        MCF_PAD_PAR_DSPI_PAR_CS5;

    MCF_PAD_PAR_TIMER = MCF_PAD_PAR_TIMER_PAR_TIN3(MCF_PAD_PAR_TIMER_PAR_TIN3_IRQ3) |
                        MCF_PAD_PAR_TIMER_PAR_TOUT3 |
                        MCF_PAD_PAR_TIMER_PAR_TIN2(MCF_PAD_PAR_TIMER_PAR_TIN2_IRQ2) |
                        MCF_PAD_PAR_TIMER_PAR_TOUT2;
}

/*
 * init serial
 */
static void init_serial(void)
{
    /* PSC0: SER1 */
    MCF_PSC0_PSCSICR = 0;       /* PSC control register: select UART mode */
    MCF_PSC0_PSCCSR = 0xDD;     /* use TX and RX baud rate from PSC timer */
    MCF_PSC0_PSCCTUR = 0x00;    /* =\ */
#if defined(MACHINE_FIREBEE)
    MCF_PSC0_PSCCTLR = 36;      /* divide sys_clk by 36 => BAUD RATE = 115200 bps */
#endif
#if defined(MACHINE_M5484LITE)
    MCF_PSC0_PSCCTLR = 27;      /* LITE board has 100 MHz sys_clk only */
#endif
    MCF_PSC0_PSCCR = 0x20;      /* reset receiver and RxFIFO */
    MCF_PSC0_PSCCR = 0x30;      /* reset transmitter and TxFIFO */
    MCF_PSC0_PSCCR = 0x40;      /* reset all error status */
    MCF_PSC0_PSCCR = 0x50;      /* reset break change interrupt */
    MCF_PSC0_PSCCR = 0x10;      /* reset MR pointer */
    MCF_PSC0_PSCIMR = 0x8700;   /* enable input port change interrupt, enable delta break interrupt, */
                                /* enable receiver interrupt/request, enable transceiver interrupt/request */

    MCF_PSC0_PSCACR = 0x03;     /* enable state change of CTS */
    MCF_PSC0_PSCMR1 = 0xb3;     /* 8 bit, no parity */
    MCF_PSC0_PSCMR2 = 0x07;     /* 1 stop bit */
    MCF_PSC0_PSCRFCR = 0x0F;
    MCF_PSC0_PSCTFCR = 0x0F;
    MCF_PSC0_PSCRFAR = 0x00F0;
    MCF_PSC0_PSCTFAR = 0x00F0;
    MCF_PSC0_PSCOPSET = 0x01;
    MCF_PSC0_PSCCR = 0x05;

#if defined(MACHINE_FIREBEE)  /* PSC3 is not connected to anything on the LITE board */
    /* PSC3: PIC */
    MCF_PSC3_PSCSICR = 0;   // UART
    MCF_PSC3_PSCCSR = 0xDD;
    MCF_PSC3_PSCCTUR = 0x00;
    MCF_PSC3_PSCCTLR = 36;  // BAUD RATE = 115200
    MCF_PSC3_PSCCR = 0x20;
    MCF_PSC3_PSCCR = 0x30;
    MCF_PSC3_PSCCR = 0x40;
    MCF_PSC3_PSCCR = 0x50;
    MCF_PSC3_PSCCR = 0x10;
    MCF_PSC3_PSCIMR = 0x0200;   // receiver interrupt enable
    MCF_PSC3_PSCACR = 0x03;
    MCF_PSC3_PSCMR1 = 0xb3;
    MCF_PSC3_PSCMR2 = 0x07;
    MCF_PSC3_PSCRFCR = 0x0F;
    MCF_PSC3_PSCTFCR = 0x0F;
    MCF_PSC3_PSCRFAR = 0x00F0;
    MCF_PSC3_PSCTFAR = 0x00F0;
    MCF_PSC3_PSCOPSET = 0x01;
    MCF_PSC3_PSCCR = 0x05;
#endif /* MACHINE_FIREBEE */

    xprintf("\r\nserial interfaces initialization: finished\r\n");
}

/********************************************************************/
/* Initialize DDR DIMMs on the EVB board */
/********************************************************************/
static bool init_ddram(void)
{
    xprintf("SDRAM controller initialization: ");

    /*
     * Check to see if the SDRAM has already been initialized
     * by a run control tool
     */
    if (!(MCF_SDRAMC_SDCR & MCF_SDRAMC_SDCR_REF)) {
        /* Basic configuration and initialization */

        /*
         * SB_E (Bits 9-8):  10 <=> 7.6 mA (SDCKE)
         * SB_C (Bits 7-6):  10 <=> 7.6 mA (SDRAM Clocks)
         * SB_A (Bits 5-4):  10 <=> 7.6 mA (RAS, CAS, SDWE, SDADDR[12:0], and SDBA)
         * SB_S (Bits 3-2):  10 <=> 7.6 mA (SDRDQS)
         * SB_D (Bits 1-0):  10 <=> 7.6 mA (SDRDQS)
         *
         * -> lowest setting the Coldfire SDRAM controller allows
         */
        MCF_SDRAMC_SDRAMDS = 0x000002AA;/* SDRAMDS configuration */

#if defined(MACHINE_FIREBEE)
        MCF_SDRAMC_CS0CFG = 0x0000001A; /* SDRAM CS0 configuration (128Mbytes 0000_0000 - 07FF_FFFF) */
        MCF_SDRAMC_CS1CFG = 0x0800001A; /* SDRAM CS1 configuration (128Mbytes 0800_0000 - 0FFF_FFFF) */
        MCF_SDRAMC_CS2CFG = 0x1000001A; /* SDRAM CS2 configuration (128Mbytes 1000_0000 - 07FF_FFFF) */
        MCF_SDRAMC_CS3CFG = 0x1800001A; /* SDRAM CS3 configuration (128Mbytes 1800_0000 - 1FFF_FFFF) */

        /*
         *
         */
        MCF_SDRAMC_SDCFG1 = MCF_SDRAMC_SDCFG1_WTLAT(3)  /* Write latency */
                | MCF_SDRAMC_SDCFG1_REF2ACT(8)          /* Refresh to Active Delay */
                | MCF_SDRAMC_SDCFG1_PRE2ACT(2)          /* Precharge to Active Delay */
                | MCF_SDRAMC_SDCFG1_ACT2RW(2)           /* Active to Read/Write Delay */
                | MCF_SDRAMC_SDCFG1_RDLAT(6)            /* Read CAS latency */
                | MCF_SDRAMC_SDCFG1_SWT2RD(3)           /* Single Write to Read/Write/Precharge delay */
                | MCF_SDRAMC_SDCFG1_SRD2RW(7);          /* Single Read to Read/Write/Precharge delay */

        MCF_SDRAMC_SDCFG2 = MCF_SDRAMC_SDCFG2_BL(7)     /* Burst Length */
                | MCF_SDRAMC_SDCFG2_BRD2WT(7)           /* Burst Read to Write delay */
                | MCF_SDRAMC_SDCFG2_BWT2RW(6)           /* Burst Write to Read/Write/Precharge delay */
                | MCF_SDRAMC_SDCFG2_BRD2PRE(4);         /* Burst Read to Read/Precharge delay */


        MCF_SDRAMC_SDCR = MCF_SDRAMC_SDCR_IPALL         /* initiate Precharge All command */
                | MCF_SDRAMC_SDCR_RCNT(13)              /* Refresh Count (= (x + 1) * 64 */
                | MCF_SDRAMC_SDCR_MUX(1)                /* Muxing control */
                | MCF_SDRAMC_SDCR_DDR
                | MCF_SDRAMC_SDCR_CKE
                | MCF_SDRAMC_SDCR_MODE_EN;

        MCF_SDRAMC_SDMR = MCF_SDRAMC_SDMR_CMD           /* Generate an LMR/LEMR command */
                | MCF_SDRAMC_SDMR_AD(0)                 /* Address */
                | MCF_SDRAMC_SDMR_BNKAD(1);             /* LEMR */
        MCF_SDRAMC_SDMR = MCF_SDRAMC_SDMR_CMD           /* Generate an LMR/LEMR command */
                | MCF_SDRAMC_SDMR_AD(0x123)
                | MCF_SDRAMC_SDMR_BNKAD(0);             /* LMR */

        MCF_SDRAMC_SDCR = 0xE10D0002;   /* SDCR + IPALL */
        MCF_SDRAMC_SDCR = 0xE10D0004;   /* SDCR + IREF (first refresh) */
        MCF_SDRAMC_SDCR = 0xE10D0004;   /* SDCR + IREF (second refresh) */
        MCF_SDRAMC_SDMR = 0x008D0000;   /* SDMR (write to LMR) */
        MCF_SDRAMC_SDCR = 0x710D0F00;   /* SDCR (lock SDMR and enable refresh) */

#elif defined(MACHINE_M5484LITE)
        MCF_SDRAMC_CS0CFG = 0x00000019; /* SDRAM CS0 configuration (64 Mbytes 0000_0000 - 03FF_FFFF) */
        MCF_SDRAMC_CS1CFG = 0x00000000; /* SDRAM CS1 configuration - off */
        MCF_SDRAMC_CS2CFG = 0x00000000; /* SDRAM CS2 configuration - off */
        MCF_SDRAMC_CS3CFG = 0x00000000; /* SDRAM CS3 configuration - off */


        /*
         *
         */
        MCF_SDRAMC_SDCFG1 = MCF_SDRAMC_SDCFG1_WTLAT(3)  /* Write latency */
                | MCF_SDRAMC_SDCFG1_REF2ACT(8)          /* Refresh to Active Delay */
                | MCF_SDRAMC_SDCFG1_PRE2ACT(2)          /* Precharge to Active Delay */
                | MCF_SDRAMC_SDCFG1_ACT2RW(2)           /* Active to Read/Write Delay */
                | MCF_SDRAMC_SDCFG1_RDLAT(6)            /* Read CAS latency */
                | MCF_SDRAMC_SDCFG1_SWT2RD(3)           /* Single Write to Read/Write/Precharge delay */
                | MCF_SDRAMC_SDCFG1_SRD2RW(7);          /* Single Read to Read/Write/Precharge delay */

        MCF_SDRAMC_SDCFG2 = MCF_SDRAMC_SDCFG2_BL(7)     /* Burst Length */
                | MCF_SDRAMC_SDCFG2_BRD2WT(7)           /* Burst Read to Write delay */
                | MCF_SDRAMC_SDCFG2_BWT2RW(6)           /* Burst Write to Read/Write/Precharge delay */
                | MCF_SDRAMC_SDCFG2_BRD2PRE(4);         /* Burst Read to Read/Precharge delay */


        MCF_SDRAMC_SDCR = MCF_SDRAMC_SDCR_IPALL         /* initiate Precharge All command */
                | MCF_SDRAMC_SDCR_RCNT(13)              /* Refresh Count (= (x + 1) * 64 */
                | MCF_SDRAMC_SDCR_MUX(1)                /* Muxing control */
                | MCF_SDRAMC_SDCR_DDR
                | MCF_SDRAMC_SDCR_CKE
                | MCF_SDRAMC_SDCR_MODE_EN;

        MCF_SDRAMC_SDMR = MCF_SDRAMC_SDMR_CMD           /* Generate an LMR/LEMR command */
                | MCF_SDRAMC_SDMR_AD(0)                 /* Address */
                | MCF_SDRAMC_SDMR_BNKAD(1);             /* LEMR */
        MCF_SDRAMC_SDMR = MCF_SDRAMC_SDMR_CMD           /* Generate an LMR/LEMR command */
                | MCF_SDRAMC_SDMR_AD(0x123)
                | MCF_SDRAMC_SDMR_BNKAD(0);             /* LMR */

        MCF_SDRAMC_SDCR = 0xE10D0002;   /* SDCR + IPALL */
        MCF_SDRAMC_SDCR = 0xE10D0004;   /* SDCR + IREF (first refresh) */
        MCF_SDRAMC_SDCR = 0xE10D0004;   /* SDCR + IREF (second refresh) */
        MCF_SDRAMC_SDMR = 0x008D0000;   /* SDMR (write to LMR) */
        MCF_SDRAMC_SDCR = 0x710D0F00;   /* SDCR (lock SDMR and enable refresh) */

#endif /* MACHINE_FIREBEE */

        xprintf("finished\r\n");

        return true;
    }
    else
    {
        xprintf("skipped. Already initialized (running from RAM)\r\n");
    }
    return false;
}

/*
 * initialize FlexBus chip select registers
 */
static void init_fbcs()
{
    xprintf("FlexBus chip select registers initialization: ");

    /* Flash */
    MCF_FBCS0_CSAR = MCF_FBCS_CSAR_BA(BOOTFLASH_BASE_ADDRESS);    /* flash base address */
    MCF_FBCS0_CSCR = MCF_FBCS_CSCR_PS_16 |              /* 16 bit word access */
            MCF_FBCS_CSCR_WS(8)|                        /* 6 wait states */
            MCF_FBCS_CSCR_AA |                          /* auto /TA acknowledge */
            MCF_FBCS_CSCR_ASET(1) |                     /* assert chip select on second rising edge after address assertion */
            MCF_FBCS_CSCR_RDAH(1);                      /* chip errata SECF077 */
    MCF_FBCS0_CSMR = BOOTFLASH_BAM |
            MCF_FBCS_CSMR_V;                            /* enable */


#if defined(MACHINE_FIREBEE) /* FBC setup for FireBee */
    MCF_FBCS1_CSAR = MCF_FBCS_CSAR_BA(0xFFF00000);      /* ATARI I/O address range */
    MCF_FBCS1_CSCR = MCF_FBCS_CSCR_PS_16                /* 16BIT PORT */
        | MCF_FBCS_CSCR_WS(32)                           /* 8 wait states */
        | MCF_FBCS_CSCR_AA;                             /* auto /TA acknowledge */
    MCF_FBCS1_CSMR = MCF_FBCS_CSMR_BAM_1M | MCF_FBCS_CSMR_V;

    MCF_FBCS2_CSAR = MCF_FBCS_CSAR_BA(0xF0000000);      /* Firebee new I/O address range */
    MCF_FBCS2_CSCR = MCF_FBCS_CSCR_PS_32                /* 32BIT PORT */
        | MCF_FBCS_CSCR_WS(32)                           /* 4 wait states */
        | MCF_FBCS_CSCR_AA;                             /* auto /TA acknowledge */
    MCF_FBCS2_CSMR = (MCF_FBCS_CSMR_BAM_128M            /* F000'0000-F7FF'FFFF */
              | MCF_FBCS_CSMR_V);

    MCF_FBCS3_CSAR = MCF_FBCS_CSAR_BA(0xF8000000);      /* Firebee SRAM */
    MCF_FBCS3_CSCR = MCF_FBCS_CSCR_PS_16                /* 16 bit port */
        | MCF_FBCS_CSCR_WS(32)                          /* 0 wait states */
        | MCF_FBCS_CSCR_AA;                             /* auto /TA acknowledge */
    MCF_FBCS3_CSMR = (MCF_FBCS_CSMR_BAM_64M             /* F800'0000-FBFF'FFFF */
              | MCF_FBCS_CSMR_V);

    /*
     * Note: burst read/write settings of the following FBCS are purely "cosmetical".
     * The Coldfire FlexBus only "bursts" on a smaller port size than 32 bit up to 32 bit,
     * i.e. it can burst on an 8 bit port up to 4 burst cycles or two on a 16 bit port.
     * Enabling burst on a 32 bit port has no effect (unfortunately).
     */
    MCF_FBCS4_CSAR = MCF_FBCS_CSAR_BA(0x40000000);      /* video ram area, FB_CS3 not used, decoded on FPGA */
    MCF_FBCS4_CSCR = MCF_FBCS_CSCR_PS_32                /* 32 bit port */
        | MCF_FBCS_CSCR_WS(32)                          /* 0 wait states */
        | MCF_FBCS_CSCR_AA                              /* /TA auto acknowledge */
        | MCF_FBCS_CSCR_BSTR                            /* burst read enable */
        | MCF_FBCS_CSCR_BSTW;                           /* burst write enable */
    MCF_FBCS4_CSMR = MCF_FBCS_CSMR_BAM_1G               /* 4000'0000-7FFF'FFFF */
              | MCF_FBCS_CSMR_V;

    /* disable FBCS5 on Firebee */
    MCF_FBCS5_CSAR = 0x0;
    MCF_FBCS5_CSCR = MCF_FBCS_CSCR_PS_8
        | MCF_FBCS_CSCR_BSTR
        | MCF_FBCS_CSCR_BSTW
        | MCF_FBCS_CSCR_RDAH(1);                /* chip errata SECF077 */
    MCF_FBCS5_CSMR = MCF_FBCS_CSMR_BAM_1G;
        //| MCF_FBCS_CSMR_V;
#elif defined(MACHINE_M5484LITE)
    /* disable other FBCS for now */
    MCF_FBCS1_CSMR = 0;
    MCF_FBCS2_CSMR = 0;
    MCF_FBCS3_CSMR = 0;
    MCF_FBCS4_CSMR = 0;

    /*
     * the FireEngine needs AA for its CPLD accessed registers
     */
    MCF_FBCS5_CSAR = MCF_FBCS_CSAR_BA(0x60000000);
    MCF_FBCS5_CSCR = MCF_FBCS_CSCR_PS_16 |              /* CPLD access 16 bit wide */
                     MCF_FBCS_CSCR_WS(32) |             /* 32 wait states */
                     MCF_FBCS_CSCR_ASET(1) |            /* chip select on second rising clock edge */
                     MCF_FBCS_CSCR_AA;                  /* auto acknowledge */
    MCF_FBCS5_CSMR = MCF_FBCS_CSMR_BAM_256M |           /* maps 0x60000000 to 0x68000000 */
                     MCF_FBCS_CSMR_V;
#endif /* MACHINE_FIREBEE */

    xprintf("finished\r\n");
}

#ifdef MACHINE_FIREBEE
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

    _VRAM = 0x00050400; /* IPALL */
    NOP();

    _VRAM = 0x00072000; /* load EMR pll on */
    NOP();

    _VRAM = 0x00070122; /* load MR: reset pll, cl=2, burst=4lw */
    NOP();

    _VRAM = 0x00050400; /* IPALL */
    NOP();

    _VRAM = 0x00060000; /* auto refresh */
    NOP();

    _VRAM = 0x00060000; /* auto refresh */
    NOP();

    /* FIXME: what's this? */
    _VRAM = 0000070022; /* load MR dll on */
    NOP();

    * (uint32_t *) 0xf0000400 = 0x01070082; /* fifo on, refresh on, ddrcs und cke on, video dac on, Falcon shift mode on */

    xprintf("finished\r\n");
}
#endif /* MACHINE_FIREBEE */

/*
 * probe for NEC compatible USB host controller and install if found
 */
void init_usb(void)
{
    extern struct pci_device_id ohci_usb_pci_table[];
    extern struct pci_device_id ehci_usb_pci_table[];
    struct pci_device_id *board;
    int32_t handle;
    int usb_found = 0;
    int index = 0;

    xprintf("USB controller initialization:\r\n");

    do
    {
        handle = pci_find_classcode(PCI_CLASS_SERIAL_USB | PCI_FIND_BASE_CLASS | PCI_FIND_SUB_CLASS, index++);
        dbg("handle 0x%02x\r\n", handle);
        if (handle > 0)
        {
            long id;
            long pci_class;

            xprintf("serial USB found at bus=0x%x, dev=0x%x, fnc=0x%x (0x%x)\r\n",
                    PCI_BUS_FROM_HANDLE(handle),
                    PCI_DEVICE_FROM_HANDLE(handle),
                    PCI_FUNCTION_FROM_HANDLE(handle),
                    handle);
            id = swpl(pci_read_config_longword(handle, PCIIDR));
            pci_class = swpl(pci_read_config_longword(handle, PCIREV));

            if (pci_class == PCI_CLASS_SERIAL_USB_EHCI)
            {
                board = ehci_usb_pci_table;
                while (board->vendor)
                {
                    if ((board->vendor == PCI_VENDOR_ID(id)) && board->device == PCI_DEVICE_ID(id))
                    {
                        if (usb_init(handle, board) >= 0)
                        {
                            usb_found++;
                        }
                    }
                    board++;
                }
            }
            if (pci_class == PCI_CLASS_SERIAL_USB_OHCI)
            {
                board = ohci_usb_pci_table;
                while (board->vendor)
                {
                    if ((board->vendor == PCI_VENDOR_ID(id)) && board->device == PCI_DEVICE_ID(id))
                    {
                        if (usb_init(handle, board) >= 0)
                            usb_found++;
                    }
                    board++;
                }
            }
        }
    } while (handle >= 0);

    xprintf("finished (found %d USB controller(s))\r\n", usb_found);
}

#if defined(MACHINE_FIREBEE)

static bool i2c_transfer_finished(void)
{
    if (MCF_I2C_I2SR & MCF_I2C_I2SR_IIF)
        return true;

    return false;
}

static void wait_i2c_transfer_finished(void)
{
    waitfor(1000, i2c_transfer_finished);      /* wait until interrupt bit has been set */
    MCF_I2C_I2SR &= ~MCF_I2C_I2SR_IIF;          /* clear interrupt bit (byte transfer finished */
}

static bool i2c_bus_free(void)
{
    return (MCF_I2C_I2SR & MCF_I2C_I2SR_IBB);
}

/*
 * TFP410 (DVI) on
 */
static void dvi_on(void)
{
    uint8_t receivedByte;
    uint8_t dummyByte;          /* only used for a dummy read */
    int num_tries = 0;

    xprintf("DVI digital video output initialization: ");

    MCF_I2C_I2FDR = 0x3c;       /* divide system clock by 1280: 100kHz standard */

    do
    {
        /* disable all i2c interrupt routing targets */
        MCF_I2C_I2ICR = 0x0;    // ~(MCF_I2C_I2ICR_IE | MCF_I2C_I2ICR_RE | MCF_I2C_I2ICR_TE | MCF_I2C_I2ICR_BNBE);

        /* disable i2c, disable i2c interrupts, slave, receive, i2c = acknowledge, no repeat start */
        MCF_I2C_I2CR = 0x0;

        /* repeat start, transmit acknowledge */
        MCF_I2C_I2CR = MCF_I2C_I2CR_RSTA | MCF_I2C_I2CR_TXAK;

        receivedByte = MCF_I2C_I2DR;                /* read a byte */
        MCF_I2C_I2SR = 0x0;                         /* clear status register */
        MCF_I2C_I2CR = 0x0;                         /* clear control register */

        MCF_I2C_I2ICR = MCF_I2C_I2ICR_IE;           /* route i2c interrupts to cpu */

        /* i2c enable, master mode, transmit acknowledge */
        MCF_I2C_I2CR = MCF_I2C_I2CR_IEN | MCF_I2C_I2CR_MSTA | MCF_I2C_I2CR_MTX;

        MCF_I2C_I2DR = 0x7a;                        /* send data: address of TFP410 */
        wait_i2c_transfer_finished();

        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)       /* next try if no acknowledge */
            goto try_again;

        MCF_I2C_I2DR = 0x00;                        /* send data: SUB ADRESS 0 */
        wait_i2c_transfer_finished();

        MCF_I2C_I2CR |= MCF_I2C_I2CR_RSTA;          /* repeat start */
        MCF_I2C_I2DR = 0x7b;                        /* begin read */

        wait_i2c_transfer_finished();
        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)       /* next try if no acknowledge */
            goto try_again;

#ifdef _NOT_USED_
        MCH_I2C_I2CR &= ~MCF_I2C_I2CR_MTX;          /* FIXME: not clear where this came from ... */
#endif /* _NOT_USED_ */
        MCF_I2C_I2CR &= 0xef;                       /* ... this actually disables the I2C module... */
        dummyByte = MCF_I2C_I2DR;                   /* dummy read */

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR |= MCF_I2C_I2CR_TXAK;          /* transmit acknowledge enable */
        receivedByte = MCF_I2C_I2DR;                /* read a byte */

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR = MCF_I2C_I2CR_IEN;            /* stop */

        dummyByte = MCF_I2C_I2DR; // dummy read

        if (receivedByte != 0x4c)
            goto try_again;

        MCF_I2C_I2CR = 0x0; // stop
        MCF_I2C_I2SR = 0x0; // clear sr

        waitfor(10000, i2c_bus_free);

        MCF_I2C_I2CR = 0xb0; // on tx master
        MCF_I2C_I2DR = 0x7A;

        wait_i2c_transfer_finished();

        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
            goto try_again;

        MCF_I2C_I2DR = 0x08; // SUB ADRESS 8

        wait_i2c_transfer_finished();

        MCF_I2C_I2DR = 0xbf; // ctl1: power on, T:M:D:S: enable

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR = 0x80; // stop
        dummyByte = MCF_I2C_I2DR; // dummy read
        MCF_I2C_I2SR = 0x0; // clear sr

        waitfor(10000, i2c_bus_free);

        MCF_I2C_I2CR = 0xb0;
        MCF_I2C_I2DR = 0x7A;

        wait_i2c_transfer_finished();

        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
            goto try_again;

        MCF_I2C_I2DR = 0x08; // SUB ADRESS 8

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR |= 0x4; // repeat start
        MCF_I2C_I2DR = 0x7b; // beginn read

        wait_i2c_transfer_finished();

        if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
            goto try_again;

        MCF_I2C_I2CR &= 0xef; // switch to rx
        dummyByte = MCF_I2C_I2DR; // dummy read

        wait_i2c_transfer_finished();
        MCF_I2C_I2CR |= 0x08; // txak=1

        wait(50);

        receivedByte = MCF_I2C_I2DR;

        wait_i2c_transfer_finished();

        MCF_I2C_I2CR = 0x80; // stop

        dummyByte = MCF_I2C_I2DR; // dummy read

try_again:
        num_tries++;
    } while ((receivedByte != 0xbf) && (num_tries < 10));

    if (num_tries >= 10)
    {
        xprintf("FAILED!\r\n");
    }
    else
    {
        xprintf("finished\r\n");
    }
    UNUSED(dummyByte);
    // Avoid warning
}


/*
 * AC97
 */
static void init_ac97(void)
{
    // PSC2: AC97 ----------
    int i;
    int zm;
    int va;
    int vb;
    int vc;

    xprintf("AC97 sound chip initialization: ");
    MCF_PAD_PAR_PSC2 = MCF_PAD_PAR_PSC2_PAR_RTS2_RTS    // PSC2=TX,RX BCLK,CTS->AC'97
           | MCF_PAD_PAR_PSC2_PAR_CTS2_BCLK
             | MCF_PAD_PAR_PSC2_PAR_TXD2
             | MCF_PAD_PAR_PSC2_PAR_RXD2;
    MCF_PSC2_PSCMR1 = 0x0;
    MCF_PSC2_PSCMR2 = 0x0;
    MCF_PSC2_PSCIMR = 0x0300;
    MCF_PSC2_PSCSICR = 0x03;    //AC97
    MCF_PSC2_PSCRFCR = 0x0f000000;
    MCF_PSC2_PSCTFCR = 0x0f000000;
    MCF_PSC2_PSCRFAR = 0x00F0;
    MCF_PSC2_PSCTFAR = 0x00F0;

    for (zm = 0; zm < 100000; zm++) // wiederholen bis synchron
    {
        MCF_PSC2_PSCCR = 0x20;
        MCF_PSC2_PSCCR = 0x30;
        MCF_PSC2_PSCCR = 0x40;
        MCF_PSC2_PSCCR = 0x05;

        // MASTER VOLUME -0dB
        MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
        MCF_PSC2_PSCTB_AC97 = 0x02000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02

        for (i = 2; i < 13; i++)
        {
            MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
        }

        // read register
        MCF_PSC2_PSCTB_AC97 = 0xc0000000;   //START SLOT1 + SLOT2, FIRST FRAME
        MCF_PSC2_PSCTB_AC97 = 0x82000000;   //SLOT1:master volume

        for (i = 2; i < 13; i++)
        {
            MCF_PSC2_PSCTB_AC97 = 0x00000000;   //SLOT2-12:RD REG ALLES 0
        }
        wait(50);

        va = MCF_PSC2_PSCTB_AC97;
        if ((va & 0x80000fff) == 0x80000800) {
            vb = MCF_PSC2_PSCTB_AC97;
            vc = MCF_PSC2_PSCTB_AC97;

            /* FIXME: that looks more than suspicious (Fredi?) */
            /* fixed with parentheses to avoid compiler warnings, but this looks still more than wrong to me */
            if (((va & 0xE0000fff) == 0xE0000800) & (vb == 0x02000000) & (vc == 0x00000000)) {
                goto livo;
            }
        }
    }
    xprintf(" NOT");
livo:
    // AUX VOLUME ->-0dB
    MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
    MCF_PSC2_PSCTB_AC97 = 0x16000000;   //SLOT1:WR REG AUX VOLUME adr 0x16
    MCF_PSC2_PSCTB_AC97 = 0x06060000;   //SLOT1:VOLUME
    for (i = 3; i < 13; i++) {
        MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
    }

    // line in VOLUME +12dB
    MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
    MCF_PSC2_PSCTB_AC97 = 0x10000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02
    for (i = 2; i < 13; i++) {
        MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
    }
    // cd in VOLUME 0dB
    MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
    MCF_PSC2_PSCTB_AC97 = 0x12000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02
    for (i = 2; i < 13; i++) {
        MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
    }
    // mono out VOLUME 0dB
    MCF_PSC2_PSCTB_AC97 = 0xE0000000;   //START SLOT1 + SLOT2, FIRST FRAME
    MCF_PSC2_PSCTB_AC97 = 0x06000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02
    MCF_PSC2_PSCTB_AC97 = 0x00000000;   //SLOT1:WR REG MASTER VOLUME adr 0x02
    for (i = 3; i < 13; i++) {
        MCF_PSC2_PSCTB_AC97 = 0x0;  //SLOT2-12:WR REG ALLES 0
    }
    MCF_PSC2_PSCTFCR |= MCF_PSC_PSCTFCR_WFR;    //set EOF
    MCF_PSC2_PSCTB_AC97 = 0x00000000;   //last data
    xprintf(" finished\r\n");
}
#endif /* MACHINE_FIREBEE */

/* Symbols from the linker script */

extern uint8_t _STRAM_END[];
#define STRAM_END ((uint32_t)_STRAM_END)

extern uint8_t _FIRETOS[];
#define FIRETOS ((uint32_t)_FIRETOS) /* where FireTOS is stored in flash */

extern uint8_t _BAS_LMA[];
#define BAS_LMA (&_BAS_LMA[0]) /* where the BaS is stored in flash */

extern uint8_t _BAS_IN_RAM[];
#define BAS_IN_RAM (&_BAS_IN_RAM[0]) /* where the BaS is run in RAM */

extern uint8_t _BAS_SIZE[];
#define BAS_SIZE ((uint32_t)_BAS_SIZE) /* size of the BaS, in bytes */

extern uint8_t _FASTRAM_END[];
#define FASTRAM_END ((uint32_t)_FASTRAM_END)

extern uint8_t _BAS_RESIDENT_TEXT[];
#define BAS_RESIDENT_TEXT ((uint32_t) _BAS_RESIDENT_TEXT)

extern uint8_t _BAS_RESIDENT_TEXT_SIZE[];
#define BAS_RESIDENT_TEXT_SIZE ((uint32_t) _BAS_RESIDENT_TEXT_SIZE)

static void clear_bss_segment(void)
{
    extern uint8_t _BAS_BSS_START[];
    uint8_t * BAS_BSS_START = &_BAS_BSS_START[0];
    extern uint8_t _BAS_BSS_END[];
    uint8_t *BAS_BSS_END = &_BAS_BSS_END[0];

    bzero(BAS_BSS_START, BAS_BSS_END - BAS_BSS_START + 1);
}

void initialize_hardware(void)
{
    /* Test for FireTOS switch: DIP switch #5 up */
#ifdef _NOT_USED_ // #if defined(MACHINE_FIREBEE)
    if (!(DIP_SWITCH & (1 << 6))) {
        /* Minimal hardware initialization */
        init_gpio();
        init_serial();
        init_slt();
        init_fbcs();
        init_ddram();
        init_fpga();

        /* Validate ST RAM */
        * (volatile uint32_t *) 0x42e = STRAM_END;  /* phystop TOS system variable */
        * (volatile uint32_t *) 0x420 = 0x752019f3; /* memvalid TOS system variable */
        * (volatile uint32_t *) 0x43a = 0x237698aa; /* memval2 TOS system variable */
        * (volatile uint32_t *) 0x51a = 0x5555aaaa; /* memval3 TOS system variable */

        /* TT-RAM */

        xprintf("FASTRAM_END = %p\r\n", FASTRAM_END);
        * (uint32_t *) 0x5a4 = FASTRAM_END; /* ramtop TOS system variable */
        * (uint32_t *) 0x5a8 = 0x1357bd13;  /* ramvalid TOS system variable */

        /* Jump into FireTOS */
        typedef void void_func(void);
        void_func* FireTOS = (void_func*) FIRETOS;
        FireTOS();  // Should never return
        return;
    }
#endif /* MACHINE_FIREBEE */
    init_gpio();
    init_serial();

    xprintf("\n\n");
    xprintf("%s BASIS system (BaS) v %d.%d (%s, %s)\r\n\r\n",
#if defined(MACHINE_FIREBEE)
    "Firebee"
#elif MACHINE_M5484LITE
    "m5484 LITEKIT"
#else
    "unknown platform"
#endif
    , MAJOR_VERSION, MINOR_VERSION, __DATE__, __TIME__);

    /*
     * Determine cause(s) of Reset
     */
    if (MCF_SIU_RSR & MCF_SIU_RSR_RST)
        xprintf("Reset. Cause: External Reset\r\n");
    if (MCF_SIU_RSR & MCF_SIU_RSR_RSTWD)
        xprintf("Reset. Cause: Watchdog Timer Reset\n");
    if (MCF_SIU_RSR & MCF_SIU_RSR_RSTJTG)
        xprintf("Reset. Cause: BDM/JTAG Reset\r\n");

    /*
     * Clear the Reset Status Register
     */
    MCF_SIU_RSR = (MCF_SIU_RSR_RST
                    | MCF_SIU_RSR_RSTWD
                    | MCF_SIU_RSR_RSTJTG);

    /*
     * Determine which processor we are running on
     */
    xprintf("JTAGID: ");
    switch (MCF_SIU_JTAGID & MCF_SIU_JTAGID_PROCESSOR)
    {
        case MCF_SIU_JTAGID_MCF5485:
            xprintf("MCF5485");
            break;
        case MCF_SIU_JTAGID_MCF5484:
            xprintf("MCF5484");
            break;
        case MCF_SIU_JTAGID_MCF5483:
            xprintf("MCF5483");
            break;
        case MCF_SIU_JTAGID_MCF5482:
            xprintf("MCF5482");
            break;
        case MCF_SIU_JTAGID_MCF5481:
            xprintf("MCF5481");
            break;
        case MCF_SIU_JTAGID_MCF5480:
            xprintf("MCF5480");
            break;
        case MCF_SIU_JTAGID_MCF5475:
            xprintf("MCF5475");
            break;
        case MCF_SIU_JTAGID_MCF5474:
            xprintf("MCF5474");
            break;
        case MCF_SIU_JTAGID_MCF5473:
            xprintf("MCF5473");
            break;
        case MCF_SIU_JTAGID_MCF5472:
            xprintf("MCF5472");
            break;
        case MCF_SIU_JTAGID_MCF5471:
            xprintf("MCF5471");
            break;
        case MCF_SIU_JTAGID_MCF5470:
            xprintf("MCF5470");
            break;
    }

    /*
     * Determine the processor revision
     */
    xprintf(" (revision %d)\r\n", ((MCF_SIU_JTAGID & MCF_SIU_JTAGID_REV) >> 28));

    /* make sure MMU is disabled */
    MCF_MMU_MMUCR = 0;  /* MMU off */
    NOP();              /* force pipeline sync */

    init_slt();
    init_fbcs();
    init_ddram();

#if defined(MACHINE_M5484LITE)
    xprintf("Fire Engine Control register:          %02x\r\n", * (uint8_t *) 0x61000000);
    xprintf("Fire Engine interrupt register:        %02x\r\n", * (uint8_t *) 0x62000000);
    xprintf("Fire Engine interrupt mask register:   %02x\r\n", * (uint8_t *) 0x63000000);
    xprintf("Fire Engine power management register: %02x\r\n", * (uint8_t *) 0x64000000);
    xprintf("Fire Engine EEPROM SPI register:       %02x\r\n", * (uint8_t *) 0x65000000);
    xprintf("Fire Engine Flash register:            %02x\r\n", * (uint8_t *) 0x66000000);
    xprintf("Fire Engine CPLD revision register:    %02x\r\n", * (uint8_t *) 0x67000000);
    xprintf("Fire Engine Hardware revision register:%02x\r\n", * (uint8_t *) 0x68000000);

    xprintf("write control register 0x%02x\r\n", 1 << 7);
    * (uint8_t *) 0x61000000 = 1 << 7;
    xprintf("Fire Engine Control register:          %02x\r\n", * (uint8_t *) 0x61000000);
#endif /* MACHINE_M5484LITE */

    /*
     * install (preliminary) exception vectors
     */
    setup_vectors();

#ifdef _NOT_USED_
    /* make sure the handlers are called */
    __asm__ __volatile__("dc.w 0xafff");  /* should trigger a line-A exception */
#endif /* _NOT_USED_ */


    /*
     * save the planet (and reduce case heat): disable clocks of unused SOC modules
     */
    MCF_CLOCK_SPCR =    0xffffffff & ~(
                            0                       |   /* leave memory clock enabled */
                            0                       |   /* leave PCI clock enabled */
                            0                       |   /* leave FlexBus clock enabled */
                            MCF_CLOCK_SPCR_CAN0EN   |   /* disable CAN0 */
                            0                       |   /* leave DMA clock enabled */
                            0                       |   /* leave FEC0 clock enabled */
                            MCF_CLOCK_SPCR_FEC1EN   |   /* disable FEC1 */
                            MCF_CLOCK_SPCR_USBEN    |   /* disable USB slave */
                            0                       |   /* leave PSC clock enabled */
                            MCF_CLOCK_SPCR_CAN1EN   |   /* disable CAN1 */
                            MCF_CLOCK_SPCR_CRYENA   |   /* disable crypto clock A */
                            MCF_CLOCK_SPCR_CRYENB   |   /* disable crypto clock B */
                            0                           /* leave core clock enabled */
                        );

    /* the following only makes sense _after_ DDRAM has been initialized */
    clear_bss_segment();
    xprintf(".bss segment cleared\r\n");

    if (BAS_LMA != BAS_IN_RAM)
    {
        memcpy((void *) BAS_IN_RAM, BAS_LMA, BAS_SIZE);

        /* we have copied a code area, so flush the caches */
        flush_and_invalidate_caches();

    }

#if defined(MACHINE_FIREBEE)
    fpga_configured = init_fpga();

    init_pll();
    init_video_ddr();
    dvi_on();
    init_ac97();
#endif /* MACHINE_FIREBEE */
    driver_mem_init();

    /* jump into the BaS */
    extern void BaS(void);
    BaS();
}
