/*
 * lcd_menu.c
 *
 * Created: 08.12.2021 11:40:18
 *  Author:
 */ 

#include "lcd_menu.h"
#include "lcd_definitions.h"
#include "lcd.h"
#include <stdlib.h>

void lcd_main_menu(uint8_t humidity_integer, uint8_t temperature_integer, uint8_t soil_humidity, uint8_t light_level)
{
    char lcd_hum_int[] = "0000";
    char lcd_temp_int[] = "0000";
    char lcd_soil_hum[] = "0000";
    char lcd_light_level[] = "0000";
    
    itoa(humidity_integer, lcd_hum_int, 10);
    itoa(temperature_integer, lcd_temp_int, 10);
    itoa(soil_humidity, lcd_soil_hum, 10);
    itoa(light_level, lcd_light_level, 10);
    
    lcd_gotoxy(0,0);
    lcd_puts("AH:");
    lcd_puts(lcd_hum_int);
    lcd_puts("%");
    lcd_gotoxy(8,0);
    lcd_puts("SM:");
    lcd_puts(lcd_soil_hum);
    lcd_puts("%");
    lcd_gotoxy(0,1);
    lcd_puts("AT:");
    lcd_puts(lcd_temp_int);
    lcd_puts("C");
    lcd_gotoxy(8,1);
    lcd_puts("LL:");
    lcd_puts(lcd_light_level);
    lcd_puts("%");
};

void lcd_humidity_menu(uint8_t humidity_integer, uint8_t humidity_fractional, uint8_t humidity_limit)
{
    char lcd_hum_int[] = "0000";
    char lcd_hum_frac[] = "0000";    
    char lcd_hum_limit[] = "0000";
    
    itoa(humidity_integer, lcd_hum_int, 10);
    itoa(humidity_fractional, lcd_hum_frac, 10);
    itoa(humidity_limit, lcd_hum_limit, 10);
    
    lcd_gotoxy(0,0);
    lcd_puts("Air Hum.: ");
    lcd_puts(lcd_hum_int);
    lcd_puts(".");
    lcd_puts(lcd_hum_frac);
    lcd_puts("%");
    lcd_gotoxy(0,1);
    lcd_puts("Nominal: ");
    lcd_puts(lcd_hum_limit);
    lcd_puts("%");
};

void lcd_temperature_menu(uint8_t temperature_integer, uint8_t temperature_fractional, uint8_t temperature_limit)
{
    char lcd_temp_int[] = "0000";
    char lcd_temp_frac[] = "0000";
    char lcd_temp_limit[] = "0000";
    
    itoa(temperature_integer, lcd_temp_int, 10);
    itoa(temperature_fractional, lcd_temp_frac, 10);
    itoa(temperature_limit, lcd_temp_limit, 10);
    
    lcd_gotoxy(0,0);
    lcd_puts("Air Temp.: ");
    lcd_puts(lcd_temp_int);
    lcd_puts(".");
    lcd_puts(lcd_temp_frac);
    lcd_puts("C");
    lcd_gotoxy(0,1);
    lcd_puts("Nominal: ");
    lcd_puts(lcd_temp_limit);
    lcd_puts("%");
};

void lcd_soil_humidity_menu(uint8_t soil_humidity, uint8_t soil_humidity_limit)
{
    char lcd_soil_hum_int[] = "0000";
    char lcd_soil_hum_limit[] = "0000";
    
    itoa(soil_humidity, lcd_soil_hum_int, 10);
    itoa(soil_humidity_limit, lcd_soil_hum_limit, 10);
    
    lcd_gotoxy(0,0);
    lcd_puts("Soil Moist.: ");
    lcd_puts(lcd_soil_hum_int);
    lcd_puts("%");
    lcd_gotoxy(0,1);
    lcd_puts("Nominal: ");
    lcd_puts(lcd_soil_hum_limit);
    lcd_puts("%");
};

void lcd_light_level_menu(uint8_t light_level, uint8_t light_level_limit)
{
    char lcd_light_level_int[] = "0000";
    char lcd_light_level_limit[] = "0000";
    
    itoa(light_level, lcd_light_level_int, 10);
    itoa(light_level_limit, lcd_light_level_limit, 10);
    
    lcd_gotoxy(0,0);
    lcd_puts("Soil Moist.: ");
    lcd_puts(lcd_light_level_int);
    lcd_puts("%");
    lcd_gotoxy(0,1);
    lcd_puts("Nominal: ");
    lcd_puts(lcd_light_level_limit);
    lcd_puts("%");
};

