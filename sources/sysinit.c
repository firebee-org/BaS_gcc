/*
 * File:		sysinit.c
 * Purpose:		Power-on Reset configuration of the Firebee board.
 *
 * Notes: 
 *
 */
#include <stdint.h>

#include "MCF5475.h"
#include "startcf.h"
#include "cache.h"
#include "sysinit.h"

extern volatile long _VRAM;	/* start address of video ram from linker script */

/*
 * wait routines
 */
void wait_10ms(void)
{
	register uint32_t target = MCF_SLT_SCNT(0) - 1320000;

	while (MCF_SLT_SCNT(0) > target);
}

void wait_1ms(void)
{
	register uint32_t target = MCF_SLT_SCNT(0) - 132000;

	while (MCF_SLT_SCNT(0) > target);
}

void wait_100us(void)
{
	register uint32_t target = MCF_SLT_SCNT(0) - 13200;

	while (MCF_SLT_SCNT(0) > target);
}

void wait_50us(void)
{
	register uint32_t target = MCF_SLT_SCNT(0) - 6600;

	while (MCF_SLT_SCNT(0) > target);
}

void wait_10us(void)
{
	register uint32_t target = MCF_SLT_SCNT(0) - 1320;

	while (MCF_SLT_SCNT(0) > target);
}

void wait_1us(void)
{
	register uint32_t target = MCF_SLT_SCNT(0) - 132;

	while (MCF_SLT_SCNT(0) > target);
}

/*
 * init SLICE TIMER 0 
 * all  = 32.538 sec = 30.736mHz
 * BYT0 = 127.1ms/tick = 7.876Hz   offset 0
 * BYT1 = 496.5us/tick = 2.014kHz  offset 1
 * BYT2 = 1.939us/tick = 515.6kHz  offset 2
 * BYT3 = 7.576ns/tick = 132.00MHz offset 3
 * count down!!! 132MHz!!!
 */
void init_slt(void)
{
	uart_out_word('SLT ');
	uart_out_word('OK! ');

	MCF_SLT0_STCNT = 0xffffffff;
	MCF_SLT0_SCR = 0x05;

	uart_out_word(0x0a0d);
}

/*
 * init GPIO ETC.
 */
void init_gpio(void)
{
	/*
	 * pad register P.S.:FBCTL and FBCS set correctly at reset
	 */
	MCF_PAD_PAR_DMA = 0b11111111;	/* NORMAL ALS DREQ DACK */
	MCF_PAD_PAR_FECI2CIRQ = 0b1111001111001111;	/* FEC0 NORMAL, FEC1 ALS I/O, I2C, #INT5..6 */
	MCF_PAD_PAR_PCIBG = 0b0000001000111111;	/* #PCI_BG4=#TBST,#PIC_BG3=I/O,#PCI_BG2..0=NORMAL */
	MCF_PAD_PAR_PCIBR = 0b0000001000111111;	/* #PCI_BR4=#INT4,#PIC_BR3=INPUT,#PCI_BR2..0=NORMAL */
	MCF_PAD_PAR_PSC3 = 0b00001100;	/* PSC3=TX,RX CTS+RTS=I/O */
	MCF_PAD_PAR_PSC1 = 0b11111100;	/* PSC1 NORMAL SERIELL */
	MCF_PAD_PAR_PSC0 = 0b11111100;	/* PSC0 NORMAL SERIELL */
	MCF_PAD_PAR_DSPI = 0b0001111111111111;	/* DSPI NORMAL */
	MCF_PAD_PAR_TIMER = 0b00101101;	/* TIN3..2=#IRQ3..2;TOUT3..2=NORMAL */
// ALLE OUTPUTS NORMAL LOW

// ALLE DIR NORMAL INPUT = 0
	MCF_GPIO_PDDR_FEC1L = 0b00011110;	/* OUT: 4=LED,3=PRG_DQ0,2=#FPGA_CONFIG,1=PRG_CLK(FPGA) */
}

/*
 * init serial
 */
