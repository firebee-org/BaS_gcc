/*
 * conout.c - lowlevel color model dependent screen handling routines
 *
 *
 * Copyright (C) 2004 by Authors (see below)
 * Copyright (C) 2016 The EmuTOS development team
 *
 * Authors:
 *  MAD     Martin Doering
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.  See doc/license.txt for details.
 */

#include "conout.h"
#include "video.h"
#include "font.h"
#include "fb.h"

#define  plane_offset   2       /* interleaved planes */

#define v_fnt_st fnt->first_ade
#define v_fnt_nd fnt->last_ade
#define v_off_ad fnt->off_table
#define v_bas_ad info_fb->screen_base

uint8_t *v_cur_ad;              /* cursor address */
int8_t v_stat_0;                /* console status byte */
uint16_t v_cur_cx;
uint16_t v_cur_cy;
int8_t v_cur_tim;
uint16_t v_cel_mx;
uint16_t v_cel_my;
uint16_t v_cel_wr;
int16_t v_cur_of;
uint16_t v_cel_ht;
int8_t v_period;
const uint16_t *v_fnt_ad;
int16_t v_fbt_wr;
int16_t v_col_bg;
int16_t v_col_fg;
uint16_t v_fnt_wr;


const int16_t v_planes = 8;
extern struct fb_info *info_fb;
#define v_lin_wr (info_fb->var.width / 2) /* length of a screen line in words */

/*
 * internal prototypes
 */
static void neg_cell(uint8_t *);
static uint8_t * cell_addr(int, int);
static void cell_xfer(uint8_t *, uint8_t *);
static bool next_cell(void);


/*
 * char_addr - retrieve the address of the source cell
 *
 *
 * Given an offset value.
 *
 * in:
 *   ch - source cell code
 *
 * out:
 *   pointer to first byte of source cell if code was valid
 */

static uint8_t *char_addr(int16_t ch)
{
    uint16_t offs;

    /* test against limits */
    if (ch >= v_fnt_st)
    {
        if (ch <= v_fnt_nd)
        {
            /* getch offset from offset table */
            offs = v_off_ad[ch];
            offs >>= 3;                 /* convert from pixels to bytes. */

            /* return valid address */
            return (uint8_t*) v_fnt_ad + offs;
        }
    }

    /* invalid code. no address returned */
    return NULL;
}



/*
 * ascii_out - prints an ascii character on the screen
 *
 * in:
 *
 * ch.w      ascii code for character
 */

void ascii_out(int ch)
{
    uint8_t * src, * dst;
    bool visible;                       /* was the cursor visible? */

    src = char_addr(ch);                /* a0 -> get character source */
    if (src == NULL)
        return;                         /* no valid character */

    dst = v_cur_ad;                     /* a1 -> get destination */

    visible = v_stat_0 & M_CVIS;        /* test visibility bit */
    if ( visible ) {
        neg_cell(v_cur_ad);             /* delete cursor. */
        v_stat_0 &= ~M_CVIS;                    /* start of critical section */
    }

    /* put the cell out (this covers the cursor) */
    cell_xfer(src, dst);

    /* advance the cursor and update cursor address and coordinates */
    if (next_cell())
    {
        uint8_t * cell;

        int y = v_cur_cy;

        /* perform cell carriage return. */
        cell = v_bas_ad + (uint32_t) v_cel_wr * y;
        v_cur_cx = 0;                   /* set X to first cell in line */

        /* perform cell line feed. */
        if ( y < v_cel_my )
        {
            cell += v_cel_wr;           /* move down one cell */
            v_cur_cy = y + 1;           /* update cursor's y coordinate */
        }
        else {
            scroll_up(0);               /* scroll from top of screen */
        }
        v_cur_ad = cell;                /* update cursor address */
    }

    /* if visible */
    if ( visible ) {
        neg_cell(v_cur_ad);             /* display cursor. */
        v_stat_0 |= M_CSTATE;           /* set state flag (cursor on). */
        v_stat_0 |= M_CVIS;             /* end of critical section. */

        /* do not flash the cursor when it moves */
        if (v_stat_0 & M_CFLASH) {
            v_cur_tim = v_period;       /* reset the timer. */
        }
    }
}




/*
 * blank_out - Fills region with the background color.
 *
 * Fills a cell-word aligned region with the background color.
 *
 * The rectangular region is specified by a top/left cell x,y and a
 * bottom/right cell x,y, inclusive.  Routine assumes top/left x is
 * even and bottom/right x is odd for cell-word alignment. This is,
 * because this routine is heavily optimized for speed, by always
 * blanking as much space as possible in one go.
 *
 * in:
 *   topx - top/left cell x position (must be even)
 *   topy - top/left cell y position
 *   botx - bottom/right cell x position (must be odd)
 *   boty - bottom/right cell y position
 */

void blank_out(int topx, int topy, int botx, int boty)
{

}



