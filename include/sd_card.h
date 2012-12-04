/*
 * sd_card.h
 *
 * Exported sd-card access routines for the FireBee BaS
 *
 *  Created on: 19.11.2012
 *      Author: mfro
 *
 * This file is part of BaS_gcc.
 *
 * BaS_gcc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BaS_gcc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BaS_gcc.  If not, see <http://www.gnu.org/licenses/>.
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
