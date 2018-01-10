/*
 * adc.h
 *
 *  Created on: Aug 21, 2014
 *      Author: djuka
 */

#ifndef ADC_H_
#define ADC_H_

#include <chip.h>
uint16_t get_adc_data();
extern uint8_t adc_conversion_finished;

void adc_init(uint32_t sample_rate, ADC_CHANNEL_T channel);
void adc_read_exec();


#endif /* ADC_H_ */
