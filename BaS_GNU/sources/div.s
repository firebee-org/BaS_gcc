
/********************************************************************/
// sd card
/********************************************************************/
#define	dspi_dtar0	0x0c
#define	dspi_dsr	0x2c	
#define	dspi_dtfr	0x34	
#define	dspi_drfr	0x38
	
sd_test:
		lea			0x40000,a5					// basis addresse
		move.l		#0x1fffffff,d0				// normal dspi
		move.l		d0,MCF_PAD_PAR_DSPI
		lea			MCF_DSPI_DMCR,a0
		move.l		#0x802d3c00,(a0)			// 8 bit 4MHz
		move.l		#0x38551120,d0
		move.l		d0,dspi_dtar0(a0)				// 
		move.l		#0x08200000,d4				// tx vorbesetzen
		mov3q.l		#-1,dspi_dsr(a0)
		
		move.b		#0xc0,(a0)			// 8 bit 4MHz
		bsr			warte_1ms
		move.b		#0x80,(a0)			// 8 bit 4MHz
// sd idle
		moveq.l		#100,d6				// 100 versuche
sd_idle:
		bsr			sd_16clk
		move.b		#0x40,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x95,d4
		bsr			sd_com
		
		bsr			sd_get_status
		cmp.b		#0x01,d5
		beq			wait_of_aktiv
		subq.l		#1,d6
		beq			sd_not
		bra			sd_idle
				
// acdm 41
wait_of_aktiv:
		bsr			sd_16clk
		move.b		#0x77,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com

		bsr			sd_get_status
		cmp.b		#0x05,d5
		beq			wait_of_aktiv	


wait_of_aktiv2:
		bsr			sd_16clk
		move.b		#0x69,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x02,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com

		bsr			sd_get_status
		tst.b		d5
		beq			sd_init_ok
		cmp.b		#0x05,d5
		beq			wait_of_aktiv2	
		bra			wait_of_aktiv

sd_init_ok:

// blockgrösse 512byt
sd_bg:
		bsr			sd_16clk
		move.b		#0x50,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#02,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com
		
		bsr			sd_get_status
		tst.b		d5
		bne			sd_bg
		
// read block
sd_rb:
		bsr			sd_16clk
		move.b		#0x51,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x08,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com
		
		bsr			sd_get_status
		tst.b		d5
		bne			sd_rb

		move.l		a5,a4				// adresse setzen
		bsr			sd_rcv_block

// write block
sd_wb:
		bsr			sd_16clk
		move.b		#0x58,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x08,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com
		
		bsr			sd_get_status
		tst.b		d5
		bne			sd_wb
		
		bsr			sd_send_block		

// read block 2
sd_rb2:
		bsr			sd_16clk
		move.b		#0x51,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x08,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com
		
		bsr			sd_get_status
		tst.b		d5
		bne			sd_rb

		bsr			sd_rcv_block
		clr.l		d0
		halt
		halt
		rts
// status holen -------------------------------
sd_not:
		moveq.l		#-1,d0
		halt
		halt
		rts

// status holen -------------------------------
sd_get_status:
		move.b		#0xff,d4
		bsr			sd_com
		cmp.b		#0xff,d5
		beq			sd_get_status
		rts
// byt senden und holen ---------------------		
sd_com:
		move.l		d4,dspi_dtfr(a0)
wait_auf_complett:
		btst.b		#7,dspi_dsr(a0)
		beq			wait_auf_complett
		move.l		dspi_drfr(a0),d5
		mov3q.l		#-1,dspi_dsr(a0)		// clr status register
		rts

// daten holen ----------------------------
sd_rcv_block:
		move.l		#512,d3				// 512 byts + 2 ccr byts
		move.b		#0xff,d4
sd_rcv_rb_w:		
		bsr			sd_get_status 
		cmp.b		#0xfe,d5			// daten bereit?
		bne			sd_rcv_rb_w		// nein->
sd_rcv_rd_rb:
		bsr			sd_com
		move.b		d5,(a4)+
		subq.l		#1,d3
		bne			sd_rcv_rd_rb
// crc holen
		bsr			sd_com				// crc 1.byt
		move.b		d5,d1
		bsr			sd_com				// crc 2.byt
		move.b		d5,d2
		rts