/*
 * cell_addr - convert cell X,Y to a screen address.
 *
 *
 * convert cell X,Y to a screen address. also clip cartesian coordinates
 * to the limits of the current screen.
 *
 * latest update:
 *
 * 18-sep-84
 * in:
 *
 * d0.w      cell X
 * d1.w      cell Y
 *
 * out:
 * a1      points to first byte of cell
 */

static uint8_t *cell_addr(int x, int y)
{
    int32_t disx, disy;

    /* check bounds against screen limits */
    if ( x >= v_cel_mx )
        x = v_cel_mx;           /* clipped x */

    if ( y >= v_cel_my )
        y = v_cel_my;           /* clipped y */

    /* X displacement = even(X) * v_planes + Xmod2 */
    disx = (int32_t)v_planes * (x & ~1);
    if ( x & 1 ) {              /* Xmod2 = 0 ? */
        disx++;                 /* Xmod2 = 1 */
    }

    /* Y displacement = Y // cell conversion factor */
    disy = (int32_t)v_cel_wr * y;

    /*
     * cell address = screen base address + Y displacement
     * + X displacement + offset from screen-begin (fix)
     */
    return v_bas_ad + disy + disx + v_cur_of;
}



/*
 * cell_xfer - Performs a byte aligned block transfer.
 *
 *
 * This routine performs a byte aligned block transfer for the purpose of
 * manipulating monospaced byte-wide text. the routine maps a single-plane,
 * arbitrarily-long byte-wide image to a multi-plane bit map.
 * all transfers are byte aligned.
 *
 * in:
 * a0.l      points to contiguous source block (1 byte wide)
 * a1.l      points to destination (1st plane, top of block)
 *
 * out:
 * a4      points to byte below this cell's bottom
 */

static void cell_xfer(uint8_t * src, uint8_t * dst)
{
    uint8_t * src_sav, * dst_sav;
    uint16_t fg;
    uint16_t bg;
    int fnt_wr, line_wr;
    int plane;

    fnt_wr = v_fnt_wr;
    line_wr = v_lin_wr;

    /* check for reversed foreground and background colors */
    if ( v_stat_0 & M_REVID)
    {
        fg = v_col_bg;
        bg = v_col_fg;
    }
    else
    {
        fg = v_col_fg;
        bg = v_col_bg;
    }

    src_sav = src;
    dst_sav = dst;

    for (plane = v_planes; plane--;)
    {
        int i;

        src = src_sav;                  /* reload src */
        dst = dst_sav;                  /* reload dst */

        if (bg & 0x0001)
        {
            if (fg & 0x0001)
            {
                /* back:1  fore:1  =>  all ones */
                for (i = v_cel_ht; i--;)
                {
                    *dst = 0xff;                /* inject a block */
                    dst += line_wr;
                }
            }
            else
            {
                /* back:1  fore:0  =>  invert block */
                for (i = v_cel_ht; i--; )
                {
                    /* inject the inverted source block */
                    *dst = ~*src;
                    dst += line_wr;
                    src += fnt_wr;
                }
            }
        }
        else {
            if (fg & 0x0001)
            {
                /* back:0  fore:1  =>  direct substitution */
                for (i = v_cel_ht; i--;)
                {
                    *dst = *src;
                    dst += line_wr;
                    src += fnt_wr;
                }
            }
            else
            {
                /* back:0  fore:0  =>  all zeros */
                for (i = v_cel_ht; i--; )
                {
                    *dst = 0x00;                /* inject a block */
                    dst += line_wr;
                }
            }
        }

        bg >>= 1;                       /* next background color bit */
        fg >>= 1;                       /* next foreground color bit */
        dst_sav += plane_offset;        /* top of block in next plane */
    }
}



/*
 * move_cursor - move the cursor.
 *
 * move the cursor and update global parameters
 * erase the old cursor (if necessary) and draw new cursor (if necessary)
 *
 * in:
 * d0.w    new cell X coordinate
 * d1.w    new cell Y coordinate
 */

void move_cursor(int x, int y)
{
    /* update cell position */

    /* clamp x,y to valid ranges */
    if (x < 0)
        x = 0;
    else if (x > v_cel_mx)
        x = v_cel_mx;

    if (y < 0)
        y = 0;
    else if (y > v_cel_my)
        y = v_cel_my;

    v_cur_cx = x;
    v_cur_cy = y;

    /* is cursor visible? */
    if ( !(v_stat_0 & M_CVIS) ) {
        /* not visible */
        v_cur_ad = cell_addr(x, y);             /* just set new coordinates */
        return;                                 /* and quit */
    }

    /* is cursor flashing? */
    if ( v_stat_0 & M_CFLASH ) {
        v_stat_0 &= ~M_CVIS;                    /* yes, make invisible...semaphore. */

        /* is cursor presently displayed ? */
        if ( !(v_stat_0 & M_CSTATE )) {
            /* not displayed */
            v_cur_ad = cell_addr(x, y);         /* just set new coordinates */

            /* show the cursor when it moves */
            neg_cell(v_cur_ad);                 /* complement cursor. */
            v_stat_0 |= M_CSTATE;
            v_cur_tim = v_period;               /* reset the timer. */

            v_stat_0 |= M_CVIS;                 /* end of critical section. */
            return;
        }
    }

    /* move the cursor after all special checks failed */
    neg_cell(v_cur_ad);                         /* erase present cursor */

    v_cur_ad = cell_addr(x, y);                 /* fetch x and y coords. */
    neg_cell(v_cur_ad);                         /* complement cursor. */

    /* do not flash the cursor when it moves */
    v_cur_tim = v_period;                       /* reset the timer. */

    v_stat_0 |= M_CVIS;                         /* end of critical section. */
}



