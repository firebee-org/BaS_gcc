/*
 *	offscreen.c
 *
 * based from Emutos / BDOS
 *
 * Copyright (c) 2001 Lineo, Inc.
 *
 * Authors: Karl T. Braun, Martin Doering, Laurent Vogel
 *
 * This file is distributed under the GPL, version 2 or at your
 * option any later version.
 */

#include "bas_types.h"
#include "bas_string.h"
#include "fb.h"

// #define DEBUG
#include "debug.h"

/* MD - Memory Descriptor */

#define MD struct _md_

MD
{
    MD *m_link;
    long m_start;
    long m_length;
    void *m_own;
};

/* MPB - Memory Partition Block */

#define MPB struct _mpb

MPB
{
    MD *mp_mfl;
    MD *mp_mal;
    MD *mp_rover;
};

#define MAXMD 256

static MD tab_md[MAXMD];
static MPB pmd;
static long wrap;

static void *xmgetblk(void)
{
    int i;
    for (i = 0; i < MAXMD; i++)
    {
        if(tab_md[i].m_own == NULL)
        {
            tab_md[i].m_own = (void*)1L;
            return(&tab_md[i]);
        }
    }
    return(NULL);
}

static void xmfreblk(void *m)
{
    int i = (int)(((long)m - (long)tab_md) / sizeof(MD));
    if((i > 0) && (i < MAXMD))
        tab_md[i].m_own = NULL;
}

static MD *ffit(long amount, MPB *mp)
{
    MD *p;
    MD *q;
    MD *p1;                             /* free list is composed of MD's */
    int maxflg;
    long maxval;
    if(amount != -1)
    {
#if 1
        amount += (wrap - 1);
        amount /= wrap;
        amount  *= wrap;                /* screen line alignment */
#else
        amount += 15;                   /* 16 bytes alignment */
        amount &= 0xFFFFFFF0;
#endif
    }
    if ((q = mp->mp_rover) == 0)        /* get rotating pointer */
        return(0) ;
    maxval = 0;
    maxflg = ((amount == -1) ? true : false) ;
    p = q->m_link;                   /* start with next MD */
    do /* search the list for an MD with enough space */
    {
        if (p == NULL)
        {
            /*  at end of list, wrap back to start  */
            q = (MD *) &mp->mp_mfl;     /*  q => mfl field  */
            p = q->m_link;              /*  p => 1st MD     */
        }
        if ((!maxflg) && (p->m_length >= amount))
        {
            /*  big enough */
            if (p->m_length == amount)
                q->m_link = p->m_link;  /* take the whole thing */
            else
            {
                /*
                 * break it up - 1st allocate a new
                 * MD to describe the remainder
                 */
                p1 = xmgetblk();
                if (p1 == NULL)
                    return NULL;

                /*
                 * init new MD
                 */
                p1->m_length = p->m_length - amount;
                p1->m_start = p->m_start + amount;
                p1->m_link = p->m_link;
                p->m_length = amount;   /* adjust allocated block */
                q->m_link = p1;
            }

            /*
             * link allocated block into allocated list,
             * mark owner of block, & adjust rover
             */
            p->m_link = mp->mp_mal;
            mp->mp_mal = p;
            mp->mp_rover = (q == (MD *) &mp->mp_mfl ? q->m_link : q);
            return p;           /* got some */
        }
        else if (p->m_length > maxval)
            maxval = p->m_length;
        p = (q = p)->m_link;
    }

    while (q != mp->mp_rover);

    /*
     * return either the max, or 0 (error)
     */
    if (maxflg)
    {
        maxval -= 15;   /* 16 bytes alignment */
        if (maxval < 0)
            maxval = 0;
        else
            maxval &= 0xFFFFFFF0;
    }
    return maxflg ? (MD *) maxval : 0;
}

static void freeit(MD *m, MPB *mp)
{
    MD *p;
    MD *q = NULL;

    for (p = mp->mp_mfl; p; p = (q = p) -> m_link)
    {
        if (m->m_start <= p->m_start)
            break;
    }
    m->m_link = p;
    if (q)
        q->m_link = m;
    else
        mp->mp_mfl = m;
    if (!mp->mp_rover)
        mp->mp_rover = m;
    if (p)
    {
        if (m->m_start + m->m_length == p->m_start)
        {
            /*
             * join to higher neighbor
             */
            m->m_length += p->m_length;
            m->m_link = p->m_link;
            if (p == mp->mp_rover)
                mp->mp_rover = m;
            xmfreblk(p);
        }
    }
    if (q)
    {
        if (q->m_start + q->m_length == m->m_start)
        {
            /* join to lower neighbor */
            q->m_length += m->m_length;
            q->m_link = m->m_link;
            if (m == mp->mp_rover)
                mp->mp_rover = q;
            xmfreblk(m);
        }
    }
}

long offscreen_free(struct fb_info *info, void *addr)
{
    MD *p;
    MD **q;
    MPB *mpb;

    dbg("%p\r\n", addr);

    //*vblsem = 0;
    mpb = &pmd;
    for (p = *(q = &mpb->mp_mal); p; p = *(q = &p->m_link))
    {
        if (addr == (void *) p->m_start)
            break;
    }
    if(!p)
    {
        // *vblsem = 1;
        return -1; //(EFAULT);
    }
    *q = p->m_link;
    freeit(p,mpb);
    //*vblsem = 1;
    return 0;
}

long offscreen_alloc(struct fb_info *info, long amount)
{
    long ret;
    MD *m;


    // *vblsem = 0;
    if (amount == -1L)
    {
        ret = (long) ffit(-1L, &pmd);
        // *vblsem = 1;
        return ret;
    }
    if (amount <= 0 )
    {
        // *vblsem = 1;
        return(0);
    }
    if ((amount & 1))
        amount++;
    m = ffit(amount, &pmd);
    if (m == NULL)
    {
        return(0);
    }
    ret = (long)m->m_start;
    // *vblsem = 1;
    return(ret);
}

long offscren_reserved(struct fb_info *info)
{
    return (long) info->ram_base + (long) info->ram_size;
}

void offscreen_init(struct fb_info *info)
{
    long size_screen;
    long max_offscreen_size;

    wrap = (long) info->var.xres_virtual * (long) (info->var.bits_per_pixel / 8);
    size_screen = (long) info->var.yres_virtual * wrap;
    if (!size_screen)
        size_screen = (long) info->screen_size;

    pmd.mp_mfl = pmd.mp_rover = &tab_md[0];
    tab_md[0].m_link = (MD *) NULL;
    tab_md[0].m_start = (long)((unsigned long) info->ram_base + (unsigned long) size_screen);
    tab_md[0].m_length = (long) info->ram_size - size_screen;
    tab_md[0].m_own = (void *)1L;

    max_offscreen_size = ((long) info->var.xres_virtual * 8192L * (long)(info->var.bits_per_pixel / 8)) - size_screen;
    if (max_offscreen_size < 0)
        max_offscreen_size = 0;
    if (tab_md[0].m_length > max_offscreen_size)
        tab_md[0].m_length = max_offscreen_size;

    dbg("offscreen_init start %p, length %ld, ram size %ld\r\n",
        tab_md[0].m_start, tab_md[0].m_length, (long) info->ram_size);
    pmd.mp_mal = (MD *) NULL;
}

