#ifndef TFT_H
#define TFT_H


#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include "FONT8x8.h"
#include "main.h"
#include <util/delay.h>



#define XMAX 320
#define YMAX 240
#define FONT_SX 8
#define FONT_SY 8

#define COLOR_BG 0xFFFF
#define COLOR_FG 0x0000
#define COLOR_R 0xF800
#define COLOR_G 0x07D0
#define COLOR_B 0x001F

/***** Function prototypes *****/
unsigned char spi_transceive(unsigned char data);
void cs_high(void);
void cs_low(void);
void dc_high(void);
void dc_low(void);
void rst_high(void);
void rst_low(void);
void wr_cmd(unsigned char cmd);
void tft_reset(void);
void pixel(uint16_t x, uint16_t y, uint16_t color);
void window(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void windowMax(void);
void drawHLine(uint16_t x0, uint16_t x1, uint16_t y, uint16_t color);
void drawVLine(uint16_t x, uint16_t y0, uint16_t y1, uint16_t color);
void drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void cls(uint16_t color);
void testPutChar(uint16_t color);
void helpChar(uint8_t in, uint16_t color);
void putChar(uint16_t x, uint16_t y, uint8_t ch, uint16_t color);
void drawCross(uint16_t x, uint16_t y, uint16_t color);
void printNum(uint16_t x, uint16_t y, uint16_t num, uint16_t color);
void printStr(uint16_t x, uint16_t y, char* s, uint8_t len, uint16_t color);
void drawTestUI(void);
void drawValve(uint16_t xp, uint16_t yp, uint16_t color, uint8_t status);

void printToBox(uint16_t xbox, uint16_t ybox, uint16_t num);
void printToBoxes(void);
/*******************************/

extern volatile uint16_t boxCount[8];

#endif