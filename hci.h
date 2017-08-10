/*
 * hci.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Zac
 */

#ifndef HCI_H_
#define HCI_H_

#include "freq_select.h"

#define SWT
#define BUT_DOWN (P1IN && 0b10000)
#define BUT_UP   (P1IN && 0b01000)

typedef uint8_t but_t;

typedef enum
{
    AUTOSEEK,
    INCREMENT
} swt_t;

khz_t handleButtonPress(but_t button, khz_t f_curr);

#endif /* HCI_H_ */
