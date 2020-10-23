#ifndef ROTARY_H
#define ROTARY_H

#include <avr/io.h>

// Rotary encoder
#define ROTARY_DDR DDRC
#define ROTARY_PORT PORTC
#define ROTARY_PIN PINC 
#define ROTARY_A PINC0
#define ROTARY_B PINC1
#define ROTARY_BUTTON PINC2
#define NO_ROTATION 0
#define CLOCKWISE 1
#define COUNTERCLOCKWISE 2
#define SET_VAL_FLAG 1

void rotaryInit(void);
uint8_t setRotaryStatus(uint8_t aQuiescent, uint8_t rotation);
uint8_t getNextMode(uint8_t mode, uint8_t *modeLast);
uint8_t getPreviousMode(uint8_t mode, uint8_t *modeLast);

#endif