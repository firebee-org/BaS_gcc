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
#else
#error "unknown machine!"
#endif /* MACHINE_FIREBEE */

#define DBG_MMU
#ifdef DBG_MMU
#define dbg(format, arg...) do { xprintf("DEBUG: " format, ##arg);} while(0)
#else
#define dbg(format, arg...) do {;} while (0)
#endif /* DBG_MMU */

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

	set_asid(0);			/* do not use address extension (ASID provides virtual 48 bit addresses */

	/*
	 * need to set data ACRs in a way that supervisor access to all memory regions
	 * becomes possible. Otherways it might be that the supervisor stack ends up in an unmapped
	 * region when further MMU TLB entries force a page steal. This would lead to a double
	 * fault since the CPU wouldn't be able to push its exception stack frame during an access
	 * exception
	 */

	/* set data access attributes in ACR0 and ACR1 */

	set_acr0(ACR_W(0) |						/* read and write accesses permitted */
			ACR_SP(0) |						/* supervisor and user mode access permitted */
			ACR_CM(CACHE_WRITETHROUGH) |	/* cacheable, write through */
			ACR_AMM(1) | 					/* region 13 MByte */
			ACR_S(ACR_S_SUPERVISOR_MODE) |	/* memory only visible from supervisor mode */
			ACR_E(1) |						/* enable ACR */
			ACR_ADMSK(0x0d) |				/* cover 13 MByte from 0x0 */
			ACR_BA(0));						/* start from 0x0 */

	set_acr1(ACR_W(0) |						/* read and write accesses permitted */
			ACR_SP(0) |						/* supervisor and user mode access permitted */
			ACR_CM(CACHE_WRITETHROUGH) |	/* cacheable, write through */
			ACR_AMM(0) | 					/* region > 16 MByte */
			ACR_S(ACR_S_SUPERVISOR_MODE) |	/* memory only visible from supervisor mode */
			ACR_E(1) |						/* enable ACR */
			ACR_ADMSK(0x1f) |				/* cover 495 MByte from 0x0f00000 */
			ACR_BA(0x00100000));			/* start from 0xf000000 */


	/*
	 * set instruction access attributes in ACR2 and ACR3. This is the same as above, basically:
	 * enable supervisor access to all SDRAM
	 */

	set_acr2(ACR_W(0) |
			ACR_SP(0) |
			ACR_CM(CACHE_WRITETHROUGH) |
			ACR_AMM(1) |
			ACR_S(ACR_S_SUPERVISOR_MODE) |
			ACR_E(1) |
			ACR_ADMSK(0x0c) |
			ACR_BA(0x0));

	set_acr3(ACR_W(0) |
			ACR_SP(0) |
			ACR_CM(CACHE_WRITETHROUGH) |
			ACR_AMM(0) |
			ACR_S(ACR_S_SUPERVISOR_MODE) |
			ACR_E(1) |
			ACR_ADMSK(0x1f) |
			ACR_BA(0x0f)); 

	set_mmubar(MMUBAR + 1);		/* set and enable MMUBAR */

	/* clear all MMU TLB entries */
	MCF_MMU_MMUOR = MCF_MMU_MMUOR_CA;
}


/*
 * TODO: this would be nicer in an include file
 */
extern uint8_t _SYS_SRAM[];
#define SYS_SRAM_ADDRESS ((uint32_t) &_SYS_SRAM[0])
extern uint8_t _SYS_SRAM_SIZE[];

struct mmu_mapping
{
	uint32_t phys;
	uint32_t virt;
	uint32_t length;
	uint32_t pagesize;
	struct map_flags flags;
};