// daten schreiben -----------------------
sd_send_block:
		move.l		#512,d3		
		move.b		#0xfe,d4			// start token
		bsr			sd_com				// senden
sd_send_wr_wb:
		move.b		#0xc7,d4			// data
		bsr			sd_com				// senden
		subq.l		#1,d3
		bne			sd_send_wr_wb	
// send crc
		move.b		d1,d4			
		bsr			sd_com				// crc 1.byt
		move.b		d2,d4
		bsr			sd_com				// crc 2.byt
sd_send_wr_ww:
		bsr			sd_get_status
		and.l		#0x1f,d5
		cmp.b		#0x05,d5
		bne			sd_send_wr_ww
sd_sendwait_wr_f:
		bsr			sd_com
		cmp.b		#0xff,d5
		bne			sd_sendwait_wr_f
		rts
// clock einfügen -------------------------------------
sd_16clk:
		move.b		#0xc0,(a0)			// 8 bit 4MHz
		nop
		move.b		#0x80,(a0)			// 8 bit 4MHz
		rts
// cdm 58
read_ocr:
		bsr			sd_16clk
		move.b		#0x7a,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com
		move.b		#0xaa,d4
		bsr			sd_com
		move.b		#0x87,d4
		bsr			sd_com
		
		bsr			sd_get_status

		halt
		
		move.l		#'Ver1',d6
		cmp.b		#5,d5
		beq			sd_v1
		cmp.b		#1,d5
		bne			read_csd
		
		move.b		#0xff,d4
		bsr			sd_com
		move.b		d5,d0
		bsr			sd_com
		move.b		d5,d1
		bsr			sd_com
		move.b		d5,d2
		bsr			sd_com
/******************************************/
#include "MCF5475.h"
#include "startcf.h"

extern unsigned long far __SP_AFTER_RESET[];
extern unsigned long far __Bas_base[];

	/* imported routines */
//extern int warten_20ms();
//extern int warten_200us();
//extern int warten_10us();
 
