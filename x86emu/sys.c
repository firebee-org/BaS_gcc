/****************************************************************************
*
*						Realmode X86 Emulator Library
*
*            	Copyright (C) 1996-1999 SciTech Software, Inc.
* 				     Copyright (C) David Mosberger-Tang
* 					   Copyright (C) 1999 Egbert Eich
*
*  ========================================================================
*
*  Permission to use, copy, modify, distribute, and sell this software and
*  its documentation for any purpose is hereby granted without fee,
*  provided that the above copyright notice appear in all copies and that
*  both that copyright notice and this permission notice appear in
*  supporting documentation, and that the name of the authors not be used
*  in advertising or publicity pertaining to distribution of the software
*  without specific, written prior permission.  The authors makes no
*  representations about the suitability of this software for any purpose.
*  It is provided "as is" without express or implied warranty.
*
*  THE AUTHORS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
*  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
*  EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
*  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
*  USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
*  OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
*  PERFORMANCE OF THIS SOFTWARE.
*
*  ========================================================================
*
* Language:		ANSI C
* Environment:	Any
* Developer:    Kendall Bennett
*
* Description:  This file includes subroutines which are related to
*				programmed I/O and memory access. Included in this module
*				are default functions with limited usefulness. For real
*				uses these functions will most likely be overriden by the
*				user library.
*
****************************************************************************/
/* $XFree86: xc/extras/x86emu/src/x86emu/sys.c,v 1.5 2000/08/23 22:10:01 tsi Exp $ */

#include "radeonfb.h"
#include "pci.h"

#include "x86emu.h"
#include "x86regs.h"
#include "x86debug.h"
#include "x86prim_ops.h"

#ifndef NULL
#define NULL ((void *)0)
#endif


extern uint8_t inb(uint16_t port);
extern uint16_t inw(uint16_t port);
extern uint32_t inl(uint16_t port);
extern void outb(uint8_t val, uint16_t port);
extern void outw(uint16_t val, uint16_t port);
extern void outl(uint32_t val, uint16_t port);
extern uint16_t swap_short(uint16_t val);
extern uint32_t swap_long(uint32_t val);

/*------------------------- Global Variables ------------------------------*/

X86EMU_sysEnv _X86EMU_env;	/* Global emulator machine state */
X86EMU_intrFuncs _X86EMU_intrTab[256];
extern struct radeonfb_info *rinfo_biosemu;
extern uint32_t offset_mem;

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read

RETURNS:
Byte value read from emulator memory.

