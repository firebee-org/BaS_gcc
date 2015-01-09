/*
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
 *
 * provide an early exception vector branch table to catch exceptions _before_ VBR has been setup eventually
 * (to RAMBAR0, in exceptions.S)
 */

#include "MCF5475.h"
#include "bas_types.h"
#include "bas_printf.h"

typedef void (*exception_handler)(void);
extern exception_handler SDRAM_VECTOR_TABLE[];

/*
 * decipher Coldfire exception stack frame and print it out in cleartext
 */
void fault_handler(uint32_t pc, uint32_t format_status)
{
    int format;
    int fault_status;
    int vector;
    int sr;

    xprintf("\007\007exception! Processor halted.\r\n");
    xprintf("format_status: %lx\r\n", format_status);
    xprintf("pc: %lx\r\n", pc);

    /*
     * extract info from format-/status word
     */
    format =        (format_status & 0b11110000000000000000000000000000) >> 28;
    fault_status = ((format_status & 0b00001100000000000000000000000000) >> 26) |
                   ((format_status & 0b00000000000000110000000000000000) >> 16);
    vector       =  (format_status & 0b00000011111111000000000000000000) >> 18;
    sr           =  (format_status & 0b00000000000000001111111111111111);

    xprintf("format: %x\r\n", format);
    xprintf("fault_status: %x (", fault_status);
    switch (fault_status)
    {
    case 0:
        xprintf("not an access or address error nor an interrupted debug service routine");
        break;
    case 1:
    case 3:
    case 11:
        xprintf("reserved");
        break;
    case 2:
        xprintf("interrupt during a debug service routine for faults other than access errors");
        break;
    case 4:
        xprintf("error (for example, protection fault) on instruction fetch");
        break;
    case 5:
        xprintf("TLB miss on opword or instruction fetch");
        break;
    case 6:
        xprintf("TLB miss on extension word of instruction fetch");
        break;
    case 7:
        xprintf("IFP access error while executing in emulator mode");
        break;
    case 8:
        xprintf("error on data write");
        break;
    case 9:
        xprintf("error on attempted write to write-protected space");
        break;
    case 10:
        xprintf("TLB miss on data write");
        break;
    case 12:
        xprintf("error on data read");
        break;
    case 13:
        xprintf("attempted read, read-modify-write of protected space");
        break;
    case 14:
        xprintf("TLB miss on data read or read-modify-write");
        break;
    case 15:
        xprintf("OEP access error while executing in emulator mode");
    }
    xprintf(")\r\n");

    xprintf("vector = %02x (", vector);
    switch (vector)
    {
    case 2:
        xprintf("access error");
        break;
    case 3:
        xprintf("address error");
        break;
    case 4:
        xprintf("illegal instruction");
        break;
    case 5:
        xprintf("divide by zero");
        break;
    case 8:
        xprintf("privilege violation");
        break;
    case 9:
        xprintf("trace");
        break;
    case 10:
        xprintf("unimplemented line-a opcode");
        break;
    case 11:
        xprintf("unimplemented line-f opcode");
        break;
    case 12:
        xprintf("non-PC breakpoint debug interrupt");
        break;
    case 13:
        xprintf("PC breakpoint debug interrupt");
        break;
    case 14:
        xprintf("format error");
        break;
    case 24:
        xprintf("spurious interrupt");
        break;
    default:
        if ( ((vector >= 6) && (vector <= 7)) ||
             ((vector >= 16) && (vector <= 23)))
        {
            xprintf("reserved");
        }
        else if ((vector >= 25) && (vector <= 31))
        {
            xprintf("level %d autovectored interrupt", fault_status - 24);
        }
        else if ((vector >= 32) && (vector <= 47))
        {
            xprintf("trap #%d", vector - 32);
        }
        else
        {
            xprintf("unknown vector\r\n");
        }
    }
    xprintf(")\r\n");
    xprintf("sr=%4x\r\n", sr);
}

void __attribute__((interrupt)) handler(void)
{
    /*
     * Prepare exception stack contents so it can be handled by a C routine.
     *
     * For standard routines, we'd have to save registers here.
     * Since we do not intend to return anyway, we just ignore that requirement.
     */
    __asm__ __volatile__("move.l    (sp),-(sp)\n\t"\
                         "move.l    8(sp),-(sp)\n\t"\
                         "bsr       _fault_handler\n\t"\
                         "halt\n\t"\
                            : : : "memory");
}

void setup_vectors(void)
{
    int i;

    xprintf("\r\ninstall early exception vector table:");

    for (i = 8; i < 256; i++)
    {
        SDRAM_VECTOR_TABLE[i] = &handler;
    }

    /*
     * make sure VBR points to our table
     */
    __asm__ __volatile__("clr.l     d0\n\t"\
                         "movec.l   d0,VBR\n\t"\
                         "nop\n\t"\
                         "move.l    d0,_rt_vbr"
                         :                      /* outputs */
                         :                      /* inputs */
                         : "d0", "memory", "cc" /* clobbered registers */
                        );

    xprintf("finished.\r\n");
}
