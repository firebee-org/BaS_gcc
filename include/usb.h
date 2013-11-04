/*
 * (C) Copyright 2001
 * Denis Peter, MPL AG Switzerland
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Note: Part of this code has been derived from linux
 *
 */
#ifndef _USB_H_
#define _USB_H_

//#include <stdlib.h>
#include <bas_string.h>
#include "pci.h"
#include "mod_devicetable.h"
#include "pci_ids.h"
#include "part.h"


extern long *tab_funcs_pci;

#define in8(addr)		Fast_read_mem_byte(usb_handle,addr)
#define in16r(addr)		Fast_read_mem_word(usb_handle,addr)
#define in32r(addr)		Fast_read_mem_longword(usb_handle,addr)
#define out8(addr,val)	Write_mem_byte(usb_handle,addr,val)
#define out16r(addr,val)	Write_mem_word(usb_handle,addr,val)
#define out32r(addr,val)	Write_mem_longword(usb_handle,addr,val)


#define __u8 uint8_t
#define __u16 uint16_t
#define __u32 uint32_t
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define uint8_t uint8_t
#define uint32_t uint32_t
#define uint16_t uint16_t

extern void kprint(const char *fmt, ...);
extern int sprintD(char *s, const char *fmt, ...);

#include "usb_defs.h"

/* Everything is aribtrary */
#define USB_ALTSETTINGALLOC		4
#define USB_MAXALTSETTING		128	/* Hard limit */

#define USB_MAX_BUS           3
#define USB_MAX_DEVICE       16
#define USB_MAXCONFIG         8
#define USB_MAXINTERFACES     8
#define USB_MAXENDPOINTS     16
#define USB_MAXCHILDREN       8	/* This is arbitrary */
#define USB_MAX_HUB          16

#define USB_CNTL_TIMEOUT 100 /* 100ms timeout */

/* String descriptor */
struct usb_string_descriptor {
	uint8_t	bLength;
	uint8_t	bDescriptorType;
	uint16_t	wData[1];
} __attribute__ ((packed));

/* device request (setup) */
struct devrequest {
	uint8_t	requesttype;
	uint8_t	request;
	uint16_t	value;
	uint16_t	index;
	uint16_t	length;
} __attribute__ ((packed));

/* All standard descriptors have these 2 fields in common */
struct usb_descriptor_header {
	uint8_t	bLength;
	uint8_t	bDescriptorType;
} __attribute__ ((packed));

/* Device descriptor */
struct usb_device_descriptor {
	uint8_t	bLength;
	uint8_t	bDescriptorType;
	uint16_t	bcdUSB;
	uint8_t	bDeviceClass;
	uint8_t	bDeviceSubClass;
	uint8_t	bDeviceProtocol;
	uint8_t	bMaxPacketSize0;
	uint16_t	idVendor;
	uint16_t	idProduct;
	uint16_t	bcdDevice;
	uint8_t	iManufacturer;
	uint8_t	iProduct;
	uint8_t	iSerialNumber;
	uint8_t	bNumConfigurations;
} __attribute__ ((packed));

/* Endpoint descriptor */
struct usb_endpoint_descriptor {
	uint8_t	bLength;
	uint8_t	bDescriptorType;
	uint8_t	bEndpointAddress;
	uint8_t	bmAttributes;
	uint16_t	wMaxPacketSize;
	uint8_t	bInterval;
	uint8_t	bRefresh;
	uint8_t	bSynchAddress;
} __attribute__ ((packed)) __attribute__ ((aligned(2)));

/* Interface descriptor */
struct usb_interface_descriptor {
	uint8_t	bLength;
	uint8_t	bDescriptorType;
	uint8_t	bInterfaceNumber;
	uint8_t	bAlternateSetting;
	uint8_t	bNumEndpoints;
	uint8_t	bInterfaceClass;
	uint8_t	bInterfaceSubClass;
	uint8_t	bInterfaceProtocol;
	uint8_t	iInterface;

	uint8_t	no_of_ep;
	uint8_t	num_altsetting;
	uint8_t	act_altsetting;

	struct usb_endpoint_descriptor ep_desc[USB_MAXENDPOINTS];
} __attribute__ ((packed));


