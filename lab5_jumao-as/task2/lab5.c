/********************************************
 *
 *  Name: Janna Joyce E. Jumao-as
 *  Email: jannajoyce.jumao-as@g.msuiit.edu.ph
 *  Section: M89
 *  Assignment: Lab 5 - Robot Essentials
 *
 ********************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ultrasonic.h"

int main(void) {
    
    setupTimer0();
    setupTimer1();
    setupServo();
    setupLed();
    setupUltrasonic();
    _delay_ms(1000);
    ledOff('r');
    ledOff('l');
    
    while(1) {
        moveServo(CENTER);
        _delay_ms(250);
        proximityReadLed(CENTER, MAX_DISTANCE);
        _delay_ms(1500);

        moveServo(RIGHT);
        _delay_ms(250);
        proximityReadLed(RIGHT, MAX_DISTANCE);
        _delay_ms(1500);

        moveServo(CENTER);
        _delay_ms(250);
        proximityReadLed(CENTER, MAX_DISTANCE);
        _delay_ms(1500);
        
        moveServo(LEFT);
        _delay_ms(250);
        proximityReadLed(LEFT, MAX_DISTANCE);
        _delay_ms(1500);
    }
    
    return 0;
}