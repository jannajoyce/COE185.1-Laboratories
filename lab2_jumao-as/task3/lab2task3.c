/********************************************
*
*  Name: Janna Joyce E. Jumao-as
*  Email: jannajoyce.jumao-as@g.msuiit.edu.ph
*  Section: M89
*  Assignment: Lab 2 - LCD Display Test
*
********************************************/


#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"


int main(void) {
    // Initialize the LCD
    lcd_init();


    // Infinite loop
    while (1) {
        lcd_writedata(0); // Send data to generate E pulses

       PORTB |= (1 << PB1);
       PORTB |= (1 << PB1);
       PORTB &= ~(1 << PB1);
    }


    return 0;
}
