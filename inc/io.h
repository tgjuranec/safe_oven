/*
 * io.h
 *
 *  Created on: Oct 5, 2014
 *      Author: tgjuranec
 */

#ifndef IO_H_
#define IO_H_
typedef enum button_state_en{
	BT_OFF,
	BT_ON,
	BT_ON_LONG
} button_state;




void io_init();
void io_set_as_input(CHIP_IOCON_PIO_T ioconpin);
bool io_get_input_state(CHIP_IOCON_PIO_T ioconpin);
void io_set_as_output(CHIP_IOCON_PIO_T ioconpin);
void io_set_output_state(CHIP_IOCON_PIO_T ioconpin, bool value);


//debounce functions
void io_debounce_init(CHIP_IOCON_PIO_T *pins ,uint32_t npins);
void io_debounce_exec();
button_state io_button_check(CHIP_IOCON_PIO_T ioconpin);
#endif /* IO_H_ */
