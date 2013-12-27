#ifndef __RADEONFB_H__
#define __RADEONFB_H__

//#include "config.h"
#include <bas_string.h>
#include "pci.h"
#include "mod_devicetable.h"
#include "pci_ids.h"
#include "fb.h"
#include "i2c.h"
#include "i2c-algo-bit.h"
#include "util.h"			/* for swpX() */
//#include "radeon_theatre.h"
#include "radeon_reg.h"

#ifndef point32_ter
#define point32_ter void*
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

/* Buffer are aligned on 4096 byte boundaries */
#define RADEON_BUFFER_ALIGN           0x00000fff

#define RADEON_IDLE_RETRY      16 /* Fall out of idle loops after this count */
#define RADEON_TIMEOUT    2000000 /* Fall out of wait loops after this count */
#define RADEON_MMIOSIZE   0x80000

#define RADEON_ALIGN(x,bytes) (((x) + ((bytes) - 1)) & ~((bytes) - 1))
  
#define ATY_RADEON_LCD_ON	0x00000001
#define ATY_RADEON_CRT_ON	0x00000002

#define FBIO_RADEON_GET_MIRROR	0x80044003
#define FBIO_RADEON_SET_MIRROR	0xC0044004

/***************************************************************
 * Most of the definitions here are adapted right from XFree86 *
 ***************************************************************/

/*
 * Chip families. Must fit in the low 16 bits of a int32_t word
 */
enum radeon_family
{
	CHIP_FAMILY_UNKNOW,
	CHIP_FAMILY_LEGACY,
	CHIP_FAMILY_RADEON,
	CHIP_FAMILY_RV100,
	CHIP_FAMILY_RS100,    /* U1 (IGP320M) or A3 (IGP320)*/
	CHIP_FAMILY_RV200,
	CHIP_FAMILY_RS200,    /* U2 (IGP330M/340M/350M) or A4 (IGP330/340/345/350), RS250 (IGP 7000) */
	CHIP_FAMILY_R200,
	CHIP_FAMILY_RV250,
	CHIP_FAMILY_RS300,    /* Radeon 9000 IGP */
	CHIP_FAMILY_RV280,
	CHIP_FAMILY_R300,
	CHIP_FAMILY_R350,
	CHIP_FAMILY_RV350,
	CHIP_FAMILY_RV380,    /* RV370/RV380/M22/M24 */
	CHIP_FAMILY_R420,     /* R420/R423/M18 */
	CHIP_FAMILY_LAST,
};

#define IS_RV100_VARIANT(rinfo) (((rinfo)->family == CHIP_FAMILY_RV100)  || \
				 ((rinfo)->family == CHIP_FAMILY_RV200)  || \
				 ((rinfo)->family == CHIP_FAMILY_RS100)  || \
				 ((rinfo)->family == CHIP_FAMILY_RS200)  || \
				 ((rinfo)->family == CHIP_FAMILY_RV250)  || \
				 ((rinfo)->family == CHIP_FAMILY_RV280)  || \
				 ((rinfo)->family == CHIP_FAMILY_RS300))


#define IS_R300_VARIANT(rinfo) (((rinfo)->family == CHIP_FAMILY_R300)  || \
				((rinfo)->family == CHIP_FAMILY_RV350) || \
				((rinfo)->family == CHIP_FAMILY_R350)  || \
				((rinfo)->family == CHIP_FAMILY_RV380) || \
				((rinfo)->family == CHIP_FAMILY_R420))

/*
 * Chip flags
 */
enum radeon_chip_flags
{
	CHIP_FAMILY_MASK	= 0x0000ffffUL,
	CHIP_FLAGS_MASK		= 0xffff0000UL,
	CHIP_IS_MOBILITY	= 0x00010000UL,
	CHIP_IS_IGP		= 0x00020000UL,
	CHIP_HAS_CRTC2		= 0x00040000UL,	
};

/*
 * Errata workarounds
 */