/********************************************************************/
void asm sd_test(void)
{
		clr.w		MCF_PAD_PAR_DSPI
		lea			MCF_GPIO_PPDSDR_DSPI,a2		// data in
		lea			MCF_GPIO_PODR_DSPI,a1		// data out
		move.b		#0x00,(a1)					// alle auf 0
		lea			MCF_GPIO_PDDR_DSPI,a0
		move.b		#0x7d,(a0)					// din = input rest output
		
		bsr			warten_20ms
		
		move.b		#0x7f,(a1)					// alle auf 1

		bsr			sd_16clk
		bsr			sd_16clk
		bsr			sd_16clk
		bsr			sd_16clk
		bsr			sd_16clk
		bsr			sd_16clk
		bsr			sd_16clk
		bsr			sd_16clk
// sd idle
sd_idle:
		bsr			sd_16clk
		moveq		#0x40,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x95,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x05,d5
		beq			sd_test
		cmp.b		#0x01,d5
		beq			wait_of_aktiv
		cmp.b		#0x04,d5
		beq			sd_init_ok
		cmp.b		#0x00,d5
		beq			sd_init_ok
		bra			sd_idle
				
// acdm 41
wait_of_aktiv:
		bsr			sd_16clk

		moveq		#0x77,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com

		bsr			sd_receive

		bsr			sd_16clk

		move.l		#0xff,d6
		moveq		#0x69,d4
		bsr			sd_com
		and			d5,d6
		moveq		#00,d4
		bsr			sd_com
		and			d5,d6
		moveq		#00,d4
		bsr			sd_com
		and			d5,d6
		moveq		#0x02,d4
		bsr			sd_com
		and			d5,d6
		moveq		#00,d4
		bsr			sd_com
		and			d5,d6
		moveq		#0x01,d4
		bsr			sd_com
		and			d5,d6

		bsr			sd_receive
				
		cmp.b		#0x00,d5
		beq			sd_init_ok
		cmp.b		#0x05,d5
		beq			sd_test	
		bra			wait_of_aktiv

sd_init_ok:

// blockgrösse 512byt
sd_bg:
		bsr			sd_16clk
		moveq		#0x50,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#02,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x00,d5
		bne			sd_bg

// read block
sd_rb:
		bsr			sd_16clk
		moveq		#0x51,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x08,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x00,d5
		bne			sd_rb
		
		lea			0xc00000,a4
		move.l		#513,d7
rd_rb:
		bsr			sd_receive
		move.b		d5,(a4)+
		subq.l		#1,d7
		bne			rd_rb	

// write block
sd_wb:
		bsr			sd_16clk
		moveq		#0x58,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x08,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x00,d5
		bne			sd_wb
		
		lea			0xc00000,a4
		move.l		#513,d7
		moveq.l		#0x66,d4
wr_wb:
		bsr			sd_com
//		subq.l		#1,d4
		moveq		#0x66,d4
		subq.l		#1,d7
		bne			wr_wb	

		bsr			sd_receive
		
wr_wb_el:	
		moveq		#0xff,d4
		bsr			sd_com
		cmp.b		#0xff,d5
		bne			wr_wb_el
		

// read block 2
sd_rb2:
		bsr			sd_16clk
		moveq		#0x51,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x08,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x00,d5
		bne			sd_rb2
		
		lea			0xc00400,a4
		move.l		#513,d7
rd_rb2:
		bsr			sd_receive
		move.b		d5,(a4)+
		subq.l		#1,d7
		bne			rd_rb2	


		nop
		nop

		rts
		
sd_receive:
		moveq		#0xff,d4
		bsr			sd_com
		cmp.b		#0xff,d5
		beq			sd_receive
		rts
		
sd_com:
		bclr.b		#6,(a1)
sd_comb:
		bsr			warten_10us
		moveq		#7,d2
		clr.l		d5
sd_com_loop:
		btst		d2,d4
		beq			sd_com2
		bset.b		#0,(a1)
		bra			sd_com2_1
sd_com2:
		bclr.b		#0,(a1)
sd_com2_1:
		bsr			sd_clk
		and.l		#0x02,d3
		beq			sd_com3
		bset.b		d2,d5
sd_com3:
		subq.l		#1,d2
		bge			sd_com_loop
		bsr			warten_10us
		bset.b		#6,(a1)
		bset.b		#0,(a1)
		bsr			warten_200us
		rts
sd_clk:
		tst.b		0xfffff700
		tst.b		0xfffff700
		bset.b		#2,(a1)
		tst.b		0xfffff700
		tst.b		0xfffff700
		move.b		(a2),d3
		tst.b		0xfffff700
		bclr.b		#2,(a1)
		rts

sd_15clk:
		move		#15,d0
		bra			sd_16clk
sd_16clk:
		moveq		#16,d0
sd_16clk1:
		bsr			sd_clk
		subq.l		#1,d0
		bne			sd_16clk1
		bsr			warten_10us
		rts
// warteschleife ca. 20ms  
warten_20ms:  		
		move.l		a0,-(sp)
		move.l		d6,-(sp) 		
		move.l		d1,-(sp) 		
		move.l		d0,-(sp) 		
  		lea			MCF_SLT0_SCNT,a0
  		move.l		(a0),d0
  		move.l		#700000,d6
  		bra			warten_loop
// warteschleife ca. 200us  
warten_200us: 
		move.l		a0,-(sp)
		move.l		d6,-(sp) 		
		move.l		d1,-(sp) 		
		move.l		d0,-(sp) 		
  		lea			MCF_SLT0_SCNT,a0
  		move.l		(a0),d0
  		move.l		#7000,d6
  		bra			warten_loop
// warteschleife ca. 10us  
warten_10us:  		
		move.l		a0,-(sp)
		move.l		d6,-(sp) 		
		move.l		d1,-(sp) 		
		move.l		d0,-(sp) 		
  		lea			MCF_SLT0_SCNT,a0
  		move.l		(a0),d0
  		move.l		#333,d6
warten_loop:
		move.l		(a0),d1
		sub.l		d0,d1
		add.l		d6,d1
		bpl			warten_loop
		move.l		(sp)+,d0
		move.l		(sp)+,d1
		move.l		(sp)+,d6
		move.l		(sp)+,a0
		rts;
}