void init_serial(void)
{
	/* PSC0: SER1 */
	MCF_PSC0_PSCSICR = 0;	// UART
	MCF_PSC0_PSCCSR = 0xDD;
	MCF_PSC0_PSCCTUR = 0x00;
	MCF_PSC0_PSCCTLR = 36;	// BAUD RATE = 115200
	MCF_PSC0_PSCCR = 0x20;
	MCF_PSC0_PSCCR = 0x30;
	MCF_PSC0_PSCCR = 0x40;
	MCF_PSC0_PSCCR = 0x50;
	MCF_PSC0_PSCCR = 0x10;
	MCF_PSC0_PSCIMR = 0x8700;
	MCF_PSC0_PSCACR = 0x03;
	MCF_PSC0_PSCMR1 = 0xb3;
	MCF_PSC0_PSCMR2 = 0x07;
	MCF_PSC0_PSCRFCR = 0x0F;
	MCF_PSC0_PSCTFCR = 0x0F;
	MCF_PSC0_PSCRFAR = 0x00F0;
	MCF_PSC0_PSCTFAR = 0x00F0;
	MCF_PSC0_PSCOPSET = 0x01;
	MCF_PSC0_PSCCR = 0x05;

	/* PSC3: PIC */
	MCF_PSC3_PSCSICR = 0;	// UART
	MCF_PSC3_PSCCSR = 0xDD;
	MCF_PSC3_PSCCTUR = 0x00;
	MCF_PSC3_PSCCTLR = 36;	// BAUD RATE = 115200
	MCF_PSC3_PSCCR = 0x20;
	MCF_PSC3_PSCCR = 0x30;
	MCF_PSC3_PSCCR = 0x40;
	MCF_PSC3_PSCCR = 0x50;
	MCF_PSC3_PSCCR = 0x10;
	MCF_PSC3_PSCIMR = 0x0200;	// receiver interrupt enable
	MCF_PSC3_PSCACR = 0x03;
	MCF_PSC3_PSCMR1 = 0xb3;
	MCF_PSC3_PSCMR2 = 0x07;
	MCF_PSC3_PSCRFCR = 0x0F;
	MCF_PSC3_PSCTFCR = 0x0F;
	MCF_PSC3_PSCRFAR = 0x00F0;
	MCF_PSC3_PSCTFAR = 0x00F0;
	MCF_PSC3_PSCOPSET = 0x01;
	MCF_PSC3_PSCCR = 0x05;
	MCF_INTC_ICR32 = 0x3F;	//MAXIMALE PRIORITY/**********/

	uart_out_word('SERI');
	uart_out_word('AL O');
	uart_out_word('K!  ');
	uart_out_word(0x0a0d);
}

/********************************************************************/
/* Initialize DDR DIMMs on the EVB board */
/********************************************************************/
/*
 * Check to see if the SDRAM has already been initialized
 * by a run control tool
 */
