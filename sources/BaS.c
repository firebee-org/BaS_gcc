/*
 * BaS
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
 */

#include <stdint.h>
#include <stdbool.h>

#include "MCF5475.h"
#include "MCF5475_SLT.h"
#include "startcf.h"
#include "cache.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "bas_types.h"
#include "sd_card.h"
#include "wait.h"

#include "diskio.h"
#include "ff.h"
#include "s19reader.h"
#include "dma.h"

/* imported routines */
extern int mmu_init();
extern int vec_init();

/* Symbols from the linker script */
extern uint8_t _STRAM_END[];
#define STRAM_END ((uint32_t)_STRAM_END)
extern uint8_t _TOS[];
#define TOS ((uint32_t)_TOS) /* final TOS location */
extern uint8_t _FASTRAM_END[];
#define FASTRAM_END ((uint32_t)_FASTRAM_END)
extern uint8_t _EMUTOS[];
#define EMUTOS ((uint32_t)_EMUTOS) /* where EmuTOS is stored in flash */
extern uint8_t _EMUTOS_SIZE[];
#define EMUTOS_SIZE ((uint32_t)_EMUTOS_SIZE) /* size of EmuTOS, in bytes */

#define NOP() __asm__ __volatile__("nop\n\t" : : : "memory")	/* need this to force pipeline sync after MMUCR write */

/*
 * check if it is possible to transfer data to PIC
 */
static inline bool pic_txready(void)
{
	if (MCF_PSC3_PSCSR & MCF_PSC_PSCSR_TXRDY)
		return true;

	return true;
}

/*
 * check if it is possible to receive data from PIC
 */
static inline bool pic_rxready(void)
{
	if (MCF_PSC3_PSCSR & MCF_PSC_PSCSR_RXRDY)
		return true;

	return false;
}

void write_pic_byte(uint8_t value)
{
    /* Wait until the transmitter is ready or 1000us are passed */
	waitfor(1000, pic_txready);

    /* Transmit the byte */
    *(volatile uint8_t*)(&MCF_PSC3_PSCTB_8BIT) = value; // Really 8-bit
}

uint8_t read_pic_byte(void)
{
    /* Wait until a byte has been received or 1000us are passed */
	waitfor(1000, pic_rxready);

    /* Return the received byte */
    return *(volatile uint8_t*)(&MCF_PSC3_PSCTB_8BIT); // Really 8-bit
}

void pic_init(void)
{
	char answer[4] = "OLD";

	xprintf("initialize the PIC: ");

	/* Send the PIC initialization string */
	write_pic_byte('A');
	write_pic_byte('C');
	write_pic_byte('P');
	write_pic_byte('F');

	/* Read the 3-char answer string. Should be "OK!". */
	answer[0] = read_pic_byte();
	answer[1] = read_pic_byte();
	answer[2] = read_pic_byte();
	answer[3] = '\0';

	if (answer[0] != 'O' || answer[1] != 'K' || answer[2] != '!')
	{
		xprintf("PIC initialization failed. Already initialized?\r\n");
	}
	else
	{
		xprintf("%s\r\n", answer);
	}
}

void nvram_init(void)
{
	int i;

	xprintf("Restore the NVRAM data: ");

	/* Request for NVRAM backup data */
	write_pic_byte(0x01);

	/* Check answer type */
	if (read_pic_byte() != 0x81)
	{
		// FIXME: PIC protocol error
		xprintf("FAILED\r\n");
		return;
	}

	/* Restore the NVRAM backup to the FPGA */
	for (i = 0; i < 64; i++)
	{
		uint8_t data = read_pic_byte();
		*(volatile uint8_t*)0xffff8961 = i;
		*(volatile uint8_t*)0xffff8963 = data;
	}

	xprintf("finished\r\n");
}

/* ACP interrupt controller */
#define FPGA_INTR_CONTRL	(volatile uint32_t *) 0xf0010000
#define FPGA_INTR_ENABLE	(volatile uint8_t *)  0xf0010004
#define FPGA_INTR_PENDIN	(volatile uint32_t *) 0xf0010008

void enable_coldfire_interrupts()
{
	xprintf("enable interrupts: ");
	*FPGA_INTR_CONTRL = 0L;				/* disable all interrupts */
	MCF_EPORT_EPPAR = 0xaaa8;			/* all interrupts on falling edge */

	MCF_GPT0_GMS = MCF_GPT_GMS_ICT(1) |	/* timer 0 on, video change capture on rising edge */
			MCF_GPT_GMS_IEN |
			MCF_GPT_GMS_TMS(1);
										/* route GPT0 interrupt on interrupt controller */
	MCF_INTC_ICR62 = 0x3f;				/* interrupt level 7, interrupt priority 7 */

	*FPGA_INTR_ENABLE  = 0xfe;	/* enable int 1-7 */
	MCF_EPORT_EPIER = 0xfe;				/* int 1-7 on */
	MCF_EPORT_EPFR = 0xff;				/* clear all pending interrupts */
	MCF_INTC_IMRL = 0xffffff00;			/* int 1-7 on */
	MCF_INTC_IMRH = 0xbffffffe;			/* psc3 and timer 0 int on */

	xprintf("finished\r\n");
}

