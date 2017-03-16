#ifndef TOUCH_H
#define TOUCH_H


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "main.h"
#include <util/delay.h>

#define XP PC0 // ADC0
#define YP PC1 // ADC1
#define XM PC2 // ADC2
#define YM PC3 // ADC3

// NOTE Code assumes this is 2, for the moment
#define NUMSAMPLES 2

/***** Function prototypes *****/
uint16_t readX(void);
uint16_t readY(void);
uint16_t readZ(void);
uint8_t isInXSpan(uint16_t xp, uint16_t xspan, uint16_t wspan);
uint8_t isInYSpan(uint16_t yp, uint16_t yspan, uint16_t hspan);
uint8_t isInBox(uint16_t xp, uint16_t yp, uint16_t xbox, uint16_t ybox, uint16_t wbox, uint16_t hbox);

void checkBoxBounds(uint16_t xp, uint16_t yp);
/*******************************/

extern volatile uint16_t boxCount[8];

#endif