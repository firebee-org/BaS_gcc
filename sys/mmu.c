#include "mmu.h"
#include "acia.h"
#include "exceptions.h"
#include "pci.h"

#define DEBUG
#include "debug.h"

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
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error "unknown machine!"x
#endif /* MACHINE_FIREBEE */

/*
 * set ASID register
 * saves new value to rt_asid and returns former value
 */
inline uint32_t set_asid(uint32_t value)
{
    extern long rt_asid;
    uint32_t ret = rt_asid;

    __asm__ __volatile__(
        "movec      %[value],ASID\n\t"
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
        "movec      %[value],ACR0\n\t"
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
        "movec      %[value],ACR1\n\t"
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
        "movec      %[value],ACR2\n\t"
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
        "movec      %[value],ACR3\n\t"
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
        "movec      %[value],MMUBAR\n\t"
        : /* no output */
        : [value] "r" (value)
        : /* no clobber */
    );
    rt_mmubar = value;
    NOP();

    return ret;
}


/*
 * translation table for virtual address ranges. Holds the physical_offset (which must be added to a virtual
 * address to get its physical counterpart) for memory ranges.
 */
struct virt_to_phys
{
    uint32_t start_address;
    uint32_t length;
    uint32_t physical_offset;
};

#if defined(MACHINE_FIREBEE)
static struct virt_to_phys translation[] =
{
    /* virtual  , length    , offset    */
    { 0x00000000, 0x00e00000, 0x60000000 },     /* map first 14 MByte to first 14 Mb of video ram */
    { 0x00e00000, 0x00100000, 0x00000000 },     /* map TOS to SDRAM */
    { 0x00f00000, 0x00100000, 0xff000000 },     /* map Falcon I/O area to FPGA */
    { 0x01000000, 0x1f000000, 0x00000000 },     /* map rest of ram virt = phys */
};
#elif defined(MACHINE_M5484LITE)
static struct virt_to_phys translation[] =
{
    /* virtual  , length    , offset    */
    { 0x00000000, 0x00e00000, 0x00000000 },     /* map first 14 MByte to first 14 Mb of SD ram */
    { 0x00e00000, 0x00100000, 0x00000000 },     /* map TOS to SDRAM */
    { 0x01000000, 0x04000000, 0x00000000 },     /* map rest of ram virt = phys */
#if 0
    { 0x04000000, 0x08000000, 0x7C000000 },     /* experimental mapping for PCI memory */
#endif
};
#elif defined(MACHINE_M54455)
/* FIXME: this is not determined yet! */
static struct virt_to_phys translation[] =
{
    /* virtual  , length    , offset    */
    { 0x00000000, 0x00e00000, 0x60000000 },     /* map first 14 MByte to first 14 Mb of video ram */
    { 0x00e00000, 0x00100000, 0x00000000 },     /* map TOS to SDRAM */
    { 0x00f00000, 0x00100000, 0xff000000 },     /* map Falcon I/O area to FPGA */
    { 0x01000000, 0x1f000000, 0x00000000 },     /* map rest of ram virt = phys */
};
#else
#error unknown machine!
#endif

static int num_translations = sizeof(translation) / sizeof(struct virt_to_phys);

static inline uint32_t lookup_phys(int32_t virt)
{
    int i;

    for (i = 0; i < num_translations; i++)
    {
        if (virt >= translation[i].start_address && virt < translation[i].start_address + translation[i].length)
        {
            return virt + translation[i].physical_offset;
        }
    }
    err("virtual address 0x%lx not found in translation table!\r\n", virt);

    return -1;
}


/*
 * page descriptors. Size depending on DEFAULT_PAGE_SIZE, either 1M (resulting in 512
 * bytes size) or 8k pages (64k descriptor array size)
 */
#define NUM_PAGES    (SDRAM_SIZE / SIZE_DEFAULT)
static struct mmu_page_descriptor_ram pages[NUM_PAGES];