/* Configuration descriptor information.. */
struct usb_config_descriptor {
	uint8_t	bLength;
	uint8_t	bDescriptorType;
	uint16_t	wTotalLength;
	uint8_t	bNumInterfaces;
	uint8_t	bConfigurationValue;
	uint8_t	iConfiguration;
	uint8_t	bmAttributes;
	uint8_t	MaxPower;

	uint8_t	no_of_if;	/* number of interfaces */
	struct usb_interface_descriptor if_desc[USB_MAXINTERFACES];
} __attribute__ ((packed));

enum {
	/* Maximum packet size; encoded as 0,1,2,3 = 8,16,32,64 */
	PACKET_SIZE_8   = 0,
	PACKET_SIZE_16  = 1,
	PACKET_SIZE_32  = 2,
	PACKET_SIZE_64  = 3,
};

struct usb_device {
	int	devnum;			/* Device number on USB bus */
	int	speed;			/* full/low/high */
	char mf[32];			/* manufacturer */
	char prod[32];		/* product */
	char serial[32];		/* serial number */

	/* Maximum packet size; one of: PACKET_SIZE_* */
	int maxpacketsize;
	/* one bit for each endpoint ([0] = IN, [1] = OUT) */
	unsigned int toggle[2];
	/* endpoint halts; one bit per endpoint # & direction;
	 * [0] = IN, [1] = OUT
	 */
	unsigned int halted[2];
	int epmaxpacketin[16];		/* INput endpoint specific maximums */
	int epmaxpacketout[16];		/* OUTput endpoint specific maximums */

	int configno;			/* selected config number */
	struct usb_device_descriptor descriptor; /* Device Descriptor */
	struct usb_config_descriptor config; /* config descriptor */

	int have_langid;		/* whether string_langid is valid yet */
	int string_langid;		/* language ID for strings */
	int (*irq_handle)(struct usb_device *dev);
	uint32_t irq_status;
	int irq_act_len;		/* transfered bytes */
	void *privptr;
	/*
	 * Child devices -  if this is a hub device
	 * Each instance needs its own set of data structures.
	 */
	uint32_t status;
	int act_len;			/* transfered bytes */
	int maxchild;			/* Number of ports if hub */
	int portnr;
	struct usb_device *parent;
	struct usb_device *children[USB_MAXCHILDREN];
	void *priv_hcd;
	int (*deregister)(struct usb_device *dev);

	struct usb_hub_device *hub;
	int usbnum;
};

typedef struct
{
	long ident;
	union
	{
		long l;
		short i[2];
		char c[4];
	} v;
} USB_COOKIE;

/**********************************************************************
 * this is how the lowlevel part communicate with the outer world
 */

int ohci_usb_lowlevel_init(uint16_t handle, const struct pci_device_id *ent, void **priv);
int ohci_usb_lowlevel_stop(void *priv);
int ohci_submit_bulk_msg(struct usb_device *dev, uint32_t pipe, void *buffer, int transfer_len);
int ohci_submit_control_msg(struct usb_device *dev, uint32_t pipe, void *buffer, int transfer_len, struct devrequest *setup);
int ohci_submit_int_msg(struct usb_device *dev, uint32_t pipe, void *buffer, int transfer_len, int interval);
void ohci_usb_enable_interrupt(int enable);

int ehci_usb_lowlevel_init(long handle, const struct pci_device_id *ent, void **priv);
int ehci_usb_lowlevel_stop(void *priv);
int ehci_submit_bulk_msg(struct usb_device *dev, uint32_t pipe, void *buffer, int transfer_len);
int ehci_submit_control_msg(struct usb_device *dev, uint32_t pipe, void *buffer, int transfer_len, struct devrequest *setup);
int ehci_submit_int_msg(struct usb_device *dev, uint32_t pipe, void *buffer, int transfer_len, int interval);
void ehci_usb_enable_interrupt(int enable);

void usb_enable_interrupt(int enable);

#define USB_MAX_STOR_DEV 5
block_dev_desc_t *usb_stor_get_dev(int index);
int usb_stor_scan(void);
int usb_stor_info(void);
int usb_stor_register(struct usb_device *dev);
int usb_stor_deregister(struct usb_device *dev);

int drv_usb_kbd_init(void);
int usb_kbd_register(struct usb_device *dev);
int usb_kbd_deregister(struct usb_device *dev);

int drv_usb_mouse_init(void);
int usb_mouse_register(struct usb_device *dev);
int usb_mouse_deregister(struct usb_device *dev);

extern char usb_error_str[256];

