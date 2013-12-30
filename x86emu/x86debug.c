/****************************************************************************
*
*                       Realmode X86 Emulator Library
*
*               Copyright (C) 1991-2004 SciTech Software, Inc.
*                    Copyright (C) David Mosberger-Tang
*                      Copyright (C) 1999 Egbert Eich
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
* Language:     ANSI C
* Environment:  Any
* Developer:    Kendall Bennett
*
* Description:  This file contains the code to handle debugging of the
*               emulator.
*
****************************************************************************/

#include "bas_types.h"
#include "bas_printf.h"
#include "x86debug.h"
#include "x86emui.h"

/*----------------------------- Implementation ----------------------------*/

#ifdef DBG_X86EMU

static void print_encoded_bytes (uint16_t s, uint16_t o);
static void print_decoded_instruction (void);
//static int      parse_line (char *s, int *ps, int *n);

/* should look something like debug's output. */
void X86EMU_trace_regs (void)
{
    if (DEBUG_TRACE()) {
        x86emu_dump_regs();
    }
    if (DEBUG_DECODE() && ! DEBUG_DECODE_NOPRINT()) {
        dbg("0x%x", M.x86.saved_cs);
        dbg(":0x%x", M.x86.saved_ip);
        dbg(" ");
        print_encoded_bytes( M.x86.saved_cs, M.x86.saved_ip);
        print_decoded_instruction();
    }
}

void X86EMU_trace_xregs (void)
{
    if (DEBUG_TRACE()) {
        x86emu_dump_xregs();
    }
}

void x86emu_just_disassemble (void)
{
    /*
     * This routine called if the flag DEBUG_DISASSEMBLE is set kind
     * of a hack!
     */
	dbg("%x:%x ", M.x86.saved_cs, M.x86.saved_ip);
    print_encoded_bytes( M.x86.saved_cs, M.x86.saved_ip);
    print_decoded_instruction();
}

#if 0
static void disassemble_forward (uint16_t seg, uint16_t off, int n)
{
    X86EMU_sysEnv tregs;
    int i;
    u8 op1;
    /*
     * hack, hack, hack.  What we do is use the exact machinery set up
     * for execution, except that now there is an additional state
     * flag associated with the "execution", and we are using a copy
     * of the register struct.  All the major opcodes, once fully
     * decoded, have the following two steps: TRACE_REGS(r,m);
     * SINGLE_STEP(r,m); which disappear if DEBUG is not defined to
     * the preprocessor.  The TRACE_REGS macro expands to:
     *
     * if (debug&DEBUG_DISASSEMBLE)
     *     {just_disassemble(); goto EndOfInstruction;}
     *     if (debug&DEBUG_TRACE) trace_regs(r,m);
     *
     * ......  and at the last line of the routine.
     *
     * EndOfInstruction: end_instr();
     *
     * Up to the point where TRACE_REG is expanded, NO modifications
     * are done to any register EXCEPT the IP register, for fetch and
     * decoding purposes.
     *
     * This was done for an entirely different reason, but makes a
     * nice way to get the system to help debug codes.
     */
    tregs = M;
    tregs.x86.R_IP = off;
    tregs.x86.R_CS = seg;

    /* reset the decoding buffers */
    tregs.x86.enc_str_pos = 0;
    tregs.x86.enc_pos = 0;

    /* turn on the "disassemble only, no execute" flag */
    tregs.x86.debug |= DEBUG_DISASSEMBLE_F;

    /* DUMP NEXT n instructions to screen in straight_line fashion */
    /*
     * This looks like the regular instruction fetch stream, except
     * that when this occurs, each fetched opcode, upon seeing the
     * DEBUG_DISASSEMBLE flag set, exits immediately after decoding
     * the instruction.  XXX --- CHECK THAT MEM IS NOT AFFECTED!!!
     * Note the use of a copy of the register structure...
     */
    for (i=0; i<n; i++) {
        op1 = (*sys_rdb)(((uint32_t)M.x86.R_CS<<4) + (M.x86.R_IP++));
        (x86emu_optab[op1])(op1);
    }
    /* end major hack mode. */
}
#endif

void x86emu_check_ip_access (void)
{
    /* NULL as of now */
}

void x86emu_check_sp_access (void)
{
}

void x86emu_check_mem_access(uint32_t dummy)
{
    /*  check bounds, etc */
}

void x86emu_check_data_access(unsigned int dummy1, unsigned int dummy2)
{
    /*  check bounds, etc */
}

void x86emu_inc_decoded_inst_len(int x)
{
	M.x86.enc_pos += x;
}

void x86emu_decode_printf(char *x)
{
#ifdef DBG_X86EMU
	strcpy(x, &M.x86.decoded_buf[M.x86.enc_str_pos & 127]);
	M.x86.enc_str_pos += strlen(x);
#endif
}

