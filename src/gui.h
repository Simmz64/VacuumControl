#ifndef GUI_H
#define GUI_H

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "main.h"
#include <util/delay.h>
#include "tft.h"
#include "touch.h"

/***** Function prototypes *****/
void drawScreen(void);
void drawMainScreen(uint16_t color);
void drawQuestionScreen(uint16_t color);
void buttons(uint16_t xp, uint16_t yp);
/*******************************/

#define CHAMBER_X 80
#define CHAMBER_Y 102
#define CHAMBER_W 60
#define CHAMBER_H 30
#define CHAMBER_RIM 2

#define PREPUMP_X 225
#define PREPUMP_Y 45
#define PREPUMP_W 50
#define PREPUMP_H 25

#define CRYO_X 225
#define CRYO_Y 160
#define CRYO_W 50
#define CRYO_H 25

extern volatile enum states state;

#endif