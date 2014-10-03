/*
 *
 * Most of this source has been derived from the Linux USB
 * project:
 * (C) Copyright Linus Torvalds 1999
 * (C) Copyright Johannes Erdfelt 1999-2001
 * (C) Copyright Andreas Gal 1999
 * (C) Copyright Gregory P. Smith 1999
 * (C) Copyright Deti Fliegl 1999 (new USB architecture)
 * (C) Copyright Randy Dunlap 2000
 * (C) Copyright David Brownell 2000 (kernel hotplug, usb_device_id)
 * (C) Copyright Yggdrasil Computing, Inc. 2000
 *     (usb_device_id matching changes by Adam J. Richter)
 *
 * Adapted for U-Boot:
 * (C) Copyright 2001 Denis Peter, MPL AG Switzerland
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

/*
 * How it works:
 *
 * Since this is a bootloader, the devices will not be automatic
 * (re)configured on hotplug, but after a restart of the USB the
 * device should work.
 *
 * For each transfer (except "Interrupt") we wait for completion.
 */

#include <stdint.h>
#include "bas_string.h"
#include "bas_printf.h"
#include "util.h"				/* for byte swap funcs */
#include "wait.h"
#include <stdarg.h>
#include "usb.h"
#include "usb_hub.h"

#define DEBUG_USB
#ifdef DEBUG_USB
#define dbg(format, arg...) do { xprintf("DEBUG: %s(): " format, __FUNCTION__, ##arg); } while (0)
#else
#define dbg(format, arg...) do { ; } while (0)
#endif /* DEBUG_USB */
#define err(format, arg...) do { xprintf("ERROR: %s(): " format, __FUNCTION__, ##arg); } while (0)
#define info(format, arg...) do { xprintf("INFO: %s(): " format, __FUNCTION__, ##arg); } while (0)

struct hci
{
    /* ------- common part -------- */
    long handle;						/* PCI BIOS */
    const struct pci_device_id *ent;
    int usbnum;
  /* ---- end of common part ---- */
};

static struct usb_device *usb_dev;
int bus_index;
static int dev_index[USB_MAX_BUS];
static struct hci *controller_priv[USB_MAX_BUS];
static int asynch_allowed;
static struct devrequest *setup_packet;

char usb_started; /* flag for the started/stopped USB status */


/*
 * some forward declarations...
 */
void usb_scan_devices(void *priv);

/*
 * Init USB device controller
 */
int usb_init(int32_t handle, const struct pci_device_id *ent)
{
    struct hci *priv;
    int res = 0;

    bus_index = 0;
    if (bus_index >= USB_MAX_BUS)
    {
        dbg("bus_index >= USB_MAX_BUS");
        return -1;
    }

    dev_index[bus_index] = 0;
    asynch_allowed = 1;

    if (handle && (ent != NULL))
    {
        if (driver_mem_init())
        {
            usb_started = 0;
            dbg("driver_mem_init failed\r\n");

            return -1; /* out of memory */
        }

        if (usb_dev == NULL)
        {
            usb_dev = (struct usb_device *) driver_mem_alloc(sizeof(struct usb_device) * USB_MAX_BUS * USB_MAX_DEVICE);
        }

        if (usb_dev == NULL)
        {
            usb_started = 0;

            dbg("could not allocate memory\r\n");

            return -1; /* out of memory */
        }
    }
    else /* restart */
    {
        int i;

        res = 0;
        for (i = 0; i < USB_MAX_BUS; i++)
        {
            if (controller_priv[i] != NULL)
            {
                long handle = controller_priv[i]->handle;

                if (handle)
                {
                    res |= usb_init(handle, NULL);	/* FIXME: recursive call!? */
                }
            }
        }
        return res;
    }

    usb_hub_reset(bus_index);

    /* init low_level USB */

    switch(ent->class)
    {
        case PCI_CLASS_SERIAL_USB_UHCI:
            //res = uhci_usb_lowlevel_init(handle, ent, &priv);
            dbg("sorry, no uhci driver available\r\n");
            break;

        case PCI_CLASS_SERIAL_USB_OHCI:
            dbg("initialize ohci host controller interface\r\n");
            res = ohci_usb_lowlevel_init(handle, ent, (void *) &priv);
            break;

        case PCI_CLASS_SERIAL_USB_EHCI:
            dbg("initialize ehci host controller interface\r\n");
            res = ehci_usb_lowlevel_init(handle, ent, (void *) &priv);
            break;

        default:
            res = -1;
            break;
    }

    if (!res)
    {
        /*
         * if lowlevel init is OK, scan the bus for devices
         * i.e. search HUBs and configure them
         */
        if (setup_packet == NULL)
        {
            setup_packet = driver_mem_alloc(sizeof(struct devrequest));
            if (setup_packet == NULL)
            {
                usb_started = 0;

                dbg("could not allocate memory\r\n");

                return -1;			/* no memory, no USB */
            }
        }

        xprintf("Scanning bus for devices... ");

        controller_priv[bus_index] = priv;
        controller_priv[bus_index]->usbnum = bus_index;

        usb_scan_devices(priv);
        bus_index++;
        usb_started = 1;

        return 0;
    }
    else
    {
        xprintf("\r\nError, couldn't init Lowlevel part\r\n");
        usb_started = 0;

        return -1;
    }
}

