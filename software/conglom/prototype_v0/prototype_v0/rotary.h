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
void setRotaryStatus(void);
void getNextMode(void);
void getPreviousMode(void);

#endif