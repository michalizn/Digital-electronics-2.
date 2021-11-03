# Lab 7: Michal Baranek

Link to this file in your GitHub repository:

(https://github.com/michalizn/Digital-electronics-2/tree/main/Labs/07-uart)


### Analog-to-Digital Conversion

1. Complete table with voltage divider, calculated, and measured ADC values for all five push buttons.

   | **Push button** | **PC0[A0] voltage** | **ADC value (calculated)** | **ADC value (measured)** |
   | :-: | :-: | :-: | :-: |
   | Right  | 0&nbsp;V | 0 | 0 |
   | Up     | 0.495&nbsp;V | 101 | 98 |
   | Down   | 1.203&nbsp;V | 246 | 256 |
   | Left   | 1.969&nbsp;V | 402 | 409 |
   | Select | 3.182&nbsp;V | 651 | 639 |
   | none   | 5&nbsp;V | 1023 | 1023 |
   
### Table for ADC converter register

   | **Operation** | **Register(s)** | **Bit(s)** | **Description** |
   | :-: | :-- | :-- | :-- |
   | Voltage reference | ADMUX | REFS1:0 | 01: AVcc voltage reference, 5V |
   | Input channel | ADMUX | MUX3:0 | 0000: ADC0, 0001: ADC1, ... |
   | ADC enable | ADCSRA | ADEN | 1: ADC Enable, 0: ADC Disable |
   | Start conversion | ADCSRA | ADSC | 1:Start conversion, when conversion complete, returns to zero |
   | ADC interrupt enable | ADCSRA | ADIE | 1: first bit in SREG is set and the ADC Conversion Complete Interrupt is activated |
   | ADC clock prescaler | ADCSRA | ADPS2:0 | 000: Division factor 2, 001: 2, 010: 4, ...|
   | ADC result | ADC | ADCL7:0 and ADCH7:0 | conversion result |

### Buttons connection

![Buttons](https://github.com/michalizn/Digital-electronics-2/blob/main/Labs/07-uart/images/Capture.PNG)

2. Code listing of ACD interrupt service routine for sending data to the LCD/UART and identification of the pressed button. Always use syntax highlighting and meaningful comments:

```c
/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display value on LCD and send it to UART.
 **********************************************************************/
ISR(ADC_vect)
{
    // WRITE YOUR CODE HERE
    uint16_t value = 0;
    char lcd_string[4] = "0000";
    char lcd_string_hex[4] = "0000";

    value = ADC;                      // Copy ADC result to 16-bit variable
    itoa(value, lcd_string, 10);      // Convert hex(16)/decimal(10)/bin(2) value to string
    itoa(value, lcd_string_hex, 16);  // Convert hex(16)/decimal(10)/bin(2) value to string

    // WRITE YOUR CODE HERE
    uart_puts(lcd_string);
    uart_puts("\r\n");
    uart_puts(lcd_string_hex);
    uart_puts("\r\n");
    
    lcd_gotoxy(8, 0);
    lcd_puts("    ");
    lcd_gotoxy(8, 0);
    lcd_puts(lcd_string);
    lcd_gotoxy(12, 0);
    lcd_puts(" ");
    lcd_gotoxy(13, 0);
    lcd_puts(lcd_string_hex);
}
```


### UART communication

1. (Hand-drawn) picture of UART signal when transmitting three character data `De2` in 4800 7O2 mode (7 data bits, odd parity, 2 stop bits, 4800&nbsp;Bd).

   ![your figure]()

2. Flowchart figure for function `uint8_t get_parity(uint8_t data, uint8_t type)` which calculates a parity bit of input 8-bit `data` according to parameter `type`. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![your figure]()

### Temperature meter

Consider an application for temperature measurement and display. Use temperature sensor [TC1046](http://ww1.microchip.com/downloads/en/DeviceDoc/21496C.pdf), LCD, one LED and a push button. After pressing the button, the temperature is measured, its value is displayed on the LCD and data is sent to the UART. When the temperature is too high, the LED will start blinking.

1. Scheme of temperature meter. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![your figure]()
