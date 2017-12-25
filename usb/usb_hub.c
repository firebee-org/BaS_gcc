/*
 * HUB "Driver"
 * Probes device for being a hub and configure it
 */

#include <stdint.h>
#include "bas_string.h"
#include "bas_printf.h"
#include "util.h"				/* for byte swap funcs */
#include "wait.h"
#include <stdarg.h>
#include "usb.h"
#include "usb_hub.h"

// // #define DEBUG
#include "debug.h"

static struct usb_hub_device hub_dev[USB_MAX_BUS][USB_MAX_HUB];
static int usb_hub_index[USB_MAX_BUS];

int usb_get_hub_descriptor(struct usb_device *dev, void *data, int size)
{
    return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
     USB_REQ_GET_DESCRIPTOR, USB_DIR_IN | USB_RT_HUB, USB_DT_HUB << 8, 0, data, size, USB_CNTL_TIMEOUT);
}

int usb_clear_hub_feature(struct usb_device *dev, int feature)
{
    return usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
     USB_REQ_CLEAR_FEATURE, USB_RT_HUB, feature, 0, NULL, 0, USB_CNTL_TIMEOUT);
}

int usb_clear_port_feature(struct usb_device *dev, int port, int feature)
{
    return usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
     USB_REQ_CLEAR_FEATURE, USB_RT_PORT, feature,	port, NULL, 0, USB_CNTL_TIMEOUT);
}

int usb_set_port_feature(struct usb_device *dev, int port, int feature)
{
    return usb_control_msg(dev, usb_sndctrlpipe(dev, 0),
     USB_REQ_SET_FEATURE, USB_RT_PORT, feature,	port, NULL, 0, USB_CNTL_TIMEOUT);
}

int usb_get_hub_status(struct usb_device *dev, void *data)
{
    return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
     USB_REQ_GET_STATUS, USB_DIR_IN | USB_RT_HUB, 0, 0, data, sizeof(struct usb_hub_status), USB_CNTL_TIMEOUT);
}

int usb_get_port_status(struct usb_device *dev, int port, void *data)
{
    return usb_control_msg(dev, usb_rcvctrlpipe(dev, 0),
     USB_REQ_GET_STATUS, USB_DIR_IN | USB_RT_PORT, 0, port, data, sizeof(struct usb_hub_status), USB_CNTL_TIMEOUT);
}

static void usb_hub_power_on(struct usb_hub_device *hub)
{
    int i;
    struct usb_device *dev;

    dev = hub->pusb_dev;
    /* Enable power to the ports */
    dbg("enabling power on all ports\r\n");
    for (i = 0; i < dev->maxchild; i++)
    {
        usb_set_port_feature(dev, i + 1, USB_PORT_FEAT_POWER);
        dbg("port %d returns %lx\r\n", i + 1, dev->status);
        wait(hub->desc.bPwrOn2PwrGood * 2 * 1000);
    }
}

void usb_hub_reset(int bus_index)
{
    usb_hub_index[bus_index] = 0;
}

struct usb_hub_device *usb_hub_allocate(void)
{
    if (usb_hub_index[bus_index] < USB_MAX_HUB)
    {
        return &hub_dev[bus_index][usb_hub_index[bus_index]++];
    }

    dbg("ERROR: USB_MAX_HUB (%d) reached\r\n", USB_MAX_HUB);

    return NULL;
}

#define MAX_TRIES 5

static inline char *portspeed(int portstatus)
{
    if (portstatus & (1 << USB_PORT_FEAT_HIGHSPEED))
    {
        return "480 Mb/s";
    }
    else if (portstatus & (1 << USB_PORT_FEAT_LOWSPEED))
    {
        return "1.5 Mb/s";
    }
    else
    {
        return "12 Mb/s";
    }
}

