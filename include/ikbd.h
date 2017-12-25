#ifndef _IKBD_H_
#define _IKBD_H_

extern void ikbd_init(void);
extern void ikbd_poll(void);
extern void ikbd_joystick(uint8_t joy, uint8_t map);
extern void ikbd_mouse(uint8_t buttons, int8_t x, int8_t y);
extern void ikbd_keyboard(uint8_t code);

#endif /* _IKBD_H_ */

