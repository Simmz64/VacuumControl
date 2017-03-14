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
#define PID_OUT1 PD5
#define PID_OUT2 PD6
#define PID_RECTW 50	// Width and height are measure from center to edge
#define PID_RECTH 25	// IE the actual width and height are 2 times these values

enum pidstates {
	PID_MAIN,
	PID_KP,
	PID_KI,
	PID_KD,
	PID_HEAD
};


/***** Function prototypes *****/
void pidInit(void);
uint16_t pidRead(void);
void pidCh1Write(uint8_t in);
void pidCh2Write(uint8_t in);
void gotoState(enum pidstates nextstate);
void drawCurrentState(uint16_t color);
void drawPIDMain(uint16_t color);
void drawPIDKP(uint16_t color);
void drawPIDKI(uint16_t color);
void drawPIDKD(uint16_t color);
void drawPIDHead(uint16_t color);
void pidButtons(uint16_t xp, uint16_t yp);
//void readNumericsScreen(uint16_t xp, uint16_t yp, volatile int32_t* k);
void readNumericsScreen(uint16_t xp, uint16_t yp, int16_t* integerpart, int16_t* decimalpart, uint16_t* scl);
void readUintScreen(uint16_t xp, uint16_t yp, uint16_t* k);
void readPos(void);
void meanPos(void);
void errCalc(void);
void adjustHeadingSimple(void);
void adjustHeading(void);
void stopTurn(void);
void turnLeft(void);
void turnRight(void);
void printPIDGain(uint16_t xp, uint16_t yp, int16_t* integerpart, int16_t* decimalpart, uint16_t color);
/*******************************/

extern volatile uint16_t potentiom;
extern volatile uint8_t decOn;



#endif