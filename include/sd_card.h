/*
 * sd_card.h
 *
 * Exported sd-card access routines for the FireBee BaS
 *
 *  Created on: 19.11.2012
 *      Author: mfro
 */

#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include <MCF5475.h>

extern int spi_init(void);
extern uint32_t sd_com(uint32_t data);
extern void sd_card_idle(void);
extern uint8_t sd_card_get_status(void);
extern uint8_t spi_send_byte(uint8_t byte);
extern uint16_t spi_send_word(uint16_t word);

#endif /* _SD_CARD_H_ */
