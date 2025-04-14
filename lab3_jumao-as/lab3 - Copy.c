#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"
#include <avr/interrupt.h>
#include <stdlib.h>

#define ENCODER_A PC1
#define ENCODER_B PC5

// Tone frequencies for beep
uint16_t frequency[8] = { 262, 294, 330, 349, 392, 440, 494, 523 };

volatile int16_t count = 0;        // Counter value
volatile uint8_t old_state = 0;    // Previous state of the encoder
volatile uint8_t new_state = 0;    // Current state of the encoder
volatile uint8_t beeping = 0;      // Flag to indicate if beeping is in progress

void play_note(uint16_t freq) {
    if (freq == 0) return; // No frequency, no note

    uint16_t period = 1000000 / freq; // Period in microseconds
    uint16_t half_period = period / 2; // Half period for a 50% duty cycle

    for (uint16_t i = 0; i < freq; i++) { // Loop for the number of cycles based on frequency
        PORTB |= (1 << PB4);  // Set PB4 high
        _delay_us(half_period); // Wait for half period
        PORTB &= ~(1 << PB4); // Set PB4 low
        _delay_us(half_period); // Wait for the other half period
    }
}

void setup() {
    DDRB |= (1 << PB4); // Set PB4 as output for the buzzer
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B)); // Set encoder pins as inputs
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B); // Enable pull-up resistors

    // Enable pin change interrupts for the encoder
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT13);

    lcd_init(); // Initialize the LCD
    lcd_clear();
    lcd_moveto(0, 0);
    lcd_stringout("Lab 3");
        lcd_moveto(1, 0);
    lcd_stringout("Jejy M. Borres");
    _delay_ms(1000);
    lcd_clear();
}

ISR(PCINT1_vect) {
    // Read encoder state
    uint8_t a = (PINC & (1 << ENCODER_A)) ? 1 : 0; // Read encoder A
    uint8_t b = (PINC & (1 << ENCODER_B)) ? 1 : 0; // Read encoder B

    new_state = (a << 1) | b; // Create new state

    // Update count based on encoder direction
    if (new_state != old_state) {
        if ((old_state == 0 && new_state == 1) ||
            (old_state == 1 && new_state == 3) ||
            (old_state == 3 && new_state == 2) ||
            (old_state == 2 && new_state == 0)) {
            // Clockwise rotation
            count++;
        } else {
            // Counter-clockwise rotation
            count--;
        }

        // Calculate note index based on (|count| % 64) / 8
        int note_index = (abs(count) % 64) / 8;

        // Update LCD display with the current count
        char buf[16];
        snprintf(buf, sizeof(buf), "Count: %d", count);
        lcd_clear();
        lcd_moveto(0, 0);
        lcd_stringout(buf);

        // Trigger beep if count is divisible by 8 and not currently beeping
        if (abs(count) % 8 == 0 && !beeping) {
            beeping = 1; // Set the beeping flag
            play_note(frequency[note_index]); // Play the corresponding note
            beeping = 0; // Reset the beeping flag after playing the note
        }
    }

    old_state = new_state; // Update old state
}

int main(void) {
    setup();
    sei(); // Enable global interrupts
    while (1) {
        // Main loop can remain empty since functionality is interrupt-driven
    }
}
