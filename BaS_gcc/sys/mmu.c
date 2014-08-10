#include "mmu.h"
#include "acia.h"

/*
 * mmu.c
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
 * derived from original assembler sources:
 * Copyright 2010 - 2012 F. Aschwanden
 * Copyright 2013        M. Froeschle
 */

#define ACR_BA(x)                   ((x) & 0xffff0000)
#define ACR_ADMSK(x)                (((x) & 0xffff) << 16)
#define ACR_E(x)                    (((x) & 1) << 15)

#define ACR_S(x)                    (((x) & 3) << 13)
#define ACR_S_USERMODE              0
#define ACR_S_SUPERVISOR_MODE       1
#define ACR_S_ALL                   2

#define ACR_AMM(x)                  (((x) & 1) << 10)

#define ACR_CM(x)                   (((x) & 3) << 5)
#define ACR_CM_CACHEABLE_WT         0x0
#define ACR_CM_CACHEABLE_CB         0x1
#define ACR_CM_CACHE_INH_PRECISE    0x2
#define ACR_CM_CACHE_INH_IMPRECISE  0x3

#define ACR_SP(x)           (((x) & 1) << 3)
#define ACR_W(x)            (((x) & 1) << 2)

#include <stdint.h>
#include "bas_printf.h"
#include "bas_types.h"
#include "MCF5475.h"
#include "pci.h"
#include "cache.h"
#include "util.h"

#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error "unknown machine!"
#endif /* MACHINE_FIREBEE */

//#define DEBUG_MMU
#ifdef DEBUG_MMU
#define dbg(format, arg...) do { xprintf("DEBUG (%s()): " format, __FUNCTION__, ##arg);} while(0)
#else
#define dbg(format, arg...) do {;} while (0)
#endif /* DEBUG_MMU */

/*
 * set ASID register
 * saves new value to rt_asid and returns former value
 */
inline uint32_t set_asid(uint32_t value)
{
	extern long rt_asid;
	uint32_t ret = rt_asid;

	__asm__ __volatile__(
		"movec		%[value],ASID\n\t"
		: /* no output */
		: [value] "r" (value)
		:
	);

	rt_asid = value;

	return ret;
}


/*
 * set ACRx register
 * saves new value to rt_acrx and returns former value
 */
inline uint32_t set_acr0(uint32_t value)
{
	extern uint32_t rt_acr0;
	uint32_t ret = rt_acr0;

	__asm__ __volatile__(
		"movec		%[value],ACR0\n\t"
		: /* not output */
		: [value] "r" (value)
		:
	);
	rt_acr0 = value;

	return ret;
}

/*
 * set ACRx register
 * saves new value to rt_acrx and returns former value
 */
inline uint32_t set_acr1(uint32_t value)
{
	extern uint32_t rt_acr1;
	uint32_t ret = rt_acr1;

	__asm__ __volatile__(
		"movec		%[value],ACR1\n\t"
		: /* not output */
		: [value] "r" (value)
		:
	);
	rt_acr1 = value;

	return ret;
}


/*
 * set ACRx register
 * saves new value to rt_acrx and returns former value
 */
inline uint32_t set_acr2(uint32_t value)
{
	extern uint32_t rt_acr2;
	uint32_t ret = rt_acr2;

	__asm__ __volatile__(
		"movec		%[value],ACR2\n\t"
		: /* not output */
		: [value] "r" (value)
		:
	);
	rt_acr2 = value;

	return ret;
}

/*
 * set ACRx register
 * saves new value to rt_acrx and returns former value
 */
inline uint32_t set_acr3(uint32_t value)
{
	extern uint32_t rt_acr3;
	uint32_t ret = rt_acr3;

	__asm__ __volatile__(
		"movec		%[value],ACR3\n\t"
		: /* not output */
		: [value] "r" (value)
		:
	);
	rt_acr3 = value;

	return ret;
}

inline uint32_t set_mmubar(uint32_t value)
{
	extern uint32_t rt_mmubar;
	uint32_t ret = rt_mmubar;

	__asm__ __volatile__(
		"movec		%[value],MMUBAR\n\t"
		: /* no output */
		: [value] "r" (value)
		: /* no clobber */
	);
	rt_mmubar = value;
	NOP();

	return ret;
}

