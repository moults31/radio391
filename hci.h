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
#define PIN_KEY_DOWN (P1IN && 0b10000)
#define PIN_KEY_UP   (P1IN && 0b01000)

typedef enum
{
    KEY_DOWN,
    KEY_UP
} key_t;

typedef enum
{
    INCREMENT,
    AUTOSEEK
} swt_t;

double handleKeyPress(key_t key, double f_curr);
void blinkRed(void);

#endif /* HCI_H_ */
