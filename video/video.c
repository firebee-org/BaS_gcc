#include "video.h"
#include "videl.h"
#include "screen.h"
#include "pci.h"
#include "pci_ids.h"
#include "mod_devicetable.h"
#include "fb.h"
#include "radeonfb.h"

//#define DBG_VIDEO
#ifdef DBG_VIDEO
#define dbg(format, arg...) do { xprintf("DEBUG: " format, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DBG_VIDEO */

#ifdef _USE_VIDEL_
#define MON_ALL     -1  /* code used in VMODE_ENTRY for match on mode only */

/*
 * tables that cover all(?) valid Falcon modes
 * note:
 *  . 256-colour and Truecolor modes are not currently supported by the VDI
 */
static const VMODE_ENTRY vga_init_table[] = {
    /* the entries in this table are for VGA/NTSC (i.e. VGA 60Hz) and VGA/PAL
     * (i.e. VGA 50Hz).  in *this* table, each entry applies to four video modes:
     * mode, mode|VIDEL_VERTICAL, mode|VIDEL_PAL, mode|VIDEL_VERTICAL|VIDEL_PAL
     */
    { 0x0011, MON_ALL,  0x0017, 0x0012, 0x0001, 0x020a, 0x0009, 0x0011, 0x0419, 0x03ff, 0x003f, 0x003f, 0x03ff, 0x0415 },
    { 0x0012, MON_ALL,  0x00c6, 0x008d, 0x0015, 0x028a, 0x006b, 0x0096, 0x0419, 0x03ff, 0x003f, 0x003f, 0x03ff, 0x0415 },
    { 0x0013, MON_ALL,  0x00c6, 0x008d, 0x0015, 0x029a, 0x007b, 0x0096, 0x0419, 0x03ff, 0x003f, 0x003f, 0x03ff, 0x0415 },
    { 0x0014, MON_ALL,  0x00c6, 0x008d, 0x0015, 0x02ac, 0x0091, 0x0096, 0x0419, 0x03ff, 0x003f, 0x003f, 0x03ff, 0x0415 },
    { 0x0018, MON_ALL,  0x00c6, 0x008d, 0x0015, 0x0273, 0x0050, 0x0096, 0x0419, 0x03ff, 0x003f, 0x003f, 0x03ff, 0x0415 },
    { 0x0019, MON_ALL,  0x0017, 0x0012, 0x0001, 0x020e, 0x000d, 0x0011, 0x0419, 0x03ff, 0x003f, 0x003f, 0x03ff, 0x0415 },
    { 0x001a, MON_ALL,  0x00c6, 0x008d, 0x0015, 0x02a3, 0x007c, 0x0096, 0x0419, 0x03ff, 0x003f, 0x003f, 0x03ff, 0x0415 },
    { 0x001b, MON_ALL,  0x00c6, 0x008d, 0x0015, 0x02ab, 0x0084, 0x0096, 0x0419, 0x03ff, 0x003f, 0x003f, 0x03ff, 0x0415 },
    { 0x0092, MON_ALL,  0x0017, 0x0012, 0x0001, 0x020e, 0x000d, 0x0011, 0x0419, 0x03af, 0x008f, 0x008f, 0x03af, 0x0415 },
    { 0x0098, MON_ALL,  0x00c6, 0x008d, 0x0015, 0x0273, 0x0050, 0x0096, 0x0419, 0x03af, 0x008f, 0x008f, 0x03af, 0x0415 },
    { 0x0099, MON_ALL,  0x0017, 0x0012, 0x0001, 0x020e, 0x000d, 0x0011, 0x0419, 0x03af, 0x008f, 0x008f, 0x03af, 0x0415 },
    { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

/*
 * Initialise palette registers
 * This routine is also used by resolution change
 */
void initialise_palette_registers(int16_t rez,int16_t mode)
{
    initialise_falcon_palette(mode);
}


/*
 * determine regc0 based on video mode & monitor type
 */
static int16_t determine_regc0(int16_t mode, int16_t monitor)
{
    if (mode&VIDEL_VGA)
        return 0x0186;

    if (!(mode&VIDEL_COMPAT))
        return (monitor==MON_TV)?0x0183:0x0181;

    /* handle ST-compatible modes */
    if ((mode&(VIDEL_80COL|VIDEL_BPPMASK)) == (VIDEL_80COL|VIDEL_1BPP)) {  /* 80-column, 2-colour */
        switch(monitor) {
        case MON_MONO:
            return 0x0080;
        case MON_TV:
            return 0x0183;
        default:
            return 0x0181;
        }
    }

    return (monitor==MON_TV)?0x0083:0x0081;
}

/*
 * determine vctl based on video mode and monitor type
 */
static int16_t determine_vctl(int16_t mode,int16_t monitor)
{
    int16_t vctl;

    if (mode&VIDEL_VGA) {
        vctl = (mode&VIDEL_80COL) ? 0x08 : 0x04;
        if (mode&VIDEL_VERTICAL)
            vctl |= 0x01;
    } else {
        vctl = (mode&VIDEL_80COL) ? 0x04 : 0x00;
        if (mode&VIDEL_VERTICAL)
            vctl |= 0x02;
    }

    if (!(mode&VIDEL_COMPAT))
        return vctl;

    switch(mode&VIDEL_BPPMASK) {
    case VIDEL_1BPP:
        if (!(mode&VIDEL_VGA) && (monitor == MON_MONO))
            vctl = 0x08;
        break;
    case VIDEL_2BPP:
        vctl = (mode&VIDEL_VGA)? 0x09 : 0x04;
        break;
    case VIDEL_4BPP:
        vctl = (mode&VIDEL_VGA)? 0x05 : 0x00;
        break;
    }

    return vctl;
}


/*
 * determine scanline width based on video mode
 */
static int16_t determine_width(int16_t mode)
{
    int16_t linewidth;

    linewidth = (mode&VIDEL_80COL) ? 40 : 20;
    linewidth <<= (mode & VIDEL_BPPMASK);
    if (mode&VIDEL_OVERSCAN)
        linewidth = linewidth * 12 / 10;    /* multiply by 1.2 */

    return linewidth;
}

static int set_videl_vga(int16_t mode)
{
    volatile char *videlregs = (char *)0xffff8200;
#define videlword(n) (*(volatile uint16_t *)(videlregs+(n)))
    const VMODE_ENTRY *p;
    int16_t linewidth, monitor, vctl;

    monitor = vmontype();

    p = lookup_videl_mode(mode,monitor);/* validate mode */
    if (!p)
        return -1;

    videlregs[0x0a] = (mode&VIDEL_PAL) ? 2 : 0; /* video sync to 50Hz if PAL */

    // FIXME: vsync() can't work if the screen is initially turned off
    //vsync(); /* wait for vbl so we're not interrupted :-) */

    videlword(0x82) = p->hht;           /* H hold timer */
    videlword(0x84) = p->hbb;           /* H border begin */
    videlword(0x86) = p->hbe;           /* H border end */
    videlword(0x88) = p->hdb;           /* H display begin */
    videlword(0x8a) = p->hde;           /* H display end */
    videlword(0x8c) = p->hss;           /* H SS */

    videlword(0xa2) = p->vft;           /* V freq timer */
    videlword(0xa4) = p->vbb;           /* V border begin */
    videlword(0xa6) = p->vbe;           /* V border end */
    videlword(0xa8) = p->vdb;           /* V display begin */
    videlword(0xaa) = p->vde;           /* V display end */
    videlword(0xac) = p->vss;           /* V SS */

    videlregs[0x60] = 0x00;             /* clear ST shift for safety */

    videlword(0x0e) = 0;                /* offset */

    linewidth = determine_width(mode);
    vctl = determine_vctl(mode,monitor);

    videlword(0x10) = linewidth;        /* scanline width */
    videlword(0xc2) = vctl;             /* video control */
    videlword(0xc0) = determine_regc0(mode,monitor);
    videlword(0x66) = 0x0000;           /* clear SPSHIFT */

    switch(mode&VIDEL_BPPMASK) {        /* set SPSHIFT / ST shift */
    case VIDEL_1BPP:                    /* 2 colours (mono) */
        if (monitor == MON_MONO)
            videlregs[0x60] = 0x02;
        else videlword(0x66) = 0x0400;
        break;
    case VIDEL_2BPP:                    /* 4 colours */
        videlregs[0x60] = 0x01;
        videlword(0x10) = linewidth;        /* writing to the ST shifter has    */
        videlword(0xc2) = vctl;             /* just overwritten these registers */
        break;
    case VIDEL_4BPP:                    /* 16 colours */
        /* if not ST-compatible, SPSHIFT was already set correctly above */
        if (mode&VIDEL_COMPAT)
            videlregs[0x60] = 0x00;         /* else set ST shifter */
        break;
    case VIDEL_8BPP:                    /* 256 colours */
        videlword(0x66) = 0x0010;
        break;
    case VIDEL_TRUECOLOR:               /* 65536 colours (Truecolor) */
        videlword(0x66) = 0x0100;
        break;
    }

    return 0;
}

int16_t current_video_mode;

/* Set physical screen address */

static void setphys(int32_t addr,int checkaddr)
{
	*(volatile uint8_t *) VIDEOBASE_ADDR_HI = ((uint32_t) addr) >> 16;
	*(volatile uint8_t *) VIDEOBASE_ADDR_MID = ((uint32_t) addr) >> 8;
	*(volatile uint8_t *) VIDEOBASE_ADDR_LOW = ((uint32_t) addr);
}

/*
 * In the original TOS there used to be an early screen init,
 * before memory configuration. This is not used here, and all is
 * done at the same time from C.
 */

void videl_screen_init(void)
{
	uint32_t screen_start;
	uint16_t boot_resolution = FALCON_DEFAULT_BOOT;
	int16_t monitor_type, sync_mode;
	int16_t rez = 0;   /* avoid 'may be uninitialized' warning */

	/* Initialize the interrupt handlers.
	 * It is important to do this first because the initialization code below
	 * may call vsync(), which temporarily enables the interrupts. */

	/* TODO: VEC_HBL = int_hbl; */
	/* TODO: VEC_VBL = int_vbl; */

/*
 * first, see what we're connected to, and set the
 * resolution / video mode appropriately
 */
	monitor_type = MON_COLOR;
	xprintf("monitor_type = %d\r\n", monitor_type);

	/* reset VIDEL on boot-up */
	/* first set the physbase to a safe memory */
	setphys(0xd00000, 0);

	if (!lookup_videl_mode(boot_resolution, monitor_type)) { /* mode isn't in table */
	    xprintf("Invalid video mode 0x%04x changed to 0x%04x\r\n",
	            boot_resolution, FALCON_DEFAULT_BOOT);
	    boot_resolution = FALCON_DEFAULT_BOOT;  /* so pick one that is */
	}

	if (!VALID_VDI_BPP(boot_resolution)) {      /* mustn't confuse VDI */
	    xprintf("VDI doesn't support video mode 0x%04x, changed to 0x%04x\r\n",
	            boot_resolution, FALCON_DEFAULT_BOOT);
	    boot_resolution = FALCON_DEFAULT_BOOT;  /* so use default */
	}

	vsetmode(boot_resolution);
	rez = FALCON_REZ;   /* fake value indicates Falcon/Videl */
	sync_mode = (boot_resolution & VIDEL_PAL) ? 0x02 : 0x00;
	*(volatile uint8_t *) SYNCMODE = sync_mode;

	/*
	 * next, set up the palette(s)
	 */
	initialise_palette_registers(rez, boot_resolution);
	/* FIXME: sshiftmod = rez; */

	/* videoram is placed just below the phystop */
	screen_start = 0xd00000;

	/* correct physical address */
	setphys(screen_start, 1);
}

#endif /* _USE_VIDEL_ */

static struct fb_info fb;
struct fb_info *info_fb = &fb;

const char monitor_layout[1024] = "CRT,CRT";
int16_t ignore_edid;

struct mode_option resolution = 
{
	.used = 0,
	.width = 640,
	.height = 480,
	.bpp = 8,
	.freq = 60,
	.flags = 0
};
int16_t force_measure_pll;

void install_vbl_timer(void *func, int remove)
{
	dbg("%s: not implemented\r\n", __FUNCTION__);
}

/*
 * detect and initialise PCI graphics cards
 */
void video_init(void)
{
	/*
	 * detect PCI video card
	 */
	
	int index = 0;
	int32_t handle;
	struct pci_device_id *board;
	int32_t id;
	bool radeon_found = false;
	
	dbg("%s\r\n", __FUNCTION__);
	do
	{
		/*
		 * scan PCI bus for graphics cards
		 */
		handle = pci_find_classcode(PCI_BASE_CLASS_DISPLAY | PCI_FIND_BASE_CLASS, index);
		if (handle > 0)		/* found a display device */
		{
			dbg("%s: handle = 0x%x\r\n", __FUNCTION__, handle);

			id = swpl(pci_read_config_longword(handle, PCIIDR));		/* get vendor + device id */
			dbg("%s: PCIIDR=0x%x\r\n", __FUNCTION__, id);

			board = &radeonfb_pci_table[0];

			do
			{
				/* check it against elements of table */
				dbg("%s: check %x %x against %08x\r\n", __FUNCTION__, board->device, board->vendor, id);
				if ((board->device == (id >> 16)) && (board->vendor == (id & 0xffff)))
				{
					radeon_found = true;

					dbg("%s: matched\r\n", __FUNCTION__);
					if (radeonfb_pci_register(handle, board) >= 0)
					{
						xprintf("RADEON video card found and registered\r\n");
					}
					else
					{
						dbg("%s: failed to register RADEON PCI video card\r\n", __FUNCTION__);
					}
					return;
				}
				board++;
			} while (board->vendor);
		}
		index++;
	} while (handle > 0);
	xprintf("%s: RADEON video card %sfound and %sregistered\r\n", __FUNCTION__,
					(radeon_found ? "" : "not "), (radeon_found ? "" : "not "));
}