/*
 * Stop USB. This stops the LowLevel Part and deregisters USB devices.
 */
int usb_stop(void)
{
    int i;
    int res = 0;

    if (usb_started)
    {
        asynch_allowed = 1;
        usb_started = 0;
        usb_hub_reset(bus_index);
        driver_mem_free(setup_packet);
        for (i = 0; i < USB_MAX_BUS; i++)
        {
            struct hci *priv = controller_priv[i];
            if (priv != NULL)
            {
                switch(priv->ent->class)
                {
#ifdef CONFIG_USB_UHCI
                    case PCI_CLASS_SERIAL_USB_UHCI:
                        res |= uhci_usb_lowlevel_stop(priv);
                        break;
#endif
#ifdef CONFIG_USB_OHCI
                    case PCI_CLASS_SERIAL_USB_OHCI:
                        res |= ohci_usb_lowlevel_stop(priv);
                        break;
#endif
#ifdef CONFIG_USB_EHCI
                    case PCI_CLASS_SERIAL_USB_EHCI:
                        res |= ehci_usb_lowlevel_stop(priv);
                        break;
#endif
                }
            }
        }
        bus_index = 0;
        driver_mem_release();	/* release all driver mem */
    }
    return res;
}


void usb_enable_interrupt(int enable)
{
    ohci_usb_enable_interrupt(enable);
    ehci_usb_enable_interrupt(enable);
}


/*
 * disables the asynch behaviour of the control message. This is used for data
 * transfers that uses the exclusiv access to the control and bulk messages.
 */
void usb_disable_asynch(int disable)
{
    asynch_allowed = !disable;
}

/*
 * Message wrappers.
 *
 */

/*
 * submits an Interrupt Message
 */
int usb_submit_int_msg(struct usb_device *dev, uint32_t pipe, void *buffer, int transfer_len, int interval)
{
    struct hci *priv = (struct hci *) dev->priv_hcd;
    int ret = 0;

    switch(priv->ent->class)
    {
        case PCI_CLASS_SERIAL_USB_OHCI:
            ret = ohci_submit_int_msg(dev, pipe, buffer, transfer_len, interval);
            break;

        case PCI_CLASS_SERIAL_USB_EHCI:
            ret = ehci_submit_int_msg(dev, pipe, buffer, transfer_len, interval);
            break;

        default:
            ret = -1;
            break;
    }
    return ret;
}

/*
 * submits a control message and waits for completion (at least timeout * 1ms)
 * If timeout is 0, we don't wait for completion (used for example to set and
 * clear keyboards LEDs). For data transfers, (storage transfers) we don't
 * allow control messages with 0 timeout, by previousely resetting the flag
 * asynch_allowed (usb_disable_asynch(1)).
 * returns the transfered length if OK or -1 if error. The transfered length
 * and the current status are stored in the dev->act_len and dev->status.
 */
int usb_control_msg(struct usb_device *dev, unsigned int pipe,
                    unsigned char request, unsigned char requesttype,
                    unsigned short value, unsigned short index,
                    void *data, unsigned short size, int timeout)
{
    struct hci *priv = (struct hci *) dev->priv_hcd;

    if ((timeout == 0) && (!asynch_allowed))
    {
        /* request for a asynch control pipe is not allowed */

        dbg("request for an async control pipe is not allowed\r\n");
        return -1;
    }

    /* set setup command */
    setup_packet->requesttype = requesttype;
    setup_packet->request = request;
    setup_packet->value = swpw(value);
    setup_packet->index = swpw(index);
    setup_packet->length = swpw(size);

    dbg("usb_control_msg: request: 0x%X, requesttype: 0x%X, value 0x%X index 0x%X length 0x%X\r\n", request, requesttype, value, index, size);

    switch(priv->ent->class)
    {
        case PCI_CLASS_SERIAL_USB_OHCI:
            dev->status = USB_ST_NOT_PROC; /* not yet processed */
            ohci_submit_control_msg(dev, pipe, data, size, setup_packet);
            break;

        case PCI_CLASS_SERIAL_USB_EHCI:
            dev->status = USB_ST_NOT_PROC; /* not yet processed */
            ehci_submit_control_msg(dev, pipe, data, size, setup_packet);
            break;

        default:
            return -1;
    }

    if (timeout == 0)
    {
        return (int) size;
    }

    if (dev->status != 0)
    {
        /*
         * Let's wait a while for the timeout to elapse.
         * It has no real use, but it keeps the interface happy.
         */
        return -1;
    }
    return dev->act_len;
}

