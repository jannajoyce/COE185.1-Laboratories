/********************************************
*
*  Name: Janna Joyce E. Jumao-as
*  Email: jannajoyce.jumao-as@g.msuiit.edu.ph
*  Section: M89
*  Assignment: Lab 2 - Up/Down counter on LCD display
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"


#define UP_BUTTON_THRESHOLD 150
#define DOWN_BUTTON_THRESHOLD 350
#define PAUSE_BUTTON_THRESHOLD 700
#define A0 0 // Analog input for buttons (ADC0)

enum states { UP, DOWN, PAUSE };
enum states counter_state = UP;
unsigned char count = 0; // Counter value (0-9)



void setupADC() {

    // Set AVcc as reference, select ADC0
    ADMUX = (1 << REFS0); // AVcc as reference voltage
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS2); // Enable ADC, set prescaler to 64
}


uint16_t readADC(uint8_t channel) {

    // Select the ADC channel
    ADMUX = (ADMUX & 0xF8) | (channel & 0x07); 
    ADCSRA |= (1 << ADSC); // Start conversion
    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
    return ADC; // Return ADC value
}


void setup() {
    lcd_init(); // Initialize LCD
    lcd_writecommand(1); // Clear the LCD screen
    lcd_writecommand(2); // Move cursor to home position
    setupADC(); // Initialize ADC for button reading
}


void loop() {
    static unsigned int tick_counter = 0; // Tracks elapsed time in ms
    static unsigned char previous_count = 0; // Stores previous count for LCD update control
    unsigned char count_changed = 0; // Flag to indicate if count has changed
    
    // Read the analog value from A0 (LCD shield buttons)
    uint16_t buttonValue = readADC(A0);


    // Determine which button is pressed
    if (buttonValue < UP_BUTTON_THRESHOLD) {
        counter_state = UP;
    } else if (buttonValue < DOWN_BUTTON_THRESHOLD) {
        counter_state = DOWN;
    } else if (buttonValue < PAUSE_BUTTON_THRESHOLD) {
        counter_state = PAUSE;
    }

    tick_counter += 50; // Increment time counter by 50ms


    // If 500ms have elapsed, update the count based on the current state
    if (tick_counter >= 500) {
        tick_counter = 0; // Reset the tick counter

    // Update count depending on the state
    if (counter_state == UP) {
        count = (count + 1) % 10; // Count from 0 to 9
        count_changed = 1;
    } else if (counter_state == DOWN) {
        count = (count == 0) ? 9 : count - 1; // Count down from 9 to 0
        count_changed = 1;
    }
    }


    // Update the LCD only if the count has changed
    if (count_changed) {
    lcd_writecommand(2); // Move cursor to the home position
    lcd_writedata(count + '0'); // Display the count

    }

    // Delay 50ms before next loop iteration
    _delay_ms(50);
}


int main() {
    setup(); // Initialize the system
    while (1) {
        loop(); // Continuously run the main loop
    }
    return 0;
}