void mmu_init(void)
{
	extern uint8_t _MMUBAR[];
	uint32_t MMUBAR = (uint32_t) &_MMUBAR[0];
	extern uint8_t _TOS[];
	uint32_t TOS = (uint32_t) &_TOS[0];

	set_asid(0);			/* do not use address extension (ASID provides virtual 48 bit addresses */

	/* set data access attributes in ACR0 and ACR1 */
	set_acr0(ACR_W(0) |								/* read and write accesses permitted */
			ACR_SP(0) |								/* supervisor and user mode access permitted */
			ACR_CM(ACR_CM_CACHE_INH_PRECISE) |		/* cache inhibit, precise */
			ACR_AMM(0) |							/* control region > 16 MB */
			ACR_S(ACR_S_ALL) |						/* match addresses in user and supervisor mode */
			ACR_E(1) |								/* enable ACR */
#if defined(MACHINE_FIREBEE)
			ACR_ADMSK(0x7f) |						/* cover 2GB area from 0x80000000 to 0xffffffff */
			ACR_BA(0x80000000));					/* (equals area from 3 to 4 GB */
#elif defined(MACHINE_M5484LITE)
			ACR_ADMSK(0x7f) |						/* cover 2 GB area from 0x80000000 to 0xffffffff */
			ACR_BA(0x80000000));
#elif defined(MACHINE_M54455)
			ACR_ADMSK(0x7f) |
			ACR_BA(0x80000000));					/* FIXME: not determined yet */
#else
#error unknown machine!
#endif /* MACHINE_FIREBEE */

	// set_acr1(0x601fc000);
	set_acr1(ACR_W(0) |
			ACR_SP(0) |
			ACR_CM(0) |
#if defined(MACHINE_FIREBEE)
			ACR_CM(ACR_CM_CACHEABLE_WT) |			/* video RAM on the Firebee */
#elif defined(MACHINE_M5484LITE)
			ACR_CM(ACR_CM_CACHE_INH_PRECISE) |		/* Compact Flash on the M548xLITE */
#elif defined(MACHINE_M54455)
			ACR_CM(ACR_CM_CACHE_INH_PRECISE) |		/* FIXME: not determined yet */
#else
#error unknown machine!
#endif /* MACHINE_FIREBEE */
			ACR_AMM(0) |
			ACR_S(ACR_S_ALL) |
			ACR_E(1) |
			ACR_ADMSK(0x1f) |
			ACR_BA(0x60000000));

	/* set instruction access attributes in ACR2 and ACR3 */

	//set_acr2(0xe007c400);
	set_acr2(ACR_W(0) |
			ACR_SP(0) |
			ACR_CM(0) |
			ACR_CM(ACR_CM_CACHEABLE_WT) |
			ACR_AMM(1) |
			ACR_S(ACR_S_ALL) |
			ACR_E(1) |
			ACR_ADMSK(0x7) |
			ACR_BA(0xe0000000));

	/* disable ACR3 */
	set_acr3(0x0);

	set_mmubar(MMUBAR + 1);		/* set and enable MMUBAR */

	/* clear all MMU TLB entries */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_CA;

	/* create locked TLB entries */

	/*
	 * 0x0000'0000 - 0x000F'FFFF (first MB of physical memory) locked virtual = physical
	 */
	MCF_MMU_MMUTR = 0x0 | 					/* virtual address */
					MCF_MMU_MMUTR_SG |		/* shared global */
					MCF_MMU_MMUTR_V;		/* valid */
	MCF_MMU_MMUDR = 0x0 |					/* physical address */
					MCF_MMU_MMUDR_SZ(0) |	/* 1 MB page size */
					MCF_MMU_MMUDR_CM(0x1) |	/* cacheable, copyback */
					MCF_MMU_MMUDR_R |		/* read access enable */
					MCF_MMU_MMUDR_W |		/* write access enable */
					MCF_MMU_MMUDR_X | 		/* execute access enable */
					MCF_MMU_MMUDR_LK;		/* lock entry */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |		/* access TLB, data */
					MCF_MMU_MMUOR_UAA;		/* update allocation address field */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB | 	/* instruction */
					MCF_MMU_MMUOR_ACC |     /* access TLB */
					MCF_MMU_MMUOR_UAA;      /* update allocation address field */

	/*
	 * 0x00d0'0000 - 0x00df'ffff (last megabyte of ST RAM = Falcon video memory) locked ID = 6
	 * mapped to physical address 0x60d0'0000 (FPGA video memory)
	 * video RAM: read write execute normal write true
	 */

	MCF_MMU_MMUTR = 0x00d00000 |			/* virtual address */