void init_ddram(void)
{
	uart_out_word('DDRA');
	if (!(MCF_SDRAMC_SDCR & MCF_SDRAMC_SDCR_REF)) {
		/* Basic configuration and initialization */
		MCF_SDRAMC_SDRAMDS = 0x000002AA;	// SDRAMDS configuration
		MCF_SDRAMC_CS0CFG = 0x0000001A;	// SDRAM CS0 configuration (128Mbytes 0000_0000 - 07FF_FFFF)
		MCF_SDRAMC_CS1CFG = 0x0800001A;	// SDRAM CS1 configuration (128Mbytes 0800_0000 - 0FFF_FFFF)
		MCF_SDRAMC_CS2CFG = 0x1000001A;	// SDRAM CS2 configuration (128Mbytes 1000_0000 - 07FF_FFFF)
		MCF_SDRAMC_CS3CFG = 0x1800001A;	// SDRAM CS3 configuration (128Mbytes 1800_0000 - 1FFF_FFFF)
//      MCF_SDRAMC_SDCFG1       = 0x53722938;   // SDCFG1
		MCF_SDRAMC_SDCFG1 = 0x73622830;	// SDCFG1
//      MCF_SDRAMC_SDCFG2       = 0x24330000;   // SDCFG2
		MCF_SDRAMC_SDCFG2 = 0x46770000;	// SDCFG2
//      MCF_SDRAMC_SDCR         = 0xE10F0002;   // SDCR + IPALL
		MCF_SDRAMC_SDCR = 0xE10D0002;	// SDCR + IPALL
		MCF_SDRAMC_SDMR = 0x40010000;	// SDMR (write to LEMR)
//      MCF_SDRAMC_SDMR         = 0x05890000;   // SDRM (write to LMR)
		MCF_SDRAMC_SDMR = 0x048D0000;	// SDRM (write to LMR)
//      MCF_SDRAMC_SDCR         = 0xE10F0002;   // SDCR + IPALL
		MCF_SDRAMC_SDCR = 0xE10D0002;	// SDCR + IPALL
//      MCF_SDRAMC_SDCR         = 0xE10F0004;   // SDCR + IREF (first refresh)
		MCF_SDRAMC_SDCR = 0xE10D0004;	// SDCR + IREF (first refresh)
//      MCF_SDRAMC_SDCR         = 0xE10F0004;   // SDCR + IREF (second refresh)
		MCF_SDRAMC_SDCR = 0xE10D0004;	// SDCR + IREF (second refresh)
///     MCF_SDRAMC_SDMR         = 0x01890000;   // SDMR (write to LMR)
		MCF_SDRAMC_SDMR = 0x008D0000;	// SDMR (write to LMR)
//      MCF_SDRAMC_SDCR         = 0x710F0F00;   // SDCR (lock SDMR and enable refresh)
		MCF_SDRAMC_SDCR = 0x710D0F00;	// SDCR (lock SDMR and enable refresh)
	}
	uart_out_word('M OK');
	uart_out_word('!   ');
	uart_out_word(0x0a0d);
}

/*
 * init FB_CSx
 */
void init_fbcs()
{
	uart_out_word('FBCS');

	/* Flash */
	MCF_FBCS0_CSAR = 0xE0000000;	// FLASH ADRESS
	MCF_FBCS0_CSCR = 0x00001180;	// 16 bit 4ws aa
	MCF_FBCS0_CSMR = 0x007F0001;	// 8MB on 

	MCF_FBCS1_CSAR = 0xFFF00000;	// ATARI I/O ADRESS
	MCF_FBCS1_CSCR = MCF_FBCS_CSCR_PS_16	// 16BIT PORT 
	    | MCF_FBCS_CSCR_WS(8)	// DEFAULT 8WS
	    | MCF_FBCS_CSCR_AA;	// AA 
	MCF_FBCS1_CSMR = (MCF_FBCS_CSMR_BAM_1M | MCF_FBCS_CSMR_V);

	MCF_FBCS2_CSAR = 0xF0000000;	// NEUER I/O ADRESS-BEREICH
	MCF_FBCS2_CSCR = MCF_FBCS_CSCR_PS_32	// 32BIT PORT
	    | MCF_FBCS_CSCR_WS(8)	// DEFAULT 4WS
	    | MCF_FBCS_CSCR_AA;	// AA 
	MCF_FBCS2_CSMR = (MCF_FBCS_CSMR_BAM_128M	// F000'0000-F7FF'FFFF
			  | MCF_FBCS_CSMR_V);

	MCF_FBCS3_CSAR = 0xF8000000;	// NEUER I/O ADRESS-BEREICH
	MCF_FBCS3_CSCR = MCF_FBCS_CSCR_PS_16	// 16BIT PORT
	    | MCF_FBCS_CSCR_AA;	// AA 
	MCF_FBCS3_CSMR = (MCF_FBCS_CSMR_BAM_64M	// F800'0000-FBFF'FFFF
			  | MCF_FBCS_CSMR_V);

	MCF_FBCS4_CSAR = 0x40000000;	// VIDEO RAM BEREICH, #FB_CS3 WIRD NICHT BENÜTZT, DECODE DIREKT AUF DEM FPGA
	MCF_FBCS4_CSCR = MCF_FBCS_CSCR_PS_32	// 32BIT PORT
	    | MCF_FBCS_CSCR_BSTR	// BURST READ ENABLE
	    | MCF_FBCS_CSCR_BSTW;	// BURST WRITE ENABLE
	MCF_FBCS4_CSMR = (MCF_FBCS_CSMR_BAM_1G	// 4000'0000-7FFF'FFFF
			  | MCF_FBCS_CSMR_V);

	uart_out_word(' OK!');
	uart_out_word(0x0a0d);
}


