/*
 * adc.c
 *
 *  Created on: Aug 21, 2014
 *      Author: djuka
 */

#include <chip.h>
#include <sch.h>
static uint16_t adc_data;
ADC_CHANNEL_T ch;

static void adc_start(ADC_CHANNEL_T channel){
	CHIP_IOCON_PIO_T iocon;
	uint32_t funcmode = 0;
	if(channel < ADC_CH5){
		funcmode = IOCON_FUNC2;
	}
	else{
		funcmode = IOCON_FUNC1;
	}
	switch(channel){
	case ADC_CH0:
		iocon = IOCON_PIO0_11;
		break;
	case ADC_CH1:
		iocon = IOCON_PIO1_0;
		break;
	case ADC_CH2:
		iocon = IOCON_PIO1_1;
		break;
	case ADC_CH3:
		iocon = IOCON_PIO1_2;
		break;
	case ADC_CH4:
		iocon = IOCON_PIO1_3;
		break;
	case ADC_CH5:
		iocon = IOCON_PIO1_5;
		break;
	case ADC_CH6:
		iocon = IOCON_PIO1_10;
		break;
	case ADC_CH7:
		iocon = IOCON_PIO1_11;
		break;
	}
	Chip_IOCON_PinMuxSet(LPC_IOCON, iocon, funcmode);
	Chip_ADC_EnableChannel(LPC_ADC, channel, ENABLE);
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
}


void adc_init(uint32_t sample_rate, ADC_CHANNEL_T channel){
	ADC_CLOCK_SETUP_T ADCSetup;
	Chip_ADC_Init(LPC_ADC, &ADCSetup);
	ADCSetup.adcRate = sample_rate;
	ADCSetup.burstMode = true;
	ADCSetup.bitsAccuracy = ADC_10BITS;
	//Chip_ADC_SetBurstCmd(LPC_ADC,ENABLE);
	//set sample rate
	Chip_ADC_SetSampleRate(LPC_ADC,&ADCSetup, sample_rate);
	adc_start(channel);
	ch = channel;
}



void adc_read_exec(){
	tmr_timeout_set(250);
	while (Chip_ADC_ReadStatus(LPC_ADC, ch, ADC_DR_DONE_STAT) != SET && tmr_timeout_off() == 0){

	}
	Chip_ADC_ReadValue(LPC_ADC, ch, &adc_data);
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
}

uint16_t get_adc_data(){
	return adc_data;
}
