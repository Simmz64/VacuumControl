#include "gui.h"

/* How to add another screen
1. Add the state to enum statess (main.h)
2. Add case in buttons
3. Add draw method
4. Add case in drawScreen
5. Add case to clean
6. Add additional detail to main screen
*/


void drawScreen(void) {
	switch(state) {
		case MAIN:
			drawMainScreen(COLOR_FG);
			break;
		case Q_CHAMBER:
			drawQChamberScreen(COLOR_FG);
			break;
		case Q_PREPUMP:
			//drawQPrepumpScreen(COLOR_FG);
			drawNumericsScreen(COLOR_FG);
			break;
		case Q_CRYO:
			drawQCryoScreen(COLOR_FG);
			break;	
		case Q_PREPUMPPIPE:
			drawQPrepumpPipeScreen(COLOR_FG);
			break;
		case Q_CRYOPIPE:
			drawQCryoPipeScreen(COLOR_FG);
			break;
		default:
			break;
	}

}

void drawMainScreen(uint16_t color) {
	uint16_t color2 = COLOR_R;

	if(color == COLOR_BG) {
		color2 = COLOR_BG;
	}


	// Draw Chamber rectangle
	drawRect(CHAMBER_X - CHAMBER_W, CHAMBER_Y - CHAMBER_H, CHAMBER_X + CHAMBER_W, CHAMBER_Y + CHAMBER_H, color);
	drawRect(CHAMBER_X - CHAMBER_W - CHAMBER_RIM, CHAMBER_Y - CHAMBER_H - CHAMBER_RIM, CHAMBER_X + CHAMBER_W + CHAMBER_RIM, CHAMBER_Y + CHAMBER_H + CHAMBER_RIM, color);
	printStr(CHAMBER_X - 3.5*8, CHAMBER_Y - 4, "Chamber", 7, color);

	// Draw Prepump rectangle
	drawRect(PREPUMP_X - PREPUMP_W, PREPUMP_Y - PREPUMP_H, PREPUMP_X + PREPUMP_W, PREPUMP_Y + PREPUMP_H, color);
	printStr(PREPUMP_X - 3.5*8, PREPUMP_Y - 4, "Prepump", 7, color2);

	// Draw Cryopump rectangle
	drawRect(CRYO_X - CRYO_W, CRYO_Y - CRYO_H, CRYO_X + CRYO_W, CRYO_Y + CRYO_H, color);
	printStr(CRYO_X - 4*8, CRYO_Y - 4, "Cryopump", 8, color);

	// Draw Chamber to Prepump pipe
	drawRect(CHAMBER_X - PIPE_W, PREPUMP_Y - PIPE_W, CHAMBER_X + PIPE_W, CHAMBER_Y - CHAMBER_H - CHAMBER_RIM, color);
	drawRect(CHAMBER_X - PIPE_W, PREPUMP_Y - PIPE_W, PREPUMP_X - PREPUMP_W, PREPUMP_Y + PIPE_W, color);
	if(prepumpPipe) {
		//Open
		printStr(((3*CHAMBER_X + PREPUMP_X)>>2) - 16, PREPUMP_Y - 3, "Open", 4, color);
	} else {
		//Closed
		printStr(((3*CHAMBER_X + PREPUMP_X)>>2) - 24, PREPUMP_Y - 3, "Closed", 6, color);
	}

	//printStr()

	// Draw Chamber to Cryopump pipe
	drawRect(CHAMBER_X - PIPE_W, CRYO_Y + PIPE_W, CHAMBER_X + PIPE_W, CHAMBER_Y + CHAMBER_H + CHAMBER_RIM, color);
	drawRect(CHAMBER_X - PIPE_W, CRYO_Y - PIPE_W, CRYO_X - CRYO_W, CRYO_Y + PIPE_W, color);
	if(cryoPipe) {
		//Open
		printStr(((3*CHAMBER_X + CRYO_X)>>2) - 16, CRYO_Y - 3, "Open", 4, color);
	} else {
		//Closed
		printStr(((3*CHAMBER_X + CRYO_X)>>2) - 24, CRYO_Y - 3, "Closed", 6, color);
	}
	

	// Draw Valves
	//drawValve((CHAMBER_X + PREPUMP_X)>>1, PREPUMP_Y, color, 1);
	//drawValve((CHAMBER_X + CRYO_X)>>1, CRYO_Y, color, 0);
	// ^Draws too slow

}


