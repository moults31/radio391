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
    uint16_t a;
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P1DIR |= 0b111;					// configure P[1.0,1.2] as output
	P2DIR |= 0b111;               // configure P[2.3,2.5] as output

	P1DIR &= 0b00111;

	sr_clear_all();

	while(1)
	{
	    a=P1IN & (0b11111111);

	    sr_set_tens(a);
	}
}