enum radeon_errata
{
	CHIP_ERRATA_R300_CG		= 0x00000001,
	CHIP_ERRATA_PLL_DUMMYREADS	= 0x00000002,
	CHIP_ERRATA_PLL_DELAY		= 0x00000004,
};


/*
 * Monitor types
 */
enum radeon_montype
{
	MT_NONE = 0,
	MT_CRT,		/* CRT */
	MT_LCD,		/* LCD */
	MT_DFP,		/* DVI */
	MT_CTV,		/* composite TV */
	MT_STV		/* S-Video out */
};

/*
 * DDC i2c ports
 */
enum ddc_type
{
	ddc_none,
	ddc_monid,
	ddc_dvi,
	ddc_vga,
	ddc_crt2,
};

/*
 * Connector types
 */
enum conn_type
{
	conn_none,
	conn_proprietary,
	conn_crt,
	conn_DVI_I,
	conn_DVI_D,
};


/*
 * PLL infos
 */
struct pll_info
{
	int32_t ppll_max;
	int32_t ppll_min;
	int32_t sclk, mclk;
	int32_t ref_div;
	int32_t ref_clk;
};


/*
 * This structure contains the various registers manipulated by this
 * driver for setting or restoring a mode. It's mostly copied from
 * XFree's RADEONSaveRec structure. A few chip settings might still be
 * tweaked without beeing reflected or saved in these registers though
 */
struct radeon_regs
{
	/* Common registers */
	uint32_t		ovr_clr;
	uint32_t		ovr_wid_left_right;
	uint32_t		ovr_wid_top_bottom;
	uint32_t		ov0_scale_cntl;
	uint32_t		mpp_tb_config;
	uint32_t		mpp_gp_config;
	uint32_t		subpic_cntl;
	uint32_t		viph_control;
	uint32_t		i2c_cntl_1;
	uint32_t		gen_int32_t_cntl;
	uint32_t		cap0_trig_cntl;
	uint32_t		cap1_trig_cntl;
	uint32_t		bus_cntl;
	uint32_t		surface_cntl;
	uint32_t		bios_5_scratch;

	/* Other registers to save for VT switches or driver load/unload */
	uint32_t		dp_datatype;
	uint32_t		rbbm_soft_reset;
	uint32_t		clock_cntl_index;
	uint32_t		amcgpio_en_reg;
	uint32_t		amcgpio_mask;

	/* Surface/tiling registers */
	uint32_t		surf_lower_bound[8];
	uint32_t		surf_upper_bound[8];
	uint32_t		surf_info[8];

	/* CRTC registers */
	uint32_t		crtc_gen_cntl;
	uint32_t		crtc_ext_cntl;
	uint32_t		dac_cntl;
	uint32_t		crtc_h_total_disp;
	uint32_t		crtc_h_sync_strt_wid;
	uint32_t		crtc_v_total_disp;
	uint32_t		crtc_v_sync_strt_wid;
	uint32_t		crtc_offset;
	uint32_t		crtc_offset_cntl;
	uint32_t		crtc_pitch;
	uint32_t		disp_merge_cntl;
	uint32_t		grph_buffer_cntl;
	uint32_t		crtc_more_cntl;

	/* CRTC2 registers */
	uint32_t		crtc2_gen_cntl;
	uint32_t		dac2_cntl;
	uint32_t		disp_output_cntl;
	uint32_t		disp_hw_debug;
	uint32_t		disp2_merge_cntl;
	uint32_t		grph2_buffer_cntl;
	uint32_t		crtc2_h_total_disp;
	uint32_t		crtc2_h_sync_strt_wid;
	uint32_t		crtc2_v_total_disp;
	uint32_t		crtc2_v_sync_strt_wid;
	uint32_t		crtc2_offset;
	uint32_t		crtc2_offset_cntl;
	uint32_t		crtc2_pitch;

