/*
 * buttons.h
 *
 *  Created on: Mar 2, 2017
 *      Author: tgjuranec
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <chip.h>
typedef enum button_state{
	BUTTON_OFF,
	BUTTON_ON,
	BUTTON_ON_LONG
} button_state;



void buttons_init(const CHIP_IOCON_PIO_T *btn, const uint8_t nbtns);
void buttons_exec();
button_state get_nbutton_state(const uint32_t n);
button_state get_button_state(const CHIP_IOCON_PIO_T pin);

#endif /* BUTTONS_H_ */
