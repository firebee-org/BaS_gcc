/*
 * driver_vec.h
 *
 * Interface for exposure of BaS drivers to the OS
 *
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
 *  Created on: 24.10.2013
 *      Author: Markus Fröschle
 */

#ifndef _DRIVER_VEC_H_
#define _DRIVER_VEC_H_

#include "xhdi_sd.h"
#include "MCD_dma.h"
#include "pci.h"

enum driver_type
{
    BLOCKDEV_DRIVER,
    CHARDEV_DRIVER,
    XHDI_DRIVER,
    MCD_DRIVER,
    VIDEO_DRIVER,
    PCI_DRIVER,
    MMU_DRIVER,
    PCI_NATIVE_DRIVER,
    END_OF_DRIVERS = 0xffffffffL,        /* marks end of driver list */
};

struct generic_driver_interface
{
    uint32_t (*init)(void);
    uint32_t (*read)(void *buf, size_t count);
    uint32_t (*write)(const void *buf, size_t count);
    uint32_t (*ioctl)(uint32_t request, ...);
};

struct dma_driver_interface
{
    int32_t version;
    int32_t magic;
    int (*dma_set_initiator)(int initiator);
    uint32_t (*dma_get_initiator)(int requestor);
    void (*dma_free_initiator)(int requestor);
    int (*dma_set_channel)(int requestor, void (*handler)(void));
    int (*dma_get_channel)(int requestor);
    void (*dma_free_channel)(int requestor);
    void (*dma_clear_channel)(int channel);
    int (*MCD_startDma)(long channel,
                        int8_t *srcAddr, unsigned int srcIncr, int8_t *destAddr, unsigned int destIncr,
                        unsigned int dmaSize, unsigned int xferSize, unsigned int initiator, int priority,
                        unsigned int flags, unsigned int funcDesc);
    int32_t (*MCD_dmaStatus)(int32_t channel);
    int32_t (*MCD_XferProgrQuery)(int32_t channel, MCD_XferProg *progRep);
    int32_t (*MCD_killDma)(int32_t channel);
    int32_t (*MCD_continDma)(int32_t channel);
    int32_t (*MCD_pauseDma)(int32_t channel);
    int32_t (*MCD_resumeDma)(int32_t channel);
    int32_t (*MCD_csumQuery)(int32_t channel, uint32_t *csum);
    void *(*dma_malloc)(uint32_t amount);
    int32_t (*dma_free)(void *addr);
};

struct xhdi_driver_interface
{
    uint32_t (*xhdivec)();
};

#include "fb.h"

struct framebuffer_driver_interface
{
    struct fb_info **framebuffer_info;  /* pointer to an fb_info struct (defined in include/fb.h) */
};

struct pci_bios_interface
{
    uint32_t subjar;
    uint32_t version;
    /* Although we declare this functions as standard gcc functions (cdecl),
     * they expect parameters inside registers (fastcall) unsupported by gcc m68k.
     * Caller will take care of parameters passing convention.
     */
    int32_t (*find_pci_device)(uint32_t id, uint16_t index);
    int32_t (*find_pci_classcode)(uint32_t class, uint16_t index);
    int32_t (*read_config_byte)(int32_t handle, uint16_t reg, uint8_t *address);
    int32_t (*read_config_word)(int32_t handle, uint16_t reg, uint16_t *address);
    int32_t (*read_config_longword)(int32_t handle, uint16_t reg, uint32_t *address);
    uint8_t (*fast_read_config_byte)(int32_t handle, uint16_t reg);
    uint16_t (*fast_read_config_word)(int32_t handle, uint16_t reg);
    uint32_t (*fast_read_config_longword)(int32_t handle, uint16_t reg);
    int32_t (*write_config_byte)(int32_t handle, uint16_t reg, uint16_t val);
    int32_t (*write_config_word)(int32_t handle, uint16_t reg, uint16_t val);
    int32_t (*write_config_longword)(int32_t handle, uint16_t reg, uint32_t val);
    int32_t (*hook_interrupt)(int32_t handle, uint32_t *routine, uint32_t *parameter);
    int32_t (*unhook_interrupt)(int32_t handle);
    int32_t (*special_cycle)(uint16_t bus, uint32_t data);
    int32_t (*get_routing)(int32_t handle);
    int32_t (*set_interrupt)(int32_t handle);
    int32_t (*get_resource)(int32_t handle);
    int32_t (*get_card_used)(int32_t handle, uint32_t *address);
    int32_t (*set_card_used)(int32_t handle, uint32_t *callback);
    int32_t (*read_mem_byte)(int32_t handle, uint32_t offset, uint8_t *address);
    int32_t (*read_mem_word)(int32_t handle, uint32_t offset, uint16_t *address);
    int32_t (*read_mem_longword)(int32_t handle, uint32_t offset, uint32_t *address);
    uint8_t (*fast_read_mem_byte)(int32_t handle, uint32_t offset);
    uint16_t (*fast_read_mem_word)(int32_t handle, uint32_t offset);
    uint32_t (*fast_read_mem_longword)(int32_t handle, uint32_t offset);
    int32_t (*write_mem_byte)(int32_t handle, uint32_t offset, uint16_t val);
    int32_t (*write_mem_word)(int32_t handle, uint32_t offset, uint16_t val);
    int32_t (*write_mem_longword)(int32_t handle, uint32_t offset, uint32_t val);
    int32_t (*read_io_byte)(int32_t handle, uint32_t offset, uint8_t *address);
    int32_t (*read_io_word)(int32_t handle, uint32_t offset, uint16_t *address);
    int32_t (*read_io_longword)(int32_t handle, uint32_t offset, uint32_t *address);
    uint8_t (*fast_read_io_byte)(int32_t handle, uint32_t offset);
    uint16_t (*fast_read_io_word)(int32_t handle, uint32_t offset);
    uint32_t (*fast_read_io_longword)(int32_t handle, uint32_t offset);
    int32_t (*write_io_byte)(int32_t handle, uint32_t offset, uint16_t val);
    int32_t (*write_io_word)(int32_t handle, uint32_t offset, uint16_t val);
    int32_t (*write_io_longword)(int32_t handle, uint32_t offset, uint32_t val);
    int32_t (*get_machine_id)(void);
    int32_t (*get_pagesize)(void);
    int32_t (*virt_to_bus)(int32_t handle, uint32_t address, PCI_CONV_ADR *pointer);
    int32_t (*bus_to_virt)(int32_t handle, uint32_t address, PCI_CONV_ADR *pointer);
    int32_t (*virt_to_phys)(uint32_t address, PCI_CONV_ADR *pointer);
    int32_t (*phys_to_virt)(uint32_t address, PCI_CONV_ADR *pointer);
    //  int32_t reserved[2];
};

