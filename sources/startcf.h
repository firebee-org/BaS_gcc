
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

#define DIP_SWITCH		(*(volatile uint8_t *)(&_MBAR[0xA2C]))
#define DIP_SWITCHa		__MBAR + 0xA2C

#define sca_page_ID 	6