/**************************************************/
void asm ide_test(void)
{
		lea			MCF_PAD_PAR_DSPI,a0
		move.w		#0x1fff,(a0)
		lea			MCF_DSPI_DCTAR0,a0
		move.l		#0x38a644e4,(a0)
		lea			MCF_DSPI_DMCR,a0
		move.l		#0x802d3c00,(a0)
		clr.l		MCF_DSPI_DTCR
		bsr			warten_20ms
		lea			MCF_DSPI_DTFR,a0
		lea			MCF_DSPI_DRFR,a1

		moveq		#10,d0		
sd_reset:
		move.l		#0x000100ff,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		subq.l		#1,d0
		bne			sd_reset
		
		moveq		#10,d1
sd_loop1:
		bsr			warten_20ms
		moveq		#-1,d0		
// cmd 0 set to idle
		move.l		#0x00200040,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200000,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200000,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200000,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200000,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200095,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		cmp.w		#0x0001,d0
		beq			sd_loop2
		subq.l		#1,d1
		bne			sd_loop1
		moveq		#10,d1
		bra			sd_test
sd_loop2:
		moveq		#-1,d0		
// cmd 41		
		move.l		#0x00200069,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200000,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200000,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200000,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200000,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		move.l		#0x00200001,(a0)
		bsr			warten_20ms
		and.l		(a1),d0
		tst.w		d0
		bne			sd_loop2

		nop
		nop
/********************************************************************/
#define		cmd_reg		(0x1d)
#define		status_reg	(0x1d)
#define		seccnt		(0x09)

ide_test:
		lea			0xfff00040,a0
		lea			0xc00000,a1
		move.b		#0xec,cmd_reg(a0)		//identify devcie cmd
		bsr			wait_int
		bsr			ds_rx
// read sector normal		
		move.b		#1,seccnt(a0)			// 1 sector
		move.b		#0x20,cmd_reg(a0)		// read cmd
		bsr			wait_int
		bsr			ds_rx

// write testpattern sector
		move.b		#1,seccnt(a0)			// 1 sector
		move.b		#0x30,cmd_reg(a0)		// write cmd
		bsr			drq_wait
// write pattern
		move.l		#256,d0
ide_test_loop3:		
		move.w		#0xa55a,(a0)
		subq.l		#1,d0
		bne			ide_test_loop3
		bsr			wait_int
// read testpattern sector
		move.b		#1,seccnt(a0)			// 1 sector
		move.b		#0x20,cmd_reg(a0)		// read
		bsr			wait_int
		bsr			ds_rx
// sector restauriern
		move.b		#1,seccnt(a0)			// 1 sector
		move.b		#0x30,cmd_reg(a0)		// write
		lea			-0x400(a1),a1			// vorletzer
		bsr			drq_wait
		bsr			ds_tx
		bsr			wait_int
// fertig und zurück		
		nop
		rts
// wait auf int
wait_int:
		move.b		0xfffffa01,d0
		btst.b		#5,d0
		bne			wait_int
		move.b		status_reg(a0),d0
		rts
// wait auf drq
drq_wait:
		move.b		status_reg(a0),d0
		btst		#3,d0
		beq			drq_wait
		rts

// 1 sector lesen word
ds_rx:
		move.l		#256,d0
ds_rx_loop:		
		move.w		(a0),(a1)+
		subq.l		#1,d0
		bne			ds_rx_loop
		rts
// 1 sector lesen long
ds_rxl:
		move.l		#128,d0
ds_rxl_loop:		
		move.l		(a0),(a1)+
		subq.l		#1,d0
		bne			ds_rxl_loop
		rts
// 1 sector schreiben word
ds_tx:
		move.l		#256,d0
ds_tx_loop:		
		move.w		(a1)+,(a0)
		subq.l		#1,d0
		bne			ds_tx_loop
		rts
// 1 sector schreiben word
ds_txl:
		move.l		#128,d0
ds_txl_loop:		
		move.l		(a1)+,(a0)
		subq.l		#1,d0
		bne			ds_txl_loop
		rts
// warteschleife ca. 20ms  
warten_20ms:  		
		move.l		a0,-(sp)
		move.l		d6,-(sp) 		
		move.l		d1,-(sp) 		
		move.l		d0,-(sp) 		
  		lea			MCF_SLT0_SCNT,a0
  		move.l		(a0),d0
  		move.l		#700000,d6
  		bra			warten_loop
// warteschleife ca. 200us  
warten_200us: 
		move.l		a0,-(sp)
		move.l		d6,-(sp) 		
		move.l		d1,-(sp) 		
		move.l		d0,-(sp) 		
  		lea			MCF_SLT0_SCNT,a0
  		move.l		(a0),d0
  		move.l		#7000,d6
  		bra			warten_loop
// warteschleife ca. 10us  
warten_10us:  		
		move.l		a0,-(sp)
		move.l		d6,-(sp) 		
		move.l		d1,-(sp) 		
		move.l		d0,-(sp) 		
  		lea			MCF_SLT0_SCNT,a0
  		move.l		(a0),d0
  		move.l		#333,d6
warten_loop:
		move.l		(a0),d1
		sub.l		d0,d1
		add.l		d6,d1
		bpl			warten_loop
		move.l		(sp)+,d0
		move.l		(sp)+,d1
		move.l		(sp)+,d6
		move.l		(sp)+,a0
		rts;
}
/********************************************************************/
//.include "startcf.h"

