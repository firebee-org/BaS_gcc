/*
 * font.h - font specific definitions
 *
 * Copyright (c) 2001 Lineo, Inc.
 * Copyright (c) 2004 by Authors:
 *
 * Authors:
 *  MAD     Martin Doering
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */

#ifndef FONT_H
#define FONT_H

#include <stdint.h>

/* font header flags */

#define F_DEFAULT 1     /* this is the default font (face and size) */
#define F_HORZ_OFF  2   /* there are left and right offset tables */
#define F_STDFORM  4    /* is the font in standard format */
#define F_MONOSPACE 8   /* is the font monospaced */

/* font style bits */

#define F_THICKEN 1
#define F_LIGHT 2
#define F_SKEW  4
#define F_UNDER 8
#define F_OUTLINE 16
#define F_SHADOW        32

/* font specific linea variables */

extern const uint16_t *v_fnt_ad;   /* address of current monospace font */
extern const uint16_t *v_off_ad;   /* address of font offset table */
extern uint16_t v_fnt_nd;          /* ascii code of last cell in font */
extern uint16_t v_fnt_st;          /* ascii code of first cell in font */
extern uint16_t v_fnt_wr;          /* font cell wrap */

/* character cell specific linea variables */

extern uint16_t    v_cel_ht;       /* cell height (width is 8) */
extern uint16_t    v_cel_mx;       /* needed by MiNT: columns on the screen minus 1 */
extern uint16_t    v_cel_my;       /* needed by MiNT: rows on the screen minus 1 */
extern uint16_t    v_cel_wr;       /* needed by MiNT: length (in int8_ts) of a line of characters */

/*
 * font_ring is a struct of four pointers, each of which points to
 * a list of font headers linked together to form a string.
 */

extern struct font_head *font_ring[4];  /* Ring of available fonts */
extern int16_t font_count;                 /* all three fonts and NULL */

/* the font header descibes a font */

struct font_head {
    int16_t font_id;
    int16_t point;
    int8_t name[32];
    uint16_t first_ade;
    uint16_t last_ade;
    uint16_t top;
    uint16_t ascent;
    uint16_t half;
    uint16_t descent;
    uint16_t bottom;
    uint16_t max_char_width;
    uint16_t max_cell_width;
    uint16_t left_offset;          /* amount character slants left when skewed */
    uint16_t right_offset;         /* amount character slants right */
    uint16_t thicken;              /* number of pixels to smear */
    uint16_t ul_size;              /* size of the underline */
    uint16_t lighten;              /* mask to and with to lighten  */
    uint16_t skew;                 /* mask for skewing */
    uint16_t flags;

    const uint8_t *hor_table;     /* horizontal offsets */
    const uint16_t *off_table;     /* character offsets  */
    const uint16_t *dat_table;     /* character definitions */
    uint16_t form_width;
    uint16_t form_height;

    struct font_head *next_font;/* pointer to next font */
    uint16_t font_seg;
};



/* prototypes */

void font_init(void);           /* initialize BIOS font ring */
void font_set_default(void);    /* choose the default font */

extern const struct font_head *fnt_st_8x16;

#endif /* FONT_H */