int hub_port_reset(struct usb_device *dev, int port, unsigned short *portstat)
{
    int tries;
    struct usb_port_status portsts;
    unsigned short portstatus, portchange;

    dbg("");
    dbg("hub_port_reset: resetting port %d...\r\n", port + 1);

    for (tries = 0; tries < MAX_TRIES; tries++)
    {
        usb_set_port_feature(dev, port + 1, USB_PORT_FEAT_RESET);

#ifdef USB_POLL_HUB
        if (pxCurrentTCB != NULL)
            vTaskDelay((200 * configTICK_RATE_HZ) / 1000);
        else
#endif
            wait(10 * 1000);
        if (usb_get_port_status(dev, port + 1, &portsts) < 0)
        {
            dbg("get_port_status failed status %lX\r\n", dev->status);
            return -1;
        }

        portstatus = swpw(portsts.wPortStatus);
        portchange = swpw(portsts.wPortChange);

        dbg("USB %d portstatus 0x%x, change 0x%x, %s\r\n", dev->usbnum, portstatus, portchange, portspeed(portstatus));
        dbg("STAT_C_CONNECTION = %d STAT_CONNECTION = %d USB_PORT_STAT_ENABLE = %d\r\n",
                (portchange & USB_PORT_STAT_C_CONNECTION) ? 1 : 0,
                (portstatus & USB_PORT_STAT_CONNECTION) ? 1 : 0,
                (portstatus & USB_PORT_STAT_ENABLE) ? 1 : 0);

        if ((portchange & USB_PORT_STAT_C_CONNECTION) || !(portstatus & USB_PORT_STAT_CONNECTION))
        {
            return -1;
        }

        if (portstatus & USB_PORT_STAT_ENABLE)
        {
            break;
        }

#ifdef USB_POLL_HUB
        if (pxCurrentTCB != NULL)
            vTaskDelay((200*configTICK_RATE_HZ)/1000);
        else
#endif
            wait(20 * 1000);
    }

    if (tries == MAX_TRIES)
    {
        dbg("USB %d, cannot enable port %d after %d retries, disabling port.\r\n", dev->usbnum, port + 1, MAX_TRIES);
        dbg("Maybe the USB cable is bad?\r\n");

        return -1;
    }
    usb_clear_port_feature(dev, port + 1, USB_PORT_FEAT_C_RESET);

    *portstat = portstatus;

    return 0;
}

void usb_hub_port_connect_change(struct usb_device *dev, int port)
{
    struct usb_device *usb;
    struct usb_port_status portsts;
    unsigned short portstatus;

    /* Check status */
    if (usb_get_port_status(dev, port + 1, &portsts) < 0)
    {
        dbg("USB %d get_port_status failed\r\n", dev->usbnum);

        return;
    }

    portstatus = swpw(portsts.wPortStatus);
#ifdef USB_DEBUG
    {
        unsigned short portchange;

        portchange = swpw(portsts.wPortChange);
        dbg("USB %d, portstatus %x, change %x, %s\r\n", dev->usbnum, portstatus, portchange, portspeed(portstatus));
    }
#endif /* USB_DEBUG */

    /* Clear the connection change status */
    usb_clear_port_feature(dev, port + 1, USB_PORT_FEAT_C_CONNECTION);

    /* Disconnect any existing devices under this port */

    if (((!(portstatus & USB_PORT_STAT_CONNECTION))
     && (!(portstatus & USB_PORT_STAT_ENABLE))) || (dev->children[port]))
    {
        dbg("USB %d port %i disconnected\r\n", dev->usbnum, port + 1);
        usb_disconnect(&dev->children[port]);

        /* Return now if nothing is connected */
        if (!(portstatus & USB_PORT_STAT_CONNECTION))
        {
            return;
        }
    }
#ifdef USB_POLL_HUB
    if (pxCurrentTCB != NULL)
        vTaskDelay((200*configTICK_RATE_HZ)/1000);
    else
#endif
        wait(2000);
    /* Reset the port */
    if (hub_port_reset(dev, port, &portstatus) < 0)
    {
        dbg("USB %d cannot reset port %i!?\r\n", dev->usbnum, port + 1);

        return;
    }
#ifdef USB_POLL_HUB
    if (pxCurrentTCB != NULL)
        vTaskDelay((200*configTICK_RATE_HZ)/1000);
    else
#endif
        wait(2000);

    /* Allocate a new device struct for it */
    usb = usb_alloc_new_device(dev->usbnum, dev->priv_hcd);

    if (portstatus & USB_PORT_STAT_HIGH_SPEED)
    {
        usb->speed = USB_SPEED_HIGH;
    }
    else if (portstatus & USB_PORT_STAT_LOW_SPEED)
    {
        usb->speed = USB_SPEED_LOW;
    }
    else
    {
        usb->speed = USB_SPEED_FULL;
    }

    dbg("usb device = %p\r\n", usb);
    dev->children[port] = usb;
    usb->parent = dev;

    /* Run it through the hoops (find a driver, etc) */
    if (usb_new_device(usb))
    {
        /* Woops, disable the port */
        dbg("USB %d hub: disabling port %d\r\n", dev->usbnum, port + 1);
        usb_clear_port_feature(dev, port + 1, USB_PORT_FEAT_ENABLE);
    }

#ifdef USB_POLL_HUB
    else if (pxCurrentTCB != NULL)
    {
        usb_kbd_register(usb);
        usb_mouse_register(usb);
#ifdef CONFIG_USB_STORAGE
        usb_stor_register(usb);
#endif /* CONFIG_USB_STORAGE */
    }
#endif
}

