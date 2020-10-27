#include "rotary.h"

volatile uint8_t setVal = 0;

void rotaryInit(void) {
	ROTARY_DDR &= ~((1<<ROTARY_A) | (1<<ROTARY_B) | (1<<ROTARY_BUTTON)); // encoder pins and button set as input
	ROTARY_PORT |= (1<<ROTARY_A) | (1<<ROTARY_B); // set pull-up resistors on encoder pins.
	initHeartbeatTimer();
}

// Init timer0 - int0 8bit PS=1024 , ticks = 78 , freq ~= 200Hz , period ~= 5ms
void initHeartbeatTimer() {
	TCNT0 = 78;					// set Timer0 to tick for 10ms
	TCCR0B |= (1<<CS02)|(1<<CS00);	// pre-scaler = 1024
	TIMSK0 |= (1<<TOIE0);			// overflow interrupt enable
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
	return rotation;
}

/* Interrupt Service Routine for Timer0
	Occurs every 10ms to poll button pin */
ISR (TIMER0_OVF_vect) {
		if (ROTARY_PIN & (1<<ROTARY_BUTTON)) setVal ^= SET_VAL_FLAG;
}