/*
 * prototype_v0.c
 *
 * Created: 10/21/2020 12:02:42 PM
 * Author : Christopher
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "userinterface.h" 
#include "AD9833.h"

/* modes
	1. signal mode display 
	2. voltage display
	3. frequency display
	4. phase display
	In each mode display the mode and the appro
*/

// extern volatile uint8_t mode;
// extern volatile uint8_t modeLast;
extern volatile uint8_t setVal;
/*extern volatile uint8_t aQuiescent;*/
/*extern volatile uint8_t aState;*/
/*extern volatile uint8_t rotation;*/

// extern volatile const char* labels[4];
// extern volatile const char* units[3];
// extern volatile const char* signals[4];

// extern volatile uint32_t frequency;
// extern volatile uint16_t phase;
// extern volatile int8_t voltage;
// extern volatile uint8_t signal;


int main(void) {
	uint32_t frequency = INIT_FREQ;
	uint16_t phase = 0;
	int8_t voltage = 1; // probably won't be able to display voltage
	uint8_t signal = SIGNAL_SIN;
	
	uint8_t mode = MODE_SIGNAL;
	uint8_t modeLast = 0xFF;
	const uint8_t aQuiescent = ROTARY_PIN & (1<<ROTARY_A);
	uint8_t rotation = NO_ROTATION;
	
	
	uint8_t signalLast = signal;
	uint32_t freqLast = frequency;
	uint32_t fOut = frequency;
	uint16_t phaseLast = phase;
	uiInit();
	SPI_init();
	AD9833_init();
    while (1) {
		rotation = setRotaryStatus(aQuiescent, rotation);
		if (setVal) {
			switch(mode) {
				case MODE_SIGNAL:
					if (rotation == CLOCKWISE)
						signal = (signal+1)&3;
					else if (rotation == COUNTERCLOCKWISE)
						signal = (signal-1)&3;
					break;
				case MODE_VOLTAGE:
					if ((rotation == CLOCKWISE) && (voltage < V_MAX)) {
						voltage++;
					}
					else if ((rotation == COUNTERCLOCKWISE) && (voltage > V_MIN)) {
						voltage--;
					}
					break;
				case MODE_FREQUENCY:
					if ((rotation == CLOCKWISE) && (frequency < FREQ_MAX))
						frequency+=1000;
					else if ((rotation == COUNTERCLOCKWISE) && (frequency > FREQ_MIN))
						frequency-=1000;
					fOut = frequency;
					break;
				case MODE_PHASE:
					if ((rotation == CLOCKWISE) && (phase < PHASE_MAX))
						phase++;
					else if ((rotation == COUNTERCLOCKWISE) && (phase > PHASE_MIN))
						phase--;
					break;
			}
		}
		else {
			if (rotation == CLOCKWISE)
				rotation = getNextMode(mode, &modeLast);
			else if (rotation == COUNTERCLOCKWISE)
				rotation = getPreviousMode(mode, &modeLast);
		}
		// display mode and value with units
		if (rotation != NO_ROTATION) {
			displayRefresh(mode, &modeLast);
			if (signal != signalLast)
				setSignalOut();
			if (frequency != freqLast)
				freqChange(fOut,0);
			if (phase != phaseLast)
				phaseChange(phase,0);	
		}
		signalLast = signal;
		freqLast = frequency;
		phaseLast = phase;
		_delay_ms(10);
	}
}