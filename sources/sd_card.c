/*
 * sd card
 */

#include <stdint.h>
#include <MCF5475.h>

#define		dspi_dtar0	0x0c
#define		dspi_dsr	0x2c
#define		dspi_dtfr	0x34
#define		dspi_drfr	0x38
#define		time1us		1320

extern void wait_10ms(void);


uint8_t sd_com(uint32_t cmd)
{
	uint8_t res;

	MCF_DSPI_DTFR = cmd;
	while (! (MCF_DSPI_DSR & (1 << 7)));

	res = MCF_DSPI_DRFR;
	MCF_DSPI_DSR = -1L;

	return res;
}

/*
 * fetch status from SD controller
 */
uint8_t sd_get_status(void)
{
	uint8_t res;

	while ((res = sd_com(0xff)) == 0xff);

	return res;
}

void sd_rcv_info(uint8_t *buf, uint32_t size)
{
	uint32_t rcvd = 0;

	while (sd_get_status() != 0xfe);	/* loop until data available */

	do {
		*buf++ = sd_com(0x18);
		rcvd++;
	} while (rcvd <= size);
}


void sd_card_idle(void)
{

#ifdef _NOT_USED_
	asm
	{
// sd idle
// speed =400kHz
		move.l		#0x082000ff,d4			// tx vorbesetzen
		lea			MCF_DSPI_DMCR,a0
		move.l		#0x38558897,d0				
		move.l		d0,dspi_dtar0(a0)		// 400kHz 

		move.b		#0xff,d4
		bsr			sd_com					// clocks 
		move.b		#0x40,d4				// cmd idle
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
	}
#endif
}