void wait_pll(void)
{
	register uint32_t trgt = MCF_SLT0_SCNT - 100000;
	do {
	} while ((* (volatile uint16_t *) 0xf0000800 > 0) && MCF_SLT0_SCNT > trgt);
}

static volatile uint8_t *pll_base = (volatile uint8_t *) 0xf0000600;

void init_pll(void)
{
	uart_out_word('PLL ');

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x48) = 0x27;	/* loopfilter  r */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x08) = 1;		/* charge pump 1 */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x00) = 12;		/* N counter high = 12 */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x40) = 12;		/* N counter low = 12 */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x114) = 1;		/* ck1 bypass */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x118) = 1;		/* ck2 bypass */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x11c) = 1;		/* ck3 bypass */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x10) = 1;		/* ck0 high  = 1 */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x50) = 1;		/* ck0 low = 1 */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x144) = 1;		/* M odd division */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x44) = 1;		/* M low = 1 */

	wait_pll();
	* (volatile uint16_t *) (pll_base + 0x04) = 145;	/* M high = 145 = 146 MHz */

	wait_pll();

	* (volatile uint8_t *) 0xf0000800 = 0;				/* set */

	uart_out_word('SET!');
	uart_out_word(0x0a0d);
}



/*
 * INIT VIDEO DDR RAM
 */

#define NOP() __asm__ __volatile__("nop\n\t" : : : "memory")

void init_video_ddr(void) {
	* (volatile uint16_t *) 0xf0000400 = 0xb;	/* set cke = 1, cs=1, config = 1 */
	NOP();

	_VRAM = 0x00050400;	/* IPALL */
	NOP();

	_VRAM = 0x00072000;	/* load EMR pll on */
	NOP();

	_VRAM = 0x00070122;	/* load MR: reset pll, cl=2, burst=4lw */
	NOP();

	_VRAM = 0x00050400;	/* IPALL */
	NOP();

	_VRAM = 0x00060000;	/* auto refresh */
	NOP();

	_VRAM = 0x00060000;	/* auto refresh */
	NOP();

	_VRAM = 0000070022;	/* load MR dll on */
	NOP();

	* (uint32_t *) 0xf0000400 = 0x01070002;
}


#define	 PCI_MEMORY_OFFSET	(0x80000000)
#define	 PCI_MEMORY_SIZE	(0x40000000)
#define	 PCI_IO_OFFSET		(0xD0000000)
#define	 PCI_IO_SIZE		(0x10000000)

/*
 * INIT PCI
 */
