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
   * Function => Something that CPU do normally all the time.
   * Interrupt service routine => Some interrupt from some peripherals, which cause some behivour to CPU, that have to stop a function a do the Interrupt service routine.

2. Part of the header file listing with syntax highlighting, which defines settings for Timer/Counter0:

```c
/**
 * @name  Definitions of Timer/Counter0
 * @note  F_CPU = 16 MHz
 */
/** @brief Stop timer, prescaler 000 --> STOP */
#define TIM1_stop()           TCCR0B &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));
/** @brief Set overflow 16u, prescaler 001 --> 1 */
#define TIM1_overflow_4ms()   TCCR0B &= ~((1<<CS02) | (1<<CS01)); TCCR1B |= (1<<CS00);
/** @brief Set overflow 128u, prescaler 010 --> 8 */
#define TIM1_overflow_33ms()  TCCR0B &= ~((1<<CS02) | (1<<CS00)); TCCR1B |= (1<<CS01);
/** @brief Set overflow 1024u, prescaler 011 --> 64 */
#define TIM1_overflow_262ms() TCCR0B &= ~(1<<CS02); TCCR0B |= (1<<CS01) | (1<<CS10);
/** @brief Set overflow 4096u, prescaler 100 --> 256 */
#define TIM1_overflow_1s()    TCCR0B &= ~((1<<CS01) | (1<<CS00)); TCCR1B |= (1<<CS02);
/** @brief Set overflow 16384u, prescaler // 101 --> 1024 */
#define TIM1_overflow_4s()    TCCR0B &= ~(1<<CS01); TCCR0B |= (1<<CS02) | (1<<CS00);
/** @brief Enable overflow interrupt, 1 --> enable */
#define TIM1_overflow_interrupt_enable()  TIMSK0 |= (1<<TOIE0);
/** @brief Disable overflow interrupt, 0 --> disable */
#define TIM1_overflow_interrupt_disable() TIMSK0 &= ~(1<<TOIE0);
```

3. Flowchart figure for function `main()` and interrupt service routine `ISR(TIMER1_OVF_vect)` of application that ensures the flashing of one LED in the timer interruption. When the button is pressed, the blinking is faster, when the button is released, it is slower. Use only a timer overflow and not a delay library.

   ![your figure]()


### Knight Rider

1. Scheme of Knight Rider application with four LEDs and a push button, connected according to Multi-function shield. Connect AVR device, LEDs, resistors, push button, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure]()
