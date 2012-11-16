

//.include "startcf.h"

//.extern ___MBAR
//#define MCF_SLT0_SCNT	___MBAR+0x908

//.global ide_test

.text
/*
sd_test:
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
/********************************************************************/
