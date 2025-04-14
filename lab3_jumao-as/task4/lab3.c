#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"
#include <avr/interrupt.h>

#define ENCODER_A PC1        // Define the encoder output A pin
#define ENCODER_B PC5        // Define the encoder output B pin

// Variables for holding encoder state information
volatile int16_t count = 1;  // Signed variable to store count value
volatile uint8_t old_state = 0; // Previous state of the encoder

void setup() {
    // Set up the encoder pins as inputs
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B));

    // Enable pull-up resistors on encoder pins
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B);

    // Enable pin change interrupts on PC1 and PC5
    PCICR |= (1 << PCIE1); // Enable pin change interrupt for PCIE1
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT13); // Enable PCINT for PC1 and PC5

    // Initialize the LCD
    lcd_init();

    // Splash screen
    lcd_clear();
    lcd_moveto(0, 0);
    lcd_stringout("Lab 3");
    lcd_moveto(1, 0);
    lcd_stringout("Janna Jumao-as");
    _delay_ms(1000);
    lcd_clear();

    // Initialize the count display
    char buf[16]; // Declare buffer for display
    snprintf(buf, sizeof(buf), "Count: %d", count);
    lcd_stringout(buf);
}

// Interrupt service routine for the pin change interrupt
ISR(PCINT1_vect) {
    uint8_t a = (PINC & (1 << ENCODER_A)) ? 1 : 0; // Read encoder A
    uint8_t b = (PINC & (1 << ENCODER_B)) ? 1 : 0; // Read encoder B

    // Combine states
    uint8_t new_state = (a << 1) | b; // Create new state (2 bits)

    // Determine direction
    if (new_state != old_state) {
        if ((old_state == 0 && new_state == 1) ||
            (old_state == 1 && new_state == 3) ||
            (old_state == 3 && new_state == 2) ||
            (old_state == 2 && new_state == 0)) {
            // Clockwise rotation
            count += 1; // Increment count
        } else {
            // Counterclockwise rotation
            count -= 1; // Decrement count
        }

        // Update the LCD with the new count
        char buf[16]; // Declare buffer for displaying the count
        snprintf(buf, sizeof(buf), " %d", count);
        lcd_clear();
        lcd_moveto(0, 0);
        lcd_stringout(buf);
    }

    // Update old state
    old_state = new_state;
}

void loop() {
    // Main loop does nothing; counting is handled by the interrupt
}

int main(void) {
    setup();
    sei(); // Enable global interrupts
    while (1) {
        loop();
    }
}
