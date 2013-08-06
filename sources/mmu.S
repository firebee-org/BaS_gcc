/*
 * INIT ACR and MMU
 */

#include "startcf.h"

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
// area that in turn cause a TLB miss exception themself after the MMU is enabled. At least the exception handler must live
// in an area that's either covered by one of the ACR's or a locked MMU TLB entry.
//
.text
_mmu_init:
		move.l	d3,-(sp)			// Backup registers
		move.l	d2,-(sp)

 		clr.l	d0
		movec	d0,ASID				// ASID always 0
		move.l	d0,_rt_asid			// save shadow register
		
		move.l	#0xC03FC040,d0		// data r/w precise  c000'0000-ffff'ffff
		movec	d0,ACR0
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

// 0000'0000 locked
  		moveq.l	#0x00000000|std_mmutr,d0
  		moveq.l	#0x00000000|copyback_mmudr|MCF_MMU_MMUDR_LK,d1
		moveq.l	#mmuord_d,d2				// MMU update data
		moveq.l	#mmuord_i,d3				// MMU update instruction
 		move.l	d0,MCF_MMU_MMUTR		
 		move.l	d1,MCF_MMU_MMUDR
 		move.l	d2,MCF_MMU_MMUOR			// MMU update data
 		move.l	d3,MCF_MMU_MMUOR			// MMU update instruction

//---------------------------------------------------------------------------------------
// 00d0'0000 locked ID=6
// video ram: read write execute normal write true
 		move.l	#0x00d00000|MCF_MMU_MMUTR_ID(sca_page_ID)|std_mmutr,d0
		move.l	#0x60d00000|writethrough_mmudr|MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR						// MMU update data
 		move.l	#0x00d00000|std_mmutr,d0
		move.l	d3,MCF_MMU_MMUOR						// MMU update instruction  
		
		move.l	#0x2000,d0
		move.l	d0,_video_tlb							// set page as video page
  		clr.l	_video_sbt								// clear time


//-------------------------------------------------------------------------------------
// Make the TOS (in SDRAM) read-only
 		move.l	#__TOS+std_mmutr,d0
		move.l	#__TOS+copyback_mmudr+MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
 		move.l	d2,MCF_MMU_MMUOR						// setzen read only ?????? noch nicht
		move.l	d3,MCF_MMU_MMUOR						// setzen

// 00f0'0000 locked		
		move.l	#0x00f00000|std_mmutr,d0
		move.l	#0xfff00000|nocache_precise_mmudr|MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR						// mapped to ffffxxx, precise,
		move.l	d3,MCF_MMU_MMUOR						// mapped to ffffxxx, precise,

// 1fe0'0000 locked
		move.l	#0x1E000000|std_mmutr,d0
		move.l	#0x1E000000|copyback_mmudr|MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR						// setzen data
		move.l	d3,MCF_MMU_MMUOR						// setzen instr

// 1ff0'0000 locked (FIXME: why is this?)
		move.l	#0x1FF00000|std_mmutr,d0
		move.l	#0x1FF00000|copyback_mmudr|MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d2,MCF_MMU_MMUOR						// setzen data
		move.l	d3,MCF_MMU_MMUOR						// setzen instr 
// instr 0xFFF0'0000 nach 0x1FF0'0000 umleiten -->> short sprung 
/*		move.l	#0xFFF00000|std_mmutr,d0
		move.l	#0x1FF00000|copyback_mmudr|MCF_MMU_MMUDR_LK,d1
		move.l	d0,MCF_MMU_MMUTR		
		move.l	d1,MCF_MMU_MMUDR
		move.l	d3,MCF_MMU_MMUOR						// setzen instr
*/
		move.l	(sp)+,d2			// Restore registers
		move.l	(sp)+,d3
		rts

/*
 * MMU table add on miss
 */
_mmutr_miss:
		lea		-8 * 4(sp),sp
		movem.l	d0-d3/a0-a3,(sp)

		move.l	d0,-(sp)
		pea		MISS_text
		jsr		_xprintf
		addq.l	#8,sp

		movem.l	(sp),d0-d3/a0-a3
		lea		8 * 4(sp),sp

		bsr		cpusha				// clear caches

		and.l	#0xFFF00000,d0		// d0 is the address not found (MMUAR at the time of the exception)
		or.l	#std_mmutr,d0		// mark shared and valid
		move.l	d0,MCF_MMU_MMUTR	// add to TLB
		and.l	#0xFFF00000,d0		// mask out page
		or.l	#copyback_mmudr,d0	// 1MB page size, cachable copyback, read, write, execute
		move.l	d0,MCF_MMU_MMUDR	// add to TLB
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