int mmu_map_instruction_page(uint32_t virt, uint8_t asid)
{
    const uint32_t size_mask = ~ (SIZE_DEFAULT - 1);                /* pagesize */
    int page_index = (virt & size_mask) / SIZE_DEFAULT;             /* index into page_descriptor array */
    struct mmu_page_descriptor_ram *page = &pages[page_index];      /* attributes of page to map */
    int ipl;
    uint32_t phys = lookup_phys(virt);                               /* virtual to physical translation of page */

    if (phys == (uint32_t) -1)
    {
        /* no valid mapping found, caller will issue a bus error in return */
        dbg("no mapping found\r\n");
        return 0;
    }

#ifdef DBG_MMU
    register int sp asm("sp");
    dbg("page_descriptor: 0x%02x, ssp = 0x%08x\r\n", * (uint8_t *) page, sp);
#endif /* DBG_MMU */

    /*
     * add page to TLB
     */

    ipl = set_ipl(7);                                               /* do not disturb */

    MCF_MMU_MMUAR = (virt & size_mask);

    MCF_MMU_MMUTR = (virt & size_mask) |                            /* virtual address */
        MCF_MMU_MMUTR_ID(asid) |                                    /* address space id (ASID) */
        (page->global ? MCF_MMU_MMUTR_SG : 0) |                     /* shared global */
        MCF_MMU_MMUTR_V;                                            /* valid */

    MCF_MMU_MMUDR = (phys & size_mask) |                            /* physical address */
        MCF_MMU_MMUDR_SZ(MMU_PAGE_SIZE_DEFAULT) |                   /* page size */
        MCF_MMU_MMUDR_CM(page->cache_mode) |                        /* cache mode */
        (page->supervisor_protect ? MCF_MMU_MMUDR_SP : 0) |         /* supervisor protect */
        (page->read ? MCF_MMU_MMUDR_R : 0) |                        /* read access enable */
        (page->write ? MCF_MMU_MMUDR_W : 0) |                       /* write access enable */
        (page->execute ? MCF_MMU_MMUDR_X : 0) |                     /* execute access enable */
        (page->locked ? MCF_MMU_MMUDR_LK : 0);

    MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB |    /* instruction */
        MCF_MMU_MMUOR_ACC |                 /* access TLB */
        MCF_MMU_MMUOR_UAA;                  /* update allocation address field */

    set_ipl(ipl);

    dbg("mapped virt=0x%08x to phys=0x%08x\r\n", virt & size_mask, phys & size_mask);

    dbg("ITLB: MCF_MMU_MMUOR = %08x\r\n", MCF_MMU_MMUOR);

    return 1;
}

struct mmu_page_descriptor_ram pci_descriptor =
{
    .cache_mode = CACHE_COPYBACK,
    .supervisor_protect = 0,
    .read = 1,
    .write = 1,
    .execute = 1,
    .global = 1,
    .locked = 0
};

