/*
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 */

#include <stdint.h>
#include "bas_printf.h"
#include "usb.h"
#include "exceptions.h"
#include "driver_mem.h"

#define DEBUG_USBMOUSE
#ifdef DEBUG_USBMOUSE
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DEBUG_USBMOUSE */

extern void ltoa(char *buf, long n, unsigned long base);
extern void call_mousevec(unsigned char *data, void (**mousevec)(void *));
//extern void call_ikbdvec(unsigned char code, _IOREC *iorec, void (**ikbdvec)());

static unsigned char *new;
static unsigned char old[8];
static int mouse_installed;

/* forward declaration */
static int usb_mouse_probe(struct usb_device *dev, unsigned int ifnum);

/* deregistering the mouse */
int usb_mouse_deregister(struct usb_device *dev)
{
	dev->irq_handle = NULL;
	if (new != NULL)
	{
		driver_mem_free(new);
		new = NULL;
	}
	mouse_installed = 0;
	dbg("USB MOUSE deregister\r\n");
	return 1;
}

/* registering the mouse */
int usb_mouse_register(struct usb_device *dev)
{
	if (!mouse_installed && (dev->devnum != -1) && (usb_mouse_probe(dev, 0) == 1))
	{
		/* Ok, we found a mouse */
		dbg("USB MOUSE found (USB: %d, devnum: %d)\r\n", dev->usbnum, dev->devnum);
		mouse_installed = 1;
		dev->deregister = usb_mouse_deregister;
		return 1;
	}
	/* no USB Mouse found */
	return -1;
}

/* search for mouse and register it if found */
int drv_usb_mouse_init(void)
{
	int i;
	int j;

	/*
	 * check if mouse is already initialized
	 */
	if (mouse_installed)
	{
		xprintf("USB mouse already initialized\r\n");

		return -1;
	}

	/* scan all USB Devices */
	for (j = 0; j < USB_MAX_BUS; j++)
	{
		for (i = 0; i < USB_MAX_DEVICE; i++)
		{
			struct usb_device *dev = usb_get_dev_index(i, j); /* get device */

			if (dev == NULL)
				break;

			xprintf("Try to register usb device %d,%d as mouse\r\n", i, j);
			if (usb_mouse_register(dev) > 0)
				return 1;
		}
	}
	/* no USB Mouse found */
	return -1;
}

/**************************************************************************
 * Low Level drivers
 */
static void usb_kbd_send_code(unsigned char code)
{
	dbg("FIXME: usb_kbd_send_code 0x%x not implemented\r\n", code);
}

/* Interrupt service routine */
static int usb_mouse_irq(struct usb_device *dev)
{
#ifdef CONFIG_USB_INTERRUPT_POLLING
	int level;
#endif
	int i, change = 0;
	if ((dev->irq_status != 0) || (dev->irq_act_len < 3) || (dev->irq_act_len > 8))
	{
		dbg("USB MOUSE error %lX, len %d\r\n", dev->irq_status, dev->irq_act_len);
		return 1;
	}
	for (i = 0; i < dev->irq_act_len; i++)
	{
		if (new[i] != old[i])
		{
			change = 1;
			break;
		}
	}
	if (change)
	{
		char wheel = 0, buttons, old_buttons;
		dbg("USB MOUSE len:%d %02X %02X %02X %02X %02X %02X\r\n", dev->irq_act_len, new[0], new[1], new[2], new[3], new[4], new[5]);
#ifdef CONFIG_USB_INTERRUPT_POLLING
		level = set_ipl(7); /* mask interrupts */
#endif
		if ((dev->irq_act_len >= 6) && (new[0] == 1)) /* report-ID */
		{
			buttons = new[1];
			old_buttons = old[1];
			new[0] = ((new[1] & 1) << 1) + ((new[1] & 2) >> 1) + 0xF8;
			new[1] = new[2];
			new[2] = new[3];
			wheel = new[4];
		}
		else /* boot report */
		{
			buttons = new[0];
			old_buttons = old[0];
			new[0] = ((new[0] & 1) << 1) + ((new[0] & 2) >> 1) + 0xF8;
			if (dev->irq_act_len >= 3)
				wheel = new[3];
		}
		if ((buttons ^ old_buttons) & 4) /* 3rd button */
		{
		  if (buttons & 4)
		  {
				usb_kbd_send_code(0x72); /* ENTER */
				usb_kbd_send_code(0xF2);
			}
		}
		if (wheel != 0) /* actually like Eiffel */
		{
#define REPEAT_WHEEL 3
			int i;
			if (wheel > 0)
			{
				for (i = 0; i < REPEAT_WHEEL; i++)
				{
					usb_kbd_send_code(0x48); /* UP */
					usb_kbd_send_code(0xC8);
				}
			}
			else
			{
				for (i = 0; i < REPEAT_WHEEL; i++)
				{
					usb_kbd_send_code(0x50); /* DOWN */
					usb_kbd_send_code(0xD0);
				}
			}
		}
		xprintf("FIXME: call_mousevec(new, mousevec) not implemented\r\n");
		//if(mousevec != NULL)
			//call_mousevec(new, mousevec);
#ifdef CONFIG_USB_INTERRUPT_POLLING
		set_ipl(level);
#endif
		old[0] = new[0];
		old[1] = new[1];
		old[2] = new[2];
		old[3] = new[3];
		old[4] = new[4];
		old[5] = new[5];
	}
	return 1; /* install IRQ Handler again */
}

