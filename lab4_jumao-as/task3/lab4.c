#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define ENCODER_A PC4  // Encoder A signal
#define ENCODER_B PC5  // Encoder B signal


volatile uint8_t pwm_led_value = 128;      // Initial PWM value for LED (50%)
volatile uint8_t prev_state= 0;
volatile uint8_t present_state= 0;          
volatile uint8_t changed_value = 0;  

// Timer2 initialization for LED PWM
void timer2_init(void) {
    TCCR2A |= (0b11 << WGM20);              // Fast PWM mode
    TCCR2A |= (0b10 << COM2A0);             // Turn D11 on at 0x00 and off at OCR2A
    OCR2A = count;                          // Initialize pulse width with starting count value
    TCCR2B |= (0b111 << CS20);              // Prescaler = 1024 (16ms period)
    DDRB |= (1 << PB3);                     // Set PB3 as output for PWM
}

// Rotary encoder interrupt service routine
ISR(PCINT1_vect) {
    uint8_t a = (PINC & (1 << ENCODER_A)) ? 1 : 0;
    uint8_t b = (PINC & (1 << ENCODER_B)) ? 1 : 0;

    present_state = (a << 1) | b;

    if (present_state != prev_state) {
        if ((prev_state == 0 && present_state == 1) ||
            (prev_state == 1 && present_state == 3) ||
            (prev_state == 3 && present_state == 2) ||
            (prev_state == 2 && present_state == 0)) {
            count++;  
            if (count > 255) count = 0;  // Wrap around 0 if it exceeds 255
        } else {
            count--;  
            if (count < 0) count = 255;  // Wrap around 255 if it goes below 0
        }
        changed_value = 1;  // Set flag to indicate count change
    }

    prev_state = present_state;
}

void setup() {
    DDRB |= (1 << PB4); 
    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B)); 
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B); 

    // Enable Pin Change Interrupts for PC4 and PC5
    PCICR |= (1 << PCIE1);          
    PCMSK1 |= (1 << PCINT12) | (1 << PCINT13);

    lcd_init(); 
    lcd_clear();
    lcd_moveto(0, 0);
    lcd_stringout("Lab 3");
     lcd_moveto(1, 0);
    lcd_stringout("Janna Jumao-as");
    _delay_ms(1000);
    lcd_clear();

    timer2_init();  // Initialize Timer2 for PWM
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
        }
        _delay_ms(100); 
    }
}