#ifndef MAIN_H
#define MAIN_H

#define  F_CPU 12000000UL
#define fixpShift 16 // MSB is sign, followed by an integer of length (32-fixpShift) followed by decimals of length fixpShift
#define fixpMask ((1UL << fixpShift) - 1) // All LSB set, all MSB cleared

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "tft.h"
#include "touch.h"
#include "gui.h"
#include "pid.h"
//#include "script.h"

/***** Function prototypes *****/
void init(void);
void blinkLED(void);
void testTouch(void);
void testGUI(void);
void pidLoop(void);
/*******************************/

enum states {
	MAIN,
	Q_CHAMBER,
	Q_PREPUMP,
	Q_CRYO, 
	Q_PREPUMPPIPE,
	Q_CRYOPIPE,
	Q_NUMERICS
};





#endif