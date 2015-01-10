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
    END_OF_DRIVERS = 0xffffffff,        /* marks end of driver list */
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

/*
 * Interpretation of offset for color fields: All offsets are from the right,
 * inside a "pixel" value, which is exactly 'bits_per_pixel' wide (means: you
 * can use the offset as right argument to <<). A pixel afterwards is a bit
 * stream and is written to video memory as that unmodified. This implies
 * big-endian byte order if bits_per_pixel is greater than 8.
 */
struct fb_bitfield
{
    unsigned long offset;           /* beginning of bitfield    */
    unsigned long length;           /* length of bitfield       */
    unsigned long msb_right;        /* != 0 : Most significant bit is */
    /* right */
};

/*
 * the following structures define the interface to the BaS-builtin-framebuffer video driver
 */
struct fb_var_screeninfo
{
    unsigned long xres;             /* visible resolution       */
    unsigned long yres;
    unsigned long xres_virtual;     /* virtual resolution       */
    unsigned long yres_virtual;
    unsigned long xoffset;          /* offset from virtual to visible */
    unsigned long yoffset;          /* resolution           */

    unsigned long bits_per_pixel;   /* guess what           */
    unsigned long grayscale;        /* != 0 Graylevels instead of colors */

    struct fb_bitfield red;         /* bitfield in fb mem if true color, */
    struct fb_bitfield green;       /* else only length is significant */
    struct fb_bitfield blue;
    struct fb_bitfield transp;      /* transparency         */

    unsigned long nonstd;           /* != 0 Non standard pixel format */

    unsigned long activate;         /* see FB_ACTIVATE_*        */

    unsigned long height;           /* height of picture in mm    */
    unsigned long width;            /* width of picture in mm     */

    unsigned long accel_flags;      /* (OBSOLETE) see fb_info.flags */

    /* Timing: All values in pixclocks, except pixclock (of course) */
    unsigned long pixclock;         /* pixel clock in ps (pico seconds) */
    unsigned long left_margin;      /* time from sync to picture    */
    unsigned long right_margin;     /* time from picture to sync    */
    unsigned long upper_margin;     /* time from sync to picture    */
    unsigned long lower_margin;
    unsigned long hsync_len;        /* length of horizontal sync    */
    unsigned long vsync_len;        /* length of vertical sync  */
    unsigned long sync;             /* see FB_SYNC_*        */
    unsigned long vmode;            /* see FB_VMODE_*       */
    unsigned long rotate;           /* angle we rotate counter clockwise */
    unsigned long refresh;
    unsigned long reserved[4];      /* Reserved for future compatibility */
};

struct fb_fix_screeninfo
{
    char id[16];                    /* identification string eg "TT Builtin" */
    unsigned long smem_start;       /* Start of frame buffer mem */
    /* (physical address) */
    unsigned long smem_len;         /* Length of frame buffer mem */
    unsigned long type;             /* see FB_TYPE_*        */
    unsigned long type_aux;         /* Interleave for interleaved Planes */
    unsigned long visual;           /* see FB_VISUAL_*      */
    unsigned short xpanstep;        /* zero if no hardware panning  */
    unsigned short ypanstep;        /* zero if no hardware panning  */
    unsigned short ywrapstep;       /* zero if no hardware ywrap    */
    unsigned long line_length;      /* length of a line in bytes    */
    unsigned long mmio_start;       /* Start of Memory Mapped I/O   */
    /* (physical address) */
    unsigned long mmio_len;         /* Length of Memory Mapped I/O  */
    unsigned long accel;            /* Indicate to driver which */
    /*  specific chip/card we have  */
    unsigned short reserved[3];     /* Reserved for future compatibility */
};

struct fb_chroma
{
    unsigned long redx;             /* in fraction of 1024 */
    unsigned long greenx;
    unsigned long bluex;
    unsigned long whitex;
    unsigned long redy;
    unsigned long greeny;
    unsigned long bluey;
    unsigned long whitey;
};

struct fb_monspecs
{
    struct fb_chroma chroma;
    struct fb_videomode *modedb;    /* mode database */
    unsigned char  manufacturer[4]; /* Manufacturer */
    unsigned char  monitor[14];     /* Monitor String */
    unsigned char  serial_no[14];   /* Serial Number */
    unsigned char  ascii[14];       /* ? */
    unsigned long modedb_len;       /* mode database length */
    unsigned long model;            /* Monitor Model */
    unsigned long serial;           /* Serial Number - Integer */
    unsigned long year;             /* Year manufactured */
    unsigned long week;             /* Week Manufactured */
    unsigned long hfmin;            /* hfreq lower limit (Hz) */
    unsigned long hfmax;            /* hfreq upper limit (Hz) */
    unsigned long dclkmin;          /* pixelclock lower limit (Hz) */
    unsigned long dclkmax;          /* pixelclock upper limit (Hz) */
    unsigned short input;           /* display type - see FB_DISP_* */
    unsigned short dpms;            /* DPMS support - see FB_DPMS_ */
    unsigned short signal;          /* Signal Type - see FB_SIGNAL_* */
    unsigned short vfmin;           /* vfreq lower limit (Hz) */
    unsigned short vfmax;           /* vfreq upper limit (Hz) */
    unsigned short gamma;           /* Gamma - in fractions of 100 */
    unsigned short gtf  : 1;        /* supports GTF */
    unsigned short misc;            /* Misc flags - see FB_MISC_* */
    unsigned char  version;         /* EDID version... */
    unsigned char  revision;        /* ...and revision */
    unsigned char  max_x;           /* Maximum horizontal size (cm) */
    unsigned char  max_y;           /* Maximum vertical size (cm) */
};

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
    int32_t (*map_page_locked)(uint32_t address, uint32_t length, int asid);
    int32_t (*unlock_page)(uint32_t address, uint32_t length, int asid);
    int32_t (*report_locked_pages)(uint32_t *num_itlb, uint32_t *num_dtlb);
    uint32_t (*report_pagesize)(void);
};

union interface
{
    struct generic_driver_interface *gdi;
    struct xhdi_driver_interface *xhdi;
    struct dma_driver_interface *dma;
    struct framebuffer_driver_interface *fb;
    struct pci_bios_interface *pci;
    struct mmu_driver_interface *mmu;
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