	/* Flat panel regs */
	uint32_t 	fp_crtc_h_total_disp;
	uint32_t		fp_crtc_v_total_disp;
	uint32_t		fp_gen_cntl;
	uint32_t		fp2_gen_cntl;
	uint32_t		fp_h_sync_strt_wid;
	uint32_t		fp2_h_sync_strt_wid;
	uint32_t		fp_horz_stretch;
	uint32_t		fp_panel_cntl;
	uint32_t		fp_v_sync_strt_wid;
	uint32_t		fp2_v_sync_strt_wid;
	uint32_t		fp_vert_stretch;
	uint32_t		lvds_gen_cntl;
	uint32_t		lvds_pll_cntl;
	uint32_t		tmds_crc;
	uint32_t		tmds_transmitter_cntl;

	/* Computed values for PLL */
	uint32_t		dot_clock_freq;
	uint32_t		pll_output_freq;
	int32_t		feedback_div;
	int32_t		post_div;	

	/* PLL registers */
	uint32_t		ppll_div_3;
	uint32_t		ppll_ref_div;
	uint32_t		vclk_ecp_cntl;
	uint32_t		clk_cntl_index;
	uint32_t		htotal_cntl;

	/* Computed values for PLL2 */
	uint32_t		dot_clock_freq_2;
	uint32_t		pll_output_freq_2;
	int32_t		feedback_div_2;
	int32_t		post_div_2;

	/* PLL2 registers */
	uint32_t		p2pll_ref_div;
	uint32_t		p2pll_div_0;
	uint32_t		htotal_cntl2;
};

struct panel_info
{
	int32_t xres, yres;
	int32_t valid;
	int32_t clock;
	int32_t hOver_plus, hSync_width, hblank;
	int32_t vOver_plus, vSync_width, vblank;
	int32_t hAct_high, vAct_high, int32_terlaced;
	int32_t pwr_delay;
	int32_t use_bios_dividers;
	int32_t ref_divider;
	int32_t post_divider;
	int32_t fbk_divider;
};

struct radeonfb_info;

#ifdef CONFIG_FB_RADEON_I2C
struct radeon_i2c_chan
{
	struct radeonfb_info		*rinfo;
	uint32_t ddc_reg;
	struct i2c_adapter		adapter;
	struct i2c_algo_bit_data	algo;
};
#endif

enum radeon_pm_mode {
	radeon_pm_none	= 0,		/* Nothing supported */
	radeon_pm_d2	= 0x00000001,	/* Can do D2 state */
	radeon_pm_off	= 0x00000002,	/* Can resume from D3 cold */
};

typedef struct
{
	uint8_t table_revision;
	uint8_t table_size;
	uint8_t tuner_type;
	uint8_t audio_chip;
	uint8_t product_id;
	uint8_t tuner_voltage_teletext_fm;
	uint8_t i2s_config; /* configuration of the sound chip */
	uint8_t video_decoder_type;
	uint8_t video_decoder_host_config;
	uint8_t input[5];
} _MM_TABLE;

struct radeonfb_info
{
	int32_t handle;     /* PCI BIOS, must be 1st place */
	int32_t big_endian; /* PCI BIOS */
	
	uint32_t cursor_x;
	uint32_t cursor_y;
	int32_t cursor_show;
	uint32_t cursor_start;
	uint32_t cursor_end;
	int32_t cursor_fg;
	int32_t cursor_bg;

	int32_t fifo_slots;       /* Free slots in the FIFO (64 max)   */

	/* Computed values for Radeon */
	uint32_t dp_gui_master_cntl_clip;
	uint32_t trans_color;

	/* Saved values for ScreenToScreenCopy */
	int32_t xdir;
	int32_t ydir;

	/* ScanlineScreenToScreenColorExpand support */
	int32_t scanline_h;
	int32_t scanline_words;
	int32_t scanline_bpp;     /* Only used for ImageWrite */

	/* Saved values for DashedTwoPoint32_tLine */
	int32_t dashLen;
	uint32_t dashPattern;
	int32_t dash_fg;
	int32_t dash_bg;

	struct fb_info *info;

	struct radeon_regs state;
	struct radeon_regs init_state;

	uint8_t name[50];

	uint32_t io_base_phys;
	uint32_t mmio_base_phys;
	uint32_t fb_base_phys;