static void usb_hub_events(struct usb_device *dev)
{
    int i;
    struct usb_hub_device *hub = dev->hub;

    if (hub == NULL)
    {
        return;
    }

    for (i = 0; i < dev->maxchild; i++)
    {
        struct usb_port_status portsts;
        unsigned short portstatus, portchange;

        if (usb_get_port_status(dev, i + 1, &portsts) < 0)
        {
            dbg("get_port_status failed\r\n");

            continue;
        }
        portstatus = swpw(portsts.wPortStatus);
        portchange = swpw(portsts.wPortChange);

        dbg("USB %d Port %d Status %X Change %X\r\n", dev->usbnum, i + 1, portstatus, portchange);

        if (portchange & USB_PORT_STAT_C_CONNECTION)
        {
            dbg("USB %d port %d connection change\r\n", dev->usbnum, i + 1);
            usb_hub_port_connect_change(dev, i);
        }

        if (portchange & USB_PORT_STAT_C_ENABLE)
        {
            dbg("USB %d port %d enable change, status %x\r\n", dev->usbnum, i + 1, portstatus);
            usb_clear_port_feature(dev, i + 1, USB_PORT_FEAT_C_ENABLE);

            /*
             * EM interference sometimes causes bad shielded USB
             * devices to be shutdown by the hub, this hack enables
             * them again. Works at least with mouse driver
             */
            if (!(portstatus & USB_PORT_STAT_ENABLE) && (portstatus & USB_PORT_STAT_CONNECTION) && ((dev->children[i])))
            {
                dbg("USB %d already running port %i disabled by hub (EMI?), re-enabling...\r\n", dev->usbnum, i + 1);
                usb_hub_port_connect_change(dev, i);
            }
        }

        if (portstatus & USB_PORT_STAT_SUSPEND)
        {
            dbg("USB %d port %d suspend change\r\n", dev->usbnum, i + 1);
            usb_clear_port_feature(dev, i + 1, USB_PORT_FEAT_SUSPEND);
        }

        if (portchange & USB_PORT_STAT_C_OVERCURRENT)
        {
            dbg("USB %d port %d over-current change\r\n", dev->usbnum, i + 1);
            usb_clear_port_feature(dev, i + 1, USB_PORT_FEAT_C_OVER_CURRENT);
            usb_hub_power_on(hub);
        }

        if (portchange & USB_PORT_STAT_C_RESET)
        {
            dbg("USB %d port %d reset change\r\n", dev->usbnum, i + 1);
            usb_clear_port_feature(dev, i + 1, USB_PORT_FEAT_C_RESET);
        }
    } /* end for i all ports */
}

#ifdef USB_POLL_HUB
void usb_poll_hub_task(void *pvParameters)
{
    int index_bus = 0;
    portTickType timeout = configTICK_RATE_HZ/10;
    if (pvParameters);
    while(1)
    {
        if (xQueueAltReceive(queue_poll_hub, &index_bus, timeout) == pdPASS)
        {
            if ((index_bus >= 0) && (index_bus < USB_MAX_BUS) && (controller_priv[index_bus] != NULL))
            {
                dbg("USB %d event change\r\n", index_bus);
#ifdef CONFIG_USB_INTERRUPT_POLLING
                *vblsem = 0;
#endif
                usb_hub_events(&usb_dev[index_bus * USB_MAX_DEVICE]);
#ifdef CONFIG_USB_INTERRUPT_POLLING
                *vblsem = 1;
#endif
            }
        }
        else /* timeout */
        {
            int i;
#ifdef CONFIG_USB_INTERRUPT_POLLING
            *vblsem = 0;
#endif
            for (i = 0; i < USB_MAX_BUS ; i++)
            {
                if (controller_priv[i] != NULL)
                    usb_hub_events(&usb_dev[i * USB_MAX_DEVICE]);
            }
#ifdef CONFIG_USB_INTERRUPT_POLLING
            *vblsem = 1;
#endif
        }
        timeout = portMAX_DELAY;
    }
}
#endif /* USB_POLL_HUB */

