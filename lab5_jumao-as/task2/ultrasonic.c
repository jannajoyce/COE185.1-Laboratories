#include "ultrasonic.h"

// Global variables for input capture and ultrasonic signal processing
volatile uint16_t start_time = 0;   // Stores the time at which the rising edge is captured
volatile uint16_t pulse_width = 0;   // Stores the duration of the pulse between rising and falling edge
volatile uint8_t edge_count = 0;     // Used to track the edge state (rising/falling)

// Function to initialize the LED pins as outputs and perform an initial blink test
void setupLed() {
    // Set LED pins as output
    DDRB |= (1 << R_LED) | (1 << L_LED);
    
    // Initial LED blink test
    ledOn('r');
    _delay_ms(200);
    ledOff('r');
    ledOn('l');
    _delay_ms(200);
    ledOff('l');
}

// Function to turn on a specific LED (either red or green)
void ledOn(char led) {
    if(led == 'r') {
        PORTB |= (1 << R_LED);  // Turn on red LED (PB0)
    } else if(led == 'l') {
        PORTB |= (1 << L_LED);  // Turn on green LED (PB3)
    }
}

// Function to turn off a specific LED (either red or green)
void ledOff(char led) {
    if(led == 'r') {
        PORTB &= ~(1 << R_LED);  // Turn off red LED (PB0)
    } else if(led == 'l') {
        PORTB &= ~(1 << L_LED);  // Turn off green LED (PB3)
    }
}

// Function to control LEDs based on the distance measurement and the given position
void proximityReadLed(int position, double threshold) {
    // Turn off LEDs first
    ledOff('r');
    ledOff('l');
    
    // Take a single reading
    uint16_t distance = measureDistance();
    
    // Only proceed if we have a valid reading (not timeout value) AND within 12 inches
    if(distance != 65535 && distance <= MAX_DISTANCE) {
        if(position == LEFT) {
            ledOn('l');  // Turn on green LED for left wall
        }
        else if(position == RIGHT) {
            ledOn('r');  // Turn on red LED for right wall
        }
        else if(position == CENTER) {
            ledOn('r');  // Turn on both LEDs for center
            ledOn('l');
        }
    }
}

// Function to configure Timer0 for PWM control of the servo
void setupTimer0() {
    // Configure Timer0 for PWM
    TCCR0A = (1 << COM0A1) |  // Clear OC0A on Compare Match
             (1 << WGM01)  |   // Fast PWM Mode
             (1 << WGM00);    // Fast PWM Mode
             
    TCCR0B = (1 << CS01) |    // Prescaler = 64
             (1 << CS00);     // for ~50Hz PWM
}

// Function to configure Timer1 for input capture to measure pulse width of the ultrasonic echo
void setupTimer1() {
    // Configure Timer1 for Input Capture
    TCCR1B |= (1 << ICES1);   // Rising edge capture initially
    TCCR1B |= (1 << CS11);    // Prescaler = 8
    TIMSK1 |= (1 << ICIE1);   // Enable input capture interrupt
    sei();                    // Enable global interrupts
}

// Interrupt Service Routine for Timer1 Input Capture event
ISR(TIMER1_CAPT_vect) {
    if(edge_count == 0) {
        // Rising edge detected
        start_time = ICR1;
        TCCR1B &= ~(1 << ICES1);  // Switch to falling edge capture
        edge_count = 1;
    } else {
        // Falling edge detected
        pulse_width = ICR1 - start_time;
        TCCR1B |= (1 << ICES1);   // Switch back to rising edge capture
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
    
    // Set initial servo position to CENTER
    OCR1B = CENTER;
}

// Function to initialize the ultrasonic sensor
void setupUltrasonic() {
    // Set Trigger pin as output
    DDRB |= (1 << TRIGGER_PIN);
    PORTB &= ~(1 << TRIGGER_PIN);  // Set low initially
    
    // Set Echo pin as input
    DDRB &= ~(1 << ECHO_PIN);
}

// Function to measure the distance using the ultrasonic sensor
uint16_t measureDistance() {
    uint16_t distance;
    
    // Send trigger pulse (10 microseconds)
    PORTB |= (1 << TRIGGER_PIN);
    _delay_us(10);
    PORTB &= ~(1 << TRIGGER_PIN);
    
    // Wait for echo start (pin goes high)
    uint16_t timeout = 0;
    while(!(PINB & (1 << ECHO_PIN))) {
        timeout++;
        if(timeout > 60000) return 65535;  // Return max value if timeout occurs
    }
    
    // Start counting the time for echo duration
    TCNT1 = 0;
    
    // Wait for echo end (pin goes low)
    timeout = 0;
    while(PINB & (1 << ECHO_PIN)) {
        timeout++;
        if(timeout > 60000) return 65535;  // Return max value if timeout occurs
    }
    
    distance = TCNT1;
    
    // Convert the pulse width to distance (in inches)
    return (distance / 148);
}

// Function to move the servo to a specific position
void moveServo(uint16_t position) {
    OCR1B = position;
    _delay_ms(1000);  // Wait for servo to reach position
}