//.extern ___MBAR
//#define MCF_SLT0_SCNT	___MBAR+0x908

//.global ide_test

.text
/********************************************************************/
// sd card
/********************************************************************/
sd_test:
		lea			0x40000,a5					// basis addresse
		clr.w		MCF_PAD_PAR_DSPI
		lea			MCF_GPIO_PPDSDR_DSPI,a2		// data in
		lea			MCF_GPIO_PODR_DSPI,a1		// data out
		move.b		#0x00,(a1)					// alle auf 0
		lea			MCF_GPIO_PDDR_DSPI,a0
		move.b		#0x7d,(a0)					// din = input rest output
		
		bsr			warten_20ms
		
		move.b		#0x7f,(a1)					// alle auf 1

		bsr			sd_16clk
		
// sd idle
sd_idle:
		bsr			sd_16clk
		moveq		#0x40,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x95,d4
		bsr			sd_com
		
		bsr			sd_receive
		
		cmp.b		#0x05,d5
		beq			sd_test
		cmp.b		#0x01,d5
		beq			wait_of_aktiv
		cmp.b		#0x04,d5
		beq			sd_init_ok
		cmp.b		#0x00,d5
		beq			sd_init_ok
		bra			sd_idle
				
// acdm 41
wait_of_aktiv:
		bsr			sd_16clk

		moveq		#0x77,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com

		bsr			sd_receive

		bsr			sd_16clk

		move.l		#0xff,d6
		moveq		#0x69,d4
		bsr			sd_com
		and			d5,d6
		moveq		#00,d4
		bsr			sd_com
		and			d5,d6
		moveq		#00,d4
		bsr			sd_com
		and			d5,d6
		moveq		#0x02,d4
		bsr			sd_com
		and			d5,d6
		moveq		#00,d4
		bsr			sd_com
		and			d5,d6
		moveq		#0x01,d4
		bsr			sd_com
		and			d5,d6

		bsr			sd_receive
				
		cmp.b		#0x00,d5
		beq			sd_init_ok
		cmp.b		#0x05,d5
		beq			sd_test	
		bra			wait_of_aktiv

sd_init_ok:

// blockgrösse 512byt
sd_bg:
		bsr			sd_16clk
		moveq		#0x50,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#02,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x00,d5
		bne			sd_bg

// read block
sd_rb:
		bsr			sd_16clk
		moveq		#0x51,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x08,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x00,d5
		bne			sd_rb
		
		move.l		a5,a4
		move.l		#513,d7
rd_rb:
		bsr			sd_receive
		move.b		d5,(a4)+
		subq.l		#1,d7
		bne			rd_rb	
		
// write block
sd_wb:
		bsr			sd_16clk
		moveq		#0x58,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x08,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x00,d5
		bne			sd_wb
		
		move.l		#513,d7
		moveq.l		#0xbb,d4
wr_wb:
		bsr			sd_com
//		subq.l		#1,d4
		moveq		#0xbb,d4
		subq.l		#1,d7
		bne			wr_wb	

		bsr			sd_receive
		
