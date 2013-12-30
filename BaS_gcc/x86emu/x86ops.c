/****************************************************************************
 *
 *						Realmode X86 Emulator Library
 *
 *            	Copyright (C) 1991-2004 SciTech Software, Inc.
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
 * Description:  This file includes subroutines to implement the decoding
 *               and emulation of all the x86 processor instructions.
 *
 * There are approximately 250 subroutines in here, which correspond
 * to the 256 byte-"opcodes" found on the 8086.  The table which
 * dispatches this is found in the files optab.[ch].
 *
 * Each opcode proc has a comment preceeding it which gives it's table
 * address.  Several opcodes are missing (undefined) in the table.
 *
 * Each proc includes information for decoding (DECODE_PRINTF and
 * DECODE_PRINTF2), debugging (TRACE_REGS, SINGLE_STEP), and misc
 * functions (START_OF_INSTR, END_OF_INSTR).
 *
 * Many of the procedures are *VERY* similar in coding.  This has
 * allowed for a very large amount of code to be generated in a fairly
 * short amount of time (i.e. cut, paste, and modify).  The result is
 * that much of the code below could have been folded into subroutines
 * for a large reduction in size of this file.  The downside would be
 * that there would be a penalty in execution speed.  The file could
 * also have been *MUCH* larger by inlining certain functions which
 * were called.  This could have resulted even faster execution.  The
 * prime directive I used to decide whether to inline the code or to
 * modularize it, was basically: 1) no unnecessary subroutine calls,
 * 2) no routines more than about 200 lines in size, and 3) modularize
 * any code that I might not get right the first time.  The fetch_*
 * subroutines fall into the latter category.  The The decode_* fall
 * into the second category.  The coding of the "switch(mod){ .... }"
 * in many of the subroutines below falls into the first category.
 * Especially, the coding of {add,and,or,sub,...}_{byte,word}
 * subroutines are an especially glaring case of the third guideline.
 * Since so much of the code is cloned from other modules (compare
 * opcode #00 to opcode #01), making the basic operations subroutine
 * calls is especially important; otherwise mistakes in coding an
 * "add" would represent a nightmare in maintenance.
 *
 ****************************************************************************/

#include "x86debug.h"
#include "x86emui.h"

extern unsigned decode_rmXX_address(int mod, int rm);

/*----------------------------- Implementation ----------------------------*/

/* constant arrays to do several instructions in just one function */