void drawQuestionScreen(uint16_t color) {
	char* s = "This is the question screen";
	printStr(60, 120, s, strlen(s), color);
}

void drawQChamberScreen(uint16_t color) {
	drawRect(20, 20, 300, 220, color);
	char* s = "This is the Chamber screen";
	printStr(40, 90, s, strlen(s), color);
	
	// Draw optionbox 1
	drawBackButton(color);

	// Draw optionbox 2
	drawRect(OPT2_X0, OPT2_Y0, OPT2_X1, OPT2_Y1, color);
	char* s2 = "Test";
	uint16_t len = strlen(s2);
	uint16_t offset = len*FONT_SX;
	printStr(((OPT2_X0+OPT2_X1)>>1) - (offset>>1), ((OPT2_Y0+OPT2_Y1)>>1) - (FONT_SY>>1), s2, len, color); 

}

void drawQPrepumpScreen(uint16_t color) {
	drawRect(20, 20, 300, 220, color);
	char* s = "This is the Prepump screen";
	printStr(40, 90, s, strlen(s), color);

	// Draw optionbox 1
	drawBackButton(color);

	// Draw optionbox 2
	drawRect(OPT2_X0, OPT2_Y0, OPT2_X1, OPT2_Y1, color);
	char* s2 = "Test";
	uint16_t len = strlen(s2);
	uint16_t offset = len*FONT_SX;
	printStr(((OPT2_X0+OPT2_X1)>>1) - (offset>>1), ((OPT2_Y0+OPT2_Y1)>>1) - (FONT_SY>>1), s2, len, color);
}

void drawQCryoScreen(uint16_t color) {
	drawRect(20, 20, 300, 220, color);
	char* s = "This is the Cryopump screen";
	printStr(40, 90, s, strlen(s), color);

	// Draw optionbox 1
	drawBackButton(color);

	// Draw optionbox 2
	drawOptionButton("Test", color);
}

void drawQPrepumpPipeScreen(uint16_t color) {
	drawRect(20, 20, 300, 220, color);
	char* s = "This is the Prepump PIPE screen";
	printStr(40, 120, s, strlen(s), color);

	drawBackButton(color);

	if(prepumpPipe) {
		drawOptionButton("Close valve", color);
	} else {
		drawOptionButton(" Open valve ", color);
	}
}

void drawQCryoPipeScreen(uint16_t color) {
	drawRect(20, 20, 300, 220, color);
	char* s = "This is the Cryo PIPE screen";
	printStr(40, 90, s, strlen(s), color);

	// Draw optionbox 1
	drawBackButton(color);

	// Draw optionbox 2
	if(cryoPipe) {
		drawOptionButton("Close valve", color);
	} else {
		drawOptionButton(" Open valve ", color);
	}
}

void clean(void) {
	switch (state) {
		case MAIN:
			drawMainScreen(COLOR_BG);
			break;
		case Q_CHAMBER:
			drawQChamberScreen(COLOR_BG);
			break;
		case Q_PREPUMP:
			drawQPrepumpScreen(COLOR_BG);
			break;
		case Q_CRYO:
			drawQCryoScreen(COLOR_BG);
			break;
		case Q_PREPUMPPIPE:
			drawQPrepumpPipeScreen(COLOR_BG);
			break;
		case Q_CRYOPIPE:
			drawQCryoPipeScreen(COLOR_BG);
			break;
		case Q_NUMERICS:
			drawNumericsScreen(COLOR_BG);
			break;
		default:
			break;

	}
}


