#ifndef __CONOUT_H__
#define __CONOUT_H__

#include "bas_types.h"

/*
 * conout.h - lowlevel color model dependent screen handling routines
 *
 *
 * Copyright (C) 2004-2016 by Authors:
 *
 * Authors:
 *  MAD     Martin Doering
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */



/* Defines for cursor */
#define  M_CFLASH       0x0001  /* cursor flash         0:disabled 1:enabled */
#define  M_CSTATE       0x0002  /* cursor flash state   0:off 1:on */
#define  M_CVIS         0x0004  /* cursor visibility    0:invisible 1:visible */

/*
 * The visibility flag is also used as a semaphore to prevent
 * the interrupt-driven cursor blink logic from colliding with
 * escape function/sequence cursor drawing activity.
 */

#define  M_CEOL         0x0008  /* end of line handling 0:overwrite 1:wrap */
#define  M_REVID        0x0010  /* reverse video        0:on        1:off */
#define  M_SVPOS        0x0020  /* position saved flag. 0:false,    1:true */
#define  M_CRIT         0x0040  /* reverse video        0:on        1:off */

/* Color related linea variables */

extern int16_t v_col_bg;           /* current background color */
extern int16_t v_col_fg;           /* current foreground color */

/* Cursor related linea variables */

extern uint8_t *v_cur_ad;         /* current cursor address */
extern int16_t v_cur_of;           /* cursor offset */
extern int8_t v_cur_tim;          /* cursor blink timer */

extern int8_t v_period;
extern int16_t disab_cnt;          /* disable depth count. (>0 means disabled) */
extern int8_t v_stat_0;           /* video cell system status */
extern int16_t sav_cur_x;          /* saved cursor cell x */
extern int16_t sav_cur_y;          /* saved cursor cell y */

/* Prototypes */

extern void ascii_out(int);
extern void move_cursor(int, int);
extern void blank_out (int, int, int, int);
extern void invert_cell(int, int);
extern void scroll_up(int);
extern void scroll_down(int);

#endif /* __CONOUT_H__ */
