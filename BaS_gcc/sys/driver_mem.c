/*
 *	driver_mem.c
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

#include <stdint.h>
#include <stdbool.h>
#include "bas_string.h"
#include "bas_printf.h"
#include "usb.h"
#include "exceptions.h"		/* set_ipl() */

#if defined(MACHINE_FIREBEE)
#include "firebee.h"
#elif defined(MACHINE_M5484LITE)
#include "m5484l.h"
#elif defined(MACHINE_M54455)
#include "m54455.h"
#else
#error "unknown machine!"
#endif

#define DBG_DM
#ifdef	DBG_DM
#define	dbg(fmt, args...)	xprintf(fmt, ##args)
#else
#define dbg(fmt, args...)
#endif

extern long offscren_reserved(void);

extern uint8_t driver_mem_buffer[DRIVER_MEM_BUFFER_SIZE];	/* defined in linker control file */

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

static void *xmgetblk(void)
{
	int i;

	for (i = 0; i < MAXMD; i++)
	{
		if (tab_md[i].m_own == NULL)
		{
			tab_md[i].m_own = (void*)1L;
			return(&tab_md[i]);
		}
	}
	return NULL;
}

static void xmfreblk(void *m)
{
	int i = (int)(((long) m - (long) tab_md) / sizeof(MD));
	if ((i > 0) && (i < MAXMD))
	{
		tab_md[i].m_own = NULL;
	}
}

static MD *ffit(long amount, MPB *mp)
{
	MD *p, *q, *p1;                    /* free list is composed of MD's */
	int maxflg;
	long maxval;

	if (amount != -1)
	{
		amount += 15;                  /* 16 bytes alignment */
		amount &= 0xFFFFFFF0;
	}

	if ((q = mp->mp_rover) == 0)      /* get rotating pointer */
	{
		return 0;
	}

	maxval = 0;
	maxflg = ((amount == -1) ? true : false) ;
	p = q->m_link;                   /* start with next MD */
	do /* search the list for an MD with enough space */
	{
		if (p == 0)
		{
			/*  at end of list, wrap back to start  */
			q = (MD *) &mp->mp_mfl;      /*  q => mfl field  */
			p = q->m_link;               /*  p => 1st MD     */
		}
		if ((!maxflg) && (p->m_length >= amount))
		{
			/*  big enough */
			if (p->m_length == amount)
			{
				q->m_link = p->m_link;     /* take the whole thing */
			}
			else
			{
				/*
				 * break it up - 1st allocate a new
				 * MD to describe the remainder
				 */
				p1 = xmgetblk();
				if (p1 == NULL)
				{
					return(NULL);
				}

				/* init new MD */
				p1->m_length = p->m_length - amount;
				p1->m_start = p->m_start + amount;
				p1->m_link = p->m_link;
				p->m_length = amount;      /* adjust allocated block */
				q->m_link = p1;
			}
			/* link allocate block into allocated list,
				mark owner of block, & adjust rover  */
			p->m_link = mp->mp_mal;
			mp->mp_mal = p;
			mp->mp_rover = (q == (MD *) &mp->mp_mfl ? q->m_link : q);
			return(p);                   /* got some */
		}
		else if (p->m_length > maxval)
			maxval = p->m_length;
		p = ( q=p )->m_link;
	} while(q != mp->mp_rover);

	/*
	 * return either the max, or 0 (error)
	 */
	if (maxflg)
	{
		maxval -= 15;		/* 16 bytes alignment */
		if (maxval < 0)
		{
			maxval = 0;
		}
		else
		{
			maxval &= 0xFFFFFFF0;
		}
	}
	return(maxflg ? (MD *) maxval : 0);
}

static void freeit(MD *m, MPB *mp)
{
	MD *p, *q;

	q = 0;
	for (p = mp->mp_mfl; p ; p = (q = p) -> m_link)
	{
		if (m->m_start <= p->m_start)
		{
			break;
		}
	}
	m->m_link = p;

	if (q)
	{
		q->m_link = m;
	}
	else
	{
		mp->mp_mfl = m;
	}

	if (!mp->mp_rover)
	{
		mp->mp_rover = m;
	}

	if (p)
	{
		if (m->m_start + m->m_length == p->m_start)
		{
			/* join to higher neighbor */
			m->m_length += p->m_length;
			m->m_link = p->m_link;
			if (p == mp->mp_rover)
			{
				mp->mp_rover = m;
			}
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
			{
				mp->mp_rover = q;
			}
			xmfreblk(m);
		}
	}
}

int32_t driver_mem_free(void *addr)
{
	int level;
	MD *p, **q;
	MPB *mpb;
	mpb = &pmd;
	level = set_ipl(7);

	for(p = *(q = &mpb->mp_mal); p; p = *(q = &p->m_link))
	{
		if ((long) addr == p->m_start)
		{
			break;
		}
	}

	if (!p)
	{
		set_ipl(level);
		return(-1);
	}

	*q = p->m_link;
	freeit(p, mpb);
	set_ipl(level);

	dbg("%s: driver_mem_free(0x%08X)\r\n", __FUNCTION__, addr);

	return(0);
}

void *driver_mem_alloc(uint32_t amount)
{
	void *ret = NULL;
	int level;
	MD *m;

	if (amount == -1L)
	{
		return (void *) ffit(-1L, &pmd);
	}

	if (amount <= 0 )
	{
		return(0);
	}

	if ((amount & 1))
	{
		amount++;
	}

	level = set_ipl(7);
	m = ffit(amount, &pmd);

	if (m != NULL)
	{
		ret = (void *) m->m_start;
	}
	set_ipl(level);
	dbg("%s: driver_mem_alloc(%d) = 0x%08X\r\n", __FUNCTION__, amount, ret);

	return ret;
}

static int use_count = 0;

int driver_mem_init(void)
{
	if (use_count == 0)
	{
		dbg("%s: initialise driver_mem_buffer[] at %p, size 0x%x\r\n", __FUNCTION__, driver_mem_buffer, DRIVER_MEM_BUFFER_SIZE);
		memset(driver_mem_buffer, 0, DRIVER_MEM_BUFFER_SIZE);

		pmd.mp_mfl = pmd.mp_rover = &tab_md[0];
		tab_md[0].m_link = (MD *) NULL;
		tab_md[0].m_start = ((long) driver_mem_buffer + 15) & ~15;
		tab_md[0].m_length = DRIVER_MEM_BUFFER_SIZE;
		tab_md[0].m_own = (void *) 1L;
		pmd.mp_mal = (MD *) NULL;
		memset(driver_mem_buffer, 0, tab_md[0].m_length);

		dbg("%s: uncached driver memory buffer at 0x%08X size %d\r\n", __FUNCTION__, tab_md[0].m_start, tab_md[0].m_length);
	}
	use_count++;
	dbg("%s: driver_mem now has a use count of %d\r\n", __FUNCTION__, use_count);

	return 0;
}

void driver_mem_release(void)
{
	if (use_count-- == 0)
	{
#ifndef CONFIG_USB_MEM_NO_CACHE
#ifdef USE_RADEON_MEMORY
		if (driver_mem_buffer == (void *) offscren_reserved())
			return;

#endif
#endif
	}
	dbg("%s: driver_mem use count now %d\r\n", __FUNCTION__, use_count);
}



