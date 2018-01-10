/*
 * io.c
 *
 *  Created on: Oct 5, 2014
 *      Author: tgjuranec
 */

#include <chip.h>
#include <io.h>

#define MAX_BUTTONS 12
#define MAX_DEBOUNCE_COUNT 32
#define MAX_DEBOUNCE_LONG  1000


typedef struct deb_button_td{
	CHIP_IOCON_PIO_T iopin;
	button_state st;
	int16_t debounce_counter;
} deb_button;


static deb_button bt[MAX_BUTTONS];
static uint8_t nactive_buttons;

void io_init(){
	Chip_GPIO_Init(LPC_GPIO);
}

/*
 * function returns uint16 which consists of:
 * uint8_t port (high byte) << 8
 * uint8_t pin  (low byte) << 0
 */

static uint16_t iocon_to_portpin(CHIP_IOCON_PIO_T ioconpin){
	uint8_t port, pin;
	uint16_t portpin = 0;

	switch (ioconpin){
		case IOCON_PIO0_0: port = 0; pin = 0; break;
		case IOCON_PIO0_1: port = 0; pin = 1; break;
		case IOCON_PIO0_2: port = 0; pin = 2; break;
		case IOCON_PIO0_3: port = 0; pin = 3; break;
		case IOCON_PIO0_4: port = 0; pin = 4; break;
		case IOCON_PIO0_5: port = 0; pin = 5; break;
		case IOCON_PIO0_6: port = 0; pin = 6; break;
		case IOCON_PIO0_7: port = 0; pin = 7; break;
		case IOCON_PIO0_8: port = 0; pin = 8; break;
		case IOCON_PIO0_9: port = 0; pin = 9; break;
		case IOCON_PIO0_10: port = 0; pin = 10; break;
		case IOCON_PIO0_11: port = 0; pin = 11; break;

		case IOCON_PIO1_0: port = 1; pin = 0; break;
		case IOCON_PIO1_1: port = 1; pin = 1; break;
		case IOCON_PIO1_2: port = 1; pin = 2; break;
		case IOCON_PIO1_3: port = 1; pin = 3; break;
		case IOCON_PIO1_4: port = 1; pin = 4; break;
		case IOCON_PIO1_5: port = 1; pin = 5; break;
		case IOCON_PIO1_6: port = 1; pin = 6; break;
		case IOCON_PIO1_7: port = 1; pin = 7; break;
		case IOCON_PIO1_8: port = 1; pin = 8; break;
		case IOCON_PIO1_9: port = 1; pin = 9; break;
		case IOCON_PIO1_10: port = 1; pin = 10; break;
		case IOCON_PIO1_11: port = 1; pin = 11; break;

		case IOCON_PIO2_0: port = 2; pin = 0; break;
		case IOCON_PIO2_1: port = 2; pin = 1; break;
		case IOCON_PIO2_2: port = 2; pin = 2; break;
		case IOCON_PIO2_3: port = 2; pin = 3; break;
		case IOCON_PIO2_4: port = 2; pin = 4; break;
		case IOCON_PIO2_5: port = 2; pin = 5; break;
		case IOCON_PIO2_6: port = 2; pin = 6; break;
		case IOCON_PIO2_7: port = 2; pin = 7; break;
		case IOCON_PIO2_8: port = 2; pin = 8; break;
		case IOCON_PIO2_9: port = 2; pin = 8; break;
		case IOCON_PIO2_10: port = 2; pin = 10; break;
	#if !defined(CHIP_LPC1125)
		case IOCON_PIO2_11: port = 2; pin = 11; break;
	#endif

		case IOCON_PIO3_0: port = 3; pin = 0; break;
	#if !defined(CHIP_LPC1125)
		case IOCON_PIO3_1: port = 3; pin = 1; break;
	#endif
		case IOCON_PIO3_2: port = 3; pin = 2; break;
		case IOCON_PIO3_3: port = 3; pin = 3; break;
		case IOCON_PIO3_4: port = 3; pin = 4; break;
		case IOCON_PIO3_5: port = 3; pin = 5; break;
		default: port = 0xFF; pin = 0xFF; break;			//error
	}
	portpin = ((uint16_t) port << 8) | pin;
	return portpin;
}





void io_set_as_input(CHIP_IOCON_PIO_T ioconpin){
	uint16_t portpin = iocon_to_portpin(ioconpin);
	uint8_t port, pin;
	port = (portpin >> 8) & 0xFF;
	pin = portpin & 0xFF;
	if((port >= 4) || (pin >= 12)) return;
	switch(ioconpin ){
	case IOCON_PIO0_0:
		Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLUP, IOCON_FUNC1);
		break;
	case IOCON_PIO0_10:
		Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLUP, IOCON_FUNC1);
		break;
	case IOCON_PIO0_11:
		Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLUP, IOCON_FUNC1);
		break;
	case IOCON_PIO1_0:
		Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLUP, IOCON_FUNC1);
		break;
	case IOCON_PIO1_1:
		Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLUP, IOCON_FUNC1);
		break;
	case IOCON_PIO1_2:
		Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLUP, IOCON_FUNC1);
		break;
	case IOCON_PIO1_3:
		Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLUP, IOCON_FUNC1);
		break;
	default:
		Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLUP, IOCON_FUNC0);
		break;
	}
	Chip_GPIO_SetPinDIRInput(LPC_GPIO,port,pin);
}