#ifdef DEBUG
static char *x86emu_GenOpName[8] = {
	"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "CMP"};
#endif

/* used by several opcodes  */
static uint8_t (*genop_byte_operation[])(uint8_t d, uint8_t s) =
{
	add_byte,           /* 00 */
	or_byte,            /* 01 */
	adc_byte,           /* 02 */
	sbb_byte,           /* 03 */
	and_byte,           /* 04 */
	sub_byte,           /* 05 */
	xor_byte,           /* 06 */
	cmp_byte,           /* 07 */
};

static uint16_t (*genop_word_operation[])(uint16_t d, uint16_t s) =
{
	add_word,           /*00 */
	or_word,            /*01 */
	adc_word,           /*02 */
	sbb_word,           /*03 */
	and_word,           /*04 */
	sub_word,           /*05 */
	xor_word,           /*06 */
	cmp_word,           /*07 */
};

static uint32_t (*genop_long_operation[])(uint32_t d, uint32_t s) =
{
	add_long,           /*00 */
	or_long,            /*01 */
	adc_long,           /*02 */
	sbb_long,           /*03 */
	and_long,           /*04 */
	sub_long,           /*05 */
	xor_long,           /*06 */
	cmp_long,           /*07 */
};

/* used by opcodes 80, c0, d0, and d2. */
static uint8_t(*opcD0_byte_operation[])(uint8_t d, uint8_t s) =
{
	rol_byte,
	ror_byte,
	rcl_byte,
	rcr_byte,
	shl_byte,
	shr_byte,
	shl_byte,           /* sal_byte === shl_byte  by definition */
	sar_byte,
};

/* used by opcodes c1, d1, and d3. */
static uint16_t(*opcD1_word_operation[])(uint16_t s, uint8_t d) =
{
	rol_word,
	ror_word,
	rcl_word,
	rcr_word,
	shl_word,
	shr_word,
	shl_word,           /* sal_byte === shl_byte  by definition */
	sar_word,
};

/* used by opcodes c1, d1, and d3. */
static uint32_t (*opcD1_long_operation[])(uint32_t s, uint8_t d) =
{
	rol_long,
	ror_long,
	rcl_long,
	rcr_long,
	shl_long,
	shr_long,
	shl_long,           /* sal_byte === shl_byte  by definition */
	sar_long,
};

#ifdef DEBUG

static char *opF6_names[8] =
{ "TEST\t", "", "NOT\t", "NEG\t", "MUL\t", "IMUL\t", "DIV\t", "IDIV\t" };

#endif

/****************************************************************************
PARAMETERS:
op1 - Instruction op code

REMARKS:
Handles illegal opcodes.
 ****************************************************************************/
void x86emuOp_illegal_op(
		uint8_t op1)
{
	START_OF_INSTR();
	if (M.x86.R_SP != 0) {
		DECODE_PRINTF("ILLEGAL X86 OPCODE\r\n");
		TRACE_REGS();
		dbg("%x:%x: %x\r\n", M.x86.R_CS, M.x86.R_IP - 1, op1);
		dbg(" ILLEGAL X86 OPCODE!\r\n");
		HALT_SYS();
	}
	else {
		/* If we get here, it means the stack pointer is back to zero
		 * so we are just returning from an emulator service call
		 * so therte is no need to display an error message. We trap
		 * the emulator with an 0xF1 opcode to finish the service
		 * call.
		 */
		X86EMU_halt_sys();
	}
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcodes 0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38
 ****************************************************************************/
void x86emuOp_genop_byte_RM_R(uint8_t op1)
{
	int mod, rl, rh;
	unsigned int destoffset;
	uint8_t *destreg, *srcreg;
	uint8_t destval;

	op1 = (op1 >> 3) & 0x7;

	START_OF_INSTR();
	DECODE_PRINTF(x86emu_GenOpName[op1]);
	DECODE_PRINTF("\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if(mod<3)
	{ destoffset = decode_rmXX_address(mod,rl);
		DECODE_PRINTF(",");
		destval = fetch_data_byte(destoffset);
		srcreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		destval = genop_byte_operation[op1](destval, *srcreg);
		if (op1 != 7)
			store_data_byte(destoffset, destval);
	}
	else
	{                       /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF(",");
		srcreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*destreg = genop_byte_operation[op1](*destreg, *srcreg);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcodes 0x01, 0x09, 0x11, 0x19, 0x21, 0x29, 0x31, 0x39
 ****************************************************************************/
void x86emuOp_genop_word_RM_R(uint8_t op1)
{
	int mod, rl, rh;
	unsigned int destoffset;

	op1 = (op1 >> 3) & 0x7;

	START_OF_INSTR();
	DECODE_PRINTF(x86emu_GenOpName[op1]);
	DECODE_PRINTF("\t");
	FETCH_DECODE_MODRM(mod, rh, rl);

	if(mod<3) {
		destoffset = decode_rmXX_address(mod,rl);
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval;
			uint32_t *srcreg;

			DECODE_PRINTF(",");
			destval = fetch_data_long(destoffset);
			srcreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			destval = genop_long_operation[op1](destval, *srcreg);
			if (op1 != 7)
				store_data_long(destoffset, destval);
		} else {
			uint16_t destval;
			uint16_t *srcreg;

			DECODE_PRINTF(",");
			destval = fetch_data_word(destoffset);
			srcreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			destval = genop_word_operation[op1](destval, *srcreg);
			if (op1 != 7)
				store_data_word(destoffset, destval);
		}
	} else {                    /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg, *srcreg;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = genop_long_operation[op1](*destreg, *srcreg);
		} else {
			uint16_t *destreg, *srcreg;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = genop_word_operation[op1](*destreg, *srcreg);
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcodes 0x02, 0x0a, 0x12, 0x1a, 0x22, 0x2a, 0x32, 0x3a
 ****************************************************************************/
void x86emuOp_genop_byte_R_RM(uint8_t op1)
{
	int mod, rl, rh;
	uint8_t *destreg, *srcreg;
	unsigned int srcoffset;
	uint8_t srcval;

	op1 = (op1 >> 3) & 0x7;

	START_OF_INSTR();
	DECODE_PRINTF(x86emu_GenOpName[op1]);
	DECODE_PRINTF("\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF(",");
		srcoffset = decode_rmXX_address(mod,rl);
		srcval = fetch_data_byte(srcoffset);
	} else {     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF(",");
		srcreg = DECODE_RM_BYTE_REGISTER(rl);
		srcval = *srcreg;
	}
	DECODE_PRINTF("\r\n");
	TRACE_AND_STEP();
	*destreg = genop_byte_operation[op1](*destreg, srcval);

	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcodes 0x03, 0x0b, 0x13, 0x1b, 0x23, 0x2b, 0x33, 0x3b
 ****************************************************************************/
void x86emuOp_genop_word_R_RM(uint8_t op1)
{
	int mod, rl, rh;
	unsigned int srcoffset;
	uint32_t *destreg32, srcval;
	uint16_t *destreg;

	op1 = (op1 >> 3) & 0x7;

	START_OF_INSTR();
	DECODE_PRINTF(x86emu_GenOpName[op1]);
	DECODE_PRINTF("\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		srcoffset = decode_rmXX_address(mod,rl);
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			destreg32 = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF(",");
			srcval = fetch_data_long(srcoffset);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg32 = genop_long_operation[op1](*destreg32, srcval);
		} else {
			destreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF(",");
			srcval = fetch_data_word(srcoffset);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = genop_word_operation[op1](*destreg, srcval);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *srcreg;
			destreg32 = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg32 = genop_long_operation[op1](*destreg32, *srcreg);
		} else {
			uint16_t *srcreg;
			destreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = genop_word_operation[op1](*destreg, *srcreg);
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcodes 0x04, 0x0c, 0x14, 0x1c, 0x24, 0x2c, 0x34, 0x3c
 ****************************************************************************/
void x86emuOp_genop_byte_AL_IMM(uint8_t op1)
{
	uint8_t srcval;

	op1 = (op1 >> 3) & 0x7;

	START_OF_INSTR();
	DECODE_PRINTF(x86emu_GenOpName[op1]);
	DECODE_PRINTF("\tAL,");
	srcval = fetch_byte_imm();
	DECODE_PRINTF2("%x\r\n", srcval);
	TRACE_AND_STEP();
	M.x86.R_AL = genop_byte_operation[op1](M.x86.R_AL, srcval);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcodes 0x05, 0x0d, 0x15, 0x1d, 0x25, 0x2d, 0x35, 0x3d
 ****************************************************************************/
void x86emuOp_genop_word_AX_IMM(uint8_t op1)
{
	uint32_t srcval;

	op1 = (op1 >> 3) & 0x7;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF(x86emu_GenOpName[op1]);
		DECODE_PRINTF("\tEAX,");
		srcval = fetch_long_imm();
	} else {
		DECODE_PRINTF(x86emu_GenOpName[op1]);
		DECODE_PRINTF("\tAX,");
		srcval = fetch_word_imm();
	}
	DECODE_PRINTF2("%x\r\n", srcval);
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		M.x86.R_EAX = genop_long_operation[op1](M.x86.R_EAX, srcval);
	} else {
		M.x86.R_AX = genop_word_operation[op1](M.x86.R_AX, (uint16_t)srcval);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x06
 ****************************************************************************/
void x86emuOp_push_ES(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("PUSH\tES\r\n");
	TRACE_AND_STEP();
	push_word(M.x86.R_ES);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x07
 ****************************************************************************/
void x86emuOp_pop_ES(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("POP\tES\r\n");
	TRACE_AND_STEP();
	M.x86.R_ES = pop_word();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x0e
 ****************************************************************************/
void x86emuOp_push_CS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("PUSH\tCS\r\n");
	TRACE_AND_STEP();
	push_word(M.x86.R_CS);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x0f. Escape for two-byte opcode (286 or better)
 ****************************************************************************/
void x86emuOp_two_byte(uint8_t X86EMU_UNUSED(op1))
{
	uint8_t op2 = (*sys_rdb)(((uint32_t)M.x86.R_CS << 4) + (M.x86.R_IP++));
	INC_DECODED_INST_LEN(1);
	(*x86emu_optab2[op2])(op2);
}

/****************************************************************************
REMARKS:
Handles opcode 0x16
 ****************************************************************************/
void x86emuOp_push_SS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("PUSH\tSS\r\n");
	TRACE_AND_STEP();
	push_word(M.x86.R_SS);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x17
 ****************************************************************************/
void x86emuOp_pop_SS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("POP\tSS\r\n");
	TRACE_AND_STEP();
	M.x86.R_SS = pop_word();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x1e
 ****************************************************************************/
void x86emuOp_push_DS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("PUSH\tDS\r\n");
	TRACE_AND_STEP();
	push_word(M.x86.R_DS);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x1f
 ****************************************************************************/
void x86emuOp_pop_DS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("POP\tDS\r\n");
	TRACE_AND_STEP();
	M.x86.R_DS = pop_word();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x26
 ****************************************************************************/
void x86emuOp_segovr_ES(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("ES:\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_SEGOVR_ES;
	/*
	 * note the lack of DECODE_CLEAR_SEGOVR(r) since, here is one of 4
	 * opcode subroutines we do not want to do this.
	 */
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x27
 ****************************************************************************/
void x86emuOp_daa(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("DAA\r\n");
	TRACE_AND_STEP();
	M.x86.R_AL = daa_byte(M.x86.R_AL);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x2e
 ****************************************************************************/
void x86emuOp_segovr_CS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("CS:\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_SEGOVR_CS;
	/* note no DECODE_CLEAR_SEGOVR here. */
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x2f
 ****************************************************************************/
void x86emuOp_das(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("DAS\r\n");
	TRACE_AND_STEP();
	M.x86.R_AL = das_byte(M.x86.R_AL);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x36
 ****************************************************************************/
void x86emuOp_segovr_SS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("SS:\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_SEGOVR_SS;
	/* no DECODE_CLEAR_SEGOVR ! */
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x37
 ****************************************************************************/
void x86emuOp_aaa(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("AAA\r\n");
	TRACE_AND_STEP();
	M.x86.R_AX = aaa_word(M.x86.R_AX);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x3e
 ****************************************************************************/
void x86emuOp_segovr_DS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("DS:\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_SEGOVR_DS;
	/* NO DECODE_CLEAR_SEGOVR! */
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x3f
 ****************************************************************************/
void x86emuOp_aas(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("AAS\r\n");
	TRACE_AND_STEP();
	M.x86.R_AX = aas_word(M.x86.R_AX);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x40 - 0x47
 ****************************************************************************/
void x86emuOp_inc_register(uint8_t op1)
{
	START_OF_INSTR();
	op1 &= 0x7;
	DECODE_PRINTF("INC\t");
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		uint32_t *reg;
		reg = DECODE_RM_LONG_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*reg = inc_long(*reg);
	} else {
		uint16_t *reg;
		reg = DECODE_RM_WORD_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*reg = inc_word(*reg);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x48 - 0x4F
 ****************************************************************************/
void x86emuOp_dec_register(uint8_t op1)
{
	START_OF_INSTR();
	op1 &= 0x7;
	DECODE_PRINTF("DEC\t");
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		uint32_t *reg;
		reg = DECODE_RM_LONG_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*reg = dec_long(*reg);
	} else {
		uint16_t *reg;
		reg = DECODE_RM_WORD_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*reg = dec_word(*reg);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x50 - 0x57
 ****************************************************************************/
void x86emuOp_push_register(uint8_t op1)
{
	START_OF_INSTR();
	op1 &= 0x7;
	DECODE_PRINTF("PUSH\t");
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		uint32_t *reg;
		reg = DECODE_RM_LONG_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		push_long(*reg);
	} else {
		uint16_t *reg;
		reg = DECODE_RM_WORD_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		push_word(*reg);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x58 - 0x5F
 ****************************************************************************/
void x86emuOp_pop_register(uint8_t op1)
{
	START_OF_INSTR();
	op1 &= 0x7;
	DECODE_PRINTF("POP\t");
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		uint32_t *reg;
		reg = DECODE_RM_LONG_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*reg = pop_long();
	} else {
		uint16_t *reg;
		reg = DECODE_RM_WORD_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*reg = pop_word();
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x60
 ****************************************************************************/
void x86emuOp_push_all(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("PUSHAD\r\n");
	} else {
		DECODE_PRINTF("PUSHA\r\n");
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		uint32_t old_sp = M.x86.R_ESP;

		push_long(M.x86.R_EAX);
		push_long(M.x86.R_ECX);
		push_long(M.x86.R_EDX);
		push_long(M.x86.R_EBX);
		push_long(old_sp);
		push_long(M.x86.R_EBP);
		push_long(M.x86.R_ESI);
		push_long(M.x86.R_EDI);
	} else {
		uint16_t old_sp = M.x86.R_SP;

		push_word(M.x86.R_AX);
		push_word(M.x86.R_CX);
		push_word(M.x86.R_DX);
		push_word(M.x86.R_BX);
		push_word(old_sp);
		push_word(M.x86.R_BP);
		push_word(M.x86.R_SI);
		push_word(M.x86.R_DI);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x61
 ****************************************************************************/
void x86emuOp_pop_all(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("POPAD\r\n");
	} else {
		DECODE_PRINTF("POPA\r\n");
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		M.x86.R_EDI = pop_long();
		M.x86.R_ESI = pop_long();
		M.x86.R_EBP = pop_long();
		M.x86.R_ESP += 4;              /* skip ESP */
		M.x86.R_EBX = pop_long();
		M.x86.R_EDX = pop_long();
		M.x86.R_ECX = pop_long();
		M.x86.R_EAX = pop_long();
	} else {
		M.x86.R_DI = pop_word();
		M.x86.R_SI = pop_word();
		M.x86.R_BP = pop_word();
		M.x86.R_SP += 2;               /* skip SP */
		M.x86.R_BX = pop_word();
		M.x86.R_DX = pop_word();
		M.x86.R_CX = pop_word();
		M.x86.R_AX = pop_word();
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/*opcode 0x62   ILLEGAL OP, calls x86emuOp_illegal_op() */
/*opcode 0x63   ILLEGAL OP, calls x86emuOp_illegal_op() */

/****************************************************************************
REMARKS:
Handles opcode 0x64
 ****************************************************************************/
void x86emuOp_segovr_FS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("FS:\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_SEGOVR_FS;
	/*
	 * note the lack of DECODE_CLEAR_SEGOVR(r) since, here is one of 4
	 * opcode subroutines we do not want to do this.
	 */
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x65
 ****************************************************************************/
void x86emuOp_segovr_GS(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("GS:\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_SEGOVR_GS;
	/*
	 * note the lack of DECODE_CLEAR_SEGOVR(r) since, here is one of 4
	 * opcode subroutines we do not want to do this.
	 */
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x66 - prefix for 32-bit register
 ****************************************************************************/
void x86emuOp_prefix_data(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("DATA:\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_PREFIX_DATA;
	/* note no DECODE_CLEAR_SEGOVR here. */
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x67 - prefix for 32-bit address
 ****************************************************************************/
void x86emuOp_prefix_addr(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("ADDR:\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_PREFIX_ADDR;
	/* note no DECODE_CLEAR_SEGOVR here. */
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x68
 ****************************************************************************/
void x86emuOp_push_word_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint32_t imm;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		imm = fetch_long_imm();
	} else {
		imm = fetch_word_imm();
	}
	DECODE_PRINTF2("PUSH\t%x\r\n", imm);
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		push_long(imm);
	} else {
		push_word((uint16_t)imm);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x69
 ****************************************************************************/
void x86emuOp_imul_word_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int srcoffset;

	START_OF_INSTR();
	DECODE_PRINTF("IMUL\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		srcoffset = decode_rmXX_address(mod, rl);
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg;
			uint32_t srcval;
			uint32_t res_lo,res_hi;
			int32_t imm;

			destreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF(",");
			srcval = fetch_data_long(srcoffset);
			imm = fetch_long_imm();
			DECODE_PRINTF2(",%d\r\n", (int32_t)imm);
			TRACE_AND_STEP();
			imul_long_direct(&res_lo,&res_hi,(int32_t)srcval,(int32_t)imm);
			if ((((res_lo & 0x80000000) == 0) && (res_hi == 0x00000000)) ||
					(((res_lo & 0x80000000) != 0) && (res_hi == 0xFFFFFFFF))) {
				CLEAR_FLAG(F_CF);
				CLEAR_FLAG(F_OF);
			} else {
				SET_FLAG(F_CF);
				SET_FLAG(F_OF);
			}
			*destreg = (uint32_t)res_lo;
		} else {
			uint16_t *destreg;
			uint16_t srcval;
			uint32_t res;
			int16_t imm;

			destreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF(",");
			srcval = fetch_data_word(srcoffset);
			imm = fetch_word_imm();
			DECODE_PRINTF2(",%d\r\n", (int32_t)imm);
			TRACE_AND_STEP();
			res = (int16_t)srcval * (int16_t)imm;
			if ((((res & 0x8000) == 0) && ((res >> 16) == 0x0000)) ||
					(((res & 0x8000) != 0) && ((res >> 16) == 0xFFFF))) {
				CLEAR_FLAG(F_CF);
				CLEAR_FLAG(F_OF);
			} else {
				SET_FLAG(F_CF);
				SET_FLAG(F_OF);
			}
			*destreg = (uint16_t)res;
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg,*srcreg;
			uint32_t res_lo,res_hi;
			int32_t imm;

			destreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rl);
			imm = fetch_long_imm();
			DECODE_PRINTF2(",%d\r\n", (int32_t)imm);
			TRACE_AND_STEP();
			imul_long_direct(&res_lo,&res_hi,(int32_t)*srcreg,(int32_t)imm);
			if ((((res_lo & 0x80000000) == 0) && (res_hi == 0x00000000)) ||
					(((res_lo & 0x80000000) != 0) && (res_hi == 0xFFFFFFFF))) {
				CLEAR_FLAG(F_CF);
				CLEAR_FLAG(F_OF);
			} else {
				SET_FLAG(F_CF);
				SET_FLAG(F_OF);
			}
			*destreg = (uint32_t)res_lo;
		} else {
			uint16_t *destreg,*srcreg;
			uint32_t res;
			int16_t imm;

			destreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rl);
			imm = fetch_word_imm();
			DECODE_PRINTF2(",%d\r\n", (int32_t)imm);
			res = (int16_t)*srcreg * (int16_t)imm;
			if ((((res & 0x8000) == 0) && ((res >> 16) == 0x0000)) ||
					(((res & 0x8000) != 0) && ((res >> 16) == 0xFFFF))) {
				CLEAR_FLAG(F_CF);
				CLEAR_FLAG(F_OF);
			} else {
				SET_FLAG(F_CF);
				SET_FLAG(F_OF);
			}
			*destreg = (uint16_t)res;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x6a
 ****************************************************************************/
void x86emuOp_push_byte_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int16_t imm;

	START_OF_INSTR();
	imm = (int8_t)fetch_byte_imm();
	DECODE_PRINTF2("PUSH\t%d\r\n", imm);
	TRACE_AND_STEP();
	push_word(imm);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x6b
 ****************************************************************************/
void x86emuOp_imul_byte_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int srcoffset;
	int8_t  imm;

	START_OF_INSTR();
	DECODE_PRINTF("IMUL\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		srcoffset = decode_rmXX_address(mod, rl);
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg;
			uint32_t srcval;
			uint32_t res_lo,res_hi;

			destreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF(",");
			srcval = fetch_data_long(srcoffset);
			imm = fetch_byte_imm();
			DECODE_PRINTF2(",%d\r\n", (int32_t)imm);
			TRACE_AND_STEP();
			imul_long_direct(&res_lo,&res_hi,(int32_t)srcval,(int32_t)imm);
			if ((((res_lo & 0x80000000) == 0) && (res_hi == 0x00000000)) ||
					(((res_lo & 0x80000000) != 0) && (res_hi == 0xFFFFFFFF))) {
				CLEAR_FLAG(F_CF);
				CLEAR_FLAG(F_OF);
			} else {
				SET_FLAG(F_CF);
				SET_FLAG(F_OF);
			}
			*destreg = (uint32_t)res_lo;
		} else {
			uint16_t *destreg;
			uint16_t srcval;
			uint32_t res;

			destreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF(",");
			srcval = fetch_data_word(srcoffset);
			imm = fetch_byte_imm();
			DECODE_PRINTF2(",%d\r\n", (int32_t)imm);
			TRACE_AND_STEP();
			res = (int16_t)srcval * (int16_t)imm;
			if ((((res & 0x8000) == 0) && ((res >> 16) == 0x0000)) ||
					(((res & 0x8000) != 0) && ((res >> 16) == 0xFFFF))) {
				CLEAR_FLAG(F_CF);
				CLEAR_FLAG(F_OF);
			} else {
				SET_FLAG(F_CF);
				SET_FLAG(F_OF);
			}
			*destreg = (uint16_t)res;
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg,*srcreg;
			uint32_t res_lo,res_hi;

			destreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rl);
			imm = fetch_byte_imm();
			DECODE_PRINTF2(",%d\r\n", (int32_t)imm);
			TRACE_AND_STEP();
			imul_long_direct(&res_lo,&res_hi,(int32_t)*srcreg,(int32_t)imm);
			if ((((res_lo & 0x80000000) == 0) && (res_hi == 0x00000000)) ||
					(((res_lo & 0x80000000) != 0) && (res_hi == 0xFFFFFFFF))) {
				CLEAR_FLAG(F_CF);
				CLEAR_FLAG(F_OF);
			} else {
				SET_FLAG(F_CF);
				SET_FLAG(F_OF);
			}
			*destreg = (uint32_t)res_lo;
		} else {
			uint16_t *destreg,*srcreg;
			uint32_t res;

			destreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rl);
			imm = fetch_byte_imm();
			DECODE_PRINTF2(",%d\r\n", (int32_t)imm);
			TRACE_AND_STEP();
			res = (int16_t)*srcreg * (int16_t)imm;
			if ((((res & 0x8000) == 0) && ((res >> 16) == 0x0000)) ||
					(((res & 0x8000) != 0) && ((res >> 16) == 0xFFFF))) {
				CLEAR_FLAG(F_CF);
				CLEAR_FLAG(F_OF);
			} else {
				SET_FLAG(F_CF);
				SET_FLAG(F_OF);
			}
			*destreg = (uint16_t)res;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x6c
 ****************************************************************************/
void x86emuOp_ins_byte(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("INSB\r\n");
	ins(1);
	TRACE_AND_STEP();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x6d
 ****************************************************************************/
void x86emuOp_ins_word(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("INSD\r\n");
		ins(4);
	} else {
		DECODE_PRINTF("INSW\r\n");
		ins(2);
	}
	TRACE_AND_STEP();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x6e
 ****************************************************************************/
void x86emuOp_outs_byte(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("OUTSB\r\n");
	outs(1);
	TRACE_AND_STEP();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x6f
 ****************************************************************************/
void x86emuOp_outs_word(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("OUTSD\r\n");
		outs(4);
	} else {
		DECODE_PRINTF("OUTSW\r\n");
		outs(2);
	}
	TRACE_AND_STEP();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x70 - 0x7F
 ****************************************************************************/
int x86emu_check_jump_condition(uint8_t op);

void x86emuOp_jump_near_cond(uint8_t op1)
{
	int8_t offset;
	uint16_t target;
	int cond;

	/* jump to byte offset if overflow flag is set */
	START_OF_INSTR();
	cond = x86emu_check_jump_condition(op1 & 0xF);
	offset = (int8_t)fetch_byte_imm();
	target = (uint16_t)(M.x86.R_IP + (int16_t)offset);
	DECODE_PRINTF2("%x\r\n", target);
	TRACE_AND_STEP();
	if (cond)
		M.x86.R_IP = target;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x80
 ****************************************************************************/
void x86emuOp_opc80_byte_RM_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg;
	unsigned int destoffset;
	uint8_t imm;
	uint8_t destval;

	/*
	 * Weirdo special case instruction format.  Part of the opcode
	 * held below in "RH".  Doubly nested case would result, except
	 * that the decoded instruction
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */

		switch (rh) {
			case 0:
				DECODE_PRINTF("ADD\t");
				break;
			case 1:
				DECODE_PRINTF("OR\t");
				break;
			case 2:
				DECODE_PRINTF("ADC\t");
				break;
			case 3:
				DECODE_PRINTF("SBB\t");
				break;
			case 4:
				DECODE_PRINTF("AND\t");
				break;
			case 5:
				DECODE_PRINTF("SUB\t");
				break;
			case 6:
				DECODE_PRINTF("XOR\t");
				break;
			case 7:
				DECODE_PRINTF("CMP\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	if (mod < 3) {
		DECODE_PRINTF("BYTE PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF(",");
		destval = fetch_data_byte(destoffset);
		imm = fetch_byte_imm();
		DECODE_PRINTF2("%x\r\n", imm);
		TRACE_AND_STEP();
		destval = (*genop_byte_operation[rh]) (destval, imm);
		if (rh != 7)
			store_data_byte(destoffset, destval);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF(",");
		imm = fetch_byte_imm();
		DECODE_PRINTF2("%x\r\n", imm);
		TRACE_AND_STEP();
		*destreg = (*genop_byte_operation[rh]) (*destreg, imm);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x81
 ****************************************************************************/
void x86emuOp_opc81_word_RM_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	/*
	 * Weirdo special case instruction format.  Part of the opcode
	 * held below in "RH".  Doubly nested case would result, except
	 * that the decoded instruction
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */

		switch (rh) {
			case 0:
				DECODE_PRINTF("ADD\t");
				break;
			case 1:
				DECODE_PRINTF("OR\t");
				break;
			case 2:
				DECODE_PRINTF("ADC\t");
				break;
			case 3:
				DECODE_PRINTF("SBB\t");
				break;
			case 4:
				DECODE_PRINTF("AND\t");
				break;
			case 5:
				DECODE_PRINTF("SUB\t");
				break;
			case 6:
				DECODE_PRINTF("XOR\t");
				break;
			case 7:
				DECODE_PRINTF("CMP\t");
				break;
		}
	}
#endif
	/*
	 * Know operation, decode the mod byte to find the addressing
	 * mode.
	 */
	if (mod < 3) {
		DECODE_PRINTF("DWORD PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval,imm;

			DECODE_PRINTF(",");
			destval = fetch_data_long(destoffset);
			imm = fetch_long_imm();
			DECODE_PRINTF2("%x\r\n", imm);
			TRACE_AND_STEP();
			destval = (*genop_long_operation[rh]) (destval, imm);
			if (rh != 7)
				store_data_long(destoffset, destval);
		} else {
			uint16_t destval,imm;

			DECODE_PRINTF(",");
			destval = fetch_data_word(destoffset);
			imm = fetch_word_imm();
			DECODE_PRINTF2("%x\r\n", imm);
			TRACE_AND_STEP();
			destval = (*genop_word_operation[rh]) (destval, imm);
			if (rh != 7)
				store_data_word(destoffset, destval);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg, imm;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF(",");
			imm = fetch_long_imm();
			DECODE_PRINTF2("%x\r\n", imm);
			TRACE_AND_STEP();
			*destreg = (*genop_long_operation[rh]) (*destreg, imm);
		} else {
			uint16_t *destreg, imm;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF(",");
			imm = fetch_word_imm();
			DECODE_PRINTF2("%x\r\n", imm);
			TRACE_AND_STEP();
			*destreg = (*genop_word_operation[rh]) (*destreg, imm);
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x82
 ****************************************************************************/
void x86emuOp_opc82_byte_RM_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg;
	unsigned int destoffset;
	uint8_t imm;
	uint8_t destval;

	/*
	 * Weirdo special case instruction format.  Part of the opcode
	 * held below in "RH".  Doubly nested case would result, except
	 * that the decoded instruction Similar to opcode 81, except that
	 * the immediate byte is sign extended to a word length.
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */
		switch (rh) {
			case 0:
				DECODE_PRINTF("ADD\t");
				break;
			case 1:
				DECODE_PRINTF("OR\t");
				break;
			case 2:
				DECODE_PRINTF("ADC\t");
				break;
			case 3:
				DECODE_PRINTF("SBB\t");
				break;
			case 4:
				DECODE_PRINTF("AND\t");
				break;
			case 5:
				DECODE_PRINTF("SUB\t");
				break;
			case 6:
				DECODE_PRINTF("XOR\t");
				break;
			case 7:
				DECODE_PRINTF("CMP\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	if (mod < 3) {
		DECODE_PRINTF("BYTE PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		destval = fetch_data_byte(destoffset);
		imm = fetch_byte_imm();
		DECODE_PRINTF2(",%x\r\n", imm);
		TRACE_AND_STEP();
		destval = (*genop_byte_operation[rh]) (destval, imm);
		if (rh != 7)
			store_data_byte(destoffset, destval);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		imm = fetch_byte_imm();
		DECODE_PRINTF2(",%x\r\n", imm);
		TRACE_AND_STEP();
		*destreg = (*genop_byte_operation[rh]) (*destreg, imm);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x83
 ****************************************************************************/
void x86emuOp_opc83_word_RM_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	/*
	 * Weirdo special case instruction format.  Part of the opcode
	 * held below in "RH".  Doubly nested case would result, except
	 * that the decoded instruction Similar to opcode 81, except that
	 * the immediate byte is sign extended to a word length.
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */
		switch (rh) {
			case 0:
				DECODE_PRINTF("ADD\t");
				break;
			case 1:
				DECODE_PRINTF("OR\t");
				break;
			case 2:
				DECODE_PRINTF("ADC\t");
				break;
			case 3:
				DECODE_PRINTF("SBB\t");
				break;
			case 4:
				DECODE_PRINTF("AND\t");
				break;
			case 5:
				DECODE_PRINTF("SUB\t");
				break;
			case 6:
				DECODE_PRINTF("XOR\t");
				break;
			case 7:
				DECODE_PRINTF("CMP\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	if (mod < 3) {
		DECODE_PRINTF("DWORD PTR ");
		destoffset = decode_rmXX_address(mod,rl);

		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval,imm;

			destval = fetch_data_long(destoffset);
			imm = (int8_t) fetch_byte_imm();
			DECODE_PRINTF2(",%x\r\n", imm);
			TRACE_AND_STEP();
			destval = (*genop_long_operation[rh]) (destval, imm);
			if (rh != 7)
				store_data_long(destoffset, destval);
		} else {
			uint16_t destval,imm;

			destval = fetch_data_word(destoffset);
			imm = (int8_t) fetch_byte_imm();
			DECODE_PRINTF2(",%x\r\n", imm);
			TRACE_AND_STEP();
			destval = (*genop_word_operation[rh]) (destval, imm);
			if (rh != 7)
				store_data_word(destoffset, destval);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg, imm;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			imm = (int8_t) fetch_byte_imm();
			DECODE_PRINTF2(",%x\r\n", imm);
			TRACE_AND_STEP();
			*destreg = (*genop_long_operation[rh]) (*destreg, imm);
		} else {
			uint16_t *destreg, imm;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			imm = (int8_t) fetch_byte_imm();
			DECODE_PRINTF2(",%x\r\n", imm);
			TRACE_AND_STEP();
			*destreg = (*genop_word_operation[rh]) (*destreg, imm);
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x84
 ****************************************************************************/
void x86emuOp_test_byte_RM_R(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg, *srcreg;
	unsigned int destoffset;
	uint8_t destval;

	START_OF_INSTR();
	DECODE_PRINTF("TEST\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF(",");
		destval = fetch_data_byte(destoffset);
		srcreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		test_byte(destval, *srcreg);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF(",");
		srcreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		test_byte(*destreg, *srcreg);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x85
 ****************************************************************************/
void x86emuOp_test_word_RM_R(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	START_OF_INSTR();
	DECODE_PRINTF("TEST\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval;
			uint32_t *srcreg;

			DECODE_PRINTF(",");
			destval = fetch_data_long(destoffset);
			srcreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			test_long(destval, *srcreg);
		} else {
			uint16_t destval;
			uint16_t *srcreg;

			DECODE_PRINTF(",");
			destval = fetch_data_word(destoffset);
			srcreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			test_word(destval, *srcreg);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg,*srcreg;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			test_long(*destreg, *srcreg);
		} else {
			uint16_t *destreg,*srcreg;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			test_word(*destreg, *srcreg);
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x86
 ****************************************************************************/
void x86emuOp_xchg_byte_RM_R(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg, *srcreg;
	unsigned int destoffset;
	uint8_t destval;
	uint8_t tmp;

	START_OF_INSTR();
	DECODE_PRINTF("XCHG\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF(",");
		destval = fetch_data_byte(destoffset);
		srcreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		tmp = *srcreg;
		*srcreg = destval;
		destval = tmp;
		store_data_byte(destoffset, destval);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF(",");
		srcreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		tmp = *srcreg;
		*srcreg = *destreg;
		*destreg = tmp;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x87
 ****************************************************************************/
void x86emuOp_xchg_word_RM_R(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	START_OF_INSTR();
	DECODE_PRINTF("XCHG\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF(",");
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *srcreg;
			uint32_t destval,tmp;

			destval = fetch_data_long(destoffset);
			srcreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			tmp = *srcreg;
			*srcreg = destval;
			destval = tmp;
			store_data_long(destoffset, destval);
		} else {
			uint16_t *srcreg;
			uint16_t destval,tmp;

			destval = fetch_data_word(destoffset);
			srcreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			tmp = *srcreg;
			*srcreg = destval;
			destval = tmp;
			store_data_word(destoffset, destval);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg,*srcreg;
			uint32_t tmp;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			tmp = *srcreg;
			*srcreg = *destreg;
			*destreg = tmp;
		} else {
			uint16_t *destreg,*srcreg;
			uint16_t tmp;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			tmp = *srcreg;
			*srcreg = *destreg;
			*destreg = tmp;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x88
 ****************************************************************************/
void x86emuOp_mov_byte_RM_R(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg, *srcreg;
	unsigned int destoffset;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF(",");
		srcreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		store_data_byte(destoffset, *srcreg);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF(",");
		srcreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*destreg = *srcreg;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x89
 ****************************************************************************/
void x86emuOp_mov_word_RM_R(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *srcreg;

			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			store_data_long(destoffset, *srcreg);
		} else {
			uint16_t *srcreg;

			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			store_data_word(destoffset, *srcreg);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg,*srcreg;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = *srcreg;
		} else {
			uint16_t *destreg,*srcreg;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = *srcreg;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x8a
 ****************************************************************************/
void x86emuOp_mov_byte_R_RM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg, *srcreg;
	unsigned int srcoffset;
	uint8_t srcval;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF(",");
		srcoffset = decode_rmXX_address(mod, rl);
		srcval = fetch_data_byte(srcoffset);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*destreg = srcval;
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rh);
		DECODE_PRINTF(",");
		srcreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*destreg = *srcreg;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x8b
 ****************************************************************************/
void x86emuOp_mov_word_R_RM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int srcoffset;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg;
			uint32_t srcval;

			destreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF(",");
			srcoffset = decode_rmXX_address(mod, rl);
			srcval = fetch_data_long(srcoffset);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = srcval;
		} else {
			uint16_t *destreg;
			uint16_t srcval;

			destreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF(",");
			srcoffset = decode_rmXX_address(mod, rl);
			srcval = fetch_data_word(srcoffset);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = srcval;
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg, *srcreg;

			destreg = DECODE_RM_LONG_REGISTER(rh);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = *srcreg;
		} else {
			uint16_t *destreg, *srcreg;

			destreg = DECODE_RM_WORD_REGISTER(rh);
			DECODE_PRINTF(",");
			srcreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = *srcreg;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x8c
 ****************************************************************************/
void x86emuOp_mov_word_RM_SR(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint16_t *destreg, *srcreg;
	unsigned int destoffset;
	uint16_t destval;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF(",");
		srcreg = decode_rm_seg_register(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		destval = *srcreg;
		store_data_word(destoffset, destval);
	} else {                     /* register to register */
		destreg = DECODE_RM_WORD_REGISTER(rl);
		DECODE_PRINTF(",");
		srcreg = decode_rm_seg_register(rh);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*destreg = *srcreg;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x8d
 ****************************************************************************/
void x86emuOp_lea_word_R_M(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint16_t *srcreg;
	unsigned int destoffset;

	/*
	 * TODO: Need to handle address size prefix!
	 *
	 * lea  eax,[eax+ebx*2] ??
	 */

	START_OF_INSTR();
	DECODE_PRINTF("LEA\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		srcreg = DECODE_RM_WORD_REGISTER(rh);
		DECODE_PRINTF(",");
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*srcreg = (uint16_t)destoffset;
	}
	/* } else { undefined.  Do nothing. } */
DECODE_CLEAR_SEGOVR();
END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x8e
 ****************************************************************************/
void x86emuOp_mov_word_SR_RM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint16_t *destreg, *srcreg;
	unsigned int srcoffset;
	uint16_t srcval;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		destreg = decode_rm_seg_register(rh);
		DECODE_PRINTF(",");
		srcoffset = decode_rmXX_address(mod, rl);
		srcval = fetch_data_word(srcoffset);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*destreg = srcval;
	} else {                     /* register to register */
		destreg = decode_rm_seg_register(rh);
		DECODE_PRINTF(",");
		srcreg = DECODE_RM_WORD_REGISTER(rl);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*destreg = *srcreg;
	}
	/*
	 * Clean up, and reset all the R_xSP pointers to the correct
	 * locations.  This is about 3x too much overhead (doing all the
	 * segreg ptrs when only one is needed, but this instruction
	 * *cannot* be that common, and this isn't too much work anyway.
	 */
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x8f
 ****************************************************************************/
void x86emuOp_pop_RM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	START_OF_INSTR();
	DECODE_PRINTF("POP\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (rh != 0) {
		DECODE_PRINTF("ILLEGAL DECODE OF OPCODE 8F\r\n");
		HALT_SYS();
	}
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval;

			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			destval = pop_long();
			store_data_long(destoffset, destval);
		} else {
			uint16_t destval;

			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			destval = pop_word();
			store_data_word(destoffset, destval);
		}
	} else {                    /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = pop_long();
		} else {
			uint16_t *destreg;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF("\r\n");
			TRACE_AND_STEP();
			*destreg = pop_word();
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x90
 ****************************************************************************/
void x86emuOp_nop(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("NOP\r\n");
	TRACE_AND_STEP();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x91-0x97
 ****************************************************************************/
void x86emuOp_xchg_word_AX_register(uint8_t X86EMU_UNUSED(op1))
{
	uint32_t tmp;

	op1 &= 0x7;

	START_OF_INSTR();

	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		uint32_t *reg32;
		DECODE_PRINTF("XCHG\tEAX,");
		reg32 = DECODE_RM_LONG_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		tmp = M.x86.R_EAX;
		M.x86.R_EAX = *reg32;
		*reg32 = tmp;
	} else {
		uint16_t *reg16;
		DECODE_PRINTF("XCHG\tAX,");
		reg16 = DECODE_RM_WORD_REGISTER(op1);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		tmp = M.x86.R_AX;
		M.x86.R_AX = *reg16;
		*reg16 = (uint16_t)tmp;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x98
 ****************************************************************************/
void x86emuOp_cbw(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("CWDE\r\n");
	} else {
		DECODE_PRINTF("CBW\r\n");
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		if (M.x86.R_AX & 0x8000) {
			M.x86.R_EAX |= 0xffff0000;
		} else {
			M.x86.R_EAX &= 0x0000ffff;
		}
	} else {
		if (M.x86.R_AL & 0x80) {
			M.x86.R_AH = 0xff;
		} else {
			M.x86.R_AH = 0x0;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x99
 ****************************************************************************/
void x86emuOp_cwd(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("CDQ\r\n");
	} else {
		DECODE_PRINTF("CWD\r\n");
	}
	DECODE_PRINTF("CWD\r\n");
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		if (M.x86.R_EAX & 0x80000000) {
			M.x86.R_EDX = 0xffffffff;
		} else {
			M.x86.R_EDX = 0x0;
		}
	} else {
		if (M.x86.R_AX & 0x8000) {
			M.x86.R_DX = 0xffff;
		} else {
			M.x86.R_DX = 0x0;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x9a
 ****************************************************************************/
void x86emuOp_call_far_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t farseg, faroff;

	START_OF_INSTR();
	DECODE_PRINTF("CALL\t");
	faroff = fetch_word_imm();
	farseg = fetch_word_imm();
	DECODE_PRINTF2("%x:", farseg);
	DECODE_PRINTF2("%x\r\n", faroff);
	CALL_TRACE(M.x86.saved_cs, M.x86.saved_ip, farseg, faroff, "FAR ");

	/* XXX
	 *
	 * Hooked interrupt vectors calling into our "BIOS" will cause
	 * problems unless all intersegment stuff is checked for BIOS
	 * access.  Check needed here.  For moment, let it alone.
	 */
	TRACE_AND_STEP();
	push_word(M.x86.R_CS);
	M.x86.R_CS = farseg;
	push_word(M.x86.R_IP);
	M.x86.R_IP = faroff;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x9b
 ****************************************************************************/
void x86emuOp_wait(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("WAIT");
	TRACE_AND_STEP();
	/* NADA.  */
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x9c
 ****************************************************************************/
void x86emuOp_pushf_word(uint8_t X86EMU_UNUSED(op1))
{
	uint32_t flags;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("PUSHFD\r\n");
	} else {
		DECODE_PRINTF("PUSHF\r\n");
	}
	TRACE_AND_STEP();

	/* clear out *all* bits not representing flags, and turn on real bits */
	flags = (M.x86.R_EFLG & F_MSK) | F_ALWAYS_ON;
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		push_long(flags);
	} else {
		push_word((uint16_t)flags);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x9d
 ****************************************************************************/
void x86emuOp_popf_word(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("POPFD\r\n");
	} else {
		DECODE_PRINTF("POPF\r\n");
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		M.x86.R_EFLG = pop_long();
	} else {
		M.x86.R_FLG = pop_word();
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x9e
 ****************************************************************************/
void x86emuOp_sahf(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("SAHF\r\n");
	TRACE_AND_STEP();
	/* clear the lower bits of the flag register */
	M.x86.R_FLG &= 0xffffff00;
	/* or in the AH register into the flags register */
	M.x86.R_FLG |= M.x86.R_AH;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0x9f
 ****************************************************************************/
void x86emuOp_lahf(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("LAHF\r\n");
	TRACE_AND_STEP();
	M.x86.R_AH = (uint8_t)(M.x86.R_FLG & 0xff);
	/*undocumented TC++ behavior??? Nope.  It's documented, but
	  you have too look real hard to notice it. */
	M.x86.R_AH |= 0x2;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa0
 ****************************************************************************/
void x86emuOp_mov_AL_M_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t offset;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\tAL,");
	offset = fetch_word_imm();
	DECODE_PRINTF2("[%x]\r\n", offset);
	TRACE_AND_STEP();
	M.x86.R_AL = fetch_data_byte(offset);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa1
 ****************************************************************************/
void x86emuOp_mov_AX_M_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t offset;

	START_OF_INSTR();
	offset = fetch_word_imm();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF2("MOV\tEAX,[%x]\r\n", offset);
	} else {
		DECODE_PRINTF2("MOV\tAX,[%x]\r\n", offset);
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		M.x86.R_EAX = fetch_data_long(offset);
	} else {
		M.x86.R_AX = fetch_data_word(offset);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa2
 ****************************************************************************/
void x86emuOp_mov_M_AL_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t offset;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	offset = fetch_word_imm();
	DECODE_PRINTF2("[%x],AL\r\n", offset);
	TRACE_AND_STEP();
	store_data_byte(offset, M.x86.R_AL);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa3
 ****************************************************************************/
void x86emuOp_mov_M_AX_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t offset;

	START_OF_INSTR();
	offset = fetch_word_imm();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF2("MOV\t[%x],EAX\r\n", offset);
	} else {
		DECODE_PRINTF2("MOV\t[%x],AX\r\n", offset);
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		store_data_long(offset, M.x86.R_EAX);
	} else {
		store_data_word(offset, M.x86.R_AX);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa4
 ****************************************************************************/
void x86emuOp_movs_byte(uint8_t X86EMU_UNUSED(op1))
{
	uint8_t  val;
	uint32_t count;
	int inc;

	START_OF_INSTR();
	DECODE_PRINTF("MOVS\tBYTE\r\n");
	if (ACCESS_FLAG(F_DF))   /* down */
		inc = -1;
	else
		inc = 1;
	TRACE_AND_STEP();
	count = 1;
	if (M.x86.mode & (SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE)) {
		/* dont care whether REPE or REPNE */
		/* move them until CX is ZERO. */
		count = M.x86.R_CX;
		M.x86.R_CX = 0;
		M.x86.mode &= ~(SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE);
	}
	while (count--) {
		val = fetch_data_byte(M.x86.R_SI);
		store_data_byte_abs(M.x86.R_ES, M.x86.R_DI, val);
		M.x86.R_SI += inc;
		M.x86.R_DI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa5
 ****************************************************************************/
void x86emuOp_movs_word(uint8_t X86EMU_UNUSED(op1))
{
	uint32_t val;
	int inc;
	uint32_t count;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("MOVS\tDWORD\r\n");
		if (ACCESS_FLAG(F_DF))      /* down */
			inc = -4;
		else
			inc = 4;
	} else {
		DECODE_PRINTF("MOVS\tWORD\r\n");
		if (ACCESS_FLAG(F_DF))      /* down */
			inc = -2;
		else
			inc = 2;
	}
	TRACE_AND_STEP();
	count = 1;
	if (M.x86.mode & (SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE)) {
		/* dont care whether REPE or REPNE */
		/* move them until CX is ZERO. */
		count = M.x86.R_CX;
		M.x86.R_CX = 0;
		M.x86.mode &= ~(SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE);
	}
	while (count--) {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			val = fetch_data_long(M.x86.R_SI);
			store_data_long_abs(M.x86.R_ES, M.x86.R_DI, val);
		} else {
			val = fetch_data_word(M.x86.R_SI);
			store_data_word_abs(M.x86.R_ES, M.x86.R_DI, (uint16_t)val);
		}
		M.x86.R_SI += inc;
		M.x86.R_DI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa6
 ****************************************************************************/
void x86emuOp_cmps_byte(uint8_t X86EMU_UNUSED(op1))
{
	int8_t val1, val2;
	int inc;

	START_OF_INSTR();
	DECODE_PRINTF("CMPS\tBYTE\r\n");
	TRACE_AND_STEP();
	if (ACCESS_FLAG(F_DF))   /* down */
		inc = -1;
	else
		inc = 1;

	if (M.x86.mode & (SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE)) {
		/* REPE  */
		/* move them until CX is ZERO. */
		while (M.x86.R_CX != 0) {
			val1 = fetch_data_byte(M.x86.R_SI);
			val2 = fetch_data_byte_abs(M.x86.R_ES, M.x86.R_DI);
			cmp_byte(val1, val2);
			M.x86.R_CX -= 1;
			M.x86.R_SI += inc;
			M.x86.R_DI += inc;
			if ( (M.x86.mode & SYSMODE_PREFIX_REPE) && (ACCESS_FLAG(F_ZF) == 0) ) break;
			if ( (M.x86.mode & SYSMODE_PREFIX_REPNE) && ACCESS_FLAG(F_ZF) ) break;
		}
		M.x86.mode &= ~(SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE);
	} else {
		val1 = fetch_data_byte(M.x86.R_SI);
		val2 = fetch_data_byte_abs(M.x86.R_ES, M.x86.R_DI);
		cmp_byte(val1, val2);
		M.x86.R_SI += inc;
		M.x86.R_DI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa7
 ****************************************************************************/
void x86emuOp_cmps_word(uint8_t X86EMU_UNUSED(op1))
{
	uint32_t val1,val2;
	int inc;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("CMPS\tDWORD\r\n");
		inc = 4;
	} else {
		DECODE_PRINTF("CMPS\tWORD\r\n");
		inc = 2;
	}
	if (ACCESS_FLAG(F_DF))   /* down */
		inc = -inc;

	TRACE_AND_STEP();
	if (M.x86.mode & (SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE)) {
		/* REPE  */
		/* move them until CX is ZERO. */
		while (M.x86.R_CX != 0) {
			if (M.x86.mode & SYSMODE_PREFIX_DATA) {
				val1 = fetch_data_long(M.x86.R_SI);
				val2 = fetch_data_long_abs(M.x86.R_ES, M.x86.R_DI);
				cmp_long(val1, val2);
			} else {
				val1 = fetch_data_word(M.x86.R_SI);
				val2 = fetch_data_word_abs(M.x86.R_ES, M.x86.R_DI);
				cmp_word((uint16_t)val1, (uint16_t)val2);
			}
			M.x86.R_CX -= 1;
			M.x86.R_SI += inc;
			M.x86.R_DI += inc;
			if ( (M.x86.mode & SYSMODE_PREFIX_REPE) && ACCESS_FLAG(F_ZF) == 0 ) break;
			if ( (M.x86.mode & SYSMODE_PREFIX_REPNE) && ACCESS_FLAG(F_ZF) ) break;
		}
		M.x86.mode &= ~(SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE);
	} else {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			val1 = fetch_data_long(M.x86.R_SI);
			val2 = fetch_data_long_abs(M.x86.R_ES, M.x86.R_DI);
			cmp_long(val1, val2);
		} else {
			val1 = fetch_data_word(M.x86.R_SI);
			val2 = fetch_data_word_abs(M.x86.R_ES, M.x86.R_DI);
			cmp_word((uint16_t)val1, (uint16_t)val2);
		}
		M.x86.R_SI += inc;
		M.x86.R_DI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa8
 ****************************************************************************/
void x86emuOp_test_AL_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int imm;

	START_OF_INSTR();
	DECODE_PRINTF("TEST\tAL,");
	imm = fetch_byte_imm();
	DECODE_PRINTF2("%x\r\n", imm);
	TRACE_AND_STEP();
	test_byte(M.x86.R_AL, (uint8_t)imm);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xa9
 ****************************************************************************/
void x86emuOp_test_AX_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint32_t srcval;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("TEST\tEAX,");
		srcval = fetch_long_imm();
	} else {
		DECODE_PRINTF("TEST\tAX,");
		srcval = fetch_word_imm();
	}
	DECODE_PRINTF2("%x\r\n", srcval);
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		test_long(M.x86.R_EAX, srcval);
	} else {
		test_word(M.x86.R_AX, (uint16_t)srcval);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xaa
 ****************************************************************************/
void x86emuOp_stos_byte(uint8_t X86EMU_UNUSED(op1))
{
	int inc;

	START_OF_INSTR();
	DECODE_PRINTF("STOS\tBYTE\r\n");
	if (ACCESS_FLAG(F_DF))   /* down */
		inc = -1;
	else
		inc = 1;
	TRACE_AND_STEP();
	if (M.x86.mode & (SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE)) {
		/* dont care whether REPE or REPNE */
		/* move them until CX is ZERO. */
		while (M.x86.R_CX != 0) {
			store_data_byte_abs(M.x86.R_ES, M.x86.R_DI, M.x86.R_AL);
			M.x86.R_CX -= 1;
			M.x86.R_DI += inc;
		}
		M.x86.mode &= ~(SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE);
	} else {
		store_data_byte_abs(M.x86.R_ES, M.x86.R_DI, M.x86.R_AL);
		M.x86.R_DI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xab
 ****************************************************************************/
void x86emuOp_stos_word(uint8_t X86EMU_UNUSED(op1))
{
	int inc;
	uint32_t count;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("STOS\tDWORD\r\n");
		if (ACCESS_FLAG(F_DF))   /* down */
			inc = -4;
		else
			inc = 4;
	} else {
		DECODE_PRINTF("STOS\tWORD\r\n");
		if (ACCESS_FLAG(F_DF))   /* down */
			inc = -2;
		else
			inc = 2;
	}
	TRACE_AND_STEP();
	count = 1;
	if (M.x86.mode & (SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE)) {
		/* dont care whether REPE or REPNE */
		/* move them until CX is ZERO. */
		count = M.x86.R_CX;
		M.x86.R_CX = 0;
		M.x86.mode &= ~(SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE);
	}
	while (count--) {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			store_data_long_abs(M.x86.R_ES, M.x86.R_DI, M.x86.R_EAX);
		} else {
			store_data_word_abs(M.x86.R_ES, M.x86.R_DI, M.x86.R_AX);
		}
		M.x86.R_DI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xac
 ****************************************************************************/
void x86emuOp_lods_byte(uint8_t X86EMU_UNUSED(op1))
{
	int inc;

	START_OF_INSTR();
	DECODE_PRINTF("LODS\tBYTE\r\n");
	TRACE_AND_STEP();
	if (ACCESS_FLAG(F_DF))   /* down */
		inc = -1;
	else
		inc = 1;
	if (M.x86.mode & (SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE)) {
		/* dont care whether REPE or REPNE */
		/* move them until CX is ZERO. */
		while (M.x86.R_CX != 0) {
			M.x86.R_AL = fetch_data_byte(M.x86.R_SI);
			M.x86.R_CX -= 1;
			M.x86.R_SI += inc;
		}
		M.x86.mode &= ~(SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE);
	} else {
		M.x86.R_AL = fetch_data_byte(M.x86.R_SI);
		M.x86.R_SI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xad
 ****************************************************************************/
void x86emuOp_lods_word(uint8_t X86EMU_UNUSED(op1))
{
	int inc;
	uint32_t count;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("LODS\tDWORD\r\n");
		if (ACCESS_FLAG(F_DF))   /* down */
			inc = -4;
		else
			inc = 4;
	} else {
		DECODE_PRINTF("LODS\tWORD\r\n");
		if (ACCESS_FLAG(F_DF))   /* down */
			inc = -2;
		else
			inc = 2;
	}
	TRACE_AND_STEP();
	count = 1;
	if (M.x86.mode & (SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE)) {
		/* dont care whether REPE or REPNE */
		/* move them until CX is ZERO. */
		count = M.x86.R_CX;
		M.x86.R_CX = 0;
		M.x86.mode &= ~(SYSMODE_PREFIX_REPE | SYSMODE_PREFIX_REPNE);
	}
	while (count--) {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			M.x86.R_EAX = fetch_data_long(M.x86.R_SI);
		} else {
			M.x86.R_AX = fetch_data_word(M.x86.R_SI);
		}
		M.x86.R_SI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xae
 ****************************************************************************/
void x86emuOp_scas_byte(uint8_t X86EMU_UNUSED(op1))
{
	int8_t val2;
	int inc;

	START_OF_INSTR();
	DECODE_PRINTF("SCAS\tBYTE\r\n");
	TRACE_AND_STEP();
	if (ACCESS_FLAG(F_DF))   /* down */
		inc = -1;
	else
		inc = 1;
	if (M.x86.mode & SYSMODE_PREFIX_REPE) {
		/* REPE  */
		/* move them until CX is ZERO. */
		while (M.x86.R_CX != 0) {
			val2 = fetch_data_byte_abs(M.x86.R_ES, M.x86.R_DI);
			cmp_byte(M.x86.R_AL, val2);
			M.x86.R_CX -= 1;
			M.x86.R_DI += inc;
			if (ACCESS_FLAG(F_ZF) == 0)
				break;
		}
		M.x86.mode &= ~SYSMODE_PREFIX_REPE;
	} else if (M.x86.mode & SYSMODE_PREFIX_REPNE) {
		/* REPNE  */
		/* move them until CX is ZERO. */
		while (M.x86.R_CX != 0) {
			val2 = fetch_data_byte_abs(M.x86.R_ES, M.x86.R_DI);
			cmp_byte(M.x86.R_AL, val2);
			M.x86.R_CX -= 1;
			M.x86.R_DI += inc;
			if (ACCESS_FLAG(F_ZF))
				break;          /* zero flag set means equal */
		}
		M.x86.mode &= ~SYSMODE_PREFIX_REPNE;
	} else {
		val2 = fetch_data_byte_abs(M.x86.R_ES, M.x86.R_DI);
		cmp_byte(M.x86.R_AL, val2);
		M.x86.R_DI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xaf
 ****************************************************************************/
void x86emuOp_scas_word(uint8_t X86EMU_UNUSED(op1))
{
	int inc;
	uint32_t val;

	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("SCAS\tDWORD\r\n");
		if (ACCESS_FLAG(F_DF))   /* down */
			inc = -4;
		else
			inc = 4;
	} else {
		DECODE_PRINTF("SCAS\tWORD\r\n");
		if (ACCESS_FLAG(F_DF))   /* down */
			inc = -2;
		else
			inc = 2;
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_REPE) {
		/* REPE  */
		/* move them until CX is ZERO. */
		while (M.x86.R_CX != 0) {
			if (M.x86.mode & SYSMODE_PREFIX_DATA) {
				val = fetch_data_long_abs(M.x86.R_ES, M.x86.R_DI);
				cmp_long(M.x86.R_EAX, val);
			} else {
				val = fetch_data_word_abs(M.x86.R_ES, M.x86.R_DI);
				cmp_word(M.x86.R_AX, (uint16_t)val);
			}
			M.x86.R_CX -= 1;
			M.x86.R_DI += inc;
			if (ACCESS_FLAG(F_ZF) == 0)
				break;
		}
		M.x86.mode &= ~SYSMODE_PREFIX_REPE;
	} else if (M.x86.mode & SYSMODE_PREFIX_REPNE) {
		/* REPNE  */
		/* move them until CX is ZERO. */
		while (M.x86.R_CX != 0) {
			if (M.x86.mode & SYSMODE_PREFIX_DATA) {
				val = fetch_data_long_abs(M.x86.R_ES, M.x86.R_DI);
				cmp_long(M.x86.R_EAX, val);
			} else {
				val = fetch_data_word_abs(M.x86.R_ES, M.x86.R_DI);
				cmp_word(M.x86.R_AX, (uint16_t)val);
			}
			M.x86.R_CX -= 1;
			M.x86.R_DI += inc;
			if (ACCESS_FLAG(F_ZF))
				break;          /* zero flag set means equal */
		}
		M.x86.mode &= ~SYSMODE_PREFIX_REPNE;
	} else {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			val = fetch_data_long_abs(M.x86.R_ES, M.x86.R_DI);
			cmp_long(M.x86.R_EAX, val);
		} else {
			val = fetch_data_word_abs(M.x86.R_ES, M.x86.R_DI);
			cmp_word(M.x86.R_AX, (uint16_t)val);
		}
		M.x86.R_DI += inc;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xb0 - 0xb7
 ****************************************************************************/
void x86emuOp_mov_byte_register_IMM(uint8_t op1)
{
	uint8_t imm, *ptr;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	ptr = DECODE_RM_BYTE_REGISTER(op1 & 0x7);
	DECODE_PRINTF(",");
	imm = fetch_byte_imm();
	DECODE_PRINTF2("%x\r\n", imm);
	TRACE_AND_STEP();
	*ptr = imm;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xb8 - 0xbf
 ****************************************************************************/
void x86emuOp_mov_word_register_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint32_t srcval;

	op1 &= 0x7;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		uint32_t *reg32;
		reg32 = DECODE_RM_LONG_REGISTER(op1);
		srcval = fetch_long_imm();
		DECODE_PRINTF2(",%x\r\n", srcval);
		TRACE_AND_STEP();
		*reg32 = srcval;
	} else {
		uint16_t *reg16;
		reg16 = DECODE_RM_WORD_REGISTER(op1);
		srcval = fetch_word_imm();
		DECODE_PRINTF2(",%x\r\n", srcval);
		TRACE_AND_STEP();
		*reg16 = (uint16_t)srcval;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc0
 ****************************************************************************/
void x86emuOp_opcC0_byte_RM_MEM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg;
	unsigned int destoffset;
	uint8_t destval;
	uint8_t amt;

	/*
	 * Yet another weirdo special case instruction format.  Part of
	 * the opcode held below in "RH".  Doubly nested case would
	 * result, except that the decoded instruction
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */

		switch (rh) {
			case 0:
				DECODE_PRINTF("ROL\t");
				break;
			case 1:
				DECODE_PRINTF("ROR\t");
				break;
			case 2:
				DECODE_PRINTF("RCL\t");
				break;
			case 3:
				DECODE_PRINTF("RCR\t");
				break;
			case 4:
				DECODE_PRINTF("SHL\t");
				break;
			case 5:
				DECODE_PRINTF("SHR\t");
				break;
			case 6:
				DECODE_PRINTF("SAL\t");
				break;
			case 7:
				DECODE_PRINTF("SAR\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	if (mod < 3) {
		DECODE_PRINTF("BYTE PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		amt = fetch_byte_imm();
		DECODE_PRINTF2(",%x\r\n", amt);
		destval = fetch_data_byte(destoffset);
		TRACE_AND_STEP();
		destval = (*opcD0_byte_operation[rh]) (destval, amt);
		store_data_byte(destoffset, destval);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		amt = fetch_byte_imm();
		DECODE_PRINTF2(",%x\r\n", amt);
		TRACE_AND_STEP();
		destval = (*opcD0_byte_operation[rh]) (*destreg, amt);
		*destreg = destval;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc1
 ****************************************************************************/
void x86emuOp_opcC1_word_RM_MEM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;
	uint8_t amt;

	/*
	 * Yet another weirdo special case instruction format.  Part of
	 * the opcode held below in "RH".  Doubly nested case would
	 * result, except that the decoded instruction
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */

		switch (rh) {
			case 0:
				DECODE_PRINTF("ROL\t");
				break;
			case 1:
				DECODE_PRINTF("ROR\t");
				break;
			case 2:
				DECODE_PRINTF("RCL\t");
				break;
			case 3:
				DECODE_PRINTF("RCR\t");
				break;
			case 4:
				DECODE_PRINTF("SHL\t");
				break;
			case 5:
				DECODE_PRINTF("SHR\t");
				break;
			case 6:
				DECODE_PRINTF("SAL\t");
				break;
			case 7:
				DECODE_PRINTF("SAR\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	if (mod < 3) {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval;

			DECODE_PRINTF("DWORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			amt = fetch_byte_imm();
			DECODE_PRINTF2(",%x\r\n", amt);
			destval = fetch_data_long(destoffset);
			TRACE_AND_STEP();
			destval = (*opcD1_long_operation[rh]) (destval, amt);
			store_data_long(destoffset, destval);
		} else {
			uint16_t destval;

			DECODE_PRINTF("WORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			amt = fetch_byte_imm();
			DECODE_PRINTF2(",%x\r\n", amt);
			destval = fetch_data_word(destoffset);
			TRACE_AND_STEP();
			destval = (*opcD1_word_operation[rh]) (destval, amt);
			store_data_word(destoffset, destval);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			amt = fetch_byte_imm();
			DECODE_PRINTF2(",%x\r\n", amt);
			TRACE_AND_STEP();
			*destreg = (*opcD1_long_operation[rh]) (*destreg, amt);
		} else {
			uint16_t *destreg;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			amt = fetch_byte_imm();
			DECODE_PRINTF2(",%x\r\n", amt);
			TRACE_AND_STEP();
			*destreg = (*opcD1_word_operation[rh]) (*destreg, amt);
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc2
 ****************************************************************************/
void x86emuOp_ret_near_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t imm;

	START_OF_INSTR();
	DECODE_PRINTF("RET\t");
	imm = fetch_word_imm();
	DECODE_PRINTF2("%x\r\n", imm);
	RETURN_TRACE("RET",M.x86.saved_cs,M.x86.saved_ip);
	TRACE_AND_STEP();
	M.x86.R_IP = pop_word();
	M.x86.R_SP += imm;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc3
 ****************************************************************************/
void x86emuOp_ret_near(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("RET\r\n");
	RETURN_TRACE("RET",M.x86.saved_cs,M.x86.saved_ip);
	TRACE_AND_STEP();
	M.x86.R_IP = pop_word();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc4
 ****************************************************************************/
void x86emuOp_les_R_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rh, rl;
	uint16_t *dstreg;
	unsigned int srcoffset;

	START_OF_INSTR();
	DECODE_PRINTF("LES\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		dstreg = DECODE_RM_WORD_REGISTER(rh);
		DECODE_PRINTF(",");
		srcoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*dstreg = fetch_data_word(srcoffset);
		M.x86.R_ES = fetch_data_word(srcoffset + 2);
	}
	/* else UNDEFINED!                   register to register */

	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc5
 ****************************************************************************/
void x86emuOp_lds_R_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rh, rl;
	uint16_t *dstreg;
	unsigned int srcoffset;

	START_OF_INSTR();
	DECODE_PRINTF("LDS\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (mod < 3) {
		dstreg = DECODE_RM_WORD_REGISTER(rh);
		DECODE_PRINTF(",");
		srcoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		*dstreg = fetch_data_word(srcoffset);
		M.x86.R_DS = fetch_data_word(srcoffset + 2);
	}
	/* else UNDEFINED! */
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc6
 ****************************************************************************/
void x86emuOp_mov_byte_RM_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg;
	unsigned int destoffset;
	uint8_t imm;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (rh != 0) {
		DECODE_PRINTF("ILLEGAL DECODE OF OPCODE c6\r\n");
		HALT_SYS();
	}
	if (mod < 3) {
		DECODE_PRINTF("BYTE PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		imm = fetch_byte_imm();
		DECODE_PRINTF2(",%x\r\n", imm);
		TRACE_AND_STEP();
		store_data_byte(destoffset, imm);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		imm = fetch_byte_imm();
		DECODE_PRINTF2(",%x\r\n", imm);
		TRACE_AND_STEP();
		*destreg = imm;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc7
 ****************************************************************************/
void x86emuOp_mov_word_RM_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	START_OF_INSTR();
	DECODE_PRINTF("MOV\t");
	FETCH_DECODE_MODRM(mod, rh, rl);
	if (rh != 0) {
		DECODE_PRINTF("ILLEGAL DECODE OF OPCODE 8F\r\n");
		HALT_SYS();
	}
	if (mod < 3) {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t imm;

			DECODE_PRINTF("DWORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			imm = fetch_long_imm();
			DECODE_PRINTF2(",%x\r\n", imm);
			TRACE_AND_STEP();
			store_data_long(destoffset, imm);
		} else {
			uint16_t imm;

			DECODE_PRINTF("WORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			imm = fetch_word_imm();
			DECODE_PRINTF2(",%x\r\n", imm);
			TRACE_AND_STEP();
			store_data_word(destoffset, imm);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg;
			uint32_t imm;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			imm = fetch_long_imm();
			DECODE_PRINTF2(",%x\r\n", imm);
			TRACE_AND_STEP();
			*destreg = imm;
		} else {
			uint16_t *destreg;
			uint16_t imm;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			imm = fetch_word_imm();
			DECODE_PRINTF2(",%x\r\n", imm);
			TRACE_AND_STEP();
			*destreg = imm;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc8
 ****************************************************************************/
void x86emuOp_enter(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t local,frame_pointer;
	uint8_t  nesting;
	int i;

	START_OF_INSTR();
	local = fetch_word_imm();
	nesting = fetch_byte_imm();
	DECODE_PRINTF2("ENTER %x\r\n", local);
	DECODE_PRINTF2(",%x\r\n", nesting);
	TRACE_AND_STEP();
	push_word(M.x86.R_BP);
	frame_pointer = M.x86.R_SP;
	if (nesting > 0) {
		for (i = 1; i < nesting; i++) {
			M.x86.R_BP -= 2;
			push_word(fetch_data_word_abs(M.x86.R_SS, M.x86.R_BP));
		}
		push_word(frame_pointer);
	}
	M.x86.R_BP = frame_pointer;
	M.x86.R_SP = (uint16_t)(M.x86.R_SP - local);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xc9
 ****************************************************************************/
void x86emuOp_leave(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("LEAVE\r\n");
	TRACE_AND_STEP();
	M.x86.R_SP = M.x86.R_BP;
	M.x86.R_BP = pop_word();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xca
 ****************************************************************************/
void x86emuOp_ret_far_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t imm;

	START_OF_INSTR();
	DECODE_PRINTF("RETF\t");
	imm = fetch_word_imm();
	DECODE_PRINTF2("%x\r\n", imm);
	RETURN_TRACE("RETF",M.x86.saved_cs,M.x86.saved_ip);
	TRACE_AND_STEP();
	M.x86.R_IP = pop_word();
	M.x86.R_CS = pop_word();
	M.x86.R_SP += imm;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xcb
 ****************************************************************************/
void x86emuOp_ret_far(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("RETF\r\n");
	RETURN_TRACE("RETF",M.x86.saved_cs,M.x86.saved_ip);
	TRACE_AND_STEP();
	M.x86.R_IP = pop_word();
	M.x86.R_CS = pop_word();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xcc
 ****************************************************************************/
void x86emuOp_int3(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t tmp;

	START_OF_INSTR();
	DECODE_PRINTF("INT 3\r\n");
	tmp = (uint16_t) mem_access_word(3 * 4 + 2);
	/* access the segment register */
	TRACE_AND_STEP();
	if (_X86EMU_intrTab[3]) {
		(*_X86EMU_intrTab[3])(3);
	} else {
		push_word((uint16_t)M.x86.R_FLG);
		CLEAR_FLAG(F_IF);
		CLEAR_FLAG(F_TF);
		push_word(M.x86.R_CS);
		M.x86.R_CS = mem_access_word(3 * 4 + 2);
		push_word(M.x86.R_IP);
		M.x86.R_IP = mem_access_word(3 * 4);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xcd
 ****************************************************************************/
void x86emuOp_int_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t tmp;
	uint8_t intnum;

	START_OF_INSTR();
	DECODE_PRINTF("INT\t");
	intnum = fetch_byte_imm();
	DECODE_PRINTF2("%x\r\n", intnum);
	tmp = mem_access_word(intnum * 4 + 2);
	TRACE_AND_STEP();
	if (_X86EMU_intrTab[intnum]) {
		(*_X86EMU_intrTab[intnum])(intnum);
	} else {
		push_word((uint16_t)M.x86.R_FLG);
		CLEAR_FLAG(F_IF);
		CLEAR_FLAG(F_TF);
		push_word(M.x86.R_CS);
		M.x86.R_CS = mem_access_word(intnum * 4 + 2);
		push_word(M.x86.R_IP);
		M.x86.R_IP = mem_access_word(intnum * 4);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xce
 ****************************************************************************/
void x86emuOp_into(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t tmp;

	START_OF_INSTR();
	DECODE_PRINTF("INTO\r\n");
	TRACE_AND_STEP();
	if (ACCESS_FLAG(F_OF)) {
		tmp = mem_access_word(4 * 4 + 2);
		if (_X86EMU_intrTab[4]) {
			(*_X86EMU_intrTab[4])(4);
		} else {
			push_word((uint16_t)M.x86.R_FLG);
			CLEAR_FLAG(F_IF);
			CLEAR_FLAG(F_TF);
			push_word(M.x86.R_CS);
			M.x86.R_CS = mem_access_word(4 * 4 + 2);
			push_word(M.x86.R_IP);
			M.x86.R_IP = mem_access_word(4 * 4);
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xcf
 ****************************************************************************/
void x86emuOp_iret(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("IRET\r\n");

	TRACE_AND_STEP();

	M.x86.R_IP = pop_word();
	M.x86.R_CS = pop_word();
	M.x86.R_FLG = pop_word();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xd0
 ****************************************************************************/
void x86emuOp_opcD0_byte_RM_1(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg;
	unsigned int destoffset;
	uint8_t destval;

	/*
	 * Yet another weirdo special case instruction format.  Part of
	 * the opcode held below in "RH".  Doubly nested case would
	 * result, except that the decoded instruction
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */
		switch (rh) {
			case 0:
				DECODE_PRINTF("ROL\t");
				break;
			case 1:
				DECODE_PRINTF("ROR\t");
				break;
			case 2:
				DECODE_PRINTF("RCL\t");
				break;
			case 3:
				DECODE_PRINTF("RCR\t");
				break;
			case 4:
				DECODE_PRINTF("SHL\t");
				break;
			case 5:
				DECODE_PRINTF("SHR\t");
				break;
			case 6:
				DECODE_PRINTF("SAL\t");
				break;
			case 7:
				DECODE_PRINTF("SAR\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	if (mod < 3) {
		DECODE_PRINTF("BYTE PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF(",1\r\n");
		destval = fetch_data_byte(destoffset);
		TRACE_AND_STEP();
		destval = (*opcD0_byte_operation[rh]) (destval, 1);
		store_data_byte(destoffset, destval);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF(",1\r\n");
		TRACE_AND_STEP();
		destval = (*opcD0_byte_operation[rh]) (*destreg, 1);
		*destreg = destval;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xd1
 ****************************************************************************/
void x86emuOp_opcD1_word_RM_1(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	/*
	 * Yet another weirdo special case instruction format.  Part of
	 * the opcode held below in "RH".  Doubly nested case would
	 * result, except that the decoded instruction
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */
		switch (rh) {
			case 0:
				DECODE_PRINTF("ROL\t");
				break;
			case 1:
				DECODE_PRINTF("ROR\t");
				break;
			case 2:
				DECODE_PRINTF("RCL\t");
				break;
			case 3:
				DECODE_PRINTF("RCR\t");
				break;
			case 4:
				DECODE_PRINTF("SHL\t");
				break;
			case 5:
				DECODE_PRINTF("SHR\t");
				break;
			case 6:
				DECODE_PRINTF("SAL\t");
				break;
			case 7:
				DECODE_PRINTF("SAR\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	if (mod < 3) {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval;

			DECODE_PRINTF("DWORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			DECODE_PRINTF(",1\r\n");
			destval = fetch_data_long(destoffset);
			TRACE_AND_STEP();
			destval = (*opcD1_long_operation[rh]) (destval, 1);
			store_data_long(destoffset, destval);
		} else {
			uint16_t destval;

			DECODE_PRINTF("WORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			DECODE_PRINTF(",1\r\n");
			destval = fetch_data_word(destoffset);
			TRACE_AND_STEP();
			destval = (*opcD1_word_operation[rh]) (destval, 1);
			store_data_word(destoffset, destval);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval;
			uint32_t *destreg;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF(",1\r\n");
			TRACE_AND_STEP();
			destval = (*opcD1_long_operation[rh]) (*destreg, 1);
			*destreg = destval;
		} else {
			uint16_t destval;
			uint16_t *destreg;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF(",1\r\n");
			TRACE_AND_STEP();
			destval = (*opcD1_word_operation[rh]) (*destreg, 1);
			*destreg = destval;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xd2
 ****************************************************************************/
void x86emuOp_opcD2_byte_RM_CL(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg;
	unsigned int destoffset;
	uint8_t destval;
	uint8_t amt;

	/*
	 * Yet another weirdo special case instruction format.  Part of
	 * the opcode held below in "RH".  Doubly nested case would
	 * result, except that the decoded instruction
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */
		switch (rh) {
			case 0:
				DECODE_PRINTF("ROL\t");
				break;
			case 1:
				DECODE_PRINTF("ROR\t");
				break;
			case 2:
				DECODE_PRINTF("RCL\t");
				break;
			case 3:
				DECODE_PRINTF("RCR\t");
				break;
			case 4:
				DECODE_PRINTF("SHL\t");
				break;
			case 5:
				DECODE_PRINTF("SHR\t");
				break;
			case 6:
				DECODE_PRINTF("SAL\t");
				break;
			case 7:
				DECODE_PRINTF("SAR\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	amt = M.x86.R_CL;
	if (mod < 3) {
		DECODE_PRINTF("BYTE PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF(",CL\r\n");
		destval = fetch_data_byte(destoffset);
		TRACE_AND_STEP();
		destval = (*opcD0_byte_operation[rh]) (destval, amt);
		store_data_byte(destoffset, destval);
	} else {                     /* register to register */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF(",CL\r\n");
		TRACE_AND_STEP();
		destval = (*opcD0_byte_operation[rh]) (*destreg, amt);
		*destreg = destval;
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xd3
 ****************************************************************************/
void x86emuOp_opcD3_word_RM_CL(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;
	uint8_t amt;

	/*
	 * Yet another weirdo special case instruction format.  Part of
	 * the opcode held below in "RH".  Doubly nested case would
	 * result, except that the decoded instruction
	 */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */
		switch (rh) {
			case 0:
				DECODE_PRINTF("ROL\t");
				break;
			case 1:
				DECODE_PRINTF("ROR\t");
				break;
			case 2:
				DECODE_PRINTF("RCL\t");
				break;
			case 3:
				DECODE_PRINTF("RCR\t");
				break;
			case 4:
				DECODE_PRINTF("SHL\t");
				break;
			case 5:
				DECODE_PRINTF("SHR\t");
				break;
			case 6:
				DECODE_PRINTF("SAL\t");
				break;
			case 7:
				DECODE_PRINTF("SAR\t");
				break;
		}
	}
#endif
	/* know operation, decode the mod byte to find the addressing
	   mode. */
	amt = M.x86.R_CL;
	if (mod < 3) {
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval;

			DECODE_PRINTF("DWORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			DECODE_PRINTF(",CL\r\n");
			destval = fetch_data_long(destoffset);
			TRACE_AND_STEP();
			destval = (*opcD1_long_operation[rh]) (destval, amt);
			store_data_long(destoffset, destval);
		} else {
			uint16_t destval;

			DECODE_PRINTF("WORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			DECODE_PRINTF(",CL\r\n");
			destval = fetch_data_word(destoffset);
			TRACE_AND_STEP();
			destval = (*opcD1_word_operation[rh]) (destval, amt);
			store_data_word(destoffset, destval);
		}
	} else {                     /* register to register */
		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg;

			destreg = DECODE_RM_LONG_REGISTER(rl);
			DECODE_PRINTF(",CL\r\n");
			TRACE_AND_STEP();
			*destreg = (*opcD1_long_operation[rh]) (*destreg, amt);
		} else {
			uint16_t *destreg;

			destreg = DECODE_RM_WORD_REGISTER(rl);
			DECODE_PRINTF(",CL\r\n");
			TRACE_AND_STEP();
			*destreg = (*opcD1_word_operation[rh]) (*destreg, amt);
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xd4
 ****************************************************************************/
void x86emuOp_aam(uint8_t X86EMU_UNUSED(op1))
{
	uint8_t a;

	START_OF_INSTR();
	DECODE_PRINTF("AAM\r\n");
	a = fetch_byte_imm();      /* this is a stupid encoding. */
	if (a != 10) {
		DECODE_PRINTF("ERROR DECODING AAM\r\n");
		TRACE_REGS();
		HALT_SYS();
	}
	TRACE_AND_STEP();
	/* note the type change here --- returning AL and AH in AX. */
	M.x86.R_AX = aam_word(M.x86.R_AL);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xd5
 ****************************************************************************/
void x86emuOp_aad(uint8_t X86EMU_UNUSED(op1))
{
	uint8_t a;

	START_OF_INSTR();
	DECODE_PRINTF("AAD\r\n");
	a = fetch_byte_imm();
	TRACE_AND_STEP();
	M.x86.R_AX = aad_word(M.x86.R_AX);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/* opcode 0xd6 ILLEGAL OPCODE */

/****************************************************************************
REMARKS:
Handles opcode 0xd7
 ****************************************************************************/
void x86emuOp_xlat(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t addr;

	START_OF_INSTR();
	DECODE_PRINTF("XLAT\r\n");
	TRACE_AND_STEP();
	addr = (uint16_t)(M.x86.R_BX + (uint8_t)M.x86.R_AL);
	M.x86.R_AL = fetch_data_byte(addr);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/* instuctions  D8 .. DF are in i87_ops.c */

/****************************************************************************
REMARKS:
Handles opcode 0xe0
 ****************************************************************************/
void x86emuOp_loopne(uint8_t X86EMU_UNUSED(op1))
{
	int16_t ip;

	START_OF_INSTR();
	DECODE_PRINTF("LOOPNE\t");
	ip = (int8_t) fetch_byte_imm();
	ip += (int16_t) M.x86.R_IP;
	DECODE_PRINTF2("%x\r\n", ip);
	TRACE_AND_STEP();
	M.x86.R_CX -= 1;
	if (M.x86.R_CX != 0 && !ACCESS_FLAG(F_ZF))      /* CX != 0 and !ZF */
		M.x86.R_IP = ip;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe1
 ****************************************************************************/
void x86emuOp_loope(uint8_t X86EMU_UNUSED(op1))
{
	int16_t ip;

	START_OF_INSTR();
	DECODE_PRINTF("LOOPE\t");
	ip = (int8_t) fetch_byte_imm();
	ip += (int16_t) M.x86.R_IP;
	DECODE_PRINTF2("%x\r\n", ip);
	TRACE_AND_STEP();
	M.x86.R_CX -= 1;
	if (M.x86.R_CX != 0 && ACCESS_FLAG(F_ZF))       /* CX != 0 and ZF */
		M.x86.R_IP = ip;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe2
 ****************************************************************************/
void x86emuOp_loop(uint8_t X86EMU_UNUSED(op1))
{
	int16_t ip;

	START_OF_INSTR();
	DECODE_PRINTF("LOOP\t");
	ip = (int8_t) fetch_byte_imm();
	ip += (int16_t) M.x86.R_IP;
	DECODE_PRINTF2("%x\r\n", ip);
	TRACE_AND_STEP();
	M.x86.R_CX -= 1;
	if (M.x86.R_CX != 0)
		M.x86.R_IP = ip;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe3
 ****************************************************************************/
void x86emuOp_jcxz(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t target;
	int8_t  offset;

	/* jump to byte offset if overflow flag is set */
	START_OF_INSTR();
	DECODE_PRINTF("JCXZ\t");
	offset = (int8_t)fetch_byte_imm();
	target = (uint16_t)(M.x86.R_IP + offset);
	DECODE_PRINTF2("%x\r\n", target);
	TRACE_AND_STEP();
	if (M.x86.R_CX == 0)
		M.x86.R_IP = target;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe4
 ****************************************************************************/
void x86emuOp_in_byte_AL_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint8_t port;

	START_OF_INSTR();
	DECODE_PRINTF("IN\t");
	port = (uint8_t) fetch_byte_imm();
	DECODE_PRINTF2("%x,AL\r\n", port);
	TRACE_AND_STEP();
	M.x86.R_AL = (*sys_inb)(port);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe5
 ****************************************************************************/
void x86emuOp_in_word_AX_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint8_t port;

	START_OF_INSTR();
	DECODE_PRINTF("IN\t");
	port = (uint8_t) fetch_byte_imm();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF2("EAX,%x\r\n", port);
	} else {
		DECODE_PRINTF2("AX,%x\r\n", port);
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		M.x86.R_EAX = (*sys_inl)(port);
	} else {
		M.x86.R_AX = (*sys_inw)(port);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe6
 ****************************************************************************/
void x86emuOp_out_byte_IMM_AL(uint8_t X86EMU_UNUSED(op1))
{
	uint8_t port;

	START_OF_INSTR();
	DECODE_PRINTF("OUT\t");
	port = (uint8_t) fetch_byte_imm();
	DECODE_PRINTF2("%x,AL\r\n", port);
	TRACE_AND_STEP();
	(*sys_outb)(port, M.x86.R_AL);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe7
 ****************************************************************************/
void x86emuOp_out_word_IMM_AX(uint8_t X86EMU_UNUSED(op1))
{
	uint8_t port;

	START_OF_INSTR();
	DECODE_PRINTF("OUT\t");
	port = (uint8_t) fetch_byte_imm();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF2("%x,EAX\r\n", port);
	} else {
		DECODE_PRINTF2("%x,AX\r\n", port);
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		(*sys_outl)(port, M.x86.R_EAX);
	} else {
		(*sys_outw)(port, M.x86.R_AX);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe8
 ****************************************************************************/
void x86emuOp_call_near_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int16_t ip;

	START_OF_INSTR();
	DECODE_PRINTF("CALL\t");
	ip = (int16_t) fetch_word_imm();
	ip += (int16_t) M.x86.R_IP;    /* CHECK SIGN */
	DECODE_PRINTF2("%x\r\n", ip);
	CALL_TRACE(M.x86.saved_cs, M.x86.saved_ip, M.x86.R_CS, ip, "");
	TRACE_AND_STEP();
	push_word(M.x86.R_IP);
	M.x86.R_IP = ip;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xe9
 ****************************************************************************/
void x86emuOp_jump_near_IMM(uint8_t X86EMU_UNUSED(op1))
{
	int ip;

	START_OF_INSTR();
	DECODE_PRINTF("JMP\t");
	ip = (int16_t)fetch_word_imm();
	ip += (int16_t)M.x86.R_IP;
	DECODE_PRINTF2("%x\r\n", ip);
	TRACE_AND_STEP();
	M.x86.R_IP = (uint16_t)ip;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xea
 ****************************************************************************/
void x86emuOp_jump_far_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t cs, ip;

	START_OF_INSTR();
	DECODE_PRINTF("JMP\tFAR ");
	ip = fetch_word_imm();
	cs = fetch_word_imm();
	DECODE_PRINTF2("%x:", cs);
	DECODE_PRINTF2("%x\r\n", ip);
	TRACE_AND_STEP();
	M.x86.R_IP = ip;
	M.x86.R_CS = cs;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xeb
 ****************************************************************************/
void x86emuOp_jump_byte_IMM(uint8_t X86EMU_UNUSED(op1))
{
	uint16_t target;
	int8_t offset;

	START_OF_INSTR();
	DECODE_PRINTF("JMP\t");
	offset = (int8_t)fetch_byte_imm();
	target = (uint16_t)(M.x86.R_IP + offset);
	DECODE_PRINTF2("%x\r\n", target);
	TRACE_AND_STEP();
	M.x86.R_IP = target;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xec
 ****************************************************************************/
void x86emuOp_in_byte_AL_DX(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("IN\tAL,DX\r\n");
	TRACE_AND_STEP();
	M.x86.R_AL = (*sys_inb)(M.x86.R_DX);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xed
 ****************************************************************************/
void x86emuOp_in_word_AX_DX(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("IN\tEAX,DX\r\n");
	} else {
		DECODE_PRINTF("IN\tAX,DX\r\n");
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		M.x86.R_EAX = (*sys_inl)(M.x86.R_DX);
	} else {
		M.x86.R_AX = (*sys_inw)(M.x86.R_DX);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xee
 ****************************************************************************/
void x86emuOp_out_byte_DX_AL(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("OUT\tDX,AL\r\n");
	TRACE_AND_STEP();
	(*sys_outb)(M.x86.R_DX, M.x86.R_AL);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xef
 ****************************************************************************/
void x86emuOp_out_word_DX_AX(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	if (M.x86.mode & SYSMODE_PREFIX_DATA) {
		DECODE_PRINTF("OUT\tDX,EAX\r\n");
	} else {
		DECODE_PRINTF("OUT\tDX,AX\r\n");
	}
	TRACE_AND_STEP();
	if (M.x86.mode & SYSMODE_PREFIX_DATA)
		(*sys_outl)(M.x86.R_DX, M.x86.R_EAX);
	else
		(*sys_outw)(M.x86.R_DX, M.x86.R_AX);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xf0
 ****************************************************************************/
void x86emuOp_lock(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("LOCK:\r\n");
	TRACE_AND_STEP();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/*opcode 0xf1 ILLEGAL OPERATION */

/****************************************************************************
REMARKS:
Handles opcode 0xf2
 ****************************************************************************/
void x86emuOp_repne(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("REPNE\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_PREFIX_REPNE;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xf3
 ****************************************************************************/
void x86emuOp_repe(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("REPE\r\n");
	TRACE_AND_STEP();
	M.x86.mode |= SYSMODE_PREFIX_REPE;
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xf4
 ****************************************************************************/
void x86emuOp_halt(uint8_t X86EMU_UNUSED(op1))
{
	START_OF_INSTR();
	DECODE_PRINTF("HALT\r\n");
	TRACE_AND_STEP();
	HALT_SYS();
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xf5
 ****************************************************************************/
void x86emuOp_cmc(uint8_t X86EMU_UNUSED(op1))
{
	/* complement the carry flag. */
	START_OF_INSTR();
	DECODE_PRINTF("CMC\r\n");
	TRACE_AND_STEP();
	TOGGLE_FLAG(F_CF);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xf6
 ****************************************************************************/
void x86emuOp_opcF6_byte_RM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	uint8_t *destreg;
	unsigned int destoffset;
	uint8_t destval, srcval;

	/* long, drawn out code follows.  Double switch for a total
	   of 32 cases.  */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
	DECODE_PRINTF(opF6_names[rh]);
	if (mod < 3) {
		DECODE_PRINTF("BYTE PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		destval = fetch_data_byte(destoffset);

		switch (rh) {
			case 0:         /* test byte imm */
				DECODE_PRINTF(",");
				srcval = fetch_byte_imm();
				DECODE_PRINTF2("%x\r\n", srcval);
				TRACE_AND_STEP();
				test_byte(destval, srcval);
				break;
			case 1:
				DECODE_PRINTF("ILLEGAL OP MOD=00 RH=01 OP=F6\r\n");
				HALT_SYS();
				break;
			case 2:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				destval = not_byte(destval);
				store_data_byte(destoffset, destval);
				break;
			case 3:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				destval = neg_byte(destval);
				store_data_byte(destoffset, destval);
				break;
			case 4:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				mul_byte(destval);
				break;
			case 5:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				imul_byte(destval);
				break;
			case 6:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				div_byte(destval);
				break;
			default:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				idiv_byte(destval);
				break;
		}
	} else {                     /* mod=11 */
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		switch (rh) {
			case 0:         /* test byte imm */
				DECODE_PRINTF(",");
				srcval = fetch_byte_imm();
				DECODE_PRINTF2("%x\r\n", srcval);
				TRACE_AND_STEP();
				test_byte(*destreg, srcval);
				break;
			case 1:
				DECODE_PRINTF("ILLEGAL OP MOD=00 RH=01 OP=F6\r\n");
				HALT_SYS();
				break;
			case 2:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				*destreg = not_byte(*destreg);
				break;
			case 3:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				*destreg = neg_byte(*destreg);
				break;
			case 4:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				mul_byte(*destreg);      /*!!!  */
				break;
			case 5:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				imul_byte(*destreg);
				break;
			case 6:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				div_byte(*destreg);
				break;
			default:
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				idiv_byte(*destreg);
				break;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xf7
 ****************************************************************************/
void x86emuOp_opcF7_word_RM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rl, rh;
	unsigned int destoffset;

	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
	DECODE_PRINTF(opF6_names[rh]);
	if (mod < 3) {

		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t destval, srcval;

			DECODE_PRINTF("DWORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			destval = fetch_data_long(destoffset);

			switch (rh) {
				case 0:
					DECODE_PRINTF(",");
					srcval = fetch_long_imm();
					DECODE_PRINTF2("%x\r\n", srcval);
					TRACE_AND_STEP();
					test_long(destval, srcval);
					break;
				case 1:
					DECODE_PRINTF("ILLEGAL OP MOD=00 RH=01 OP=F7\r\n");
					HALT_SYS();
					break;
				case 2:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					destval = not_long(destval);
					store_data_long(destoffset, destval);
					break;
				case 3:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					destval = neg_long(destval);
					store_data_long(destoffset, destval);
					break;
				case 4:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					mul_long(destval);
					break;
				case 5:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					imul_long(destval);
					break;
				case 6:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					div_long(destval);
					break;
				case 7:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					idiv_long(destval);
					break;
			}
		} else {
			uint16_t destval, srcval;

			DECODE_PRINTF("WORD PTR ");
			destoffset = decode_rmXX_address(mod, rl);
			destval = fetch_data_word(destoffset);

			switch (rh) {
				case 0:         /* test word imm */
					DECODE_PRINTF(",");
					srcval = fetch_word_imm();
					DECODE_PRINTF2("%x\r\n", srcval);
					TRACE_AND_STEP();
					test_word(destval, srcval);
					break;
				case 1:
					DECODE_PRINTF("ILLEGAL OP MOD=00 RH=01 OP=F7\r\n");
					HALT_SYS();
					break;
				case 2:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					destval = not_word(destval);
					store_data_word(destoffset, destval);
					break;
				case 3:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					destval = neg_word(destval);
					store_data_word(destoffset, destval);
					break;
				case 4:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					mul_word(destval);
					break;
				case 5:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					imul_word(destval);
					break;
				case 6:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					div_word(destval);
					break;
				case 7:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					idiv_word(destval);
					break;
			}
		}

	} else {                     /* mod=11 */

		if (M.x86.mode & SYSMODE_PREFIX_DATA) {
			uint32_t *destreg;
			uint32_t srcval;

			destreg = DECODE_RM_LONG_REGISTER(rl);

			switch (rh) {
				case 0:         /* test word imm */
					DECODE_PRINTF(",");
					srcval = fetch_long_imm();
					DECODE_PRINTF2("%x\r\n", srcval);
					TRACE_AND_STEP();
					test_long(*destreg, srcval);
					break;
				case 1:
					DECODE_PRINTF("ILLEGAL OP MOD=00 RH=01 OP=F6\r\n");
					HALT_SYS();
					break;
				case 2:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					*destreg = not_long(*destreg);
					break;
				case 3:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					*destreg = neg_long(*destreg);
					break;
				case 4:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					mul_long(*destreg);      /*!!!  */
					break;
				case 5:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					imul_long(*destreg);
					break;
				case 6:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					div_long(*destreg);
					break;
				case 7:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					idiv_long(*destreg);
					break;
			}
		} else {
			uint16_t *destreg;
			uint16_t srcval;

			destreg = DECODE_RM_WORD_REGISTER(rl);

			switch (rh) {
				case 0:         /* test word imm */
					DECODE_PRINTF(",");
					srcval = fetch_word_imm();
					DECODE_PRINTF2("%x\r\n", srcval);
					TRACE_AND_STEP();
					test_word(*destreg, srcval);
					break;
				case 1:
					DECODE_PRINTF("ILLEGAL OP MOD=00 RH=01 OP=F6\r\n");
					HALT_SYS();
					break;
				case 2:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					*destreg = not_word(*destreg);
					break;
				case 3:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					*destreg = neg_word(*destreg);
					break;
				case 4:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					mul_word(*destreg);      /*!!!  */
					break;
				case 5:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					imul_word(*destreg);
					break;
				case 6:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					div_word(*destreg);
					break;
				case 7:
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					idiv_word(*destreg);
					break;
			}
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xf8
 ****************************************************************************/
void x86emuOp_clc(uint8_t X86EMU_UNUSED(op1))
{
	/* clear the carry flag. */
	START_OF_INSTR();
	DECODE_PRINTF("CLC\r\n");
	TRACE_AND_STEP();
	CLEAR_FLAG(F_CF);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xf9
 ****************************************************************************/
void x86emuOp_stc(uint8_t X86EMU_UNUSED(op1))
{
	/* set the carry flag. */
	START_OF_INSTR();
	DECODE_PRINTF("STC\r\n");
	TRACE_AND_STEP();
	SET_FLAG(F_CF);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xfa
 ****************************************************************************/
void x86emuOp_cli(uint8_t X86EMU_UNUSED(op1))
{
	/* clear interrupts. */
	START_OF_INSTR();
	DECODE_PRINTF("CLI\r\n");
	TRACE_AND_STEP();
	CLEAR_FLAG(F_IF);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xfb
 ****************************************************************************/
void x86emuOp_sti(uint8_t X86EMU_UNUSED(op1))
{
	/* enable  interrupts. */
	START_OF_INSTR();
	DECODE_PRINTF("STI\r\n");
	TRACE_AND_STEP();
	SET_FLAG(F_IF);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xfc
 ****************************************************************************/
void x86emuOp_cld(uint8_t X86EMU_UNUSED(op1))
{
	/* clear interrupts. */
	START_OF_INSTR();
	DECODE_PRINTF("CLD\r\n");
	TRACE_AND_STEP();
	CLEAR_FLAG(F_DF);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xfd
 ****************************************************************************/
void x86emuOp_std(uint8_t X86EMU_UNUSED(op1))
{
	/* clear interrupts. */
	START_OF_INSTR();
	DECODE_PRINTF("STD\r\n");
	TRACE_AND_STEP();
	SET_FLAG(F_DF);
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xfe
 ****************************************************************************/
void x86emuOp_opcFE_byte_RM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rh, rl;
	uint8_t destval;
	unsigned int destoffset;
	uint8_t *destreg;

	/* Yet another special case instruction. */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */

		switch (rh) {
			case 0:
				DECODE_PRINTF("INC\t");
				break;
			case 1:
				DECODE_PRINTF("DEC\t");
				break;
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				DECODE_PRINTF2("ILLEGAL OP MAJOR OP 0xFE MINOR OP %x \r\n", mod);
				HALT_SYS();
				break;
		}
	}
#endif
	if (mod < 3) {
		DECODE_PRINTF("BYTE PTR ");
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF("\r\n");
		destval = fetch_data_byte(destoffset);
		TRACE_AND_STEP();
		if (rh == 0)
			destval = inc_byte(destval);
		else
			destval = dec_byte(destval);
		store_data_byte(destoffset, destval);
	} else {
		destreg = DECODE_RM_BYTE_REGISTER(rl);
		DECODE_PRINTF("\r\n");
		TRACE_AND_STEP();
		if (rh == 0)
			*destreg = inc_byte(*destreg);
		else
			*destreg = dec_byte(*destreg);
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/****************************************************************************
REMARKS:
Handles opcode 0xff
 ****************************************************************************/
void x86emuOp_opcFF_word_RM(uint8_t X86EMU_UNUSED(op1))
{
	int mod, rh, rl;
	unsigned int destoffset = 0;
	uint16_t *destreg;
	uint16_t destval,destval2;

	/* Yet another special case instruction. */
	START_OF_INSTR();
	FETCH_DECODE_MODRM(mod, rh, rl);
#ifdef DEBUG
	if (DEBUG_DECODE()) {
		/* XXX DECODE_PRINTF may be changed to something more
		   general, so that it is important to leave the strings
		   in the same format, even though the result is that the
		   above test is done twice. */

		switch (rh) {
			case 0:
				if (M.x86.mode & SYSMODE_PREFIX_DATA) {
					DECODE_PRINTF("INC\tDWORD PTR ");
				} else {
					DECODE_PRINTF("INC\tWORD PTR ");
				}
				break;
			case 1:
				if (M.x86.mode & SYSMODE_PREFIX_DATA) {
					DECODE_PRINTF("DEC\tDWORD PTR ");
				} else {
					DECODE_PRINTF("DEC\tWORD PTR ");
				}
				break;
			case 2:
				DECODE_PRINTF("CALL\t ");
				break;
			case 3:
				DECODE_PRINTF("CALL\tFAR ");
				break;
			case 4:
				DECODE_PRINTF("JMP\t");
				break;
			case 5:
				DECODE_PRINTF("JMP\tFAR ");
				break;
			case 6:
				DECODE_PRINTF("PUSH\t");
				break;
			case 7:
				DECODE_PRINTF("ILLEGAL DECODING OF OPCODE FF\t");
				HALT_SYS();
				break;
		}
	}
#endif
	if (mod < 3) {
		destoffset = decode_rmXX_address(mod, rl);
		DECODE_PRINTF("\r\n");
		switch (rh) {
			case 0:         /* inc word ptr ... */
				if (M.x86.mode & SYSMODE_PREFIX_DATA) {
					uint32_t destval;

					destval = fetch_data_long(destoffset);
					TRACE_AND_STEP();
					destval = inc_long(destval);
					store_data_long(destoffset, destval);
				} else {
					uint16_t destval;

					destval = fetch_data_word(destoffset);
					TRACE_AND_STEP();
					destval = inc_word(destval);
					store_data_word(destoffset, destval);
				}
				break;
			case 1:         /* dec word ptr ... */
				if (M.x86.mode & SYSMODE_PREFIX_DATA) {
					uint32_t destval;

					destval = fetch_data_long(destoffset);
					TRACE_AND_STEP();
					destval = dec_long(destval);
					store_data_long(destoffset, destval);
				} else {
					uint16_t destval;

					destval = fetch_data_word(destoffset);
					TRACE_AND_STEP();
					destval = dec_word(destval);
					store_data_word(destoffset, destval);
				}
				break;
			case 2:         /* call word ptr ... */
				destval = fetch_data_word(destoffset);
				TRACE_AND_STEP();
				push_word(M.x86.R_IP);
				M.x86.R_IP = destval;
				break;
			case 3:         /* call far ptr ... */
				destval = fetch_data_word(destoffset);
				destval2 = fetch_data_word(destoffset + 2);
				TRACE_AND_STEP();
				push_word(M.x86.R_CS);
				M.x86.R_CS = destval2;
				push_word(M.x86.R_IP);
				M.x86.R_IP = destval;
				break;
			case 4:         /* jmp word ptr ... */
				destval = fetch_data_word(destoffset);
				TRACE_AND_STEP();
				M.x86.R_IP = destval;
				break;
			case 5:         /* jmp far ptr ... */
				destval = fetch_data_word(destoffset);
				destval2 = fetch_data_word(destoffset + 2);
				TRACE_AND_STEP();
				M.x86.R_IP = destval;
				M.x86.R_CS = destval2;
				break;
			case 6:         /*  push word ptr ... */
				if (M.x86.mode & SYSMODE_PREFIX_DATA) {
					uint32_t destval;

					destval = fetch_data_long(destoffset);
					TRACE_AND_STEP();
					push_long(destval);
				} else {
					uint16_t destval;

					destval = fetch_data_word(destoffset);
					TRACE_AND_STEP();
					push_word(destval);
				}
				break;
		}
	} else {
		switch (rh) {
			case 0:
				if (M.x86.mode & SYSMODE_PREFIX_DATA) {
					uint32_t *destreg;

					destreg = DECODE_RM_LONG_REGISTER(rl);
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					*destreg = inc_long(*destreg);
				} else {
					uint16_t *destreg;

					destreg = DECODE_RM_WORD_REGISTER(rl);
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					*destreg = inc_word(*destreg);
				}
				break;
			case 1:
				if (M.x86.mode & SYSMODE_PREFIX_DATA) {
					uint32_t *destreg;

					destreg = DECODE_RM_LONG_REGISTER(rl);
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					*destreg = dec_long(*destreg);
				} else {
					uint16_t *destreg;

					destreg = DECODE_RM_WORD_REGISTER(rl);
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					*destreg = dec_word(*destreg);
				}
				break;
			case 2:         /* call word ptr ... */
				destreg = DECODE_RM_WORD_REGISTER(rl);
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				push_word(M.x86.R_IP);
				M.x86.R_IP = *destreg;
				break;
			case 3:         /* jmp far ptr ... */
				DECODE_PRINTF("OPERATION UNDEFINED 0XFF \r\n");
				TRACE_AND_STEP();
				HALT_SYS();
				break;

			case 4:         /* jmp  ... */
				destreg = DECODE_RM_WORD_REGISTER(rl);
				DECODE_PRINTF("\r\n");
				TRACE_AND_STEP();
				M.x86.R_IP = (uint16_t) (*destreg);
				break;
			case 5:         /* jmp far ptr ... */
				DECODE_PRINTF("OPERATION UNDEFINED 0XFF \r\n");
				TRACE_AND_STEP();
				HALT_SYS();
				break;
			case 6:
				if (M.x86.mode & SYSMODE_PREFIX_DATA) {
					uint32_t *destreg;

					destreg = DECODE_RM_LONG_REGISTER(rl);
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					push_long(*destreg);
				} else {
					uint16_t *destreg;

					destreg = DECODE_RM_WORD_REGISTER(rl);
					DECODE_PRINTF("\r\n");
					TRACE_AND_STEP();
					push_word(*destreg);
				}
				break;
		}
	}
	DECODE_CLEAR_SEGOVR();
	END_OF_INSTR();
}

/***************************************************************************
 * Single byte operation code table:
 **************************************************************************/
void (*x86emu_optab[256])(uint8_t) =
{
	/*  0x00 */ x86emuOp_genop_byte_RM_R,
	/*  0x01 */ x86emuOp_genop_word_RM_R,
	/*  0x02 */ x86emuOp_genop_byte_R_RM,
	/*  0x03 */ x86emuOp_genop_word_R_RM,
	/*  0x04 */ x86emuOp_genop_byte_AL_IMM,
	/*  0x05 */ x86emuOp_genop_word_AX_IMM,
	/*  0x06 */ x86emuOp_push_ES,
	/*  0x07 */ x86emuOp_pop_ES,

	/*  0x08 */ x86emuOp_genop_byte_RM_R,
	/*  0x09 */ x86emuOp_genop_word_RM_R,
	/*  0x0a */ x86emuOp_genop_byte_R_RM,
	/*  0x0b */ x86emuOp_genop_word_R_RM,
	/*  0x0c */ x86emuOp_genop_byte_AL_IMM,
	/*  0x0d */ x86emuOp_genop_word_AX_IMM,
	/*  0x0e */ x86emuOp_push_CS,
	/*  0x0f */ x86emuOp_two_byte,

	/*  0x10 */ x86emuOp_genop_byte_RM_R,
	/*  0x11 */ x86emuOp_genop_word_RM_R,
	/*  0x12 */ x86emuOp_genop_byte_R_RM,
	/*  0x13 */ x86emuOp_genop_word_R_RM,
	/*  0x14 */ x86emuOp_genop_byte_AL_IMM,
	/*  0x15 */ x86emuOp_genop_word_AX_IMM,
	/*  0x16 */ x86emuOp_push_SS,
	/*  0x17 */ x86emuOp_pop_SS,

	/*  0x18 */ x86emuOp_genop_byte_RM_R,
	/*  0x19 */ x86emuOp_genop_word_RM_R,
	/*  0x1a */ x86emuOp_genop_byte_R_RM,
	/*  0x1b */ x86emuOp_genop_word_R_RM,
	/*  0x1c */ x86emuOp_genop_byte_AL_IMM,
	/*  0x1d */ x86emuOp_genop_word_AX_IMM,
	/*  0x1e */ x86emuOp_push_DS,
	/*  0x1f */ x86emuOp_pop_DS,

	/*  0x20 */ x86emuOp_genop_byte_RM_R,
	/*  0x21 */ x86emuOp_genop_word_RM_R,
	/*  0x22 */ x86emuOp_genop_byte_R_RM,
	/*  0x23 */ x86emuOp_genop_word_R_RM,
	/*  0x24 */ x86emuOp_genop_byte_AL_IMM,
	/*  0x25 */ x86emuOp_genop_word_AX_IMM,
	/*  0x26 */ x86emuOp_segovr_ES,
	/*  0x27 */ x86emuOp_daa,

	/*  0x28 */ x86emuOp_genop_byte_RM_R,
	/*  0x29 */ x86emuOp_genop_word_RM_R,
	/*  0x2a */ x86emuOp_genop_byte_R_RM,
	/*  0x2b */ x86emuOp_genop_word_R_RM,
	/*  0x2c */ x86emuOp_genop_byte_AL_IMM,
	/*  0x2d */ x86emuOp_genop_word_AX_IMM,
	/*  0x2e */ x86emuOp_segovr_CS,
	/*  0x2f */ x86emuOp_das,

	/*  0x30 */ x86emuOp_genop_byte_RM_R,
	/*  0x31 */ x86emuOp_genop_word_RM_R,
	/*  0x32 */ x86emuOp_genop_byte_R_RM,
	/*  0x33 */ x86emuOp_genop_word_R_RM,
	/*  0x34 */ x86emuOp_genop_byte_AL_IMM,
	/*  0x35 */ x86emuOp_genop_word_AX_IMM,
	/*  0x36 */ x86emuOp_segovr_SS,
	/*  0x37 */ x86emuOp_aaa,

	/*  0x38 */ x86emuOp_genop_byte_RM_R,
	/*  0x39 */ x86emuOp_genop_word_RM_R,
	/*  0x3a */ x86emuOp_genop_byte_R_RM,
	/*  0x3b */ x86emuOp_genop_word_R_RM,
	/*  0x3c */ x86emuOp_genop_byte_AL_IMM,
	/*  0x3d */ x86emuOp_genop_word_AX_IMM,
	/*  0x3e */ x86emuOp_segovr_DS,
	/*  0x3f */ x86emuOp_aas,

	/*  0x40 */ x86emuOp_inc_register,
	/*  0x41 */ x86emuOp_inc_register,
	/*  0x42 */ x86emuOp_inc_register,
	/*  0x43 */ x86emuOp_inc_register,
	/*  0x44 */ x86emuOp_inc_register,
	/*  0x45 */ x86emuOp_inc_register,
	/*  0x46 */ x86emuOp_inc_register,
	/*  0x47 */ x86emuOp_inc_register,

	/*  0x48 */ x86emuOp_dec_register,
	/*  0x49 */ x86emuOp_dec_register,
	/*  0x4a */ x86emuOp_dec_register,
	/*  0x4b */ x86emuOp_dec_register,
	/*  0x4c */ x86emuOp_dec_register,
	/*  0x4d */ x86emuOp_dec_register,
	/*  0x4e */ x86emuOp_dec_register,
	/*  0x4f */ x86emuOp_dec_register,

	/*  0x50 */ x86emuOp_push_register,
	/*  0x51 */ x86emuOp_push_register,
	/*  0x52 */ x86emuOp_push_register,
	/*  0x53 */ x86emuOp_push_register,
	/*  0x54 */ x86emuOp_push_register,
	/*  0x55 */ x86emuOp_push_register,
	/*  0x56 */ x86emuOp_push_register,
	/*  0x57 */ x86emuOp_push_register,

	/*  0x58 */ x86emuOp_pop_register,
	/*  0x59 */ x86emuOp_pop_register,
	/*  0x5a */ x86emuOp_pop_register,
	/*  0x5b */ x86emuOp_pop_register,
	/*  0x5c */ x86emuOp_pop_register,
	/*  0x5d */ x86emuOp_pop_register,
	/*  0x5e */ x86emuOp_pop_register,
	/*  0x5f */ x86emuOp_pop_register,

	/*  0x60 */ x86emuOp_push_all,
	/*  0x61 */ x86emuOp_pop_all,
	/*  0x62 */ x86emuOp_illegal_op,   /* bound */
	/*  0x63 */ x86emuOp_illegal_op,   /* arpl */
	/*  0x64 */ x86emuOp_segovr_FS,
	/*  0x65 */ x86emuOp_segovr_GS,
	/*  0x66 */ x86emuOp_prefix_data,
	/*  0x67 */ x86emuOp_prefix_addr,

	/*  0x68 */ x86emuOp_push_word_IMM,
	/*  0x69 */ x86emuOp_imul_word_IMM,
	/*  0x6a */ x86emuOp_push_byte_IMM,
	/*  0x6b */ x86emuOp_imul_byte_IMM,
	/*  0x6c */ x86emuOp_ins_byte,
	/*  0x6d */ x86emuOp_ins_word,
	/*  0x6e */ x86emuOp_outs_byte,
	/*  0x6f */ x86emuOp_outs_word,

	/*  0x70 */ x86emuOp_jump_near_cond,
	/*  0x71 */ x86emuOp_jump_near_cond,
	/*  0x72 */ x86emuOp_jump_near_cond,
	/*  0x73 */ x86emuOp_jump_near_cond,
	/*  0x74 */ x86emuOp_jump_near_cond,
	/*  0x75 */ x86emuOp_jump_near_cond,
	/*  0x76 */ x86emuOp_jump_near_cond,
	/*  0x77 */ x86emuOp_jump_near_cond,

	/*  0x78 */ x86emuOp_jump_near_cond,
	/*  0x79 */ x86emuOp_jump_near_cond,
	/*  0x7a */ x86emuOp_jump_near_cond,
	/*  0x7b */ x86emuOp_jump_near_cond,
	/*  0x7c */ x86emuOp_jump_near_cond,
	/*  0x7d */ x86emuOp_jump_near_cond,
	/*  0x7e */ x86emuOp_jump_near_cond,
	/*  0x7f */ x86emuOp_jump_near_cond,

	/*  0x80 */ x86emuOp_opc80_byte_RM_IMM,
	/*  0x81 */ x86emuOp_opc81_word_RM_IMM,
	/*  0x82 */ x86emuOp_opc82_byte_RM_IMM,
	/*  0x83 */ x86emuOp_opc83_word_RM_IMM,
	/*  0x84 */ x86emuOp_test_byte_RM_R,
	/*  0x85 */ x86emuOp_test_word_RM_R,
	/*  0x86 */ x86emuOp_xchg_byte_RM_R,
	/*  0x87 */ x86emuOp_xchg_word_RM_R,

	/*  0x88 */ x86emuOp_mov_byte_RM_R,
	/*  0x89 */ x86emuOp_mov_word_RM_R,
	/*  0x8a */ x86emuOp_mov_byte_R_RM,
	/*  0x8b */ x86emuOp_mov_word_R_RM,
	/*  0x8c */ x86emuOp_mov_word_RM_SR,
	/*  0x8d */ x86emuOp_lea_word_R_M,
	/*  0x8e */ x86emuOp_mov_word_SR_RM,
	/*  0x8f */ x86emuOp_pop_RM,

	/*  0x90 */ x86emuOp_nop,
	/*  0x91 */ x86emuOp_xchg_word_AX_register,
	/*  0x92 */ x86emuOp_xchg_word_AX_register,
	/*  0x93 */ x86emuOp_xchg_word_AX_register,
	/*  0x94 */ x86emuOp_xchg_word_AX_register,
	/*  0x95 */ x86emuOp_xchg_word_AX_register,
	/*  0x96 */ x86emuOp_xchg_word_AX_register,
	/*  0x97 */ x86emuOp_xchg_word_AX_register,

	/*  0x98 */ x86emuOp_cbw,
	/*  0x99 */ x86emuOp_cwd,
	/*  0x9a */ x86emuOp_call_far_IMM,
	/*  0x9b */ x86emuOp_wait,
	/*  0x9c */ x86emuOp_pushf_word,
	/*  0x9d */ x86emuOp_popf_word,
	/*  0x9e */ x86emuOp_sahf,
	/*  0x9f */ x86emuOp_lahf,

	/*  0xa0 */ x86emuOp_mov_AL_M_IMM,
	/*  0xa1 */ x86emuOp_mov_AX_M_IMM,
	/*  0xa2 */ x86emuOp_mov_M_AL_IMM,
	/*  0xa3 */ x86emuOp_mov_M_AX_IMM,
	/*  0xa4 */ x86emuOp_movs_byte,
	/*  0xa5 */ x86emuOp_movs_word,
	/*  0xa6 */ x86emuOp_cmps_byte,
	/*  0xa7 */ x86emuOp_cmps_word,
	/*  0xa8 */ x86emuOp_test_AL_IMM,
	/*  0xa9 */ x86emuOp_test_AX_IMM,
	/*  0xaa */ x86emuOp_stos_byte,
	/*  0xab */ x86emuOp_stos_word,
	/*  0xac */ x86emuOp_lods_byte,
	/*  0xad */ x86emuOp_lods_word,
	/*  0xac */ x86emuOp_scas_byte,
	/*  0xad */ x86emuOp_scas_word,

	/*  0xb0 */ x86emuOp_mov_byte_register_IMM,
	/*  0xb1 */ x86emuOp_mov_byte_register_IMM,
	/*  0xb2 */ x86emuOp_mov_byte_register_IMM,
	/*  0xb3 */ x86emuOp_mov_byte_register_IMM,
	/*  0xb4 */ x86emuOp_mov_byte_register_IMM,
	/*  0xb5 */ x86emuOp_mov_byte_register_IMM,
	/*  0xb6 */ x86emuOp_mov_byte_register_IMM,
	/*  0xb7 */ x86emuOp_mov_byte_register_IMM,

	/*  0xb8 */ x86emuOp_mov_word_register_IMM,
	/*  0xb9 */ x86emuOp_mov_word_register_IMM,
	/*  0xba */ x86emuOp_mov_word_register_IMM,
	/*  0xbb */ x86emuOp_mov_word_register_IMM,
	/*  0xbc */ x86emuOp_mov_word_register_IMM,
	/*  0xbd */ x86emuOp_mov_word_register_IMM,
	/*  0xbe */ x86emuOp_mov_word_register_IMM,
	/*  0xbf */ x86emuOp_mov_word_register_IMM,

	/*  0xc0 */ x86emuOp_opcC0_byte_RM_MEM,
	/*  0xc1 */ x86emuOp_opcC1_word_RM_MEM,
	/*  0xc2 */ x86emuOp_ret_near_IMM,
	/*  0xc3 */ x86emuOp_ret_near,
	/*  0xc4 */ x86emuOp_les_R_IMM,
	/*  0xc5 */ x86emuOp_lds_R_IMM,
	/*  0xc6 */ x86emuOp_mov_byte_RM_IMM,
	/*  0xc7 */ x86emuOp_mov_word_RM_IMM,
	/*  0xc8 */ x86emuOp_enter,
	/*  0xc9 */ x86emuOp_leave,
	/*  0xca */ x86emuOp_ret_far_IMM,
	/*  0xcb */ x86emuOp_ret_far,
	/*  0xcc */ x86emuOp_int3,
	/*  0xcd */ x86emuOp_int_IMM,
	/*  0xce */ x86emuOp_into,
	/*  0xcf */ x86emuOp_iret,

	/*  0xd0 */ x86emuOp_opcD0_byte_RM_1,
	/*  0xd1 */ x86emuOp_opcD1_word_RM_1,
	/*  0xd2 */ x86emuOp_opcD2_byte_RM_CL,
	/*  0xd3 */ x86emuOp_opcD3_word_RM_CL,
	/*  0xd4 */ x86emuOp_aam,
	/*  0xd5 */ x86emuOp_aad,
	/*  0xd6 */ x86emuOp_illegal_op,   /* Undocumented SETALC instruction */
	/*  0xd7 */ x86emuOp_xlat,
	/*  0xd8 */ x86emuOp_esc_coprocess_d8,
	/*  0xd9 */ x86emuOp_esc_coprocess_d9,
	/*  0xda */ x86emuOp_esc_coprocess_da,
	/*  0xdb */ x86emuOp_esc_coprocess_db,
	/*  0xdc */ x86emuOp_esc_coprocess_dc,
	/*  0xdd */ x86emuOp_esc_coprocess_dd,
	/*  0xde */ x86emuOp_esc_coprocess_de,
	/*  0xdf */ x86emuOp_esc_coprocess_df,

	/*  0xe0 */ x86emuOp_loopne,
	/*  0xe1 */ x86emuOp_loope,
	/*  0xe2 */ x86emuOp_loop,
	/*  0xe3 */ x86emuOp_jcxz,
	/*  0xe4 */ x86emuOp_in_byte_AL_IMM,
	/*  0xe5 */ x86emuOp_in_word_AX_IMM,
	/*  0xe6 */ x86emuOp_out_byte_IMM_AL,
	/*  0xe7 */ x86emuOp_out_word_IMM_AX,

	/*  0xe8 */ x86emuOp_call_near_IMM,
	/*  0xe9 */ x86emuOp_jump_near_IMM,
	/*  0xea */ x86emuOp_jump_far_IMM,
	/*  0xeb */ x86emuOp_jump_byte_IMM,
	/*  0xec */ x86emuOp_in_byte_AL_DX,
	/*  0xed */ x86emuOp_in_word_AX_DX,
	/*  0xee */ x86emuOp_out_byte_DX_AL,
	/*  0xef */ x86emuOp_out_word_DX_AX,

	/*  0xf0 */ x86emuOp_lock,
	/*  0xf1 */ x86emuOp_illegal_op,
	/*  0xf2 */ x86emuOp_repne,
	/*  0xf3 */ x86emuOp_repe,
	/*  0xf4 */ x86emuOp_halt,
	/*  0xf5 */ x86emuOp_cmc,
	/*  0xf6 */ x86emuOp_opcF6_byte_RM,
	/*  0xf7 */ x86emuOp_opcF7_word_RM,

	/*  0xf8 */ x86emuOp_clc,
	/*  0xf9 */ x86emuOp_stc,
	/*  0xfa */ x86emuOp_cli,
	/*  0xfb */ x86emuOp_sti,
	/*  0xfc */ x86emuOp_cld,
	/*  0xfd */ x86emuOp_std,
	/*  0xfe */ x86emuOp_opcFE_byte_RM,
	/*  0xff */ x86emuOp_opcFF_word_RM,
};