	void *io_base;
	void *mmio_base;
	void *fb_base;
	
	uint32_t fb_local_base;
	uint32_t fb_offset;
	
	uint32_t bios_seg_phys;
	void *bios_seg;
	int32_t fp_bios_start;

	struct
	{
		uint8_t red, green, blue, pad;
	} palette[256];

	int32_t	chipset;
	uint8_t family;
	uint8_t rev;
	int32_t errata;
	uint32_t video_ram;
	uint32_t mapped_vram;
	int32_t vram_width;
	int32_t vram_ddr;

	int32_t pitch, bpp, depth;

	int32_t has_CRTC2;
	int32_t is_mobility;
	int32_t is_IGP;
	int32_t reversed_DAC;
	int32_t reversed_TMDS;
	struct panel_info	panel_info;
	int32_t mon1_type;
	uint8_t *mon1_EDID;
	struct fb_videomode	*mon1_modedb;
	int32_t mon1_dbsize;
	int32_t mon2_type;
	uint8_t *mon2_EDID;

	uint32_t dp_gui_master_cntl;

	struct pll_info bios_pll;
	struct pll_info pll;

	uint32_t save_regs[100];
	int32_t asleep;
	int32_t lock_blank;
	int32_t dynclk;
	int32_t no_schedule;
	enum radeon_pm_mode	pm_mode;

	/* Timer used for delayed LVDS operations */
  int32_t lvds_timer;
	uint32_t pending_lvds_gen_cntl;

#ifdef CONFIG_FB_RADEON_I2C
	struct radeon_i2c_chan i2c[4];
#endif

  /* Texture */
  
	int32_t RenderInited3D;
	int32_t tilingEnabled;
	void *RenderTex;
	uint32_t RenderTexOffset;
	int32_t RenderTexSize;
	void (*RenderCallback)(struct radeonfb_info *rinfo);
	uint32_t RenderTimeout;
	uint32_t dst_pitch_offset;


#ifdef _NOT_USED_
	/* Video & theatre */

	TheatrePtr theatre;

	int32_t MM_TABLE_valid;
	_MM_TABLE MM_TABLE;

  int32_t RageTheatreCrystal;
	int32_t RageTheatreTunerPort;
	int32_t RageTheatreCompositePort;
	int32_t RageTheatreSVideoPort;
	int32_t tunerType;
	
	int32_t videoStatus;
	int32_t encoding;
	int32_t overlay_deint32_terlacing_method;
	int32_t video_stream_active;
	int32_t capture_vbi_data;
	int32_t v;
	void *videoLinear;
	int32_t videoLinearSize;
	struct
	{
		uint32_t y,u,v;
	} videoLinearOffset;
#endif /* _NOT_USED_ */
	
	int32_t dec_hue;
	int32_t dec_saturation;
	int32_t dec_contrast;
	int32_t dec_brightness;
};

#define PRIMARY_MONITOR(rinfo)	(rinfo->mon1_type)

/*
 * IO macros
 */

/* Note about this function: we have some rare cases where we must not schedule,
 * this typically happen with our special "wake up early" hook which allows us to
 * wake up the graphic chip (and thus get the console back) before everything else
 * on some machines that support that mecanism. At this point, intterrupts are off
 * and scheduling is not permitted
 */
static inline void _radeon_msleep(struct radeonfb_info *rinfo, uint32_t ms)
{
		mdelay(ms);
}

#define radeon_msleep(ms)		_radeon_msleep(rinfo,ms)

extern void _OUTREGP(struct radeonfb_info *rinfo, uint32_t addr, uint32_t val, uint32_t mask);
extern void radeon_pll_errata_after_index(struct radeonfb_info *rinfo);
extern void radeon_pll_errata_after_data(struct radeonfb_info *rinfo);
extern uint32_t __INPLL(struct radeonfb_info *rinfo, uint32_t addr);
extern void __OUTPLL(struct radeonfb_info *rinfo, uint32_t index, uint32_t val);
extern void __OUTPLLP(struct radeonfb_info *rinfo, uint32_t index, uint32_t val, uint32_t mask);

