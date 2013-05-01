/*
 * xhdi_sd.h
 *
 *  Created on: 01.05.2013
 *      Author: mfro
 */
#ifndef _XHDI_SD_H_
#define _XHDI_SD_H_

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
extern uint32_t xhdi_inquire_target(uint32_t major, uint32_t minor, uint32_t block_size, uint32_t flags,
		char *product_name);		/* XHDI 1 */
extern uint32_t xhdi_reserve(UINT16_T major, UINT16_T minor, UINT16_T do_reserve, UINT16_T key);	/* XHDI 2 */
extern uint32_t xhdi_lock(UINT16_T major, UINT16_T minor, UINT16_T do_lock, UINT16_T key);	/* XHDI 3 */
extern uint32_t xhdi_stop(UINT16_T major, UINT16_T minor, UINT16_T do_lock, UINT16_T key);	/* XHDI 4 */
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

#endif /* _XHDI_SD_H_ */
