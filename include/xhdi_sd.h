/*
 * xhdi_sd.h
 *
 *  Created on: 01.05.2013
 *      Author: mfro
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

/*
 * FIXME: dangerous TRAP here!
 *
 * all of these functions get compiled with "normal" GCC integers (32 bit). However, since they will be called
 * from code compiled with -mshort, integers must be declared uint32_t for those compilation units to adhere
 * to "normal" GCC calling conventions.
 *
 * This is ugly and slow (all stack frames from -mshort compiled code need to be rearranged for "normal"
 * calling conventions), but that's the way it currently is...
 *
 */
#ifdef __MSHORT__
#define UINT16_T	uint32_t
#else
#define UINT16_T	uint16_t
#endif

extern uint32_t xhdi_call(int xhdi_fun, ...);

extern uint32_t xhdi_version(void);	/* XHDI 0 */
extern uint32_t xhdi_inquire_target(UINT16_T major, UINT16_T minor, uint32_t *block_size, uint32_t *flags,
		char *product_name);		/* XHDI 1 */
extern uint32_t xhdi_reserve(UINT16_T major, UINT16_T minor, UINT16_T do_reserve, UINT16_T key);	/* XHDI 2 */
extern uint32_t xhdi_lock(UINT16_T major, UINT16_T minor, UINT16_T do_lock, UINT16_T key);	/* XHDI 3 */
extern uint32_t xhdi_stop(UINT16_T major, UINT16_T minor, UINT16_T do_stop, UINT16_T key);	/* XHDI 4 */
extern uint32_t xhdi_eject(UINT16_T major, UINT16_T minor, UINT16_T do_eject, UINT16_T key);	/* XHDI 5 */
extern uint32_t xhdi_drivemap(void);	/* XHDI 6 */
extern uint32_t xhdi_inquire_device(UINT16_T bios_device, UINT16_T *major, UINT16_T *minor,
        uint32_t *start_sector, /* BPB */ void *bpb);	/* XHDI 7 */
extern uint32_t xhdi_inquire_driver(UINT16_T bios_device, char *name, char *version,
		char *company, UINT16_T *ahdi_version, UINT16_T *maxIPL);	/* XHDI 8 */
extern uint32_t xhdi_new_cookie(void *newcookie);	/* XHDI 9 */
extern uint32_t xhdi_read_write(UINT16_T major, UINT16_T minor, UINT16_T rwflag,
        uint32_t recno, UINT16_T count, void *buf);	/* XHDI 10 */
extern uint32_t xhdi_inquire_target2(UINT16_T major, UINT16_T minor, uint32_t *block_size,
        uint32_t *device_flags, char *product_name, UINT16_T stringlen);	/* XHDI 11 */
extern uint32_t xhdi_inquire_device2(UINT16_T bios_device, UINT16_T *major, UINT16_T *minor,
        UINT16_T *start_sector, /* BPB */ void *bpb, uint32_t *blocks, char *partid); /* XHDI 12 */
extern uint32_t xhdi_driver_special(uint32_t key1, uint32_t key2, UINT16_T subopcode, void *data); /* XHDI 13 */
extern uint32_t xhdi_get_capacity(UINT16_T major, UINT16_T minor, uint32_t *blocks, uint32_t *bs); /* XHDI 14 */
extern uint32_t xhdi_medium_changed(UINT16_T major, UINT16_T minor);	/* XHDI 15 */
extern uint32_t xhdi_mint_info(UINT16_T opcode, void *data);			/* XHDI 16 */
extern uint32_t xhdi_dos_limits(UINT16_T which, uint32_t limit);		/* XHDI 17 */
extern uint32_t xhdi_last_access(UINT16_T major, UINT16_T minor, uint32_t *ms);	/* XHDI 18 */
extern uint32_t xhdi_reaccess(UINT16_T major, UINT16_T minor);	/* XHDI 19 */
#endif /* _XHDI_SD_H_ */
