#include "pid.h"


volatile enum pidstates pidstate = PID_MAIN;
extern volatile uint8_t decOn;
volatile int8_t sign = 1;
volatile int32_t Kp = 0, Ki = 0, Kd = 0;


int16_t diff(int16_t e, int16_t e_prev) {
	// Time discrete differentiation
	return e - e_prev;
}


// Reads an analog value from the ADC on port PC4
uint16_t pidRead(void) {
	DDRC |= (1 << PID_IN);

	// Choose PC4 as input
	ADMUX = (0 << REFS1)|(0 << REFS0)|(0 << ADLAR)|(1 << MUX2)|(0 << MUX1)|(0 << MUX0);

	_delay_us(20);

	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	return ADC;
}


// Outputs a signal using PWM
// The port should be connected to a LP filter to smooth the signal level
void pidWrite(uint8_t in) {
	// TIMER2
	//   mode 3: fast PWM   top=0xff
	//   clk/1 prescaler

	TCCR2A = (1 << COM2A1) | (0 << COM2A0) 
     	| (1 << COM2B1) | (0 << COM2B0)
     	| (1 << WGM21)  | (1 << WGM20);
	TCCR2B = (0 << WGM22)
     	| (0 << CS22)   | (0 << CS21)   | (1 << CS20);
	
	OCR2B  = in;    // output compare register A -> pin 11, used as trigger
	DDRD   = (1 << PID_OUT);   // set PWM-pins to output
}


void gotoState(enum pidstates nextstate) {
	drawCurrentState(COLOR_BG); // Perform cleanup
	pidstate = nextstate;
	drawCurrentState(COLOR_FG); // Draw new state
}


// Graphics for the PID
void drawCurrentState(uint16_t color) {
	switch(pidstate) {
		case PID_MAIN:
			// Draw main screen
			drawPIDMain(color);
			break;
		case PID_KP:
			// Draw numerical input with KP status
			drawPIDKP(color);
			break;
		case PID_KI:
			// Draw numerical input with KI status
			drawPIDKI(color);
			break;
		case PID_KD:
			// Draw numerical input with KD status
			drawPIDKD(color);
			break;
		default:
			break;
	}
}

void drawPIDMain(uint16_t color) {
	// Draw KP rectangle
	drawRect((XMAX>>1) - PID_RECTW, (YMAX>>2) - PID_RECTH, (XMAX>>1) + PID_RECTW, (YMAX>>2) + PID_RECTH, color);
	printStr((XMAX>>1) - FONT_SX, (YMAX>>2) - (PID_RECTH >> 1) - (FONT_SY>>1), "Kp", 2, color);
	printFixpDec((XMAX>>1) - PID_RECTW + FONT_SX, (YMAX>>2) + (PID_RECTH >> 1) - (FONT_SY>>1), Kp, color);

	// Draw KI rectangle
	drawRect((XMAX>>1) - PID_RECTW, (YMAX>>1) - PID_RECTH, (XMAX>>1) + PID_RECTW, (YMAX>>1) + PID_RECTH, color);
	printStr((XMAX>>1) - FONT_SX, (YMAX>>1) - (PID_RECTH >> 1) - (FONT_SY>>1), "Ki", 2, color);
	printFixpDec((XMAX>>1) - PID_RECTW + FONT_SX, (YMAX>>1) + (PID_RECTH >> 1) - (FONT_SY>>1), Ki, color);

	// Draw KD rectangle
	drawRect((XMAX>>1) - PID_RECTW, (YMAX>>1) + (YMAX>>2) - PID_RECTH, (XMAX>>1) + PID_RECTW, (YMAX>>1) + (YMAX>>2) + PID_RECTH, color);
	printStr((XMAX>>1) - FONT_SX, (YMAX>>1) + (YMAX>>2) - (PID_RECTH >> 1) - (FONT_SY>>1), "Kd", 2, color);
	printFixpDec((XMAX>>1) - PID_RECTW + FONT_SX, (YMAX>>1) + (YMAX>>2) + (PID_RECTH >> 1) - (FONT_SY>>1), Kd, color);


	// Draw some lines to make it look better
}

void drawPIDKP(uint16_t color) {
	drawNumericsScreen(color);
	fillRect(20, 20, 14*8, 8, COLOR_BG);
	printFixpDec(20, 20, Kp, color);
}

void drawPIDKI(uint16_t color) {
	drawNumericsScreen(color);
	fillRect(20, 20, 14*8, 8, COLOR_BG);
	printFixpDec(20, 20, Ki, color);
}

void drawPIDKD(uint16_t color) {
	drawNumericsScreen(color);
	fillRect(20, 20, 14*8, 8, COLOR_BG);
	printFixpDec(20, 20, Kd, color);
}

// Function to interpret if a button has been pressed
void pidButtons(uint16_t xp, uint16_t yp) {
	
	switch(pidstate) {
		case PID_MAIN:
			// Is a rectangle pressed?
			if(xp > (XMAX>>1) - PID_RECTW && xp < (XMAX>>1) + PID_RECTW) {
				// X-wise all checks are made here, only need to check y now
				if(yp > (YMAX>>2) - PID_RECTH && yp < (YMAX>>2) + PID_RECTH) {
					// Kp is pressed
					gotoState(PID_KP);
				}

				if(yp > (YMAX>>1) - PID_RECTH && yp < (YMAX>>1) + PID_RECTH) {
					// Ki is pressed
					gotoState(PID_KI);
				}

				if(yp > (YMAX>>1) + (YMAX>>2) - PID_RECTH && yp < (YMAX>>1) + (YMAX>>2) + PID_RECTH) {
					// Kd is pressed
					gotoState(PID_KD);
				}
			}
			break;
		case PID_KP:
			// Numerical input for KP
			readNumericsScreen(xp, yp, &Kp);
			break;
		case PID_KI:
			// Numerical input for KI
			readNumericsScreen(xp, yp, &Ki);
			break;
		case PID_KD:
			// Numerical input for KD
			readNumericsScreen(xp, yp, &Kd);
			break;
		default:
			break;
	}
}


void readNumericsScreen(uint16_t xp, uint16_t yp, volatile int32_t* k) {
	uint8_t inc = 0;
	int32_t tempInt = 0, tempDec = 0;

	inc = checkNumerics(xp, yp);
	if(inc < 10) {
		if(decOn) {
			tempDec = *k & fixpMask;
			tempInt = *k & ~fixpMask;
			tempDec *= 10;
			tempDec += inc;

			if(tempDec <= 32767 && tempDec >= -32767) {
				*k = tempDec + tempInt;
			}

		} else {
			tempInt = *k & ~fixpMask; // Get MSBs
			tempInt >>= fixpShift;
			tempDec = *k & fixpMask; // Get LSBs
			tempInt *= 10;
			tempInt += sign*inc;

			if(tempInt <= 32767 && tempInt >= -32767) {
				*k = tempDec + (tempInt << fixpShift);
			}
		}

	} else if(inc == 10) {
		*k = 0;
	} else if(inc == 11) {
		gotoState(PID_MAIN);
	} else if(inc == 12) {
		decOn = !decOn;
	} else if(inc == 13) {
		*k = ~(*k) + 1; //Flip first bit (sign bit)
		sign *= -1;
	}
}
