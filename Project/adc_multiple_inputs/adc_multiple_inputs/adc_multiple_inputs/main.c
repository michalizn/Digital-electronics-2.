/*
 * adc_multiple_inputs.c
 *
 * Created: 01.12.2021 13:40:07
 * Author : student
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include <stdlib.h>
#include "twi.h"

uint16_t value0 = 0;
uint16_t value1 = 0;
uint16_t value2 = 0;

int main(void)
{
    lcd_init(LCD_DISP_ON);
    ADMUX &= ~(1<<REFS1); ADMUX |= (1<<REFS0);
    // Set input channel to ADC0
    ADMUX &= ~((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
    // Enable ADC module
    ADCSRA |= (1<<ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1<<ADIE);
    // Set clock prescaler to 128
    ADCSRA |= ((1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0));
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 1s and enable overflow interrupt
    TIM1_overflow_1s();
    TIM1_overflow_interrupt_enable();
     
    // Enables interrupts by setting the global interrupt mask
    sei();

    while (1) 
    {
    }
}
ISR(TIMER1_OVF_vect)
{
    ADCSRA |= (1<<ADSC); 
}    

ISR(ADC_vect)
{
    switch(ADMUX)
    {
        case 0x40:
        value0 = ADC;
        ADMUX = 0x41;
        break;
        
        case 0x41:
        value1 = ADC;
        ADMUX = 0x43;
        break;
        
        case 0x43:
        value2 = ADC;
        ADMUX = 0x40;
        break;
        
        default:
        ADMUX = 0x40;
        break;
    }   
}    

