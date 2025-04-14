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

// Declare global volatile variables for shared access
volatile int16_t count_ms = 0;        
volatile uint8_t old_state = 0;    
volatile uint8_t new_state = 0;   
volatile uint8_t beep = 0;     
volatile uint8_t changed = 0;      

void play_note(uint16_t freq) {
    if (freq == 0) return; // No frequency, no note

    uint16_t period = 1000000 / freq; 
    uint16_t half_period = period / 2; 

    for (uint16_t i = 0; i < freq; i++) { 
        PORTB |= (1 << PB4); 
        _delay_us(half_period); 
        PORTB &= ~(1 << PB4); 
        _delay_us(half_period); 
    }
}

void setup() {
    DDRB |= (1 << PB4); 
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B)); 
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B); 


    // Enable pin change interrupts for Port C (PCINT1) on PC1 and PC5
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT9) | (1 << PCINT13);

    lcd_init(); 
    lcd_clear();
    lcd_moveto(0, 0);
    lcd_stringout("Lab 3");
     lcd_moveto(1, 0);
    lcd_stringout("Janna Jumao-as");
    _delay_ms(1000);
    lcd_clear();
}

ISR(PCINT1_vect) {

    // Read encoder state
    uint8_t a = (PINC & (1 << ENCODER_A)) ? 1 : 0; // Read encoder A
    uint8_t b = (PINC & (1 << ENCODER_B)) ? 1 : 0; // Read encoder B

    new_state = (a << 1) | b; 

    // Update count based on encoder direction if state has changed
    if (new_state != old_state) {
        if ((old_state == 0 && new_state == 1) ||
            (old_state == 1 && new_state == 3) ||
            (old_state == 3 && new_state == 2) ||
            (old_state == 2 && new_state == 0)) {
            // Clockwise rotation
            count_ms++;
        } else {
            // Counter-clockwise rotation
            count_ms--;
        }

        changed = 1; // Set flag to indicate that count has changed
    }

    old_state = new_state; // Update old state
}

int main(void) {
    setup();
    sei(); // Enable global interrupts

    while (1) {
        
        if (changed) {
            changed = 0; // Reset the changed flag

            // Update the display with the current count
            char buf[16];
            snprintf(buf, sizeof(buf), " %d", count_ms);
            lcd_clear();
            lcd_moveto(0, 0);
            lcd_stringout(buf);

            
            int note_index = (abs(count_ms) % 64) / 8;

            // Play tone if count is a multiple of 8 and not currently beeping
            if (abs(count_ms) % 8 == 0 && !beep) {
                beep = 1; 
                play_note(frequency[note_index]); 
                beep = 0; // Reset beeping flag after playing the note
            }
        }
    }
}
