#include "tft.h"


unsigned char spi_transceive (unsigned char data) {
	// Put data in data register
	SPDR = data;

	// Wait until transmission is complete
	while(!(SPSR & (1<<SPIF)));

	// Return received data
	return(SPDR);
}


// TODO Change low functions to &= ~(1 << PBX)
void cs_high(void) {
	PORTB |= (1<<PB1);
}

void cs_low(void) {
	PORTB &= ~(1<<PB1);
}

void dc_high(void) {
	PORTB |= (1<<PB2);
}

void dc_low(void) {
	PORTB &= ~(1<<PB2);
}

void rst_high(void) {
	PORTD |= (1<<PD7);
}

void rst_low(void) {
	PORTD &= ~(1<<PD7);
}

void wr_cmd(unsigned char cmd) {
	cs_low();
	dc_low();
	spi_transceive(cmd);
	dc_high();
}


void tft_reset(void) {

	cs_high();
	dc_high();
	rst_low();                        // display reset

	_delay_ms(1);
	rst_high();                       // end hardware reset
	_delay_ms(5);
	
	wr_cmd(0x01);                     // SW reset
	_delay_ms(5);
	wr_cmd(0x28);                     // display off
	
	/* Start Initial Sequence ----------------------------------------------------*/
	wr_cmd(0xCF);
	spi_transceive(0x00);
	spi_transceive(0x83);
	spi_transceive(0x30);
	cs_high();
	
	wr_cmd(0xED);
	spi_transceive(0x64);
	spi_transceive(0x03);
	spi_transceive(0x12);
	spi_transceive(0x81);
	cs_high();
	
	wr_cmd(0xE8);
	spi_transceive(0x85);
	spi_transceive(0x01);
	spi_transceive(0x79);
	cs_high();
	
	wr_cmd(0xCB);
	spi_transceive(0x39);
	spi_transceive(0x2C);
	spi_transceive(0x00);
	spi_transceive(0x34);
	spi_transceive(0x02);
	cs_high();
	
	wr_cmd(0xF7);
	spi_transceive(0x20);
	cs_high();
	
	wr_cmd(0xEA);
	spi_transceive(0x00);
	spi_transceive(0x00);
	cs_high();
	
	wr_cmd(0xC0);                     // POWER_CONTROL_1
	spi_transceive(0x26);
	cs_high();
	
	wr_cmd(0xC1);                     // POWER_CONTROL_2
	spi_transceive(0x11);
	cs_high();
	
	wr_cmd(0xC5);                     // VCOM_CONTROL_1
	spi_transceive(0x35);
	spi_transceive(0x3E);
	cs_high();
	
	wr_cmd(0xC7);                     // VCOM_CONTROL_2
	spi_transceive(0xBE);
	cs_high();
	
	wr_cmd(0x36);                     // MEMORY_ACCESS_CONTROL
	spi_transceive(0x28);			   // Default: 0x48
	cs_high();
	
	wr_cmd(0x3A);                     // COLMOD_PIXEL_FORMAT_SET
	spi_transceive(0x55);                 // 16 bit pixel
	cs_high();
	
	wr_cmd(0xB1);                     // Frame Rate
	spi_transceive(0x00);
	spi_transceive(0x1B);
	cs_high();
	
	wr_cmd(0xF2);                     // Gamma Function Disable
	spi_transceive(0x08);
	cs_high();
	
	wr_cmd(0x26);
	spi_transceive(0x01);                 // gamma set for curve 01/2/04/08
	cs_high();
	
	wr_cmd(0xE0);                     // positive gamma correction
	spi_transceive(0x1F);
	spi_transceive(0x1A);
	spi_transceive(0x18);
	spi_transceive(0x0A);
	spi_transceive(0x0F);
	spi_transceive(0x06);
	spi_transceive(0x45);
	spi_transceive(0x87);
	spi_transceive(0x32);
	spi_transceive(0x0A);
	spi_transceive(0x07);
	spi_transceive(0x02);
	spi_transceive(0x07);
	spi_transceive(0x05);
	spi_transceive(0x00);
	cs_high();
	
	wr_cmd(0xE1);                     // negativ gamma correction
	spi_transceive(0x00);
	spi_transceive(0x25);
	spi_transceive(0x27);
	spi_transceive(0x05);
	spi_transceive(0x10);
	spi_transceive(0x09);
	spi_transceive(0x3A);
	spi_transceive(0x78);
	spi_transceive(0x4D);
	spi_transceive(0x05);
	spi_transceive(0x18);
	spi_transceive(0x0D);
	spi_transceive(0x38);
	spi_transceive(0x3A);
	spi_transceive(0x1F);
	cs_high();

	windowMax();
	
	
	
	wr_cmd(0x34);                     // tearing effect off
	cs_high();
	
	//wr_cmd(0x35);                     // tearing effect on
	//cs_high();
	
	wr_cmd(0xB7);                       // entry mode
	spi_transceive(0x07);
	cs_high();
	
	wr_cmd(0xB6);                       // display function control
	spi_transceive(0x0A);
	spi_transceive(0x82);
	spi_transceive(0x27);
	spi_transceive(0x00);
	cs_high();
	
	wr_cmd(0x11);                     // sleep out
	cs_high();

	_delay_ms(100);
	
	wr_cmd(0x29);                     // display on
	cs_high();
	
	_delay_ms(100);
	
}