/*
 * submits bulk message, and waits for completion. returns 0 if Ok or
 * -1 if Error.
 * synchronous behavior
 */
int usb_bulk_msg(struct usb_device *dev, unsigned int pipe, void *data, int len, int *actual_length, int timeout)
{
    struct hci *priv = (struct hci *) dev->priv_hcd;

    if (len < 0)
    {
        return -1;
    }

    switch(priv->ent->class)
    {
        case PCI_CLASS_SERIAL_USB_OHCI:
            dev->status = USB_ST_NOT_PROC; /* not yet processed */
            ohci_submit_bulk_msg(dev, pipe, data, len);
            break;

        case PCI_CLASS_SERIAL_USB_EHCI:
            dev->status = USB_ST_NOT_PROC; /* not yet processed */
            ehci_submit_bulk_msg(dev, pipe, data, len);
            break;

        default:
            return -1;
    }

    while (timeout--)
    {
        if (!((volatile uint32_t) dev->status & USB_ST_NOT_PROC))	/* FIXME: this volatile does nothing! */
            break;
        wait(1);
    }

    *actual_length = dev->act_len;

    if (dev->status == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}


/*
 * Max Packet stuff
 */

/*
 * returns the max packet size, depending on the pipe direction and
 * the configurations values
 */
int usb_maxpacket(struct usb_device *dev, uint32_t pipe)
{
    /* direction is out -> use emaxpacket out */
    if ((pipe & USB_DIR_IN) == 0)
    {
        return dev->epmaxpacketout[((pipe >> 15) & 0xf)];
    }
    else
    {
        return dev->epmaxpacketin[((pipe >> 15) & 0xf)];
    }
}

/*
 * The routine usb_set_maxpacket_ep() is extracted from the loop of routine
 * usb_set_maxpacket(), because the optimizer of GCC 4.x chokes on this routine
 * when it is inlined in 1 single routine. What happens is that the register r3
 * is used as loop-count 'i', but gets overwritten later on.
 * This is clearly a compiler bug, but it is easier to workaround it here than
 * to update the compiler (Occurs with at least several GCC 4.{1,2},x
 * CodeSourcery compilers like e.g. 2007q3, 2008q1, 2008q3 lite editions on ARM)
 *
 * We probably do not need that for Coldfire - at least I hope so.
 */
static void usb_set_maxpacket_ep(struct usb_device *dev, struct usb_endpoint_descriptor *ep)
{
    int b;

    b = ep->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;

    if ((ep->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_CONTROL)
    {
        /* Control => bidirectional */
        dev->epmaxpacketout[b] = ep->wMaxPacketSize;
        dev->epmaxpacketin[b] = ep->wMaxPacketSize;
        dbg("##Control EP epmaxpacketout/in[%d] = %d\r\n", b, dev->epmaxpacketin[b]);
    }
    else
    {
        if ((ep->bEndpointAddress & 0x80) == 0)
        {
            /* OUT Endpoint */
            if (ep->wMaxPacketSize > dev->epmaxpacketout[b])
            {
                dev->epmaxpacketout[b] = ep->wMaxPacketSize;
                dbg("##EP epmaxpacketout[%d] = %d\r\n", b, dev->epmaxpacketout[b]);
            }
        }
        else
        {
            /* IN Endpoint */
            if (ep->wMaxPacketSize > dev->epmaxpacketin[b])
            {
                dev->epmaxpacketin[b] = ep->wMaxPacketSize;
                dbg("##EP epmaxpacketin[%d] = %d\r\n", b, dev->epmaxpacketin[b]);
            }
        } /* if out */
    } /* if control */
}

/*
 * set the max packed value of all endpoints in the given configuration
 */
int usb_set_maxpacket(struct usb_device *dev)
{
    int i;
    int ii;

    for (i = 0; i < dev->config.bNumInterfaces; i++)
    {
        for (ii = 0; ii < dev->config.if_desc[i].bNumEndpoints; ii++)
        {
            usb_set_maxpacket_ep(dev,&dev->config.if_desc[i].ep_desc[ii]);
        }
    }
    return 0;
}

/*
 * Parse the config, located in buffer, and fills the dev->config structure.
 * Note that all little/big endian swapping are done automatically.
 */
int usb_parse_config(struct usb_device *dev, unsigned char *buffer, int cfgno)
{
    struct usb_descriptor_header *head;
    int index;
    int ifno;
    int epno;
    int curr_if_num;

    ifno = -1;
    epno = -1;
    curr_if_num = -1;
    dev->configno = cfgno;
    head = (struct usb_descriptor_header *) &buffer[0];

    if (head->bDescriptorType != USB_DT_CONFIG)
    {
        dbg(" ERROR: NOT USB_CONFIG_DESC %x\r\n", head->bDescriptorType);
        return -1;
    }
    memcpy(&dev->config, buffer, buffer[0]);
    dev->config.wTotalLength = swpw(dev->config.wTotalLength);
    dev->config.no_of_if = 0;
    index = dev->config.bLength;

    /*
     * Ok the first entry must be a configuration entry,
     * now process the others
     */
    head = (struct usb_descriptor_header *) &buffer[index];

    while (index + 1 < dev->config.wTotalLength)
    {
        switch (head->bDescriptorType)
        {
            case USB_DT_INTERFACE:
                if (((struct usb_interface_descriptor *) &buffer[index])->bInterfaceNumber != curr_if_num)
                {
                    /* this is a new interface, copy new desc */
                    ifno = dev->config.no_of_if;
                    dev->config.no_of_if++;
                    memcpy(&dev->config.if_desc[ifno], &buffer[index], buffer[index]);
                    dev->config.if_desc[ifno].no_of_ep = 0;
                    dev->config.if_desc[ifno].num_altsetting = 1;
                    curr_if_num = dev->config.if_desc[ifno].bInterfaceNumber;
                }
                else
                {
                    /* found alternate setting for the interface */
                    dev->config.if_desc[ifno].num_altsetting++;
                }
                break;

            case USB_DT_ENDPOINT:
                epno = dev->config.if_desc[ifno].no_of_ep;
                /* found an endpoint */
                dev->config.if_desc[ifno].no_of_ep++;
                memcpy(&dev->config.if_desc[ifno].ep_desc[epno], &buffer[index], buffer[index]);
                dev->config.if_desc[ifno].ep_desc[epno].wMaxPacketSize = swpw(dev->config.if_desc[ifno].ep_desc[epno].wMaxPacketSize);
                dbg("if %d, ep %d\r\n", ifno, epno);
                break;

            default:
                if (head->bLength == 0)
                    return 1;
                dbg("unknown Descriptor Type : %x\r\n", head->bDescriptorType);
#ifdef USB_DEBUG
                {
                    unsigned char *ch;
                    int i;

                    ch = (unsigned char *) head;
                    for (i = 0; i < head->bLength; i++)
                    {
                        dbg(" %02X", *ch++);
                    }
                    dbg("\r\n");
                }
#endif /* USB_DEBUG */
                break;
        }
        index += head->bLength;
        head = (struct usb_descriptor_header *) &buffer[index];
    }
    return 1;
}

/*
 * Clears an endpoint
 * endp: endpoint number in bits 0-3;
 * direction flag in bit 7 (1 = IN, 0 = OUT)
 */
int usb_clear_halt(struct usb_device *dev, int pipe)
{
    int result;
    int endp = usb_pipeendpoint(pipe) | (usb_pipein(pipe) << 7);

    result = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
                 USB_REQ_CLEAR_FEATURE, USB_RECIP_ENDPOINT, 0, endp, NULL, 0, USB_CNTL_TIMEOUT * 3);

    /* don't clear if failed */
    if (result < 0)
    {
        return result;
    }

    /*
     * NOTE: we do not get status and verify reset was successful
     * as some devices are reported to lock up upon this check..
     */
    usb_endpoint_running(dev, usb_pipeendpoint(pipe), usb_pipeout(pipe));

    /* toggle is reset on clear */
    usb_settoggle(dev, usb_pipeendpoint(pipe), usb_pipeout(pipe), 0);

    return 0;
}

/*
 * get_descriptor type
 */
int usb_get_descriptor(struct usb_device *dev, unsigned char type, unsigned char index, void *buf, int size)
{
    int res;

    res = usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
            USB_REQ_GET_DESCRIPTOR, USB_DIR_IN, (type << 8) + index, 0, buf, size, USB_CNTL_TIMEOUT);
    return res;
}

