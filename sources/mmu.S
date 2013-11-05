/*
 * INIT ACR and MMU
 */

#include "startcf.h"
#ifdef MACHINE_FIREBEE
#include "firebee.h"
#endif /* MACHINE_FIREBEE */
#ifdef MACHINE_M5484LITE
#include "m5484l.h"
#endif /* MACHINE_M5484LITE */

	.extern _rt_vbr
	.extern _rt_cacr
	.extern _rt_asid
	.extern _rt_acr0
	.extern _rt_acr1
	.extern _rt_acr2
	.extern _rt_acr3
	.extern _rt_mmubar
	.extern ___MMUBAR
	.extern cpusha
	.extern _video_tlb
	.extern _video_sbt
	.extern __TOS

/* Register read/write macros */
#define MCF_MMU_MMUCR	__MMUBAR
#define MCF_MMU_MMUOR	__MMUBAR+0x04
#define MCF_MMU_MMUSR	__MMUBAR+0x08
#define MCF_MMU_MMUAR	__MMUBAR+0x10
#define MCF_MMU_MMUTR	__MMUBAR+0x14
#define MCF_MMU_MMUDR	__MMUBAR+0x18


/* Bit definitions and macros for MCF_MMU_MMUCR */
#define MCF_MMU_MMUCR_EN                     (0x1)
#define MCF_MMU_MMUCR_ASM                    (0x2)

/* Bit definitions and macros for MCF_MMU_MMUOR */
#define MCF_MMU_MMUOR_UAA                    (0x1)
#define MCF_MMU_MMUOR_ACC                    (0x2)
#define MCF_MMU_MMUOR_RW                     (0x4)
#define MCF_MMU_MMUOR_ADR                    (0x8)
#define MCF_MMU_MMUOR_ITLB                   (0x10)
#define MCF_MMU_MMUOR_CAS                    (0x20)
#define MCF_MMU_MMUOR_CNL                    (0x40)
#define MCF_MMU_MMUOR_CA                     (0x80)
#define MCF_MMU_MMUOR_STLB                   (0x100)
#define MCF_MMU_MMUOR_AA(x)                  (((x)&0xFFFF)<<0x10)

/* Bit definitions and macros for MCF_MMU_MMUSR */
#define MCF_MMU_MMUSR_HIT                    (0x2)
#define MCF_MMU_MMUSR_WF                     (0x8)
#define MCF_MMU_MMUSR_RF                     (0x10)
#define MCF_MMU_MMUSR_SPF                    (0x20)

/* Bit definitions and macros for MCF_MMU_MMUAR */
#define MCF_MMU_MMUAR_FA(x)                  (((x)&0xFFFFFFFF)<<0)

/* Bit definitions and macros for MCF_MMU_MMUTR */
#define MCF_MMU_MMUTR_V                      (0x1)
#define MCF_MMU_MMUTR_SG                     (0x2)
#define MCF_MMU_MMUTR_ID(x)                  (((x)&0xFF)<<0x2)
#define MCF_MMU_MMUTR_VA(x)                  (((x)&0x3FFFFF)<<0xA)

/* Bit definitions and macros for MCF_MMU_MMUDR */
#define MCF_MMU_MMUDR_LK                     (0x2)
#define MCF_MMU_MMUDR_X                      (0x4)
#define MCF_MMU_MMUDR_W                      (0x8)
#define MCF_MMU_MMUDR_R                      (0x10)
#define MCF_MMU_MMUDR_SP                     (0x20)
#define MCF_MMU_MMUDR_CM(x)                  (((x)&0x3)<<0x6)
#define MCF_MMU_MMUDR_SZ(x)                  (((x)&0x3)<<0x8)
#define MCF_MMU_MMUDR_PA(x)                  (((x)&0x3FFFFF)<<0xA)

#define	 std_mmutr	(MCF_MMU_MMUTR_SG|MCF_MMU_MMUTR_V)
#define	 mmuord_d	(                   MCF_MMU_MMUOR_ACC|MCF_MMU_MMUOR_UAA)
#define	 mmuord_i	(MCF_MMU_MMUOR_ITLB|MCF_MMU_MMUOR_ACC|MCF_MMU_MMUOR_UAA)
#define	 writethrough_mmudr		(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(00)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
#define	 copyback_mmudr			(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(01)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)
#define	 nocache_precise_mmudr	(MCF_MMU_MMUDR_SZ(00)|MCF_MMU_MMUDR_CM(10)|MCF_MMU_MMUDR_R|MCF_MMU_MMUDR_W|MCF_MMU_MMUDR_X)

		.global _mmu_init
		.global _mmutr_miss

