#ifndef _FB_H
#define _FB_H

#include <stdint.h>

/* Definitions of frame buffers	*/

#define FB_MAJOR                29
#define FB_MAX                  32	/* sufficient for now */

/* ioctls 0x46 is 'F' */
#define FBIOGET_VSCREENINFO     0x4600
#define FBIOPUT_VSCREENINFO     0x4601
#define FBIOGET_FSCREENINFO     0x4602
#define FBIOPAN_DISPLAY         0x4606
#define FBIOBLANK               0x4611      /* arg: 0 or vesa level + 1 */
#define FBIO_ALLOC              0x4613
#define FBIO_FREE               0x4614

/* picture format */
#define PICT_FORMAT(bpp, type, a, r, g, b)  (((bpp) << 24) | ((type) << 16) | ((a) << 12) | ((r) << 8) | ((g) << 4) | ((b)))

/* gray/color formats use a visual index instead of argb */
#define PICT_VISFORMAT(bpp, type, vi)   (((bpp) << 24) | ((type) << 16) | ((vi)))
#define PICT_FORMAT_BPP(f)              (((f) >> 24)       )
#define PICT_FORMAT_TYPE(f)             (((f) >> 16) & 0xff)
#define PICT_FORMAT_A(f)                (((f) >> 12) & 0x0f)
#define PICT_FORMAT_R(f)                (((f) >>  8) & 0x0f)
#define PICT_FORMAT_G(f)                (((f) >>  4) & 0x0f)
#define PICT_FORMAT_B(f)                (((f)      ) & 0x0f)
#define PICT_FORMAT_RGB(f)              (((f)      ) & 0xfff)
#define PICT_FORMAT_VIS(f)              (((f)      ) & 0xffff)

#define PICT_TYPE_OTHER         0
#define PICT_TYPE_A             1
#define PICT_TYPE_ARGB          2
#define PICT_TYPE_ABGR          3
#define PICT_TYPE_COLOR         4
#define PICT_TYPE_GRAY          5
#define PICT_FORMAT_COLOR(f)    (PICT_FORMAT_TYPE(f) & 2)

/* 32bpp formats */
#define PICT_a8r8g8b8           PICT_FORMAT(32, PICT_TYPE_ARGB, 8, 8, 8, 8)
#define PICT_x8r8g8b8           PICT_FORMAT(32, PICT_TYPE_ARGB, 0, 8, 8, 8)
#define PICT_a8b8g8r8           PICT_FORMAT(32, PICT_TYPE_ABGR, 8, 8, 8, 8)
#define PICT_x8b8g8r8           PICT_FORMAT(32, PICT_TYPE_ABGR, 0, 8, 8, 8)

/* 24bpp formats */
#define PICT_r8g8b8             PICT_FORMAT(24, PICT_TYPE_ARGB, 0, 8, 8, 8)
#define PICT_b8g8r8             PICT_FORMAT(24, PICT_TYPE_ABGR, 0, 8, 8, 8)

/* 16bpp formats */
#define PICT_r5g6b5             PICT_FORMAT(16, PICT_TYPE_ARGB, 0, 5, 6, 5)
#define PICT_b5g6r5             PICT_FORMAT(16, PICT_TYPE_ABGR, 0, 5, 6, 5)
#define PICT_a1r5g5b5           PICT_FORMAT(16, PICT_TYPE_ARGB, 1, 5, 5, 5)
#define PICT_x1r5g5b5           PICT_FORMAT(16, PICT_TYPE_ARGB, 0, 5, 5, 5)
#define PICT_a1b5g5r5           PICT_FORMAT(16, PICT_TYPE_ABGR, 1, 5, 5, 5)
#define PICT_x1b5g5r5           PICT_FORMAT(16, PICT_TYPE_ABGR, 0, 5, 5, 5)
#define PICT_a4r4g4b4           PICT_FORMAT(16, PICT_TYPE_ARGB, 4, 4, 4, 4)
#define PICT_x4r4g4b4           PICT_FORMAT(16, PICT_TYPE_ARGB, 4, 4, 4, 4)
#define PICT_a4b4g4r4           PICT_FORMAT(16, PICT_TYPE_ARGB, 4, 4, 4, 4)
#define PICT_x4b4g4r4           PICT_FORMAT(16, PICT_TYPE_ARGB, 4, 4, 4, 4)

