# Lab 6: Michal Baranek

Link to your `Digital-electronics-2` GitHub repository:

(https://github.com/michalizn/Digital-electronics-2/tree/main/Labs/06-lcd)


### LCD display module

## Connection of LCD pins and meaning of affiliated signals

  | **LCD signal(s)** | **AVR pin(s)** | **Description** |
   | :-: | :-: | :-- |
   | RS | PB0 | Register selection signal. Selection between Instruction register (RS=0) and Data register (RS=1) |
   | R/W | GND | Read/Write selection |
   | E | PB1 | Enable |
   | D[3:0] | Not connected | Lower nibble for data byte |
   | D[7:4] | PD[7-4] | Higher nibble for data byte |
## ASCII
ASCII is American Standard Code for Information Interchange. It is a table treating english characters and other mostly used characters.
![ASCII](https://github.com/michalizn/Digital-electronics-2/blob/main/Labs/06-lcd/pictures/1200px-ASCII_Code_Chart.svg.png)
Values for uppercase 'A' to 'Z' are 0x40 to 0x5a, 'a' to 'z' are 0x61 to 0x7a, '0' to '9' are 0x30 to 0x39.

## Input parameters and description of the functions

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `lcd_init` | `LCD_DISP_OFF`<br>`LCD_DISP_ON`<br>`LCD_DISP_ON_CURSOR`<br>`LCD_DISP_ON_CURSOR_BLINK` | Display off&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<br>&nbsp;<br>&nbsp;<br>&nbsp; | `lcd_init(LCD_DISP_OFF);`<br>&nbsp;<br>&nbsp;<br>&nbsp; |
   | `lcd_clrscr` | `VOID`| Clear display and set cursor to home position | `lcd_clrscr();` |
   | `lcd_gotoxy` | `x`(horizontal position)<br>`y(vertical position)` | Set cursor to specified position &nbsp;<br>&nbsp;| `lcd_gotoxy();`&nbsp;<br>&nbsp; |
   | `lcd_putc` | `c`(single character) | Display character at current cursor position | `lcd_putc('k');` |
   | `lcd_puts` | `a`(string) | Display string from program memory without auto linefeed | `lcd_puts(string_variable);` |
   | `lcd_command` | `cmd`(instruction to send to LCD controller | Send data byte to LCD controller | `lcd_command(1 << LCD_DDRAM);` |
   | `lcd_data` | `data`(byte to send to LCD controller) | Send data byte to LCD controller | `lcd_data(customChar[i]);` |

1. In your words, describe what ASCII table is.
   * ASCII => It's code table for translating our symbols to the computer language in ones and zeros

2. (Hand-drawn) picture of time signals between ATmega328P and LCD keypad shield (HD44780 driver) when transmitting three character data `De2`.

   ![your figure]()


### Stopwatch

1. Flowchart figure for `TIMER2_OVF_vect` interrupt service routine which overflows every 16&nbsp;ms but it updates the stopwatch LCD approximately every 100&nbsp;ms (6 x 16&nbsp;ms = 100&nbsp;ms). Display tenths of a second and seconds `00:seconds.tenths`. Let the stopwatch counts from `00:00.0` to `00:59.9` and then starts again. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![your figure]()


### Custom characters

1. Code listing with syntax highlighting of two custom character definition:

```c
/* Variables ---------------------------------------------------------*/
// Custom character definition
uint8_t customChar[16] = {
    // WRITE YOUR CODE HERE
    0b01110,
    0b10001,
    0b01110,
    0b00100,
    0b11111,
    0b00100,
    0b01010,
    0b11011,
    0b00000,
    0b00111,
    0b01110,
    0b11100,
    0b10000,
    0b00000,
    0b01110,
    0b11011
};
```


### Kitchen alarm

Consider a kitchen alarm with an LCD, one LED and three push buttons: start, +1 minute, -1 minute. Use the +1/-1 minute buttons to increment/decrement the timer value. After pressing the Start button, the countdown starts. The countdown value is shown on the display in the form of mm.ss (minutes.seconds). At the end of the countdown, the LED will start blinking.

1. Scheme of kitchen alarm; do not forget the supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![your figure]()