/*
 * gets configuration cfgno and store it in the buffer
 */
int usb_get_configuration_no(struct usb_device *dev, unsigned char *buffer, int cfgno)
{
    int result;
    unsigned int tmp;
    struct usb_config_descriptor *config;

    config = (struct usb_config_descriptor *) &buffer[0];
    result = usb_get_descriptor(dev, USB_DT_CONFIG, cfgno, buffer, 9);

    if (result < 9)
    {
        if (result < 0)
        {
            dbg("unable to get descriptor, error %lX\r\n", dev->status);
        }
        else
        {
            dbg("config descriptor too short (expected %i, got %i)\n", 9, result);
        }
        return -1;
    }

    tmp = swpw(config->wTotalLength);

    if (tmp > USB_BUFSIZ)
    {
        dbg("usb_get_configuration_no: failed to get descriptor - too long: %d\r\n", tmp);
        return -1;
    }
    result = usb_get_descriptor(dev, USB_DT_CONFIG, cfgno, buffer, tmp);
    dbg("get_conf_no %d Result %d, wLength %d\r\n", cfgno, result, tmp);

    return result;
}

/*
 * set address of a device to the value in dev->devnum.
 * This can only be done by addressing the device via the default address (0)
 */
int usb_set_address(struct usb_device *dev)
{
    int res;

    dbg("set address %d\r\n", dev->devnum);
    res = usb_control_msg(dev, usb_snddefctrl(dev), USB_REQ_SET_ADDRESS, 0, (dev->devnum), 0, NULL, 0, USB_CNTL_TIMEOUT);
    return res;
}

