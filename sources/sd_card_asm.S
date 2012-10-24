/********************************************************************/
// sd card
/********************************************************************/
#define		dspi_dtar0	0x0c
#define		dspi_dsr	0x2c
#define		dspi_dtfr	0x34
#define		dspi_drfr	0x38

#define LONGASC(a, b, c, d) ((a << 24) | (b << 16) | (c << 8) | (d))

#define MCF_PAD_PAR_DSPI	(__MBAR+0xA50)
#define MCF_PSC0_PSCTB_8BIT (__MBAR+0x860C)
#define MCF_DSPI_DMCR (__MBAR+0x8A00)
#define MCF_SLT0_SCNT (__MBAR + 0x908)

		.text

warte_10ms:
		move.l		d0,-(sp)
		move.l		MCF_SLT0_SCNT,d0
		sub.l		#1320000,d0
warte_d6:
		cmp.l		MCF_SLT0_SCNT,d0
		bcs			warte_d6
		move.l		(sp)+,d0
		rts

warte_1ms:
		move.l		d0,-(sp)
		move.l		MCF_SLT0_SCNT,d0
		sub.l		#132000,d0
warte_d5:
		cmp.l		MCF_SLT0_SCNT,d0
		bcs			warte_d5
		move.l		(sp)+,d0
		rts

		.global		sd_idle
		.global		sd_init

sd_init:
		lea			MCF_PSC0_PSCTB_8BIT,a6
		move.l   	#LONGASC('S', 'D', '-', 'C'),(a6)
		move.l		#LONGASC('a', 'r', 'd', ' '),(a6)

		move.l		buffer,a5				// basis addresse (diesen bereich brauchen wir nicht mehr!)
		move.l		#0x1fffffff,d0				// normal dspi
		move.l		d0,MCF_PAD_PAR_DSPI
		lea			MCF_DSPI_DMCR,a0
		move.l		#0x800d3c00,(a0)			// 8 bit cs5 on
		move.l		#0x38558897,d0				
		move.l		d0,dspi_dtar0(a0)			// 400kHz 
		move.l		#0x082000ff,d4				// tx vorbesetzen
		mov3q.l		#-1,dspi_dsr(a0)
		
		bsr.l		warte_1ms
		move.l		#0xc00d3c00,(a0)			// 8 bit 4MHz clocken cs off
		bsr.l		warte_10ms
		move.l		#0x800d3c00,(a0)			// 8 bit 4MHz normal cs on
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		move.l		#0x802d3c00,(a0)			// 8 bit 4MHz normal cs off
		clr.b		d4
		bsr			sd_com
		bsr			sd_com
		move.l		#0x800d3c00,(a0)			// 8 bit 4MHz normal cs on
		move.b		#0xff,d4
		bsr			sd_com
		bsr			sd_com
		move.l		#0x802d3c00,(a0)			// 8 bit 4MHz normal cs off
		bsr.l		warte_10ms

// sd idle
		move.l		#100,d6				// 100 versuche
		move.l		#10,d3				// 10 versuche
sd_idle:
		move.b		#0xff,d4			// receive byt
		bsr			sd_com
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
		
		move.b		#0xff,d4			// receive byt
		bsr			sd_com
		cmp.b		#0x01,d5
		beq			idle_end
		bsr			sd_com
		cmp.b		#0x01,d5
		beq			idle_end
		bsr			sd_com
		cmp.b		#0x01,d5
		beq			idle_end
		bsr			sd_com
		cmp.b		#0x01,d5
		beq			idle_end
		bsr			sd_com
		cmp.b		#0x01,d5
		beq			idle_end
		bsr			sd_com
		cmp.b		#0x01,d5
		beq			idle_end
		subq.l		#1,d6
		beq			sd_not
		bra			sd_idle
idle_end:
// cdm 8
read_ic:
		move.b		#0xff,d4			// receive byt
		bsr			sd_com
		move.b		#0x48,d4
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
		cmp.b		#5,d5
		beq			sd_v1
		cmp.b		#1,d5
		bne			read_ic
		
		move.b		#0xff,d4
		bsr			sd_com
		move.b		d5,d0
		bsr			sd_com
		move.b		d5,d1
		bsr			sd_com
		move.b		d5,d2
		bsr			sd_com
		cmp.b		#0xaa,d5
		bne			sd_testd3
		move.l		#LONGASC('S', 'D', 'H', 'C'),(a6)
		move.b		#' ',(a6)
sd_v1:
		
// cdm 58
read_ocr:
		move.b		#0xff,d4			// receive byt
		bsr			sd_com
		move.b		#0x7a,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com
		
		bsr			sd_get_status
		move.l		#LONGASC('V', 'e', 'r', '1'),d6
		cmp.b		#5,d5
		beq			read_ocr
		cmp.b		#1,d5
		bne			read_ocr
		
		move.b		#0xff,d4
		bsr			sd_com
		move.b		d5,d0
		bsr			sd_com
		move.b		d5,d1
		bsr			sd_com
		move.b		d5,d2
		bsr			sd_com

// acdm 41
		move.l		#20000,d6			// 20000 versuche ready can bis 1 sec gehen
wait_of_aktiv:
		move.b		#0xff,d4			// receive byt
		bsr			sd_com
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
		move.b		#0x95,d4
		bsr			sd_com

		bsr			sd_get_status
		cmp.b		#0x05,d5
		beq			wait_of_aktiv

wait_of_aktiv2:
		move.b		#0xff,d4			// receive byt
		bsr			sd_com
		move.b		#0x69,d4
		bsr			sd_com
		move.b		#0x40,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x95,d4
		bsr			sd_com

		bsr			sd_get_status
		tst.b		d5
		beq			sd_init_ok
		cmp.b		#0x05,d5
		beq			wait_of_aktiv2	
		subq.l		#1,d6
		bne			wait_of_aktiv