REMARKS:
Reads a byte value from the emulator memory. 
****************************************************************************/
uint8_t X86API rdb(uint32_t addr)
{
	uint8_t val;
	
	if((addr >= 0xA0000) && (addr <= 0xBFFFF))
	{
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX("rdb(", addr);
#endif
		val = *(uint8_t *)(offset_mem+addr);
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX(") = ", val);
		DPRINT("\r\n");
#endif
	}
	else
	{
#if 0
		if(addr >= M.mem_size)
		{
			DB( { DPRINTVALHEX("mem_ptr: address ", addr);
			      DPRINT(" out of range!\r\n"); } )
			    HALT_SYS();
		}
#endif
#if 0
		if(addr < 0x200)
		{
			DPRINTVALHEXWORD("", M.x86.R_CS);
			DPRINTVALHEXWORD(":", M.x86.R_IP);		
			DPRINTVALHEX(" updating int vector ", addr >> 2);
			DPRINT("\r\n");
		}
#endif
		val = *(uint8_t *)(M.mem_base + addr);
	}
	DB(if (DEBUG_MEM_TRACE())
	{
		DPRINTVALHEXLONG("", addr);
		DPRINTVALHEXBYTE(" 1 -> ", val);
		DPRINT("\r\n");
	} )
	return val;
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read

RETURNS:
Word value read from emulator memory.

REMARKS:
Reads a word value from the emulator memory.
****************************************************************************/
uint16_t X86API rdw(uint32_t addr)
{
	uint16_t val;
	
	if((addr >= 0xA0000) && (addr <= 0xBFFFF))
	{
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX("rdw(", addr);
#endif
		val = swpw(*(uint16_t *)(offset_mem+addr));
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX(") = ", val);
		DPRINT("\r\n");
#endif
	}
	else
	{
#if 0
		if(addr > M.mem_size - 2)
		{
			DB( { DPRINTVALHEX("mem_ptr: address ", addr);
			      DPRINT(" out of range!\r\n"); } )
			    HALT_SYS();
		}
#endif
#if 0
		if(addr < 0x200)
		{
			DPRINTVALHEXWORD("", M.x86.R_CS);
			DPRINTVALHEXWORD(":", M.x86.R_IP);		
			DPRINTVALHEX(" updating int vector ", addr >> 2);
			DPRINT("\r\n");
		}
#endif
		val = (uint16_t)(*(uint8_t *)(M.mem_base + addr));
		val |= (((uint16_t)( *(uint8_t *)(M.mem_base + addr + 1))) << 8);
//		val = *(uint16_t *)(M.mem_base + addr);
	}
	DB(if (DEBUG_MEM_TRACE())
	{
		DPRINTVALHEXLONG("", addr);
		DPRINTVALHEXWORD(" 2 -> ", val);
		DPRINT("\r\n");
	} )
	return val;
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read

RETURNS:
Long value read from emulator memory.
REMARKS:
Reads a long value from the emulator memory. 
****************************************************************************/
uint32_t X86API rdl(uint32_t addr)
{
	uint32_t val;

	if((addr >= 0xA0000) && (addr <= 0xBFFFF))
	{
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX("rdl(", addr);
#endif
		val = swap_long(*(uint32_t *)(offset_mem+addr));
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX(") = ", val);
		DPRINT("\r\n");
#endif
	}
	else
	{
#if 0
		if(addr > M.mem_size - 4)
		{
			DB( { DPRINTVALHEX("mem_ptr: address ", addr);
			      DPRINT(" out of range!\r\n"); } )
			    HALT_SYS();
		}
#endif
#if 0
		if(addr < 0x200)
		{
			DPRINTVALHEXWORD("", M.x86.R_CS);
			DPRINTVALHEXWORD(":", M.x86.R_IP);		
			DPRINTVALHEX(" updating int vector ", addr >> 2);
			DPRINT("\r\n");
		}
#endif
		val = swap_long(*(uint32_t *)(M.mem_base + addr));
//	  val = *(uint32_t *)(M.mem_base + addr);
	}
	DB(if (DEBUG_MEM_TRACE())
	{
		DPRINTVALHEXLONG("", addr);
		DPRINTVALHEXLONG(" 4 -> ", val);
		DPRINT("\r\n");
	} )
	return val;
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read
val		- Value to store

REMARKS:
Writes a byte value to emulator memory.
****************************************************************************/
void X86API wrb(uint32_t addr, uint8_t val)
{
	if((addr >= 0xA0000) && (addr <= 0xBFFFF))
	{
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX("wrb(", addr);
		DPRINTVALHEX(") = ", val);
		DPRINT("\r\n");
#endif
		*(uint8_t *)(offset_mem+addr) = val;
	}
	else
	{
		if(addr >= M.mem_size)
		{
			DB( { DPRINTVALHEX("mem_ptr: address ", addr);
			      DPRINT(" out of range!\r\n"); } )
			    HALT_SYS();
		}
#if 0
		if(addr < 0x200)
		{
			DPRINTVALHEXWORD("", M.x86.R_CS);
			DPRINTVALHEXWORD(":", M.x86.R_IP);		
			DPRINTVALHEX(" updating int vector ", addr >> 2);
			DPRINT("\r\n");
		}
#endif
		*(uint8_t *)(M.mem_base + addr) = val;
	}
	DB(if (DEBUG_MEM_TRACE())
	{
		DPRINTVALHEXLONG("", addr);
		DPRINTVALHEXBYTE(" 1 <- ", val);
		DPRINT("\r\n");
	} )
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read
val		- Value to store

REMARKS:
Writes a word value to emulator memory.
****************************************************************************/
void X86API wrw(uint32_t addr, uint16_t val)
{
	if((addr >= 0xA0000) && (addr <= 0xBFFFF))
	{
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX("wrw(", addr);
		DPRINTVALHEX(") = ", val);
		DPRINT("\r\n");
#endif
#ifdef DIRECT_ACCESS
		*(uint16_t *)(offset_mem+addr) = swap_short(val);
#else
#ifdef PCI_XBIOS
		write_mem_word(rinfo_biosemu->handle, offset_mem+addr, val);
#else
		Write_mem_word(rinfo_biosemu->handle, offset_mem+addr, val);
#endif
#endif
	}
	else
	{
		if(addr > M.mem_size - 2)
		{
			DB( { DPRINTVALHEX("mem_ptr: address ", addr);
			      DPRINT(" out of range!\r\n"); } )
			    HALT_SYS();
		}
#if 0
		if(addr < 0x200)
		{
			DPRINTVALHEXWORD("", M.x86.R_CS);
			DPRINTVALHEXWORD(":", M.x86.R_IP);		
			DPRINTVALHEX(" updating int vector ", addr >> 2);
			DPRINT("\r\n");
		}
#endif
		*(uint8_t *)(M.mem_base + addr) = (uint8_t)val;
		*(uint8_t *)(M.mem_base + addr + 1) = (uint8_t)(val >> 8);
//		*(uint16_t *)(M.mem_base + addr) = val;
	}
	DB(if (DEBUG_MEM_TRACE())
	{
		DPRINTVALHEXLONG("", addr);
		DPRINTVALHEXWORD(" 2 <- ", val);
		DPRINT("\r\n");
	} )
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read
val		- Value to store

REMARKS:
Writes a long value to emulator memory. 
****************************************************************************/
void X86API wrl(uint32_t addr, uint32_t val)
{
	if((addr >= 0xA0000) && (addr <= 0xBFFFF))
	{
#ifdef DEBUG_X86EMU_PCI
		DPRINTVALHEX("wrl(", addr);
		DPRINTVALHEX(") = ", val);
		DPRINT("\r\n");
#endif
#ifdef DIRECT_ACCESS
		*(uint32_t *)(offset_mem+addr) = swap_long(val);
#else
#ifdef PCI_XBIOS
		write_mem_longword(rinfo_biosemu->handle, offset_mem+addr, val);
#else
		Write_mem_longword(rinfo_biosemu->handle, offset_mem+addr, val);
#endif
#endif
	}
	else
	{
		if(addr > M.mem_size - 4)
		{
			DB( { DPRINTVALHEX("mem_ptr: address ", addr);
			      DPRINT(" out of range!\r\n"); } )
			    HALT_SYS();
		}
#if 0
		if(addr < 0x200)
		{
			DPRINTVALHEXWORD("", M.x86.R_CS);
			DPRINTVALHEXWORD(":", M.x86.R_IP);		
			DPRINTVALHEX(" updating int vector ", addr >> 2);
			DPRINT("\r\n");
		}
#endif
		*(uint32_t *)(M.mem_base + addr) = swap_long(val);
//		*(uint32_t *)(M.mem_base + addr) = val;
	}
	DB(if (DEBUG_MEM_TRACE())
	{
		DPRINTVALHEXLONG("", addr);
		DPRINTVALHEXLONG(" 4 <- ", val);
		DPRINT("\r\n");
	} )
}

/****************************************************************************
PARAMETERS:
addr	- PIO address to read
RETURN:
0
REMARKS:
Default PIO byte read function. Doesn't perform real inb.
****************************************************************************/
static uint8_t X86API p_inb(X86EMU_pioAddr addr)
{
	DB(if (DEBUG_IO_TRACE())
	{
		DPRINTVALHEXWORD("inb ", addr);
		DPRINT(" \r\n");
	} )
	return inb(addr);
}

/****************************************************************************
PARAMETERS:
addr	- PIO address to read
RETURN:
0
REMARKS:
Default PIO word read function. Doesn't perform real inw.
****************************************************************************/
static uint16_t X86API p_inw(X86EMU_pioAddr addr)
{
	DB(if (DEBUG_IO_TRACE())
	{
		DPRINTVALHEXWORD("inw ", addr);
		DPRINT(" \r\n");
	} )
	return inw(addr);
}

/****************************************************************************
PARAMETERS:
addr	- PIO address to read
RETURN:
0
REMARKS:
Default PIO long read function. Doesn't perform real inl.
****************************************************************************/
static uint32_t X86API p_inl(X86EMU_pioAddr addr)
{
	DB(if (DEBUG_IO_TRACE())
	{
		DPRINTVALHEXWORD("inl ", addr);
		DPRINT(" \r\n");
	} )
	return inl(addr);
}

/****************************************************************************
PARAMETERS:
addr	- PIO address to write
val     - Value to store
REMARKS:
Default PIO byte write function. Doesn't perform real outb.
****************************************************************************/
static void X86API p_outb(X86EMU_pioAddr addr, uint8_t val)
{
	DB(if (DEBUG_IO_TRACE())
	{
		DPRINTVALHEXBYTE("outb ", val);
		DPRINTVALHEXWORD(" -> ", addr);
		DPRINT(" \r\n");
	} )
	outb(val, addr);
	return;
}

/****************************************************************************
PARAMETERS:
addr	- PIO address to write
val     - Value to store
REMARKS:
Default PIO word write function. Doesn't perform real outw.
****************************************************************************/
static void X86API p_outw(X86EMU_pioAddr addr, uint16_t val)
{
	DB(if (DEBUG_IO_TRACE())
	{
		DPRINTVALHEXWORD("outw ", (unsigned long)val);
		DPRINTVALHEXWORD(" -> ", addr);
		DPRINT(" \r\n");
	} )
	outw(val, addr);
	return;
}

/****************************************************************************
PARAMETERS:
addr	- PIO address to write
val     - Value to store
REMARKS:
Default PIO ;ong write function. Doesn't perform real outl.
****************************************************************************/
static void X86API p_outl(X86EMU_pioAddr addr, uint32_t val)
{
	DB(if (DEBUG_IO_TRACE())
	{
		DPRINTVALHEXLONG("outl ", val);
		DPRINTVALHEXWORD(" -> ", addr);
		DPRINT(" \r\n");
	} )
	outl(val, addr);
	return;
}

/*------------------------- Global Variables ------------------------------*/

uint8_t(X86APIP sys_rdb) (uint32_t addr) = rdb;
uint16_t(X86APIP sys_rdw) (uint32_t addr) = rdw;
uint32_t(X86APIP sys_rdl) (uint32_t addr) = rdl;
void (X86APIP sys_wrb) (uint32_t addr, uint8_t val) = wrb;
void (X86APIP sys_wrw) (uint32_t addr, uint16_t val) = wrw;
void (X86APIP sys_wrl) (uint32_t addr, uint32_t val) = wrl;
uint8_t(X86APIP sys_inb) (X86EMU_pioAddr addr) = p_inb;
uint16_t(X86APIP sys_inw) (X86EMU_pioAddr addr) = p_inw;
uint32_t(X86APIP sys_inl) (X86EMU_pioAddr addr) = p_inl;
void (X86APIP sys_outb) (X86EMU_pioAddr addr, uint8_t val) = p_outb;
void (X86APIP sys_outw) (X86EMU_pioAddr addr, uint16_t val) = p_outw;
void (X86APIP sys_outl) (X86EMU_pioAddr addr, uint32_t val) = p_outl;

/*----------------------------- Setup -------------------------------------*/

#if 0 // cannot works whith data in flash
/****************************************************************************
PARAMETERS:
funcs	- New memory function pointers to make active

REMARKS:
This function is used to set the pointers to functions which access
memory space, allowing the user application to override these functions
and hook them out as necessary for their application.
****************************************************************************/
void X86EMU_setupMemFuncs(X86EMU_memFuncs * funcs)
{
	sys_rdb = funcs->rdb;
	sys_rdw = funcs->rdw;
	sys_rdl = funcs->rdl;
	sys_wrb = funcs->wrb;
	sys_wrw = funcs->wrw;
	sys_wrl = funcs->wrl;
}

/****************************************************************************
PARAMETERS:
funcs	- New programmed I/O function pointers to make active

REMARKS:
This function is used to set the pointers to functions which access
I/O space, allowing the user application to override these functions
and hook them out as necessary for their application.
****************************************************************************/
void X86EMU_setupPioFuncs(X86EMU_pioFuncs * funcs)
{
	sys_inb = funcs->inb;
	sys_inw = funcs->inw;
	sys_inl = funcs->inl;
	sys_outb = funcs->outb;
	sys_outw = funcs->outw;
	sys_outl = funcs->outl;
}
#endif

/****************************************************************************
PARAMETERS:
funcs	- New interrupt vector table to make active

REMARKS:
This function is used to set the pointers to functions which handle
interrupt processing in the emulator, allowing the user application to
hook interrupts as necessary for their application. Any interrupts that
are not hooked by the user application, and reflected and handled internally
in the emulator via the interrupt vector table. This allows the application
to get control when the code being emulated executes specific software
interrupts.
****************************************************************************/
void X86EMU_setupIntrFuncs(X86EMU_intrFuncs funcs[])
{
	int i;

	for (i = 0; i < 256; i++)
		_X86EMU_intrTab[i] = NULL;
	if (funcs) {
		for (i = 0; i < 256; i++)
			_X86EMU_intrTab[i] = funcs[i];
	}
}

/****************************************************************************
PARAMETERS:
int	- New software interrupt to prepare for

REMARKS:
This function is used to set up the emulator state to exceute a software
interrupt. This can be used by the user application code to allow an
interrupt to be hooked, examined and then reflected back to the emulator
so that the code in the emulator will continue processing the software
interrupt as per normal. This essentially allows system code to actively
hook and handle certain software interrupts as necessary.
****************************************************************************/
void X86EMU_prepareForInt(int num)
{
	push_word((uint16_t) M.x86.R_FLG);
	CLEAR_FLAG(F_IF);
	CLEAR_FLAG(F_TF);
	push_word(M.x86.R_CS);
	M.x86.R_CS = mem_access_word(num * 4 + 2);
	push_word(M.x86.R_IP);
	M.x86.R_IP = mem_access_word(num * 4);
	M.x86.intr = 0;
}

void X86EMU_setMemBase(void *base, unsigned long size)
{
	M.mem_base = (int) base;
	M.mem_size = size;
}
