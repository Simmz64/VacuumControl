#include "pid.h"


volatile enum pidstates pidstate = PID_MAIN;
volatile int8_t sign = 1;
//volatile int32_t Kp = 0, Ki = 0, Kd = 0;
uint16_t posbuf[5] = {0, 0, 0, 0, 0};
uint8_t bufptr = 0;
uint16_t pos = 0;
uint16_t dest = 512;
int16_t err = 0;
int32_t integrerr = 0;
int16_t deriverr = 0;
int16_t preverr = 0;
int16_t dtinv = 10;
uint16_t head = 400;
uint8_t update = 1;

int16_t Kp_int = 0, Ki_int = 0, Kd_int = 0;
int16_t Kp_dec = 0, Ki_dec = 0, Kd_dec = 0;
uint16_t Kp_scl = 1, Ki_scl = 1, Kd_scl = 1;


/************************************** ADC ***************************************/
void pidInit(void) {
	// TIMER0
	//   mode 3: fast PWM   top=0xff
	//   clk/1 prescaler

	TCCR0A = (1 << COM0A1) | (0 << COM0A0) 
     	| (1 << COM0B1) | (0 << COM0B0)
     	| (1 << WGM01)  | (1 << WGM00);
	TCCR0B = (0 << WGM02)
     	| (0 << CS02)   | (0 << CS01)   | (1 << CS00);

    DDRD   |= (1 << PID_OUT1)|(1 << PID_OUT2)|(1 << PD4)|(1 << PD3);   // set PWM-pins to output
}

// Reads an analog value from the ADC on port PC4
uint16_t pidRead(void) {
	//DDRC &= ~(1 << PID_IN);

	// Choose PC4 as input
	ADMUX = (0 << REFS1)|(0 << REFS0)|(0 << ADLAR)|(1 << MUX2)|(0 << MUX1)|(0 << MUX0);

	_delay_us(20);

	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));

	return ADC;
}


// Outputs a signal using PWM
// The port should be connected to a LP filter to smooth the signal level
// Ch1 is configured to PD5
void pidCh1Write(uint8_t in) {

	
	OCR0B  = in;    // output compare register A -> pin 11, used as trigger

}

// Ch2 is configured to PD6
void pidCh2Write(uint8_t in) {

	OCR0A  = in;    // output compare register A -> pin 11, used as trigger

}
/************************************** ADC ***************************************/


void gotoState(enum pidstates nextstate) {
	drawCurrentState(COLOR_BG); // Perform cleanup
	pidstate = nextstate;
	drawCurrentState(COLOR_FG); // Draw new state
}


/************************************** Drawing methods ***************************************/
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
		case PID_HEAD:
			drawPIDHead(color);
			break;
		default:
			break;
	}
}

void drawPIDMain(uint16_t color) {
	// Draw KP rectangle
	drawRect((XMAX>>1) - PID_RECTW, (YMAX>>2) - PID_RECTH, (XMAX>>1) + PID_RECTW, (YMAX>>2) + PID_RECTH, color);
	printStr((XMAX>>1) - FONT_SX, (YMAX>>2) - (PID_RECTH >> 1) - (FONT_SY>>1), "Kp", 2, color);
	printPIDGain((XMAX>>1) - PID_RECTW + FONT_SX, (YMAX>>2) + (PID_RECTH >> 1) - (FONT_SY>>1), &Kp_int, &Kp_dec, color);

	// Draw KI rectangle
	drawRect((XMAX>>1) - PID_RECTW, (YMAX>>1) - PID_RECTH, (XMAX>>1) + PID_RECTW, (YMAX>>1) + PID_RECTH, color);
	printStr((XMAX>>1) - FONT_SX, (YMAX>>1) - (PID_RECTH >> 1) - (FONT_SY>>1), "Ki", 2, color);
	printPIDGain((XMAX>>1) - PID_RECTW + FONT_SX, (YMAX>>1) + (PID_RECTH >> 1) - (FONT_SY>>1), &Ki_int, &Ki_dec, color);

	// Draw KD rectangle
	drawRect((XMAX>>1) - PID_RECTW, (YMAX>>1) + (YMAX>>2) - PID_RECTH, (XMAX>>1) + PID_RECTW, (YMAX>>1) + (YMAX>>2) + PID_RECTH, color);
	printStr((XMAX>>1) - FONT_SX, (YMAX>>1) + (YMAX>>2) - (PID_RECTH >> 1) - (FONT_SY>>1), "Kd", 2, color);
	printPIDGain((XMAX>>1) - PID_RECTW + FONT_SX, (YMAX>>1) + (YMAX>>2) + (PID_RECTH >> 1) - (FONT_SY>>1), &Kd_int, &Kd_dec, color);

	// Draw potentiometer value
	fillRect(20, 20, 20+8*5, 28, COLOR_BG);
	printNum(20, 20, pos, color);

	// Draw heading select rectangle
	drawRect(20, (YMAX>>1) - PID_RECTH, 100, (YMAX>>1) + PID_RECTH, color);
	printStr(20 + FONT_SX, (YMAX>>1) - (PID_RECTH >> 1) - (FONT_SY>>1), "Heading", 7, color);
	//fillRect(20 + FONT_SX, (YMAX>>1) + (PID_RECTH >> 1) - (FONT_SY>>1), 20 + 7*FONT_SX, (YMAX>>1) + (PID_RECTH >> 1) + (FONT_SY>>1), color);
	//fillRect(20 + FONT_SX, (YMAX>>1) + (PID_RECTH >> 1) - (FONT_SY>>1), 20 + 8*FONT_SX, (YMAX>>1) + (PID_RECTH >> 1) - (FONT_SY>>1), COLOR_BG);
	printNum(20 + FONT_SX, (YMAX>>1) + (PID_RECTH >> 1) - (FONT_SY>>1), head, color);

	// Draw some lines to make it look better
}