int sd_card_init(void)
{
	return 0; /* for now, just to make the compiler happy */
#ifdef _NOT_USED_
	long		az_sectors;

	asm
	{
		lea			MCF_PSC0_PSCTB_8BIT,a1
		move.l  	#'SD-C',(a1)
		move.l  	#'ard ',(a1)

		move.l		buffer,a5					// basis addresse (diesen bereich brauchen wir nicht mehr!)
		move.l		#0x1fffffff,d0				// normal dspi
		move.l		d0,MCF_PAD_PAR_DSPI
		lea			MCF_DSPI_DMCR,a0
		move.l		#0x802d3c00,(a0)			// 8 bit cs off clear fifo
		move.l		#0x38558897,d0				
		move.l		d0,dspi_dtar0(a0)			// 400kHz 
		move.l		#0x082000ff,d4				// tx vorbesetzen
		move.l		d4,dspi_dtfr				// und setzen
		mov3q.l		#-1,dspi_dsr(a0)			// status register l�schen
		
		move.l		#0xc00d3c00,(a0)			// clock on cs ist on
		bsr			wait_10ms
		move.l		#0x802d3c00,(a0)			// clock off cs off
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
		move.l		#0x800d3c00,(a0)			// cs on
		bsr			sd_com						
		bsr			sd_com
		move.l		#0x802d3c00,(a0)			// cs off
		bsr			sd_com
		bsr			sd_com
		bsr			wait_10ms

// sd idle
		move.l		#100,d6						// 100 versuche
		move.l		#10,d3						// 10 versuche
sd_idle:
		bsr			sd_card_idle

		move.l		#10,d7
		move.b		#0xff,d4
sd_idle_leeren:
		bsr			sd_com
		cmp.b		#0x01,d5
		beq			idle_end
		subq.l		#1,d7
		bne			sd_idle_leeren
		subq.l		#1,d6
		beq			sd_not
		bra			sd_idle
idle_end:

// cdm 8
read_ic:
		move.b		#0xff,d4			// clocks 
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
		
		bsr			sd_com			// 4byts zum wegwerfen
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com
		cmp.b		#0xaa,d5		// pattern zur�ckgekommen?
		bne			sd_testd3		// nein ->

		move.l		#'SDHC',(a1)
		move.b		#' ',(a1)
sd_v1:
		
// cdm 58
read_ocr:
		move.b		#0xff,d4			// clocks 
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
		move.l		#'Ver1',d6
		cmp.b		#5,d5
		beq			read_ocr
		cmp.b		#1,d5
		bne			read_ocr
		
		bsr			sd_com				// 4 byts zum wegwerfen
		bsr			sd_com
		bsr			sd_com
		bsr			sd_com

// acdm 41
		move.l		#20000,d6			// 20000 versuche ready can bis 1 sec gehen
wait_of_aktiv:
		move.b		#0xff,d4			// clocks 
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
		move.b		#0xff,d4			// clocks 
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
// fullspeed 
		move.l		#0x38551120,d0				// 22Mbit/sec
		move.l		d0,dspi_dtar0(a0)			// setzen 

// cdm 10
read_cid:
		move.b		#0xff,d4			// clocks 
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
		
		move.l		a5,a2				// adresse setzen
		bsr			sd_rcv_info

// name ausgeben
		lea			1(a5),a2
		moveq		#7,d7
sd_nam_loop:
		move.b		(a2)+,(a1)
		subq.l		#1,d7
		bne			sd_nam_loop
		move.b		#' ',(a1)

// cdm 9
read_csd:
		move.b		#0xff,d4			// clocks 
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
		
		move.l		a5,a2				// adresse setzen
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
		move.l		d1,d3
		lsr.l		#6,d3				//x65636 /64 -> anzahl sectors
		move.l		d3,az_sectors
		lsl.l		#1,d1
		bcc			sd_16G
		move.l		#'32GB',(a1)
		bra			sd_ok
sd_16G:
		lsl.l		#1,d1
		bcc			sd_8G
		move.l		#'16GB',(a1)
		bra			sd_ok
sd_8G:
		lsl.l		#1,d1
		bcc			sd_4G
		move.l		#' 8GB',(a1)
		bra			sd_ok
sd_4G:
		lsl.l		#1,d1
		bcc			sd_2G
		move.l		#' 4GB',(a1)
		bra			sd_ok
sd_2G:
		lsl.l		#1,d1
		bcc			sd_1G
		move.l		#' 2GB',(a1)
		bra			sd_ok
sd_1G:
		lsl.l		#1,d1
		bcc			sd_512M
		move.l		#' 1GB',(a1)
		bra			sd_ok
sd_512M:
		lsl.l		#1,d1
		bcc			sd_256M
		move.b		#'5',(a1)
		move.l		#'12MB',(a1)
		bra			sd_ok
sd_256M:
		lsl.l		#1,d1
		bcc			sd_128M
		move.b		#'2',(a1)
		move.l		#'56MB',(a1)
		bra			sd_ok
sd_128M:
		lsl.l		#1,d1
		bcc			sd_64M
		move.b		#'1',(a1)
		move.l		#'28MB',(a1)
		bra			sd_ok
sd_64M:
		lsl.l		#1,d1
		bcc			sd_32M
		move.l		#'64MB',(a1)
		bra			sd_ok
sd_32M:
		lsl.l		#1,d1
		bcc			sd_16M
		move.l		#'32MB',(a1)
		bra			sd_ok
sd_16M:
		lsl.l		#1,d1
		bcc			sd_8M
		move.l		#'16MB',(a1)
		bra			sd_ok
sd_8M:
		move.l		#'<9MB',(a1)
sd_ok:		
		move.l  	#' OK!',(a1)
		move.l		#0x0a0d,(a1)
		bra			sd_c_ok
// subs ende -------------------------------
sd_error:
		move.l  	#'Erro',(a1)
		move.l  	#'r!',(a1)
		move.l		#0x0a0d,(a1)
		bra			sd_c_error
sd_not:
		move.l  	#'non!',(a1)
		move.l		#0x0a0d,(a1)
		bra			sd_c_not
buffer:	dc.l		0,0,0,0,0,0,0,0
	}
sd_c_ok:
		return		az_sectors;
sd_c_not:
		return		-2;
sd_c_error:
		return		-1;

#endif /* _NOT_USED_ */
}

#ifdef _NOT_USED_
void sd_rcv_sector(void)	 // 1 sector daten holen ----------------------------
{
	if (sd_get_status() == 0xfe)
	{
		for (i = 0; i < 512; i++)
	}
	}
	asm
	{
		bsr			sd_get_status 
		cmp.b		#0xfe,d5				// daten bereit?
		bne			sd_rs_end				// nein-> error
		move.l		#512,d3					// sonst 512 byts abholen
sd_rs_loop:
		bsr			sd_com
		move.b		d5,(a2)+
		subq.l		#1,d3
		bne			sd_rs_loop
// crc holen
		bsr			sd_com
		bsr			sd_com
		clr.l		d5						// alles ok
sd_rs_end:
	}
}

