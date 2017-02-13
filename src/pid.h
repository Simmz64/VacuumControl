#ifndef PID_H
#define PID_H

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "main.h"
#include <util/delay.h>
#include "tft.h"
#include "touch.h"

#define PID_IN PC4		//If this is changed, the ADMUX in pidRead should also be changed
#define PID_OUT PD3

/***** Function prototypes *****/
int16_t diff(int16_t e, int16_t e_prev);
uint16_t pidRead(void);
void pidWrite(uint8_t in);
/*******************************/

#endif