void drawPIDKP(uint16_t color) {
	drawNumericsScreen(color);
	fillRect(20, 20, 14*8, 8, COLOR_BG);
	printPIDGain(20, 20, &Kp_int, &Kp_dec, color);
}

void drawPIDKI(uint16_t color) {
	drawNumericsScreen(color);
	fillRect(20, 20, 14*8, 8, COLOR_BG);
	printPIDGain(20, 20, &Ki_int, &Ki_dec, color);
}

void drawPIDKD(uint16_t color) {
	drawNumericsScreen(color);
	fillRect(20, 20, 14*8, 8, COLOR_BG);
	printPIDGain(20, 20, &Kd_int, &Kd_dec, color);
}

void drawPIDHead(uint16_t color) {
	drawNumericsScreen(color);
	fillRect(20, 20, 14*8, 8, COLOR_BG);
	printFixpDec(20, 20, head, color);
}
/************************************** Drawing methods ***************************************/



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
			} else if(xp > (XMAX>>2) - PID_RECTW && xp < (XMAX>>2) + PID_RECTW) {
				if(yp > (YMAX>>1) - PID_RECTH && yp < (YMAX>>1) + PID_RECTH) {
					gotoState(PID_HEAD);
				}
			}
			break;
		case PID_KP:
			// Numerical input for KP
			readNumericsScreen(xp, yp, &Kp_int, &Kp_dec, &Kp_scl);
			break;
		case PID_KI:
			// Numerical input for KI
			readNumericsScreen(xp, yp, &Ki_int, &Ki_dec, &Ki_scl);
			break;
		case PID_KD:
			// Numerical input for KD
			readNumericsScreen(xp, yp, &Kd_int, &Kd_dec, &Kd_scl);
			break;
		case PID_HEAD:
			// Numerical input for heading
			readUintScreen(xp, yp, &head);
		default:
			break;
	}
}


//void readNumericsScreen(uint16_t xp, uint16_t yp, volatile int32_t* k) {
void readNumericsScreen(uint16_t xp, uint16_t yp, int16_t* integerpart, int16_t* decimalpart, uint16_t* scl) {
	uint8_t inc = 0;
	int32_t tempInt = 0;
	uint16_t tempDec = 0, tempscl = *scl;

	inc = checkNumerics(xp, yp);
	if(inc < 10) {
		if(decOn) {
			tempDec = *decimalpart;
			tempDec *= 10;
			tempDec += inc;
			tempscl *= 10;

			if(tempDec <= 32767 && tempDec >= 0) {
				*decimalpart = tempDec;
				*scl = tempscl;
			}

		} else {
			tempInt = *integerpart;
			tempInt *= 10;
			tempInt += sign*inc;

			if(tempInt <= 32767 && tempInt >= -32767) {
				*integerpart = tempInt;
			}
		}

	} else if(inc == 10) {
		*decimalpart = 0;
		*integerpart = 0;
		*scl = 1;
	} else if(inc == 11) {
		gotoState(PID_MAIN);
	} else if(inc == 12) {
		decOn = !decOn;
	} else if(inc == 13) {
		*integerpart = -1 * (*integerpart); //Flip first bit (sign bit)
		sign *= -1;
	}

/********* ALTERED TO USE TWO VARIABLES TO HOLD FIXPOINT
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
*/
}

