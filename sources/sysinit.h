/*
 * File:		sysinit.h
 * Purpose:		Firebee Power-on Reset configuration
 *
 * Notes:
 *
 */

#ifndef __SYSINIT_H__
#define __SYSINIT_H__

extern void wait_10us(void);

/* send a 16-bit word out on the serial port */
#define uart_out_word(a)	MCF_PSC0_PSCTB_8BIT = (a);

/* adresses where FPGA data lives in flash */
static const uint8_t *FPGA_FLASH_DATA = (uint8_t *) 0xe0700000L;
static const uint8_t *FPGA_FLASH_DATA_END = (uint8_t *) 0xe0800000L;

/* function(s) from init_fpga.c */
extern void init_fpga(void);

#endif /* __SYSINIT_H__ */