void disable_coldfire_interrupts()
{
	*FPGA_INTR_ENABLE = 0;		/* disable all interrupts */
	MCF_EPORT_EPIER = 0x0;
	MCF_EPORT_EPFR = 0x0;
	MCF_INTC_IMRL = 0xfffffffe;
	MCF_INTC_IMRH = 0xffffffff;
}

void BaS(void)
{
	uint8_t *src;
	uint8_t *dst = (uint8_t *)TOS;
	uint32_t *adr;

	pic_init();
	nvram_init();

	xprintf("copy EmuTOS: ");

	/* copy EMUTOS */
	src = (uint8_t *) EMUTOS;
	memcpy(dst, src, EMUTOS_SIZE);
	xprintf("finished\r\n");

	/* we have copied a code area, so flush the caches */
	flush_and_invalidate_caches();

	xprintf("initialize MMU: ");
	mmu_init();
	xprintf("finished\r\n");

	xprintf("initialize exception vector table: ");
	vec_init();
	xprintf("finished\r\n");

	xprintf("enable MMU: ");
	MCF_MMU_MMUCR = MCF_MMU_MMUCR_EN;	/* MMU on */
	NOP();								/* force pipeline sync */
	xprintf("finished\r\n");

	xprintf("IDE reset: ");
	/* IDE reset */
	* (volatile uint8_t *) (0xffff8802 - 2) = 14;
	* (volatile uint8_t *) (0xffff8802 - 0) = 0x80;
	wait(1);

	* (volatile uint8_t *) (0xffff8802 - 0) = 0;

	xprintf("finished\r\n");
	xprintf("enable video: ");
	/*
	 * video setup (25MHz)
	 */
	* (volatile uint32_t *) (0xf0000410 + 0) = 0x032002ba;	/* horizontal 640x480 */
	* (volatile uint32_t *) (0xf0000410 + 4) = 0x020c020a;	/* vertical 640x480 */
	* (volatile uint32_t *) (0xf0000410 + 8) = 0x0190015d;	/* horizontal 320x240 */
	* (volatile uint32_t *) (0xf0000410 + 12) = 0x020C020A;	/* vertical 320x230 */

#ifdef _NOT_USED_
//  32MHz
	* (volatile uint32_t *) (0xf0000410 + 0) = 0x037002ba;	/* horizontal 640x480 */
	* (volatile uint32_t *) (0xf0000410 + 4) = 0x020d020a;	/* vertical 640x480 */
	* (volatile uint32_t *) (0xf0000410 + 8) = 0x02a001e0;	/* horizontal 320x240 */
	* (volatile uint32_t *) (0xf0000410 + 12) = 0x05a00160;	/* vertical 320x230 */
#endif /* _NOT_USED_ */

	/* fifo on, refresh on, ddrcs and cke on, video dac on */
	* (volatile uint32_t *) (0xf0000410 - 0x20) = 0x01070002;

	xprintf("finished\r\n");

	sd_card_init();

	/*
	 * memory setup
	 */
	for (adr = (uint32_t *) 0x400L; adr < (uint32_t *) 0x800L; ) {
		*adr++ = 0x0L;
		*adr++ = 0x0L;
		*adr++ = 0x0L;
		*adr++ = 0x0L;
	}

	/* set Falcon bus control register */
	/* sets bit 3 and 6. Both are undefined on an original Falcon? */

	* (volatile uint8_t *) 0xffff8007 = 0x48;	/* FIXME: what's that ? */

	/* ST RAM */

	* (uint32_t *) 0x42e = STRAM_END;	/* phystop TOS system variable */
	* (uint32_t *) 0x420 = 0x752019f3;	/* memvalid TOS system variable */
	* (uint32_t *) 0x43a = 0x237698aa;	/* memval2 TOS system variable */
	* (uint32_t *) 0x51a = 0x5555aaaa;	/* memval3 TOS system variable */

	/* TT-RAM */

	* (uint32_t *) 0x5a4 = FASTRAM_END;	/* ramtop TOS system variable */
	* (uint32_t *) 0x5a4 = 0x1d000000;
	* (uint32_t *) 0x5a8 = 0x1357bd13;	/* ramvalid TOS system variable */

	xprintf("init ACIA: ");
	/* init ACIA */
	* (uint8_t *) 0xfffffc00 = 3;
	NOP();

	* (uint8_t *) 0xfffffc04 = 3;
	NOP();

	* (uint8_t *) 0xfffffc00 = 0x96;
	NOP();

	* (uint8_t *) 0xfffffa0f = -1;
	NOP();

	* (uint8_t *) 0xfffffa11 = -1;
	NOP();

	xprintf("finished\r\n");

	/* Test for pseudo-supervisor mode: DIP switch #6 down */
	if (DIP_SWITCH & (1 << 7)) {
		/* In this mode, the OS actually runs in user mode
		 * and all the supervisor instructions are emulated. */
		__asm__ __volatile__("move.w #0x0700,sr	\n\t" : : : "memory");
	}

	srec_execute("BASFLASH.S19");

	/* Jump into the OS */
	typedef void void_func(void);
	typedef struct {
		void *initial_sp;
		void_func *initial_pc;
	} ROM_HEADER;

	xprintf("Call OS. BaS initialization finished...\r\n");
	enable_coldfire_interrupts();

	spidma_init();

	ROM_HEADER* os_header = (ROM_HEADER*)TOS;
	os_header->initial_pc();
}
