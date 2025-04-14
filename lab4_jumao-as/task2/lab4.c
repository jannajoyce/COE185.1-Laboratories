#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"
#include <avr/interrupt.h>
#include <stdlib.h>

#define ENCODER_A PC4  
#define ENCODER_B PC5  


//volatile uint8_t pwm_led_value = 128;      // Initial PWM value for LED (50%)
volatile uint8_t prev_state= 0;
volatile uint8_t present_state= 0;          
volatile uint8_t changed_value = 0;  
volatile int16_t count = 0;

// Timer2 initialization for LED PWM
void timer2_init(void) {
    TCCR2A |= (0b11 << WGM20);              // Fast PWM mode
    TCCR2A |= (0b10 << COM2A0);             // Turn D11 on at 0x00 and off at OCR2A
    OCR2A = 128;                          // Initialize pulse width with starting count value
    TCCR2B |= (0b111 << CS20);              // Prescaler = 1024 (16ms period)
    DDRB |= (1 << PB3);                     // Set PB3 as output for PWM
}


void setup() {
    DDRB |= (1 << PB4); 
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B)); 
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B); 

    // Enable Pin Change Interrupts for PC4 and PC5
    //PCICR |= (1 << PCIE1);          

    lcd_init(); 
    lcd_clear();
    lcd_moveto(0, 0);
    lcd_stringout("Lab 4");
     lcd_moveto(1, 0);
    lcd_stringout("Janna Jumao-as");
    _delay_ms(1000);
    lcd_clear();

    // timer1_init();
    timer2_init();  // Initialize Timer2 for PWM
}

int main(void) {
    setup();
    sei(); 

    uint8_t old_a = 2; // A value last seen, initialized to an invalid state (not 0 or 1)

    while (1) {
       
        uint8_t new_a = (PINC & (1 << ENCODER_A)) ? 1 : 0;

        // Check if the A value has changed 
        if (new_a != old_a) {

            if (new_a == 0) {
                OCR2A = 51; // 20% duty cycle
            } else {
                OCR2A = 204; // 80% duty cycle
            }

            // Update the LCD only if the A input changed
            lcd_clear();
            lcd_moveto(0, 0);
            lcd_stringout("Duty Cycle is ");
            lcd_moveto(1, 0);

            if (a == 0) {
                lcd_stringout("20%");
            } else {
                lcd_stringout("80%");
            }

            old_a = new_a; // Store the current A
        }

    }
}