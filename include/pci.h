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

#include <stdint.h>

#define	 PCI_MEMORY_OFFSET	(0x80000000)
#define	 PCI_MEMORY_SIZE	(0x40000000)	/* 1 GByte PCI memory window */
#define	 PCI_IO_OFFSET		(0xD0000000)
#define	 PCI_IO_SIZE		(0x10000000)	/* 128 MByte PCI I/O window */

#define PCIIDR                0x00   /* PCI Configuration ID Register       */
#define PCICSR                0x04   /* PCI Command/Status Register         */
#define PCICR                 0x04   /* PCI Command Register                */
#define PCISR                 0x06   /* PCI Status Register                 */
#define PCIREV                0x08   /* PCI Revision ID Register            */
#define PCICCR                0x08   /* PCI Class Code Register             */
#define PCICLSR               0x0F   /* PCI Cache Line Size Register        */
#define PCILTR                0x0E   /* PCI Latency Timer Register          */
#define PCIHTR                0x0D   /* PCI Header Type Register            */
#define PCIBISTR              0x0C   /* PCI Build-In Self Test Register     */
#define PCIBAR0               0x10   /* PCI Base Address Register for Memory
                                        Accesses to Local, Runtime, and DMA */
#define PCIBAR1               0x14   /* PCI Base Address Register for I/O
                                        Accesses to Local, Runtime, and DMA */
#define PCIBAR2               0x18   /* PCI Base Address Register for Memory
                                        Accesses to Local Address Space 0   */
#define PCIBAR3               0x1C   /* PCI Base Address Register for Memory
                                        Accesses to Local Address Space 1   */ 
#define PCIBAR4               0x20   /* PCI Base Address Register, reserved */
#define PCIBAR5               0x24   /* PCI Base Address Register, reserved */
#define PCICIS                0x28   /* PCI Cardbus CIS Pointer, not support*/
#define PCISVID               0x2E   /* PCI Subsystem Vendor ID             */
#define PCISID                0x2C   /* PCI Subsystem ID                    */
#define PCIERBAR              0x30   /* PCI Expansion ROM Base Register     */
#define CAP_PTR               0x37   /* New Capability Pointer              */
#define PCIILR                0x3F   /* PCI Interrupt Line Register         */
#define PCIIPR                0x3E   /* PCI Interrupt Pin Register          */
#define PCIMGR                0x3D   /* PCI Min_Gnt Register                */
#define PCIMLR                0x3C   /* PCI Max_Lat Register                */

// FIXME: register numbers swapped from here on
#define PMCAPID               0x40   /* Power Management Capability ID      */
#define PMNEXT                0x41   /* Power Management Next Capability
                                        Pointer                             */
#define PMC                   0x42   /* Power Management Capabilities       */
#define PMCSR                 0x44   /* Power Management Control/Status     */
#define PMCSR_BSE             0x46   /* PMCSR Bridge Support Extensions     */
#define PMDATA                0x47   /* Power Management Data               */
#define HS_CNTL               0x48   /* Hot Swap Control                    */
#define HS_NEXT               0x49   /* Hot Swap Next Capability Pointer    */
#define HS_CSR                0x4A   /* Hot Swap Control/Status             */
#define PVPDCNTL              0x4C   /* PCI Vital Product Data Control      */
#define PVPD_NEXT             0x4D   /* PCI Vital Product Data Next
                                        Capability Pointer                  */
#define PVPDAD                0x4E   /* PCI Vital Product Data Address      */
#define PVPDATA               0x50   /* PCI VPD Data                        */

/* Header type 1 (PCI-to-PCI bridges) */
#define PCI_PRIMARY_BUS        0x18  /* Primary bus number */
#define PCI_SECONDARY_BUS      0x19  /* Secondary bus number */
#define PCI_SUBORDINATE_BUS    0x1A  /* Highest bus number behind the bridge */
#define PCI_SEC_LATENCY_TIMER  0x1B  /* Latency timer for secondary interface */
#define PCI_IO_BASE            0x1C  /* I/O range behind the bridge */
#define PCI_IO_LIMIT           0x1D
#define PCI_SEC_STATUS         0x1E  /* Secondary status register, only bit 14 used */
#define PCI_MEMORY_BASE        0x20  /* Memory range behind */
#define PCI_MEMORY_LIMIT       0x22
#define PCI_PREF_MEMORY_BASE   0x24  /* Prefetchable memory range behind */
#define PCI_PREF_MEMORY_LIMIT  0x26
#define PCI_PREF_BASE_UPPER32  0x28  /* Upper half of prefetchable memory range */
#define PCI_PREF_LIMIT_UPPER32 0x2C
#define PCI_IO_BASE_UPPER16    0x30  /* Upper half of I/O addresses */
#define PCI_IO_LIMIT_UPPER16   0x32
#define PCI_BRIDGE_CONTROL     0x3E  /* Bridge Control */