int mmu_map_data_page(uint32_t virt, uint8_t asid)
{
    uint16_t ipl;
    const uint32_t size_mask = ~ (SIZE_DEFAULT - 1);            /* pagesize */
    int page_index = (virt & size_mask) / SIZE_DEFAULT;         /* index into page_descriptor array */
    struct mmu_page_descriptor_ram *page;
    uint32_t phys = 0L;

    if (page_index < sizeof(pages) / sizeof(struct mmu_page_descriptor_ram))
    {
        page = &pages[page_index];  /* attributes of page to map */
        phys = lookup_phys(virt);                           /* virtual to physical translation of page */
    }

    /*
     * check if we are trying to access PCI space
     */
    else if (virt >= PCI_MEMORY_OFFSET && virt <= PCI_MEMORY_OFFSET + PCI_MEMORY_SIZE)
    {
        phys = virt;
        page = &pci_descriptor;
    };



    if (phys == (uint32_t) -1)
    {
        /* no valid mapping found, caller will issue a bus error in return */
        dbg("no mapping found\r\n");
        return 0;
    }

#ifdef DEBUG
    register int sp asm("sp");
    dbg("page_descriptor: 0x%02x, ssp = 0x%08x\r\n", * (uint8_t *) page, sp);
#endif /* DBG_MMU */

    /*
     * add page to TLB
     */

    ipl = set_ipl(7);                                               /* do not disturb */

    MCF_MMU_MMUTR = (virt & size_mask) |                            /* virtual address */
        MCF_MMU_MMUTR_ID(asid) |                                    /* address space id (ASID) */
        (page->global ? MCF_MMU_MMUTR_SG : 0) |                     /* shared global */
        MCF_MMU_MMUTR_V;                                            /* valid */

    MCF_MMU_MMUDR = (phys & size_mask) |                            /* physical address */
        MCF_MMU_MMUDR_SZ(MMU_PAGE_SIZE_DEFAULT) |                   /* page size */
        MCF_MMU_MMUDR_CM(page->cache_mode) |                        /* cache mode */
        (page->supervisor_protect ? MCF_MMU_MMUDR_SP : 0) |         /* supervisor protect */
        (page->read ? MCF_MMU_MMUDR_R : 0) |                        /* read access enable */
        (page->write ? MCF_MMU_MMUDR_W : 0) |                       /* write access enable */
        (page->execute ? MCF_MMU_MMUDR_X : 0) |                     /* execute access enable */
        (page->locked ? MCF_MMU_MMUDR_LK : 0);

    MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |     /* access TLB, data */
        MCF_MMU_MMUOR_UAA;                  /* update allocation address field */

    set_ipl(ipl);

    dbg("mapped virt=0x%08x to phys=0x%08x\r\n", virt & size_mask, phys & size_mask);

    dbg("DTLB: MCF_MMU_MMUOR = %08x\r\n", MCF_MMU_MMUOR);

    return 1;
}

/*
 * map a page of memory using virt and phys as addresses with the Coldfire MMU.
 *
 * Theory of operation: the Coldfire MMU in the Firebee has 64 TLB entries, 32 for data (DTLB), 32 for
 * instructions (ITLB). Mappings can either be done locked (normal MMU TLB misses will not consider them
 * for replacement) or unlocked (mappings will reallocate using a LRU scheme when the MMU runs out of
 * TLB entries). For proper operation, the MMU needs at least two ITLBs and/or four free/allocatable DTLBs
 * per instruction as a minimum, more for performance. Thus locked pages (that can't be touched by the
 * LRU algorithm) should be used sparsingly.
 */
uint32_t mmu_map_page(uint32_t virt, uint32_t phys, enum mmu_page_size sz, uint8_t page_id, const struct mmu_page_descriptor_ram *flags)
{
    int size_mask;
    int ipl;

    switch (sz)
    {
        case MMU_PAGE_SIZE_1M:
            size_mask = ~ (SIZE_1M - 1);
            break;

        case MMU_PAGE_SIZE_8K:
            size_mask = ~ (SIZE_8K - 1);
            break;

        case MMU_PAGE_SIZE_4K:
            size_mask = ~ (SIZE_4K - 1);
            break;

        case MMU_PAGE_SIZE_1K:
            size_mask = ~ (SIZE_1K - 1);
            break;

        default:
            dbg("illegal map size %d\r\n", sz);
            return 0;
    }

    /*
     * add page to TLB
     */

    ipl = set_ipl(7);       /* do not disturb */

    MCF_MMU_MMUTR = (virt & size_mask) |                            /* virtual address */
        MCF_MMU_MMUTR_ID(page_id) |                                 /* address space id (ASID) */
        (flags->global ? MCF_MMU_MMUTR_SG : 0) |                    /* shared global */
        MCF_MMU_MMUTR_V;                                            /* valid */

    MCF_MMU_MMUDR = (phys & size_mask) |                            /* physical address */
        MCF_MMU_MMUDR_SZ(sz) |                                      /* page size */
        MCF_MMU_MMUDR_CM(flags->cache_mode) |
        (flags->read ? MCF_MMU_MMUDR_R : 0) |                       /* read access enable */
        (flags->write ? MCF_MMU_MMUDR_W : 0) |                      /* write access enable */
        (flags->execute ? MCF_MMU_MMUDR_X : 0) |                    /* execute access enable */
        (flags->supervisor_protect ? MCF_MMU_MMUDR_SP : 0) |        /* supervisor protect */
        (flags->locked ? MCF_MMU_MMUDR_LK : 0);

    MCF_MMU_MMUOR = MCF_MMU_MMUOR_ACC |     /* access TLB, data */
        MCF_MMU_MMUOR_UAA;                  /* update allocation address field */
    NOP();

    MCF_MMU_MMUOR = MCF_MMU_MMUOR_ITLB |    /* instruction */
        MCF_MMU_MMUOR_ACC |                 /* access TLB */
        MCF_MMU_MMUOR_UAA;                  /* update allocation address field */

    set_ipl(ipl);

    dbg("mapped virt=0x%08x to phys=0x%08x size mask 0x%lx\r\n", virt, phys, size_mask);

    return 1;
}

