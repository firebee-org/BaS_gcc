/*
 * File:		sysinit.h
 * Purpose:		COLDARI Power-on Reset configuration
 *
 * Notes:
 *
 */

#ifndef __SYSINIT_H__
#define __SYSINIT_H__

#ifdef __cplusplus
extern "C" {
#endif


#if ENABLE_UART_SUPPORT==1 

/*
 * System Bus Clock Info
 */
// 5475EVB has 133Mhz system clock
#define SYSTEM_CLOCK_KHZ    133000     /* system bus frequency in kHz */


/*** 
 * Serial Port Info
 * The baud rate to be : 19200
 * Data bits : 8
 * Parity : None
 * Stop Bits : 1
 * Flow Control : None
 */
#define TERMINAL_PORT       (0)          /* PSC channel used as terminal */
#define TERMINAL_BAUD       kBaud19200   /* 115200 */
#undef  HARDWARE_FLOW_CONTROL       /* Flow control ON or OFF */
#endif

/*** 
 * Board Memory map definitions from linker command files:
 * __SDRAM,__SDRAM_SIZE, __FLASH, __FLASH_SIZE linker 
 * symbols must be defined in the linker command file.
 */
extern __declspec(system) uint8 __BOOT_FLASH[];
extern __declspec(system) uint8 __BOOT_FLASH_SIZE[];

extern __declspec(system) uint8 __SDRAM[];
extern __declspec(system) uint8 __SDRAM_SIZE[];


#define BOOT_FLASH_ADDRESS		(uint32)__BOOT_FLASH
#define BOOT_FLASH_SIZE			(uint32)__BOOT_FLASH_SIZE

#define SDRAM_ADDRESS		    (uint32)__SDRAM
#define SDRAM_SIZE			   	(uint32)__SDRAM_SIZE




/********************************************************************/
/* __initialize_hardware Startup code routine
 * 
 * __initialize_hardware is called by the startup code right after reset, 
 * with interrupt disabled and SP pre-set to a valid memory area.
 * Here you should initialize memory and some peripherics;
 * at this point global variables are not initialized yet.
 * The startup code will initialize SP on return of this function.
 */
void __initialize_hardware(void);

/********************************************************************/
/* __initialize_system Startup code routine
 * 
 * __initialize_system is called by the startup code when all languages 
 * specific initialization are done to allow additional hardware setup.
 */ 
void __initialize_system(void);



#ifdef __cplusplus
}
#endif

#endif /* __SYSINIT_H__ */


