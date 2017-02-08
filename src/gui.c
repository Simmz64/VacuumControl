#include "gui.h"


void drawScreen(void) {
	switch(state) {
		case MAIN:
			drawMainScreen(COLOR_FG);
			break;
		case Q_CHAMBER:
			drawQChamberScreen(COLOR_FG);
			break;
		case Q_PREPUMP:
			drawQPrepumpScreen(COLOR_FG);
			break;
		case Q_CRYO:
			drawQCryoScreen(COLOR_FG);
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
	printStr(PREPUMP_X - 3.5*8, PREPUMP_Y - 4, "Prepump", 7, color);

	// Draw Cryopump rectangle
	drawRect(CRYO_X - CRYO_W, CRYO_Y - CRYO_H, CRYO_X + CRYO_W, CRYO_Y + CRYO_H, color);
	printStr(CRYO_X - 4*8, CRYO_Y - 4, "Cryopump", 8, color);

	// Draw Chamber to Prepump pipe
	drawRect(CHAMBER_X - PIPE_W, PREPUMP_Y - PIPE_W, CHAMBER_X + PIPE_W, CHAMBER_Y - CHAMBER_H - CHAMBER_RIM, color);
	drawRect(CHAMBER_X - PIPE_W, PREPUMP_Y - PIPE_W, PREPUMP_X - PREPUMP_W, PREPUMP_Y + PIPE_W, color);

	// Draw Chamber to Cryopump pipe
	drawRect(CHAMBER_X - PIPE_W, CRYO_Y + PIPE_W, CHAMBER_X + PIPE_W, CHAMBER_Y + CHAMBER_H + CHAMBER_RIM, color);
	drawRect(CHAMBER_X - PIPE_W, CRYO_Y - PIPE_W, CRYO_X - CRYO_W, CRYO_Y + PIPE_W, color);

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
	drawRect(OPT1_X0, OPT1_Y0, OPT1_X1, OPT1_Y1, color);
	char* s2 = "Back";
	uint16_t len = strlen(s2);
	uint16_t offset = len*FONT_SX;
	printStr(((OPT1_X0+OPT1_X1)>>1) - (offset>>1), ((OPT1_Y0+OPT1_Y1)>>1) - (FONT_SY>>1), s2, len, color);

	drawRect(OPT2_X0, OPT2_Y0, OPT2_X1, OPT2_Y1, color);
	s2 = "Disco";
	len = strlen(s2);
	offset = len*FONT_SX;
	printStr(((OPT2_X0+OPT2_X1)>>1) - (offset>>1), ((OPT2_Y0+OPT2_Y1)>>1) - (FONT_SY>>1), s2, len, color); 

}

void drawQPrepumpScreen(uint16_t color) {
	drawRect(20, 20, 300, 220, color);
	char* s = "This is the Prepump screen";
	printStr(40, 120, s, strlen(s), color);
}

void drawQCryoScreen(uint16_t color) {
	drawRect(20, 20, 300, 220, color);
	char* s = "This is the Cryopump screen";
	printStr(40, 120, s, strlen(s), color);
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
		default:
			break;

	}
}


// Checks whether a button is pressed at xp at yp
void buttons(uint16_t xp, uint16_t yp) {
	//Check chamber
	switch(state) {
		case MAIN:
			if(xp < (CHAMBER_X + CHAMBER_W) && xp > (CHAMBER_X - CHAMBER_W)) {
				if(yp < (CHAMBER_Y + CHAMBER_H) && yp > (CHAMBER_Y - CHAMBER_H)) {
					clean();
					state = Q_CHAMBER;
				}
			}

			if(xp < (PREPUMP_X + PREPUMP_W) && xp > (PREPUMP_X - PREPUMP_W)) {
				if(yp < (PREPUMP_Y + PREPUMP_H) && yp > (PREPUMP_Y - PREPUMP_H)) {
					clean();
					state = Q_PREPUMP;
				}
			}

			if(xp < (CRYO_X + CRYO_W) && xp > (CRYO_X - CRYO_W)) {
				if(yp < (CRYO_Y + CRYO_H) && yp > (CRYO_Y - CRYO_H)) {
					clean();
					state = Q_CRYO;
				}
			}
			break;
		
		case Q_CHAMBER:
			if(xp < OPT1_X1 && xp > OPT1_X0) {
				if(yp < OPT1_Y1 && yp > OPT1_Y0) {
					clean();
					state = MAIN;
				}
			}

			if(xp < OPT2_X1 && xp > OPT2_X0) {
				if(yp < OPT2_Y1 && yp > OPT2_Y0) {
					clean();
					drawQChamberScreen(COLOR_B);
				}
			}

			break;
		case Q_PREPUMP:
			clean();
			state = MAIN;
			break;
		case Q_CRYO:
			clean();
			state = MAIN;
			break;
		default:
			break;
	}

}