/* 8bpp formats */
#define PICT_a8                 PICT_FORMAT(8, PICT_TYPE_A, 8, 0, 0, 0)
#define PICT_r3g3b2             PICT_FORMAT(8, PICT_TYPE_ARGB, 0, 3, 3, 2)
#define PICT_b2g3r3             PICT_FORMAT(8, PICT_TYPE_ABGR, 0, 3, 3, 2)
#define PICT_a2r2g2b2           PICT_FORMAT(8, PICT_TYPE_ARGB, 2, 2, 2, 2)
#define PICT_a2b2g2r2           PICT_FORMAT(8, PICT_TYPE_ABGR, 2, 2, 2, 2)
#define PICT_c8                 PICT_FORMAT(8, PICT_TYPE_COLOR, 0, 0, 0, 0)
#define PICT_g8                 PICT_FORMAT(8, PICT_TYPE_GRAY, 0, 0, 0, 0)

/* fVDI */
#define MODE_EMUL_MONO_FLAG     1
#define MODE_VESA_FLAG          2   /* for modedb.c */

struct mode_option
{
    short used; /* Whether the mode option was used or not. */
    short width;
    short height;
    short bpp;
    short freq;
    short flags;
};

extern struct mode_option resolution; /* fVDI */

#define FB_TYPE_PACKED_PIXELS       0	/* Packed Pixels	*/
#define FB_TYPE_PLANES              1	/* Non interleaved planes */
#define FB_TYPE_INTERLEAVED_PLANES  2	/* Interleaved planes	*/
#define FB_TYPE_TEXT                3	/* Text/attributes	*/
#define FB_TYPE_VGA_PLANES          4	/* EGA/VGA planes	*/

#define FB_AUX_TEXT_MDA             0	/* Monochrome text */
#define FB_AUX_TEXT_CGA             1	/* CGA/EGA/VGA Color text */
#define FB_AUX_TEXT_S3_MMIO         2	/* S3 MMIO fasttext */
#define FB_AUX_TEXT_MGA_STEP16      3	/* MGA Millenium I: text, attr, 14 reserved bytes */
#define FB_AUX_TEXT_MGA_STEP8       4	/* other MGAs:      text, attr,  6 reserved bytes */

#define FB_AUX_VGA_PLANES_VGA4      0	/* 16 color planes (EGA/VGA) */
#define FB_AUX_VGA_PLANES_CFB4      1	/* CFB4 in planes (VGA) */
#define FB_AUX_VGA_PLANES_CFB8      2	/* CFB8 in planes (VGA) */

#define FB_VISUAL_MONO01            0	/* Monochr. 1=Black 0=White */
#define FB_VISUAL_MONO10            1	/* Monochr. 1=White 0=Black */
#define FB_VISUAL_TRUECOLOR         2	/* True color	*/
#define FB_VISUAL_PSEUDOCOLOR       3	/* Pseudo color (like atari) */
#define FB_VISUAL_DIRECTCOLOR       4	/* Direct color */
#define FB_VISUAL_STATIC_PSEUDOCOLOR    5	/* Pseudo color readonly */

