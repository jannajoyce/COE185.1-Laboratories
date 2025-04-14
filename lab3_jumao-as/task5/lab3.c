#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"

#define ENCODER_A PC1
#define ENCODER_B PC5

// Tone frequencies for beeps
uint16_t frequency[8] = { 262, 294, 330, 349, 392, 440, 494, 523 };

// Global variables for encoder state and count
volatile int16_t count = 0;      // Counter value
uint8_t prev_state = 0;          // Previous encoder state
uint8_t is_beeping = 0;          // Flag to indicate if a beep has already been played at the current count

// Function to play a single note
void play_note(uint16_t freq) {
    if (freq == 0) return;  // No frequency, no note

    uint16_t period = 1000000 / freq;      // Period in microseconds
    uint16_t half_period = period / 2;     // Half period for a 50% duty cycle

    for (uint16_t i = 0; i < 200; i++) {   // Play the tone for a short time
        PORTB |= (1 << PB4);               // Set PB4 high
        _delay_us(half_period);            // Wait for half the period
        PORTB &= ~(1 << PB4);              // Set PB4 low
        _delay_us(half_period);            // Wait for the other half
    }
}

void setup() {
    DDRB |= (1 << PB4);                   // Set PB4 as output for the buzzer
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B));  // Set encoder pins as inputs
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B);    // Enable pull-up resistors

    lcd_init();                           // Initialize the LCD
    lcd_clear();
    lcd_moveto(0, 0);
    lcd_stringout("Lab 3");
        lcd_moveto(1, 0);
    lcd_stringout("Janna Jumao-as");
    _delay_ms(1000);
    lcd_clear();
}

// Function to update the encoder state and count
void update_encoder() {
    // Read current state of encoder
    uint8_t a = (PINC & (1 << ENCODER_A)) ? 1 : 0;
    uint8_t b = (PINC & (1 << ENCODER_B)) ? 1 : 0;
    uint8_t current_state = (a << 1) | b; // Combine states into a 2-bit number

    // Check if encoder state has changed (debounced)
    if (current_state != prev_state) {
        // Determine the direction of rotation
        if ((prev_state == 0b00 && current_state == 0b01) ||
            (prev_state == 0b01 && current_state == 0b11) ||
            (prev_state == 0b11 && current_state == 0b10) ||
            (prev_state == 0b10 && current_state == 0b00)) {
            count++;  // Clockwise
        } 
        else if ((prev_state == 0b00 && current_state == 0b10) ||
                 (prev_state == 0b10 && current_state == 0b11) ||
                 (prev_state == 0b11 && current_state == 0b01) ||
                 (prev_state == 0b01 && current_state == 0b00)) {
            count--;  // Counter-clockwise
        }

        prev_state = current_state;  // Update previous state
        is_beeping = 0;              // Reset beeping flag
        _delay_ms(2);                // Short delay for debouncing
    }
}

int main(void) {
    setup();

    while (1) {
        // Poll the encoder state and update count
        update_encoder();

        // Display the current count on the LCD
        char buf[16];
        snprintf(buf, sizeof(buf), " %d", count);
        lcd_clear();
        lcd_moveto(0, 0);
        lcd_stringout(buf);

        // Play tone only when the count is a multiple of 8
        if (abs(count) % 8 == 0 && !is_beeping) {
            is_beeping = 1;  // Set the beeping flag to avoid repeated beeps

            // Determine the note based on the count value
            int note_index = (abs(count) % 64) / 8;
            play_note(frequency[note_index]);
        }

        _delay_ms(50);  // Short delay to reduce polling frequency
    }
}