struct pci_rd          /* structure of resource descriptor    */
{
    unsigned short next;               /* length of the following structure   */
    unsigned short flags;              /* type of resource and misc. flags    */
    unsigned long start;               /* start-address of resource           */
    unsigned long length;              /* length of resource                  */
    unsigned long offset;              /* offset PCI to phys. CPU Address     */
    unsigned long dmaoffset;           /* offset for DMA-transfers            */
} __attribute__ ((packed));

typedef struct                       /* structure of address conversion     */
{
    unsigned long adr;                 /* calculated address (CPU<->PCI)      */
    unsigned long len;                 /* length of memory range              */
} PCI_CONV_ADR;

/******************************************************************************/
/*                          PCI-BIOS Error Codes                              */
/******************************************************************************/
#define PCI_SUCCESSFUL            0  /* everything's fine         */
#define PCI_FUNC_NOT_SUPPORTED   -2  /* function not supported    */
#define PCI_BAD_VENDOR_ID        -3  /* wrong Vendor ID           */
#define PCI_DEVICE_NOT_FOUND     -4  /* PCI-Device not found      */
#define PCI_BAD_REGISTER_NUMBER  -5  /* wrong register number     */
#define PCI_SET_FAILED           -6  /* reserved for later use    */
#define PCI_BUFFER_TOO_SMALL     -7  /* reserved for later use    */
#define PCI_GENERAL_ERROR        -8  /* general BIOS error code   */
#define PCI_BAD_HANDLE           -9  /* wrong/unknown PCI-handle  */

/******************************************************************************/
/*                      Flags used in Resource-Descriptor                     */
/******************************************************************************/
#define FLG_IO        0x4000         /* Ressource in IO range               */
#define FLG_ROM       0x2000         /* Expansion ROM */
#define FLG_LAST      0x8000         /* last ressource                      */
#define FLG_8BIT      0x0100         /* 8 bit accesses allowed              */
#define FLG_16BIT     0x0200         /* 16 bit accesses allowed             */
#define FLG_32BIT     0x0400         /* 32 bit accesses allowed             */
#define FLG_ENDMASK   0x000F         /* mask for byte ordering              */

/******************************************************************************/
/*                 Values used in FLG_ENDMASK for Byte Ordering               */
/******************************************************************************/
#define ORD_MOTOROLA       0         /* Motorola (big endian)               */
#define ORD_INTEL_AS       1         /* Intel (little endian), addr.swapped */
#define ORD_INTEL_LS       2         /* Intel (little endian), lane swapped */
#define ORD_UNKNOWN       15         /* unknown (BIOS-calls allowed only)   */

/******************************************************************************/
/*                   Status Info used in Device-Descriptor                    */
/******************************************************************************/
#define DEVICE_FREE        0         /* Device is not used                  */
#define DEVICE_USED        1         /* Device is used by another driver    */
#define DEVICE_CALLBACK    2         /* used, but driver can be cancelled   */
#define DEVICE_AVAILABLE   3         /* used, not available                 */
#define NO_DEVICE         -1         /* no device detected                  */

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

extern int16_t pci_find_device(uint16_t device_id, uint16_t vendor_id, int index);

extern uint32_t pci_read_config_longword(uint16_t handle, uint16_t offset);
extern uint16_t pci_read_config_word(uint16_t handle, uint16_t offset);
extern uint8_t pci_read_config_byte(uint16_t handle, uint16_t offset);

extern void pci_write_config_longword(uint16_t handle, uint16_t offset, uint32_t value);
extern void pci_write_config_word(uint16_t handle, uint16_t offset, uint16_t value);
extern void pci_write_config_byte(uint16_t handle, uint16_t offset, uint8_t value);

extern struct pci_rd *pci_get_resource(uint16_t handle);
extern int16_t pci_hook_interrupt(uint16_t handle, void *interrupt_handler, void *parameter);
extern int16_t pci_unhook_interrupt(uint16_t handle);



#define PCI_HANDLE(bus, slot, function)	(0 | ((bus & 0xff) << 8 | (slot & 0x1f) << 3 | (function & 7)))
#define PCI_BUS_FROM_HANDLE(h)				(((h) & 0xff00) >> 8)
#define PCI_DEVICE_FROM_HANDLE(h)			(((h) & 0xf8) >> 3)
#define PCI_FUNCTION_FROM_HANDLE(h)			(((h) & 0x7))

#endif /* _PCI_H_ */
