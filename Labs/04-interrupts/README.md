# Lab 4: Michal Baranek

Link to your `Digital-electronics-2` GitHub repository:

   (https://github.com/michalizn/Digital-electronics-2)


### Overflow times

1. Complete table with overflow times.

| **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| Timer/Counter0 | 8  | 16u | 128u | -- | 1024u | -- | 4096u | 16384u |
| Timer/Counter1 | 16 | 4.096m | 32.8m | -- | 262.2m | -- | 1.05 | 4.2 |
| Timer/Counter2 | 8  | 16u | 128u | -- | 1024u | -- | 4096u | 16384u |

### Timer library

1. In your words, describe the difference between common C function and interrupt service routine.
   * Function => function is invoked by execution of instructions, which perform the specific tasks. It can be called from anywhere in program.
   * Interrupt service routine => interrupt is initiated by an internal or a external signal. After storing the current status of the program in the stack, the ISR is executed. ISR performs different types of tasks depending on the device which interrupted or instructions written by a programmer.

2. Part of the header file listing with syntax highlighting, which defines settings for Timer/Counter0:

```c
/**
 * @name  Definitions of Timer/Counter0
 * @note  F_CPU = 16 MHz
 */
#define TIM0_stop()           TCCR0B &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));
/** @brief Set overflow 16u, prescaler 001 --> 1 */
#define TIM0_overflow_16u()   TCCR0B &= ~((1<<CS02) | (1<<CS01)); TCCR1B |= (1<<CS00);
/** @brief Set overflow 128u, prescaler 010 --> 8 */
#define TIM0_overflow_128u()  TCCR0B &= ~((1<<CS02) | (1<<CS00)); TCCR1B |= (1<<CS01);
/** @brief Set overflow 1024u, prescaler 011 --> 64 */
#define TIM0_overflow_1024u() TCCR0B &= ~(1<<CS02); TCCR0B |= (1<<CS01) | (1<<CS00);
/** @brief Set overflow 4096u, prescaler 100 --> 256 */
#define TIM0_overflow_4096u()    TCCR0B &= ~((1<<CS01) | (1<<CS00)); TCCR1B |= (1<<CS02);
/** @brief Set overflow 16384u, prescaler // 101 --> 1024 */
#define TIM0_overflow_16384u()    TCCR0B &= ~(1<<CS01); TCCR0B |= (1<<CS02) | (1<<CS00);
/** @brief Enable overflow interrupt, 1 --> enable */
#define TIM0_overflow_interrupt_enable()  TIMSK0 |= (1<<TOIE0);
/** @brief Disable overflow interrupt, 0 --> disable */
#define TIM0_overflow_interrupt_disable() TIMSK0 &= ~(1<<TOIE0);

```

3. Flowchart figure for function `main()` and interrupt service routine `ISR(TIMER1_OVF_vect)` of application that ensures the flashing of one LED in the timer interruption. When the button is pressed, the blinking is faster, when the button is released, it is slower. Use only a timer overflow and not a delay library.

   ![Flowchart](https://github.com/michalizn/Digital-electronics-2/blob/main/Labs/04-interrupts/images/aa.drawio.png)


### Knight Rider

1. Scheme of Knight Rider application with four LEDs and a push button, connected according to Multi-function shield. Connect AVR device, LEDs, resistors, push button, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![Scheme](https://github.com/michalizn/Digital-electronics-2/blob/main/Labs/04-interrupts/images/Capture.PNG)