#if defined(MACHINE_FIREBEE)
					MCF_MMU_MMUTR_ID(SCA_PAGE_ID) |
#endif /* MACHINE_FIREBEE */
					MCF_MMU_MMUTR_SG |		/* shared global */
					MCF_MMU_MMUTR_V;		/* valid */
#if defined(MACHINE_FIREBEE)
					/* map FPGA video memory for FireBee only */
	MCF_MMU_MMUDR = 0x60d00000 |			/* physical address */
#elif defined(MACHINE_M5484LITE)
	MCF_MMU_MMUDR = 0x00d00000 |			/* physical address */
#elif defined(MACHINE_M54455)
	MCF_MMU_MMUDR = 0x60d00000 |			/* FIXME: not determined yet */
#else
#error unknown machine!
#endif /* MACHINE_FIREBEE */
					MCF_MMU_MMUDR_SZ(0) |	/* 1 MB page size */
					MCF_MMU_MMUDR_CM(0x0) |	/* cachable writethrough */
					/* caveat: can't be supervisor protected since TOS puts the application stack there! */
					//MCF_MMU_MMUDR_SP |		/* supervisor protect */
					MCF_MMU_MMUDR_R |		/* read access enable */
					MCF_MMU_MMUDR_W |		/* write access enable */
					MCF_MMU_MMUDR_X |		/* execute access enable */
					MCF_MMU_MMUDR_LK;		/* lock entry */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |		/* access TLB, data */
					MCF_MMU_MMUOR_UAA;		/* update allocation address field */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB | 	/* instruction */
					MCF_MMU_MMUOR_ACC |     /* access TLB */
					MCF_MMU_MMUOR_UAA;      /* update allocation address field */

#if defined(MACHINE_FIREBEE)
	video_tlb = 0x2000;						/* set page as video page */
	video_sbt = 0x0;						/* clear time */
#endif /* MACHINE_FIREBEE */

	/*
	 * Make the TOS (in SDRAM) read-only
	 * This maps virtual 0x00e0'0000 - 0x00ef'ffff to the same virtual address
	 */
	MCF_MMU_MMUTR = TOS |					/* virtual address */
					MCF_MMU_MMUTR_SG |		/* shared global */
					MCF_MMU_MMUTR_V;		/* valid */
	MCF_MMU_MMUDR = TOS |					/* physical address */
					MCF_MMU_MMUDR_SZ(0) |	/* 1 MB page size */
					MCF_MMU_MMUDR_CM(0x1) |	/* cachable copyback */
					MCF_MMU_MMUDR_R |		/* read access enable */
					//MCF_MMU_MMUDR_W |		/* write access enable (FIXME: for now) */
					MCF_MMU_MMUDR_X |		/* execute access enable */
					MCF_MMU_MMUDR_LK;		/* lock entry */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |		/* access TLB, data */
					MCF_MMU_MMUOR_UAA;		/* update allocation address field */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB | 	/* instruction */
					MCF_MMU_MMUOR_ACC |     /* access TLB */
					MCF_MMU_MMUOR_UAA;      /* update allocation address field */

#if MACHINE_FIREBEE
	/*
	 * Map FireBee I/O area (0xfff0'0000 - 0xffff'ffff physical) to the Falcon-compatible I/O
	 * area (0x00f0'0000 - 0x00ff'ffff virtual) for the FireBee
	 */

	MCF_MMU_MMUTR = 0x00f00000 |			/* virtual address */
					MCF_MMU_MMUTR_SG |		/* shared global */
					MCF_MMU_MMUTR_V;		/* valid */
	MCF_MMU_MMUDR = 0xfff00000 |			/* physical address */
					MCF_MMU_MMUDR_SZ(0) |	/* 1 MB page size */
					MCF_MMU_MMUDR_CM(0x2) |	/* nocache precise */
					MCF_MMU_MMUDR_SP |		/* supervisor protect */
					MCF_MMU_MMUDR_R |		/* read access enable */
					MCF_MMU_MMUDR_W |		/* write access enable */
					MCF_MMU_MMUDR_X |		/* execute access enable */
					MCF_MMU_MMUDR_LK;		/* lock entry */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |		/* access TLB, data */
					MCF_MMU_MMUOR_UAA;		/* update allocation address field */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB | 	/* instruction */
					MCF_MMU_MMUOR_ACC |     /* access TLB */
					MCF_MMU_MMUOR_UAA;      /* update allocation address field */
