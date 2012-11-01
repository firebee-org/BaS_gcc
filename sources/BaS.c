/*
 * BaS
 *
 */
#include <stdint.h>

#include "MCF5475.h"
#include "MCF5475_SLT.h"
#include "startcf.h"
#include "cache.h"
#include "bas_printf.h"

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

/* Symbols from the linker script */
extern uint8_t _EMUTOS[];
#define EMUTOS ((uint32_t)_EMUTOS) /* where EmuTOS is stored in flash */
extern uint8_t _EMUTOS_SIZE[];
#define EMUTOS_SIZE ((uint32_t)_EMUTOS_SIZE) /* size of EmuTOS, in bytes */

/********************************************************************/
void BaS(void)
{
	int	az_sectors;
	int	i;
	uint8_t *src;
	uint8_t *dst = tos_base;
	uint32_t *adr;
/*
	az_sectors = sd_card_init();
		
	if (az_sectors > 0)
	{
		sd_card_idle();
	}
	*/
	/* Initialize the NVRAM */

	xprintf("initialize the NVRAM: ");
	MCF_PSC3_PSCTB_8BIT = 'ACPF';
	wait_10ms();

	* (volatile uint8_t *) &_MBAR[0x860C] = (uint8_t) (MCF_PSC3_PSCRB_8BIT);
	* (volatile uint8_t *) &_MBAR[0x860C] = (uint8_t) (MCF_PSC3_PSCRB_8BIT);
	* (volatile uint8_t *) &_MBAR[0x860C] = (uint8_t) (MCF_PSC3_PSCRB_8BIT);

	xprintf("finished\r\n");

	//MCF_PSC3_PSCTB_8BIT = 0x01;	/* request RTC data */

	* (volatile uint8_t *) &_MBAR[0x890C] = 0x01;

	//if (MCF_PSC3_PSCRB_8BIT == 0x81)
	if (* (volatile uint8_t *) &_MBAR[0X890C] == 0x81)
	{
		for (i = 0; i < 64; i++)
		{
			* (volatile int8_t *) 0xffff8963 = (uint8_t) MCF_PSC3_PSCRB_8BIT;	/* Copy the NVRAM data from the PIC to the FPGA */
		}
	}

	xprintf("copy EmuTOS: ");

	/* copy EMUTOS */
	src = (uint8_t *) EMUTOS;
	while (src < (uint8_t *)(EMUTOS + EMUTOS_SIZE))
	{
		*dst++ = *src++;
	}

	xprintf("finished\r\n");

	/* we have copied a code area, so flush the caches */
	flush_and_invalidate_caches();

	xprintf("initialize MMU: ");
	mmu_init();
	xprintf("finished\r\n");

	xprintf("initialize exception vector table: ");
	vec_init();
	illegal_table_make();
	xprintf("finished\r\n");
		
	/* interrupts */

	xprintf("enable interrupts: ");
	* (volatile uint32_t *) 0xf0010004 = 0L;		/* disable all interrupts */
	MCF_EPORT_EPPAR = 0xaaa8;			/* all interrupts on falling edge */

	MCF_GPT0_GMS = MCF_GPT_GMS_ICT(1) |	/* timer 0 on, video change capture on rising edge */
			MCF_GPT_GMS_IEN |
			MCF_GPT_GMS_TMS(1);
	MCF_INTC_ICR62 = 0x3f;

	* (volatile uint8_t *) 0xf0010004 = 0xfe;	/* enable int 1-7 */
	MCF_EPORT_EPIER = 0xfe;				/* int 1-7 on */
	MCF_EPORT_EPFR = 0xff;				/* clear all pending interrupts */
	MCF_INTC_IMRL = 0xffffff00;			/* int 1-7 on */
	MCF_INTC_IMRH = 0xbffffffe;			/* psc3 and timer 0 int on */

	//MCF_MMU_MMUCR = MCF_MMU_MMUCR_EN;	/* MMU on */
	xprintf("finished\r\n");

	xprintf("IDE reset: ");
	/* IDE reset */
	* (volatile uint8_t *) (0xffff8802 - 2) = 14;
	* (volatile uint8_t *) (0xffff8802 - 0) = 0x80;
	wait_1ms();

	* (volatile uint8_t *) (0xffff8802 - 0) = 0;

	xprintf("finished\r\n");

	/*
	 * video setup (25MHz)
	 */
	* (volatile uint32_t *) (0xf0000410 + 0) = 0x032002ba;	/* horizontal 640x480 */
	* (volatile uint32_t *) (0xf0000410 + 4) = 0x020c020a;	/* vertical 640x480 */
	* (volatile uint32_t *) (0xf0000410 + 8) = 0x0190015d;	/* horizontal 320x240 */
	* (volatile uint32_t *) (0xf0000410 + 12) = 0x020C020A;	/* vertical 320x230 */

#ifdef _NOT_USED_
//  32MHz
		move.l	#0x037002ba,(a0)+			// horizontal 640x480
		move.l	#0x020d020a,(a0)+			// vertikal 640x480
		move.l	#0x02A001e0,(a0)+			// horizontal 320x240
		move.l	#0x05a00160,(a0)+			// vertikal 320x240 
#endif /* _NOT_USED_ */

	/* fifo on, refresh on, ddrcs and cke on, video dac on */
	* (volatile uint32_t *) (0xf0000410 - 0x20) = 0x01070002;

	/*
	 * memory setup
	 */
	for (adr = (uint32_t *) 0x400L; adr < (uint32_t *) 0x800L; ) {
		*adr++ = 0x0L;
		*adr++ = 0x0L;
		*adr++ = 0x0L;
		*adr++ = 0x0L;
	}

	* (volatile uint8_t *) 0xffff8007 = 0x48;	/* FIXME: what's that ? */

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
	* (uint8_t *) 0xfffffa0f = -1;
	__asm__ __volatile__("nop	\n\t" : : : "memory");
	* (uint8_t *) 0xfffffa11 = -1;
	__asm__ __volatile__("nop	\n\t" : : : "memory");

	/* Test for pseudo-supervisor mode: DIP switch #6 down */
	if (DIP_SWITCH & (1 << 7)) {
		/* In this mode, the OS actually runs in user mode
		 * and all the supervisor instructions are emulated. */
		__asm__ __volatile__("move.w #0x0700,sr	\n\t" : : : "memory");
	}

	/* Jump into the OS */
	typedef void void_func(void);
	typedef struct {
		void *initial_sp;
		void_func *initial_pc;
	} ROM_HEADER;
	ROM_HEADER* os_header = (ROM_HEADER*)tos_base;
	os_header->initial_pc();
}
