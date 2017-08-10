#include <msp430.h>
#include <stdint.h>
#include "hal.h"
#include "freq_select.h"

/**
 * blink.c
 */
void main(void)
{
    uint32_t i = 0;
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P1DIR |= 0b111;					// configure P[1.0,1.2] as output
	P2DIR |= 0b111;               // configure P[2.3,2.5] as output

	while(1)
	{
	    freq_set(i++);
	}
}
