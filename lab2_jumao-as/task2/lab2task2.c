/********************************************
*
*  Name: Janna Joyce E. Jumao-as
*  Email: jannajoyce.jumao-as@g.msuiit.edu.ph
*  Section: M89
*  Assignment: Lab 2 - Up/Down counter on LCD display
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

enum states { UP, DOWN, PAUSE };

int main(void) {

    // Setup DDR and PORT bits for the 3 input buttons as necessary

    // Initialize the LCD
   
    lcd_init();

    // Use a state machine approach to organize your code
    //   - Declare and initialize a variable to track what state you
    //     are in by assigning the values UP, DOWN or PAUSE to that variable.


        lcd_writedata(0x4D);//M
        lcd_writedata(0x53);//S
        lcd_writedata(0x55);//U

    while (1) {               // Loop forever

        // Use "if" statements to read the buttons and determine which
        // state you are in

	// Based on the current state, determine the new count value


	// Perform the output operations if necessary


        // Delay before we go to the next iteration of the loop

    }

    return 0;   /* never reached */
}

