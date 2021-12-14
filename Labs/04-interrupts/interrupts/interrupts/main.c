/***********************************************************************
 * 
 * Control LEDs using functions from GPIO and Timer libraries. Do not 
 * use delay library any more.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB7
#define LED_D2  PB6
#define LED_D3  PB5


#ifndef F_CPU           // Preprocessor directive allows for conditional
// compilation. The #ifndef means "if not defined".
# define F_CPU 16000000 // CPU frequency in Hz required for delay
#endif                  // The #ifndef directive must be closed by #endif


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <util/delay.h> // Functions for busy-wait delay loops
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED on the Multi-function shield using 
             the internal 8- or 16-bit Timer/Counter.
 * Returns:  none
 **********************************************************************/
int main(void)
{
	
    // Configuration of LED(s) at port B
	/*
    GPIO_config_output(&DDRB, LED_D1);
    GPIO_write_low(&PORTB, LED_D1);
    GPIO_config_output(&DDRC, LED_D2);
    GPIO_write_low(&PORTC, LED_D2);
    GPIO_config_output(&DDRC, LED_D3);
    GPIO_write_low(&PORTC, LED_D3);

    // Configuration of 16-bit Timer/Counter1 for LED blinking
    // Set the overflow prescaler to 262 ms and enable interrupt
    TIM0_overflow_1024u();
    TIM0_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
	*/
	/*
    DDRB = DDRB | (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB = PORTB & ~(1<<LED_GREEN);
	*/
	
	GPIO_config_output(&DDRB, LED_D1);
	GPIO_write_low(&PORTB, LED_D1);
	GPIO_config_output(&DDRB, LED_D2);
	GPIO_write_low(&PORTB, LED_D2);
	GPIO_config_output(&DDRB, LED_D3);
	GPIO_write_low(&PORTB, LED_D3);
	
	//TIM1_overflow_1s();
	//TIM1_overflow_interrupt_enable();
	
	//sei();

    // Infinite loop
    while (1)
    {
	    static uint16_t i=0;
	    if(i>4){
		    i=0;
	    }
	    else{
		    i++;
	    }
	    
	    if(i==1){
		    GPIO_toggle(&DDRB, LED_D1);
	    }
	    else if(i==2){
		    GPIO_toggle(&DDRB, LED_D2);
	    }
	    else if(i==3){
		    GPIO_toggle(&DDRB, LED_D3);
	    }

    }
	return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Toggle D1 LED on Multi-function shield.
 **********************************************************************/
/*ISR(TIMER1_OVF_vect)
{
	
        static uint16_t i=0;
        if(i>=5){
	        i=0;
        }
        else{
	        i++;
        }
        
        if(i==1){
	        GPIO_toggle(&DDRB, LED_GREEN);
        }
        else if(i==2){
	        GPIO_toggle(&DDRB, LED_GREEN);
        }
        else if(i==3){
	        GPIO_toggle(&DDRB, LED_GREEN);
        }
        else if(i==4){
	        GPIO_toggle(&DDRB, LED_GREEN);
		}
       
}
*/