static struct mmu_mapping memory_map[] =
{
	/* map system vectors supervisor-protected */
	{
		0,
		0,
		0x800,
		MMU_PAGE_SIZE_1K,
		{ CACHE_WRITETHROUGH, SV_USER, 0, ACCESS_READ | ACCESS_WRITE | ACCESS_EXECUTE },
	},
	{
		0x800,
		0x800,
		0x800,
		MMU_PAGE_SIZE_1K,
		{ CACHE_WRITETHROUGH, SV_USER, 0, ACCESS_READ | ACCESS_WRITE | ACCESS_EXECUTE },
	},
	{
		/* when the first 4k are filled with 1k pages, we can switch to 8k pages */
		0x1000,
		0x1000,
		0xff000,
		MMU_PAGE_SIZE_8K,
		{ CACHE_WRITETHROUGH, SV_USER, 0, ACCESS_READ | ACCESS_WRITE | ACCESS_EXECUTE },
	},
	{
		/* arrived at a 1Meg border, we can switch to 1Meg pages */
		0x100000,
		0x100000,
		0xc00000,
		MMU_PAGE_SIZE_1M,
		{ CACHE_WRITETHROUGH, SV_USER, 0, ACCESS_READ | ACCESS_WRITE | ACCESS_EXECUTE },
	},
	{
		/* Falcon video memory. Needs special care */
		0xd00000,
		0x60d00000,
		0x100000,
		MMU_PAGE_SIZE_8K,
		{ CACHE_WRITETHROUGH, SV_USER, SCA_PAGE_ID, ACCESS_READ | ACCESS_WRITE | ACCESS_EXECUTE },
	}, 
	{
		/* ROM */
		0xe00000,
		0xe00000,
		0x100000,
		MMU_PAGE_SIZE_1M,
		{ CACHE_WRITETHROUGH, SV_USER, 0, ACCESS_READ | ACCESS_EXECUTE },
	},
	{
		/* MBAR */
		MBAR_ADDRESS,
		MBAR_ADDRESS,
		0x100000,
		MMU_PAGE_SIZE_1M,
		{ CACHE_NOCACHE_PRECISE, SV_PROTECT, 0, ACCESS_READ | ACCESS_WRITE },
	},
	{
		/* RAMBAR0 */
		RAMBAR0_ADDRESS,
		RAMBAR0_ADDRESS,
		(uint32_t) _RAMBAR0_SIZE,
		MMU_PAGE_SIZE_1K,
		{ CACHE_WRITETHROUGH, SV_PROTECT, 0, ACCESS_READ | ACCESS_WRITE | ACCESS_EXECUTE },
	},
	{
		/* RAMBAR1 */
		RAMBAR1_ADDRESS,
		RAMBAR1_ADDRESS,
		(uint32_t) _RAMBAR1_SIZE,
		MMU_PAGE_SIZE_1K,
		{ CACHE_WRITETHROUGH, SV_PROTECT, 0, ACCESS_READ | ACCESS_WRITE | ACCESS_EXECUTE },
	},
	{
		/* SYSTEM SRAM */
		SYS_SRAM_ADDRESS,
		SYS_SRAM_ADDRESS,
		(uint32_t) _SYS_SRAM_SIZE,
		MMU_PAGE_SIZE_8K,
		{ CACHE_WRITETHROUGH, SV_PROTECT, 0, ACCESS_READ | ACCESS_WRITE | ACCESS_EXECUTE },
	},
	{
		/* Firebee FPGA registers */
		(uint32_t) 0xf0000000,
		(uint32_t) 0xf0000000,
		(uint32_t) 0x08000000,
		MMU_PAGE_SIZE_1M,
		{ CACHE_NOCACHE_PRECISE, SV_PROTECT, 0, ACCESS_READ | ACCESS_WRITE },
	},
	{
		/* Falcon I/O registers */
		(uint32_t) 0xfff00000,
		(uint32_t) 0xfff00000,
		(uint32_t) 0x100000,
		MMU_PAGE_SIZE_1M,
		{ CACHE_NOCACHE_PRECISE, SV_PROTECT, 0, ACCESS_READ | ACCESS_WRITE },
	},
	{
		/* the same, but different virtual address */
		(uint32_t) 0x00f00000,
		(uint32_t) 0xfff00000,
		(uint32_t) 0x100000,
		MMU_PAGE_SIZE_1M,
		{ CACHE_NOCACHE_PRECISE, SV_PROTECT, ACCESS_READ | ACCESS_WRITE },
	}
};

