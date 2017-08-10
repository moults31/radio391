/*
 * hci.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Zac
 */

#include "hci.h"

khz_t handleKeyPress(key_t key, khz_t f_curr)
{
    khz_t f_new = KHZ606;
    int swt = 1;
    volatile int keyVal;

    switch(swt)
    {
    case AUTOSEEK:
        //do autoseek and return f
        break;
    case INCREMENT:
        switch(key)
        {
        case KEY_UP:
            if (f_curr < KHZMAX-1)
            {
                f_new = f_curr + 1;
                freq_set(f_new);
            }
            else blinkRed();
            break;
        case KEY_DOWN:
            if (f_curr > (khz_t)0)
            {
                f_new = f_curr - 1;
                freq_set(f_new);
            }
            else blinkRed();
            break;
        }
        break;
    default:
        //do nothing
        break;
    }

    //wait for both keys to be released before continuing
    while(1)
    {
        keyVal=P1IN & 0b11000;
        if(keyVal == 24) return f_new;
    }
}

void blinkRed(void)
{

}