//
// to avoid chicken and egg situations, we need to make sure that MMU TLB miss exceptions do not end up in a memory
// area that in turn cause a TLB miss exception themself after the MMU is enabled. At least the exception handler
// must live in an area that's either covered by one of the ACR's or a locked MMU TLB entry. This is especially
// important when we link BaS for RAM.
//

// some ACR bit defines upfront
#define ACR_BA(x)					((x) & 0xffff0000)
#define ACR_ADMSK(x)				(((x) & 0xffff) << 16)
#define ACR_E(x)					(((x) & 1) << 15)

#define ACR_S(x)					(((x) & 3) << 13)
#define ACR_S_USERMODE				0
#define ACR_S_SUPERVISOR_MODE		1
#define ACR_S_ALL					2

#define ACR_AMM(x)					(((x) & 1) << 10)

#define	ACR_CM(x)					(((x) & 3) << 5)
#define	ACR_CM_CACHEABLE_WT			0x0
#define ACR_CM_CACHEABLE_CB			0x1
#define ACR_CM_CACHE_INH_PRECISE	0x2
#define ACR_CM_CACHE_INH_IMPRECISE	0x3

#define ACR_SP(x)			(((x) & 1) << 3)
#define ACR_W(x)			(((x) & 1) << 2)

.text
_mmu_init:
		move.l	d3,-(sp)			// Backup registers
		move.l	d2,-(sp)

 		clr.l	d0
		movec	d0,ASID				// ASID always 0
		move.l	d0,_rt_asid			// save shadow register

		move.l	#0 |\
				ACR_W(0) |			/* read and write accesses permitted */ \
				ACR_SP(0) |			/* supervisor AND user mode access permitted */ \
				ACR_CM(ACR_CM_CACHE_INH_PRECISE) |	/* cache inhibit, precise */ \
				ACR_AMM(0) |		/* control region > 16M */ \
				ACR_S(ACR_S_ALL) |	/* match addresses in user AND supervisor mode */ \
				ACR_E(1) |			/* enable ACR */ \
				ACR_ADMSK(0x3f) |	/* cover 1 GB area from 0xC0000000 to 0xFFFFFFFF */ \
				ACR_BA(0xC0000000),d0	/* (equals area from 3 to 4 GB) */
		movec	d0,ACR0				// ACR0 covers data
		move.l	d0,_rt_acr0			// save shadow register
		
		move.l	#0x601FC000,d0		// data r/w wt 6000'0000-7fff'ffff
		movec	d0,ACR1
		move.l	d0,_rt_acr1			// save shadow register
		
		move.l	#0xe007C400,d0		// instruction r wt e000'0000-e07f'ffff
		movec	d0,ACR2
		move.l	d0,_rt_acr2			// save shadow register

		clr.l	d0					// acr3 off
		movec	d0,ACR3
		move.l	d0,_rt_acr3			// save shadow register

		move.l	#__MMUBAR+1,d0
		movec	d0,MMUBAR			// set MMUBAR
		move.l	d0,_rt_mmubar		// save shadow register
		
		nop
		
		move.l	#MCF_MMU_MMUOR_CA,d0	// clear all entries, 
		move.l	d0,MCF_MMU_MMUOR		
		nop

// 0000'0000 - 000F'FFFF (first megabyte of physical memory) locked virtual = physical
  		moveq.l	#0x00000000|std_mmutr,d0
  		moveq.l	#0x00000000|copyback_mmudr|MCF_MMU_MMUDR_LK,d1
		moveq.l	#mmuord_d,d2				// MMU update data
		moveq.l	#mmuord_i,d3				// MMU update instruction
 		move.l	d0,MCF_MMU_MMUTR		
 		move.l	d1,MCF_MMU_MMUDR
 		move.l	d2,MCF_MMU_MMUOR			// MMU update data
 		move.l	d3,MCF_MMU_MMUOR			// MMU update instruction

