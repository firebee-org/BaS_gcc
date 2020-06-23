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
#elif defined(MACHINE_M5475EVB)
#include "m5475e.h"
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error "unknown machine"
#endif /* MACHINE_FIREBEE */

// #define DEBUG
#include "debug.h"


/*
 * init SLICE TIMER 0
 * all  = 32.538 sec = 30.736mHz
 * BYT0 = 127.1ms/tick = 7.876Hz   offset 0
 * BYT1 = 496.5us/tick = 2.014kHz  offset 1
 * BYT2 = 1.939us/tick = 515.6kHz  offset 2
 * BYT3 = 7.576ns/tick = 132.00MHz offset 3/
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
#elif defined(MACHINE_M5484LITE) || defined(MACHINE_M5475EVB)
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
#elif defined(MACHINE_M5484LITE) || defined(MACHINE_M5475EVB)
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
#if defined(MACHINE_FIREBEE) || defined(MACHINE_M5475EVB)
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

#elif defined(MACHINE_M5484LITE) || defined(MACHINE_M5475EVB)
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
    MCF_FBCS1_CSAR = MCF_FBCS_CSAR_BA(0xFFF80000);      /* ATARI I/O address range */
    MCF_FBCS1_CSCR = MCF_FBCS_CSCR_PS_16                /* 16BIT PORT */
        | MCF_FBCS_CSCR_WS(32)                          /* 32 wait states */
//        | MCF_FBCS_CSCR_BSTR                            /* burst read enable */
//        | MCF_FBCS_CSCR_BSTW                            /* burst write enable */
        | MCF_FBCS_CSCR_AA;                             /* auto /TA acknowledge */
    MCF_FBCS1_CSMR = MCF_FBCS_CSMR_BAM_512K | MCF_FBCS_CSMR_V;

    MCF_FBCS2_CSAR = MCF_FBCS_CSAR_BA(0xF0000000);      /* Firebee new I/O address range */
    MCF_FBCS2_CSCR = MCF_FBCS_CSCR_PS_32                /* 32BIT PORT */
        | MCF_FBCS_CSCR_WS(32)                           /* 4 wait states */
        | MCF_FBCS_CSCR_AA;                             /* auto /TA acknowledge */
    MCF_FBCS2_CSMR = (MCF_FBCS_CSMR_BAM_128M            /* F000'0000-F7FF'FFFF */
              | MCF_FBCS_CSMR_V);

    MCF_FBCS3_CSAR = MCF_FBCS_CSAR_BA(0xFFF00000);      /* IDE I/O address BURST! */
    MCF_FBCS3_CSCR = MCF_FBCS_CSCR_PS_16                /* 16BIT PORT */
                    | MCF_FBCS_CSCR_WS(16)              /* 16 wait states */
                    | MCF_FBCS_CSCR_BSTR                /* Burst read enable */
                    | MCF_FBCS_CSCR_BSTW                /* Burst write enable */
                    | MCF_FBCS_CSCR_AA;                 /* auto /TA acknowledge */
    MCF_FBCS3_CSMR = (MCF_FBCS_CSMR_BAM_512K            /* FFF0'0000-FFF7'FFFF */
                    | MCF_FBCS_CSMR_V);

    /*
     * Note: burst read/write settings of the following FBCS are purely "cosmetical".
     * The Coldfire FlexBus only "bursts" on a smaller port size than 32 bit up to 32 bit,
     * i.e. it can burst on an 8 bit port up to 4 burst cycles or two on a 16 bit port.
     * Enabling burst on a 32 bit port has no effect (unfortunately).
     */
    MCF_FBCS4_CSAR = MCF_FBCS_CSAR_BA(0x40000000);      /* video ram area, FB_CS4 not used, decoded on FPGA */
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
#elif defined(MACHINE_M5484LITE) || defined(MACHINE_M5475EVB)
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
    init_gpio();
    init_serial();

    xprintf("\n\n");
    xprintf("%s BASIS system (BaS) v %d.%d (%s, %s)\r\n\r\n",
#if defined(MACHINE_FIREBEE)
    "Firebee"
#elif MACHINE_M5484LITE
    "m5484 LITEKIT"
#elif MACHINE_M5475EVB
    "m5475 EVB"
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

#if defined(MACHINE_M5484LITE) || defined(MACHINE_M5475EVB)
    dbg("Fire Engine Control register:          %02x\r\n", * (uint8_t *) 0x61000000);
    dbg("Fire Engine interrupt register:        %02x\r\n", * (uint8_t *) 0x62000000);
    dbg("Fire Engine interrupt mask register:   %02x\r\n", * (uint8_t *) 0x63000000);
    dbg("Fire Engine power management register: %02x\r\n", * (uint8_t *) 0x64000000);
    dbg("Fire Engine EEPROM SPI register:       %02x\r\n", * (uint8_t *) 0x65000000);
    dbg("Fire Engine Flash register:            %02x\r\n", * (uint8_t *) 0x66000000);
    dbg("Fire Engine CPLD revision register:    %02x\r\n", * (uint8_t *) 0x67000000);
    dbg("Fire Engine Hardware revision register:%02x\r\n", * (uint8_t *) 0x68000000);

    dbg("write control register 0x%02x\r\n", 1 << 7);
    * (uint8_t *) 0x61000000 = 1 << 7;
    dbg("Fire Engine Control register:          %02x\r\n", * (uint8_t *) 0x61000000);
#endif /* MACHINE_M5484LITE */

    /*
     * install (preliminary) exception vectors
     */
    setup_vectors();

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
        xprintf("copy BaS to RAM\r\n");
        memcpy((void *) BAS_IN_RAM, BAS_LMA, BAS_SIZE);

        /* we have copied a code area, so flush the caches */
        flush_and_invalidate_caches();
    }

    /* jump into the BaS */
    extern void BaS(void);
    BaS();
}
