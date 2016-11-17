/*	$NetBSD: x86emu_util.c,v 1.1 2007/11/30 20:02:50 joerg Exp $	*/

/****************************************************************************
*
*  Realmode X86 Emulator Library
*
*  Copyright (C) 1996-1999 SciTech Software, Inc.
*  Copyright (C) David Mosberger-Tang
*  Copyright (C) 1999 Egbert Eich
*  Copyright (C) 2007 Joerg Sonnenberger
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
****************************************************************************/

#include "x86emu.h"
#include "x86emu_regs.h"

#define DEBUG
#include "debug.h"

static __inline uint16_t le16dec(const void *buf)
{
    const uint8_t *p = (uint8_t *) buf;

    return ((p[1] << 8) | p[0]);
}

static __inline uint32_t le32dec(const void *buf)
{
    const uint8_t *p = (uint8_t *) buf;

    return ((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | p[0]);
}

static __inline void le16enc(void *buf, uint16_t u)
{
    uint8_t *p = buf;

    p[0] = u & 0xff;
    p[1] = ((unsigned) u >> 8) & 0xff;
}

static __inline void le32enc(void *buf, uint32_t u)
{
    uint8_t *p = buf;

    p[0] = u & 0xff;
    p[1] = (u >> 8) & 0xff;
    p[2] = (u >> 16) & 0xff;
    p[3] = (u >> 24) & 0xff;
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read

RETURNS:
Byte value read from emulator memory.

REMARKS:
Reads a byte value from the emulator memory.
****************************************************************************/
static uint8_t rdb(struct X86EMU *emu, uint32_t addr)
{
    if (addr > emu->mem_size - 1)
    {
        dbg("attempted read outside system memory: 0x%lx. Halt emulator.\r\n", addr);
        X86EMU_halt_sys(emu);
    }
    return emu->mem_base[addr];
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read

RETURNS:
Word value read from emulator memory.

REMARKS:
Reads a word value from the emulator memory.
****************************************************************************/
static uint16_t rdw(struct X86EMU *emu, uint32_t addr)
{
    if (addr > emu->mem_size - 2)
    {
        dbg("attempted read outside system memory: 0x%lx. Halt emulator.\r\n", addr);
        X86EMU_halt_sys(emu);
    }
    return le16dec(emu->mem_base + addr);
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read

RETURNS:
Long value read from emulator memory.
REMARKS:
Reads a long value from the emulator memory.
****************************************************************************/
static uint32_t rdl(struct X86EMU *emu, uint32_t addr)
{
    if (addr > emu->mem_size - 4)
    {
        dbg("attempted read outside system memory: 0x%lx. Halt emulator.\r\n", addr);
        X86EMU_halt_sys(emu);
    }
    return le32dec(emu->mem_base + addr);
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read
val		- Value to store

REMARKS:
Writes a byte value to emulator memory.
****************************************************************************/
static void wrb(struct X86EMU *emu, uint32_t addr, uint8_t val)
{
    if (addr > emu->mem_size - 1)
    {
        dbg("attempted write outside system memory: 0x%lx (0x%02x). Halt emulator.\r\n", addr, val);
        X86EMU_halt_sys(emu);
    }
    emu->mem_base[addr] = val;
}

/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to read
val		- Value to store

REMARKS:
Writes a word value to emulator memory.
****************************************************************************/
static void wrw(struct X86EMU *emu, uint32_t addr, uint16_t val)
{
    if (addr > emu->mem_size - 2)
    {
        dbg("attempted write outside system memory: 0x%lx (0x%04x). Halt emulator\r\n", addr, val);
        X86EMU_halt_sys(emu);
    }
    le16enc(emu->mem_base + addr, val);
}
/****************************************************************************
PARAMETERS:
addr	- Emulator memory address to write
val		- Value to store

REMARKS:
Writes a long value to emulator memory.
****************************************************************************/
static void wrl(struct X86EMU *emu, uint32_t addr, uint32_t val)
{
    if (addr > emu->mem_size - 4)
    {
        dbg("attempted write outside system memory: 0x%lx (0x%08x)\r\n", addr, val);
        X86EMU_halt_sys(emu);
    }
    le32enc(emu->mem_base + addr, val);
}

/*----------------------------- Setup -------------------------------------*/

void X86EMU_init_default(struct X86EMU *emu)
{
    int i;

    emu->emu_rdb = rdb;
    emu->emu_rdw = rdw;
    emu->emu_rdl = rdl;
    emu->emu_wrb = wrb;
    emu->emu_wrw = wrw;
    emu->emu_wrl = wrl;

    for (i = 0; i < 256; i++)
    {
        emu->_X86EMU_intrTab[i] = NULL;
    }
}
