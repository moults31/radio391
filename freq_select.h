/*
 * freq_select.h
 *
 *  Created on: Aug 9, 2017
 *      Author: Zac
 */

#ifndef FREQ_SELECT_H_
#define FREQ_SELECT_H_

//named indexes into LUT
typedef enum{
    KHZ606, KHZ610, KHZ620, KHZ640, KHZ650,
    KHZ670, KHZ680, KHZ690, KHZ700, KHZ720,
    KHZ730, KHZ750, KHZ780, KHZ800, KHZ820,
    KHZ830, KHZ850, KHZ870, KHZ900, KHZ920,
    KHZ930, KHZ940, KHZ975, KHZ980, KHZ990,
    KHZ1000, KHZ1010, KHZ1020, KHZ1030, KHZ1050,
    KHZ1060, KHZ1080, KHZ1100, KHZ1120, KHZ1140,
    KHZ1150, KHZ1170, KHZ1190, KHZ1200, KHZ1210,
    KHZ1220, KHZ1230, KHZ1240, KHZ1250, KHZ1260,
    KHZ1270, KHZ1290, KHZ1300, KHZ1320, KHZ1330,
    KHZ1350, KHZ1400
} khz_t;



void sr_set_tens(uint16_t vals);
void sr_set_hunds(uint16_t vals);
void freq_set(khz_t f);

#endif /* FREQ_SELECT_H_ */
