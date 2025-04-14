#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include "lcd.h"

// Frequencies for natural notes  from middle C (C4)
uint16_t frequency[8] = { 262, 294, 330, 349, 392, 440, 494, 523 };

int main(void) {
    // Initialize DDR and PORT registers and LCD
    lcd_init();

    // Display splash screen
    lcd_moveto(0, 0); // Move to the first row, first column
    lcd_stringout("Janna Jumao-as"); // Replace "Your Name" with your actual name

    // Use snprintf to create a string with your birthdate
    char birthdate[16];
    snprintf(birthdate, sizeof(birthdate), "%02d/%02d/%d", 01, 27, 2003); // Replace with your birthdate (MM/DD/YYYY)

    // Move to the second row, centered
    lcd_moveto(1, (16 - strlen(birthdate)) / 2); // Center the birthdate
    lcd_stringout(birthdate); // Display the birthdate

    // Delay for 1 second
    _delay_ms(2000);

    // Clear the screen
    lcd_writecommand(LCD_CLR);

    while(1) {
        // Your main loop code here
    }
}
