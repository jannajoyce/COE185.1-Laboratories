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
#include "ultrasonic.h"
#include "serial.h"

int main(void) {
    setupSerial();
    setupServo();
    setupUltrasonic();
   
    _delay_ms(1000);
    
    moveServo(CENTER);
    _delay_ms(1000);
    
    serialWriteString("Ultrasonic Sensor Reading\r\n");
    serialWriteString("==========================\r\n");
    
    while(1) {
        uint16_t distance = measureDistance();
        
        serialWriteString("Measured Distance: ");
        serialWriteInt(distance);
        serialWriteString(" in\r\n");

        _delay_ms(500);
    }
    
    return 0;
}