void init_PCI(void) {
	uart_out_word('PCI ');

	MCF_PCIARB_PACR = MCF_PCIARB_PACR_INTMPRI
		 + MCF_PCIARB_PACR_EXTMPRI(0x1F)
		 + MCF_PCIARB_PACR_INTMINTEN
		 + MCF_PCIARB_PACR_EXTMINTEN(0x1F);

	// Setup burst parameters
	MCF_PCI_PCICR1 = MCF_PCI_PCICR1_CACHELINESIZE(4) + MCF_PCI_PCICR1_LATTIMER(32);
	MCF_PCI_PCICR2 = MCF_PCI_PCICR2_MINGNT(16) + MCF_PCI_PCICR2_MAXLAT(16);

	// Turn on error signaling
	MCF_PCI_PCIICR = MCF_PCI_PCIICR_TAE + MCF_PCI_PCIICR_TAE + MCF_PCI_PCIICR_REE + 32;
	MCF_PCI_PCIGSCR |= MCF_PCI_PCIGSCR_SEE;

	/* Configure Initiator Windows */
	/* initiator window 0 base / translation adress register */
	MCF_PCI_PCIIW0BTAR = (PCI_MEMORY_OFFSET + ((PCI_MEMORY_SIZE -1) >> 8)) & 0xffff0000;

	/* initiator window 1 base / translation adress register */
	MCF_PCI_PCIIW1BTAR = (PCI_IO_OFFSET + ((PCI_IO_SIZE - 1) >> 8)) & 0xffff0000;

	/* initiator window 2 base / translation address register */
	MCF_PCI_PCIIW2BTAR = 0L;	/* not used */

	/* initiator window configuration register */
	MCF_PCI_PCIIWCR = MCF_PCI_PCIIWCR_WINCTRL0_MEMRDLINE + MCF_PCI_PCIIWCR_WINCTRL1_IO;

	/* reset PCI devices */
	MCF_PCI_PCIGSCR &= ~MCF_PCI_PCIGSCR_PR;

	uart_out_word('OK! ');
	uart_out_word(0x0d0a);
}
	

/*
 * probe for UPC720101 (USB)
 */
void test_upd720101(void) 
{
	uart_out_word('NEC ');

	/* select UPD720101 AD17 */
	MCF_PCI_PCICAR = MCF_PCI_PCICAR_E +
		MCF_PCI_PCICAR_DEVNUM(17) +
		MCF_PCI_PCICAR_FUNCNUM(0) +
		MCF_PCI_PCICAR_DWORD(0);

	if (* (uint32_t *) PCI_IO_OFFSET == 0x33103500)
	{
		MCF_PCI_PCICAR = MCF_PCI_PCICAR_E + 
			MCF_PCI_PCICAR_DEVNUM(17) +
			MCF_PCI_PCICAR_FUNCNUM(0) +
			MCF_PCI_PCICAR_DWORD(57);

		* (uint8_t *) PCI_IO_OFFSET = 0x20;
	}
	else
	{
		MCF_PSC0_PSCTB_8BIT = 'NOT ';

		MCF_PCI_PCICAR = MCF_PCI_PCICAR_DEVNUM(17) +
			MCF_PCI_PCICAR_FUNCNUM(0) +
			MCF_PCI_PCICAR_DWORD(57);
	}
	uart_out_word('OK! ');
	uart_out_word(0x0d0a);
}

/*
 * TFP410 (DVI) on
 */
