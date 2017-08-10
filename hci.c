/*
 * hci.c
 *
 *  Created on: Aug 9, 2017
 *      Author: Zac
 */

#include "hci.h"

khz_t handleButtonPress(but_t but, khz_t f_curr)
{
    khz_t f_new = KHZ606;

    switch(but)
    {
    case AUTOSEEK:
        //do autoseek and return f
        break;
    case INCREMENT:
        //do increment and return f
        break;
    default:
        //do nothing
        break;
    }

    return f_new;
}
