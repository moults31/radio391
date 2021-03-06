#include <main.h>
#include <msp430.h>
#include <stdint.h>
#include "freq_select.h"
#include "hci.h"


void main(void)
{
    volatile uint16_t a,b,c;
    double f = 688;

	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer

    P1DIR &= 0b000111;               //configure P1.3,1.4 as inputs for keys, and P1.5 as input for ADC
    P2DIR &= 0b00010000;               //configure P2.4 as inputs for swt

	P1DIR |= 0b11000111;		    // configure P[1.0,1.2] and P[1.6,1.7] as outputs for hundreds divider
	P2DIR |= 0b00001111;                // configure P[2.0,2.3] as outputs for shift regs

	adc_config();

	sr_clear_all();

	freq_set_lin(&f);

	while(1)
	{
	    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
	    __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled

	    a=P1IN;
	    b = a&(0b10000);
	    c = a&(0b01000);

	    if(b == 0) f = handleKeyPress(KEY_UP, f);
	    if(c == 0) f = handleKeyPress(KEY_DOWN, f);
	}
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Return to active mode
}
