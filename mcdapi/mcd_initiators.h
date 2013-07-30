#ifndef _MCD_INITIATORS_H_
#define _MCD_INITIATORS_H_

/* initiators */

/* direct connected initiators */
#define DMA_ALWAYS		(0)
#define DMA_DSPI_RXFIFO	(1)
#define DMA_DSPI_TXFIFO	(2)
#define	DMA_DREQ0		(3)
#define DMA_PSC0_RX		(4)
#define	DMA_PSCO_TX		(5)
#define	DMA_USB_EP0		(6)
#define	DMA_USB_EP1		(7)
#define DMA_USB_EP2		(8)
#define DMA_USB_EP3		(9)
#define DMA_PCI_TX		(10)
#define DMA_PCI_RX		(11)
#define DMA_PSC1_RX		(12)
#define DMA_PSC1_TX		(13)
#define	DMA_I2C_RX		(14)
#define DMA_I2C_TX		(15)

/* multiplexed initiators */
#define DMA_FEC0_RX 	(16)
#define DMA_FEC0_TX 	(17)
#define DMA_FEC1_RX 	(18)
#define DMA_FEC1_TX 	(19)
#define DMA_DREQ1 		(20)
#define DMA_CTM0 		(21)
#define DMA_CTM1 		(22)
#define DMA_CTM2 		(23)
#define DMA_CTM3 		(24)
#define DMA_CTM4 		(25)
#define DMA_CTM5 		(26)
#define DMA_CTM6 		(27)
#define DMA_CTM7 		(28)
#define DMA_USBEP4 		(29)
#define DMA_USBEP5 		(30)
#define DMA_USBEP6		(31)
#define DMA_PSC2_RX		(32)
#define DMA_PSC2_TX		(33)
#define DMA_PSC3_RX		(34)
#define DMA_PSC3_TX		(35)
#define DMA_FEC_RX(x)	((x == 0) ? DMA_FEC0_RX : DMA_FEC1_RX)
#define DMA_FEC_TX(x)	((x == 0) ? DMA_FEC0_TX : DMA_FEC1_TX)

#endif /* _MCD_INITIATORS_H_ */