/*
 * set interface number to interface
 */
int usb_set_interface(struct usb_device *dev, int interface, int alternate)
{
    struct usb_interface_descriptor *if_face = NULL;
    int ret, i;

    for (i = 0; i < dev->config.bNumInterfaces; i++)
    {
        if (dev->config.if_desc[i].bInterfaceNumber == interface)
        {
            if_face = &dev->config.if_desc[i];

            break;
        }
    }

    if (!if_face)
    {
        dbg("selecting invalid interface %d", interface);

        return -1;
    }

    /*
     * We should return now for devices with only one alternate setting.
     * According to 9.4.10 of the Universal Serial Bus Specification
     * Revision 2.0 such devices can return with a STALL. This results in
     * some USB sticks timeouting during initialization and then being
     * unusable in U-Boot.
     */
    if (if_face->num_altsetting == 1)
    {
        return 0;
    }

    ret = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
                USB_REQ_SET_INTERFACE, USB_RECIP_INTERFACE, alternate, interface, NULL, 0, USB_CNTL_TIMEOUT * 5);
    if (ret < 0)
    {
        return ret;
    }

    return 0;
}

/*
 * set configuration number to configuration
 */
int usb_set_configuration(struct usb_device *dev, int configuration)
{
    int res;

    dbg("set configuration %d\r\n", configuration);
    /* set setup command */
    res = usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
                USB_REQ_SET_CONFIGURATION, 0, configuration, 0, NULL, 0, USB_CNTL_TIMEOUT);
    if (res == 0)
    {
        dev->toggle[0] = 0;
        dev->toggle[1] = 0;

        return 0;
    }
    else
    {
        return -1;
    }
}

/*
 * set protocol to protocol
 */
int usb_set_protocol(struct usb_device *dev, int ifnum, int protocol)
{
    return usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
                           USB_REQ_SET_PROTOCOL, USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                           protocol, ifnum, NULL, 0, USB_CNTL_TIMEOUT);
}

/*
 * set idle
 */
int usb_set_idle(struct usb_device *dev, int ifnum, int duration, int report_id)
{
    return usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
                           USB_REQ_SET_IDLE, USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                           (duration << 8) | report_id, ifnum, NULL, 0, USB_CNTL_TIMEOUT);
}

/*
 * get report
 */
int usb_get_report(struct usb_device *dev, int ifnum, unsigned char type,
           unsigned char id, void *buf, int size)
{
    return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
                           USB_REQ_GET_REPORT, USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE,
                           (type << 8) + id, ifnum, buf, size, USB_CNTL_TIMEOUT);
}

/*
 * get class descriptor
 */
int usb_get_class_descriptor(struct usb_device *dev, int ifnum,
        unsigned char type, unsigned char id, void *buf, int size)
{
    return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
                           USB_REQ_GET_DESCRIPTOR, USB_RECIP_INTERFACE | USB_DIR_IN,
                           (type << 8) + id, ifnum, buf, size, USB_CNTL_TIMEOUT);
}

/*
 * get string index in buffer
 */
int usb_get_string(struct usb_device *dev, unsigned short langid, unsigned char index, void *buf, int size)
{
    int i;
    int result;

    for (i = 0; i < 3; ++i)
    {
        /* some devices are flaky */
        result = usb_control_msg(dev, usb_rcvctrlpipe(dev, 0), USB_REQ_GET_DESCRIPTOR, USB_DIR_IN,
                                 (USB_DT_STRING << 8) + index, langid, buf, size, USB_CNTL_TIMEOUT);
        if (result > 0)
        {
            break;
        }
    }
    return result;
}

static void usb_try_string_workarounds(unsigned char *buf, int *length)
{
    int newlength;
    int oldlength = *length;

    for (newlength = 2; newlength + 1 < oldlength; newlength += 2)
    {
        char c = buf[newlength];

        if ((c < ' ') || (c >= 127) || buf[newlength + 1])
            break;
    }

    if (newlength > 2)
    {
        buf[0] = newlength;
        *length = newlength;
    }
}

