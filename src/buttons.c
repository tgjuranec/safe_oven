/*
 * buttons.c
 *
 *  Created on: Mar 2, 2017
 *      Author: tgjuranec
 */


#include <buttons.h>
#include <io.h>



/*
 * Function for initialization of buttons
 * btn - array of buttons to control
 * nbuttons - number of buttons arranged to control
 *
 */



#define COUNTER_SHORT_LIMIT  32 //detecting press
#define COUNTER_LONG_LIMIT 2048 //detecting long press
#define MAX_BUTTONS 16

//button structure

typedef struct buttontd{
	CHIP_IOCON_PIO_T pin;
	button_state bstate;
	uint32_t counter;
} button;
static button btn[MAX_BUTTONS];


//number of active buttons
uint8_t nbuttons;


void buttons_init(const CHIP_IOCON_PIO_T *pin, const uint8_t nbtns){
	uint8_t i;
	for(i = nbuttons; i < (nbuttons+nbtns); i++){
		btn[i].bstate = BUTTON_OFF;
		io_set_as_input(btn[i].pin);
		btn[i].counter = 0;
	}
	nbuttons += nbtns;
}


void buttons_exec(){
	uint8_t i;
	for(i = 0;i < nbuttons; i++){
		//BUTTON OFF
		if(btn[i].bstate == BUTTON_OFF){
			//PIN = 0
			if(io_get_input_state(btn[i].pin) == 0){
				btn[i].counter++;
				if(btn[i].counter >= COUNTER_SHORT_LIMIT){
					btn[i].counter = COUNTER_SHORT_LIMIT;
					btn[i].bstate = BUTTON_ON;
				}
			}
			//PIN = 1
			else{
				btn[i].counter = 0;
			}
		}
		//BUTTON_ON
		else {
			//PIN = 1
			if(io_get_input_state(btn[i].pin) == 1){
				btn[i].counter--;
				if(btn[i].counter == 0){
					btn[i].bstate = BUTTON_OFF;
				}
			}
			//PIN = 0
			else{
				btn[i].counter = COUNTER_SHORT_LIMIT;
			}

		}
	}
}

button_state get_nbutton_staten(const uint32_t n){
	return btn[n].bstate;
}

button_state get_button_state(const CHIP_IOCON_PIO_T pin){
	uint8_t i;
	for (i = 0; i < nbuttons; i++){
		if(btn[i].pin == pin){
			return btn[i].bstate;
		}
		else{

		}
	}
	/*
	 * if we are here, argument is not our button
	 * default return is OFF
	 */
	return BUTTON_OFF;
}