/* memory */
void *usb_malloc(long amount);
int usb_free(void *addr);
int usb_mem_init(void);
void usb_mem_stop(void);

/* routines */
USB_COOKIE *usb_get_cookie(long id);
void usb_error_msg(const char *const fmt, ... );
int usb_init(uint16_t handle, const struct pci_device_id *ent); /* initialize the USB Controller */
int usb_stop(void); /* stop the USB Controller */

int usb_set_protocol(struct usb_device *dev, int ifnum, int protocol);
int usb_set_idle(struct usb_device *dev, int ifnum, int duration, int report_id);
struct usb_device *usb_get_dev_index(int index, int bus);
int usb_control_msg(struct usb_device *dev, unsigned int pipe, uint8_t request, uint8_t requesttype, uint16_t value, 
 uint16_t index, void *data, uint16_t size, int timeout);
int usb_bulk_msg(struct usb_device *dev, unsigned int pipe, void *data, int len, int *actual_length, int timeout);
int usb_submit_int_msg(struct usb_device *dev, uint32_t pipe, void *buffer, int transfer_len, int interval);
void usb_disable_asynch(int disable);
int usb_maxpacket(struct usb_device *dev, uint32_t pipe);
void wait_ms(uint32_t ms);
int usb_get_configuration_no(struct usb_device *dev, uint8_t *buffer, int cfgno);
int usb_get_report(struct usb_device *dev, int ifnum, uint8_t type, uint8_t id, void *buf, int size);
int usb_get_class_descriptor(struct usb_device *dev, int ifnum, uint8_t type, uint8_t id, void *buf, int size);
int usb_clear_halt(struct usb_device *dev, int pipe);
int usb_string(struct usb_device *dev, int index, char *buf, size_t size);
int usb_set_interface(struct usb_device *dev, int interface, int alternate);

/*
 * Calling this entity a "pipe" is glorifying it. A USB pipe
 * is something embarrassingly simple: it basically consists
 * of the following information:
 *  - device number (7 bits)
 *  - endpoint number (4 bits)
 *  - current Data0/1 state (1 bit)
 *  - direction (1 bit)
 *  - speed (2 bits)
 *  - max packet size (2 bits: 8, 16, 32 or 64)
 *  - pipe type (2 bits: control, interrupt, bulk, isochronous)
 *
 * That's 18 bits. Really. Nothing more. And the USB people have
 * documented these eighteen bits as some kind of glorious
 * virtual data structure.
 *
 * Let's not fall in that trap. We'll just encode it as a simple
 * unsigned int. The encoding is:
 *
 *  - max size:		bits 0-1	(00 = 8, 01 = 16, 10 = 32, 11 = 64)
 *  - direction:	bit 7		(0 = Host-to-Device [Out],
 *					(1 = Device-to-Host [In])
 *  - device:		bits 8-14
 *  - endpoint:		bits 15-18
 *  - Data0/1:		bit 19
 *  - speed:		bit 26		(0 = Full, 1 = Low Speed, 2 = High)
 *  - pipe type:	bits 30-31	(00 = isochronous, 01 = interrupt,
 *					 10 = control, 11 = bulk)
 *
 * Why? Because it's arbitrary, and whatever encoding we select is really
 * up to us. This one happens to share a lot of bit positions with the UHCI
 * specification, so that much of the uhci driver can just mask the bits
 * appropriately.
 */
/* Create various pipes... */
#define create_pipe(dev,endpoint) \
		(((dev)->devnum << 8) | (endpoint << 15) | \
		((dev)->speed << 26) | (dev)->maxpacketsize)
#define default_pipe(dev) ((dev)->speed << 26)

#define usb_sndctrlpipe(dev, endpoint)	((PIPE_CONTROL << 30) | \
					 create_pipe(dev, endpoint))
#define usb_rcvctrlpipe(dev, endpoint)	((PIPE_CONTROL << 30) | \
					 create_pipe(dev, endpoint) | \
					 USB_DIR_IN)
#define usb_sndisocpipe(dev, endpoint)	((PIPE_ISOCHRONOUS << 30) | \
					 create_pipe(dev, endpoint))
#define usb_rcvisocpipe(dev, endpoint)	((PIPE_ISOCHRONOUS << 30) | \
					 create_pipe(dev, endpoint) | \
					 USB_DIR_IN)
