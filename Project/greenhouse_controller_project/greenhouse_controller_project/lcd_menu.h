/*
 * lcd_print.h
 *
 * Created: 08.12.2021 11:32:06
 *  Author:
 */ 


#ifndef LCD_PRINT_H_
#define LCD_PRINT_H_

#include <avr/io.h>

void lcd_main_menu(uint8_t humidity_integer, uint8_t temperature_integer, uint8_t soil_humidity, uint8_t light_level);
void lcd_humidity_menu(uint8_t humidity_integer, uint8_t humidity_fractional, uint8_t humidity_limit);
void lcd_temperature_menu(uint8_t temperature_integer, uint8_t temperature_fractional, uint8_t temperature_limit);
void lcd_soil_moisture_menu(uint8_t soil_moisture, uint8_t soil_moisture_limit);
void lcd_light_level_menu(uint8_t light_level, uint8_t light_level_limit);

#endif /* LCD_PRINT_H_ */