#define FB_ACCEL_NONE               0	/* no hardware accelerator	*/
#define FB_ACCEL_ATARIBLITT         1	/* Atari Blitter		*/
#define FB_ACCEL_AMIGABLITT         2	/* Amiga Blitter                */
#define FB_ACCEL_S3_TRIO64          3	/* Cybervision64 (S3 Trio64)    */
#define FB_ACCEL_NCR_77C32BLT       4	/* RetinaZ3 (NCR 77C32BLT)      */
#define FB_ACCEL_S3_VIRGE           5	/* Cybervision64/3D (S3 ViRGE)	*/
#define FB_ACCEL_ATI_MACH64GX       6	/* ATI Mach 64GX family		*/
#define FB_ACCEL_DEC_TGA            7	/* DEC 21030 TGA		*/
#define FB_ACCEL_ATI_MACH64CT       8	/* ATI Mach 64CT family		*/
#define FB_ACCEL_ATI_MACH64VT       9	/* ATI Mach 64CT family VT class */
#define FB_ACCEL_ATI_MACH64GT       10	/* ATI Mach 64CT family GT class */
#define FB_ACCEL_SUN_CREATOR        11	/* Sun Creator/Creator3D	*/
#define FB_ACCEL_SUN_CGSIX          12	/* Sun cg6			*/
#define FB_ACCEL_SUN_LEO            13	/* Sun leo/zx			*/
#define FB_ACCEL_IMS_TWINTURBO      14	/* IMS Twin Turbo		*/
#define FB_ACCEL_3DLABS_PERMEDIA2   15	/* 3Dlabs Permedia 2		*/
#define FB_ACCEL_MATROX_MGA2064W    16	/* Matrox MGA2064W (Millenium)	*/
#define FB_ACCEL_MATROX_MGA1064SG   17	/* Matrox MGA1064SG (Mystique)	*/
#define FB_ACCEL_MATROX_MGA2164W    18	/* Matrox MGA2164W (Millenium II) */
#define FB_ACCEL_MATROX_MGA2164W_AGP    19	/* Matrox MGA2164W (Millenium II) */
#define FB_ACCEL_MATROX_MGAG100     20	/* Matrox G100 (Productiva G100) */
#define FB_ACCEL_MATROX_MGAG200     21	/* Matrox G200 (Myst, Mill, ...) */
#define FB_ACCEL_SUN_CG14           22	/* Sun cgfourteen		 */
#define FB_ACCEL_SUN_BWTWO          23	/* Sun bwtwo			*/
#define FB_ACCEL_SUN_CGTHREE        24	/* Sun cgthree			*/
#define FB_ACCEL_SUN_TCX            25	/* Sun tcx			*/
#define FB_ACCEL_MATROX_MGAG400     26	/* Matrox G400			*/
#define FB_ACCEL_NV3                27	/* nVidia RIVA 128              */
#define FB_ACCEL_NV4                28	/* nVidia RIVA TNT		*/
#define FB_ACCEL_NV5                29	/* nVidia RIVA TNT2		*/
#define FB_ACCEL_CT_6555x           30	/* C&T 6555x			*/
#define FB_ACCEL_3DFX_BANSHEE       31	/* 3Dfx Banshee			*/
#define FB_ACCEL_ATI_RAGE128        32	/* ATI Rage128 family		*/
#define FB_ACCEL_IGS_CYBER2000      33	/* CyberPro 2000		*/
#define FB_ACCEL_IGS_CYBER2010      34	/* CyberPro 2010		*/
#define FB_ACCEL_IGS_CYBER5000      35	/* CyberPro 5000		*/
#define FB_ACCEL_SIS_GLAMOUR        36	/* SiS 300/630/540              */
#define FB_ACCEL_3DLABS_PERMEDIA3   37	/* 3Dlabs Permedia 3		*/
#define FB_ACCEL_ATI_RADEON         38	/* ATI Radeon family		*/
#define FB_ACCEL_I810               39      /* Intel 810/815                */
#define FB_ACCEL_SIS_GLAMOUR_2      40	/* SiS 315, 650, 740		*/
#define FB_ACCEL_SIS_XABRE          41	/* SiS 330 ("Xabre")		*/
#define FB_ACCEL_I830               42      /* Intel 830M/845G/85x/865G     */
#define FB_ACCEL_NV_10              43      /* nVidia Arch 10               */
#define FB_ACCEL_NV_20              44      /* nVidia Arch 20               */
#define FB_ACCEL_NV_30              45      /* nVidia Arch 30               */
#define FB_ACCEL_NV_40              46      /* nVidia Arch 40               */
#define FB_ACCEL_NEOMAGIC_NM2070    90	/* NeoMagic NM2070              */
#define FB_ACCEL_NEOMAGIC_NM2090    91	/* NeoMagic NM2090              */
#define FB_ACCEL_NEOMAGIC_NM2093    92	/* NeoMagic NM2093              */
#define FB_ACCEL_NEOMAGIC_NM2097    93	/* NeoMagic NM2097              */
#define FB_ACCEL_NEOMAGIC_NM2160    94	/* NeoMagic NM2160              */
#define FB_ACCEL_NEOMAGIC_NM2200    95	/* NeoMagic NM2200              */
#define FB_ACCEL_NEOMAGIC_NM2230    96	/* NeoMagic NM2230              */
#define FB_ACCEL_NEOMAGIC_NM2360    97	/* NeoMagic NM2360              */
#define FB_ACCEL_NEOMAGIC_NM2380    98	/* NeoMagic NM2380              */

