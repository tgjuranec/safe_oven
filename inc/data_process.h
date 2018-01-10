/*
 * data_process.h
 *
 *  Created on: Mar 2, 2017
 *      Author: tgjuranec
 */

#ifndef DATA_PROCESS_H_
#define DATA_PROCESS_H_



void data_process_init(const CHIP_IOCON_PIO_T pushbutton,
		const CHIP_IOCON_PIO_T switchp,
		const CHIP_IOCON_PIO_T outputpin);


void data_process_exec();

#endif /* DATA_PROCESS_H_ */