void x86emu_decode_printf2(char *x, int y)
{
	char temp[100], *p;

#ifdef DBG_X86EMU
	sprintf(temp, x, y);
	strcpy(temp, &M.x86.decoded_buf[M.x86.enc_str_pos & 127]);
	M.x86.enc_str_pos += strlen(temp);
#endif
}

void x86emu_end_instr(void)
{
    M.x86.enc_str_pos = 0;
    M.x86.enc_pos = 0;
}

static void print_encoded_bytes (uint16_t s, uint16_t o)
{
    int i;
    for (i = 0; i < M.x86.enc_pos; i++)
        dbg("%x", fetch_data_byte_abs(s,o+i));
    for ( ; i < 10; i++)
        dbg("  ");
}

static void print_decoded_instruction (void)
{
    dbg("%s", M.x86.decoded_buf);
}

void x86emu_print_int_vect (uint16_t iv)
{
    uint16_t seg,off;

    if (iv > 256) return;
    seg = fetch_data_word_abs(0,iv*4);
    off = fetch_data_word_abs(0,iv*4+2);
    dbg("%x:%x", seg, off);
    dbg(" ");
}

void X86EMU_dump_memory (uint16_t seg, uint16_t off, uint32_t amt)
{
    uint32_t start = off & 0xfffffff0;
    uint32_t end  = (off+16) & 0xfffffff0;
    uint32_t i;
    uint32_t current;

    current = start;
    while (end <= off + amt) {
        dbg("%x:%x: ", seg, start);
        dbg(" ");
        for (i = start; i < off; i++)
          dbg("   ");
        for ( ; i< end; i++)
          dbg(" %x\r\n", fetch_data_byte_abs(seg,i));
        start = end;
        end = start + 16;
    }
}

void x86emu_single_step (void)
{
#if 0
    char s[1024];
    int ps[10];
    int ntok;
    int cmd;
    int done;
        int segment;
    int offset;
    static int breakpoint;
    static int noDecode = 1;

    char *p;

        if (DEBUG_BREAK()) {
                if (M.x86.saved_ip != breakpoint) {
                        return;
                } else {
              M.x86.debug &= ~DEBUG_DECODE_NOPRINT_F;
                        M.x86.debug |= DEBUG_TRACE_F;
                        M.x86.debug &= ~DEBUG_BREAK_F;
                        print_decoded_instruction ();
                        X86EMU_trace_regs();
                }
        }
    done=0;
    offset = M.x86.saved_ip;
    while (!done) {
        DPRINT("-");
        p = fgets(s, 1023, stdin);
        cmd = parse_line(s, ps, &ntok);
        switch(cmd) {
          case 'u':
            disassemble_forward(M.x86.saved_cs,(uint16_t)offset,10);
            break;
          case 'd':
                            if (ntok == 2) {
                                    segment = M.x86.saved_cs;
                                    offset = ps[1];
                                    X86EMU_dump_memory(segment,(uint16_t)offset,16);
                                    offset += 16;
                            } else if (ntok == 3) {
                                    segment = ps[1];
                                    offset = ps[2];
                                    X86EMU_dump_memory(segment,(uint16_t)offset,16);
                                    offset += 16;
                            } else {
                                    segment = M.x86.saved_cs;
                                    X86EMU_dump_memory(segment,(uint16_t)offset,16);
                                    offset += 16;
                            }
            break;
          case 'c':
            M.x86.debug ^= DEBUG_TRACECALL_F;
            break;
          case 's':
            M.x86.debug ^= DEBUG_SVC_F | DEBUG_SYS_F | DEBUG_SYSINT_F;
            break;
          case 'r':
            X86EMU_trace_regs();
            break;
          case 'x':
            X86EMU_trace_xregs();
            break;
          case 'g':
            if (ntok == 2) {
                breakpoint = ps[1];
        if (noDecode) {
                        M.x86.debug |= DEBUG_DECODE_NOPRINT_F;
        } else {
                        M.x86.debug &= ~DEBUG_DECODE_NOPRINT_F;
        }
        M.x86.debug &= ~DEBUG_TRACE_F;
        M.x86.debug |= DEBUG_BREAK_F;
        done = 1;
            }
            break;
          case 'q':
          M.x86.debug |= DEBUG_EXIT;
          return;
      case 'P':
          noDecode = (noDecode)?0:1;
          DPRINT("Toggled decoding to ");
          DPRINT((noDecode)?"FALSE":"TRUE");
          DPRINT("\r\n");
          break;
          case 't':
      case 0:
            done = 1;
            break;
        }
    }
#endif
}