void readUintScreen(uint16_t xp, uint16_t yp, uint16_t* k) {
	uint8_t inc = 0;
	int32_t tempInt = 0;
	update = 0;

	inc = checkNumerics(xp, yp);
	if(inc < 10) {
		tempInt = *k * 10;
		tempInt += inc;
		if(tempInt < 1024 && tempInt > 0) {
			*k = tempInt;
		}
	} else if(inc == 10) {
		*k = 0;
	} else if(inc == 11) {
		update = 1;
		gotoState(PID_MAIN);
	} else if(inc == 12) {
		//decOn = !decOn;
	} else if(inc == 13) {
		//*k = ~(*k) + 1; //Flip first bit (sign bit)
		//sign *= -1;
	}
}

// Function to read a position value and place it in the ringbuffer
void readPos(void) {
	posbuf[bufptr] = pidRead();
	bufptr++;
	bufptr %= 5;
	meanPos();
}

// Function to return mean value of readings in ringbuffer
void meanPos(void) {
	uint16_t vals = 0;
	uint8_t i;
	for(i = 0; i < 5; i++) {
		vals += posbuf[i];
	}

	pos = vals/5;
}

// Function to calculate the error (min (|A|, |B|, |C|))
void errCalc(void) {
	

	if(update) {
		dest = head;
	}
	
	int16_t A, B, C;
	A = pos - dest;
	A = abs(A);
	B = pos - (dest + 1023);
	B = abs(B);
	C = pos - (dest - 1023);
	C = abs(C);

	if(A < B && A < C) {
		err = pos - dest;
	} else if(B < C) {
		err = pos - (dest + 1023);
	} else {
		err = pos - (dest - 1023);
	}

	if(!(integrerr > 2000000) && (Ki_int != 0 || Ki_dec != 0)) {
		integrerr += err;
	} else if (Ki_int == 0 && Ki_dec == 0) {
		integrerr = 0;
	}
	deriverr = err - preverr;
	preverr = err;
}

// Simple heading adjustment
void adjustHeadingSimple(void) {
	errCalc();
	if(err > 0) {
		// Turn counter-clockwise
         pidCh1Write(0);
         pidCh2Write(255);
	} else if(err < 0) {
		// Turn clockwise
		pidCh2Write(0);
        pidCh1Write(255);
	}
}

// Adjust heading using PID-conntrol
void adjustHeading(void) {
	// PID-algorithm: out = Kp * err + (Ki * integrerr * dt) + ((Kd * deriverr) / dt);
	// integrerr adds err every loop
	// deriverr is current err - previous err
	// dt is execution time of loop / how often outputs are sent
	errCalc();
	
	int32_t outint = ((int32_t) Ki_int * integrerr)/dtinv + Kp_int * err + (Kd_int * deriverr)*(dtinv);
	int32_t outdec = (((int32_t) Ki_dec * integrerr) / Ki_scl) / dtinv + (Kp_dec * err) / Kp_scl + ((Kd_dec * deriverr * (dtinv)) / Kd_scl) ;
	int32_t out = outint + outdec;


	if(out > 0) {
		turnLeft();
	} else if(out < 0) {
		turnRight();
	} else {
		stopTurn();
	}

	if(out < -255 || out > 255) {
		pidCh1Write(0);
	} else if (out < 0) {
		pidCh1Write(255 + out);
	} else {
		pidCh1Write(255 - out);
	}


	fillRect(200, 20, 64, 8, COLOR_BG);
	printNum(200, 20, out, COLOR_FG);
}

void stopTurn(void) {
	PORTD &= ~((1 << PD4) | (1 << PD3));
}

void turnLeft(void) {
	// Break
	stopTurn();
	// Make
	PORTD |= (1 << PD4);
}

void turnRight(void) {
	// Break
	stopTurn();
	// Make
	PORTD |= (1 << PD3);
}

void printPIDGain(uint16_t xp, uint16_t yp, int16_t* integerpart, int16_t* decimalpart, uint16_t color) {
	char buf[16];
	itoa(*integerpart, buf, 10);
	uint8_t len = strlen(buf);
	buf[len] = '.';
	itoa(*decimalpart, (buf + len + 1), 10);
	len = strlen(buf);
	printStr(xp, yp, buf, len, color);
}