#include "ultrasonic.h"


volatile uint16_t start_time = 0;   
volatile uint16_t pulse_width = 0;   
volatile uint8_t edge_count = 0;     

void setupLed() {
   
    DDRB |= (1 << R_LED) | (1 << L_LED);
    
    ledOn('r');
    _delay_ms(200);
    ledOff('r');
    ledOn('l');
    _delay_ms(200);
    ledOff('l');
}

void ledOn(char led) {
    if(led == 'r') {
        PORTB |= (1 << R_LED);  
    } else if(led == 'l') {
        PORTB |= (1 << L_LED);  
    }
}

void ledOff(char led) {
    if(led == 'r') {
        PORTB &= ~(1 << R_LED);  
    } else if(led == 'l') {
        PORTB &= ~(1 << L_LED);  
    }
}

void proximityReadLed(int position, double threshold) {
    
    ledOff('r');
    ledOff('l');
    
    uint16_t distance = measureDistance();
    
    if(distance != 65535 && distance <= MAX_DISTANCE) {
        if(position == LEFT) {
            ledOn('l');  
        }
        else if(position == RIGHT) {
            ledOn('r');  
        }
        else if(position == CENTER) {
            ledOn('r');  
            ledOn('l');
        }
    }
}


void setupTimer0() {
    // Configure Timer0 for PWM
    TCCR0A = (1 << COM0A1) |  // Clear OC0A on Compare Match
             (1 << WGM01)  |   // Fast PWM Mode
             (1 << WGM00);    // Fast PWM Mode
             
    TCCR0B = (1 << CS01) |    // Prescaler = 64
             (1 << CS00);     // for ~50Hz PWM
}


void setupTimer1() {
    
    TCCR1B |= (1 << ICES1);   
    TCCR1B |= (1 << CS11);    // Prescaler = 8
    TIMSK1 |= (1 << ICIE1);
    sei();                    
}

// Interrupt Service Routine for Timer1 Input Capture event
ISR(TIMER1_CAPT_vect) {
    if(edge_count == 0) {
       
        start_time = ICR1;
        TCCR1B &= ~(1 << ICES1);  
        edge_count = 1;
    } else {

        pulse_width = ICR1 - start_time;
        TCCR1B |= (1 << ICES1);   
        edge_count = 0;
    }
}

// Function to set up the servo by configuring Timer1 for PWM operation
void setupServo() {
    // Set servo pin as output (PB2 - OC1B)
    DDRB |= (1 << SERVO_PIN);
    
    // Clear all timer settings first
    TCCR1A = 0;
    TCCR1B = 0;
    
    // Set Timer1 for Phase Correct PWM mode
    ICR1 = 40000;   // TOP value for 50Hz (20ms period)
    
    // Configure Timer1 for PWM operation
    TCCR1A |= (1 << COM1B1)   // Clear OC1B on Compare Match when up-counting
             | (1 << WGM11);   // Fast PWM Mode with ICR1 as TOP
             
    TCCR1B |= (1 << WGM13)    // Fast PWM Mode with ICR1 as TOP
             | (1 << WGM12)    // Fast PWM Mode
             | (1 << CS11);    // Prescaler = 8
    
    OCR1B = CENTER;
}

void setupUltrasonic() {
    
    DDRB |= (1 << TRIGGER_PIN);
    PORTB &= ~(1 << TRIGGER_PIN); 
    
    DDRB &= ~(1 << ECHO_PIN);
}

uint16_t measureDistance() {
    uint16_t distance;
    
    // Send trigger pulse (10 microseconds)
    PORTB |= (1 << TRIGGER_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIGGER_PIN);
    
    uint16_t timeout = 0;
    while(!(PINB & (1 << ECHO_PIN))) {
        timeout++;
        if(timeout > 60000) return 65535;  
    }
    
    TCNT1 = 0;
    
    timeout = 0;
    while(PINB & (1 << ECHO_PIN)) {
        timeout++;
        if(timeout > 60000) return 65535;  
    }
    
    distance = TCNT1;
    
    // Convert the pulse width to distance (in inches)
    return (distance / 148);
}


void moveServo(uint16_t position) {
    OCR1B = position;
    _delay_ms(1000);  
}
