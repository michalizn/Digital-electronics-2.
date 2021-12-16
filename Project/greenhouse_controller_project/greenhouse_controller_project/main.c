/**********************************************************************
 *
 * Greenhouse monitoring and control system
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2 
 * 
 * Authors: Baranek, Barina, Bekec, Drapal
 * Created: 24.11.2021
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/ 

/* Defines -----------------------------------------------------------*/
#define SHADE        PD3	// AVR pin where shader is connected
#define VENT         PD2	// AVR pin where ventilator is connected
#define WATER_PUMP   PD1	// AVR pin where water pump is connected
#define WINDOW       PD0	// AVR pin where window opener is connected
#ifndef F_CPU
# define F_CPU 16000000		// CPU frequency in Hz
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>			// AVR device-specific IO definitions
#include <avr/interrupt.h>	// Interrupts standard C library for AVR-GCC
#include <stdlib.h>			// C library. Needed for conversion function
#include "gpio.h"			// GPIO library for AVR-GCC
#include "timer.h"			// Timer library for AVR-GCC
#include "lcd.h"			// Peter Fleury's LCD library
#include "twi.h"			// TWI library for AVR-GCC
#include "adc_control.h"	// ADC configure library for AVR-GCC
#include "lcd_menu.h"		// LCD menu library for greenhouse interface
#include "button.h"			// Shield button library for AVR-GCC

/* Variables ---------------------------------------------------------*/
uint8_t	humidity_integer = 0;
uint8_t	humidity_fractional = 0;
uint8_t	temperature_integer = 0;
uint8_t	temperature_fractional = 0;
uint8_t	checksum = 0;
uint8_t	soil_moisture = 0;
uint8_t	light_level = 0;
uint16_t soil_moisture_raw = 0;
uint16_t light_level_raw = 0;
uint16_t button_pressed_adc = 0;

uint8_t	humidity_limit = 50;
uint8_t	temperature_limit = 20;
uint8_t	soil_moisture_limit = 60;
uint8_t	light_level_limit = 0;

uint8_t	half_range = 1;

typedef enum {				// FSM declaration
    STATE_IDLE = 1,
    STATE_HUMIDITY,
    STATE_TEMPERATURE,
    STATE_SOIL_MOISTURE,
    STATE_LIGHT_LEVEL    
} state_t;

state_t state = STATE_IDLE;	// Current state of the FSM

int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
	// Initialize I2C (TWI)
    twi_init();
	// Initialize ADC
    adc_init();
    
	// Informative string
    lcd_gotoxy(0,0); lcd_puts("Initialization..");
    
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 262 ms and enable overflow interrupt
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();
    
    // Configure 8-bit Timer/Counter2 to update FSM
    // Set prescaler to 33 ms and enable interrupt
    TIM2_overflow_16ms();
    TIM2_overflow_interrupt_enable();
    
    // Enables interrupts by setting the global interrupt mask
    sei();
	// Configuration of a ventilator at port D
    GPIO_config_output(&DDRD, VENT);
    GPIO_write_low(&PORTD, VENT);
    // Configuration of water pump at port D
    GPIO_config_output(&DDRD, WATER_PUMP);
    GPIO_write_low(&PORTD, WATER_PUMP);
    // Configuration of window opener at port D
    GPIO_config_output(&DDRD, WINDOW);
    GPIO_write_low(&PORTD, WINDOW);
    // Configuration of shader at port D
    GPIO_config_output(&DDRD, SHADE);
    GPIO_write_low(&PORTD, SHADE);

    // Infinite loop
    while (1)
    {
        if (humidity_integer >= humidity_limit + half_range)
        {
			// Pull pin 0 up and open the window
            GPIO_write_high(&PORTD, WINDOW);
        }
        if (humidity_integer <= humidity_limit - half_range)
        {
			// Pull pin 0 down and close the window
            GPIO_write_low(&PORTD, WINDOW);
        }
        if (temperature_integer >= temperature_limit + half_range)
        {
			// Pull pin 2 up and start the ventilator
            GPIO_write_high(&PORTD, VENT);
			// Pull pin 0 up and open the window
            GPIO_write_high(&PORTD, WINDOW);
        }
        if (temperature_integer <= temperature_limit - half_range)
        {
			// Pull pin 2 down and shutdown the ventilator
            GPIO_write_low(&PORTD, VENT);
			// Pull pin 0 down and close the window
            GPIO_write_low(&PORTD, WINDOW);
        }
        if (soil_moisture >= soil_moisture_limit + half_range)
        {
			// Pull pin 1 down and shutdown the water pump
            GPIO_write_low(&PORTD, WATER_PUMP);
        }
        if (soil_moisture <= soil_moisture_limit - half_range)
        {
			// Pull pin 1 up and start the watering
            GPIO_write_high(&PORTD, WATER_PUMP);
        }
        if (light_level >= light_level_limit + half_range)
        {   
			// Pull pin 3 down and shade the window
            GPIO_write_low(&PORTD, SHADE);
        }
        if (light_level <= light_level_limit - half_range)
        {
			// Pull pin 3 up and unshade the window
            GPIO_write_high(&PORTD, SHADE);
        }
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routine -----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Reading the data bytes from slave device and sending it 
 *           into I2C stream
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
	// Start communication on TWI bus for writing to TWI device
    twi_start((0x5c<<1) + TWI_WRITE);
	// Send one data byte to TWI slave device
    twi_write(0x00);
	// Generates stop condition on TWI bus
    twi_stop();
	// Start communication on TWI bus for reading from TWI device
    twi_start((92<<1) + TWI_READ);
	/* Reading bytes according to datasheet of DHT12 sensor from 
	 * TWI slave device and acknowledge it by ACK */
	humidity_integer = twi_read_ack();
    humidity_fractional = twi_read_ack();
    temperature_integer = twi_read_ack();
    temperature_fractional = twi_read_ack();
	// Read one byte from TWI slave device and acknowledge it by NACK
    checksum = twi_read_nack();
	// Generates stop condition on TWI bus
    twi_stop();
}

