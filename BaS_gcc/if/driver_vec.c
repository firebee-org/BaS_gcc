/*
 * driver_vec.c
 *
 * Expose BaS drivers to OS
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
 *
 *  Created on: 24.10.2013
 *      Author: Markus Fröschle
 */

#include <bas_types.h>
#include "version.h"
#include "xhdi_sd.h"
#include "dma.h"
#include "driver_vec.h"
#include "driver_mem.h"
#include "pci.h"
#include "mmu.h"

/*
 * driver interface struct for the SD card BaS driver
 */
static struct xhdi_driver_interface xhdi_call_interface =
{
    xhdi_call
};

/*
 * driver interface struct for the BaS multichannel DMA driver
 * This is exactly the same thing FireTOS provides and the MiNT FEC drivers expect.
 * It can be directly used in TOS to register the corresponding cookie ("DMAC").
 */
static struct dma_driver_interface dma_interface =
{
    .version = 0x0101,
    .magic = 0x444d4143, 						/* 'DMAC' */
    .dma_set_initiator = dma_set_initiator,
    .dma_get_initiator = dma_get_initiator,
    .dma_free_initiator = dma_free_initiator,
    .dma_set_channel = dma_set_channel,
    .dma_get_channel = dma_get_channel,
    .dma_free_channel = dma_free_channel,
    .dma_clear_channel = dma_clear_channel,
    .MCD_startDma = (int (*)(long, int8_t *, unsigned int, int8_t *, unsigned int,
                    unsigned int, unsigned int, unsigned int, int,
                    unsigned int, unsigned int)) MCD_startDma,
    .MCD_dmaStatus = (int32_t (*)(int32_t)) MCD_dmaStatus,
    .MCD_XferProgrQuery = (int32_t (*)(int32_t, MCD_XferProg *)) MCD_XferProgrQuery,
    .MCD_killDma = (int32_t (*)(int32_t)) MCD_killDma,
    .MCD_continDma = (int32_t (*)(int32_t)) MCD_continDma,
    .MCD_pauseDma = (int32_t (*)(int32_t)) MCD_pauseDma,
    .MCD_resumeDma = (int32_t (*)(int32_t)) MCD_resumeDma,
    .MCD_csumQuery = (int32_t (*)(int32_t, uint32_t *)) MCD_csumQuery,
    .dma_malloc = driver_mem_alloc,
    .dma_free = driver_mem_free
};

extern struct fb_info *info_fb;

/*
 * driver interface struct for the PCI_BIOS BaS driver
 */
static struct pci_bios_interface pci_interface =
{
    .subjar = 0,
    .version = 0x00010000,
    .find_pci_device = wrapper_find_pci_device,
    .find_pci_classcode = wrapper_find_pci_classcode,
    .read_config_byte = wrapper_read_config_byte,
    .read_config_word = wrapper_read_config_word,
    .read_config_longword = wrapper_read_config_longword,
    .fast_read_config_byte = wrapper_fast_read_config_byte,
    .fast_read_config_word = wrapper_fast_read_config_word,
    .fast_read_config_longword = wrapper_fast_read_config_longword,
    .write_config_byte = wrapper_write_config_byte,
    .write_config_word = wrapper_write_config_word,
    .write_config_longword = wrapper_write_config_longword,
    .hook_interrupt = wrapper_hook_interrupt,
    .unhook_interrupt = wrapper_unhook_interrupt,
    .special_cycle = wrapper_special_cycle,
    .get_routing = wrapper_get_routing,
    .set_interrupt = wrapper_set_interrupt,
    .get_resource = wrapper_get_resource,
    .get_card_used = wrapper_get_card_used,
    .set_card_used = wrapper_set_card_used,
    .read_mem_byte = wrapper_read_mem_byte,
    .read_mem_word = wrapper_read_mem_word,
    .read_mem_longword = wrapper_read_mem_longword,
    .fast_read_mem_byte = wrapper_fast_read_mem_byte,
    .fast_read_mem_word = wrapper_fast_read_mem_word,
    .fast_read_mem_longword = wrapper_fast_read_mem_longword,
    .write_mem_byte = wrapper_write_mem_byte,
    .write_mem_word = wrapper_write_mem_word,
    .write_mem_longword = wrapper_write_mem_longword,
    .read_io_byte = wrapper_read_io_byte,
    .read_io_word = wrapper_read_io_word,
    .read_io_longword = wrapper_read_io_longword,
    .fast_read_io_byte = wrapper_fast_read_io_byte,
    .fast_read_io_word = wrapper_fast_read_io_word,
    .fast_read_io_longword = wrapper_fast_read_io_longword,
    .write_io_byte = wrapper_write_io_byte,
    .write_io_word = wrapper_write_io_word,
    .write_io_longword = wrapper_write_io_longword,
    .get_machine_id = wrapper_get_machine_id,
    .get_pagesize = wrapper_get_pagesize,
    .virt_to_bus = wrapper_virt_to_bus,
    .bus_to_virt = wrapper_bus_to_virt,
    .virt_to_phys = wrapper_virt_to_phys,
    .phys_to_virt = wrapper_phys_to_virt,
};

