/*
 * ui_modeSwitch.c
 *
 * Created: 10/7/2020 6:06:28 PM
 * Author : Christopher
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
#include "lcd.h"

extern void lcd_backlight(char on); 
uint8_t getRotaryStatus(void);

/* modes
	1. signal mode display 
	2. voltage display
	3. frequency display
	4. phase display
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

#define MODE_SIGNAL 0
#define MODE_VOLTAGE 1
#define MODE_FREQUENCY 2
#define MODE_PHASE 3

uint8_t mode = 0;
uint8_t count = 0;



int main(void)
{
	ROTARY_DDR &= ~((1<<ROTARY_A) | (1<<ROTARY_B) | (1<<ROTARY_BUTTON));
	ROTARY_PORT |= (1<<ROTARY_A) | (1<<ROTARY_B) | (1<<ROTARY_BUTTON);
	uint8_t aLastState = ROTARY_PIN & (1<<ROTARY_A);
	uint8_t aState = 0;
    while (1) 
    {
		aState = ROTARY_PIN & (1<<ROTARY_A);
		if (aState != aLastState) {
			if ((ROTARY_PIN & (1<<ROTARY_B)) != aState) {
				count++;
			}
			else {
				count--;
			}
		}

// 		switch(mode) {
// 			case MODE_SIGNAL:
// 				break;
// 			case MODE_VOLTAGE:
// 				break;
// 			case MODE_FREQUENCY:
// 				break;
// 			case MODE_PHASE:
// 				break;
// 			case default:
// 				break;
     }
}


// rotary encoder 
uint8_t nextMode(uint8_t currentMode) {
	uint8_t next;
	return next = (currentMode+1)&3;
}

uint8_t previousMode(uint8_t currentMode) {
	uint8_t prev;
	return prev = (currentMode-1)&3;
}

uint8_t getRotaryStatus(void) {
	uint8_t status = 0;
	if (!(ROTARY_PIN & (1<<ROTARY_A))) {
		while(ROTARY_PIN&(1<<ROTARY_B));
		status = 1;
	}
	if (!(ROTARY_PIN & (1<<ROTARY_B))) {
		while(ROTARY_PIN&(1<<ROTARY_A));
		status = 2;
	}
	return status;
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
	clearScreen();
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