#define INREG8(addr)		*((uint8_t *)(rinfo->mmio_base+addr))
#define INREG16(addr)		swpw(*(uint16_t *)(rinfo->mmio_base+addr))
#define INREG(addr)			swpl(*(uint32_t *)(rinfo->mmio_base+addr))
#define OUTREG8(addr,val)	(*((uint8_t *)(rinfo->mmio_base+addr)) = val)
#define OUTREG16(addr,val)	(*((uint16_t *)(rinfo->mmio_base+addr)) = swpw(val))
#define OUTREG(addr,val)	(*((uint32_t *)(rinfo->mmio_base+addr)) = swpl(val))

extern int32_t *tab_funcs_pci;
#define BIOS_IN8(v)		(* ((uint8_t *) rinfo->bios_seg_phys + v))
#define BIOS_IN16(v)	(swpw(*(uint16_t *) ((uint8_t *) rinfo->bios_seg_phys + v)))
#define BIOS_IN32(v)	(swpl(*(uint32_t *) ((uint8_t *) rinfo->bios_seg_phys + v)))

#define ADDRREG(addr)			((volatile uint32_t *)(rinfo->mmio_base + (addr)))
#define OUTREGP(addr,val,mask)	_OUTREGP(rinfo, addr, val, mask)
#define INPLL(addr)				__INPLL(rinfo, addr)
#define OUTPLL(index,val)		__OUTPLL(rinfo, index, val)
#define OUTPLLP(index,val,mask)	__OUTPLLP(rinfo, index, val, mask)

/*
 * Inline utilities
 */

static inline uint32_t radeon_get_dstbpp(uint16_t depth)
{
	switch(depth)
	{
		case 8: return DST_8BPP;
		case 15: return DST_15BPP;
		case 16: return DST_16BPP;
		case 32: return DST_32BPP;
		default: return 0;
	}
}

/* I2C Functions */
extern void radeon_create_i2c_busses(struct radeonfb_info *rinfo);
extern void radeon_delete_i2c_busses(struct radeonfb_info *rinfo);
extern int32_t radeon_probe_i2c_connector(struct radeonfb_info *rinfo, int32_t conn, uint8_t **out_edid);

/* PM Functions */
/* extern int32_t radeonfb_pci_suspend(struct pci_dev *pdev, uint32_t state);
extern int32_t radeonfb_pci_resume(struct pci_dev *pdev);  */
extern void radeonfb_pm_init(struct radeonfb_info *rinfo, int32_t dynclk);
extern void radeonfb_pm_exit(struct radeonfb_info *rinfo);

/* Monitor probe functions */
extern void radeon_probe_screens(struct radeonfb_info *rinfo,
				 const uint8_t *monitor_layout, int32_t ignore_edid);
extern void radeon_check_modes(struct radeonfb_info *rinfo, struct mode_option *resolution);
extern int32_t radeon_match_mode(struct radeonfb_info *rinfo,
			     struct fb_var_screeninfo *dest,
			     const struct fb_var_screeninfo *src);

/* Video functions */
void RADEONResetVideo(struct radeonfb_info *rinfo);
int32_t RADEONVIP_read(struct radeonfb_info *rinfo, uint32_t address, uint32_t count, uint8_t *buffer);
int32_t RADEONVIP_fifo_read(struct radeonfb_info *rinfo, uint32_t address, uint32_t count, uint8_t *buffer);
int32_t RADEONVIP_write(struct radeonfb_info *rinfo, uint32_t address, uint32_t count, uint8_t *buffer);
int32_t RADEONVIP_fifo_write(struct radeonfb_info *rinfo, uint32_t address, uint32_t count, uint8_t *buffer);
void RADEONVIP_reset(struct radeonfb_info *rinfo);

