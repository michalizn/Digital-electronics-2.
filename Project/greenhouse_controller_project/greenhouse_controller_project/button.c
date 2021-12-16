/*
 * button.c
 *
 * Library for button conversion
 *
 * Created: 15. 12. 2021 0:19:46
 *  Author: Alexander Bekeč
 */ 

#include "button.h"
#include <stdlib.h>

uint8_t button_analog(uint16_t adc_input_value)
{
    uint8_t return_value = 0;
    if (adc_input_value >= 0 && adc_input_value < 51) {
        return_value = 6; //right
    }
    else if (adc_input_value >= 51 && adc_input_value < 174) {
        return_value = 8; //up
    }
    else if (adc_input_value >= 174 && adc_input_value < 325) {
        return_value = 2; //down
    }
    else if (adc_input_value >= 325 && adc_input_value < 527) {
        return_value = 4; //left
    }
    else if (adc_input_value >= 527 && adc_input_value < 837) {
        return_value = 5; //select
    }
    else if (adc_input_value >= 837 && adc_input_value < 1024) {
        return_value = 0; //none
    }
    else {
        return_value = 255; //error
    }
    
    return return_value;
};
