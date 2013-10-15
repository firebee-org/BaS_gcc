/*----------------------------------------------------------------------*/
/* FatFs sample project for generic microcontrollers (C)ChaN, 2012      */
/*----------------------------------------------------------------------*/

#include <stdio.h>
#include "ff.h"


FATFS Fatfs;		/* File system object */
FIL Fil;			/* File object */
uint8_t Buff[128];		/* File read buffer */


void die (		/* Stop with dying message */
	FRESULT rc	/* FatFs return value */
)
{
	xprintf("Failed with rc=%u.\n", rc);
	for (;;) ;
}


/*-----------------------------------------------------------------------*/
/* Program Main                                                          */
/*-----------------------------------------------------------------------*/

int main (void)
{
	FRESULT rc;				/* Result code */
	DIR dir;				/* Directory object */
	FILINFO fno;			/* File information object */
	uint32_t bw, br, i;


	f_mount(0, &Fatfs);		/* Register volume work area (never fails) */

	xprintf("\nOpen an existing file (message.txt).\n");
	rc = f_open(&Fil, "MESSAGE.TXT", FA_READ);
	if (rc) die(rc);

	xprintf("\nType the file content.\n");
	for (;;) {
		rc = f_read(&Fil, Buff, sizeof Buff, &br);	/* Read a chunk of file */
		if (rc || !br) break;			/* Error or end of file */
		for (i = 0; i < br; i++)		/* Type the data */
			putchar(Buff[i]);
	}
	if (rc) die(rc);

	xprintf("\nClose the file.\n");
	rc = f_close(&Fil);
	if (rc) die(rc);

	xprintf("\nCreate a new file (hello.txt).\n");
	rc = f_open(&Fil, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
	if (rc) die(rc);

	xprintf("\nWrite a text data. (Hello world!)\n");
	rc = f_write(&Fil, "Hello world!\r\n", 14, &bw);
	if (rc) die(rc);
	xprintf("%u bytes written.\n", bw);

	xprintf("\nClose the file.\n");
	rc = f_close(&Fil);
	if (rc) die(rc);

	xprintf("\nOpen root directory.\n");
	rc = f_opendir(&dir, "");
	if (rc) die(rc);

	xprintf("\nDirectory listing...\n");
	for (;;) {
		rc = f_readdir(&dir, &fno);		/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		if (fno.fattrib & AM_DIR)
			xprintf("   <dir>  %s\n", fno.fname);
		else
			xprintf("%8lu  %s\n", fno.fsize, fno.fname);
	}
	if (rc) die(rc);

	xprintf("\nTest completed.\n");
	for (;;) ;
}



/*---------------------------------------------------------*/
/* User Provided Timer Function for FatFs module           */
/*---------------------------------------------------------*/

uint32_t get_fattime (void)
{
	return	  ((uint32_t)(2012 - 1980) << 25)	/* Year = 2012 */
			| ((uint32_t)1 << 21)				/* Month = 1 */
			| ((uint32_t)1 << 16)				/* Day_m = 1*/
			| ((uint32_t)0 << 11)				/* Hour = 0 */
			| ((uint32_t)0 << 5)				/* Min = 0 */
			| ((uint32_t)0 >> 1);				/* Sec = 0 */
}