void RADEONInitVideo(struct radeonfb_info *rinfo);
void RADEONShutdownVideo(struct radeonfb_info *rinfo);
int32_t RADEONPutVideo(struct radeonfb_info *rinfo, int32_t src_x, int32_t src_y, int32_t src_w, int32_t src_h,
						int32_t drw_x, int32_t drw_y, int32_t drw_w, int32_t drw_h);
void RADEONStopVideo(struct radeonfb_info *rinfo, int32_t cleanup);

/* Theatre functions */
//extern TheatrePtr DetectTheatre(struct radeonfb_info *rinfo);
//extern void RT_SetTint32_t(TheatrePtr t, int32_t hue);
//extern void RT_SetSaturation(TheatrePtr t, int32_t Saturation);
//extern void RT_SetBrightness(TheatrePtr t, int32_t Brightness);
//extern void RT_SetSharpness(TheatrePtr t, uint16_t wSharpness);
//extern void RT_SetContrast(TheatrePtr t, int32_t Contrast);
//extern void RT_SetInterlace(TheatrePtr t, uint8_t bInterlace);
//extern void RT_SetStandard(TheatrePtr t, uint16_t wStandard);
//extern void RT_SetCombFilter(TheatrePtr t, uint16_t wStandard, uint16_t wConnector);
//extern void RT_SetOutputVideoSize(TheatrePtr t, uint16_t wHorzSize, uint16_t wVertSize, uint8_t fCC_On, uint8_t fVBICap_On);
//extern void RT_SetConnector(TheatrePtr t, uint16_t wConnector, int32_t tunerFlag);
//extern void InitTheatre(TheatrePtr t);
//extern void ShutdownTheatre(TheatrePtr t);
//extern void ResetTheatreRegsForNoTVout(TheatrePtr t);
//extern void ResetTheatreRegsForTVout(TheatrePtr t);
extern void RADEONVIP_reset(struct radeonfb_info *rinfo);

/* Accel functions */

extern void RADEONWaitForFifoFunction(struct radeonfb_info *rinfo, int32_t entries);
extern void RADEONEngineFlush(struct radeonfb_info *rinfo);
extern void RADEONEngineReset(struct radeonfb_info *rinfo);
extern void RADEONEngineRestore(struct radeonfb_info *rinfo);
extern void RADEONEngineInit(struct radeonfb_info *rinfo);
extern void RADEONWaitForIdleMMIO(struct radeonfb_info *rinfo);

#define RADEONWaitForFifo(rinfo, entries)				\
do {									\
	if(rinfo->fifo_slots < entries)					\
		RADEONWaitForFifoFunction(rinfo, entries);			\
	rinfo->fifo_slots -= entries;					\
} while(0)

#define radeon_engine_flush(rinfo) RADEONEngineFlush(rinfo)
#define radeonfb_engine_reset(rinfo) RADEONEngineReset(rinfo)
#define radeonfb_engine_init(rinfo) RADEONEngineInit(rinfo)
#define radeon_engine_idle() RADEONWaitForIdleMMIO(rinfo)

static inline int radeonfb_sync(struct fb_info *info)
{
	struct radeonfb_info *rinfo = info->par;
	radeon_engine_idle();
	return 0;
}

extern void radeon_restore_accel_state_mmio(struct fb_info *info);
extern void radeon_setup_for_solid_fill(struct fb_info *info, int color, int rop, unsigned int planemask);
extern void radeon_subsequent_solid_fill_rect_mmio(struct fb_info *info, int x, int y, int w, int h);
extern void radeon_setup_for_solid_line_mmio(struct fb_info *info, int color, int rop, unsigned int planemask);
extern void radeon_subsequent_solid_hor_vert_line_mmio(struct fb_info *info, int x, int y, int len, int dir);   
extern void radeon_subsequent_solid_two_point_line_mmio(struct fb_info *info, int xa, int ya, int xb,
														int yb, int flags);  
extern void radeon_setup_for_dashed_line_mmio(struct fb_info *info, int fg, int bg,
												int rop, unsigned int planemask, int length, unsigned char *pattern);
extern void radeon_subsequent_dashed_two_point_line_mmio(struct fb_info *info,
            				int xa, int ya, int xb, int yb, int flags, int phase);  
