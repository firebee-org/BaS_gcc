/*
 * BaS
 *
 */
#include <stdint.h>

#include "MCF5475.h"
#include "MCF5475_SLT.h"
#include "startcf.h"

extern uint32_t Bas_base[];
extern uint8_t tos_base[];

/* imported routines */
extern int mmu_init();
extern int vec_init();
extern int illegal_table_make();
extern void sd_card_idle();
extern int sd_card_init();

/* wait...() routines moved to sysinit.c */
extern void wait_10ms();
extern void wait_1ms();

/********************************************************************/
void BaS(void)
{
	int	az_sectors;
	int	i;
	uint8_t *src;
	uint8_t *dst = tos_base;
	uint32_t *adr;

	az_sectors = sd_card_init();
		
	if(az_sectors>0)
	{
		sd_card_idle();
	}
	
	/* copy tos */
	if (DIP_SWITCH & (1 << 6))
	{
		MCF_PSC3_PSCTB_8BIT = 'ACPF';
		wait_10ms();

		MCF_PSC0_PSCTB_8BIT = 'PIC ';

		MCF_PSC0_PSCTB_8BIT = MCF_PSC3_PSCRB_8BIT;
		MCF_PSC0_PSCTB_8BIT = MCF_PSC3_PSCRB_8BIT;
		MCF_PSC0_PSCTB_8BIT = MCF_PSC3_PSCRB_8BIT;
		MCF_PSC0_PSCTB_8BIT = 0x0d0a;
		MCF_PSC3_PSCTB_8BIT = 0x01;	/* request RTC data */

		if (MCF_PSC3_PSCRB_8BIT == 0x81)
		{
			for (i = 0; i < 64; i++)
			{
				* (uint8_t *) 0xffff8963 = MCF_PSC3_PSCRB_8BIT;	/* Copy the NVRAM data from the PIC to the FPGA */
			}
		}

		/* copy EMUTOS */
		src = (uint8_t *) 0xe0600000L;
		while (src < (uint8_t *) 0xe0700000L)
		{
			*dst++ = *src++;
		}
	}
	else
	{
		/* copy FireTOS */
		src = (uint8_t *) 0xe0400000L;
		while (src < (uint8_t *) 0xe0500000L)
		{
			*dst++ = *src++;
		}
	}

#ifdef _NOT_USED_
	/*
	 * set the NVRAM checksum as invalid
	 */
		// Set the NVRAM checksum as invalid
		move.b	#63,(a0)
		move.b	2(a0),d0
		add		#1,d0
		move.b	d0,2(a0)
#endif /* NOT_USED */

	mmu_init();
	vec_init();
	illegal_table_make();
		
	/* interrupts */

	* (uint32_t *) 0xf0010004 = 0L;		/* disable all interrupts */
	MCF_EPORT_EPPAR = 0xaaa8;			/* all interrupts on falling edge */

	MCF_GPT0_GMS = MCF_GPT_GMS_ICT(1) |	/* timer 0 on, video change capture on rising edge */
			MCF_GPT_GMS_IEN |
			MCF_GPT_GMS(1);
	MCF_INTC_ICR62 = 0x3f;

	* (uint8_t *) 0xf0010004 = 0xfe;	/* enable int 1-7 */
	MCF_EPORT_EPIER = 0xfe;				/* int 1-7 on */
	MCF_EPORT_EPFR = 0xff;				/* clear all pending interrupts */
	MCF_INTC_IMRL = 0xffffff00;			/* int 1-7 on */
	MCF_INTC_IMRH = 0x9ffffffe;			/* psc3 and timer 0 int on */

	MCF_MMU_MMUCR = MCF_MMU_MMUCR_EN;	/* MMU on */

	/* IDE reset */
	* (uint8_t *) (0xffff8802 - 2) = 14;
	* (uint8_t *) (0xffff8802 - 0) = 0x80;
	wait_1ms();

	* (uint8_t *) (0xffff8802 - 0) = 0;

	/*
	 * video setup (25MHz)
	 */
	* (uint32_t *) (0xf0000410 + 0) = 0x032002ba;	/* horizontal 640x480 */
	* (uint32_t *) (0xf0000410 + 4) = 0x020c020a;	/* vertical 640x480 */
	* (uint32_t *) (0xf0000410 + 8) = 0x0190015d;	/* horizontal 320x240 */
	* (uint32_t *) (0xf0000410 + 12) = 0x020C020A;	/* vertical 320x230 */

#ifdef _NOT_USED_
//  32MHz
		move.l	#0x037002ba,(a0)+			// horizontal 640x480
		move.l	#0x020d020a,(a0)+			// vertikal 640x480
		move.l	#0x02A001e0,(a0)+			// horizontal 320x240
		move.l	#0x05a00160,(a0)+			// vertikal 320x240 
#endif /* _NOT_USED_ */

	/* fifo on, refresh on, ddrcs and cke on, video dac on */
	* (uint32_t *) (0xf0000410 - 0x20) = 0x01070002;

	/*
	 * memory setup
	 */
	for (adr = (uint32_t *) 0x400L; adr < (uint32_t *) 0x800L; adr += 32) {
		*adr = 0x0L;
		*adr = 0x0L;
		*adr = 0x0L;
		*adr = 0x0L;
	}

	* (uint8_t *) 0xffff8007 = 0x48;	/* FIXME: what's that ? */

	/* ST RAM */

	* (uint32_t *) 0x42e = 0xe00000;	/* phystop TOS system variable */
	* (uint32_t *) 0x420 = 0x752019f3;	/* memvalid TOS system variable */
	* (uint32_t *) 0x43a = 0x237698aa;	/* memval2 TOS system variable */
	* (uint32_t *) 0x51a = 0x5555aaaa;	/* memval3 TOS system variable */

	/* TT-RAM */

	* (uint32_t *) 0x5a4 = (uint32_t) Bas_base;	/* ramtop TOS system variable */
	* (uint32_t *) 0x5a8 = 0x1357bd13;	/* ramvalid TOS system variable */

	/* init ACIA */
	* (uint8_t *) 0xfffffc00 = 3;
	__asm__ __volatile__("nop	\n\t" : : : "memory");
	* (uint8_t *) 0xfffffc04 = 3;
	__asm__ __volatile__("nop	\n\t" : : : "memory");
	* (uint8_t *) 0xfffffc00 = 0x96;
	__asm__ __volatile__("nop	\n\t" : : : "memory");
	* (uint8_t *) 0xfffffa0f = 0;
	__asm__ __volatile__("nop	\n\t" : : : "memory");
	* (uint8_t *) 0xfffffa11 = 0;
	__asm__ __volatile__("nop	\n\t" : : : "memory");

	if (DIP_SWITCH & (1 << 7)) {
		__asm__ __volatile__("move.w #0x0700,sr	\n\t" : : : "memory");
	}
	__asm__ __volatile__("jmp 0xe00030	\n\t" : : : "memory");
}