bool io_get_input_state(CHIP_IOCON_PIO_T ioconpin){
	uint16_t portpin = iocon_to_portpin(ioconpin);
	uint8_t port, pin;
	port = (portpin >> 8) & 0xFF;
	pin = portpin & 0xFF;
	if((port >= 4) || (pin >= 12)) return false;		//TODO: make correct this it should signalize error
	return Chip_GPIO_GetPinState(LPC_GPIO, port,pin);
}




void io_set_as_output(CHIP_IOCON_PIO_T ioconpin){
	uint16_t portpin = iocon_to_portpin(ioconpin);
	uint8_t port, pin;
	port = (portpin >> 8) & 0xFF;
	pin = portpin & 0xFF;
	if((port >= 4) || (pin >= 12)) return;
	if(ioconpin == IOCON_PIO0_11)Chip_IOCON_PinMux(LPC_IOCON, ioconpin, 0, IOCON_FUNC1);
	else if(ioconpin == IOCON_PIO0_0)Chip_IOCON_PinMux(LPC_IOCON, ioconpin, 0, IOCON_FUNC1);
	else if(ioconpin == IOCON_PIO0_4)Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_STDI2C_EN, IOCON_FUNC0);
	else if(ioconpin == IOCON_PIO0_5)Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_STDI2C_EN, IOCON_FUNC0);
	else Chip_IOCON_PinMux(LPC_IOCON, ioconpin, IOCON_MODE_PULLDOWN, IOCON_FUNC0);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,port,pin);
}



void io_set_output_state(CHIP_IOCON_PIO_T ioconpin, bool value){
	uint16_t portpin = iocon_to_portpin(ioconpin);

	uint8_t port, pin;
	port = (portpin >> 8) & 0xFF;
	pin = portpin & 0xFF;
	if((port >= 4) || (pin >= 12)) return;
	Chip_GPIO_SetPinState(LPC_GPIO,port,pin,value);
	return;
}

void io_debounce_init(CHIP_IOCON_PIO_T *pins ,uint32_t npins){
	uint32_t i;
	io_init();
	for(i = nactive_buttons; i< (npins+nactive_buttons) && i < MAX_BUTTONS ;i++){
		io_set_as_input(pins[i]);
		bt[i].iopin = pins[i];
		bt[i].st = BT_OFF;
		bt[i].debounce_counter = 0;
	}
	if((npins+nactive_buttons) <= MAX_BUTTONS){
		nactive_buttons += npins;
	}
	else {
		nactive_buttons = MAX_BUTTONS;
	}
}

void io_debounce_exec(){
	uint32_t i;
	for(i = 0; i< nactive_buttons ;i++){
		//button is OFF
		if(bt[i].st == BT_OFF){
			//button pressed - TRANSITION OFF->ON
			if(io_get_input_state(bt[i].iopin) == FALSE){
				bt[i].debounce_counter++;
				//TRANSITION OFF->ON
				if(bt[i].debounce_counter >= MAX_DEBOUNCE_COUNT){
					bt[i].st = BT_ON;
					bt[i].debounce_counter = MAX_DEBOUNCE_COUNT;
				}
			}
			//button released
			else{
				bt[i].debounce_counter = 0;
			}
		}
		//button is ON
		else if (bt[i].st == BT_ON){
			//button pressed
			if(io_get_input_state(bt[i].iopin) == FALSE){
				bt[i].debounce_counter++;
				//TRANSITION ON-> LONG
				if(bt[i].debounce_counter >= MAX_DEBOUNCE_LONG){
					bt[i].st = BT_ON_LONG;
					bt[i].debounce_counter = MAX_DEBOUNCE_LONG;
				}
			}
			//button released - TRANSITION ON->OFF
			else{
				bt[i].debounce_counter--;
				//TRANSITION ON->OFF
				if(bt[i].debounce_counter <= 0){
					bt[i].st = BT_OFF;
					bt[i].debounce_counter = 0;
				}
			}
		}
		//button is LONG_ON
		else {
			//button pressed
			if(io_get_input_state(bt[i].iopin) == FALSE){
				bt[i].debounce_counter = MAX_DEBOUNCE_COUNT;
			}
			//button released - TRANSITION LONG_ON->OFF
			else{
				bt[i].debounce_counter--;
				//TRANSITION ON->OFF
				if(bt[i].debounce_counter <= 0){
					bt[i].st = BT_OFF;
					bt[i].debounce_counter = 0;
				}
			}
		}
	}
}


button_state io_button_check(CHIP_IOCON_PIO_T ioconpin){
	uint32_t i;
	for(i = 0; i< nactive_buttons; i++){
		if(bt[i].iopin == ioconpin){
			return bt[i].st;
		}
	}
	return BT_OFF;
}