/* Interrupt service routine -----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Conversion ADC values to percentage values and updating 
 *			 Finite State Machine 
 **********************************************************************/
ISR(TIMER2_OVF_vect)
{
    static uint8_t update = 0;
	// Increment update value
    update++;
    
    if (update >= 50)
    {
        update = 0;
		// Conversion ADC values to percentage values
        float frac = 100.0/1024.0;
        float soil_moisture_float = soil_moisture_raw * frac;
        float light_level_float = light_level_raw * frac;
        soil_moisture = soil_moisture_float;
        light_level = light_level_float;
        // 
        uint8_t button_pressed = button_analog(button_pressed_adc);
		// FSM
        switch (state)
        {
        case STATE_IDLE:
			// Update main menu
            lcd_main_menu(humidity_integer, temperature_integer, 
						  soil_moisture, light_level);
            // FSM
			switch (button_pressed)
            {
				// Button left is pressed
                case 4:
                state = STATE_LIGHT_LEVEL;
                break;
				// Button right is pressed
                case 6: 
                state = STATE_HUMIDITY;
                break;
				// Button select is pressed
                case 5:
                state = STATE_IDLE;
                break;
				// If something unexpected happens move to main menu
                default:
                state = STATE_IDLE;
                break;        
            }
            break;
        case STATE_HUMIDITY:
			// Update humidity menu
            lcd_humidity_menu(humidity_integer, humidity_fractional, 
							  humidity_limit);
            // FSM
			switch (button_pressed)
            {
				// Button left is pressed
                case 4:
                state = STATE_IDLE;
                break;
				// Button right is pressed
                case 6:
                state = STATE_TEMPERATURE;
                break;
				// Button up is pressed 
                case 8:
				/* If conditions passes, increase the humidity 
				 * nominal value */
                if (humidity_limit < 95)
				{
                    humidity_limit++;
                }
                break;
				// Button down is pressed 
                case 2:
				/* If conditions passes, decrease the humidity
				 * nominal value */
                if (humidity_limit > 40)
				{
                    humidity_limit--;
                }
                break;
				// Button select is pressed
                case 5:
                state = STATE_IDLE;
                break;
				/* If something unexpected happens move to 
				 * humidity menu */
                default:
                state = STATE_HUMIDITY;
                break;
            }
            break;
        case STATE_TEMPERATURE:
			// Update temperature menu
            lcd_temperature_menu(temperature_integer, 
								 temperature_fractional, 
								 temperature_limit);
            // FSM
			switch (button_pressed)
            {
				// Button left is pressed
                case 4:
                state = STATE_HUMIDITY;
                break;
				// Button right is pressed
                case 6:
                state = STATE_SOIL_MOISTURE;
                break;
				// Button up is pressed 
                case 8:
				/* If conditions passes, increase the temperature 
				 * nominal value */
                if (temperature_limit < 40)
				{
                    temperature_limit++;
                }
                break;
				// Button down is pressed 
                case 2:
				/* If conditions passes, decrease the temperature
				 * nominal value */
                if (temperature_limit > 15)
				{
                    temperature_limit--;
                }                
                break;
				// Button select is pressed
                case 5:
                state = STATE_IDLE;
                break;
				/* If something unexpected happens move to 
				 * temperature menu */
                default:
                state = STATE_TEMPERATURE;
                break;
            }
            break;
        case STATE_SOIL_MOISTURE:
			// Update soil moisture menu
            lcd_soil_moisture_menu(soil_moisture, soil_moisture_limit);
            // FSM
			switch (button_pressed)
            {
				// Button left is pressed
                case 4:
                state = STATE_TEMPERATURE;
                break;
				// Button right is pressed
                case 6:
                state = STATE_LIGHT_LEVEL;
                break;
				// Button up is pressed 
                case 8:
				/* If conditions passes, increase the soil moisture
				 * nominal value */
                if (soil_moisture_limit < 70)
				{
                    soil_moisture_limit++;
                }
                break;
				// Button down is pressed 
                case 2:
				/* If conditions passes, decrease the soil moisture
				 * nominal value */
                if (soil_moisture_limit > 10)
				{
                    soil_moisture_limit--;
                }
                break;
				// Button select is pressed
                case 5:
                state = STATE_IDLE;
                break;
				/* If something unexpected happens move to 
				 * soil moisture menu */
                default:
                state = STATE_SOIL_MOISTURE;
                break;
            }
            break;
        case STATE_LIGHT_LEVEL:
			// Update light level menu 
            lcd_light_level_menu(light_level, light_level_limit);
            // FSM
			switch (button_pressed)
            {
				// Button left is pressed
                case 4:
                state = STATE_SOIL_MOISTURE;
                break;
				// Button right is pressed
                case 6:
                state = STATE_IDLE;
                break;
				// Button up is pressed 
                case 8:
				/* If conditions passes, increase the light level
				 * nominal value */
                if (light_level_limit < 100)
				{
                    light_level_limit++;
                }
                break;
				// Button down is pressed 
                case 2:
				/* If conditions passes, decrease the light level
				 * nominal value */
                if (light_level_limit > 10)
				{
                    light_level_limit--;
                }
                break;
				// Button select is pressed
                case 5:
                state = STATE_IDLE;
                break;
				/* If something unexpected happens move to 
				 * light level */
                default:
                state = STATE_LIGHT_LEVEL;
                break;
            }
            break;
		// If something unexpected happens move to main menu
        default:
			// Update main menu
            lcd_main_menu(humidity_integer, temperature_integer, 
						  soil_moisture, light_level);
            // FSM
			switch (button_pressed)
            {
				// Button left is pressed
                case 4:
                state = STATE_LIGHT_LEVEL;
                break;
				// Button right is pressed
                case 6:
                state = STATE_HUMIDITY;
                break;
				// Button select is pressed
                case 5:
                state = STATE_IDLE;
                break;
				// If something unexpected happens move to main menu
                default:
                state = STATE_IDLE;
                break;
            }
            break;
        }
    }
    // Start ADC conversion
    ADCSRA |= (1<<ADSC);
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Switching between ADC Multiplexer Selection Registers for 
			 collecting value from each one
 **********************************************************************/
ISR(ADC_vect)
{
	// FSM
    switch(ADMUX)
    {
		// Configure ADC to convert PC0[A0] analog value
        case 0x40:
		// Copy ADC result to 16-bit variable
        soil_moisture_raw = ADC;
        ADMUX = 0x41;
        break;
        // Configure ADC to convert PC1[A1] analog value
        case 0x41:
		// Copy ADC result to 16-bit variable
        light_level_raw = ADC;
        ADMUX = 0x42;
        break;
        // Configure ADC to convert PC2[A2] analog value
        case 0x42:
		// Copy ADC result to 16-bit variable
        button_pressed_adc = ADC;
        ADMUX = 0x40;
        break;
        /* If something unexpected happens configure ADC to convert
		 * PC0[A0] analog value */
        default:
        ADMUX = 0x40;
        break;
    }
}    
