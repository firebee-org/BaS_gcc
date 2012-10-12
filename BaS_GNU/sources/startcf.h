/******************************************************************************
  FILE    : startcf.h
  PURPOSE : startup code for ColdFire
  LANGUAGE: C


  Notes:
        1) Default entry point is _startup. 
           . disable interrupts
           . the SP is set to __SP_AFTER_RESET
           . SP must be initialized to valid memory 
             in case the memory it points to is not valid using MEMORY_INIT macro
        2) __initialize_hardware is called. Here you can initialize memory and some peripherics
           at this point global variables are not initialized yet
        3) After __initialize_hardware memory is setup; initialize SP to _SP_INIT and perform 
           needed initialisations for the language (clear memory, data rom copy).
        4) void __initialize_system(void); is called
           to allow additional hardware initialization (UART, GPIOs, etc...)
        5) Jump to main 

*/
/********************************************************************************/

#define cf_stack

//#define ii_on
#define halten
#define halten_dbcc
#define halten_and
#define halten_add
#define halten_sub
#define halten_or
#define halten_op
#define halten_opc
#define halten_movem
#define halten_lea
#define halten_shift
#define halten_move
#define halten_exg
#define halten_movep
#define halten_ewf

#define DIP_SWITCH		(*(volatile uint8_t *)(&__MBAR[0xA2C]))
#define DIP_SWITCHa		___MBAR + 0xA2C

#define sca_page_ID 	6

