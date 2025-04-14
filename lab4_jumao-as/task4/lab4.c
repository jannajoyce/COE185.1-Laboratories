#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define ENCODER_A PC4     
#define ENCODER_B PC5   

volatile uint8_t pwm_led_value = 128;      // Initial PWM value for LED (50%)
volatile uint8_t prev_state= 0;
volatile uint8_t present_state= 0;          
volatile uint16_t pwm_servo_value = 1500;  // Initial PWM width for servo in microseconds
volatile uint8_t changed_value = 0;        

// Constants for servo PWM
// #define SERVO_MIN 750      // Minimum pulse width (0.75ms)
// #define SERVO_MAX 2250     // Maximum pulse width (2.25ms)
// #define SERVO_CENTER 1500  // Center pulse width (1.5ms)

// Timer1 initialization for servo PWM
void timer1_init(void) {
    TCCR1A |= (1 << WGM11) | (1 << WGM10);  // Fast PWM mode (WGM13:WGM10 = 1111)
    TCCR1B |= (1 << WGM13) | (1 << WGM12); // Fast PWM mode continued
    TCCR1A |= (1 << COM1B1);               // set COM1B1 to 1 for PWM output on OC1B
    OCR1B = pwm_servo_value;               // Set initial pulse width for servo
    TCCR1B |= (1 << CS11);                 // Prescaler = 8 (50Hz PWM frequency)
    DDRB |= (1 << PB2);                    // Set PB2 as output
}

// Timer2 initialization for LED PWM
void timer2_init(void) {
    TCCR2A |= (0b11 << WGM20);              // Fast PWM mode
    TCCR2A |= (0b10 << COM2A0);             // Turn D11 on at 0x00 and off at OCR2A
    OCR2A = changed_value;                          // Initialize pulse width with starting count value
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
            changed_value++;  
            if (changed_value > 255) changed_value = 0;  // Wrap around 0 if it exceeds 255
        } else {
            changed_value--;  
            if (changed_value < 0) changed_value = 255;  // Wrap around 255 if it goes below 0
        }

        pwm_servo_value += (present_state == 1 || present_state == 2) ? 10 : -10;  // Increment or decrement the servo PWM width

     //   if (pwm_servo_value < 1000) pwm_servo_value = 1000;  // Minimum 0.75ms pulse width
     //  if (pwm_servo_value > 2000) pwm_servo_value = 2000;  // Maximum 2.25ms pulse width

        
        OCR2A = changed_value;  // Update LED PWM (Timer2)
        OCR1B = pwm_servo_value;  // Update Servo PWM (Timer1)

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

    timer2_init(); // Initialize Timer2 for LED PWM 
    timer1_init(); // Initialize Timer1 for Servo PWM 
}

int main(void) {
    setup();
    sei(); 

    while (1) {
        
        _delay_ms(100); 
    }
}