#define FB_ACCEL_SAVAGE4            0x80	/* S3 Savage4                   */
#define FB_ACCEL_SAVAGE3D           0x81	/* S3 Savage3D                  */
#define FB_ACCEL_SAVAGE3D_MV        0x82	/* S3 Savage3D-MV               */
#define FB_ACCEL_SAVAGE2000         0x83	/* S3 Savage2000                */
#define FB_ACCEL_SAVAGE_MX_MV       0x84	/* S3 Savage/MX-MV              */
#define FB_ACCEL_SAVAGE_MX          0x85	/* S3 Savage/MX                 */
#define FB_ACCEL_SAVAGE_IX_MV       0x86	/* S3 Savage/IX-MV              */
#define FB_ACCEL_SAVAGE_IX          0x87	/* S3 Savage/IX                 */
#define FB_ACCEL_PROSAVAGE_PM       0x88	/* S3 ProSavage PM133           */
#define FB_ACCEL_PROSAVAGE_KM       0x89	/* S3 ProSavage KM133           */
#define FB_ACCEL_S3TWISTER_P        0x8a	/* S3 Twister                   */
#define FB_ACCEL_S3TWISTER_K        0x8b	/* S3 TwisterK                  */
#define FB_ACCEL_SUPERSAVAGE        0x8c    /* S3 Supersavage               */
#define FB_ACCEL_PROSAVAGE_DDR      0x8d	/* S3 ProSavage DDR             */
#define FB_ACCEL_PROSAVAGE_DDRK     0x8e	/* S3 ProSavage DDR-K           */

struct fb_fix_screeninfo
{
    char id[16];                    /* identification string eg "TT Builtin" */
    uint32_t smem_start;       /* Start of frame buffer mem */
    /* (physical address) */
    uint32_t smem_len;         /* Length of frame buffer mem */
    uint32_t type;             /* see FB_TYPE_*		*/
    uint32_t type_aux;         /* Interleave for interleaved Planes */
    uint32_t visual;           /* see FB_VISUAL_*		*/
    uint16_t xpanstep;        /* zero if no hardware panning  */
    uint16_t ypanstep;        /* zero if no hardware panning  */
    uint16_t ywrapstep;       /* zero if no hardware ywrap    */
    uint32_t line_length;      /* length of a line in bytes    */
    uint32_t mmio_start;       /* Start of Memory Mapped I/O   */
    /* (physical address) */
    uint32_t mmio_len;         /* Length of Memory Mapped I/O  */
    uint32_t accel;            /* Indicate to driver which	*/
    /*  specific chip/card we have	*/
    uint16_t reserved[3];     /* Reserved for future compatibility */
};

/* Interpretation of offset for color fields: All offsets are from the right,
 * inside a "pixel" value, which is exactly 'bits_per_pixel' wide (means: you
 * can use the offset as right argument to <<). A pixel afterwards is a bit
 * stream and is written to video memory as that unmodified. This implies
 * big-endian byte order if bits_per_pixel is greater than 8.
 */
struct fb_bitfield
{
    uint32_t offset;           /* beginning of bitfield	*/
    uint32_t length;           /* length of bitfield		*/
    uint32_t msb_right;        /* != 0 : Most significant bit is */
    /* right */
};

#define FB_NONSTD_HAM           1   /* Hold-And-Modify (HAM)        */

#define FB_ACTIVATE_NOW         0   /* set values immediately (or vbl)*/
#define FB_ACTIVATE_NXTOPEN     1   /* activate on next open	*/
#define FB_ACTIVATE_TEST        2   /* don't set, round up impossible */
#define FB_ACTIVATE_MASK       15
/* values */
#define FB_ACTIVATE_VBL	       16   /* activate values on next vbl  */
#define FB_CHANGE_CMAP_VBL     32   /* change colormap on vbl	*/
#define FB_ACTIVATE_ALL	       64   /* change all VCs on this fb	*/
#define FB_ACTIVATE_FORCE     128   /* force apply even when no change*/
#define FB_ACTIVATE_INV_MODE  256   /* invalidate videomode */

#define FB_ACCELF_TEXT          1   /* (OBSOLETE) see fb_info.flags and vc_mode */

#define FB_SYNC_HOR_HIGH_ACT    1   /* horizontal sync high active	*/
#define FB_SYNC_VERT_HIGH_ACT   2   /* vertical sync high active	*/
#define FB_SYNC_EXT             4   /* external sync		*/
#define FB_SYNC_COMP_HIGH_ACT   8   /* composite sync high active   */
#define FB_SYNC_BROADCAST       16  /* broadcast video timings      */
/* vtotal = 144d/288n/576i => PAL  */
/* vtotal = 121d/242n/484i => NTSC */
#define FB_SYNC_ON_GREEN        32  /* sync on green */