/* probes the USB device dev for mouse type */
static int usb_mouse_probe(struct usb_device *dev, unsigned int ifnum)
{
	struct usb_interface_descriptor *iface;
	struct usb_endpoint_descriptor *ep;
	int pipe;
	int maxp;

	if (dev->descriptor.bNumConfigurations != 1)
	{
		dbg("dev->descriptor.bNumConfigurations != 1\r\n");

		return 0;
	}

	iface = &dev->config.if_desc[ifnum];

	if (iface->bInterfaceClass != USB_CLASS_HID)
	{
		dbg("iface->bInterfaceClass != USB_CLASS_HID (%d instead)\r\n", iface->bInterfaceClass);

		return 0;
	}


	if (iface->bInterfaceSubClass != USB_SUB_HID_BOOT)
	{
		dbg("iface->bInterfaceSubClass != USB_SUB_HID_BOOT (%d instead)\r\n", iface->bInterfaceSubClass);

		return 0;
	}

	if (iface->bInterfaceProtocol != USB_PROT_HID_MOUSE)
	{
		dbg("iface->bInterfaceProtocol != USB_PROT_HID_MOUSE (%d)\r\n", iface->bInterfaceProtocol);

		return 0;
	}

	if (iface->bNumEndpoints != 1)
	{
		dbg("iface->bNumEndpoints != 1\r\n");

		return 0;
	}

	ep = &iface->ep_desc[0];

	if (!(ep->bEndpointAddress & 0x80))
	{
		dbg("! ep->bEndpointAddress & 0x80\r\n");

		return 0;
	}

	if ((ep->bmAttributes & 3) != 3)
	{
		dbg("ep->bmAttributes & 3 != 3\r\n");

		return 0;
	}

	new = (unsigned char *) driver_mem_alloc(8);
	if (new == NULL)
	{
		dbg("new == NULL\r\n");

		return 0;
	}

	dbg("USB MOUSE found set protocol...\r\n");

	/* ok, we found a USB Mouse, install it */
	pipe = usb_rcvintpipe(dev, ep->bEndpointAddress);
	maxp = usb_maxpacket(dev, pipe);
//	if(maxp < 6)
//		usb_set_protocol(dev, iface->bInterfaceNumber, 0); /* boot */
//	else
	usb_set_protocol(dev, iface->bInterfaceNumber, 1); /* report */
	dbg("USB MOUSE found set idle...\r\n");
	usb_set_idle(dev, iface->bInterfaceNumber, 0, 0); /* report infinite */
	memset(&new[0], 0, 8);
	memset(&old[0], 0, 8);
	dev->irq_handle = usb_mouse_irq;
	dbg("USB MOUSE enable interrupt pipe (maxp: %d)...\r\n", maxp);
	usb_submit_int_msg(dev, pipe, &new[0], maxp > 8 ? 8 : maxp, ep->bInterval);
	return 1;
}

