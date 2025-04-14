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

// Global variables
volatile int16_t count_ms = 0;
volatile uint8_t old_state = 0;
volatile uint8_t new_state = 0;
volatile uint8_t changed = 0;
volatile uint16_t isr_toggle_count = 0; 
volatile uint16_t isr_toggle_target = 0; 

void timer1_init() {
    TCCR1A = 0; 
    TCCR1B = (1 << WGM12); 
    TIMSK1 = (1 << OCIE1A); 
}

void play_note(uint16_t freq) {
    if (freq == 0) return;

    OCR1A = 16000000 / (2 * freq); 
    isr_toggle_target = freq * 2; 
    TCCR1B |= (1 << CS10); 
    isr_toggle_count = 0; 
}

ISR(TIMER1_COMPA_vect) {
    PORTB ^= (1 << PB4); 

    isr_toggle_count++;
    if (isr_toggle_count >= isr_toggle_target) {
        TCCR1B &= ~(1 << CS10); 
    }
}

void setup() {
    DDRB |= (1 << PB4); 
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B)); 
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B); 

    // Enable pin change interrupts for encoder
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

    timer1_init();
}

ISR(PCINT1_vect) {
    uint8_t a = (PINC & (1 << ENCODER_A)) ? 1 : 0;
    uint8_t b = (PINC & (1 << ENCODER_B)) ? 1 : 0;

    new_state = (a << 1) | b;

    if (new_state != old_state) {
        if ((old_state == 0 && new_state == 1) ||
            (old_state == 1 && new_state == 3) ||
            (old_state == 3 && new_state == 2) ||
            (old_state == 2 && new_state == 0)) {
            count_ms++;
        } else {
            count_ms--;
        }
        changed = 1;
    }
    old_state = new_state;
}

int main(void) {
    setup();
    sei(); // Enable global interrupts

    while (1) {
        if (changed) {
            changed = 0;
            char buf[16];
            snprintf(buf, sizeof(buf), " %d", count_ms);
            lcd_clear();
            lcd_moveto(0, 0);
            lcd_stringout(buf);

            int note_index = (abs(count_ms) % 64) / 8;

            if (abs(count_ms) % 8 == 0 && !(TCCR1B & (1 << CS10))) {
                play_note(frequency[note_index]);
            }
        }
    }
}
