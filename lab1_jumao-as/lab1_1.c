/********************************************
*
*  Name: Janna Joyce E. Jumao-as
*  Email: jannajoyce.jumao-as@g.msuiit.edu.ph
*  Lab section: M89
*  Assignment: Lab 1 - Arduino Input and Output
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#define DOT_LENGTH  250         /* Define the length of a "dot" time in msec */

void dot(void);
void dash(void);
void makeOutput(char);
char checkInput(char);


int main(void)
{

    // Initialize appropriate DDR registers
	DDRD |= (1 << DDD2);
	DDRB &= ~(1 << DDB3);

    // Initialize the LED output to 0
	
    // Enable the pull-up resistors for the 
    // 3 button inputs 
	PORTB |= (1 << PB3);

    // Loop forever
    while (1) {                 
    
	//  Use "if" statements and the checkInput()
	//  function to determine if a button
	//  is being pressed and then output
	//  the correct dot/dash sequence by
	//  calling the dot() and dash(), and 
	//  using appropriate delay functions
	

    if (checkInput(3)) {  // Is button 1 pressed?
		makeoutput(1);
	}
    
	}

    return 0;   /* never reached */
}

/*
  dot() - Makes the output LED blink a "dot".

  Write code to generate a dot by using the makeOutput function
  to turn the output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay an
  appropriate time after you turn the output off.
*/


/*
  dash() - Makes the output LED blink a "dash".

  Write code to generate a dash by using the makeOutput function
  to turn the output ON and OFF with appropriate delays.
  Be sure you don't forget to also delay an
  appropriate time after you turn the output off.
*/

/*
  makeOutput() - Changes the output bit (Group D, bit 2) to either
  a zero or one, based on the input argument "value".
  
  If the argument is zero, turn the output OFF,
  otherwise turn the output ON. 
  
  Do not use any delays here.  Just use bit-wise operations
  to make the appropriate PORT bit turn on or off.
*/
void makeOutput(char value)
{
	if (value) {
        PORTD |= (1 << PD2);  // Turn the LED on
    } else {
        PORTD &= ~(1 << PD2); // Turn the LED off
    }
}

/*
  checkInput(bit) - Checks the state of the input bit in Group B specified by
  the "bit" argument (0-7), and returns 1 if the button is pressed, or 0 if
  the button is not pressed.
  
  Write code to use the appropriate group's PIN register and determine if  
  the specified bit (which is passed as the argument) of that group is
  pressed or not.  Think carefully about what bit value means "pressed"
  when connected to a pushbutton.
 */
char checkInput(char bit)
{
	return !(PINB & (1 << bit));
}