void pixel(uint16_t x, uint16_t y, uint16_t color) {
	wr_cmd(0x2A);
	spi_transceive(x >> 8);
	spi_transceive(x);
	cs_high();
	
	wr_cmd(0x2B);
	spi_transceive(y >> 8);
	spi_transceive(y);
	cs_high();
	
	wr_cmd(0x2C);  // send pixel
	spi_transceive(color >> 8);
	spi_transceive(color & 0xFF);
	cs_high();
}

void window(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	wr_cmd(0x2A);
	spi_transceive(x >> 8);
	spi_transceive(x);
	spi_transceive((x+w-1) >> 8);
	spi_transceive(x+w-1);
	cs_high();

	wr_cmd(0x2B);
	spi_transceive(y >> 8);
	spi_transceive(y);
	spi_transceive((y+h-1) >> 8);
	spi_transceive(y+h-1);
	cs_high();
}

void windowMax(void) {
	window(0, 0, XMAX-1, YMAX-1);
}

void drawHLine(uint16_t x0, uint16_t x1, uint16_t y, uint16_t color) {
	uint16_t w;
	w = x1 - x0 + 1;
	uint16_t j;
	window(x0, y, w, 1);
	wr_cmd(0x2C);
	for (j=0; j<w; j++) {
		spi_transceive(color >> 8);
		spi_transceive(color & 0xFF);
	}
	cs_high();

	windowMax();
}

void drawVLine(uint16_t x, uint16_t y0, uint16_t y1, uint16_t color) {
	uint16_t h;
	h = y1 - y0 + 1;
	uint16_t j;
	window(x, y0, 1, h);
	wr_cmd(0x2C);
	for (j = 0; j < h; j++) {
		spi_transceive(color >> 8);
		spi_transceive(color & 0xFF);
	}
	cs_high();

	windowMax();
}

void drawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
	if (x1 > x0) drawHLine(x0,x1,y0,color);
	else  drawHLine(x1,x0,y0,color);

	if (y1 > y0) drawVLine(x0,y0,y1,color);
	else drawVLine(x0,y1,y0,color);

	if (x1 > x0) drawHLine(x0,x1,y1,color);
	else  drawHLine(x1,x0,y1,color);

	if (y1 > y0) drawVLine(x1,y0,y1,color);
	else drawVLine(x1,y1,y0,color);
}


void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
	uint16_t j;
	window(x, y, w, h);
	wr_cmd(0x2C);
	for(j = 0; j < w*h; j++) {
		spi_transceive(color >> 8);
		spi_transceive(color & 0xFF);
	}
	cs_high();

	windowMax();
}

void cls(uint16_t color) {
	// Divide screen into 8 strips, 320 wide by 30 high
	// Use a fillRect for each strip

	uint8_t j;

	for(j = 0; j < 8; j++) {

		fillRect(0, j*30, XMAX-1, 30, color);

	}

}

