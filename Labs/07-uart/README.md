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

### Analog to digital conversion to the UART transmitter

   | **Function name** | **Function parameter(s)** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `uart_init` | `UART_BAUD_SELECT(9600, F_CPU)` | Initialize UART to 8N1 and set baudrate to 9600&nbsp;Bd | `uart_init(UART_BAUD_SELECT(9600, F_CPU));` |
   | `uart_getc` |  |  |
   | `uart_putc` |  |  |
   | `uart_puts` |  |  |
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
    uint16_t value = 0;
    char lcd_string[4] = "0000";

    value = ADC;                  // Copy ADC result to 16-bit variable
    itoa(value, lcd_string, 10);  // Convert decimal value to string

    // WRITE YOUR CODE HERE
	 static char buttons[5][7] = 
	{
		"RIGHT ",
		"UP    ",
		"LEFT  ",
		"DOWN  ",
		"SELECT"
	};
	static char current_button[7];
    uint16_t value = 0;
    char lcd_string[4] = "    ";
	value = ADC;    // Copy ADC result to 16-bit variable
	lcd_gotoxy(7, 0);
	lcd_puts(lcd_string);
    itoa(value, lcd_string, 10);    // Convert to string in decimal
	lcd_gotoxy(7, 0);
	lcd_puts(lcd_string);
	lcd_gotoxy(12, 0);
	lcd_puts("   ");
	lcd_gotoxy(12, 0);
	itoa(value, lcd_string, 16);
	lcd_puts(lcd_string);
	if(value < 50)
	{
		for(int i = 0; i <= 7; i++)
		{
		current_button[i] = buttons[0][i];
		}
	}
	else if((value > 50) && (value < 180))
	{
		for(int i = 0; i <= 7; i++)
		{
			current_button[i] = buttons[1][i];
		}
	}
	else if((value > 180) && (value < 300))
	{
		for(int i = 0; i <= 7; i++)
		{
			current_button[i] = buttons[2][i];
		}
	}
	else if((value > 300) && (value < 500))
	{
		for(int i = 0; i <= 7; i++)
		{
			current_button[i] = buttons[3][i];
		}
	}
	else if((value > 500) && (value < 800))
	{
		for(int i = 0; i <= 7; i++)
		{
			current_button[i] = buttons[4][i];
		}
	}
	else
	{
		for(int i = 0; i <= 7; i++)
		{
			current_button[i] = ' ';
		}
	}
	uint16_t Voltage = value * 4.88;
	lcd_gotoxy(8, 1);
	lcd_puts(current_button);
	if(value < 1010)
	{
		uart_puts(current_button);
		uart_puts(" : ");
		itoa(value, lcd_string, 10);
		uart_puts(lcd_string);
		uart_puts(" @ ");
		itoa(Voltage, lcd_string, 10);
		uart_puts(lcd_string);
		uart_puts(" mV");
		uart_puts(" Parity: ");
		uint8_t parity = func_parity(value);
		lcd_gotoxy(13, 1);
		itoa(parity, lcd_string, 2);
		lcd_puts(lcd_string);
		uart_puts(lcd_string);
		uart_puts("\r\n");
	}
}
```


### UART communication

1. (Hand-drawn) picture of UART signal when transmitting three character data `De2` in 4800 7O2 mode (7 data bits, odd parity, 2 stop bits, 4800&nbsp;Bd).

   ![your figure]()

2. Flowchart figure for function `uint8_t get_parity(uint8_t data, uint8_t type)` which calculates a parity bit of input 8-bit `data` according to parameter `type`. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![your figure]()

```c
	int8_t func_parity( uint16_t n)
	{
		uint8_t parity = 0;
		while(n)
		{
			parity = !parity;
			n = n & (n-1);
		}
		return parity;
	}
```

### Temperature meter

Consider an application for temperature measurement and display. Use temperature sensor [TC1046](http://ww1.microchip.com/downloads/en/DeviceDoc/21496C.pdf), LCD, one LED and a push button. After pressing the button, the temperature is measured, its value is displayed on the LCD and data is sent to the UART. When the temperature is too high, the LED will start blinking.

1. Scheme of temperature meter. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![your figure]()
