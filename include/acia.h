#ifndef _ACIA_H_
#define _ACIA_H_

/*
 * ACIA registers
 */
#define keyctl	volatile uint8_t *0xfffc00
#define keybd	volatile uint8_t *0xfffc02
#define midictl	volatile uint8_t *0xfffc04
#define midi	volatile uint8_t *0xfffc06

#endif /* _ACIA_H_ */