struct mmu_driver_interface
{
    uint32_t (*map_page_locked)(uint32_t address, uint32_t length, int asid);
    uint32_t (*unlock_page)(uint32_t address, uint32_t length, int asid);
    uint32_t (*report_locked_pages)(uint32_t *num_itlb, uint32_t *num_dtlb);
    uint32_t (*report_pagesize)(void);
};

struct pci_native_driver_interface_0_1
{
    uint32_t (*pci_read_config_longword)(int32_t handle, int offset);
    uint16_t (*pci_read_config_word)(int32_t handle, int offset);
    uint8_t (*pci_read_config_byte)(int32_t handle, int offset);

    int32_t (*pci_write_config_longword)(int32_t handle, int offset, uint32_t value);
    int32_t (*pci_write_config_word)(int32_t handle, int offset, uint16_t value);
    int32_t (*pci_write_config_byte)(int32_t handle, int offset, uint8_t value);
    int32_t (*pci_hook_interrupt)(int32_t handle, void *handler, void *parameter);
    int32_t (*pci_unhook_interrupt)(int32_t handle);

    struct pci_rd * (*pci_get_resource)(int32_t handle);
};

struct pci_native_driver_interface
{
    uint32_t (*pci_read_config_longword)(int32_t handle, int offset);
    uint16_t (*pci_read_config_word)(int32_t handle, int offset);
    uint8_t (*pci_read_config_byte)(int32_t handle, int offset);

    int32_t (*pci_write_config_longword)(int32_t handle, int offset, uint32_t value);
    int32_t (*pci_write_config_word)(int32_t handle, int offset, uint16_t value);
    int32_t (*pci_write_config_byte)(int32_t handle, int offset, uint8_t value);
    int32_t (*pci_hook_interrupt)(int32_t handle, void *handler, void *parameter);
    int32_t (*pci_unhook_interrupt)(int32_t handle);
    int32_t (*pci_find_device)(uint16_t device_id, uint16_t vendor_id, int index);
    int32_t (*pci_find_classcode)(uint32_t classcode, int index);
    struct pci_rd * (*pci_get_resource)(int32_t handle);
};

union interface
{
    struct generic_driver_interface *gdi;
    struct xhdi_driver_interface *xhdi;
    struct dma_driver_interface *dma;
    struct framebuffer_driver_interface *fb;
    struct pci_bios_interface *pci;
    struct mmu_driver_interface *mmu;
    struct pci_native_driver_interface_0_1 *pci_native_0_1;
    struct pci_native_driver_interface *pci_native;
};

struct generic_interface
{
    enum driver_type type;
    char name[16];
    char description[64];
    int version;
    int revision;
    union interface interface;
};

struct driver_table
{
    uint32_t bas_version;
    uint32_t bas_revision;
    void (*remove_handler)(void);           /* calling this will disable the BaS' hook into trap #0 */
    struct generic_interface *interfaces;
};


#endif /* _DRIVER_VEC_H_ */