// Checks whether a button is pressed at xp and yp
void buttons(uint16_t xp, uint16_t yp) {
	uint8_t num, pos = 20;
	//Check chamber
	switch(state) {
		case MAIN:

			// Press CHAMBER
			if(xp < (CHAMBER_X + CHAMBER_W) && xp > (CHAMBER_X - CHAMBER_W)) {
				if(yp < (CHAMBER_Y + CHAMBER_H) && yp > (CHAMBER_Y - CHAMBER_H)) {
					clean();
					state = Q_CHAMBER;
				}
			}

			// Press PREPUMP
			if(xp < (PREPUMP_X + PREPUMP_W) && xp > (PREPUMP_X - PREPUMP_W)) {
				if(yp < (PREPUMP_Y + PREPUMP_H) && yp > (PREPUMP_Y - PREPUMP_H)) {
					clean();
					state = Q_PREPUMP;
				}
			}

			// Press CRYOPUMP
			if(xp < (CRYO_X + CRYO_W) && xp > (CRYO_X - CRYO_W)) {
				if(yp < (CRYO_Y + CRYO_H) && yp > (CRYO_Y - CRYO_H)) {
					clean();
					state = Q_CRYO;
				}
			}

			// Press PREPUMPPIPE
			if(xp < (PREPUMP_X - PREPUMP_W) && xp > (CHAMBER_X - PIPE_W)) {
				if(yp < (PREPUMP_Y + PIPE_W + PIPE_BTN_EXTRA) && yp > (PREPUMP_Y - PIPE_W - PIPE_BTN_EXTRA)) {
					clean();
					state = Q_PREPUMPPIPE;
				}
			}

			// Press CRYOPIPE
			if(xp < (CRYO_X - CRYO_W) && xp > (CHAMBER_X - PIPE_W)) {
				if(yp < (CRYO_Y + PIPE_W + PIPE_BTN_EXTRA) && yp > (CRYO_Y - PIPE_W - PIPE_BTN_EXTRA)) {
					clean();
					state = Q_CRYOPIPE;
				}
			}
			break;
		

		case Q_CHAMBER:
			checkBackButton(xp, yp);

			if(checkOptionButton(xp, yp)) {
				clean();
				drawQChamberScreen(COLOR_B);
			}
			break;


		case Q_PREPUMP:
			num = checkNumerics(xp, yp);
			printNum(pos, 20, num, COLOR_G);
			pos += 8;

			break;


		case Q_CRYO:
			checkBackButton(xp, yp);
			break;


		case Q_PREPUMPPIPE:
			checkBackButton(xp, yp);

			if(checkOptionButton(xp, yp)) {
				prepumpPipe = !prepumpPipe;
				
			}
			break;


		case Q_CRYOPIPE:
			checkBackButton(xp, yp);

			if(checkOptionButton(xp, yp)) {
				cryoPipe = !cryoPipe;
			}
			break;


		default:
			break;
	}

}

void drawBackButton(uint16_t color) {
	// Draw optionbox 1
	drawRect(OPT1_X0, OPT1_Y0, OPT1_X1, OPT1_Y1, color);
	char* s2 = "Back";
	uint16_t len = strlen(s2);
	uint16_t offset = len*FONT_SX;
	printStr(((OPT1_X0+OPT1_X1)>>1) - (offset>>1), ((OPT1_Y0+OPT1_Y1)>>1) - (FONT_SY>>1), s2, len, color);

}

void checkBackButton(uint16_t xp, uint16_t yp) {
	if(xp < OPT1_X1 && xp > OPT1_X0) {
		if(yp < OPT1_Y1 && yp > OPT1_Y0) {
			clean();
			state = MAIN;
		}
	}
}

void drawOptionButton(char* text, uint16_t color) {
	drawRect(OPT2_X0, OPT2_Y0, OPT2_X1, OPT2_Y1, color);
	uint16_t len = strlen(text);
	uint16_t offset = len*FONT_SX;
	printStr(((OPT2_X0+OPT2_X1)>>1) - (offset>>1), ((OPT2_Y0+OPT2_Y1)>>1) - (FONT_SY>>1), text, len, color);
}

uint8_t checkOptionButton(uint16_t xp, uint16_t yp) {

	if(xp < OPT2_X1 && xp > OPT2_X0) {
		if(yp < OPT2_Y1 && yp > OPT2_Y0) {
			return 1;
		}
	}

	return 0;

}