void dvi_on(void) {
	uint8_t RBYT;
	uint8_t DBYT;	/* FIXME: produces a warning about being unused when it is in fact (for a dummy read) */
	int tries;
	
	uart_out_word('DVI ');

	MCF_I2C_I2FDR = 0x3c;	// 100kHz standard
	tries = 0;

loop_i2c:
	if (tries++ > 10)
		goto next;
	
	MCF_I2C_I2ICR = 0x0;
	MCF_I2C_I2CR = 0x0;
	MCF_I2C_I2CR = 0xA;
	RBYT = MCF_I2C_I2DR;
	MCF_I2C_I2SR = 0x0;
	MCF_I2C_I2CR = 0x0;
	MCF_I2C_I2ICR = 0x01;
	MCF_I2C_I2CR = 0xb0;
	MCF_I2C_I2DR = 0x7a;	// ADRESSE TFP410

	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));	// warten auf fertig

	MCF_I2C_I2SR &= 0xfd;	// clear bit
	if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
		goto loop_i2c;	// ack erhalten? -> nein

	MCF_I2C_I2DR = 0x00;	// SUB ADRESS 0
	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));
	
	MCF_I2C_I2SR &= 0xfd;
	MCF_I2C_I2CR |= 0x4;	// repeat start
	MCF_I2C_I2DR = 0x7b;	// beginn read
	
	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));	// warten auf fertig

	MCF_I2C_I2SR &= 0xfd;	// clear bit

   if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
		goto loop_i2c;	// ack erhalten? -> nein

	MCF_I2C_I2CR &= 0xef;	// switch to rx 
	DBYT = MCF_I2C_I2DR;	// dummy read

	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));
	
	MCF_I2C_I2SR &= 0xfd;
	
	MCF_I2C_I2CR |= 0x08;	// txak=1
	RBYT = MCF_I2C_I2DR;
	
	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));
	
	MCF_I2C_I2SR &= 0xfd;
	MCF_I2C_I2CR = 0x80;	// stop

	DBYT = MCF_I2C_I2DR;	// dummy read

	if (RBYT != 0x4c)
		goto loop_i2c;

	MCF_I2C_I2CR = 0x0;	// stop
	MCF_I2C_I2SR = 0x0;	// clear sr

	while ((MCF_I2C_I2SR & MCF_I2C_I2SR_IBB));	// wait auf bus free

	MCF_I2C_I2CR = 0xb0;	// on tx master 
	MCF_I2C_I2DR = 0x7A;
	
	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));	// warten auf fertig
	
	MCF_I2C_I2SR &= 0xfd;	// clear bit

	if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
		goto loop_i2c;	// ack erhalten? -> nein
	  
	MCF_I2C_I2DR = 0x08;	// SUB ADRESS 8

	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));
	
	MCF_I2C_I2SR &= 0xfd;
	MCF_I2C_I2DR = 0xbf;	// ctl1: power on, T:M:D:S: enable

	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));

	MCF_I2C_I2SR &= 0xfd;;
	MCF_I2C_I2CR = 0x80;	// stop
	DBYT = MCF_I2C_I2DR;	// dummy read
	MCF_I2C_I2SR = 0x0;	// clear sr

	while ((MCF_I2C_I2SR & MCF_I2C_I2SR_IBB));	// wait auf bus free

	MCF_I2C_I2CR = 0xb0;
	MCF_I2C_I2DR = 0x7A;
	
	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));	// warten auf fertig

	MCF_I2C_I2SR &= 0xfd;	// clear bit

	if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
		goto loop_i2c;	// ack erhalten? -> nein

	MCF_I2C_I2DR = 0x08;	// SUB ADRESS 8

	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));
	
	MCF_I2C_I2SR &= 0xfd;
	MCF_I2C_I2CR |= 0x4;	// repeat start
	MCF_I2C_I2DR = 0x7b;	// beginn read

	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));	// warten auf fertig

	MCF_I2C_I2SR &= 0xfd;	// clear bit

	if (MCF_I2C_I2SR & MCF_I2C_I2SR_RXAK)
		goto loop_i2c;	// ack erhalten? -> nein
	  
	MCF_I2C_I2CR &= 0xef;	// switch to rx 
	DBYT = MCF_I2C_I2DR;	// dummy read

	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));
	
	MCF_I2C_I2SR &= 0xfd;
	MCF_I2C_I2CR |= 0x08;	// txak=1

	wait_50us();
	
	RBYT = MCF_I2C_I2DR;
	
	while (!(MCF_I2C_I2SR & MCF_I2C_I2SR_IIF));
	
	MCF_I2C_I2SR &= 0xfd;
	MCF_I2C_I2CR = 0x80;	// stop

	DBYT = MCF_I2C_I2DR;	// dummy read

	if (RBYT != 0xbf)
		goto loop_i2c;
	goto dvi_ok;
next:
	uart_out_word('NOT ');
dvi_ok:
	uart_out_word('OK! ');
	uart_out_word(0x0a0d);
	MCF_I2C_I2CR = 0x0;	// i2c off
}


/*
 * AC97
 */
