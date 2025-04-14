#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>
#include "lcd.h"


#define BUZZER_PIN PB4 // Define the buzzer pin
#define ENCODER_A PC1 // Define the encoder output A pin
#define ENCODER_B PC5 // Define the encoder output B pin

void setup() {
    // Set up the buzzer pin as an output
    DDRB |= (1 << BUZZER_PIN);
    
    // Set up the encoder pins as inputs
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B));
    
    // Enable pull-up resistors on encoder pins
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B);
}

void loop() {
    // Sample both encoder pins at the same time
    uint8_t x = PINC & ((1 << ENCODER_A) | (1 << ENCODER_B));
    
    // Determine the individual values of A and B from the sampled value
    uint8_t a = (x & (1 << ENCODER_A)) >> ENCODER_A;
    uint8_t b = (x & (1 << ENCODER_B)) >> ENCODER_B;

    // Logic to determine the direction of rotation
    if (a && !b) {
        // Clockwise rotation logic
        PORTB |= (1 << BUZZER_PIN); // Turn on the buzzer
    } else if (!a && b) {
        // Counter-clockwise rotation logic
        PORTB &= ~(1 << BUZZER_PIN); // Turn off the buzzer
    }

    // Add a small delay to avoid bouncing
    _delay_ms(50);
}

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

    setup();
    while (1) {
        loop();
    }
}