wr_wb_el:	
		moveq		#0xff,d4
		bsr			sd_com
		cmp.b		#0xff,d5
		bne			wr_wb_el
		

// read block 2
sd_rb2:
		bsr			sd_16clk
		moveq		#0x51,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x08,d4
		bsr			sd_com
		moveq		#00,d4
		bsr			sd_com
		moveq		#0x01,d4
		bsr			sd_com
		
		bsr			sd_receive

		cmp.b		#0x00,d5
		bne			sd_rb2
		
		move.l		#513,d7
rd_rb2:
		bsr			sd_receive
		move.b		d5,(a4)+
		subq.l		#1,d7
		bne			rd_rb2	

halt
halt
		nop
		nop

		rts
		
sd_receive:
		moveq		#0xff,d4
		bsr			sd_com
		cmp.b		#0xff,d5
		beq			sd_receive
		rts
		
sd_com:
		bclr.b		#6,(a1)			// nCS=0
sd_comb:
		bsr			warten_10us
		moveq		#7,d2
		clr.l		d5
sd_com_loop:
		bclr.b		#0,(a1)			// default 0 bit senden
		btst		d2,d4			// ist 0?
		beq			sd_com2			// ja->
		bset.b		#0,(a1)			// sonst auf 1
sd_com2:
		bsr			sd_clk			// clocken
		and.l		#0x02,d3
		beq			sd_com3
		bset.b		d2,d5
sd_com3:
		subq.l		#1,d2
		bge			sd_com_loop
		bsr			warten_10us
		bset.b		#6,(a1)			// nCS=1
		bset.b		#0,(a1)			// daten = 0
		bsr			warten_200us
		rts
// clocken
sd_clk:
		tst.b		0xfffff700		// wait
		tst.b		0xfffff700
		bset.b		#2,(a1)			// clock high
		tst.b		0xfffff700		// wait
		tst.b		0xfffff700		// wait
		move.b		(a2),d3			// bit holen
		bclr.b		#2,(a1)			// clock low
		rts

sd_16clk:
		move.l		#160,d0
sd_16clk1:
		bsr			sd_clk
		subq.l		#1,d0
		bne			sd_16clk1
		bsr			warten_10us
		rts
/********************************************************************/
// video pll
/********************************************************************/
		lea		0xf0000800,a1

		lea		0xf0000600,a0
		move.l	#128,d0
clr_pll:
		bsr		wait_pll
		clr.w	(a0)
		addq.l	#4,a0
		subq.l	#1,d0
		bne		clr_pll
		
		lea		0xf0000600,a0
		bsr		wait_pll
		move.w	#27,0x48(a0)		// loopfilter r
		bsr		wait_pll
		move.w	#1,0x08(a0)			// charge pump I
		bsr		wait_pll
		move.w	#12,0x0(a0)			// N counter high = 12
		bsr		wait_pll
		move.w	#12,0x40(a0)		// N counter low = 12
		bsr		wait_pll
		move.w	#1,0x114(a0)		// ck1 bypass
		bsr		wait_pll
		move.w	#1,0x118(a0)		// ck2 bypass
		bsr		wait_pll
		move.w	#1,0x11c(a0)		// ck3 bypass
		bsr		wait_pll
		move.w	#1,0x10(a0)			// ck0 high = 1
		bsr		wait_pll
		move.w	#1,0x50(a0)			// ck0 low	= 1
		
		bsr		wait_pll
		move.w	#1,0x144(a0)		// M odd division
		bsr		wait_pll
		move.w	#1,0x44(a0)			// M low = 1

		bsr		wait_pll
		move.w	#99,0x04(a0)		// M high = 100

		bsr		wait_pll
		clr.b	(a1)				// set

set_pll:
		bsr		read_pll
		halt
		move.w	d0,(a0)
		bsr		wait_pll
		clr.b	(a1)
		bra		set_pll

read_pll:
		lea		0xf0000600,a3
		lea		0x10000,a2
		move.l	#128,d3
read1_pll:
		bsr		wait_pll
		move.w	(a3),d1
		bsr		wait_pll
		move.w	(a3),(a2)
		addq.l	#4,a3
		addq.l	#4,a2
		subq.l	#1,d3
		bne		read1_pll
		rts
