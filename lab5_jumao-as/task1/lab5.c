/********************************************
 *
 *  Name: Janna Joyce E. Jumao-as
 *  Email: jannajoyce.jumao-as@g.msuiit.edu.ph
 *  Section: M89
 *  Assignment: Lab 5 - Robot Essentials
 *
 ********************************************/
#include <avr/io.h>
#include <util/delay.h>

// Pin definitions for the servo
#define SERVO_PIN   PB2    // OC1B - Pin 10
#define RIGHT        900    // Servo position for right (~1ms pulse / -90)
#define CENTER       2800   // Servo position for center (~1.5ms pulse / 0)
#define LEFT         4800   // Servo position for left (~2ms pulse / 90)

// Function to initialize the servo
void setupServo() {
    // Set servo pin as output (PB2 - OC1B)
    DDRB |= (1 << SERVO_PIN);

    // Clear all timer settings first
    TCCR1A = 0;
    TCCR1B = 0;

    // Set Timer1 for Phase Correct PWM mode
    ICR1 = 40000;   // TOP value for 50Hz (20ms)

    // Configure Timer1
    TCCR1A |= (1 << COM1B1)   // Clear OC1B on Compare Match when up-counting
             | (1 << WGM11);   // Fast PWM Mode with ICR1 as TOP

    TCCR1B |= (1 << WGM13)    // Fast PWM Mode with ICR1 as TOP
             | (1 << WGM12)    // Fast PWM Mode
             | (1 << CS11);    // Prescaler = 8

    // Set initial position to center
    OCR1B = CENTER;
}

// Function to move the servo to a specific position
void moveServo(uint16_t position) {
    OCR1B = position;
    _delay_ms(1000);  // Wait for the servo to reach the position
}

// Main function
int main(void) {
    // Initialize the servo
    setupServo();

    // Main loop to move the servo in a Left → Center → Right → Center → Left → Center pattern
    while (1) {
        // Move the servo to the left position
        moveServo(LEFT);
        // Move the servo to the center position
        moveServo(CENTER);
        
        // Move the servo to the right position
        moveServo(RIGHT);
        // Move the servo to the center position again
        moveServo(CENTER);
    }

    return 0;
}
