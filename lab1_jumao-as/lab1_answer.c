/********************************************
*
*  Name: Janna Joyce E. Jumao-as
*  Email: jannajoyce.jumao-as@g.msuiit.edu.ph
*  Lab section: M89
*  Assignment: Lab 1 - Arduino Input and Output
* CODE FOR LAB1_ANSWER
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DOT_LENGTH  250

void dot(void);
void dash(void);
char checkInput(char);

int main(void)
{
    DDRD &= ~(1 << DDD3) | ~(1 << DDD4) | ~(1 << DDD5); // Inputs for Buttons 1-3
    DDRD |= (1 << DDD2); // Output for LED

    PORTD |= (1 << PD3) | (1 << PD4) | (1 << PD5); // Pull-up resistors for Buttons

    while (1) {
        if (checkInput(3)) { 
            dash();
            dash();
            _delay_ms(900);
        } else if (checkInput(4)) {
            dot();
            dot();
            dot();
            _delay_ms(900);
        } else if (checkInput(5)) {
            dot();
            dot();
            dash();
            _delay_ms(900);
        }
    }

    return 0;
}

void dot()
{
    PORTD = 0x04; // Turn on LED
    _delay_ms(300);
    PORTD = 0x00; // Turn off LED
    _delay_ms(300);
}

void dash()
{
    PORTD = 0x04; // Turn on LED
    _delay_ms(900);
    PORTD = 0x00; // Turn off LED
    _delay_ms(300);
}

char checkInput(char bit)
{
    return (PIND & (1 << bit)) == 0;
}