void testPutChar(uint16_t color) {
	window(40, 40, 8, 8);
	wr_cmd(0x2C);
	helpChar(0x18, color);
	helpChar(0x3C, color);
	helpChar(0x66, color);
	helpChar(0x66, color);
	helpChar(0x7E, color);
	helpChar(0x66, color);
	helpChar(0x66, color);
	helpChar(0x00, color);
	cs_high();
	windowMax();
}

void helpChar(uint8_t in, uint16_t color) {
	uint8_t j;
	for(j = FONT_SX; j > 0; j--) {
		if((in >> (j-1)) & 0x01) {
			spi_transceive(color >> 8);
			spi_transceive(color & 0xFF);
			} else {
			spi_transceive(0xFF);
			spi_transceive(0xFF);
		}
	}
}

void putChar(uint16_t x, uint16_t y, uint8_t ch, uint16_t color) {
	window(x, y, FONT_SX, FONT_SX); // Font is 8x8
	uint8_t ind = ch - 31; // Convert char to corresponding index in font
	uint8_t j;
	wr_cmd(0x2C);
	for(j = 0; j < FONT_SY; j++) {
		helpChar(FONT8x8[ind][j], color);
	}
	cs_high();
	windowMax();
}

void drawCross(uint16_t x, uint16_t y, uint16_t color) {
	// Draws a 15x15 cross with the center at the provided x, y coordinates
	// No check is made for boundaries
	drawVLine(x, y-7, y+7, color);
	drawHLine(x-7, x+7, y, color);
	
}

void printNum(uint16_t x, uint16_t y, uint16_t num, uint16_t color) {
	uint16_t xpos = x;

	putChar(xpos, y, ((uint8_t) (num/1000))+48, color);
	num = num % 1000;
	xpos = xpos + 8;
	putChar(xpos, y, ((uint8_t) (num/100))+48, color);
	num = num % 100;
	xpos = xpos + 8;
	putChar(xpos, y, ((uint8_t) (num/10))+48, color);
	num = num % 10;
	xpos = xpos + 8;
	putChar(xpos, y, ((uint8_t) num)+48, color);

}

void printStr(uint16_t x, uint16_t y, char* s, uint8_t len, uint16_t color) {
	uint16_t xpos = x;
	uint8_t i;

	for(i = 0; i < len; i++) {
		putChar(xpos, y, *(s+i), color);
		xpos += 8;
	}
}

void drawTestUI(void) {

	uint8_t j, i;
	
	drawRect(20, 20, 300, 220, 0x0000);
	drawRect(220, 20, 300, 36, 0x0000);

	for(i = 0; i < 2; i++) {
		for(j = 0; j < 4; j++) {
			drawRect(20+j*70, 36+i*92, 90+j*70, 128+i*92, 0x0000);
		}
	}

}

void drawValve(uint16_t xp, uint16_t yp, uint16_t color, uint8_t status) {
	// Draws a valve symbol at xp, and yp positiong
	// If status is 0, the valve is drawn closed (filled in), else open
	drawHLine(xp - VALVE_W, xp + VALVE_W, yp, color);
	uint8_t i;

	for(i = VALVE_W; i > 0; i--) {
		drawVLine(xp - i, yp-i, yp+i, color);
	}

	for(i = 0; i < VALVE_W; i++) {
		drawVLine(xp + i + 1, yp-i, yp+i, color);
	}

	if(status) {
		for(i = VALVE_W-1; i > 0; i--) {
			drawVLine(xp - i, yp-i+1, yp+i-1, COLOR_BG);
		}

		for(i = 0; i < VALVE_W-1; i++) {
			drawVLine(xp + i + 1, yp-i+1, yp+i-1, COLOR_BG);
		}
	}
}


void printToBox(uint16_t xbox, uint16_t ybox, uint16_t num) {
	printNum(xbox+19, ybox+42, num, 0x0000);
}

void printToBoxes(void) {
	uint8_t i, j;

	for(i = 0; i < 2; i++) {
		for(j = 0; j < 4; j++) {
			printToBox(20+j*70, 36+i*92, boxCount[(i*4) + j]);
		}
	}
}