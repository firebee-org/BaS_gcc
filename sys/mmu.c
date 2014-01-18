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
			ACR_CM(ACR_CM_CACHEABLE_WT) |	/* cacheable, write through */
			ACR_AMM(1) | 					/* region 13 MByte */
			ACR_S(ACR_S_SUPERVISOR_MODE) |	/* memory only visible from supervisor mode */
			ACR_E(1) |						/* enable ACR */
			ACR_ADMSK(0x0d) |				/* cover 13 MByte from 0x0 */
			ACR_BA(0));						/* start from 0x0 */

	set_acr1(ACR_W(0) |						/* read and write accesses permitted */
			ACR_SP(0) |						/* supervisor and user mode access permitted */
			ACR_CM(ACR_CM_CACHEABLE_WT) |	/* cacheable, write through */
			ACR_AMM(0) | 					/* region > 16 MByte */
			ACR_S(ACR_S_SUPERVISOR_MODE) |	/* memory only visible from supervisor mode */
			ACR_E(1) |						/* enable ACR */
			ACR_ADMSK(0x1f) |				/* cover 495 MByte from 0x0f00000 */
			ACR_BA(0x0f000000));					/* start from 0xf000000 */


	/*
	 * set instruction access attributes in ACR2 and ACR3. This is the same as above, basically:
	 * enable supervisor access to all SDRAM
	 */

	set_acr2(ACR_W(0) |
			ACR_SP(0) |
			ACR_CM(ACR_CM_CACHEABLE_WT) |
			ACR_AMM(1) |
			ACR_S(ACR_S_SUPERVISOR_MODE) |
			ACR_E(1) |
			ACR_ADMSK(0x0c) |
			ACR_BA(0x0));

	set_acr3(ACR_W(0) |
			ACR_SP(0) |
			ACR_CM(ACR_CM_CACHEABLE_WT) |
			ACR_AMM(0) |
			ACR_S(ACR_S_SUPERVISOR_MODE) |
			ACR_E(1) |
			ACR_ADMSK(0x1f) |
			ACR_BA(0x0f)); 

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


/*
 * TODO: this would be nicer in an include file
 */
extern uint8_t _MBAR[];
extern uint8_t _RAMBAR0[];
extern uint8_t _RAMBAR1[];
extern uint8_t _SYS_SRAM[];
extern uint8_t _SYS_SRAM_SIZE[];

static struct mmu_mapping
{
	uint32_t phys;
	uint32_t virt;
	uint32_t length;
	uint32_t pagesize;
	uint32_t flags;
} memory_map[] =
{
	{
		_RAMBAR0,
		_RAMBAR0,
		_RAMBAR0_SIZE,
		MMU_PAGE_SIZE_1K,
		MMU_CACHE_WRITETHROUGH,
	},
	{
		_RAMBAR1,
		_RAMBAR1,
		_RAMBAR1_SIZE,
		MMU_PAGE_SIZE_1K,
		MMU_CACHE_WRITETHROUGH,
	},
	{
		_SYS_SRAM,
		_SYS_SRAM,
		_SYS_SRAM_SIZE,
		MMU_PAGE_SIZE_1K,
		MMU_CACHE_WRITETHROUGH,
	}
};

static int num_mmu_maps = sizeof(memory_map) / sizeof(struct mmu_mapping);

/*
 * handle an access error 
 * upper level routine called from access_exception inside exceptions.S
 */
bool access_exception(uint32_t pc, uint32_t format_status)
{
	int fault_status;
	uint32_t fault_address;
	bool is_tlb_miss = false;	/* assume access error is not a TLB miss */
	extern uint8_t _FASTRAM_END[];
	uint32_t FASTRAM_END = (uint32_t) &_FASTRAM_END[0];

	/*
	 * extract fault status from format_status exception stack field
	 */
	fault_status = (((format_status & 0xc000000) >> 24) |
					((format_status & 0x30000) >> 16));

	/*
	 * determine if access fault was caused by a TLB miss
	 */
	switch (fault_status)
	{
		case 0x5:	/* TLB miss on opword of instruction fetch */
		case 0x6:	/* TLB miss on extension word of instruction fetch */
		case 0xa:	/* TLB miss on data write */
		case 0xe:	/* TLB miss on data read or read-modify-write */
			dbg("%s: access fault - TLB miss at %p. Fault status = 0x0%x\r\n", __FUNCTION__, pc, fault_status);
			is_tlb_miss = true;
			break;

		default:
			break;
	}

	if (is_tlb_miss)
	{
		if (MCF_MMU_MMUSR & 1)		/* did the last fault hit in TLB? */
		{
			/*
			 * if yes, then we already mapped that page during a previous turn and this is in fact a bus error
			 */
			is_tlb_miss = false;
		}
		else
		{
			uint32_t flags;

			/* TODO: MBAR, MMUBAR, PCI MEMORY, PCI IO, DMA BUFFERS */
			fault_address = MCF_MMU_MMUAR;	/* retrieve fault access address from MMU */
			if (fault_address >= _RAMBAR0 && fault_address <= _RAMBAR0 + (uint32_t) _RAMBAR0_SIZE)
			{
				mmu_map_page(fault_address & 0xfffff400, fault_address & 0xfffff400, MMU_PAGE_SIZE_1K, flags);
			}
			else if (fault_address >= _RAMBAR1 && fault_address <= _RAMBAR1 + (uint32_t) _RAMBAR1_SIZE)
			{
				mmu_map_page(fault_address & 0xfffff400, fault_address & 0xfffff400, MMU_PAGE_SIZE_1K, flags);
			}
			else if (fault_address >= _SYS_SRAM && fault_address <= _SYS_SRAM + (uint32_t) _SYS_SRAM_SIZE)
			{
				mmu_map_page(fault_address & 0xfffff400, fault_address & 0xfffff400, MMU_PAGE_SIZE_1K, flags);
			}
			else if (fault_address >= FASTRAM_END)
			{
				is_tlb_miss = false;	/* this is a bus error */
			}
			else	/* map this page */
			{
				mmu_map_page(fault_address & 0xfff00000, fault_address & 0xfff00000,
							MMU_PAGE_SIZE_1M, flags);
				return true;
			}
		}
	}
	return false;
}


void mmu_map_page(uint32_t virt, uint32_t phys, uint32_t map_size, uint32_t map_flags)
{
	dbg("%s: map virt=%p to phys=%p\r\n", __FUNCTION__, virt, phys);

	/*
	 * add page to TLB
	 */
	MCF_MMU_MMUTR = virt |					/* virtual address */
					MCF_MMU_MMUTR_SG |		/* shared global */
					MCF_MMU_MMUTR_V;		/* valid */

	MCF_MMU_MMUDR = phys |					/* physical address */
					MCF_MMU_MMUDR_SZ(map_size) |	/* 1 MB page size */
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



