#ifndef _M5484L_H_
#define _M5484L_H_

#define BOOTFLASH_BASE_ADDRESS	0xE0000000
#define BOOTFLASH_SIZE			0x400000				/* LITEKIT has 4MB flash */
#define BOOTFLASH_BAM			(BOOTFLASH_SIZE - 1)

#define SDRAM_START             0x00000000  /* start at address 0 */
#define SDRAM_SIZE              0x4000000  	/* 64 MB on the LITEKIT */

#endif /* _M5484L_H_ */
