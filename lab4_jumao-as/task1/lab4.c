#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "lcd.h"
#include <stdio.h>

#define ENCODER_A PC4
#define ENCODER_B PC5
#define LED_PIN PB3

void setup() {

    DDRC &= ~((1 << ENCODER_A) | (1 << ENCODER_B)); 
    PORTC |= (1 << ENCODER_A) | (1 << ENCODER_B);  

    // Set LED pin as output
    DDRB |= (1 << LED_PIN);

    
    TCCR2A = (1 << WGM21) | (1 << WGM20) | (1 << COM2A1); // Fast PWM (non-inverting)
    TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);     // Prescaler: 1024
    OCR2A = 0; // Initial duty cycle = 0 (LED OFF)

    lcd_init();

    lcd_moveto(0, 0); 
    lcd_stringout("Janna Joyce E. Jumao-as"); 

    char birthdate[16];
    snprintf(birthdate, sizeof(birthdate), "%02d/%02d/%d", 01, 27, 2003); 

    lcd_moveto(1, (16 - strlen(birthdate)) / 2); 
    lcd_stringout(birthdate); 

    _delay_ms(2000);

    lcd_writecommand(LCD_CLR);
}

uint8_t debounce(uint8_t pin) {
    uint8_t state = PINC & (1 << pin);
    _delay_ms(10); 
    return (PINC & (1 << pin)) == state ? state : 0;
}

void loop() {
    static int8_t prev_state = 0; // Store last state of encoder
    int8_t new_state;
    static uint8_t brightness = 0; // Store brightness level (0-255)

    // Read encoder states
    uint8_t encoder_a = debounce(ENCODER_A) ? 1 : 0;
    uint8_t encoder_b = debounce(ENCODER_B) ? 1 : 0;

    // Combine states into a single variable
    new_state = (encoder_a << 1) | encoder_b;

   
    if ((prev_state == 0b00 && new_state == 0b01) || 
        (prev_state == 0b01 && new_state == 0b11) || 
        (prev_state == 0b11 && new_state == 0b10) || 
        (prev_state == 0b10 && new_state == 0b00)) {
       
        // increase brightness (clockwise)
        if (brightness < 255) brightness++;
    } else if ((prev_state == 0b00 && new_state == 0b10) || 
               (prev_state == 0b10 && new_state == 0b11) || 
               (prev_state == 0b11 && new_state == 0b01) || 
               (prev_state == 0b01 && new_state == 0b00)) {
       
        //  decrease brightness (counterclockwise)
        if (brightness > 0) brightness--;
    }

    prev_state = new_state;

    OCR2A = brightness;

    // Update LCD
    char buf[16];
    snprintf(buf, sizeof(buf), "A=%d B=%d", encoder_a, encoder_b);  // Encoder states
    lcd_moveto(0, 0);  // Move to top-left corner
    lcd_stringout(buf);

    snprintf(buf, sizeof(buf), "Brightness=%d", brightness); // Brightness level
    lcd_moveto(1, 0);  // Move to second row
    lcd_stringout(buf);

    // Small delay for stability
    _delay_ms(50);
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
}
