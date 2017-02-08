#include "gui.h"


void drawScreen(void) {
	switch(state) {
		case MAIN:
			drawQuestionScreen(COLOR_BG);
			drawMainScreen(COLOR_FG);
			break;
		case QUESTION:
			drawMainScreen(COLOR_BG);
			drawQuestionScreen(COLOR_FG);
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

}


void drawQuestionScreen(uint16_t color) {
	char* s = "This is the question screen";
	printStr(60, 120, s, strlen(s), color);
}


// Checks whether a button is pressed at xp at yp
void buttons(uint16_t xp, uint16_t yp) {
	//Check chamber
	switch(state) {
		case MAIN:
			if(xp < (CHAMBER_X + CHAMBER_W) && xp > (CHAMBER_X - CHAMBER_W)) {
				if(yp < (CHAMBER_Y + CHAMBER_H) && yp > (CHAMBER_Y - CHAMBER_H)) {
					//printStr(20, 20, "Chamber pressed", 15, COLOR_B);
					state = QUESTION;
				}
			}
			break;
		
		case QUESTION:
			state = MAIN;
			break;
		default:
			break;
	}

}