// 00D0'0000 - 0x00DF'FFFF (last megabyte of ST RAM = Falcon video memory) locked ID=6
// mapped to physical address 60D0'0000 (FPGA video memory)
// video ram: read write execute normal write true
#if MACHINE_FIREBEE
 		move.l	#0x00d00000|MCF_MMU_MMUTR_ID(sca_page_ID)|std_mmutr,d0
		move.l	#0x60d00000|writethrough_mmudr|MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR			// MMU update data
 		move.l	#0x00d00000|std_mmutr,d0
		move.l	d3,MCF_MMU_MMUOR			// MMU update instruction  
		
		move.l	#0x2000,d0
		move.l	d0,_video_tlb				// set page as video page
  		clr.l	_video_sbt					// clear time
#endif /* MACHINE_FIREBEE */


// Make the TOS (in SDRAM) read-only
// this maps virtual 0x00E0'0000-0x00EF'FFFF to the same physical address
 		move.l	#__TOS+std_mmutr,d0
		move.l	#__TOS+copyback_mmudr+MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
 		move.l	d2,MCF_MMU_MMUOR						// setzen read only ?????? noch nicht
		move.l	d3,MCF_MMU_MMUOR						// setzen

// 00f0'0000 locked		
// this maps virtual 0x00F0'0000 - 0x00FF'FFFF to physical 0xFFF0'0000 - 0xFFFF'FFFF effectively making I/O area
// accesses ST-compatible (just the same what Atari made for TT and Falcon). This does not get mapped for the
// m5484LITE boards which enables us to later act on I/O access attempts during a page miss exception
#if MACHINE_FIREBEE
		move.l	#0x00f00000|std_mmutr,d0
		move.l	#0xfff00000|nocache_precise_mmudr|MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR						// mapped to ffffxxx, precise,
		move.l	d3,MCF_MMU_MMUOR						// mapped to ffffxxx, precise,
#elif MACHINE_M5484LITE
//
// Instead, we lock PCI address space. Uncached, precise. For now, only for the M5484LITE
//
		move.l	#0x80000000|std_mmutr,d0
		move.l	#0x80000000|nocache_precise_mmudr|MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR						// mapped to ffffxxx, precise,
		move.l	d3,MCF_MMU_MMUOR						// mapped to ffffxxx, precise,
#endif /* MACHINE_FIREBEE */

// maps (locked) the last MB (this is where BaS .data and .bss resides) of physical SDRAM to the same physical address
		move.l	#(SDRAM_START + SDRAM_SIZE - 0x100000) | std_mmutr, d0	
		move.l	#(SDRAM_START + SDRAM_SIZE - 0x100000) | copyback_mmudr | MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR						// setzen data
		move.l	d3,MCF_MMU_MMUOR						// setzen instr

		move.l	(sp)+,d2			// Restore registers
		move.l	(sp)+,d3
		rts

/*
 * Everything else (that is not filtered out in the access error handler) gets a 1:1 mapping on miss
 */
_mmutr_miss:
		lea		-4 * 4(sp),sp
		movem.l	d0-d1/a0-a1,(sp)	// save gcc scratch registers

		move.l	d0,-(sp)
		pea		MISS_text
		jsr		_xprintf
		addq.l	#8,sp

		movem.l	(sp),d0-d1/a0-a1	// restore registers
		lea		4 * 4(sp),sp

		bsr		cpusha				// clear caches

		and.l	#0xFFF00000,d0		// d0 is the address not found (MMUAR at the time of the exception)
		or.l	#std_mmutr,d0		// mark shared and valid
		move.l	d0,MCF_MMU_MMUTR	// add to TLB
		and.l	#0xFFF00000,d0		// mask out page
		or.l	#copyback_mmudr,d0	// 1MB page size, cachable copyback, read, write, execute
		move.l	d0,MCF_MMU_MMUDR	// add physical address to TLB
		moveq.l	#mmuord_d,d0		// MMU update data
		move.l	d0,MCF_MMU_MMUOR	// set
		moveq.l	#mmuord_i,d0		// MMU update instruction
		move.l	d0,MCF_MMU_MMUOR	// set

		move.l	(sp)+,d0			// restore register saved in acess
		rte

		.data
MISS_text:
		.ascii	"MMU TLB MISS at %p"
		.byte	13, 10, 0
