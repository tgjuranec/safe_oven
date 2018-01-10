/*
===============================================================================
 Name        : safe_oven.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#endif
#endif

#include <cr_section_macros.h>
#include <lpc_types.h>
#include <adc.h>
#include <buzzer.h>
#include <sch.h>
#include <io.h>
#include <data_process.h>
#include <LCD1602.h>



int main(void) {

    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();

    sch_init();
    LCD1602_init(IOCON_PIO1_7,		/*RS  - CONN2 - PIN 16*/ \
    		IOCON_PIO3_4,			/*RW - NC*/ \
			IOCON_PIO0_8,			/*D7 - CONN2 - PIN 12*/ \
			IOCON_PIO1_10,			/*D6 - CONN2 - PIN 10*/ \
			IOCON_PIO2_11,			/*D5 - CONN2 - PIN 8*/ \
			IOCON_PIO0_6,			/*D4 - CONN1 - PIN 8*/ \
			IOCON_PIO0_6,
			IOCON_PIO0_6,\
			IOCON_PIO0_6,\
			IOCON_PIO0_6,\
			IOCON_PIO1_6,			/*E - CONN2 - PIN 14*/ \
			IOCON_PIO3_4,\
			IOCON_PIO3_4);

	adc_init(1000,ADC_CH0);   //IOCON_PIO0_11 - CONN2 - PIN 6

	buzzer_init(IOCON_PIO1_9);	// - CONN1 - PIN 6
	CHIP_IOCON_PIO_T bt[2];
	bt[0] = IOCON_PIO0_5;	// - CONN1 - PIN 4
	bt[1] = IOCON_PIO0_0;	//CONN2 - PIN 11
	io_debounce_init(&bt[0],2);
	data_process_init(bt[0],bt[1],IOCON_PIO0_9);	// - CONN2 - PIN 7

	sch_addtask(adc_read_exec, 12,100, 1);
	sch_addtask(buzzer_exec,14,100,1);
	sch_addtask(io_debounce_exec,0,1,0);
	sch_addtask(LCD1602_exec,11,2,1);
	sch_addtask(data_process_exec,13,100,1);
	sch_start();


	while(1){

		sch_dispatch();
	}
    return 0 ;
}
