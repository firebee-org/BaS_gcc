/*
 * basflash_start.c
 *
 *  Created on: 16.02.2013
 *      Author: mfro
 */


void startup(void)
{
	void basflash(void);
	void init_serial(void);

	init_serial();
	basflash();
}