void init_ac97(void) {
	// PSC2: AC97 ----------
	int i;
	int zm;
	int va;
	int vb;
	int vc;
	
	uart_out_word('AC97');
	MCF_PAD_PAR_PSC2 = MCF_PAD_PAR_PSC2_PAR_RTS2_RTS	// PSC2=TX,RX BCLK,CTS->AC'97
	       | MCF_PAD_PAR_PSC2_PAR_CTS2_BCLK
			 | MCF_PAD_PAR_PSC2_PAR_TXD2
			 | MCF_PAD_PAR_PSC2_PAR_RXD2;
	MCF_PSC2_PSCMR1 = 0x0;
	MCF_PSC2_PSCMR2 = 0x0;
	MCF_PSC2_PSCIMR = 0x0300;
	MCF_PSC2_PSCSICR = 0x03;	//AC97           
	MCF_PSC2_PSCRFCR = 0x0f000000;
	MCF_PSC2_PSCTFCR = 0x0f000000;
	MCF_PSC2_PSCRFAR = 0x00F0;
	MCF_PSC2_PSCTFAR = 0x00F0;

	for (zm = 0; zm < 100000; zm++)	// wiederholen bis synchron
	{
		MCF_PSC2_PSCCR = 0x20;
		MCF_PSC2_PSCCR = 0x30;
		MCF_PSC2_PSCCR = 0x40;
		MCF_PSC2_PSCCR = 0x05;

		// MASTER VOLUME -0dB
		MCF_PSC2_PSCTB_AC97 = 0xE0000000;	//START SLOT1 + SLOT2, FIRST FRAME
		MCF_PSC2_PSCTB_AC97 = 0x02000000;	//SLOT1:WR REG MASTER VOLUME adr 0x02

		for (i = 2; i < 13; i++)
		{
			MCF_PSC2_PSCTB_AC97 = 0x0;	//SLOT2-12:WR REG ALLES 0
		}
		
		// read register
		MCF_PSC2_PSCTB_AC97 = 0xc0000000;	//START SLOT1 + SLOT2, FIRST FRAME
		MCF_PSC2_PSCTB_AC97 = 0x82000000;	//SLOT1:master volume

		for (i = 2; i < 13; i++)
		{
			MCF_PSC2_PSCTB_AC97 = 0x00000000;	//SLOT2-12:RD REG ALLES 0
		}
		wait_50us();
		
		va = MCF_PSC2_PSCTB_AC97;
		if ((va & 0x80000fff) == 0x80000800) {
			vb = MCF_PSC2_PSCTB_AC97;
			vc = MCF_PSC2_PSCTB_AC97;

			/* FIXME: that looks more than suspicious (Fredi?) */
			if ((va & 0xE0000fff) == 0xE0000800 & vb == 0x02000000 & vc == 0x00000000) {
				goto livo;
			}
		}
	}
	uart_out_word(' NOT');
livo:
	// AUX VOLUME ->-0dB
	MCF_PSC2_PSCTB_AC97 = 0xE0000000;	//START SLOT1 + SLOT2, FIRST FRAME
	MCF_PSC2_PSCTB_AC97 = 0x16000000;	//SLOT1:WR REG AUX VOLUME adr 0x16
	MCF_PSC2_PSCTB_AC97 = 0x06060000;	//SLOT1:VOLUME
	for (i = 3; i < 13; i++) {
		MCF_PSC2_PSCTB_AC97 = 0x0;	//SLOT2-12:WR REG ALLES 0
	}

	// line in VOLUME +12dB
	MCF_PSC2_PSCTB_AC97 = 0xE0000000;	//START SLOT1 + SLOT2, FIRST FRAME
	MCF_PSC2_PSCTB_AC97 = 0x10000000;	//SLOT1:WR REG MASTER VOLUME adr 0x02
	for (i = 2; i < 13; i++) {
		MCF_PSC2_PSCTB_AC97 = 0x0;	//SLOT2-12:WR REG ALLES 0
	}
	// cd in VOLUME 0dB
	MCF_PSC2_PSCTB_AC97 = 0xE0000000;	//START SLOT1 + SLOT2, FIRST FRAME
	MCF_PSC2_PSCTB_AC97 = 0x12000000;	//SLOT1:WR REG MASTER VOLUME adr 0x02
	for (i = 2; i < 13; i++) {
		MCF_PSC2_PSCTB_AC97 = 0x0;	//SLOT2-12:WR REG ALLES 0
	}
	// mono out VOLUME 0dB
	MCF_PSC2_PSCTB_AC97 = 0xE0000000;	//START SLOT1 + SLOT2, FIRST FRAME
	MCF_PSC2_PSCTB_AC97 = 0x06000000;	//SLOT1:WR REG MASTER VOLUME adr 0x02
	MCF_PSC2_PSCTB_AC97 = 0x00000000;	//SLOT1:WR REG MASTER VOLUME adr 0x02
	for (i = 3; i < 13; i++) {
		MCF_PSC2_PSCTB_AC97 = 0x0;	//SLOT2-12:WR REG ALLES 0
	}
	MCF_PSC2_PSCTFCR |= MCF_PSC_PSCTFCR_WFR;	//set EOF
	MCF_PSC2_PSCTB_AC97 = 0x00000000;	//last data
	uart_out_word(' OK!');
	uart_out_word(0x0a0d);
}

