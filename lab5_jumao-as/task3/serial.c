#include "serial.h"
#include <util/delay.h>

// Function to initialize serial communication
void setupSerial() {
    // Set baud rate to 9600 for 16MHz clock
    UBRR0H = 0;
    UBRR0L = 103;  // For 9600 baud rate
    
    // Enable the transmitter
    UCSR0B = (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit, no parity
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Function to send a single character over serial
void serialWrite(char c) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Send the character
    UDR0 = c;
}

// Function to send a string over serial
void serialWriteString(const char* str) {
    // Loop through the string and send each character
    while (*str) {
        serialWrite(*str++);
    }
}

// Function to send an integer over serial
void serialWriteInt(uint16_t num) {
    char buffer[6];  // Buffer to hold the string representation of the integer
    uint8_t i = 0;   // Index for buffer
    
    // Handle the zero case explicitly
    if (num == 0) {
        serialWrite('0');
        return;
    }
    
    // Convert the integer to a string (reverse order)
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);  // Store digits in reverse order
        num /= 10;
    }
    
    // Send the string in correct order (reverse the buffer)
    while (i > 0) {
        serialWrite(buffer[--i]);
    }
}
