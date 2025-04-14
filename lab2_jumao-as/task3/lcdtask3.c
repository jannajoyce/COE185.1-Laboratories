/*
  lcd.c - Routines for sending data and commands to the LCD shield
*/

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"                // Declarations of the LCD functions


/* This function not declared in lcd.h since
   should only be used by the routines in this file. */
void lcd_writenibble(unsigned char);


/* Define a couple of masks for the bits in Port B and Port D */
#define DATA_BITS ((1 << PD7)|(1 << PD6)|(1 << PD5)|(1 << PD4))
#define CTRL_BITS ((1 << PB1)|(1 << PB0))


/*
  lcd_init - Do various things to initialize the LCD display
*/
void lcd_init(void) {
    // Set PB0 and PB1 as outputs for control bits, leaving other bits unchanged
    DDRB |= CTRL_BITS;


    // Set PD4-PD7 as outputs for data bits, leaving other bits unchanged
    DDRD |= DATA_BITS;


    _delay_ms(15);              // Delay at least 15ms


    lcd_writenibble(0x30);      // Send 0b0011
    _delay_ms(5);               // Delay at least 4ms


    lcd_writenibble(0x30);      // Send 0b0011 again
    _delay_us(110);             // Delay at least 100us


    lcd_writenibble(0x30);      // Send 0b0011, no delay


    lcd_writenibble(0x20);      // Send 0b0010 to switch to 4-bit mode
    _delay_ms(2);               // Delay at least 2ms


    lcd_writecommand(0x28);     // Function Set: 4-bit interface, 2 lines


    lcd_writecommand(0x0f);     // Display on, cursor on, blink on
}


/*
  lcd_writecommand - Output a byte to the LCD command register.
*/
void lcd_writecommand(unsigned char cmd) {
    /* Clear PB0 to 0 for a command transfer */
    PORTB &= ~(1 << PB0);


    /* Call lcd_writenibble to send UPPER four bits of "cmd" argument */
    lcd_writenibble(cmd >> 4);


    /* Call lcd_writenibble to send LOWER four bits of "cmd" argument */
    lcd_writenibble(cmd);


    /* Delay 2ms */
    _delay_ms(2);
}


/*
  lcd_writedata - Output a byte to the LCD data register
*/
void lcd_writedata(unsigned char dat) {
    /* Set PB0 to 1 for a data transfer */
    PORTB |= (1 << PB0);


    /* Call lcd_writenibble to send UPPER four bits of "dat" argument */
    lcd_writenibble(dat >> 4);


    /* Call lcd_writenibble to send LOWER four bits of "dat" argument */
    lcd_writenibble(dat);


    /* Delay 2ms */
    _delay_ms(2);
}

/*
  lcd_writenibble - Output the UPPER four bits of "lcdbits" to the LCD
*/
void lcd_writenibble(unsigned char lcdbits) {
    /* Load PORTD, bits 7-4 with bits 7-4 of "lcdbits" */
    PORTD &= ~DATA_BITS;        // Clear the upper 4 bits of PORTD
    PORTD |= lcdbits & DATA_BITS;  // Set the upper 4 bits with lcdbits

    /* Make E signal (PB1) go to 1 and back to 0 */
    PORTB |= (1 << PB1);        // Set E to 1

    _delay_us(1);               // Short delay
    //PORTB |= (1 << PB1);        // Make E longer (still 1)
   
    PORTB &= ~(1 << PB1);       // Set E to 0
}