static struct pci_native_driver_interface pci_native_interface =
{
    .pci_read_config_longword = pci_read_config_longword,
    .pci_read_config_word = pci_read_config_word,
    .pci_read_config_byte = pci_read_config_byte,
    .pci_write_config_longword = pci_write_config_longword,
    .pci_write_config_word = pci_write_config_word,
    .pci_write_config_byte = pci_write_config_byte,
    .pci_hook_interrupt = pci_hook_interrupt,
    .pci_unhook_interrupt = pci_unhook_interrupt,
    .pci_get_resource = pci_get_resource,
};

/*
 * driver interface struct for the BaS framebuffer video driver
 */
static struct framebuffer_driver_interface framebuffer_interface =
{
    .framebuffer_info = &info_fb
};

/*
 * driver interface struct for the BaS MMU driver
 */
static struct mmu_driver_interface mmu_interface =
{
    .map_page_locked = &mmu_map_data_page_locked,
    .unlock_page = &mmu_unlock_data_page,
    .report_locked_pages = &mmu_report_locked_pages,
    .report_pagesize = &mmu_report_pagesize
};

static struct generic_interface interfaces[] =
{
    {
        /* BaS SD-card driver interface */

        .type = XHDI_DRIVER,
        .name = "SDCARD",
        .description = "BaS SD Card driver",
        .version = 0,
        .revision = 1,
        .interface.xhdi = &xhdi_call_interface
    },
    {
        .type = MCD_DRIVER,
        .name = "MCDDMA",
        .description = "BaS Multichannel DMA driver",
        .version = 0,
        .revision = 1,
        .interface.dma = &dma_interface,
    },
    {
        .type = VIDEO_DRIVER,
        .name = "RADEON",
        .description = "BaS RADEON framebuffer driver",
        .version = 0,
        .revision = 1,
        .interface.fb = &framebuffer_interface,
    },
    {
        .type = PCI_DRIVER,
        .name = "PCI",
        .description = "BaS PCI_BIOS driver",
        .version = 0,
        .revision = 1,
        .interface.pci = &pci_interface,
    },
    {
        .type = MMU_DRIVER,
        .name = "MMU",
        .description = "BaS MMU driver",
        .version = 0,
        .revision = 1,
        .interface.mmu = &mmu_interface,
    },
    {
        .type = PCI_NATIVE_DRIVER,
        .name = "PCI_N",
        .description = "BaS PCI native",
        .version = 0,
        .revision = 1,
        .interface.pci_native = &pci_native_interface,
    },
    /* insert new drivers here */

    {
        .type = END_OF_DRIVERS
    }
};

extern void remove_handler(void);   /* forward declaration */

/*
 * this is the driver table we expose to the OS
 */
static struct driver_table bas_drivers =
{
    .bas_version = MAJOR_VERSION,
    .bas_revision = MINOR_VERSION,
    .remove_handler = remove_handler,
    .interfaces = interfaces
};

void remove_handler(void)
{
    extern void std_exc_vec(void);
    uint32_t *trap_0_vector = (uint32_t *) 0x80;

    *trap_0_vector = (uint32_t) std_exc_vec;
}

void __attribute__((interrupt)) get_bas_drivers(void)
{
    __asm__ __volatile(
        /*
         * sp should now point to the next instruction after the trap
         * The trap itself is 2 bytes, the four bytes before that must
         * read '_BAS' or we are not meant by this call
         */
        "		move.l	a0,-(sp)				\n\t"	// save registers
        "       move.l  d0,-(sp)                \n\t"
        "		move.l	12(sp),a0         		\n\t"	// get return address
        "       move.l  -6(a0),d0               \n\t"   //
        "		cmp.l	#0x5f424153,d0			\n\t"	// is it '_BAS'?
        "		beq		fetch_drivers			\n\t"	// yes
        /*
         * This seems indeed a "normal" trap #0. Better pass control to "normal" trap #0 processing
         * If trap #0 isn't set to something sensible, we'll probably crash here, but this must be
         * prevented on the caller side.
         */
        "		move.l	(sp)+,d0				\n\t"	// restore registers
        "		move.l	(sp)+,a0				\n\t"
        "		move.l	0x80,-(sp)				\n\t"	// fetch vector
        "		rts								\n\t"	// and jump through it

        "fetch_drivers:							\n\t"
        "		move.l	#%[drivers],d0			\n\t"	// return driver struct in d0
        "		addq.l	#4,sp					\n\t"	// adjust stack
        "       move.l  (sp)+,a0                \n\t"   // restore register
        :                                   /* no output */
        :	[drivers] "o" (bas_drivers)		/* input */
        :									/* clobber */
    );
}
