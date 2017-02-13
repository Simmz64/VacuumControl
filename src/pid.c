#include "pid.h"

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