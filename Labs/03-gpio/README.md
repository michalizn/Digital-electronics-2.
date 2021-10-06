# Lab 3: Michal Baranek

Link to your `Digital-electronics-2` GitHub repository:

   (https://github.com/michalizn/Digital-electronics-2)


### Data types in C

1. Complete table.

| **Data type** | **Number of bits** | **Range** | **Description** |
| :-: | :-: | :-: | :-- | 
| `uint8_t`  | 8 | 0, 1, 2 to 255 | Unsigned 8-bit integer |
| `int8_t`   | 8 | -128, -127, 126 to 127 | Signed 8-bit integer |
| `uint16_t` | 16 | 0, 1, 2 to 65,535 | Unsigned 16-bit integer |
| `int16_t`  | 16 | -32,768, -32,767 to 32,767 | Signed 16-bit integer |
| `float`    | 32 | -3.4e+38, ..., 3.4e+38 | Single-precision floating-point |
| `void`     | 0 | 0 | Function return type |


### GPIO library

1. In your words, describe the difference between the declaration and the definition of the function in C.
   * Function declaration =>  function declaration tells the compiler about what should the function return, tells the name of the function and how to call it. For example a function declaration looks like this return_type function_name( parameter list ); (Parameter names are not important just their type)
   * Function definition => function definition gives us the actual "body" of a function (code in brackets under the function declaration)

2. Part of the C code listing with syntax highlighting, which toggles LEDs only if push button is pressed. Otherwise, the value of the LEDs does not change. Use function from your GPIO library. Let the push button is connected to port D:

```c
int main(void)
{
    // Green LED at port B
    GPIO_config_output(&DDRB, LED_GREEN);
    GPIO_write_low(&PORTB, LED_GREEN);

    // Configure the second LED at port C
    GPIO_config_output(&DDRC, LED_BLUE);
    GPIO_write_low(&PORTC, LED_BLUE);

    // Configure Push button at port D and enable internal pull-up resistor
    GPIO_config_input_pullup(&DDRD, PUSH_BUTTON);

    // Infinite loop
    while (1)
    {
        uint8_t BUTTON_TRIGGER = GPIO_read(&PIND, PUSH_BUTTON);
        
        if(BUTTON_TRIGGER == 0)
        {
            // Pause several milliseconds
            _delay_ms(BLINK_DELAY);
            GPIO_toggle(&PORTB, LED_GREEN);
            
            // Pause several milliseconds
            _delay_ms(BLINK_DELAY);
            GPIO_toggle(&PORTC, LED_BLUE);
        }
    }

    // Will never reach this
    return 0;
}
```


### Traffic light

1. Scheme of traffic light application with one red/yellow/green light for cars and one red/green light for pedestrians. Connect AVR device, LEDs, resistors, one push button (for pedestrians), and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure]()