#define FB_VMODE_NONINTERLACED  0   /* non interlaced */
#define FB_VMODE_INTERLACED     1   /* interlaced	*/
#define FB_VMODE_DOUBLE         2   /* double scan */
#define FB_VMODE_MASK           255

#define FB_VMODE_YWRAP          256	/* ywrap instead of panning     */
#define FB_VMODE_SMOOTH_XPAN    512	/* smooth xpan possible (internally used) */
#define FB_VMODE_CONUPDATE      512	/* don't update x/yoffset	*/

#define PICOS2KHZ(a)            (1000000000UL / (a))
#define KHZ2PICOS(a)            (1000000000UL / (a))

struct fb_var_screeninfo
{
    uint32_t xres;             /* visible resolution */
    uint32_t yres;
    uint32_t xres_virtual;     /* virtual resolution */
    uint32_t yres_virtual;
    uint32_t xoffset;          /* offset from virtual to visible */
    uint32_t yoffset;          /* resolution			*/

    uint32_t bits_per_pixel;   /* guess what			*/
    uint32_t grayscale;        /* != 0 Graylevels instead of colors */

    struct fb_bitfield red;         /* bitfield in fb mem if true color, */
    struct fb_bitfield green;       /* else only length is significant */
    struct fb_bitfield blue;
    struct fb_bitfield transp;      /* transparency			*/

    uint32_t nonstd;           /* != 0 Non standard pixel format */

    uint32_t activate;         /* see FB_ACTIVATE_*		*/

    uint32_t height;           /* height of picture in mm    */
    uint32_t width;            /* width of picture in mm     */

    uint32_t accel_flags;      /* (OBSOLETE) see fb_info.flags */

    /* Timing: All values in pixclocks, except pixclock (of course) */
    uint32_t pixclock;         /* pixel clock in ps (pico seconds) */
    uint32_t left_margin;      /* time from sync to picture	*/
    uint32_t right_margin;     /* time from picture to sync	*/
    uint32_t upper_margin;     /* time from sync to picture	*/
    uint32_t lower_margin;
    uint32_t hsync_len;        /* length of horizontal sync	*/
    uint32_t vsync_len;        /* length of vertical sync	*/
    uint32_t sync;             /* see FB_SYNC_*		*/
    uint32_t vmode;            /* see FB_VMODE_*		*/
    uint32_t rotate;           /* angle we rotate counter clockwise */
    uint32_t refresh;
    uint32_t reserved[4];      /* Reserved for future compatibility */
};

/* VESA Blanking Levels */
#define VESA_NO_BLANKING        0
#define VESA_VSYNC_SUSPEND      1
#define VESA_HSYNC_SUSPEND      2
#define VESA_POWERDOWN          3

enum
{
    /* screen: unblanked, hsync: on,  vsync: on */
    FB_BLANK_UNBLANK       = VESA_NO_BLANKING,
    /* screen: blanked,   hsync: on,  vsync: on */
    FB_BLANK_NORMAL        = VESA_NO_BLANKING + 1,
    /* screen: blanked,   hsync: on,  vsync: off */
    FB_BLANK_VSYNC_SUSPEND = VESA_VSYNC_SUSPEND + 1,
    /* screen: blanked,   hsync: off, vsync: on */
    FB_BLANK_HSYNC_SUSPEND = VESA_HSYNC_SUSPEND + 1,
    /* screen: blanked,   hsync: off, vsync: off */
    FB_BLANK_POWERDOWN     = VESA_POWERDOWN + 1
};

#define FB_VBLANK_VBLANKING     0x001	/* currently in a vertical blank */
#define FB_VBLANK_HBLANKING     0x002	/* currently in a horizontal blank */
#define FB_VBLANK_HAVE_VBLANK	0x004	/* vertical blanks can be detected */
#define FB_VBLANK_HAVE_HBLANK	0x008	/* horizontal blanks can be detected */
#define FB_VBLANK_HAVE_COUNT	0x010	/* global retrace counter is available */
#define FB_VBLANK_HAVE_VCOUNT	0x020	/* the vcount field is valid */
#define FB_VBLANK_HAVE_HCOUNT	0x040	/* the hcount field is valid */
#define FB_VBLANK_VSYNCING      0x080	/* currently in a vsync */
#define FB_VBLANK_HAVE_VSYNC	0x100	/* verical syncs can be detected */