int usb_hub_configure(struct usb_device *dev)
{
    unsigned char *buffer;
    unsigned char *bitmap;
    struct usb_hub_descriptor *descriptor;
    int i;
    struct usb_hub_device *hub;

    /* "allocate" Hub device */
    hub = usb_hub_allocate();
    dev->hub = hub;

    if (hub == NULL)
    {
        dbg("could not allocate hub\r\n");

        return -1;
    }

    hub->pusb_dev = dev;
    buffer = (unsigned char *) driver_mem_alloc(USB_BUFSIZ);

    if (buffer == NULL)
    {
        dbg("driver_mem_alloc() failure\r\n");
        return -1;
    }

    /* Get the the hub descriptor */
    if (usb_get_hub_descriptor(dev, buffer, 4) < 0)
    {
        dbg("failed to get hub descriptor, giving up %lX\r\n", dev->status);
        driver_mem_free(buffer);
        return -1;
    }

    dbg("bLength:0x%02X bDescriptorType:0x%02X bNbrPorts:0x%02X\r\n", buffer[0], buffer[1], buffer[2]);
    descriptor = (struct usb_hub_descriptor *)buffer;

    /* silence compiler warning if USB_BUFSIZ is > 256 [= sizeof(char)] */
    i = descriptor->bLength;

    if (i > USB_BUFSIZ)
    {
        dbg("failed to get hub descriptor - too long: %d\r\n", descriptor->bLength);
        driver_mem_free(buffer);

        return -1;
    }

    if (usb_get_hub_descriptor(dev, buffer, descriptor->bLength) < 0)
    {
        dbg("failed to get hub descriptor 2nd giving up %lX\r\n", dev->status);
        driver_mem_free(buffer);

        return -1;
    }

    memcpy((unsigned char *) &hub->desc, buffer, descriptor->bLength);

    /* adjust 16bit values */
    hub->desc.wHubCharacteristics = swpw(descriptor->wHubCharacteristics);

    /* set the bitmap */
    bitmap = (unsigned char *) &hub->desc.DeviceRemovable[0];

    /* devices not removable by default */

    memset(bitmap, 0xff, (USB_MAXCHILDREN + 1 + 7) / 8);
    bitmap = (unsigned char *) &hub->desc.PortPowerCtrlMask[0];
    memset(bitmap, 0xff, (USB_MAXCHILDREN + 1 + 7) / 8); /* PowerMask = 1B */

    for (i = 0; i < ((hub->desc.bNbrPorts + 1 + 7) / 8); i++)
    {
        hub->desc.DeviceRemovable[i] = descriptor->DeviceRemovable[i];
    }

    for (i = 0; i < ((hub->desc.bNbrPorts + 1 + 7) / 8); i++)
    {
        hub->desc.DeviceRemovable[i] = descriptor->PortPowerCtrlMask[i];
    }

    dev->maxchild = descriptor->bNbrPorts;
    dbg("USB %d, %d ports detected\r\n", dev->usbnum, dev->maxchild);
    if (dev->maxchild >= 10)
    {
         dev->maxchild = 10;
    }

    switch(hub->desc.wHubCharacteristics & HUB_CHAR_LPSM)
    {
        case 0x00: dbg("ganged power switching\r\n"); break;
        case 0x01: dbg("individual port power switching\r\n"); break;
        case 0x02:
        case 0x03: dbg("unknown reserved power switching mode\r\n"); break;
    }

    if (hub->desc.wHubCharacteristics & HUB_CHAR_COMPOUND)
    {
        dbg("part of a compound device\r\n");
    }
    else
    {
        dbg("standalone hub\r\n");
    }

    switch(hub->desc.wHubCharacteristics & HUB_CHAR_OCPM)
    {
        case 0x00: dbg("global over-current protection\r\n"); break;
        case 0x08: dbg("individual port over-current protection\r\n"); break;
        case 0x10:
        case 0x18: dbg("no over-current protection\r\n"); break;
    }

    dbg("power on to power good time: %dms\r\n", descriptor->bPwrOn2PwrGood * 2);
    dbg("hub controller current requirement: %dmA\r\n", descriptor->bHubContrCurrent);

    for (i = 0; i < dev->maxchild; i++)
    {
        dbg("USB %d port %d is%s removable\r\n",
            dev->usbnum, i + 1,
            hub->desc.DeviceRemovable[(i + 1) / 8] & (1 << ((i + 1) % 8)) ? " not" : "");
    }

    if (sizeof(struct usb_hub_status) > USB_BUFSIZ)
    {
        dbg("usb_hub_configure: failed to get Status - too long: %d\r\n", descriptor->bLength);
        driver_mem_free(buffer);

        return -1;
    }

    if (usb_get_hub_status(dev, buffer) < 0)
    {
        dbg("usb_hub_configure: failed to get Status %lX\r\n", dev->status);
        driver_mem_free(buffer);

        return -1;
    }

#ifdef DEBUG_HUB
    {
        struct usb_hub_status *hubsts;

        hubsts = (struct usb_hub_status *) buffer;
        dbg("get_hub_status returned status %X, change %X\r\n",
                        swpw(hubsts->wHubStatus), swpw(hubsts->wHubChange));
        dbg("local power source is %s\r\n",
                        (swpw(hubsts->wHubStatus) & HUB_STATUS_LOCAL_POWER) ? "lost (inactive)" : "good");
        dbg("%sover-current condition exists\r\n",
                        (swpw(hubsts->wHubStatus) & HUB_STATUS_OVERCURRENT) ? "" : "no ");
    }
#endif /* USB_DEBUG */

    usb_hub_power_on(hub);

#ifdef USB_POLL_HUB
    if ((queue_poll_hub == NULL) && (pxCurrentTCB != NULL))
    {
        queue_poll_hub = xQueueCreate(64, sizeof(int));
        if (queue_poll_hub != NULL)
        {
            /* Create poll/event task */
            if (xTaskCreate(usb_poll_hub_task, (void *)"USBHub", configMINIMAL_STACK_SIZE, NULL, 16, NULL) != pdPASS)
            {
                vQueueDelete(queue_poll_hub);
                queue_poll_hub = NULL;
            }
        }
        vTaskDelay(configTICK_RATE_HZ);
    }
    if (queue_poll_hub == NULL)
#endif

    usb_hub_events(dev);
    driver_mem_free(buffer);

    return 0;
}

