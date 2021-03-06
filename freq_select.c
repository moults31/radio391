/*
 * freq_select.c
 *
 *  Created on: Aug 9, 2017
 *      Author: moults31
 */
#include "freq_select.h"


/**************************************************
 ***  FREQUENCY SETTING FUNCTION DEFINITIONS *********
 * ************************************************
 */

void freq_set_lut(khz_t f)
{
    uint32_t a = (uint32_t) f;
    static const uint16_t f2bin[52]=
    {
     0b0000111000, 0b0000100000, 0b0001100011, 0b0001100111, 0b0001111111,
     0b0001111110, 0b0001111100, 0b0011100111, 0b0001111000, 0b0011101111,
     0b0011100001, 0b0011111111, 0b0011111110, 0b0011111100, 0b0011110000,
     0b0011100000, 0b0111101111, 0b0111111111, 0b0111111110, 0b0111111100,
     0b0111111000, 0b0111100000, 0b1111100111, 0b1111101111, 0b1111111111,
     0b1111111110, 0b1111111100, 0b1111000001, 0b1111110000, 0b1111100000,
     0b1111000111, 0b1111001111, 0b1111011111, 0b1111011110, 0b1110000001,
     0b1111011000, 0b1111010000, 0b1110001111, 0b1111000000, 0b1000000001,
     0b1110000000, 0b1000000011, 0b1100000001, 0b1100011111, 0b1100011110,
     0b1100011100, 0b1100011000, 0b1100010000, 0b1000011000, 0b1000010000,
     0b1000000000, 0b0000000000
    };

    sr_set_tens(f2bin[a]);
    sr_set_units(f2bin[a]);
}

void freq_set_lin(double *f)
{
    //recover the frequency and divide it by our synthesizer's resolution
    double khz = (*f)/1.0;

    uint16_t f_hundreds = (uint16_t) (khz/100)%10;
    uint16_t f_tens = (uint16_t) (khz/10)%10;
    uint16_t f_units = (uint16_t) (khz/1)%10;

    static const uint16_t div2bin[10]=
    {
     0b00000, 0b00001, 0b00011, 0b00111, 0b01111, 0b11111,
     0b11110, 0b11100, 0b11000, 0b10000, 0b00000
    };

    direct_set_hunds(div2bin[f_hundreds+1]);
    freq_set_tens(div2bin[f_tens+1]);
    freq_set_units(div2bin[f_units]);
}

void freq_set_units(uint16_t vals)
{
    sr_set_units(vals);
}

void freq_set_tens(uint16_t vals)
{
    sr_set_tens(vals);
}

void freq_set_hunds(uint16_t vals)
{
    direct_set_hunds(vals);
}

void direct_set_hunds(uint16_t vals)
{
    vals = (vals&0b11000)<<3 | (vals&0b111);

    //set upper 2 bits
    P1OUT = vals;
}

/**************************************************
 ***  SHIFT REGISTER FUNCTION DEFINITIONS *********
 * ************************************************
 */

void sr_clear_all(void)
{
    sr_set_tens(0);
    sr_set_units(0);
}


/*
 * Brief: Set 8 values into the "tens" shift register IC
 * Param: vals: 8-entry array of 1-bit values to set in IC
 */
void sr_set_tens(uint16_t vals)
{
    uint8_t i, val;
    uint8_t clk, data;

    //set enable high for tens(P2.3), low for units(P2.1)
    uint8_t en = 0b10;

    for(i=0; i<SR_SIZE; i++)
    {
        clk = 0;
        data = 0b1 & vals;

        //build output "data,enable,clk"
        val = clk<<2 | en | data;

        PIN_SR = val;

        //send a rising edge on clk
        clk = 1;
        PIN_SR |= clk<<2;

        vals = vals>>1;
    }
}


/*
 * Brief: Set 8 values into the "hundreds" shift register IC
 * Param: vals: 8-entry array of 1-bit values to set in IC
 */