// Function for drawing the numerics input screen
void drawNumericsScreen(uint16_t color) {
	uint8_t i,j;

	// Draw the buttons
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			drawRect(NUM_BTN_EDGEX + i*(NUM_BTN_W + NUM_BTN_SPC), NUM_BTN_EDGEY + j*(NUM_BTN_W + NUM_BTN_SPC), 
				NUM_BTN_EDGEX + i*(NUM_BTN_W + NUM_BTN_SPC) + NUM_BTN_W, NUM_BTN_EDGEY + j*(NUM_BTN_W + NUM_BTN_SPC) + NUM_BTN_W, color);
		}
	}

	char* s = "0";
	// Print numbers 1 - 9
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 3; j++) {
			utoa((j+1) + (i*3), s,10);
			printStr(NUM_BTN_EDGEX + j*(NUM_BTN_W + NUM_BTN_SPC) + (NUM_BTN_W>>1) - 4, NUM_BTN_EDGEY + i*(NUM_BTN_W + NUM_BTN_SPC) + (NUM_BTN_W>>1) - 4,
				s, 1, color);
		}
	}

	char* s2 = " 0 ";
	uint8_t len2 = strlen(s2);
	// Draw 0, "," , Clear and Confirm buttons
	drawRect(NUM_BTN_EDGEX + 3*NUM_BTN_W + 3*NUM_BTN_SPC, NUM_BTN_EDGEY, NUM_BTN_EDGEX + 4*NUM_BTN_W + 3*NUM_BTN_SPC, NUM_BTN_EDGEY + NUM_BTN_W, color);
	printStr((NUM_BTN_EDGEX + 3*NUM_BTN_W + 3*NUM_BTN_SPC + (NUM_BTN_W >> 1)) - 12, 
		NUM_BTN_EDGEY + (NUM_BTN_W>>1) - 4, s2, len2, color);

	s2 = ", ";
	len2 = strlen(s2);
	drawRect(NUM_BTN_EDGEX + 4*NUM_BTN_W + 4*NUM_BTN_SPC, NUM_BTN_EDGEY, XMAX - NUM_BTN_EDGEX, NUM_BTN_EDGEY + NUM_BTN_W, color);
	printStr(NUM_BTN_EDGEX + 4*NUM_BTN_W + 4*NUM_BTN_SPC + (NUM_BTN_W >> 1) - 8,
		NUM_BTN_EDGEY + (NUM_BTN_W>>1) - 4, s2, len2, color);

	char* s3 = "Clear";
	uint8_t len3 = strlen(s3);
	drawRect(NUM_BTN_EDGEX + 3*NUM_BTN_W + 3*NUM_BTN_SPC, NUM_BTN_EDGEY + NUM_BTN_W + NUM_BTN_SPC, XMAX - NUM_BTN_EDGEX, NUM_BTN_EDGEY + 2*NUM_BTN_W + NUM_BTN_SPC, color);
	printStr(((XMAX - NUM_BTN_EDGEX + NUM_BTN_EDGEX + 3*NUM_BTN_W + 3*NUM_BTN_SPC)>>1) - 20, 
		NUM_BTN_EDGEY + NUM_BTN_W + NUM_BTN_SPC + (NUM_BTN_W>>1) - 4, s3, len3, color);

	char* s4 = "Confirm";
	uint8_t len4 = strlen(s4);
	drawRect(NUM_BTN_EDGEX + 3*NUM_BTN_W + 3*NUM_BTN_SPC, NUM_BTN_EDGEY + 2*NUM_BTN_W + 2*NUM_BTN_SPC, XMAX - NUM_BTN_EDGEX, NUM_BTN_EDGEY + 3*NUM_BTN_W + 2*NUM_BTN_SPC, color);
	printStr(((XMAX - NUM_BTN_EDGEX + NUM_BTN_EDGEX + 3*NUM_BTN_W + 3*NUM_BTN_SPC)>>1) - 28, 
		NUM_BTN_EDGEY + 2*NUM_BTN_W + 2*NUM_BTN_SPC + (NUM_BTN_W>>1) - 4, s4, len4, color);


}