static int num_mmu_maps = sizeof(memory_map) / sizeof(struct mmu_mapping);

static struct mmu_mapping *lookup_mapping(uint32_t virt)
{
	int i;

	/*
	 * dumb, for now
	 */

	for (i = 0; i < num_mmu_maps; i++)
	{
		if (virt >= memory_map[i].virt && virt <= memory_map[i].virt + memory_map[i].length - 1)
			return &memory_map[i];
	}
	return NULL;
}

/*
 * handle an access error 
 * upper level routine called from access_exception inside exceptions.S
 */
bool access_exception(uint32_t pc, uint32_t format_status)
{
	int fault_status;
	uint32_t fault_address;

	/*
	 * extract fault status from format_status exception stack field
	 */
	fault_status = format_status & 0xc030000;

	/*
	 * determine if access fault was caused by a TLB miss
	 */
	switch (fault_status)
	{
		case 0x4010000:	/* TLB miss on opword of instruction fetch */
		case 0x4020000:	/* TLB miss on extension word of instruction fetch */
		case 0x8020000:	/* TLB miss on data write */
		case 0xc020000:	/* TLB miss on data read or read-modify-write */
			//dbg("%s: access fault - TLB miss at %p. Fault status = 0x0%x\r\n", __FUNCTION__, pc, fault_status);
			break;

		default:
			return false;
	}

	if (MCF_MMU_MMUSR & 2)		/* did the last fault hit in TLB? */
	{
		/*
		 * if yes, then we already mapped that page during a previous turn and this is in fact a bus error
		 */
		return false;
	}
	else
	{
		struct mmu_mapping *map;

		fault_address = MCF_MMU_MMUAR;

		if ((map = lookup_mapping(fault_address)) != NULL)
		{
			uint32_t mask;

			switch (map->pagesize)
			{
				case MMU_PAGE_SIZE_1M:
					mask = ~(0x100000 - 1);
					break;
				case MMU_PAGE_SIZE_4K:
					mask = ~(0x1000 - 1);
					break;
				case MMU_PAGE_SIZE_8K:
					mask = ~(0x2000 - 1);
					break;
				case MMU_PAGE_SIZE_1K:
					mask = ~(0x400 - 1);
					break;
			}

			mmu_map_page(map->phys & mask, map->virt & mask, map->pagesize, map->flags);

			if (map->flags.page_id == SCA_PAGE_ID)
			{
				video_tlb = 0x2000;
				video_sbt = 0x0;
			}
			return true;
		}
	}
	return false;
}


void mmu_map_page(uint32_t virt, uint32_t phys, uint32_t map_size, struct map_flags flags)
{

	/*
	 * add page to TLB
	 */
	MCF_MMU_MMUTR = virt |					/* virtual address */
		MCF_MMU_MMUTR_ID(flags.page_id) |
		MCF_MMU_MMUTR_SG |					/* shared global */
		MCF_MMU_MMUTR_V;					/* valid */

	MCF_MMU_MMUDR = phys |					/* physical address */
		MCF_MMU_MMUDR_SZ(map_size) |		/* 1 MB page size */
		MCF_MMU_MMUDR_CM(flags.cache_mode) |
		(flags.access & ACCESS_READ ? MCF_MMU_MMUDR_R : 0) |		/* read access enable */
		(flags.access & ACCESS_WRITE ? MCF_MMU_MMUDR_W : 0) |		/* write access enable */
		(flags.access & ACCESS_EXECUTE ? MCF_MMU_MMUDR_X : 0);		/* execute access enable */

	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |		/* access TLB, data */
		MCF_MMU_MMUOR_UAA;					/* update allocation address field */

	MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB | 	/* instruction */
		MCF_MMU_MMUOR_ACC |     			/* access TLB */
		MCF_MMU_MMUOR_UAA;      			/* update allocation address field */
	dbg("%s: mapped virt=%p to phys=%p\r\n", __FUNCTION__, virt, phys);
}