void sr_set_units(uint16_t vals)
{
    uint8_t i, val;
    uint8_t clk, data;

    //set enable high for units(P2.3), low for units(P2.1)
    uint8_t en = 0b10;

    for(i=0; i<SR_SIZE; i++)
    {
        clk = 0;
        data = 0b1 & vals;

        //build output "data,enable,clk"
        val = clk<<3 | en | data;

        PIN_SR = val;

        //send a rising edge on clk
        clk = 1;
        PIN_SR |= clk<<3;

        vals = vals>>1;
    }
}

/**************************************************
 ***  ADC FUNCTION DEFINITIONS *********
 * ************************************************
 */

void adc_test(void)
{
    while(1)
    {
    volatile long a = ADC10MEM;

    long b = a/10;
    }
}

void adc_config(void)
{
    /* Configure ADC Temp Sensor Channel */
    ADC10CTL1 = INCH_5 + ADC10DIV_3;         // Temp Sensor ADC10CLK/4
    ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE;
    __delay_cycles(1000);                     // Wait for ADC Ref to settle
    ADC10CTL0 |= ENC + ADC10SC;               // Sampling and conversion start
    __bis_SR_register(CPUOFF + GIE);          // LPM0 with interrupts enabled
}


/**************************************************
 ***  AUTOSEEK FUNCTION DEFINITIONS *********
 * ************************************************
 */
double autoseek(khz_t f_curr, uint8_t key)
#if !AUTOSEEK_ADC
#define VTHRESH_PEAK 200
#define VTHRESH_NOISE 20
{
    long v, vold, vpeak, vknee;
    state_t state = STATE_BEGINNING;
    double f_peak;

    vold = v = ADC10MEM;

    //Check that we have risen to a peak
    while((key==0) ? (f_curr-1>KHZMIN) : (f_curr+1<KHZMAX))
    {
        vold = v;
        v = getv();

        switch(state)
        {
        case STATE_BEGINNING:
            if(v-vold > VTHRESH_NOISE)
            {
                state = STATE_RISING;
                vknee = v;
            }

        case STATE_RISING:
            if(vold-v > VTHRESH_NOISE)
            {
                state = STATE_FALLING;
                vpeak = v;
                f_peak = f_curr;
            }

        case STATE_FALLING:
        default:
             if(v-vold > VTHRESH_NOISE)
             {
                 state = STATE_RISING;
                 vpeak = v;
                 f_peak = f_curr;
             }
             else if(vpeak-v > VTHRESH_PEAK && vpeak-vknee > VTHRESH_PEAK)
             {
                 //We found a legitimate peak, tune to it and return.
                 freq_set_lin(&f_peak);
                 return f_peak;
             }
             break;
        }

        if(key==0)
        {
            f_curr--;
            freq_set_lin(&f_curr);
        }
        else
        {
            f_curr++;
            freq_set_lin(&f_curr);
        }
    }

    //We hit the end of the tunable range with no peaks.
    //Return frequency at end of range in shame.
    return f_curr;
}

#elif AUTOSEEK_SAVED
#define NUMAMCHANS 13
{
    static const uint16_t amchans[NUMAMCHANS]=
    {
     547,  650,  690,  727,  979,  1030, 1130,
     1198, 1319, 1410, 1460, 1540, 1590
    };

    uint8_t i;

    //loop until amchans[i] is between two frequencies and return the greater.
    if(key == KEY_UP)
    {
        for(i=0;i<NUMAMCHANS-2;i++)
        {
            if(f_curr >= amchans[i] && f_curr < amchans[i+1] && amchans[i+1] < KHZMAX)
            {
                freq_set_lin(amchans[i+1]);
                return amchans[i+1];
            }
        }
    }
    //loop until amchans[i] is between two frequencies and return the less.
    else
    {
        for(i=NUMAMCHANS-1;i>0;i--)
        {
            if(f_curr <= amchans[i] && f_curr > amchans[i-1] && amchans[i-1] < KHZMAX)
            {
                freq_set_lin(amchans[i-1]);
                return amchans[i-1];
            }
        }
    }

    //cannot scan anymore in this direction. return unchanged f
    return f_curr;
}
#endif //AUTOSEEK_ADC

long getv(void)
{
    uint16_t i;
    long v = ADC10MEM;

    for(i=0;i<128;i++)
    {
        v = (ADC10MEM>v) ? ADC10MEM : v;
    }

    return v;
}