/*
 * neg_cell - negates
 *
 * This routine negates the contents of an arbitrarily-tall byte-wide cell
 * composed of an arbitrary number of (Atari-style) bit-planes.
 * Cursor display can be accomplished via this procedure.  Since a second
 * negation restores the original cell condition, there is no need to save
 * the contents beneath the cursor block.
 *
 * in:
 * a1.l      points to destination (1st plane, top of block)
 *
 * out:
 */

static void neg_cell(uint8_t * cell)
{
    int plane, len;
    int cell_len = v_cel_ht;

    v_stat_0 |= M_CRIT;                 /* start of critical section. */

    for (plane = v_planes; plane--; ) {
        uint8_t * addr = cell;            /* top of current dest plane */

        /* reset cell length counter */
        for (len = cell_len; len--; ) {
            *addr = ~*addr;
            addr += v_lin_wr;
        }
        cell += plane_offset;           /* a1 -> top of block in next plane */
    }
    v_stat_0 &= ~M_CRIT;                /* end of critical section. */
}



/*
 * invert_cell - negates the cells bits
 *
 * This routine negates the contents of an arbitrarily-tall byte-wide cell
 * composed of an arbitrary number of (Atari-style) bit-planes.
 *
 * Wrapper for neg_cell().
 *
 * in:
 * x - cell X coordinate
 * y - cell Y coordinate
 */

void invert_cell(int x, int y)
{
    /* fetch x and y coords and invert cursor. */
    neg_cell(cell_addr(x, y));
}



/*
 * next_cell - Return the next cell address.
 *
 * sets next cell address given the current position and screen constraints
 *
 * returns:
 *     false - no wrap condition exists
 *     true  - CR LF required (position has not been updated)
 */

static bool next_cell(void)
{
    /* check bounds against screen limits */
    if (v_cur_cx == v_cel_mx)
    {
        /* increment cell ptr */
        if (!(v_stat_0 & M_CEOL))
        {
            /* overwrite in effect */
            return 0;                   /* no wrap condition exists */
                                        /* don't change cell parameters */
        }

        /* call carriage return routine */
        /* call line feed routine */
        return 1;                       /* indicate that CR LF is required */
    }

    v_cur_cx += 1;                      /* next cell to right */

    /* if X is even, move to next word in the plane */
    if (v_cur_cx & 1)
    {
        /* x is odd */
        v_cur_ad += 1;                  /* a1 -> new cell */
        return 0;                       /* indicate no wrap needed */
    }

    /* new cell (1st plane), added offset to next word in plane */
    v_cur_ad += (v_planes << 1) - 1;

    return 0;                           /* indicate no wrap needed */
}


/*
 * scroll_up - Scroll upwards
 *
 *
 * Scroll copies a source region as wide as the screen to an overlapping
 * destination region on a one cell-height offset basis.  Two entry points
 * are provided:  Partial-lower scroll-up, partial-lower scroll-down.
 * Partial-lower screen operations require the cell y # indicating the
 * top line where scrolling will take place.
 *
 * After the copy is performed, any non-overlapping area of the previous
 * source region is "erased" by calling blank_out which fills the area
 * with the background color.
 *
 * in:
 *   top_line - cell y of cell line to be used as top line in scroll
 */

void scroll_up(int top_line)
{
    uint32_t count;
    uint8_t * src, * dst;

    /* screen base addr + cell y nbr * cell wrap */
    dst = v_bas_ad + (uint32_t) top_line * v_cel_wr;

    /* form source address from cell wrap + base address */
    src = dst + v_cel_wr;

    /* form # of bytes to move */
    count = (uint32_t) v_cel_wr * (v_cel_my - top_line);

    /* move BYTEs of memory*/
    memmove(dst, src, count);

    /* exit thru blank out, bottom line cell address y to top/left cell */
    blank_out(0, v_cel_my , v_cel_mx, v_cel_my );
}



/*
 * scroll_down - Scroll (partially) downwards
 */

void scroll_down(int start_line)
{
    uint32_t count;
    uint8_t * src, * dst;

    /* screen base addr + offset of start line */
    src = v_bas_ad + (uint32_t) start_line * v_cel_wr;

    /* form destination from source + cell wrap */
    dst = src + v_cel_wr;

    /* form # of bytes to move */
    count = (uint32_t) v_cel_wr * (v_cel_my - start_line);

    /* move BYTEs of memory*/
    memmove(dst, src, count);

    /* exit thru blank out */
    blank_out(0, start_line , v_cel_mx, start_line);
}
