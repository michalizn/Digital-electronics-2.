/*
 * adc_control.h
 * ADC control functions
 *
 * Created: 24.11.2021
 * Author: Bekec
 */ 


#ifndef ADC_CONTROL_H_
#define ADC_CONTROL_H_

#include <avr/io.h>

void ADC_init();

uint16_t ADC_read(uint8_t analogPin);

#endif /* ADC_CONTROL_H_ */