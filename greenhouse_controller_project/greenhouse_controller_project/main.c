/****************************************************
 * Greenhouse monitoring and control system
 * ATMEGA328P, 16MHz, 
 * 
 * Authors: Baranek, Barina, Bekec, Drapal
 * Created: 24.11.2021
 ****************************************************/ 

#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "lcd.h"
#include <stdlib.h>
#include "uart.h"           // Peter Fleury's UART library
#include "twi.h"
#include "adc_control.h"
#include "lcd_menu.h"

uint8_t humidity_integer = 0;
uint8_t humidity_fractional = 0;
uint8_t temperature_integer = 0;
uint8_t temperature_fractional = 0;
uint8_t checksum = 0;
uint16_t soil_humidity_raw = 0;
uint16_t light_level_raw = 0;
uint8_t soil_humidity = 0;
uint8_t light_level = 0;
uint16_t button_val = 0;

uint8_t humidity_limit = 0;
uint8_t temperature_limit = 0;
uint8_t soil_humidity_limit = 0;
uint8_t light_level_limit = 0;

typedef enum {
    STATE_IDLE = 1,
    STATE_HUMIDITY,
    STATE_TEMPERATURE,
    STATE_SOIL_HUMIDITY,
    STATE_LIGHT_LEVEL    
} state_t;

int main(void)
{
    // Initialize
    lcd_init(LCD_DISP_ON);
    twi_init();
    adc_init();
    
    lcd_gotoxy(0,0);
    lcd_puts("Initialization..");
    
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 262 ms and enable overflow interrupt
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();
    
    // Configure 8-bit Timer/Counter2 to update FSM
    // Set prescaler to 16 ms and enable interrupt
    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
    
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        lcd_main_menu(humidity_integer, temperature_integer, soil_humidity, light_level);
    }

    // Will never reach this
    return 0;
}

ISR(TIMER1_OVF_vect)
{
    twi_start((0x5c<<1) + TWI_WRITE);
    twi_write(0x00);
    twi_stop();
    twi_start((92<<1) + TWI_READ);
    humidity_integer = twi_read_ack();
    humidity_fractional = twi_read_ack();
    temperature_integer = twi_read_ack();
    temperature_fractional = twi_read_ack();
    checksum = twi_read_nack();
    twi_stop();
}

ISR(TIMER2_OVF_vect)
{
    static uint8_t update = 0;
    update++;
    
    if (update >= 50)
    {
        update = 0;
        float frac = 100.0/1024.0;
        float soil_humidity_float = soil_humidity_raw * frac;
        float light_level_float = light_level_raw * frac;
        soil_humidity = soil_humidity_float;
        light_level = light_level_float;
        
		static state_t state = STATE_IDLE;
		
        switch (state)
        {
        case STATE_IDLE:
            lcd_main_menu(humidity_integer, temperature_integer, soil_humidity, light_level);
			if (button_val >= 0 && button_val < 51){
				state = STATE_HUMIDITY;
				}
			else if (button_val >= 325 && button_val < 527){
				state = STATE_LIGHT_LEVEL;
				}
            break;
        case STATE_HUMIDITY:
            lcd_humidity_menu(humidity_integer, humidity_fractional, humidity_limit);
			if (button_val >= 0 && button_val < 51){
				state = STATE_TEMPERATURE;
			}
			else if (button_val >= 325 && button_val < 527){
				state = STATE_IDLE;
			}
			else if (button_val >= 51 && button_val < 174) {
				humidity_limit++;
			}
			else if (button_val >= 174 && button_val < 325) {
				humidity_limit--;
			}
            break;
        case STATE_TEMPERATURE:
            lcd_temperature_menu(temperature_integer, temperature_fractional, temperature_limit);
			if (button_val >= 0 && button_val < 51){
				state = STATE_SOIL_HUMIDITY;
			}
			else if (button_val >= 325 && button_val < 527){
				state = STATE_HUMIDITY;
			}
			else if (button_val >= 51 && button_val < 174) {
				temperature_limit++;
			}
			else if (button_val >= 174 && button_val < 325) {
				temperature_limit--;
			}
            break;
        case STATE_SOIL_HUMIDITY:
            lcd_soil_humidity_menu(soil_humidity, soil_humidity_limit);
			if (button_val >= 0 && button_val < 51){
				state = STATE_LIGHT_LEVEL;
			}
			else if (button_val >= 325 && button_val < 527){
				state = STATE_TEMPERATURE;
			}
			else if (button_val >= 51 && button_val < 174) {
				soil_humidity_limit++;
			}
			else if (button_val >= 174 && button_val < 325) {
				soil_humidity_limit--;
			}
            break;
        case STATE_LIGHT_LEVEL:
            lcd_light_level_menu(light_level, light_level_limit);
			if (button_val >= 0 && button_val < 51){
				state = STATE_IDLE;
			}
			else if (button_val >= 325 && button_val < 527){
				state = STATE_SOIL_HUMIDITY;
			}
			else if (button_val >= 51 && button_val < 174) {
				light_level_limit++;
			}
			else if (button_val >= 174 && button_val < 325) {
				light_level_limit--;
			}
            break;
        default:
            state = STATE_IDLE;
			break;
        }
    }
    
    ADCSRA |= (1<<ADSC);
}

ISR(ADC_vect)
{
    switch(ADMUX)
    {
        case 0x40:
        soil_humidity_raw = ADC;
        ADMUX = 0x41;
        break;
        
        case 0x41:
        light_level_raw = ADC;
        ADMUX = 0x42;
        break;
        
        case 0x42:
        button_val = ADC;
        ADMUX = 0x40;
        break;
        
        default:
        ADMUX = 0x40;
        break;
    }
}    