int usb_hub_probe(struct usb_device *dev, int ifnum)
{
    struct usb_interface_descriptor *iface;
    struct usb_endpoint_descriptor *ep;
    int ret;

    iface = &dev->config.if_desc[ifnum];

    /* Is it a hub? */
    if (iface->bInterfaceClass != USB_CLASS_HUB)
    {
        dbg("iface->bInterfaceClass != USB_CLASS_HUB (%d), %d instead\r\n", USB_CLASS_HUB, iface->bInterfaceClass);
        return 0;
    }

    /*
     * Some hubs have a subclass of 1, which AFAICT according to the
     * specs is not defined, but it works
     */
    if ((iface->bInterfaceSubClass != 0) && (iface->bInterfaceSubClass != 1))
    {
        dbg("iface->bInterfaceSubClass != {0, 1} (%d instead)\r\n", iface->bInterfaceSubClass);

        return 0;
    }

    /* Multiple endpoints? What kind of mutant ninja-hub is this? */
    if (iface->bNumEndpoints != 1)
    {
        dbg("iface->bNumEndpoints != 1 (%d instead)\r\n", iface->bNumEndpoints);
        return 0;
    }

    ep = &iface->ep_desc[0];

    /* Output endpoint? Curiousier and curiousier.. */
    if (!(ep->bEndpointAddress & USB_DIR_IN))
    {
        dbg("!(ep->bEndpointAddress != USB_DIR_IN (0x%x instead)\r\n", ep->bEndpointAddress);

        return 0;
    }

    /* If it's not an interrupt endpoint, we'd better punt! */
    if ((ep->bmAttributes & 3) != 3)
    {
        return 0;
    }

    /* We found a hub */
    dbg("USB %d hub found\r\n", dev->usbnum);

    ret = usb_hub_configure(dev);

    return ret;
}


