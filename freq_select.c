/*
 * freq_select.c
 *
 *  Created on: Aug 9, 2017
 *      Author: moults31
 */
#include "freq_select.h"


/**************************************************
 ***  SHIFT REGISTER FUNCTION DEFINITIONS *********
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
    double khz = (*f)/2.0;

    uint16_t f_hundreds = (uint16_t) (khz/100)%10;
    uint16_t f_tens = (uint16_t) (khz/10)%10;
    uint16_t f_units = (uint16_t) (khz/1)%10;

    static const uint16_t div2bin[10]=
    {
     0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111,
     0b11110, 0b11100, 0b11000, 0b10000, 0b00000
    };

    freq_set_hunds(div2bin[f_hundreds]);
    freq_set_tens(div2bin[f_tens]);
    freq_set_units(div2bin[f_units]);
}

void sr_clear_all(void)
{
    sr_set_tens(0);
    sr_set_units(0);
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
