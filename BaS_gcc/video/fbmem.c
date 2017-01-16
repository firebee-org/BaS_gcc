/*
 *  fbmem.c
 *
 *  Copyright (C) 1994 Martin Schaller
 *
 *	2001 - Documented with DocBook
 *	- Brad Douglas <brad@neruo.com>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include "bas_types.h"
#include "bas_printf.h"
#include "bas_string.h"
#include "fb.h"
#include "radeonfb.h"
#include "driver_mem.h"
#include "bas_string.h"

// #define DEBUG
#include "debug.h"


/*
 *  Frame buffer device initialization and setup routines
 */


int32_t fb_pan_display(struct fb_info *info, struct fb_var_screeninfo *var)
{
    int32_t xoffset = var->xoffset;
    int32_t yoffset = var->yoffset;
    int32_t err;

    dbg("\r\n");
    if ((xoffset < 0) || (yoffset < 0)
            || ((xoffset + info->var.xres) > info->var.xres_virtual))
    {
        dbg("xoffset=%d, yoffset=%d, xres=%d, xres_virtual = %d\r\n",
            xoffset, yoffset, info->var.xres, info->var.xres_virtual);
        return -1; //-EINVAL;
    }

    if ((err = info->fbops->fb_pan_display(var, info)))
    {
        dbg("fb_pan_display returned %d\r\n", err);
        return err;
    }

    info->var.xoffset = var->xoffset;
    info->var.yoffset = var->yoffset;
    if (var->vmode & FB_VMODE_YWRAP)
        info->var.vmode |= FB_VMODE_YWRAP;
    else
        info->var.vmode &= ~FB_VMODE_YWRAP;
    return 0;
}

int32_t fb_set_var(struct fb_info *info, struct fb_var_screeninfo *var)
{
    int32_t err;

    dbg("var->activate = 0x%x\r\n", var->activate);

    if (var->activate & FB_ACTIVATE_INV_MODE)
    {
        dbg("invalid mode\r\n");
        return !memcmp((char *) &info->var, (char *) var, sizeof(struct fb_var_screeninfo));
    }

    if ((var->activate & FB_ACTIVATE_FORCE)
            || memcmp((char *) &info->var, (char *) var, sizeof(struct fb_var_screeninfo)))
    {
        if ((err = info->fbops->fb_check_var(var, info)))
        {
            dbg("fb_check_var failed\r\n");
            return err;
        }

        if ((var->activate & FB_ACTIVATE_MASK) == FB_ACTIVATE_NOW)
        {
            memcpy(&info->var, var, sizeof(struct fb_var_screeninfo));
            dbg("fb_set_par() = %p\r\n", info->fbops->fb_set_par);
            info->fbops->fb_set_par(info);
            fb_pan_display(info, &info->var);
        }
    }

    return 0;
}

int32_t fb_blank(struct fb_info *info, int32_t blank)
{
    dbg("\r\n");
    if (blank > FB_BLANK_POWERDOWN)
        blank = FB_BLANK_POWERDOWN;
    return(info->fbops->fb_blank(blank, info));
}

int32_t fb_ioctl(struct fb_info *info, uint32_t cmd, uint32_t arg)
{
    struct fb_var_screeninfo var;
    struct fb_fix_screeninfo fix;
    void *argp = (void *) arg;
    int32_t i;

    dbg("\r\n");
    switch(cmd)
    {
        case FBIOGET_VSCREENINFO:
            memcpy(argp, &info->var, sizeof(var));
            return 0;

        case FBIOPUT_VSCREENINFO:
            memcpy(&var, argp, sizeof(var));
            i = fb_set_var(info, &var);
            if (i)
                return i;
            memcpy(argp, &var, sizeof(var));
            return 0;

        case FBIOGET_FSCREENINFO:
            memcpy(argp, &info->fix, sizeof(fix));
            return 0;

        case FBIOPAN_DISPLAY:
            memcpy(&var, argp, sizeof(var));
            i = fb_pan_display(info, &var);
            if (i)
                return i;
            memcpy(argp, &var, sizeof(var));
            return 0;

        case FBIOBLANK:
            i = fb_blank(info, arg);
            return i;

        case FBIO_ALLOC:
            return(offscreen_alloc(info, arg));

        case FBIO_FREE:
            return(offscreen_free(info, (void *) arg));

        default:
            return(info->fbops->fb_ioctl(cmd, arg, info));
    }
}


/**
 * framebuffer_alloc - creates a new frame buffer info structure
 *
 * @size: size of driver private data, can be zero
 * @dev: pointer to the device for this fb, this can be NULL
 *
 * Creates a new frame buffer info structure. Also reserves @size bytes
 * for driver private data (info->par). info->par (if any) will be
 * aligned to sizeof(long).
 *
 * Returns the new structure, or NULL if an error occured.
 *
 */
struct fb_info *framebuffer_alloc(uint32_t size)
{
    /* changed for BaS_gcc:
     * we do not allocate anything here anymore, info_fb is statically allocated in video.c
     * This leads to the (not really existing) limitation that we only support one Radeon
     * card in the system
     */
    extern struct fb_info *info_fb;

    dbg("\r\n");
    return info_fb;
}

/**
 * framebuffer_release - marks the structure available for freeing
 *
 * @info: frame buffer info structure
 *
 * Drop the reference count of the class_device embedded in the
 * framebuffer info structure.
 *
 */
void framebuffer_release(struct fb_info *info)
{
    dbg("\r\n");
    driver_mem_free(info->par);
}

