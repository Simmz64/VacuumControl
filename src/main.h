#ifndef MAIN_H
#define MAIN_H

#define  F_CPU 12000000UL

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

/***** Function prototypes *****/
void init(void);
void blinkLED(void);
void testTouch(void);
void testGUI(void);
/*******************************/

enum states {
	MAIN,
	Q_CHAMBER,
	Q_PREPUMP,
	Q_CRYO, 
	Q_PREPUMPPIPE,
	Q_CRYOPIPE
};

#endif