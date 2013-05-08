/*
 * xhdi_sd.h
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
 *  Created on: 01.05.2013
 *  Copyright 2012 M. Fröschle
 */
#ifndef _XHDI_SD_H_
#define _XHDI_SD_H_

/* XHDI function numbers */

#define XHDI_VERSION			0
#define XHDI_INQUIRE_TARGET		1
#define XHDI_RESERVE			2
#define XHDI_LOCK				3
#define XHDI_STOP				4
#define XHDI_EJECT				5
#define XHDI_DRIVEMAP			6
#define XHDI_INQUIRE_DEVICE		7
#define XHDI_INQUIRE_DRIVER		8
#define XHDI_NEW_COOKIE			9
#define XHDI_READ_WRITE			10
#define XHDI_INQUIRE_TARGET2	11
#define XHDI_INQUIRE_DEVICE2	12
#define XHDI_DRIVER_SPECIAL		13
#define XHDI_GET_CAPACITY		14
#define XHDI_MEDIUM_CHANGED		15
#define XHDI_MINT_INFO			16
#define XHDI_DOS_LIMITS			17
#define XHDI_LAST_ACCESS		18
#define XHDI_REACCESS			19

/* XHDI error codes */

#define	E_OK		0		/* OK */
#define ERROR		-1		/* unspecified error */
#define EDRVNR		-2		/* drive not ready */
#define EUNDEV		-15		/* invalid device/target number */
#define EINVFN		-32		/* invalid function number */
#define EACCDN		-36		/* access denied (device currently reserved) */
#define EDRIVE		-46		/* BIOS device not served by driver */

/* XHDI device capabilities */

#define XH_TARGET_STOPPABLE	(1 << 0)
#define XH_TARGET_REMOVABLE (1 << 1)
#define XH_TARGET_LOCKABLE 	(1 << 2)
#define XH_TARGET_EJECTABLE	(1 << 3)
#define XH_TARGET_LOCKED	(1 << 29)
#define XH_TARGET_STOPPED	(1 << 30)
#define XH_TARGET_RESERVED	(1 << 31)

typedef struct _BPB
{
	  uint16_t   recsiz;       /* Bytes per sector                     */
	  uint16_t   clsiz;        /* Sectors per cluster                  */
	  uint16_t   clsizb;       /* Bytes per cluster                    */
	  uint16_t   rdlen;        /* Directory length                     */
	  uint16_t   fsiz;         /* Length of the FAT                    */
	  uint16_t   fatrec;       /* Start of the 2nd FAT                 */
	  uint16_t   datrec;       /* 1st free sector                      */
	  uint16_t   numcl;        /* Total numbr of clusters              */
	  uint16_t   bflags;       /* Flags as bit-vector                  */
	                       /* Bit 0: 0 (12-Bit-FAT), 1 16-Bit-FAT  */
	                       /* Bit 1: 0 (two FATs), 1 (one FAT)     */
	                       /*        only available since TOS 2.06 */
} BPB;

/* a riddle: how do you typedef a function pointer to a function that returns its own type? ;) */
typedef void* (*xhdi_call_fun)(int xhdi_fun, ...);

struct XHDICALL_args
{
	unsigned int opcode;
};

extern unsigned long xhdi_call(struct XHDICALL_args stack);

extern xhdi_call_fun xhdi_sd_install(xhdi_call_fun old_vector) __attribute__((__interrupt__));

extern uint16_t xhdi_version(void);	/* XHDI 0 */

extern uint32_t xhdi_inquire_target(uint16_t major, uint16_t minor, uint32_t *block_size, uint32_t *flags,
		char *product_name);		/* XHDI 1 */

extern uint32_t xhdi_reserve(uint16_t major, uint16_t minor, uint16_t do_reserve, uint16_t key);	/* XHDI 2 */

extern uint32_t xhdi_lock(uint16_t major, uint16_t minor, uint16_t do_lock, uint16_t key);	/* XHDI 3 */

extern uint32_t xhdi_stop(uint16_t major, uint16_t minor, uint16_t do_stop, uint16_t key);	/* XHDI 4 */

extern uint32_t xhdi_eject(uint16_t major, uint16_t minor, uint16_t do_eject, uint16_t key);	/* XHDI 5 */

extern uint32_t xhdi_drivemap(void);	/* XHDI 6 */

extern uint32_t xhdi_inquire_device(uint16_t bios_device, uint16_t *major, uint16_t *minor,
        uint32_t *start_sector, /* BPB */ void *bpb);	/* XHDI 7 */

extern uint32_t xhdi_inquire_driver(uint16_t bios_device, char *name, char *version,
		char *company, uint16_t *ahdi_version, uint16_t *maxIPL);	/* XHDI 8 */

extern uint32_t xhdi_new_cookie(uint32_t newcookie);	/* XHDI 9 */

extern uint32_t xhdi_read_write(uint16_t major, uint16_t minor, uint16_t rwflag,
        uint32_t recno, uint16_t count, void *buf);	/* XHDI 10 */

extern uint32_t xhdi_inquire_target2(uint16_t major, uint16_t minor, uint32_t *block_size,
        uint32_t *device_flags, char *product_name, uint16_t stringlen);	/* XHDI 11 */

extern uint32_t xhdi_inquire_device2(uint16_t bios_device, uint16_t *major, uint16_t *minor,
        uint32_t *start_sector, BPB *bpb, uint32_t *blocks, char *partid); /* XHDI 12 */

extern uint32_t xhdi_driver_special(uint32_t key1, uint32_t key2, uint16_t subopcode, void *data); /* XHDI 13 */

extern uint32_t xhdi_get_capacity(uint16_t major, uint16_t minor, uint32_t *blocks, uint32_t *bs); /* XHDI 14 */

extern uint32_t xhdi_medium_changed(uint16_t major, uint16_t minor);	/* XHDI 15 */

extern uint32_t xhdi_mint_info(uint16_t opcode, void *data);			/* XHDI 16 */

extern uint32_t xhdi_dos_limits(uint16_t which, uint32_t limit);		/* XHDI 17 */

extern uint32_t xhdi_last_access(uint16_t major, uint16_t minor, uint32_t *ms);	/* XHDI 18 */

extern uint32_t xhdi_reaccess(uint16_t major, uint16_t minor);	/* XHDI 19 */

#endif /* _XHDI_SD_H_ */
