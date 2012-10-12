/*
 *    CF_Startup.c - Default init/startup/termination routines for
 *                     Embedded Metrowerks C++
 *
 *    Copyright � 1993-1998 Metrowerks, Inc. All Rights Reserved.
 *    Copyright � 2005 Freescale semiConductor Inc. All Rights Reserved.
 *
 *
 *    THEORY OF OPERATION
 *
 *    This version of thestartup code is intended for linker relocated
 *    executables.  The startup code will assign the stack pointer to
 *    __SP_INIT, assign the address of the data relative base address
 *    to a5, initialize the .bss/.sbss sections to zero, call any
 *    static C++ initializers and then call main.  Upon returning from
 *    main it will call C++ destructors and call exit to terminate.
 */

#ifdef __cplusplus
#pragma cplusplus off
#endif
#pragma PID off
#pragma PIC off

#include <MCF5475.h>


	/* imported data */

extern unsigned long far _SP_INIT, _SDA_BASE;
extern unsigned long far _START_BSS, _END_BSS;
extern unsigned long far _START_SBSS, _END_SBSS;
extern unsigned long far __DATA_RAM, __DATA_ROM, __DATA_END;
extern unsigned long far __Bas_base;

extern unsigned long far __SUP_SP,__BOOT_FLASH;
extern unsigned long far rt_mbar;

	/* imported routines */

extern int BaS(int, char **);

	/* exported routines */
extern void __initialize_hardware(void);
extern void init_slt(void);


void _startup(void)
{
	asm("\n\t"
	"bra			warmstart\n\t"
	"jmp			__BOOT_FLASH + 8					// ist zugleich reset vector\n\t"
	"/* disable interrupts */\n\t"
"warmstart:\n\t"
	"// disable interrupts\n\t"
	"move.w     	#0x2700,sr\n\t"
	"// Initialize MBAR\n\t"
	"MOVE.L		#__MBAR,D0\n\t"
	"MOVEC		D0,MBAR\n\t"
	"MOVE.L		D0,rt_mbar\n\t"
"// mmu off\n\t"
	"move.l		#__MMUBAR+1,d0\n\t"
	movec		d0,MMUBAR				//mmubar setzen
	clr.l		d0
	move.l		d0,MCF_MMU_MMUCR		// mmu off
        /* Initialize RAMBARs: locate SRAM and validate it */ \
   	move.l 		#__RAMBAR0 + 0x7,d0		// supervisor only
   	movec      	d0,RAMBAR0
   	move.l 		#__RAMBAR1 + 0x1,d0		// on for all
   	movec      	d0,RAMBAR1
   	
// STACKPOINTER AUF ENDE SRAM1
	lea 			__SUP_SP,a7
	
// instruction cache on
	move.l		#0x000C8100,d0
 	movec		d0,cacr
 	nop
// initialize any hardware specific issues 
	bra          __initialize_hardware   
");
}
