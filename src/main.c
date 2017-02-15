/*
ATmega8, 48, 88, 168, 328

Reset    PC6|1   28|PC5
RxD      PD0|2   27|PC4
TxD      PD1|3   26|PC3       Y-
         PD2|4   25|PC2       X-
         PD3|5   24|PC1       Y+
         PD4|6   23|PC0       X+
         Vcc|7   22|Gnd
         Gnd|8   21|Aref
         PB6|9   20|AVcc
         PB7|10  19|PB5       SCK
         PD5|11  18|PB4       MISO
         PD6|12  17|PB3       MOSI
   RST   PD7|13  16|PB2       DC
   LED   PB0|14  15|PB1       CS
*/

#include "main.h"

volatile uint16_t boxCount[8] = {0, 0, 0, 0, 0, 0, 0, 0};
volatile enum states state = MAIN;
volatile uint8_t prepumpPipe = 0;
volatile uint8_t cryoPipe = 0;
volatile int16_t intHolder = 0;
volatile uint16_t decHolder = 0;


void init(void) {

   // Set MOSI, SCK, PB1, PB2 as output
   DDRB = (1<<PB5)|(1<<PB3) | (1<<PB1)|(1<<PB2)|(1<<PB0);
   DDRD = (1<<PD7);
   // Initialize SPI command register
   SPCR = (0<<SPIE)|(1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<CPOL)|(0<<SPR1)|(0<<SPR0);
   SPSR = (1<<SPI2X);

   // Initialize ADC
   ADCSRA = (1 << ADEN)|(0 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
   // Enable global interrupts
   sei();

}

void blinkLED(void) {
   PORTB |= (1 << PB0);
   _delay_ms(100);
   PORTB &= ~(1 << PB0);
   _delay_ms(100);
}

void testTouch(void) {
   uint16_t xpos = 0, ypos = 0, z = 0;


   while(1) {
      // Flashes LED and induces delay in loop
      blinkLED();

      drawCross(xpos, ypos, COLOR_BG);

      z = readZ();
      
      if(z > 500) {
         xpos = readX();
         ypos = readY();
      } else {
         xpos = 0;
         ypos = 0;
      }


      drawCross(xpos, ypos, COLOR_BG);
      drawTestUI();
      checkBoxBounds(xpos-512, ypos-512);
      printToBoxes();
      if(xpos > 0) {
         printNum(224, 24, xpos-512, COLOR_FG);
      }

      if(ypos > 0) {
         printNum(264, 24, ypos-512, COLOR_FG);   
      }

      if(boxCount[0] > boxCount[1]) {
         printStr(90, 24, "Test string", 11, COLOR_FG);
      } else {
         printStr(90, 24, "           ", 11, COLOR_FG);
      }
      
      printNum(24, 24, z, 0x0000);
   }
}

void testGUI(void) {
   uint16_t xpos = 0, ypos = 0, z = 0;
   uint8_t redraw = 1, pressed = 0;
   uint8_t i = 0, j = 0;



   while(1) {
      
      if(redraw) {
         drawScreen();
         redraw = 0;
      }

      z = readZ();

      if(z > 500 && !pressed) {
         xpos = readX();
         ypos = readY();
         redraw = 1;
         pressed = 1;
         buttons(xpos - 512, ypos - 512);
      } else {
         xpos = 0;
         ypos = 0;
      }

      if(z < 200) {
         pressed = 0;
      }
/*
      pidWrite(i);
      j++;
      if(j == 12) {
         j = 0;
         i++;
         i = i % 256;
      }
*/
      _delay_ms(1);
   }
}

int main (void)
{

	init();
   tft_reset();

   cls(COLOR_BG);

   testGUI();
   //testTouch();

}
