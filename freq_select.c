/*
 * freq_select.c
 *
 *  Created on: Aug 9, 2017
 *      Author: moults31
 */
#include <msp430.h>
#include <stdint.h>
#include "hal.h"
#include "freq_select.h"

#define SR_SIZE 5

typedef enum
{
    SR_TENS,
    SR_HUNDS
} sr_t;


/**************************************************
 ***  SHIFT REGISTER FUNCTION DEFINITIONS *********
 * ************************************************
 */

void freq_set(khz_t f)
{
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

    sr_set_tens(f2bin[f]);
    sr_set_hunds(f2bin[f]);
}

/*
 * Brief: Set 8 values into the "tens" shift register IC
 * Param: vals: 8-entry array of 1-bit values to set in IC
 */
void sr_set_tens(uint16_t vals)
{
    uint8_t i, val;
    uint8_t clk, data;
    uint8_t en = 1;

    //for sr_tens only use lower 5 bits
    vals &= 0b11111;

    for(i=0; i<SR_SIZE; i++)
    {
        clk = 0;
        data = 0b1 & vals;

        //build output "data,enable,clk"
        val = clk<<2 | en<<1 | data;

        PIN_SR_TENS = val;

        //send a rising edge on clk
        clk = 1;
        PIN_SR_TENS |= clk<<2;

        vals = vals>>1;
    }
}


/*
 * Brief: Set 8 values into the "hundreds" shift register IC
 * Param: vals: 8-entry array of 1-bit values to set in IC
 */
void sr_set_hunds(uint16_t vals)
{
     uint8_t i, val;
     uint8_t clk, data;
     uint8_t en = 1;

     //for sr_hunds only use upper 5 bits
     vals = (vals & 0b1111100000) >> SR_SIZE;

     for(i=0; i<SR_SIZE; i++)
     {
         clk = 0;
         data = 0b1 & vals;

         //build output "data,enable,clk"
         val = clk<<2 | en<<1 | data;

         PIN_SR_HUNDS = val;

         //send a rising edge on clk
         clk = 1;
         PIN_SR_HUNDS |= clk<<2;

         vals = vals>>1;
     }
}