struct fb_vblank
{
    uint32_t flags;			/* FB_VBLANK flags */
    uint32_t count;			/* counter of retraces since boot */
    uint32_t vcount;			/* current scanline position */
    uint32_t hcount;			/* current scandot position */
    uint32_t reserved[4];		/* reserved for future compatibility */
};

struct vm_area_struct;
struct fb_info;
struct device;
struct file;

/* Definitions below are used in the parsed monitor specs */
#define FB_DPMS_ACTIVE_OFF      1
#define FB_DPMS_SUSPEND         2
#define FB_DPMS_STANDBY         4

#define FB_DISP_DDI             1
#define FB_DISP_ANA_700_300     2
#define FB_DISP_ANA_714_286     4
#define FB_DISP_ANA_1000_400    8
#define FB_DISP_ANA_700_000     16

#define FB_DISP_MONO            32
#define FB_DISP_RGB             64
#define FB_DISP_MULTI           128
#define FB_DISP_UNKNOWN         256

#define FB_SIGNAL_NONE          0
#define FB_SIGNAL_BLANK_BLANK   1
#define FB_SIGNAL_SEPARATE      2
#define FB_SIGNAL_COMPOSITE     4
#define FB_SIGNAL_SYNC_ON_GREEN	8
#define FB_SIGNAL_SERRATION_ON  16

#define FB_MISC_PRIM_COLOR      1
#define FB_MISC_1ST_DETAIL      2	/* First Detailed Timing is preferred */

struct fb_chroma
{
    uint32_t redx;	/* in fraction of 1024 */
    uint32_t greenx;
    uint32_t bluex;
    uint32_t whitex;
    uint32_t redy;
    uint32_t greeny;
    uint32_t bluey;
    uint32_t whitey;
};

struct fb_monspecs
{
    struct fb_chroma chroma;
    struct fb_videomode *modedb;	/* mode database */
    uint8_t  manufacturer[4];	/* Manufacturer */
    uint8_t  monitor[14];		/* Monitor String */
    uint8_t  serial_no[14];	/* Serial Number */
    uint8_t  ascii[14];		/* ? */
    uint32_t modedb_len;		/* mode database length */
    uint32_t model;			/* Monitor Model */
    uint32_t serial;			/* Serial Number - Integer */
    uint32_t year;				/* Year manufactured */
    uint32_t week;				/* Week Manufactured */
    uint32_t hfmin;			/* hfreq lower limit (Hz) */
    uint32_t hfmax;			/* hfreq upper limit (Hz) */
    uint32_t dclkmin;			/* pixelclock lower limit (Hz) */
    uint32_t dclkmax;			/* pixelclock upper limit (Hz) */
    uint16_t input;			/* display type - see FB_DISP_* */
    uint16_t dpms;			/* DPMS support - see FB_DPMS_ */
    uint16_t signal;			/* Signal Type - see FB_SIGNAL_* */
    uint16_t vfmin;			/* vfreq lower limit (Hz) */
    uint16_t vfmax;			/* vfreq upper limit (Hz) */
    uint16_t gamma;			/* Gamma - in fractions of 100 */
    uint16_t gtf	: 1;		/* supports GTF */
    uint16_t misc;			/* Misc flags - see FB_MISC_* */
    uint8_t  version;			/* EDID version... */
    uint8_t  revision;		/* ...and revision */
    uint8_t  max_x;			/* Maximum horizontal size (cm) */
    uint8_t  max_y;			/* Maximum vertical size (cm) */
};

