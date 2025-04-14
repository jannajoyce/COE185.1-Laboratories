#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include "lcd.h"

#define ENCODER_A PC1
#define ENCODER_B PC5

void setup() {

    
    // Set encoder pins as inputs
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B));
    // Enable pull-up resistors
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B);
    // Initialize LCD
    lcd_init();
}

uint8_t debounce(uint8_t pin) {
    // Read the pin state
    uint8_t state = PINC & (1 << pin);
    _delay_ms(10); // Delay for debouncing
    return (PINC & (1 << pin)) == state ? state : 0; // Confirm the pin state
}

void loop() {
    static uint8_t lastA = 0; // Store last state of A
    static uint8_t lastB = 0; // Store last state of B
    static uint8_t lastOutput[16]; // Store last output string

    // Read the current state of the encoder
    uint8_t a = debounce(ENCODER_A) ? 1 : 0;
    uint8_t b = debounce(ENCODER_B) ? 1 : 0;

    // Prepare the output string
    char buf[16];
    snprintf(buf, sizeof(buf), "A=%d B=%d", a, b);

    // Check if the encoder state has changed
    if (a != lastA || b != lastB || strcmp(lastOutput, buf) != 0) {
        // Clear the LCD and display the new output only if there is a change
        lcd_clear();
        lcd_moveto(0, 0);
        lcd_stringout(buf);
        
        // Update the last state and output
        lastA = a;
        lastB = b;
        strcpy(lastOutput, buf); // Copy current output to lastOutput
    }

    // Delay to control update rate
    _delay_ms(50); // Reduced delay for smoother responsiveness
}

int main(void) {
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

    setup();
    while (1) {
        loop();
    }
}