void mmu_init(void)
{
    extern uint8_t _MMUBAR[];
    uint32_t MMUBAR = (uint32_t) &_MMUBAR[0];
    struct mmu_page_descriptor_ram flags;
    int i;

    /*
     * clear all MMU TLB entries first
     */
    MCF_MMU_MMUOR = MCF_MMU_MMUOR_CA;       /* clears _all_ TLBs (including locked ones) */
    NOP();

    /*
     * prelaminary initialization of page descriptor 0 (root) table
     */
    for (i = 0; i < NUM_PAGES; i++)
    {
        uint32_t addr = i * SIZE_DEFAULT;

#if defined(MACHINE_FIREBEE)
        if (addr >= 0x00f00000UL && addr < 0x00ffffffUL)        /* Falcon I/O area on the Firebee */
        {
            pages[i].cache_mode = CACHE_NOCACHE_PRECISE;
            pages[i].supervisor_protect = 1;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].execute = 0;
            pages[i].global = 1;
        }
        else if (addr >= 0x0UL && addr < 0x00e00000UL)          /* ST-RAM, potential video memory */
        {
            pages[i].cache_mode = CACHE_WRITETHROUGH;
            pages[i].supervisor_protect = 0; // (addr == 0x0L ? 1 : 0);
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        else if (addr >= 0x00e00000UL && addr < 0x00f00000UL)   /* EmuTOS */
        {
            pages[i].cache_mode = CACHE_COPYBACK;
            pages[i].supervisor_protect = 0;
            pages[i].read = 1;
            pages[i].write = 0;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        else if (addr >= 0x00000000 && addr <= 0x00010000)      /* first Megabyte of ST RAM */
        {
            pages[i].cache_mode = CACHE_COPYBACK;
            pages[i].supervisor_protect = 0;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        else
        {
            pages[i].cache_mode = CACHE_COPYBACK;
            pages[i].supervisor_protect = 0;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        pages[i].locked = 0;                /* not locked */

#elif defined(MACHINE_M5484LITE)
        if (addr >= 0x60000000UL && addr < 0x70000000UL)        /* Compact Flash on the m5484lite */
        {
            pages[i].cache_mode = CACHE_NOCACHE_PRECISE;
            pages[i].supervisor_protect = 0;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        else if (addr >= 0x0UL && addr < 0x00e00000UL)          /* ST-RAM, potential video memory */
        {
            pages[i].cache_mode = CACHE_WRITETHROUGH;
            pages[i].supervisor_protect = 0;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        else if (addr >= 0x00e00000UL && addr < 0x00f00000UL)   /* EmuTOS */
        {
            pages[i].cache_mode = CACHE_COPYBACK;
            pages[i].execute = 1;
            pages[i].supervisor_protect = 1;
            pages[i].read = 1;
            pages[i].write = 0;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        else
        {
            pages[i].cache_mode = CACHE_COPYBACK;               /* rest of RAM */
            pages[i].execute = 1;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].supervisor_protect = 0;
            pages[i].global = 1;
        }
        pages[i].locked = 0;                /* not locked */

#elif defined(MACHINE_M54455)
        if (addr >= 0x60000000UL && addr < 0x70000000UL)        /* Compact Flash on the m5484lite */
        {
            pages[i].cache_mode = CACHE_NOCACHE_PRECISE;
            pages[i].execute = 0;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].execute = 0;
            pages[i].global = 1;
            pages[i].supervisor_protect = 1;
        }
        else if (addr >= 0x0UL && addr < 0x00e00000UL)          /* ST-RAM, potential video memory */
        {
            pages[i].cache_mode = CACHE_WRITETHROUGH;
            pages[i].execute = 1;
            pages[i].supervisor_protect = 0;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        else if (addr >= 0x00e00000UL && addr < 0x00f00000UL)   /* EmuTOS */
        {
            pages[i].cache_mode = CACHE_COPYBACK;
            pages[i].execute = 1;
            pages[i].supervisor_protect = 1;
            pages[i].read = 1;
            pages[i].write = 0;
            pages[i].execute = 1;
            pages[i].global = 1;
        }
        else
        {
            pages[i].cache_mode = CACHE_COPYBACK;               /* rest of RAM */
            pages[i].execute = 1;
            pages[i].read = 1;
            pages[i].write = 1;
            pages[i].supervisor_protect = 0;
            pages[i].global = 1;
        }
        pages[i].locked = 0;                /* not locked */
#else
#error Unknown machine!
#endif /* MACHINE_FIREBEE */
    }

    set_asid(0);                        /* do not use address extension (ASID provides virtual 48 bit addresses */

    // pages[0].supervisor_protect = 1;    /* protect system vectors */

    /* set data access attributes in ACR0 and ACR1 */

    /* map PCI address space */
    /* set SRAM and MBAR access */
    set_acr0(ACR_W(0) |                             /* read and write accesses permitted */
            // ACR_SP(1) |                             /* supervisor only access permitted */
            ACR_CM(ACR_CM_CACHE_INH_PRECISE) |      /* cache inhibit, precise */
            ACR_AMM(0) |                            /* control region > 16 MB */
            ACR_S(ACR_S_SUPERVISOR_MODE) |          /* match addresses in supervisor and user mode */
            ACR_E(1) |                              /* enable ACR */
#if defined(MACHINE_FIREBEE)
            ACR_ADMSK(0x7f) |                       /* cover 2GB area from 0x80000000 to 0xffffffff */
            // ACR_BA(PCI_MEMORY_OFFSET));             /* (equals area from 3 to 4 GB */
            ACR_BA(0xe0000000));
#elif defined(MACHINE_M5484LITE)
            ACR_ADMSK(0x7f) |                       /* cover 2 GB area from 0x80000000 to 0xffffffff */
            // ACR_BA(PCI_MEMORY_OFFSET));
            ACR_BA(0xe0000000));
#elif defined(MACHINE_M54455)
            ACR_ADMSK(0x7f) |
            ACR_BA(0x80000000));                    /* FIXME: not determined yet */
#else
#error unknown machine!
#endif /* MACHINE_FIREBEE */

    /* data access attributes for BaS in flash */

    set_acr1(ACR_W(0) |
            ACR_SP(0) |
            ACR_CM(0) |
#if defined(MACHINE_FIREBEE)
            ACR_CM(ACR_CM_CACHEABLE_WT) |
#elif defined(MACHINE_M5484LITE)
            ACR_CM(ACR_CM_CACHEABLE_WT) |
#elif defined(MACHINE_M54455)
            ACR_CM(ACR_CM_CACHEABLE_WT) |
#else
#error unknown machine!
#endif /* MACHINE_FIREBEE */
            ACR_AMM(0) |
            ACR_S(ACR_S_ALL) |
            ACR_E(1) |
            ACR_ADMSK(0x1f) |
            ACR_BA(0xe0000000));

    /* set instruction access attributes in ACR2 and ACR3 */

    //set_acr2(0xe007c400);

    /* instruction access attribute for BaS in flash */

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

    set_mmubar(MMUBAR + 1);     /* set and enable MMUBAR */

    /*
     * Make the TOS (in SDRAM) read-only
     * This maps virtual 0x00e0'0000 - 0x00ef'ffff to the same virtual address
     */
    flags.cache_mode = CACHE_COPYBACK;
    flags.supervisor_protect = 0;  // needs to stay like this or cf_flasher will choke */
    flags.read = 1;
    flags.write = 0;
    flags.execute = 1;
    flags.locked = 1;
    mmu_map_page(0xe00000, 0xe00000, MMU_PAGE_SIZE_1M, 0, &flags);

#if defined(MACHINE_FIREBEE)
    /*
     * Map FireBee I/O area (0xfff0'0000 - 0xffff'ffff physical) to the Falcon-compatible I/O
     * area (0x00f0'0000 - 0x00ff'ffff virtual) for the FireBee
     */
    flags.cache_mode = CACHE_NOCACHE_PRECISE;
    flags.supervisor_protect = 1;
    flags.read = 1;
    flags.write = 1;
    flags.execute = 0;
    flags.locked = 1;
    mmu_map_page(0x00f00000, 0xfff00000, MMU_PAGE_SIZE_1M, 0, &flags);
#elif defined(MACHINE_M5484LITE)
    /*
     * Map m5484LITE CPLD access
     */
    flags.cache_mode = CACHE_NOCACHE_PRECISE;
    flags.supervisor_protect = 1;
    flags.read = 1;
    flags.write = 1;
    flags.execute = 0;
    flags.locked = 1;
    mmu_map_page(0x6a000000, 0x6a000000, MMU_PAGE_SIZE_1M, 0, &flags);
#elif defined(MACHINE_M54455)
#warning MMU specs for M54455 not yet determined
#else
#error Unknown machine
#endif /* MACHINE_FIREBEE */

    /*
     * Map (locked) the second last MB of physical SDRAM (this is where BaS .data and .bss reside) to the same
     * virtual address. This is also used (completely) when BaS is in RAM
     */
    flags.cache_mode = CACHE_COPYBACK;
    flags.supervisor_protect = 0;
    flags.read = 1;
    flags.write = 1;
    flags.execute = 1;
    flags.locked = 1;
    mmu_map_page(SDRAM_START + SDRAM_SIZE - 0x00200000, SDRAM_START + SDRAM_SIZE - 0x00200000, MMU_PAGE_SIZE_1M, 0, &flags);

    /*
     * Map (locked) the very last MB of physical SDRAM (this is where the driver buffers reside) to the same
     * virtual address. Used uncached for drivers.
     */
    flags.cache_mode = CACHE_NOCACHE_PRECISE;
    flags.supervisor_protect = 0;
    flags.read = 1;
    flags.write = 1;
    flags.execute = 0;
    flags.locked = 1;
    mmu_map_page(SDRAM_START + SDRAM_SIZE - 0x00100000, SDRAM_START + SDRAM_SIZE - 0x00100000, MMU_PAGE_SIZE_1M, 0, &flags);
}


uint32_t mmutr_miss(uint32_t mmu_sr, uint32_t fault_address, uint32_t pc, uint32_t format_status)
{
    uint32_t fault = format_status & 0xc030000;

    //dbg("MMU TLB MISS accessing 0x%08x\r\nFS = 0x%08x\r\nPC = 0x%08x\r\n", fault_address, format_status, pc);
    // flush_and_invalidate_caches();

    switch (fault)
    {
        /* if we have a real TLB miss, map the offending page */

        case 0x04010000:    /* TLB miss on opword of instruction fetch */
        case 0x04020000:    /* TLB miss on extension word of instruction fetch */
            dbg("MMU ITLB MISS accessing 0x%08x\r\n"
                "FS = 0x%08x\r\n"
                "MMUSR = 0x%08x\r\n"
                "PC = 0x%08x\r\n",
                fault_address, format_status, mmu_sr, pc);
            dbg("fault = 0x%08x\r\n", fault);

            if (!mmu_map_instruction_page(pc, 0))
            {
                dbg("ITLB miss bus error\r\n");
                return 1;   /* bus error */
            }

#ifdef _NOT_USED_
            /* due to prefetch, it makes sense to map the next adjacent page also for ITLBs */
            if (pc + SIZE_DEFAULT < TARGET_ADDRESS)
            {
                /*
                 * only do this if the next page is still valid RAM
                 */
                if (!mmu_map_instruction_page(pc + MMU_DEFAULT_PAGE_SIZE, 0))
                {
                    dbg("ITLB next page bus error\r\n");
                    return 1;   /* bus error */
                }
            }
#endif /* _NOT_USED_ */
            break;

        case 0x08020000:    /* TLB miss on data write */
        case 0x0c020000:    /* TLB miss on data read or read-modify-write */
            dbg("MMU DTLB MISS accessing 0x%08x\r\n"
                "FS = 0x%08x\r\n"
                "MMUSR = 0x%08x\r\n"
                "PC = 0x%08x\r\n",
                fault_address, format_status, mmu_sr, pc);
            dbg("fault = 0x%08x\r\n", fault);

            if (!mmu_map_data_page(fault_address, 0))
            {
                dbg("DTLB miss bus error\r\n");
                return 1;   /* bus error */
            }
            break;

        case 0x0c010000:
        case 0x08010000:
            dbg("privilege violation accessing 0x%08x\r\n"
                "FS = 0x%08x\r\n"
                "MMUSR = 0x%08x\r\n"
                "PC = 0x%08x\r\n",
                fault_address, format_status, mmu_sr, pc);
            dbg("fault = 0x%08x\r\n", fault);
#ifdef _DOES_NOT_WORK_
            /*
             * check if its one of our "special cases" and map a user page on top of it if user
             * mode access should be allowed
             */
            if (fault_address >= 1024 && fault_address < 0x00100000) /* ST-RAM */
            {
                struct mmu_page_descriptor flags =
                {
                    .cache_mode = CACHE_COPYBACK,
                    .supervisor_protect = 0,
                    .read = 1,
                    .write = 1,
                    .execute = 1,
                    .global = 1,
                    .locked = 0
                };

                uint32_t virt = fault_address & ~(SIZE_1K - 1);
                uint32_t phys = (fault_address & (~(SIZE_1K - 1))) + 0x60000000;
                dbg("mapping helper page virt=0x%08x to phys=0x%08x\r\n", virt, phys);
                if (!mmu_map_page(virt, phys, MMU_PAGE_SIZE_1K, 0, &flags))
                {
                    dbg("privilege violation (bus error)\r\n");
                    return 1;
                }
            }
#endif
            return 1;
            break;

        /* else issue a bus error */
        default:
            dbg("bus error accessing 0x%08x\r\n"
                "FS = 0x%08x\r\n"
                "MMUSR = 0x%08x\r\n"
                "PC = 0x%08x\r\n",
                fault_address, format_status, mmu_sr, pc);
            dbg("fault = 0x%08x\r\n", fault);
            return 1;       /* signal bus error to caller */
    }
#ifdef DBG_MMU
    xprintf("\r\n");
#endif /* DBG_MMU */

    return 0;   /* signal TLB miss handled to caller */
}


/* TODO: implement */

/*
 * API-exposed, externally callable MMU functions
 */


/*
 * lock data page(s) with address space id asid from address virt to virt + size.
 *
 * ASID probably needs an explanation - this is the "address space id" managed by
 * the MMU.
 * If its value range would be large enough, this could directly map to a PID
 * in MiNT. Unfortunately, the Coldfire MMU only allows an 8 bit value for ASID
 * (with 0 already occupied for the super user/root process and the Firebee video
 * subsystem occupying another one), so we are left with 253 distinct values.
 * MMU software needs to implement some kind of mapping and LRU scheme which will
 * lead to a throwaway of all mappings for processes not seen for a while (and thus
 * to undeterministic response/task switching times when such processes are activated
 * again).
 *
 * FIXME: There is no check for "too many locked pages", currently.
 *
 * return: 0 if failed (page not in translation table), 1 otherwise
 */
uint32_t mmu_map_data_page_locked(uint32_t virt, uint32_t size, int asid)
{
    const uint32_t size_mask = ~ (SIZE_DEFAULT - 1);       /* pagesize */
    int page_index = (virt & size_mask) / SIZE_DEFAULT;    /* index into page_descriptor array */
    struct mmu_page_descriptor_ram *page = &pages[page_index];          /* attributes of page to map */
    int i = 0;

    while (page_index * SIZE_DEFAULT < virt + size)
    {
        if (page->locked)
        {
            dbg("page at %p is already locked. Nothing to do\r\n", virt);
        }
        else
        {
            page->locked = 1;
            mmu_map_data_page(virt, 0);
            i++;
        }
        virt += SIZE_DEFAULT;
    }

    dbg("%d pages locked\r\n", i);

    return 1;   /* success */
}

/*
 * the opposite: unlock data page(s) with address space id asid from address virt to virt + size_t
 *
 * return: 0 if failed (page not found), 1 otherwise
 */
uint32_t mmu_unlock_data_page(uint32_t address, uint32_t size, int asid)
{
    int curr_asid;
    const uint32_t size_mask = ~ (SIZE_DEFAULT - 1);
    int page_index = (address & size_mask) / SIZE_DEFAULT;     /* index into page descriptor array */
    struct mmu_page_descriptor_ram *page = &pages[page_index];

    curr_asid = set_asid(asid);         /* set asid to the one to search for */

    /* TODO: check for pages[] array bounds */

    while (page_index * SIZE_DEFAULT < address + size)
    {
        MCF_MMU_MMUAR = address + page->supervisor_protect;
        MCF_MMU_MMUOR = MCF_MMU_MMUOR_STLB |        /* search TLB */
                MCF_MMU_MMUOR_ADR |
                MCF_MMU_MMUOR_RW;
        if (MCF_MMU_MMUSR & MCF_MMU_MMUSR_HIT)      /* found */
        {
#ifdef DEBUG
            uint32_t tlb_aa = MCF_MMU_MMUOR >> 16;  /* MMU internal allocation address for TLB */
#endif /* DBG_MMU */

            MCF_MMU_MMUDR &= ~MCF_MMU_MMUDR_LK;     /* clear lock bit */
            MCF_MMU_MMUOR = MCF_MMU_MMUOR_UAA |
                    MCF_MMU_MMUOR_ACC;              /* update TLB */

            dbg("DTLB %d unlocked\r\n", tlb_aa);
        }
        else
        {
            dbg("%p doesn't seem to be locked??\r\n");
        }
        page_index++;
    }
    set_asid(curr_asid);

    return 1;   /* success */
}

uint32_t mmu_report_locked_pages(uint32_t *num_itlb, uint32_t *num_dtlb)
{
    int i;
    int li = 0;
    int ld = 0;

    /* Coldfire V4e allocation addresses run from 0 to 63 */

    for (i = 0; i < 31; i++)        /* 0-31 = ITLB AA */
    {
        MCF_MMU_MMUAR = i;
        MCF_MMU_MMUOR = MCF_MMU_MMUOR_STLB |
                MCF_MMU_MMUOR_ITLB |
                MCF_MMU_MMUOR_RW;                   /* search ITLB */

        if (MCF_MMU_MMUTR & MCF_MMU_MMUTR_V)
        {
            /* entry is valid */
            if (MCF_MMU_MMUDR & MCF_MMU_MMUDR_LK)
            {
                li++;
            }
        }

    }
    for (i = 32; i < 64; i++)       /* 32-63 = DTLB AA */
    {
        MCF_MMU_MMUAR = i;
        MCF_MMU_MMUOR = MCF_MMU_MMUOR_STLB |
                MCF_MMU_MMUOR_RW;                   /* search ITLB */

        if (MCF_MMU_MMUTR & MCF_MMU_MMUTR_V)
        {
            /* entry is valid */
            if (MCF_MMU_MMUDR & MCF_MMU_MMUDR_LK)
            {
                ld++;
            }
        }
    }

    *num_itlb = li;
    *num_dtlb = ld;

    return 1;   /* success */
}

uint32_t mmu_report_pagesize(void)
{
    return SIZE_DEFAULT;
}