static int usb_string_sub(struct usb_device *dev, unsigned int langid, unsigned int index, unsigned char *buf)
{
    int rc;

    /*
     * Try to read the string descriptor by asking for the maximum
     * possible number of bytes
     */
    rc = usb_get_string(dev, langid, index, buf, 255);

    /*
     * If that failed try to read the descriptor length, then
     * ask for just that many bytes
     */
    if (rc < 2)
    {
        rc = usb_get_string(dev, langid, index, buf, 2);
        if (rc == 2)
        {
            rc = usb_get_string(dev, langid, index, buf, buf[0]);
        }
    }

    if (rc >= 2)
    {
        if (!buf[0] && !buf[1])
        {
            usb_try_string_workarounds(buf, &rc);
        }

        /* There might be extra junk at the end of the descriptor */
        if (buf[0] < rc)
        {
            rc = buf[0];
        }
        rc = rc - (rc & 1); /* force a multiple of two */
    }

    if (rc < 2)
    {
        rc = -1;
    }

    return rc;
}

/*
 * usb_string:
 * Get string index and translate it to ascii.
 * returns string length (> 0) or error (< 0)
 */
int usb_string(struct usb_device *dev, int index, char *buf, size_t size)
{
    unsigned char *tbuf;
    int err;
    unsigned int u, idx;

    if (size <= 0 || !buf || !index)
    {
        return -1;
    }

    buf[0] = 0;
    tbuf = (unsigned char *) driver_mem_alloc(USB_BUFSIZ);

    if (tbuf == NULL)
    {
        dbg("usb_string: malloc failure\r\n");
        return -1;
    }

    /* get langid for strings if it's not yet known */
    if (!dev->have_langid)
    {
        err = usb_string_sub(dev, 0, 0, tbuf);
        if (err < 0)
        {
            dbg("error getting string descriptor 0 (error=%lx)\r\n", dev->status);
            driver_mem_free(tbuf);
            return -1;
        }
        else if (tbuf[0] < 4)
        {
            dbg("string descriptor 0 too short\r\n");
            driver_mem_free(tbuf);
            return -1;
        }
        else
        {
            dev->have_langid = -1;
            dev->string_langid = tbuf[2] | (tbuf[3] << 8);
                /* always use the first langid listed */
            dbg("USB device number %d default language ID 0x%x\r\n", dev->devnum, dev->string_langid);
        }
    }
    err = usb_string_sub(dev, dev->string_langid, index, tbuf);
    if (err < 0)
    {
        driver_mem_free(tbuf);
        return err;
    }

    size--;		/* leave room for trailing NULL char in output buffer */
    for (idx = 0, u = 2; u < err; u += 2)
    {
        if (idx >= size)
        {
            break;
        }
        if (tbuf[u + 1])		/* high byte */
        {
            buf[idx++] = '?';	/* non-ASCII character */
        }
        else
        {
            buf[idx++] = tbuf[u];
        }
    }
    buf[idx] = 0;
    err = idx;
    driver_mem_free(tbuf);
    return err;
}

/*
 * USB device handling:
 * the USB device are static allocated [USB_MAX_DEVICE].
 */

/*
 * Something got disconnected. Get rid of it, and all of its children.
 */
void usb_disconnect(struct usb_device **pdev)
{
    struct usb_device *dev = *pdev;
    if (dev != NULL)
    {
        int i;

        dbg("USB %d disconnect on device %d\r\n", dev->parent->usbnum, dev->parent->devnum);
        dbg("USB %d disconnected, device number %d\r\n", dev->usbnum, dev->devnum);

        if (dev->deregister != NULL)
        {
            dev->deregister(dev);
        }

        /* Free up all the children.. */
        for (i = 0; i < USB_MAXCHILDREN; i++)
        {
            if (dev->children[i] != NULL)
            {
                dbg("USB %d, disconnect children %d\r\n", dev->usbnum, dev->children[i]->devnum);
                usb_disconnect(&dev->children[i]);
                dev->children[i] = NULL;
            }
        }

        /* Free up the device itself, including its device number */
        if (dev->devnum > 0)
        {
            dev_index[dev->usbnum]--;
            memset(dev, 0, sizeof(struct usb_device));
            dev->devnum = -1;
        }
        *pdev = NULL;
    }
}

/*
 * returns a pointer to the device with the index [index].
 * if the device is not assigned (dev->devnum==-1) returns NULL
 */
struct usb_device *usb_get_dev_index(int index, int index_bus)
{
    struct usb_device *dev;

    if ((index_bus >= USB_MAX_BUS) || (index_bus < 0)
            || (index >= USB_MAX_DEVICE) || (index < 0))
    {
        return NULL;
    }

    dev = &usb_dev[(index_bus * USB_MAX_DEVICE) + index];

    if ((controller_priv[index_bus] == NULL) || (dev->devnum == -1))
    {
        return NULL;
    }

    return dev;
}

/*
 * returns a pointer of a new device structure or NULL, if
 * no device struct is available
 */
struct usb_device *usb_alloc_new_device(int bus_index, void *priv)
{
    int i;
    int index = dev_index[bus_index];
    struct usb_device *dev;

