/*
 * buzzer.h
 *
 *  Created on: Nov 13, 2014
 *      Author: tgjuranec
 */

#ifndef BUZZER_H_
#define BUZZER_H_




void buzzer_init(CHIP_IOCON_PIO_T ioconpin);
void buzzer_const();
void buzzer_on2();
void buzzer_on4();
void buzzer_on6();
void buzzer_off();
void buzzer_exec();

#endif /* BUZZER_H_ */
