/*
 * screen.h - low-level screen routines
 *
 * Copyright (c) 2013 The EmuTOS development team
 *
 * Authors:
 *  PES   Petr Stehlik
 *  RFB   Roger Burrows
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */

#ifndef VIDEL_H
#define VIDEL_H

#define SPSHIFT             0xffff8266L

#define FRGB_BLACK     0x00000000       /* Falcon palette */
#define FRGB_BLUE      0x000000ff
#define FRGB_GREEN     0x00ff0000
#define FRGB_CYAN      0x00ff00ff
#define FRGB_RED       0xff000000
#define FRGB_MAGENTA   0xff0000ff
#define FRGB_LTGRAY    0xbbbb00bb
#define FRGB_GRAY      0x88880088
#define FRGB_LTBLUE    0x000000aa
#define FRGB_LTGREEN   0x00aa0000
#define FRGB_LTCYAN    0x00aa00aa
#define FRGB_LTRED     0xaa000000
#define FRGB_LTMAGENTA 0xaa0000aa
#define FRGB_YELLOW    0xffff0000
#define FRGB_LTYELLOW  0xaaaa0000
#define FRGB_WHITE     0xffff00ff

/* bit settings for Falcon videomodes */
#define VIDEL_VALID    0x01ff           /* the only bits allowed in a videomode */
#define VIDEL_VERTICAL 0x0100           /* if set, use interlace (TV), double line (VGA) */
#define VIDEL_COMPAT   0x0080           /* ST-compatible if set */
#define VIDEL_OVERSCAN 0x0040           /* overscan if set (not used with VGA) */
#define VIDEL_PAL      0x0020           /* PAL if set; otherwise NTSC */
#define VIDEL_VGA      0x0010           /* VGA if set; otherwise TV */
#define VIDEL_80COL    0x0008           /* 80-column mode if set; otherwise 40 */
#define VIDEL_BPPMASK  0x0007           /* mask for bits/pixel encoding */
#define VIDEL_1BPP          0               /* 2 colours */
#define VIDEL_2BPP          1               /* 4 colours */
#define VIDEL_4BPP          2               /* 16 colours */
#define VIDEL_8BPP          3               /* 256 colours */
#define VIDEL_TRUECOLOR     4               /* 65536 colours */

/* test for VDI support of videomode */
#define VALID_VDI_BPP(mode) ((mode&VIDEL_BPPMASK)<=VIDEL_8BPP)

/* selected Falcon videomodes */
#define FALCON_ST_HIGH      (VIDEL_COMPAT|VIDEL_VGA|VIDEL_80COL|VIDEL_1BPP)

#define FALCON_DEFAULT_BOOT (VIDEL_VERTICAL|VIDEL_80COL|VIDEL_8BPP) /* 640x480x256 colours, TV, NTSC */

#define FALCON_REZ     3    /* used as a Falcon indicator */

typedef struct {
    int16_t vmode;         /* video mode (-1 => end marker) */
    int16_t monitor;       /* applicable monitors */
    uint16_t hht;          /* H hold timer */
    uint16_t hbb;          /* H border begin */
    uint16_t hbe;          /* H border end */
    uint16_t hdb;          /* H display begin */
    uint16_t hde;          /* H display end */
    uint16_t hss;          /* H SS */
    uint16_t vft;          /* V freq timer */
    uint16_t vbb;          /* V border begin */
    uint16_t vbe;          /* V border end */
    uint16_t vdb;          /* V display begin */
    uint16_t vde;          /* V display end */
    uint16_t vss;          /* V SS */
} VMODE_ENTRY;

void initialise_falcon_palette(int16_t mode);
const VMODE_ENTRY *lookup_videl_mode(int16_t mode,int16_t monitor);

/* Public XBIOS functions */
int16_t vsetmode(int16_t mode);
int16_t vmontype(void);
int16_t vsetsync(int16_t external);
int32_t vgetsize(int16_t mode);
int16_t vsetrgb(int16_t index,int16_t count,int32_t *rgb);
int16_t vgetrgb(int16_t index,int16_t count,int32_t *rgb);

/* misc routines */
int16_t get_videl_mode(void);
int16_t vfixmode(int16_t mode);
int16_t videl_check_moderez(int16_t moderez);
uint32_t videl_vram_size(void);
void videl_get_current_mode_info(uint16_t *planes, uint16_t *hz_rez, uint16_t *vt_rez);

extern int16_t current_video_mode;

#endif /* VIDEL_H */