#endif /* MACHINE_FIREBEE */

	/*
	 * Map (locked) the second last MB of physical SDRAM (this is where BaS .data and .bss reside) to the same
	 * virtual address. This is also used when BaS is in RAM
	 */

	MCF_MMU_MMUTR = (SDRAM_START + SDRAM_SIZE - 0x00200000) |	/* virtual address */
					MCF_MMU_MMUTR_SG |		/* shared global */
					MCF_MMU_MMUTR_V;		/* valid */
	MCF_MMU_MMUDR = (SDRAM_START + SDRAM_SIZE - 0x00200000) |	/* physical address */
					MCF_MMU_MMUDR_SZ(0) |	/* 1 MB page size */
					MCF_MMU_MMUDR_CM(0x0) |	/* cacheable writethrough */
					MCF_MMU_MMUDR_SP |		/* supervisor protect */
					MCF_MMU_MMUDR_R |		/* read access enable */
					MCF_MMU_MMUDR_W |		/* write access enable */
					MCF_MMU_MMUDR_X |		/* execute access enable */
					MCF_MMU_MMUDR_LK;		/* lock entry */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |		/* access TLB, data */
					MCF_MMU_MMUOR_UAA;		/* update allocation address field */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB | 	/* instruction */
					MCF_MMU_MMUOR_ACC |     /* access TLB */
					MCF_MMU_MMUOR_UAA;      /* update allocation address field */

	/*
	 * Map (locked) the very last MB of physical SDRAM (this is where the driver buffers reside) to the same
	 * virtual address. Used uncached for drivers.
	 */

	MCF_MMU_MMUTR = (SDRAM_START + SDRAM_SIZE - 0x00100000) |	/* virtual address */
					MCF_MMU_MMUTR_SG |		/* shared global */
					MCF_MMU_MMUTR_V;		/* valid */
	MCF_MMU_MMUDR = (SDRAM_START + SDRAM_SIZE - 0x00100000) |	/* physical address */
					MCF_MMU_MMUDR_SZ(0) |	/* 1 MB page size */
					MCF_MMU_MMUDR_CM(0x2) |	/* nocache precise */
					MCF_MMU_MMUDR_SP |		/* supervisor protect */
					MCF_MMU_MMUDR_R |		/* read access enable */
					MCF_MMU_MMUDR_W |		/* write access enable */
					//MCF_MMU_MMUDR_X |		/* execute access enable */
					MCF_MMU_MMUDR_LK;		/* lock entry */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |		/* access TLB, data */
					MCF_MMU_MMUOR_UAA;		/* update allocation address field */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB | 	/* instruction */
					MCF_MMU_MMUOR_ACC |     /* access TLB */
					MCF_MMU_MMUOR_UAA;      /* update allocation address field */
}

void mmutr_miss(uint32_t address)
{
	dbg("MMU TLB MISS at 0x%08x\r\n", address);
	flush_and_invalidate_caches();

	switch (address)
	{
		case keyctl:
		case keybd:
			/* do something to emulate the IKBD access */
			dbg("IKBD access\r\n");
			break;

		case midictl:
		case midi:
			/* do something to emulate MIDI access */
			dbg("MIDI ACIA access\r\n");
			break;

		default:
			/* add missed page to TLB */
			MCF_MMU_MMUTR = (address & 0xfff00000) | /* virtual aligned to 1M */
							MCF_MMU_MMUTR_SG |		/* shared global */
							MCF_MMU_MMUTR_V;		/* valid */

			MCF_MMU_MMUDR = (address & 0xfff00000) |	/* physical aligned to 1M */
							MCF_MMU_MMUDR_SZ(0) |	/* 1 MB page size */
							MCF_MMU_MMUDR_CM(0x1) |	/* cacheable copyback */
							MCF_MMU_MMUDR_R |		/* read access enable */
							MCF_MMU_MMUDR_W |		/* write access enable */
							MCF_MMU_MMUDR_X;		/* execute access enable */

			MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |		/* access TLB, data */
							MCF_MMU_MMUOR_UAA;		/* update allocation address field */

			MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB | 	/* instruction */
							MCF_MMU_MMUOR_ACC |     /* access TLB */
							MCF_MMU_MMUOR_UAA;      /* update allocation address field */
	}
}



