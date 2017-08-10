#include <msp430.h>				
#include "hal.h"

/**
 * blink.c
 */
void main(void)
{
    int a = 0;
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P1DIR |= 0b111<<DEMUX2OFFSET;					// configure P[1.0,1.2] as output
	P2DIR |= 0b111<<DEMUX1OFFSET;               // configure P[2.3,2.5] as output

	volatile unsigned int i;		// volatile to prevent optimization

	while(1)
	{
	    if( a == 0b111) a = 0b00;
	    else a++;

		DEMUX2 = a<<DEMUX2OFFSET;				// toggle P1.0
		DEMUX1 = a<<DEMUX1OFFSET;
		for(i=100000; i>0; i--);     // delay
	}
}