    dbg("USB %d new device %d\r\n", bus_index, index);
    if (index >= USB_MAX_DEVICE)
    {
        dbg("ERROR, too many USB Devices, max=%d\r\n", USB_MAX_DEVICE);
        return NULL;
    }

    /* default Address is 0, real addresses start with 1 */
    dev = &usb_dev[(bus_index * USB_MAX_DEVICE) + index];
    dev->devnum = index + 1;
    dev->maxchild = 0;

    for (i = 0; i < USB_MAXCHILDREN; dev->children[i++] = NULL)
        ;

    dev->parent = NULL;
    dev->priv_hcd = priv;
    dev->usbnum = bus_index;
    dev_index[bus_index]++;

    return dev;
}

// #define CONFIG_LEGACY_USB_INIT_SEQ
/*
 * By the time we get here, the device has gotten a new device ID
 * and is in the default state. We need to identify the thing and
 * get the ball rolling..
 *
 * Returns 0 for success, != 0 for error.
 */
int usb_new_device(struct usb_device *dev)
{
    int addr;
    int err;
    int tmp;
    unsigned char *tmpbuf;

    dbg("\r\n");
#ifndef CONFIG_LEGACY_USB_INIT_SEQ
    struct usb_device_descriptor *desc;
    int port = -1;
    struct usb_device *parent = dev->parent;
    unsigned short portstatus;
#endif

    if (dev == NULL)
    {
        dbg("called with NULL device\r\n");
        return 1;
    }

    /* We still haven't set the Address yet */
    addr = dev->devnum;
    dev->devnum = 0;

    tmpbuf = (unsigned char *) driver_mem_alloc(USB_BUFSIZ);
    if (tmpbuf == NULL)
    {
        dbg("malloc failure\r\n");
        return 1;
    }

#ifdef CONFIG_LEGACY_USB_INIT_SEQ
    /*
     * this is the old and known way of initializing devices, it is
     * different than what Windows and Linux are doing. Windows and Linux
     * both retrieve 64 bytes while reading the device descriptor
     * Several USB stick devices report ERR: CTL_TIMEOUT, caused by an
     * invalid header while reading 8 bytes as device descriptor.
     */
    dev->descriptor.bMaxPacketSize0 = 8;	    /* Start off at 8 bytes  */
    dev->maxpacketsize = PACKET_SIZE_8;
    dev->epmaxpacketin[0] = 8;
    dev->epmaxpacketout[0] = 8;
    err = usb_get_descriptor(dev, USB_DT_DEVICE, 0, &dev->descriptor, 8);
    if (err < 8)
    {
        dbg("\r\nUSB device not responding, giving up (status=%lX)\r\n", dev->status);
        driver_mem_free(tmpbuf);
        return 1;
    }
#else
    /*
     * This is a Windows scheme of initialization sequence, with double
     * reset of the device (Linux uses the same sequence)
     * Some equipment is said to work only with such init sequence; this
     * patch is based on the work by Alan Stern:
     * http://sourceforge.net/mailarchive/forum.php?
     * thread_id=5729457&forum_id=5398
     */

    /*
     * send 64-byte GET-DEVICE-DESCRIPTOR request.  Since the descriptor is
     * only 18 bytes long, this will terminate with a short packet.  But if
     * the maxpacket size is 8 or 16 the device may be waiting to transmit
     * some more, or keeps on retransmitting the 8 byte header.
     */
    desc = (struct usb_device_descriptor *) tmpbuf;
    dev->descriptor.bMaxPacketSize0 = 64;	    /* Start off at 64 bytes  */

    /* Default to 64 byte max packet size */
    dev->maxpacketsize = PACKET_SIZE_64;
    dev->epmaxpacketin[0] = 64;
    dev->epmaxpacketout[0] = 64;
    err = usb_get_descriptor(dev, USB_DT_DEVICE, 0, desc, 64);

    if (err < 0)
    {
        dbg("usb_new_device: usb_get_descriptor() failed\r\n");
        driver_mem_free(tmpbuf);
        return 1;
    }
    dev->descriptor.bMaxPacketSize0 = desc->bMaxPacketSize0;

    /* find the port number we're at */
    if (parent)
    {
        int j;
        for (j = 0; j < parent->maxchild; j++)
        {
            if (parent->children[j] == dev)
            {
                port = j;
                break;
            }
        }
        dbg("port = %d\r\n", port);

        if (port < 0)
        {
            dbg("usb_new_device: cannot locate device's port.\r\n");
            driver_mem_free(tmpbuf);

            return 1;
        }

        /* reset the port for the second time */
        err = hub_port_reset(dev->parent, port, &portstatus);
        if (err < 0)
        {
            dbg("\r\nCouldn't reset port %d\r\n", port);
            driver_mem_free(tmpbuf);

            return 1;
        }
    }
#endif
    dev->epmaxpacketin[0] = dev->descriptor.bMaxPacketSize0;
    dev->epmaxpacketout[0] = dev->descriptor.bMaxPacketSize0;

    switch (dev->descriptor.bMaxPacketSize0)
    {
        case 8: dev->maxpacketsize  = PACKET_SIZE_8; break;
        case 16: dev->maxpacketsize = PACKET_SIZE_16; break;
        case 32: dev->maxpacketsize = PACKET_SIZE_32; break;
        case 64: dev->maxpacketsize = PACKET_SIZE_64; break;
    }

    dev->devnum = addr;
    err = usb_set_address(dev); /* set address */

    if (err < 0)
    {
        dbg("\r\nUSB device not accepting new address (error=%lX)\r\n", dev->status);
        driver_mem_free(tmpbuf);

        return 1;
    }

    wait(10);	/* Let the SET_ADDRESS settle */
    tmp = sizeof(dev->descriptor);
    err = usb_get_descriptor(dev, USB_DT_DEVICE, 0, &dev->descriptor, sizeof(dev->descriptor));
    if (err < tmp)
    {
        if (err < 0)
        {
            dbg("unable to get device descriptor (error=%d)\r\n", err);
        }
        else
        {
            dbg("USB device descriptor short read (expected %i, got %i)\r\n", tmp, err);
        }
        driver_mem_free(tmpbuf);

        return 1;
    }

    /* correct values */
    dev->descriptor.bcdUSB = swpw(dev->descriptor.bcdUSB);
    dev->descriptor.idVendor = swpw(dev->descriptor.idVendor);
    dev->descriptor.idProduct = swpw(dev->descriptor.idProduct);
    dev->descriptor.bcdDevice = swpw(dev->descriptor.bcdDevice);

    /* only support for one config for now */
    usb_get_configuration_no(dev, &tmpbuf[0], 0);
    usb_parse_config(dev, &tmpbuf[0], 0);
    usb_set_maxpacket(dev);

    /* we set the default configuration here */
    if (usb_set_configuration(dev, dev->config.bConfigurationValue))
    {
        dbg("failed to set default configuration len %d, status %lX\r\n", dev->act_len, dev->status);
        driver_mem_free(tmpbuf);
        return -1;
    }
    dbg("new device strings: Mfr=%d, Product=%d, SerialNumber=%d\r\n",
           dev->descriptor.iManufacturer, dev->descriptor.iProduct,
           dev->descriptor.iSerialNumber);

    memset(dev->mf, 0, sizeof(dev->mf));
    memset(dev->prod, 0, sizeof(dev->prod));
    memset(dev->serial, 0, sizeof(dev->serial));

    if (dev->descriptor.iManufacturer)
    {
        usb_string(dev, dev->descriptor.iManufacturer, dev->mf, sizeof(dev->mf));
    }
    if (dev->descriptor.iProduct)
    {
        usb_string(dev, dev->descriptor.iProduct, dev->prod, sizeof(dev->prod));
    }
    if (dev->descriptor.iSerialNumber)
    {
        usb_string(dev, dev->descriptor.iSerialNumber, dev->serial, sizeof(dev->serial));
    }
    dbg("Manufacturer %s\r\n", dev->mf);
    dbg("Product      %s\r\n", dev->prod);
    dbg("SerialNumber %s\r\n", dev->serial);

    /* now probe if the device is a hub */
    usb_hub_probe(dev, 0);
    driver_mem_free(tmpbuf);

    return 0;
}

