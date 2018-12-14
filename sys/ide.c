/*
 * ide.c
 *
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
 * derived from original assembler sources:
 * Copyright 2010 - 2018 F. Aschwanden
 * Copyright 2018        D. Gálvez
 */

#include "bas_printf.h"

#define        FALCON_IO            0xFFF00000
#define        IDE_1ST_INTERFACE    0x00
#define        IDE_2ND_INTERFACE    0x40

#define        IDE_REG_DATA         *(volatile long*)(FALCON_IO + ide_addr_offset + (0x00))
#define        IDE_REG_ERROR        *(volatile char*)(FALCON_IO + ide_addr_offset + (0x05))
#define        IDE_REG_SECCNT       *(volatile char*)(FALCON_IO + ide_addr_offset + (0x09))
#define        IDE_REG_STASEC       *(volatile char*)(FALCON_IO + ide_addr_offset + (0x0D))
#define        IDE_REG_DRVHEAD      *(volatile char*)(FALCON_IO + ide_addr_offset + (0x19))
#define        IDE_REG_CMD          *(volatile char*)(FALCON_IO + ide_addr_offset + (0x1D))
#define        IDE_REG_STATUS       *(volatile char*)(FALCON_IO + ide_addr_offset + (0x1D))

#define        IDE_REG_STATUS_DRQ    (1 << 3)
#define        IDE_REG_STATUS_BSY    (1 << 7)

#define        IDE_CMD_RESET        0x08
#define        IDE_CMD_IDENTIFY     0xEC

/* IDENTIFY INFO OFFSETS IN BYTES */
#define        IDENTIFY_MODEL                   27 * 2
#define        IDENTIFY_PIO_CYCLE_TIME_NS       68 * 2

#define        ACP_CONFIG_REG        *(volatile long*)(0xF0040000)

#define        ACP_CONFIG_REG_SPEED_U0      (3 << 16)    /* Slow (3), Middle (2), Fast (1), No drive (NEG) */
#define        ACP_CONFIG_REG_SPEED_U1      (3 << 20)    /* Slow (3), Middle (2), Fast (1), No drive (NEG) */
#define        ACP_CONFIG_REG_RST           (1 << 25)
#define        ACP_CONFIG_REG_SCSI_INT      (1 << 27)
#define        ACP_CONFIG_REG_IDE_INT       (1 << 28)
#define        ACP_CONFIG_REG_IDE_ADDR      (1 << 30)    /* 0: 0x40 1: 0x00 */
#define        ACP_CONFIG_REG_CF_ADDR       (1 << 31)    /* 0: 0x00 1: 0x40 */

extern void wait_ms(long);


int32_t get_speed(short pio_cycle_time)
{
    int32_t speed;

    /* From BaS CW sources:
     * 0  - 63  ns fast
     * 64 - 128 ns middle
     *    > 128 ns slow
     */
    if (pio_cycle_time <= 0)
            speed = -1; /* drive or card not present */
    else if (pio_cycle_time <= 63)
            speed = 0;
    else if (pio_cycle_time <= 96)
            speed = 1;
    else if (pio_cycle_time <= 128)
            speed = 2;
    else speed = 3;

    return speed;
}

int wait_ready(short ide_addr_offset)
{
    int i = 0;

    do {
        wait_ms(1);
        i++;
        if (i == 2000) /* 2 sec */
            return -1;
    } while (IDE_REG_STATUS & IDE_REG_STATUS_BSY);

    return 0;
}

short test_drive(short ide_addr_offset)
{
    uint32_t buff[128];    /* 512 bytes */
    int8_t *buff2 = (int8_t *)buff;
    int16_t pio_cycle_time;
    int i;

    /* IDE reset */

    IDE_REG_DRVHEAD = 0x00;
    IDE_REG_CMD = IDE_CMD_RESET;

    if (wait_ready(ide_addr_offset))
        return 0; /* Time-Out */

    /* Identify device */

    i = 0;
    do {
        IDE_REG_DRVHEAD = 0x00;
        IDE_REG_CMD = IDE_CMD_IDENTIFY;

        wait_ready(ide_addr_offset);
        if (++i > 3) /* 3 attempts */
            return 0;
    } while (!(IDE_REG_STATUS & IDE_REG_STATUS_DRQ));

    for (i = 0; i < 128; i++) {
        buff[i] = IDE_REG_DATA;
    }

    if (!buff[0])
        return 0; /* There is no data */

    /* Print drive info */

    for (i = 0; i < 20; i++) {
        xprintf("%c", *(buff2 + IDENTIFY_MODEL + i));
    }

    pio_cycle_time = (short) *(short *)(buff2 + IDENTIFY_PIO_CYCLE_TIME_NS);

    xprintf("PIO cycle time: %d ns\n\r", *(short *)(buff2 + IDENTIFY_PIO_CYCLE_TIME_NS));

    return pio_cycle_time;
};


void set_ide_access_mode(void)
{
    uint16_t pio_cycle_time;
    int32_t speed;

    ACP_CONFIG_REG |= ACP_CONFIG_REG_RST;
    wait_ms(10);

    /* CF 0xFFF00000, IDE 0xFFF00040, IDE INT ON, SCSI INT OFF, SPEED SLOW */
    ACP_CONFIG_REG &= (!ACP_CONFIG_REG_CF_ADDR & !ACP_CONFIG_REG_IDE_ADDR);
    wait_ms(10);
    ACP_CONFIG_REG |= (ACP_CONFIG_REG_IDE_INT | ACP_CONFIG_REG_SPEED_U1 | ACP_CONFIG_REG_SPEED_U0);
    wait_ms(10);

    pio_cycle_time = test_drive(IDE_1ST_INTERFACE);
    speed = get_speed(pio_cycle_time);
    ACP_CONFIG_REG = (ACP_CONFIG_REG & 0xfff0ffff) | (speed << 16);

    /* For now we only support the built-in CF interface */
#if 0
    pio_cycle_time = test_drive(IDE_2ND_INTERFACE);
    speed = get_speed(pio_cycle_time);
#endif
    speed = 0x0f; /* -1 (drive or card not present) */
    ACP_CONFIG_REG = (ACP_CONFIG_REG & 0xff0fffff) | (speed << 20);
};
