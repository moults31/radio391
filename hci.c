/*
 * hci.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Zac
 */

#include "hci.h"

khz_t handleKeyPress(key_t key, khz_t f_curr)
{
    khz_t f_new = 500;
    uint8_t holding = 0;
    uint32_t i;
    uint32_t waittime;
    uint8_t swt = (P2IN & 0b10000)>>4; //switch at P2.4
    volatile int keyVal;

    do
    {

    switch(swt)
    {
    case AUTOSEEK:
        //do autoseek and return f
        f_new = 999;
        adc_test();
        break;
    case INCREMENT:
        switch(key)
        {
        case KEY_UP:
            if (f_curr < KHZMAX-1)
            {
                f_new = f_curr + 1*RES;
                freq_set_lin(&f_new);
            }
            else blinkRed();
            break;
        case KEY_DOWN:
            if (f_curr > (khz_t)0)
            {
                f_new = f_curr - 1*RES;
                freq_set_lin(&f_new);
            }
            else blinkRed();
            break;
        }
        break;
    default:
        //do nothing
        break;
    }

    //wait for both keys to be released before continuing.
    //If a key is still pressed after our wait, handle it again since user is holding it.
    waittime = holding ? 1000 : 50000;
    for(i=0;i<waittime;i++);
    keyVal=P1IN & 0b11000;
    if(keyVal == 24) return f_new;
    else
    {
        f_curr = f_new;
        holding = 1;
    }

    } while(1);
}

void blinkRed(void)
{

}
