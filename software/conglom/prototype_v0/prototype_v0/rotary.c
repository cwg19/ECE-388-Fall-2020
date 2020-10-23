#include "rotary.h"

volatile uint8_t setVal = 0;

void rotaryInit(void) {
	ROTARY_DDR &= ~((1<<ROTARY_A) | (1<<ROTARY_B) | (1<<ROTARY_BUTTON)); // encoder pins and button set as input
	ROTARY_PORT |= (1<<ROTARY_A) | (1<<ROTARY_B); // set pull-up resistors on encoder pins.
}

uint8_t getNextMode(uint8_t mode, uint8_t *modeLast) {
	*modeLast = mode;
	return mode = (mode+1)&3;
}

uint8_t getPreviousMode(uint8_t mode, uint8_t *modeLast) {
	*modeLast = mode;
	return mode = (mode-1)&3;
}

uint8_t setRotaryStatus(uint8_t aQuiescent, uint8_t rotation) {
	uint8_t aState = ROTARY_PIN & (1<<ROTARY_A);
	rotation = NO_ROTATION;
	if (aState != aQuiescent) 
		rotation = (ROTARY_PIN & (1<<ROTARY_B)) != aState ? COUNTERCLOCKWISE : CLOCKWISE;
// 	if (ROTARY_PIN & (1<<ROTARY_BUTTON))
// 		setVal ^= SET_VAL_FLAG; //make ISR to check button
	return rotation;
}