int X86EMU_trace_on(void)
{
    return M.x86.debug |= DEBUG_STEP_F | DEBUG_DECODE_F | DEBUG_TRACE_F;
}

int X86EMU_trace_off(void)
{
    return M.x86.debug &= ~(DEBUG_STEP_F | DEBUG_DECODE_F | DEBUG_TRACE_F);
}

int X86EMU_set_debug(int debug)
{
	return M.x86.debug = debug;
}

#if 0
static int parse_line (char *s, int *ps, int *n)
{
    int cmd;

    *n = 0;
    while (*s == ' ' || *s == '\t') s++;
    ps[*n] = *s;
    switch (*s) {
      case '\n':
        *n += 1;
        return 0;
      default:
        cmd = *s;
        *n += 1;
    }

    while (1) {
        while (*s != ' ' && *s != '\t' && *s != '\n')  s++;

        if (*s == '\n')
            return cmd;

        while (*s == ' ' || *s == '\t') s++;

        sscanf(s,"%x",&ps[*n]);
        *n += 1;
    }
}
#endif

#endif /* DEBUG */

void x86emu_dump_regs (void)
{
    dbg("  AX=%x", M.x86.R_AX );
    dbg("  BX=%x", M.x86.R_BX );
    dbg("  CX=%x", M.x86.R_CX );
    dbg("  DX=%x", M.x86.R_DX );
    dbg("  SP=%x", M.x86.R_SP );
    dbg("  BP=%x", M.x86.R_BP );
    dbg("  SI=%x", M.x86.R_SI );
    dbg("  DI=%x", M.x86.R_DI );
    dbg("\r\n");
    dbg("  DS=%x", M.x86.R_DS );
    dbg("  ES=%x", M.x86.R_ES );
    dbg("  SS=%x", M.x86.R_SS );
    dbg("  CS=%x", M.x86.R_CS );
    dbg("  IP=%x", M.x86.R_IP );
    dbg("\r\n  ");
    if (ACCESS_FLAG(F_OF))    dbg("OV ");     /* CHECKED... */
    else                        dbg("NV ");
    if (ACCESS_FLAG(F_DF))    dbg("DN ");
    else                        dbg("UP ");
    if (ACCESS_FLAG(F_IF))    dbg("EI ");
    else                        dbg("DI ");
    if (ACCESS_FLAG(F_SF))    dbg("NG ");
    else                        dbg("PL ");
    if (ACCESS_FLAG(F_ZF))    dbg("ZR ");
    else                        dbg("NZ ");
    if (ACCESS_FLAG(F_AF))    dbg("AC ");
    else                        dbg("NA ");
    if (ACCESS_FLAG(F_PF))    dbg("PE ");
    else                        dbg("PO ");
    if (ACCESS_FLAG(F_CF))    dbg("CY ");
    else                        dbg("NC ");
    dbg("\r\n");
}

void x86emu_dump_xregs (void)
{
    dbg("  EAX=%x", M.x86.R_EAX );
    dbg("  EBX=%x", M.x86.R_EBX );
    dbg("  ECX=%x", M.x86.R_ECX );
    dbg("  EDX=%x", M.x86.R_EDX );
    dbg("\r\n");
    dbg("  ESP=%x", M.x86.R_ESP );
    dbg("  EBP=%x", M.x86.R_EBP );
    dbg("  ESI=%x", M.x86.R_ESI );
    dbg("  EDI=%x", M.x86.R_EDI );
    dbg("\r\n");
    dbg("  DS=%x", M.x86.R_DS );
    dbg("  ES=%x", M.x86.R_ES );
    dbg("  SS=%x", M.x86.R_SS );
    dbg("  CS=%x", M.x86.R_CS );
    dbg("  EIP%x=", M.x86.R_EIP );
    dbg("\r\n  ");
    if (ACCESS_FLAG(F_OF))    dbg("OV ");     /* CHECKED... */
    else                        dbg("NV ");
    if (ACCESS_FLAG(F_DF))    dbg("DN ");
    else                        dbg("UP ");
    if (ACCESS_FLAG(F_IF))    dbg("EI ");
    else                        dbg("DI ");
    if (ACCESS_FLAG(F_SF))    dbg("NG ");
    else                        dbg("PL ");
    if (ACCESS_FLAG(F_ZF))    dbg("ZR ");
    else                        dbg("NZ ");
    if (ACCESS_FLAG(F_AF))    dbg("AC ");
    else                        dbg("NA ");
    if (ACCESS_FLAG(F_PF))    dbg("PE ");
    else                        dbg("PO ");
    if (ACCESS_FLAG(F_CF))    dbg("CY ");
    else                        dbg("NC ");
    dbg("\r\n");
}