wait_pll:
		tst.w	(a1)
		bmi		wait_pll
		rts

/********************************************************************/
void ide_test(void)
/********************************************************************/
{
	asm
	{
		halt
		lea			0xfff00000,a0
		lea			0x80000,a1
		move.b		#0xec,cmd_reg(a0)		//identify devcie cmd
		bsr			wait_int
		bsr			ds_rxl
// read sector normal		
		move.b		#1,seccnt(a0)			// 1 sector 0x200
		move.b		#0x20,cmd_reg(a0)		// read cmd
		bsr			wait_int
		bsr			ds_rxl
// write pattern 0x400
		move.l		a1,a4	//a4=400
		move.l		#256,d0
		clr.l		d1
ide_test_loop3:		
		move.w		d1,(a1)+
		addq.l		#1,d1
		subq.l		#1,d0
		bne			ide_test_loop3
		move.l		a4,a1
// write testpattern sector 0x400
		move.b		#10,sector(a0)			// sector 10
		move.b		#1,seccnt(a0)			// 1 sector
		move.b		#0x30,cmd_reg(a0)		// write cmd
		bsr			drq_wait
		bsr			ds_txl
		bsr			wait_int
		move.l		a1,a2	//a2=600
		move.l		#256,d4
loop_rw:
// read testpattern sector 0x600
		move.l		a2,a1
//		halt
		move.b		#10,sector(a0)			// sector 10
		move.b		#1,seccnt(a0)			// 1 sector
		move.b		#0x20,cmd_reg(a0)		// read
		bsr			wait_int
		bsr			ds_rxl
// verändere testpattern
		move.l		a2,a1
//		halt
		move.l		#256,d0
		move.l		#0x100,d1
verae_loop:
		move.w		(a1),d2
		add.l		d1,d2
		move.w		d2,(a1)+
		subq.l		#1,d0
		bne			verae_loop
// write testpattern sector 0x600
		move.l		a2,a1
//		halt
		move.b		#10,sector(a0)			// sector 10
		move.b		#1,seccnt(a0)			// 1 sector
		move.b		#0x30,cmd_reg(a0)		// write cmd
		bsr			drq_wait
		bsr			ds_txl
		bsr			wait_int
		subq.l		#1,d4
//		bra			verae_loop
		bne			loop_rw
// sector vergleichen
//		halt
		move.l		#128,d0
		moveq.l		#-1,d1
verg_loop:
		move.l		(a2)+,d2
		cmp.l		(a4)+,d2
		bne			error
		subq.l		#1,d0
		bne			verg_loop
		clr.l		d1
error:
		halt
		rts
// sector restauriern
		move.b		#1,seccnt(a0)			// 1 sector
		move.b		#0x30,cmd_reg(a0)		// write
		lea			-0x400(a1),a1			// vorletzer
		bsr			drq_wait
		bsr			ds_txl
		
		bsr			wait_int
// fertig und zurück		
		nop
		rts
// wait auf int
wait_int:
		move.b		0xfffffa01,d0
		btst		#5,d0
		bne			wait_int
		move.b		status_reg(a0),d0
		rts
// wait auf drq
drq_wait:
		move.b		status_reg(a0),d0
		btst		#3,d0
		beq			drq_wait
		rts

// 1 sector lesen word
ds_rx:
		move.l		#256,d0
ds_rx_loop:		
		move.w		(a0),(a1)+
		subq.l		#1,d0
		bne			ds_rx_loop
		rts
// 1 sector lesen long
ds_rxl:
		move.l		#128,d0
ds_rxl_loop:		
		move.l		(a0),(a1)+
		subq.l		#1,d0
		bne			ds_rxl_loop
		rts
// 1 sector schreiben word
ds_tx:
		move.l		#256,d0
ds_tx_loop:		
		move.w		(a1)+,(a0)
		subq.l		#1,d0
		bne			ds_tx_loop
		rts
// 1 sector schreiben long
ds_txl:
		move.l		#128,d0
ds_txl_loop:		
		move.l		(a1)+,(a0)
		subq.l		#1,d0
		bne			ds_txl_loop
		rts
	}
}
/********************************************************************/
