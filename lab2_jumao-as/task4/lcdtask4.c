/*
  lcd.c - Routines for sending data and commands to the LCD shield
*/


#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"  // Declarations of the LCD functions


/* This function is not declared in lcd.h since it should only be used by the routines in this file. */
static void lcd_writenibble(unsigned char);


/* Define masks for the bits in Port B and Port D */
#define DATA_BITS ((1 << PD7) | (1 << PD6) | (1 << PD5) | (1 << PD4))
#define CTRL_BITS ((1 << PB1) | (1 << PB0))


/*
  lcd_init - Do various things to initialize the LCD display
*/
void lcd_init(void) {
    // Set PB0 and PB1 as outputs for control bits
    DDRB |= CTRL_BITS;


    // Set PD4-PD7 as outputs for data bits
    DDRD |= DATA_BITS;


    _delay_ms(15);  // Delay at least 15ms


    lcd_writenibble(0x30);  // Send 0b0011
    _delay_ms(5);            // Delay at least 4ms


    lcd_writenibble(0x30);  // Send 0b0011 again
    _delay_us(110);          // Delay at least 100us


    lcd_writenibble(0x30);  // Send 0b0011


    lcd_writenibble(0x20);  // Send 0b0010 to switch to 4-bit mode
    _delay_ms(2);            // Delay at least 2ms


    lcd_writecommand(0x28);  // Function Set: 4-bit interface, 2 lines
    lcd_writecommand(0x0C);  // Display on, cursor off, blink off
}


/*
  lcd_writecommand - Output a byte to the LCD command register.
*/
void lcd_writecommand(unsigned char cmd) {
    PORTB &= ~(1 << PB0);  // Clear PB0 for command transfer
    lcd_writenibble(cmd >> 4);  // Send upper 4 bits
    lcd_writenibble(cmd);  // Send lower 4 bits
    _delay_ms(2);
}


/*
  lcd_writedata - Output a byte to the LCD data register
*/
void lcd_writedata(unsigned char dat) {
    PORTB |= (1 << PB0);  // Set PB0 for data transfer
    lcd_writenibble(dat >> 4);  // Send upper 4 bits
    lcd_writenibble(dat);  // Send lower 4 bits
    _delay_ms(2);
}


/*
  lcd_writenibble - Output the UPPER four bits of "lcdbits" to the LCD
*/
void lcd_writenibble(unsigned char lcdbits) {
    /* Load PORTD, bits 7-4 with bits 7-4 of "lcdbits" */
    PORTD &= ~DATA_BITS;        // Clear the upper 4 bits of PORTD
    PORTD |= (lcdbits << 4) & DATA_BITS;  // Set the upper 4 bits with lcdbits

    /* Make E signal (PB1) go to 1 and back to 0 */
    PORTB |= (1 << PB1);        // Set E to 1

    _delay_us(1);               // Short delay

    PORTB &= ~(1 << PB1);       // Set E to 0
}