struct fb_ops
{
    /* checks var and eventually tweaks if to something supported,
     * DO NOT MODIFY PAR  */
    int32_t (*fb_check_var)(struct fb_var_screeninfo *var, struct fb_info *info);
    /* set the video mode according to info->var */
    int32_t (*fb_set_par)(struct fb_info *info);
    /* set color register */
    int32_t (*fb_setcolreg)(uint32_t regno, uint32_t red, uint32_t green,
                        uint32_t blue, uint32_t transp, struct fb_info *info);
    /* pan display */
    int32_t (*fb_pan_display)(struct fb_var_screeninfo *var, struct fb_info *info);
    /* blank display */
    int32_t (*fb_blank)(int32_t blank, struct fb_info *info);
    /* wait for blit idle */
    int32_t (*fb_sync)(struct fb_info *info);
    /* perform fb specific ioctl */
    int32_t (*fb_ioctl)(uint32_t cmd, uint32_t arg, struct fb_info *info);
    /* Buildthe modedb for head 1 (head 2 will come later), check panel infos
     * from either BIOS or EDID, and pick up the default mode */
    void (*fb_check_modes)(struct fb_info *info, struct mode_option *resolution);
    /* Accel functions */
#define DEGREES_0	0
#define DEGREES_90	1
#define DEGREES_180	2
#define DEGREES_270	3
#define OMIT_LAST	1
    void (*SetupForSolidFill)(struct fb_info *info, int32_t color, int32_t rop, uint32_t planemask);
    void (*SubsequentSolidFillRect)(struct fb_info *info, int32_t x, int32_t y, int32_t w, int32_t h);
    void (*SetupForSolidLine)(struct fb_info *info, int32_t color, int32_t rop, uint32_t planemask);
    void (*SubsequentSolidHorVertLine)(struct fb_info *info, int32_t x, int32_t y, int32_t len, int32_t dir);
    void (*SubsequentSolidTwoPointLine)(struct fb_info *info, int32_t xa, int32_t ya, int32_t xb, int32_t yb, int32_t flags);
    void (*SetupForDashedLine)(struct fb_info *info, int32_t fg, int32_t bg, int32_t rop, uint32_t planemask, int32_t length, uint8_t *pattern);
    void (*SubsequentDashedTwoPointLine)(struct fb_info *info, int32_t xa, int32_t ya, int32_t xb, int32_t yb, int32_t flags, int32_t phase);
    void (*SetupForScreenToScreenCopy)(struct fb_info *info, int32_t xdir, int32_t ydir, int32_t rop, uint32_t planemask, int32_t trans_color);
    void (*SubsequentScreenToScreenCopy)(struct fb_info *info, int32_t xa, int32_t ya, int32_t xb, int32_t yb, int32_t w, int32_t h);
    void (*ScreenToScreenCopy)(struct fb_info *info, int32_t xa, int32_t ya, int32_t xb, int32_t yb, int32_t w, int32_t h, int32_t rop);
    void (*SetupForMono8x8PatternFill)(struct fb_info *info, int32_t patternx, int32_t patterny, int32_t fg, int32_t bg, int32_t rop, uint32_t planemask);
    void (*SubsequentMono8x8PatternFillRect)(struct fb_info *info, int32_t patternx, int32_t patterny, int32_t x, int32_t y, int32_t w, int32_t h);
    void (*SetupForScanlineCPUToScreenColorExpandFill)(struct fb_info *info, int32_t fg, int32_t bg, int32_t rop, uint32_t planemask);
    void (*SubsequentScanlineCPUToScreenColorExpandFill)(struct fb_info *info, int32_t x, int32_t y, int32_t w, int32_t h, int32_t skipleft);
    void (*SubsequentScanline)(struct fb_info *info, uint32_t *buf);
    void (*SetupForScanlineImageWrite)(struct fb_info *info, int32_t rop, uint32_t planemask, int32_t trans_color, int32_t bpp);
    void (*SubsequentScanlineImageWriteRect)(struct fb_info *info, int32_t x, int32_t y, int32_t w, int32_t h, int32_t skipleft);
    void (*SetClippingRectangle)(struct fb_info *info, int32_t xa, int32_t ya, int32_t xb, int32_t yb);
    void (*DisableClipping)(struct fb_info *info);
    int32_t (*SetupForCPUToScreenAlphaTexture)(struct fb_info *info,
                                           int32_t op, uint16_t red, uint16_t green, uint16_t blue, uint16_t alpha, uint32_t maskFormat, uint32_t dstFormat, uint8_t *alphaPtr, int32_t alphaPitch, int32_t width, int32_t height, int32_t flags);
    int32_t (*SetupForCPUToScreenTexture)(struct fb_info *info, int32_t op, uint32_t srcFormat, uint32_t dstFormat, uint8_t *texPtr, int32_t texPitch, int32_t width, int32_t height, int32_t flags);
    void (*SubsequentCPUToScreenTexture)(struct fb_info *info, int32_t dstx, int32_t dsty, int32_t srcx, int32_t srcy, int32_t width, int32_t height);
    /* Cursor functions */
    void (*SetCursorColors)(struct fb_info *info, int32_t bg, int32_t fg);
    void (*SetCursorPosition)(struct fb_info *info, int32_t x, int32_t y);
    void (*LoadCursorImage)(struct fb_info *info, uint16_t *mask, uint16_t *data, int32_t zoom);
    void (*HideCursor)(struct fb_info *info);
    void (*ShowCursor)(struct fb_info *info);
    long (*CursorInit)(struct fb_info *info);
    void (*WaitVbl)(struct fb_info *info);
};

