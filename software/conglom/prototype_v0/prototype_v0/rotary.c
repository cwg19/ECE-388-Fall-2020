#include "rotary.h"

volatile uint8_t mode = 0;
volatile uint8_t modeLast = 0xFF;
volatile uint8_t setVal = 0;
volatile uint8_t aQuiescent = 0;
volatile uint8_t aState = 0;
volatile uint8_t rotation = 0;

void rotaryInit(void) {
	ROTARY_DDR &= ~((1<<ROTARY_A) | (1<<ROTARY_B) | (1<<ROTARY_BUTTON)); // encoder pins and button set as input
	ROTARY_PORT |= (1<<ROTARY_A) | (1<<ROTARY_B); // set pull-up resistors on encoder pins.
}

void getNextMode(void) {
	modeLast = mode;
	mode = (mode+1)&3;
}

void getPreviousMode(void) {
	modeLast = mode;
	mode = (mode-1)&3;
}

void setRotaryStatus(void) {
	aState = ROTARY_PIN & (1<<ROTARY_A);
	rotation = NO_ROTATION;
	if (aState != aQuiescent)
		rotation = (ROTARY_PIN & (1<<ROTARY_B)) != aState ? COUNTERCLOCKWISE : CLOCKWISE;
	if (ROTARY_PIN & (1<<ROTARY_BUTTON))
		setVal ^= SET_VAL_FLAG;
	
}