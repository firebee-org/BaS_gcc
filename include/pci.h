#ifndef _PCI_H_
#define _PCI_H_

/*
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
 *  Created on: 26.02.2013
 *      Author: Markus Fröschle
 */

#define	 PCI_MEMORY_OFFSET	(0x80000000)
#define	 PCI_MEMORY_SIZE	(0x40000000)	/* 1 GByte PCI memory window */
#define	 PCI_IO_OFFSET		(0xD0000000)
#define	 PCI_IO_SIZE		(0x10000000)	/* 128 MByte PCI I/O window */

/* PCI configuration space macros */

/* register 0x00 macros */
#define PCI_VENDOR_ID(i)	((i) & 0xffff)
#define PCI_DEVICE_ID(i)	(((i) & 0xffff0000) >> 16)

/* register 0x04 macros */
#define PCI_STATUS(i)		(((i) >> 16) & 0xffff)
#define PCI_COMMAND(i)		((i) & 0xffff)

/* register 0x08 macros */
#define PCI_CLASS_CODE(i)	(((i) & 0xff000000) >> 24)
#define PCI_SUBCLASS(i)		(((i) & 0xff0000) >> 16)
#define PCI_PROF_IF(i)		(((i) & 0xff00) >> 8)
#define PCI_REVISION_ID(i)	(((i) & 0xff))

/* register 0x0c macros */
#define PCI_BIST(i)			(((i) & 0xff000000) >> 24)
#define PCI_HEADER_TYPE(i)	(((i) & 0xff0000) >> 16)
#define PCI_LAT_TIMER(i)	(((i) & 0xff00) >> 8)
#define PCI_CACHELINE_SIZE(i)	(((i) & 0xff))

/* register 0x2c macros */
#define PCI_SUBSYS_ID(i)	(((i) & 0xffff0000) >> 16)
#define PCI_SUBSYS_VID(i)	(((i) & 0xffff))

/* register 0x34 macros */
#define PCI_CAPABILITIES(i)	((i) & 0xff)

/* register 0x3c macros */
#define PCI_MAX_LATENCY(i)	(((i) & 0xff000000) >> 24)
#define PCI_MIN_GRANT(i)	(((i) & 0xff0000) >> 16)
#define PCI_INTERRUPT_PIN(i)	(((i) & 0xff00) >> 8)
#define PCI_INTERRUPT_LINE(i)	(((i)) & 0xff)

#define IS_PCI_MEM_BAR(I)	((i) & 1) == 0
#define IS_PCI_IO_BAR(i)	((i) & 1) == 1
#define PCI_MEMBAR_TYPE(i)	(((i) & 0x6) >> 1)
#define PCI_IOBAR_ADR(i)	(((i) & 0xfffffffc))
#define PCI_MEMBAR_ADR(i)	(((i) & 0xfffffff0))
extern void init_eport(void);
extern void init_xlbus_arbiter(void);
extern void init_pci(void);

#endif /* _PCI_H_ */