/*
 * build device Tree
 */
void usb_scan_devices(void *priv)
{
    int i;
    struct usb_device *dev;

    /* first make all devices unknown */
    for (i = 0; i < USB_MAX_DEVICE; i++)
    {
        memset(&usb_dev[(bus_index * USB_MAX_DEVICE) + i], 0, sizeof(struct usb_device));
        usb_dev[(bus_index * USB_MAX_DEVICE) + i].devnum = -1;
    }
    dev_index[bus_index] = 0;

    /*
     * device 0 is always present (root hub, so let it analyze)
     */
    dev = usb_alloc_new_device(bus_index, priv);
    if (usb_new_device(dev))
    {
        xprintf("No USB Device found\r\n");
        if (dev != NULL)
        {
            dev_index[bus_index]--;
        }
    }
    else
    {
        xprintf("%d USB Device(s) found\r\n", dev_index[bus_index]);
    }

    /* insert "driver" if possible */
    if (drv_usb_kbd_init() < 0)
    {
        xprintf("No USB keyboard found\r\n");
    }
    else
    {
        xprintf("USB HID keyboard driver installed\r\n");
    }

    if (drv_usb_mouse_init() < 0)
    {
        xprintf("No USB mouse found\r\n");
    }
    else
    {
        xprintf("USB HID mouse driver installed\r\n");
    }
    xprintf("Scan end\r\n");
}

