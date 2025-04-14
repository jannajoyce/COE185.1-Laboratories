#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#ifndef ULTRASONIC_H
#define ULTRASONIC_H


// Pin definitions
#define SERVO_PIN   PB2    // OC1B - Pin 10 (Servo Control)
#define TRIGGER_PIN PB4    // Pin 12 (Trigger for Ultrasonic)
#define ECHO_PIN    PB5    // Pin 13 (Echo from Ultrasonic)
#define R_LED       PB0    // Pin 8 (Red LED)
#define L_LED       PB3    // Pin 11 (Green LED)

// Servo position definitions for PWM
#define RIGHT     900     // -90 degrees (~1ms pulse width)
#define CENTER    2800    // 0 degrees (~1.5ms pulse width)
#define LEFT      4800    // +90 degrees (~2ms pulse width)

// Distance threshold (inches)
#define MAX_DISTANCE 24    

// Function prototypes
void setupTimer0(void);          // Setup for Timer0 (PWM)
void setupTimer1(void);          // Setup for Timer1 (Input Capture for Echo)
void setupServo(void);           // Setup for the servo control
void setupUltrasonic(void);      // Setup for the ultrasonic sensor
uint16_t measureDistance(void);  // Measure distance using ultrasonic sensor
void moveServo(uint16_t position);  // Move servo to specified position
void setupLed(void);             // Setup LED pins as outputs
void ledOn(char led);            // Turn on specific LED (red/green)
void ledOff(char led);           // Turn off specific LED (red/green)
void proximityReadLed(int position, double threshold); // Read proximity and control LEDs based on threshold

// Global variables for input capture and ultrasonic signal processing
extern volatile uint16_t start_time;  // Stores the time at which the rising edge is captured
extern volatile uint16_t pulse_width; // Stores the duration of the pulse between rising and falling edge
extern volatile uint8_t edge_count;   // Used to track the edge state (rising/falling)
  
#endif