/* Symbols from the linker script */

extern uint8_t _FIRETOS[];
#define FIRETOS ((uint32_t)_FIRETOS) /* where FireTOS is stored in flash */

extern uint8_t _BAS_LMA[];
#define BAS_LMA ((uint32_t)_BAS_LMA) /* where the BaS is stored in flash */

extern uint8_t _BAS_VMA[];
#define BAS_VMA ((uint32_t)_BAS_VMA) /* where the BaS is run in RAM */

extern uint8_t _BAS_SIZE[];
#define BAS_SIZE ((uint32_t)_BAS_SIZE) /* size of the BaS, in bytes */

void initialize_hardware(void) {
	/* used in copy loop */
	uint32_t *src;	/* src address to read from flash */
	uint32_t *end;	/* end address to read from flash */
	uint32_t *dst;	/* destination address to copy to */
	uint32_t *jmp;	/* address of BaS() routine to jmp at after copy */

	/* Test for FireTOS switch: DIP switch #5 up */
	if (!(DIP_SWITCH & (1 << 6))) {
		/* Minimal hardware initialization */
		init_gpio();
		init_fbcs();
		init_ddram();
		init_fpga();

		/* FireTOS seems to have trouble to initialize the ST-RAM by itself, so... */
		/* Validate ST RAM */
		* (volatile uint32_t *) 0x42e = 0x00e00000;	/* phystop TOS system variable */
		* (volatile uint32_t *) 0x420 = 0x752019f3;	/* memvalid TOS system variable */
		* (volatile uint32_t *) 0x43a = 0x237698aa;	/* memval2 TOS system variable */
		* (volatile uint32_t *) 0x51a = 0x5555aaaa;	/* memval3 TOS system variable */

		/* Jump into FireTOS */
		typedef void void_func(void);
		void_func* FireTOS = (void_func*)FIRETOS;
		FireTOS(); // Should never return
		return;
	}

	init_gpio();
	init_serial();
	init_slt();
	init_fbcs();
	init_ddram();
	init_PCI();
	init_fpga();
	init_pll();
	init_video_ddr();
	dvi_on();
	test_upd720101();
	//video_1280_1024();
	init_ac97();

	/* copy the BaS code contained in flash to its final location */
	src = (uint32_t *)BAS_LMA;
	end = (uint32_t *)(BAS_LMA + BAS_SIZE);
	dst = jmp = (uint32_t *)BAS_VMA;

	/* FIXME: beware of possible alignment */
	do
	{
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
		*dst++ = *src++;
	} while (src < end);

	/* we have copied a code area, so flush the caches */
	flush_and_invalidate_caches();

	/* jump into the BaS in RAM */
	extern void BaS(void);
	BaS();
}
