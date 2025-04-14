#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>


void serialWriteInt(uint16_t num);
void serialWriteString(const char* str);
void serialWrite(char c);
void setupSerial(void);

#endif