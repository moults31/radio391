#include <main.h>
#include <msp430.h>
#include <stdint.h>
#include "freq_select.h"
#include "hci.h"

/**
 * blink.c
 */
void main(void)
{
    uint32_t i = 0;
    volatile uint16_t a,b,c;
    volatile khz_t f = KHZ606;

	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P1DIR |= 0b111;					// configure P[1.0,1.2] as output
	P2DIR |= 0b111;               // configure P[2.3,2.5] as output

	P1DIR &= 0b00111;

	sr_clear_all();

	while(1)
	{
	    a=P1IN;
	    b = a&(0b10000);
	    c = a&(0b01000);

	    if(b == 0) f = handleKeyPress(KEY_UP, f);
	    if(c == 0) f = handleKeyPress(KEY_DOWN, f);
	}
}