sd_testd3:
		subq.l		#1,d3
		bne			sd_idle
		bra			sd_error

sd_init_ok:
// cdm 10
read_cid:
		move.b		#0xff,d4			// receive byt
		bsr			sd_com
		move.b		#0x4a,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x95,d4
		bsr			sd_com
		
		move.l		a5,a4				// adresse setzen
		bsr			sd_rcv_info

// name ausgeben
		lea			1(a5),a4
		moveq		#7,d7
sd_nam_loop:
		move.b		(a4)+,(a6)
		subq.l		#1,d7
		bne			sd_nam_loop
		move.b		#' ',(a6)

// cdm 9
read_csd:
		move.b		#0xff,d4			// receive byt
		bsr			sd_com
		move.b		#0x49,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#00,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x00,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com
		
		move.l		a5,a4				// adresse setzen
		bsr			sd_rcv_info

		mvz.b		(a5),d0
		lsr.l		#6,d0
		
		bne			sd_csd2				// format v2
		move.l		6(a5),d1
		moveq		#14,d0				// bit 73..62 c_size
		lsr.l		d0,d1				// bits extrahieren
		and.l		#0xfff,d1			// 12 bits
		addq.l		#1,d1
		mvz.w		9(a5),d0
		lsr.l		#7,d0				// bits 49..47
		and.l		#0x7,d0				// 3 bits
		moveq.l		#8,d2				// x256 (dif v1 v2)
		sub.l		d0,d2		
		lsr.l		d2,d1
		bra			sd_print_size
sd_csd2:
		mvz.w		8(a5),d1
		addq.l		#1,d1
sd_print_size:		
		swap		d1
		lsl.l		#1,d1
		bcc			sd_16G
		move.l		#LONGASC('3', '2', 'G', 'B'),(a6)
		bra			sd_ok
sd_16G:
		lsl.l		#1,d1
		bcc			sd_8G
		move.l		#LONGASC('1', '6', 'G', 'B'),(a6)
		bra			sd_ok
sd_8G:
		lsl.l		#1,d1
		bcc			sd_4G
		move.l		#LONGASC(' ', '4', 'G', 'B'),(a6)
		bra			sd_ok
sd_4G:
		lsl.l		#1,d1
		bcc			sd_2G
		move.l		#LONGASC(' ', '4', 'G', 'B'),(a6)
		bra			sd_ok
sd_2G:
		lsl.l		#1,d1
		bcc			sd_1G
		move.l		#LONGASC(' ', '2', 'G', 'B'),(a6)
		bra			sd_ok
sd_1G:
		lsl.l		#1,d1
		bcc			sd_512M
		move.l		#LONGASC(' ', '1', 'G', 'B'),(a6)
		bra			sd_ok
sd_512M:
		lsl.l		#1,d1
		bcc			sd_256M
		move.b		#'5',(a6)
		move.l		#LONGASC('1', '2', 'M', 'B'),(a6)
		bra			sd_ok
sd_256M:
		lsl.l		#1,d1
		bcc			sd_128M
		move.b		#'2',(a6)
		move.l		#LONGASC('5', '6', 'M', 'B'),(a6)
		bra			sd_ok
sd_128M:
		lsl.l		#1,d1
		bcc			sd_64M
		move.b		#'1',(a6)
		move.l		#LONGASC('2', '8', 'M', 'B'),(a6)
		bra			sd_ok
sd_64M:
		lsl.l		#1,d1
		bcc			sd_32M
		move.l		#LONGASC('6', '4', 'M', 'B'),(a6)
		bra			sd_ok
sd_32M:
		lsl.l		#1,d1
		bcc			sd_16M
		move.l		#LONGASC('3', '2', 'M', 'B'),(a6)
		bra			sd_ok
sd_16M:
		lsl.l		#1,d1
		bcc			sd_8M
		move.l		#LONGASC('1', '6', 'M', 'B'),(a6)
		bra			sd_ok
sd_8M:
		move.l		#LONGASC('<', '9', 'M', 'B'),(a6)
sd_ok:
		move.l		#LONGASC(' ', 'O', 'K', '!'),(a6)
		move.l		#0x0a0d,(a6)
		halt
		halt
		rts
// subs ende -------------------------------
sd_V1:
		move.l   #LONGASC('n', 'o', 'n', '!'),(a6)
		move.l	#0x0a0d,(a6)
		halt
		halt
		rts
sd_error:
		move.l  #LONGASC('E', 'r', 'r', 'o'),(a0)
		move.l	#LONGASC('r', '!', '', ''), (a0)
		move.l	#0x0a0d,(a6)
		halt
		halt
		rts
sd_not:
		move.l	#LONGASC('n', 'o', 'n', '!'),(a0)
		move.l	#0x0a0d,(a6)
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
		mov3q.l		#-1,dspi_dsr(a0)			// clr status register
		rts

// daten holen ----------------------------
sd_rcv_info:		
		moveq		#18,d3						// 16 byts + 2 byts crc
		move.b		#0xff,d4
sd_rcv_rb_w:		
		bsr			sd_get_status 
		cmp.b		#0xfe,d5					// daten bereit?
		bne			sd_rcv_rb_w					// nein->
sd_rcv_rd_rb:
		bsr			sd_com
		move.b		d5,(a4)+
		subq.l		#1,d3
		bne			sd_rcv_rd_rb
		rts
/******************************************/

			.data
buffer:		dc.l	0, 0, 0, 0, 0, 0, 0, 0