extern void radeon_setup_for_screen_to_screen_copy_mmio(struct fb_info *info,
            				int xdir, int ydir, int rop, unsigned int planemask, int trans_color);
extern void radeon_subsequent_screen_to_screen_copy_mmio(struct fb_info *info,
            				int xa, int ya, int xb, int yb, int w, int h);   
extern void radeon_screen_to_screen_copy_mmio(struct fb_info *info,
            				int xa, int ya, int xb, int yb, int w, int h, int rop);
extern void radeon_setup_for_mono_8x8_pattern_fill_mmio(struct fb_info *info,
            			int patternx, int patterny, int fg, int bg, int rop, unsigned int planemask);
extern void radeon_subsequent_mono_8x8_pattern_fill_rect_mmio(struct fb_info *info,
            				int patternx, int patterny, int x, int y, int w, int h);
extern void radeon_setup_for_scanline_cpu_to_screen_color_expand_fill_mmio(struct fb_info *info, 
            				int fg, int bg, int rop, unsigned int planemask);
extern void radeon_subsequent_scanline_cpu_to_screen_color_expand_fill_mmio(struct fb_info *info,
            				int x, int y, int w, int h, int skipleft);
extern void radeon_subsequent_scanline_mmio(struct fb_info *info, unsigned long *buf);
extern void radeon_setup_for_scanline_image_write_mmio(struct fb_info *info,
            							int rop, unsigned int planemask, int trans_color, int bpp);
extern void radeon_subsequent_scanline_image_write_rect_mmio(struct fb_info *info,
            				int x, int y, int w, int h, int skipleft);  
extern void radeon_set_clipping_rectangle_mmio(struct fb_info *info,
            				int xa, int ya, int xb, int yb);
extern void radeon_disable_clipping_mmio(struct fb_info *info);

extern int32_t radeon_setup_for_cpu_to_screen_alpha_texture_mmio(struct fb_info *info, 
           					int op, int red, int green, int blue,
							int alpha, int maskFormat, int dstFormat,
							uint8_t *alphaPtr, int alphaPitch,
							int width, int height, int32_t flags);
extern int32_t radeon_setup_for_cpu_to_screen_texture_mmio(struct fb_info *info, int32_t op,
							uint32_t srcFormat, uint32_t dstFormat,
							uint8_t *texPtr, int32_t texPitch,
							int32_t width, int32_t height, int32_t flags);
extern void radeon_subsequent_cpu_to_screen_texture_mmio(struct fb_info *info,
            				int32_t dstx, int32_t dsty,
							int32_t srcx, int32_t srcy,
							int32_t width, int32_t height);
/* Cursor functions */
extern void radeon_set_cursor_colors(struct fb_info *info, int bg, int fg);
extern void radeon_set_cursor_position(struct fb_info *info, int x, int y);
extern void radeon_load_cursor_image(struct fb_info *info, unsigned short *mask, unsigned short *data, int zoom);
extern void radeon_hide_cursor(struct fb_info *info);
extern void radeon_show_cursor(struct fb_info *info);
extern int32_t radeon_cursor_init(struct fb_info *info);

/* Other functions */
extern int32_t radeon_screen_blank(struct radeonfb_info *rinfo, int32_t blank, int32_t mode_switch);
extern void radeon_write_mode(struct radeonfb_info *rinfo, struct radeon_regs *mode, int32_t reg_only);
int radeonfb_setcolreg(unsigned regno, unsigned red, unsigned green,
    					unsigned blue, unsigned transp, struct fb_info *info);
extern int32_t radeonfb_pci_register(int32_t handle, const struct pci_device_id *ent);
extern void radeonfb_pci_unregister(void);

/* global */
extern uint8_t monitor_layout[];
extern int16_t default_dynclk;
extern int16_t ignore_edid;
extern int16_t mirror;
extern int16_t virtual;
extern int16_t force_measure_pll;
extern int16_t zoom_mouse;

#endif /* __RADEONFB_H__ */
