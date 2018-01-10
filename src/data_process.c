/*
 * data_process.c
 *
 *  Created on: Mar 2, 2017
 *      Author: tgjuranec
 */

#include <adc.h>
#include <buzzer.h>
#include <LCD1602.h>
#include <io.h>
#include <data_process.h>

/*if output transistor is NPN - set this as '1', otherwise set it as '0'
 */
#define CONTROLOUT_NPN 1

typedef enum data_processtd{
	DP_ADJUSTMENT,
	DP_ACTIVATED,
	DP_FINISHED,
	DP_ERROR
} data_process_st;
static const char lcd_furnace_off[] = "OFF";
static char lcd_furnace_on[] = "ON              ";
static const char lcd_furnace_error[] = "TEMP ERROR";
static const char lcd_furnace_finished[] = "FINISHED";
static char lcd_timer_status[] = "Hr: 0 Min:  0";
static CHIP_IOCON_PIO_T herd_ctrl_pin;
static CHIP_IOCON_PIO_T pushbtn_pin;
static CHIP_IOCON_PIO_T switch_pin;


void data_process_init(const CHIP_IOCON_PIO_T pushbutton,
		const CHIP_IOCON_PIO_T switchp,
		const CHIP_IOCON_PIO_T outputpin){
	LCD1602_print(lcd_furnace_off,lcd_timer_status);
	herd_ctrl_pin = outputpin;
	pushbtn_pin = pushbutton;
	switch_pin = switchp;
	io_set_as_output(herd_ctrl_pin);
	io_set_as_input(pushbtn_pin);
	io_set_as_input(switch_pin);
	io_set_output_state(herd_ctrl_pin,!CONTROLOUT_NPN); //turns off herd

}


/*
 *function for update LCD
 * s -> state of data_process state machine
 * tmr_min -> state of the counter
 * sec -> state of the second counter
 */
static void update_lcd(data_process_st s, int16_t tmr_min, int16_t sec){
	char dig10,dig1;
	dig1 = (tmr_min / 60) + 0x30;
	if(dig1 == 0x30)dig10 = 0x20;
	lcd_timer_status[4] = dig1;
	dig10 = ((tmr_min%60) / 10) + 0x30;
	dig1 = ((tmr_min %60) % 10) + 0x30;
	lcd_timer_status[11] = dig10;
	lcd_timer_status[12] = dig1;
	switch(s){
	case DP_ADJUSTMENT:
		LCD1602_print(lcd_furnace_off,lcd_timer_status);
		break;
	case DP_ACTIVATED:
		if(sec >= 60) sec = 60;
		uint8_t nrects = sec/5;
		//fill with 0x20 (space)
		uint8_t i = 0;
		for(i = 2; i < 16; i++){
			lcd_furnace_on[i] = 0x20;
		}
		while(nrects){
			lcd_furnace_on[16 - nrects] = 0xFF;
			nrects--;
		}
		LCD1602_print(lcd_furnace_on,lcd_timer_status);
		break;
	case DP_FINISHED:
		LCD1602_print(lcd_furnace_finished,lcd_timer_status);
		break;
	case DP_ERROR:
		LCD1602_print(lcd_furnace_error,lcd_timer_status);
		break;
	default:
		break;
	}
}



void data_process_exec(){
	static data_process_st st;
	static int16_t tmr_min;
	static int32_t countdown_second = 600;
	static button_state push_oldstate = BT_OFF;
	//CHECK TEMPERATURE in all states
	//immediately turn off
	//reset all counters
	//wait for temperature recovery
	if(get_adc_data() > 200){
		tmr_min = 0;
		countdown_second = 600;
		st = DP_ERROR;
		io_set_output_state(herd_ctrl_pin,!CONTROLOUT_NPN); //turns off herd
		tmr_min = 0;
		//TODO: apply LCD
		update_lcd(st,tmr_min,0);
		goto end_of_data_process;
	}

	switch(st){
	case DP_ADJUSTMENT:
		//check buttons
		//Transition from BT_OFF into BT_ON
		//increment tmr_min
		if(io_button_check(pushbtn_pin) == BT_ON && push_oldstate == BT_OFF){
			tmr_min++;
			if(tmr_min >= 120){
				tmr_min = 0;
			}
			//TODO: apply LCD
			update_lcd(st,tmr_min,0);
		}
		else if(io_button_check(pushbtn_pin) == BT_ON_LONG){
			static uint32_t i;
			i++;
			if((i % 8) == 0){
				tmr_min+=10;
				if(tmr_min >= 120){
					tmr_min = 0;
				}
				//TODO: apply LCD
				update_lcd(st,tmr_min,0);
			}
		}
		else{

		}
		//SWITCH ON -> change state to ACTIVATED
		if (io_button_check(switch_pin) == BT_ON && \
				(tmr_min > 0)){
			//activate herd
			st = DP_ACTIVATED;
			io_set_output_state(herd_ctrl_pin,CONTROLOUT_NPN); //turns on herd
			update_lcd(st,tmr_min,countdown_second/10);
		}
		else{

		}
		break;

	case DP_ACTIVATED:
		//check only switch button, push button is ignored
		if(io_button_check(switch_pin) >= BT_ON){
			//decrement timer variables
			countdown_second--;
			if(countdown_second <= 0){
				tmr_min--;
				countdown_second = 600;
				//Countdown finished -> change state into DP_FINISHED
				if(tmr_min <= 0){
					tmr_min = 0;
					countdown_second = 600;
					io_set_output_state(herd_ctrl_pin,!CONTROLOUT_NPN); //turns off herd
					st = DP_FINISHED;
					//TODO: apply LCD
					update_lcd(st,tmr_min,0);
				}

				else {
					update_lcd(st,tmr_min,countdown_second/10);
				}
			}

			else{
				//TODO: apply LCD
				if((countdown_second % 50) == 0)
					update_lcd(st,tmr_min,countdown_second/10);
			}
		}
		//when someone turns off switch button
		else{
			io_set_output_state(herd_ctrl_pin,!CONTROLOUT_NPN);  //turns off herd
			st = DP_ADJUSTMENT;
			//TODO: apply LCD
			update_lcd(st,tmr_min,countdown_second/10);
		}
		break;

	case DP_FINISHED:
	case DP_ERROR:
		io_set_output_state(herd_ctrl_pin,!CONTROLOUT_NPN);		//turns off herd
		if(io_button_check(switch_pin) == BT_OFF){
			st = DP_ADJUSTMENT;
			//TODO: apply LCD
			update_lcd(st,tmr_min,0);

		}
		break;
	default:
		break;
	}
end_of_data_process:
	push_oldstate = io_button_check(pushbtn_pin);
}
