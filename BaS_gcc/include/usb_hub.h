#ifndef USB_HUB_H
#define USB_HUB_H

extern int bus_index;

extern void usb_hub_reset(int bus_index);
extern int usb_hub_probe(struct usb_device *dev, int ifnum);
extern int hub_port_reset(struct usb_device *dev, int port, unsigned short *portstat);

#endif // USB_HUB_H
