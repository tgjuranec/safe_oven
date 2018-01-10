/*
 * buzzer.c
 *
 *  Created on: Nov 13, 2014
 *      Author: tgjuranec
 */

#include <chip.h>
#include <io.h>
#include <buzzer.h>
#include <sch.h>

typedef enum buzzer_state_td{
	BUZZER_OFF,
	BUZZER_ON_CONST,
	BUZZER_ON_2_1,
	BUZZER_ON_2_2,
	BUZZER_ON_4_1,
	BUZZER_ON_4_2,
	BUZZER_ON_4_3,
	BUZZER_ON_4_4,
	BUZZER_ON_6_1,
	BUZZER_ON_6_2,
	BUZZER_ON_6_3,
	BUZZER_ON_6_4,
	BUZZER_ON_6_5,
	BUZZER_ON_6_6
} buzzer_state;


static buzzer_state bs;
static uint32_t bs_switch = 0;
static CHIP_IOCON_PIO_T pin_buzzer;

void buzzer_init(CHIP_IOCON_PIO_T ioconpin){
	pin_buzzer = ioconpin;
	io_set_output_state(ioconpin, 1);	//1 - inactive
	io_set_as_output(ioconpin);
	bs = BUZZER_OFF;
}



void buzzer_exec(){

	switch(bs){
	case BUZZER_OFF:
		io_set_output_state(pin_buzzer, 1);
		break;
	case BUZZER_ON_CONST:
		io_set_output_state(pin_buzzer, 0);
		break;
	case BUZZER_ON_2_1:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_2_2;
			io_set_output_state(pin_buzzer, 1);
			bs_switch = tick + 500;
		}
		break;
	case BUZZER_ON_2_2:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_2_1;
			io_set_output_state(pin_buzzer, 0);
			bs_switch = tick + 500;
		}
	case BUZZER_ON_4_1:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_4_2;
			io_set_output_state(pin_buzzer, 1);
			bs_switch = tick + 200;
		}
		break;
	case BUZZER_ON_4_2:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_4_3;
			io_set_output_state(pin_buzzer, 0);
			bs_switch = tick + 2000;
		}
		break;
	case BUZZER_ON_4_3:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_4_4;
			io_set_output_state(pin_buzzer, 1);
			bs_switch = tick + 1000;
		}
		break;
	case BUZZER_ON_4_4:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_4_1;
			io_set_output_state(pin_buzzer, 0);
			bs_switch = tick + 200;
		}
		break;
	case BUZZER_ON_6_1:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_6_2;
			io_set_output_state(pin_buzzer, 1);
			bs_switch = tick + 200;
		}
		break;
	case BUZZER_ON_6_2:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_6_3;
			io_set_output_state(pin_buzzer, 0);
			bs_switch = tick + 200;
		}
		break;
	case BUZZER_ON_6_3:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_6_4;
			io_set_output_state(pin_buzzer, 1);
			bs_switch = tick + 200;
		}
		break;
	case BUZZER_ON_6_4:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_6_5;
			io_set_output_state(pin_buzzer, 0);
			bs_switch = tick + 500;
		}
		break;
	case BUZZER_ON_6_5:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_6_6;
			io_set_output_state(pin_buzzer, 1);
			bs_switch = tick + 2000;
		}
		break;
	case BUZZER_ON_6_6:
		if(tick >= bs_switch ){
			bs = BUZZER_ON_6_1;
			io_set_output_state(pin_buzzer, 0);
			bs_switch = tick + 2000;
		}
		break;
	default:
		break;
	}
}

void buzzer_const(){
	bs = BUZZER_ON_CONST;
	io_set_output_state(pin_buzzer, 0);
}


void buzzer_on2(){
	bs = BUZZER_ON_2_1;
	bs_switch = tick + 500;
	io_set_output_state(pin_buzzer, 0);
}

void buzzer_on4(){
	bs = BUZZER_ON_4_1;
	bs_switch = tick + 200;
	io_set_output_state(pin_buzzer, 0);
}

void buzzer_on6(){
	bs = BUZZER_ON_6_1;
	bs_switch = tick + 2000;
	io_set_output_state(pin_buzzer, 0);
}

void buzzer_off(){
	bs = BUZZER_OFF;
	io_set_output_state(pin_buzzer, 1);
}