// Function for recognizing when a numeric button is pressed on the numerics interface
// Pressing a number between 0-9 returns that number
// Pressing the Clear button returns 10
// Pressing the Confirm button returns 11
// Pressing the "," button returns 12
uint8_t checkNumerics(uint16_t xp, uint16_t yp) {
	if(xp < (NUM_BTN_EDGEX + NUM_BTN_W + NUM_BTN_SPC + (NUM_BTN_W>>1))) {
		// Is in left half of numerics (1)
		
		if(yp < (NUM_BTN_EDGEY + NUM_BTN_W + NUM_BTN_SPC + (NUM_BTN_W>>1))) {
			// Is in upper left quadrant (2)
			
			if(xp < (NUM_BTN_EDGEX + NUM_BTN_W + (NUM_BTN_SPC>>1))) {
				// Is in left part of upper left quadrant (3)

				if(yp < (NUM_BTN_EDGEY + NUM_BTN_W + (NUM_BTN_SPC >> 1))) {
					// Pressing button 1
					return 1;
				} else {
					// Pressing upper part of button 4
					return 4;
				}

			} else {
				// Is in right part of upper left quadrant (3)

				if(yp < (NUM_BTN_EDGEY + NUM_BTN_W + (NUM_BTN_SPC >> 1))) {
					// Pressing left part of button 2
					return 2;
				} else {
					// Pressing upper left part of button 5
					return 5;
				}
			}

		} else {
			// Is in lower left quadrant (2)

			if(xp < (NUM_BTN_EDGEX + NUM_BTN_W + (NUM_BTN_SPC>>1))) {
				// Is in left part of lower left quadrant (3)

				if(yp < (NUM_BTN_EDGEY + NUM_BTN_W*2 + NUM_BTN_SPC + (NUM_BTN_SPC >> 1))) {
					// Pressing lower part of button 4
					return 4;
				} else {
					// Pressing button 7
					return 7;
				}

			} else {
				// Is in right part of lower left quadrant (3)

				if(yp < (NUM_BTN_EDGEY + NUM_BTN_W*2 + NUM_BTN_SPC + (NUM_BTN_SPC >> 1))) {
					// Pressing lower left part of button 5
					return 5;
				} else {
					// Pressing left part of button 8
					return 8;
				}

			}

		}


	} else {
		// Is in right half of numerics (1)

		if(yp < (NUM_BTN_EDGEY + NUM_BTN_W + NUM_BTN_SPC + (NUM_BTN_W>>1))) {
			// Is in upper right quadrant (2)

			if(xp < (NUM_BTN_EDGEX + NUM_BTN_W*2 + NUM_BTN_SPC + (NUM_BTN_SPC>>1))) {
				// Is in left part of upper right quadrant (3)

				if(yp < (NUM_BTN_EDGEY + NUM_BTN_W + (NUM_BTN_SPC >> 1))) {
					// Pressing right part of button 2
					return 2;
				} else {
					// Pressing upper right part of button 5 
					return 5;
				}

			} else {
				// Is in right part of upper right quadrant (3)

				if(yp < (NUM_BTN_EDGEY + NUM_BTN_W + (NUM_BTN_SPC >> 1))) {
					// Is in upper right part of upper right quadrant (4)

					if(xp < (NUM_BTN_EDGEX + NUM_BTN_W*3 + NUM_BTN_SPC*2 + (NUM_BTN_SPC>>1))) {
						// Pressing button 3 (5)
						return 3;
					} else {

						if(xp < (NUM_BTN_EDGEX + 4*NUM_BTN_W + 3*NUM_BTN_SPC + (NUM_BTN_SPC>>1))) {
							// Pressing button 0
							return 0;
						} else {
							// Pressing button ","
							return 12;
						}
					}
					
				} else {
					// Is in lower right part of upper right quadrant(4) 
					if(xp < (NUM_BTN_EDGEX + NUM_BTN_W*3 + NUM_BTN_SPC*2 + (NUM_BTN_SPC>>1))) {
						// Pressing upper part of button 6 (5)
						return 6;
					} else {
						// TODO Pressing upper part of button clear
						return 10;
					}
				}

			}

		} else {
			// Is in lower right quadrant (2)

			if(xp < (NUM_BTN_EDGEX + NUM_BTN_W*2 + NUM_BTN_SPC + (NUM_BTN_SPC>>1))) {
				// Is in left part of lower right quadrant (3)

				if(yp < (NUM_BTN_EDGEY + NUM_BTN_W*2 + NUM_BTN_SPC + (NUM_BTN_SPC >> 1))) {
					// Pressing lower right part of button 5
					return 5;
				} else {
					// Pressing right part of button 8
					return 8;
				}

			} else {
				// Is in right part of lower right quadrant (3)

				if(yp < (NUM_BTN_EDGEY + NUM_BTN_W*2 + NUM_BTN_SPC + (NUM_BTN_SPC >> 1))) {
					// Is in upper right part of lower right quadrant (4)

					if(xp < (NUM_BTN_EDGEX + NUM_BTN_W*3 + NUM_BTN_SPC*2 + (NUM_BTN_SPC>>1))) {
						// Pressing lower part of button 6
						return 6;
					} else {
						// TODO Pressing lower part of clear button
						return 10;
					}

				} else {
					// Is in lower right part of lower right quadrant (4)
					
					if(xp < (NUM_BTN_EDGEX + NUM_BTN_W*3 + NUM_BTN_SPC*2 + (NUM_BTN_SPC>>1))) {
						// Pressing button 9
						return 9;
					} else {
						// TODO Pressing confirm button
						return 11;
					}
				}

			}
		}

	}
}