#define usb_sndbulkpipe(dev, endpoint)	((PIPE_BULK << 30) | \
					 create_pipe(dev, endpoint))
#define usb_rcvbulkpipe(dev, endpoint)	((PIPE_BULK << 30) | \
					 create_pipe(dev, endpoint) | \
					 USB_DIR_IN)
#define usb_sndintpipe(dev, endpoint)	((PIPE_INTERRUPT << 30) | \
					 create_pipe(dev, endpoint))
#define usb_rcvintpipe(dev, endpoint)	((PIPE_INTERRUPT << 30) | \
					 create_pipe(dev, endpoint) | \
					 USB_DIR_IN)
#define usb_snddefctrl(dev)		((PIPE_CONTROL << 30) | \
					 default_pipe(dev))
#define usb_rcvdefctrl(dev)		((PIPE_CONTROL << 30) | \
					 default_pipe(dev) | \
					 USB_DIR_IN)

/* The D0/D1 toggle bits */
#define usb_gettoggle(dev, ep, out) (((dev)->toggle[out] >> ep) & 1)
#define usb_dotoggle(dev, ep, out)  ((dev)->toggle[out] ^= (1 << ep))
#define usb_settoggle(dev, ep, out, bit) ((dev)->toggle[out] = \
						((dev)->toggle[out] & \
						 ~(1 << ep)) | ((bit) << ep))

/* Endpoint halt control/status */
#define usb_endpoint_out(ep_dir)	(((ep_dir >> 7) & 1) ^ 1)
#define usb_endpoint_halt(dev, ep, out) ((dev)->halted[out] |= (1 << (ep)))
#define usb_endpoint_running(dev, ep, out) ((dev)->halted[out] &= ~(1 << (ep)))
#define usb_endpoint_halted(dev, ep, out) ((dev)->halted[out] & (1 << (ep)))

#define usb_packetid(pipe)	(((pipe) & USB_DIR_IN) ? USB_PID_IN : \
				 USB_PID_OUT)

#define usb_pipeout(pipe)	((((pipe) >> 7) & 1) ^ 1)
#define usb_pipein(pipe)	(((pipe) >> 7) & 1)
#define usb_pipedevice(pipe)	(((pipe) >> 8) & 0x7f)
#define usb_pipe_endpdev(pipe)	(((pipe) >> 8) & 0x7ff)
#define usb_pipeendpoint(pipe)	(((pipe) >> 15) & 0xf)
#define usb_pipedata(pipe)	(((pipe) >> 19) & 1)
#define usb_pipespeed(pipe)	(((pipe) >> 26) & 3)
#define usb_pipeslow(pipe)	(usb_pipespeed(pipe) == USB_SPEED_LOW)
#define usb_pipetype(pipe)	(((pipe) >> 30) & 3)
#define usb_pipeisoc(pipe)	(usb_pipetype((pipe)) == PIPE_ISOCHRONOUS)
#define usb_pipeint(pipe)	(usb_pipetype((pipe)) == PIPE_INTERRUPT)
#define usb_pipecontrol(pipe)	(usb_pipetype((pipe)) == PIPE_CONTROL)
#define usb_pipebulk(pipe)	(usb_pipetype((pipe)) == PIPE_BULK)


/*************************************************************************
 * Hub Stuff
 */
struct usb_port_status {
	uint16_t wPortStatus;
	uint16_t wPortChange;
} __attribute__ ((packed));

struct usb_hub_status {
	uint16_t wHubStatus;
	uint16_t wHubChange;
} __attribute__ ((packed));


/* Hub descriptor */
struct usb_hub_descriptor {
	uint8_t  bLength;
	uint8_t  bDescriptorType;
	uint8_t  bNbrPorts;
	uint16_t wHubCharacteristics;
	uint8_t  bPwrOn2PwrGood;
	uint8_t  bHubContrCurrent;
	uint8_t  DeviceRemovable[(USB_MAXCHILDREN+1+7)/8];
	uint8_t  PortPowerCtrlMask[(USB_MAXCHILDREN+1+7)/8];
	/* DeviceRemovable and PortPwrCtrlMask want to be variable-length
	   bitmaps that hold max 255 entries. (bit0 is ignored) */
} __attribute__ ((packed));


struct usb_hub_device {
	struct usb_device *pusb_dev;
	struct usb_hub_descriptor desc;
};

#endif /*_USB_H_ */