int sd_card_sector_read(long sec_nr,long buf_adr)
{
		int			status ;
	asm
	{
		lea			MCF_DSPI_DMCR,a0
		move.l		#0x082000ff,d4				// tx vorbesetzen

		move.l		sec_nr,d0
		move.l		buf_adr,a2

		lsl.l		#8,d0
		add.l		d0,d0						// x 512 !
		move.l		d0,d1						// byts kehren
		swap		d1
		move.l		d1,d2
		lsr.l		#8,d1
		
		move.b		#0xff,d4			// clocks 
		bsr			sd_com
		
		move.b		#0x51,d4
		bsr			sd_com
		move.b		d1,d4
		bsr			sd_com
		move.b		d2,d4
		bsr			sd_com
		move.l		d0,d2
		lsr.l		#8,d2
		move.b		d2,d4
		bsr			sd_com
		move.b		d0,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com

		clr.l		d5					// alles auf no error
		clr.l		status
		
		bsr			sd_get_status		// status holen
		tst.b		d5
		bne			sd_csr_end			// wenn nicht ok -> weg
// sector holen
		bsr			sd_rcv_sector
sd_csr_end:
		tst.b		d5
		beq			sd_csr_ok
		neg.l		d5					// wenn nicht ok status auf negativ
		move.l		d5,status
sd_csr_ok:
	}
	return	status;
}

void sd_send_sector(void)	 // 1 sector daten senden ----------------------------
{
	asm
	{
		move.l		#512,d3		
		move.b		#0xfe,d4			// start token
		bsr			sd_com				// senden
sd_send_wr_wb:
		move.b		(a2)+,d4			// data
		bsr			sd_com				// senden
		subq.l		#1,d3
		bne			sd_send_wr_wb	
// send crc
		move.b		#1,d4			
		bsr			sd_com				// crc 1.byt
		move.b		#1,d4			
		bsr			sd_com				// crc 2.byt
sd_send_wr_ww:
		bsr			sd_get_status
		and.l		#0x1f,d5
		clr.l		d6					//status auf OK
		cmp.b		#5,d5				//data accepted?
		beq			sd_send_end			//ja ->
		move.l		d5,d6				//sonst status sichern
sd_send_end:
		bsr			sd_com
		tst.b		d5					// warte auf geschrieben
		beq			sd_send_end
		move.l		d6,d5				// status zur�ck
	}
}

int sd_card_sector_write(long sec_nr,long buf_adr)
{
		int			status;
	asm
	{
		lea			MCF_DSPI_DMCR,a0
		move.l		#0x082000ff,d4				// tx vorbesetzen

		move.l		sec_nr,d0
		move.l		buf_adr,a2

		lsl.l		#8,d0
		add.l		d0,d0						// x 512 !
		move.l		d0,d1						// byts kehren
		swap		d1
		move.l		d1,d2
		lsr.l		#8,d1
		
		move.b		#0xff,d4			// clocks 
		bsr			sd_com
		move.b		#0x58,d4
		bsr			sd_com
		move.b		d1,d4
		bsr			sd_com
		move.b		d2,d4
		bsr			sd_com
		move.l		d0,d2
		lsr.l		#8,d2
		move.b		d2,d4
		bsr			sd_com
		move.b		d0,d4
		bsr			sd_com
		move.b		#0x01,d4
		bsr			sd_com

		clr.l		d5					// alles auf no error
		clr.l		status
		bsr			sd_get_status		// status holen
		tst.b		d5
		bne			sd_csw_end			// wenn nicht ok -> weg
// sector schreiben
		bsr			sd_send_sector
sd_csw_end:
		tst.b		d5
		beq			sd_csw_ok
		neg.l		d5					// wenn nicht ok status auf negativ
		move.l		d5,status
sd_csw_ok:
	}
	return	status;
}
#endif /* _NOT_USED */
