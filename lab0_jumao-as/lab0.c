/********************************************
*
*  Name:
*  Section:
*  Assignment:
*
********************************************/

#include <avr/io.h>

#include <util/delay.h>



int main(void)

{

    DDRB |= (1 << DDB3);  	/* Set PB5 for output */

    while(1) {

    PORTB |= (1 << PB3);  	/* LED on */

    _delay_ms(500);         	/* Wait 500 milliseconds */

    PORTB &= ~(1 << PB3); 	/* LED off */

    _delay_ms(500);         	/* Wait 500 milliseconds */

    }

    return 0;   		/* never reached */

}