struct fb_info
{
    struct fb_var_screeninfo var;   /* Current var */
    struct fb_fix_screeninfo fix;   /* Current fix */
    struct fb_monspecs monspecs;    /* Current Monitor specs */
    struct fb_videomode *mode;      /* current mode */
    char *screen_base;              /* Virtual address */
    uint32_t screen_size;
    char *ram_base;                 /* base vram */
    uint32_t ram_size;         /* vram size */
    char *screen_mono;
    long update_mono;
    struct fb_ops *fbops;
    void *par;                      /* device dependent */
};

/* fbmem.c */
extern int fb_pan_display(struct fb_info *info, struct fb_var_screeninfo *var);
extern int fb_set_var(struct fb_info *info, struct fb_var_screeninfo *var);
extern int fb_blank(struct fb_info *info, int blank);
extern int fb_ioctl(struct fb_info *info, uint32_t cmd, uint32_t arg);
extern struct fb_info *framebuffer_alloc(uint32_t size);
extern void framebuffer_release(struct fb_info *info);

/* offscreen.c */
extern long offscreen_free(struct fb_info *info, void *addr);
extern long offscreen_alloc(struct fb_info *info, long amount);
extern long offscren_reserved(struct fb_info *info);
extern void offscreen_init(struct fb_info *info);

/* fbmon.c */
#define FB_MAXTIMINGS		0
#define FB_VSYNCTIMINGS		1
#define FB_HSYNCTIMINGS		2
#define FB_DCLKTIMINGS		3
#define FB_IGNOREMON		0x100

#define FB_MODE_IS_UNKNOWN	0
#define FB_MODE_IS_DETAILED	1
#define FB_MODE_IS_STANDARD	2
#define FB_MODE_IS_VESA		4
#define FB_MODE_IS_CALCULATED	8
#define FB_MODE_IS_FIRST	16
#define FB_MODE_IS_FROM_VAR     32

extern void fb_destroy_modedb(struct fb_videomode *modedb);
extern int fb_parse_edid(uint8_t *edid, struct fb_var_screeninfo *var);
extern void fb_edid_to_monspecs(uint8_t *edid, struct fb_monspecs *specs);
extern int32_t fb_get_mode(int32_t flags, uint32_t val, struct fb_var_screeninfo *var, struct fb_info *info);
extern int32_t fb_validate_mode(const struct fb_var_screeninfo *var, struct fb_info *info);

/* modedb.c */
#define VESA_MODEDB_SIZE 34
extern int32_t fb_find_mode(struct fb_var_screeninfo *var,
                        struct fb_info *info, struct mode_option *resolution ,
                        const struct fb_videomode *db, uint32_t dbsize,
                        const struct fb_videomode *default_mode, uint32_t default_bpp);
extern void fb_var_to_videomode(struct fb_videomode *mode, struct fb_var_screeninfo *var);
extern void fb_videomode_to_var(struct fb_var_screeninfo *var, struct fb_videomode *mode);
extern int32_t fb_mode_is_equal(struct fb_videomode *mode1, struct fb_videomode *mode2);

struct fb_videomode
{
    uint16_t refresh;		/* optional */
    uint16_t xres;
    uint16_t yres;
    uint32_t pixclock;
    uint16_t left_margin;
    uint16_t right_margin;
    uint16_t upper_margin;
    uint16_t lower_margin;
    uint16_t hsync_len;
    uint16_t vsync_len;
    uint16_t sync;
    uint16_t vmode;
    uint16_t flag;
};

extern const struct fb_videomode vesa_modes[];

extern void start_timeout(void);
extern int32_t end_timeout(long msec);
extern void mdelay(long msec);
extern void install_vbl_timer(void *func, int32_t remove);
extern void uninstall_vbl_timer(void *func);

extern struct fb_info *info_fvdi;
#endif /* _FB_H */
