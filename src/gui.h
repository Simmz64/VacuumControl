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
void drawQChamberScreen(uint16_t color);
void drawQPrepumpScreen(uint16_t color);
void drawQCryoScreen(uint16_t color);
void drawQPrepumpPipeScreen(uint16_t color);
void drawQCryoPipeScreen(uint16_t color);
void clean(void);
void buttons(uint16_t xp, uint16_t yp);
void drawBackButton(uint16_t color);
void checkBackButton(uint16_t xp, uint16_t yp);
void drawOptionButton(char* text, uint16_t color);
uint8_t checkOptionButton(uint16_t xp, uint16_t yp);
void testNumericsScreen(uint16_t color);
void drawNumericsScreen(uint16_t color, uint8_t flag);
uint8_t checkNumerics(uint16_t xp, uint16_t yp);

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

#define PIPE_W 5
#define PIPE_BTN_EXTRA 10

#define VALVE_W 15

#define OPT1_X0 20
#define OPT1_X1 160
#define OPT1_Y0 170
#define OPT1_Y1 220

#define OPT2_X0 160
#define OPT2_X1 300
#define OPT2_Y0 170
#define OPT2_Y1 220

#define NUM_BTN_W 50
#define NUM_BTN_SPC 10
#define NUM_BTN_EDGEX 20
#define NUM_BTN_EDGEY 50


extern volatile enum states state;
extern volatile uint8_t prepumpPipe;
extern volatile uint8_t cryoPipe;
//extern volatile int16_t intHolder;
//extern volatile uint16_t decHolder;
extern volatile int32_t numberHolder;

#endif