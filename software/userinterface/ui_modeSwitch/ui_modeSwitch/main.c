/*
 * ui_modeSwitch.c
 *
 * Created: 10/7/2020 6:06:28 PM
 * Author : Christopher
 */ 

#include <avr/io.h>
#include <stdlib.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "lcd.h"

extern void lcd_backlight(char on); 
void setRotaryStatus(void);
void getNextMode(void);
void getPreviousMode(void);
void clearScreen(void);
void uiInit(void);
void clearLine(unsigned char y);
void clearSpace(unsigned char x1,unsigned char x2, unsigned char y);
void displayRefresh(void);


/* modes
	1. signal mode display 
	2. voltage display
	3. frequency display
	4. phase display
	In each mode display the mode and the appro
*/

// I2C
#define LCD_I2C_DDR DDRC			// DDRC or DDRE
#define LCD_I2C_PORT PORTC			// PORTC or PORTE
#define LCD_I2C_PIN PINC			// PINC or PINE
#define LCD_I2C_SDA PINC4			// PINC4 or PINE0
#define LCD_I2C_SCL PINC5			// PINC5 or PINE1

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

#define FREQ_MAX 1000000UL
#define FREQ_MIN 1

#define PHASE_MAX 180
#define PHASE_MIN  -180

#define V_MAX 20
#define V_MIN -20

#define MODE_SIGNAL 0
#define MODE_VOLTAGE 1
#define MODE_FREQUENCY 2
#define MODE_PHASE 3

#define SIGNAL_SIN 0
#define SIGNAL_COS 1
#define SIGNAL_SQ 2
#define SIGNAL_TRI 3

uint8_t mode = 0;
uint8_t modeLast = 0xFF;
uint8_t setVal = 0;
uint8_t aQuiescent = 0;
uint8_t aState = 0;
uint8_t rotation = 0;

const char* labels[4] = {"Signal:","Voltage:","Frequency:","Phase:"};
const char* units[3] = {" V"," Hz"," deg"};
const char* signals[4] = {"sine","cosine","square","triangle"};

uint32_t frequency = 0;
int16_t phase = 0;
int8_t voltage = 0;
uint8_t signal = 0;



int main(void) {
	ROTARY_DDR &= ~((1<<ROTARY_A) | (1<<ROTARY_B) | (1<<ROTARY_BUTTON)); // encoder pins and button set as input
	ROTARY_PORT |= (1<<ROTARY_A) | (1<<ROTARY_B); // set pull-up resistors on encoder pins.
	aQuiescent = ROTARY_PIN & (1<<ROTARY_A);
	uiInit();
    while (1) {
		setRotaryStatus();
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
						frequency++;
					else if ((rotation == COUNTERCLOCKWISE) && (frequency > FREQ_MIN))
						frequency--;
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
				getNextMode();
			else if (rotation == COUNTERCLOCKWISE)
				getPreviousMode();
		}
		// display mode and value with units
		if (rotation != NO_ROTATION) {
			displayRefresh();
			_delay_ms(50);
		}
	}
}


// rotary encoder 
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

// lcd

void clearScreen(void) {
	lcd_clrscr();
	_delay_ms(10);
}

void uiInit(void) {
	lcd_init(LCD_ON_DISPLAY);
	lcd_backlight(0);
	_delay_ms(500);
	lcd_backlight(1);
	_delay_ms(500);
	//clearScreen();
	displayRefresh();
}

void clearLine(unsigned char y) {
	lcd_gotoxy(0,y);
	lcd_puts("                    ");
}

void clearSpace(unsigned char x1,unsigned char x2, unsigned char y) {
	for (uint8_t i = x1; i < x2; i++) {
		lcd_gotoxy(i,y);
		lcd_puts(" ");
	}
	lcd_gotoxy(x1,y);
}

void displayRefresh(void) {
	char val[10];
	if (mode != modeLast) {
		clearScreen();
		lcd_puts(labels[mode]);
	}
	else
		clearLine(1);
	lcd_gotoxy(0,1);
	if (mode == MODE_SIGNAL)
		lcd_puts(signals[signal]);
	else {
		if (mode == MODE_VOLTAGE)
			itoa(voltage,val,10);
		else if (mode == MODE_FREQUENCY)
			itoa(frequency,val,10);
		else if (mode == MODE_PHASE)
			itoa(phase,val,10);
		lcd_puts(val);
		lcd_puts(units[mode-1]);
	}
}