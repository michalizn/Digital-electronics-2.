/*
 * adc_control.c
 * ADC 
 *
 * Created: 24.11.2021 12:24:56
 *  Author: student
 */ 


#include "adc_control.h"

void adc_init()
{
    // Set ADC reference to AVcc
    ADMUX &= ~(1<<REFS1); ADMUX |= (1<<REFS0);
    // Set input channel to ADC0
    ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
    // Enable ADC Module
    ADCSRA |= (1<<ADEN);
    // Enable Conversion Complete Interrupt
    ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA &= ~((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
    
};