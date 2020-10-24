#include "userinterface.h"
#include <stdlib.h>

const char* labels[4] = {"Signal:","Voltage:","Frequency:","Phase:"};
const char* units[3] = {" V"," Hz"," deg"};
const char* signals[4] = {"sine","cosine","square","triangle"};

extern void lcd_backlight(char on);

void uiInit(void) {
	const uint8_t initMode = 0;
	const uint8_t initModeLast = 0xFF;
	rotaryInit();
	LCD_I2C_DDR &= ~((1<<LCD_I2C_SCL) | (1<<LCD_I2C_SDA));
	LCD_I2C_PORT |= (1<<LCD_I2C_SCL) | (1<<LCD_I2C_SDA);
	lcd_init(LCD_ON_DISPLAY);
	lcd_backlight(0);
	_delay_ms(500);
	lcd_backlight(1);
	_delay_ms(500);
	displayRefresh(initMode,&initModeLast,100000,0,1,0);
}

void clearScreen(void) {
	lcd_clrscr();
	_delay_ms(10);
}



void clearLine(unsigned char y) {
	lcd_gotoxy(0,y);
	for (uint8_t i = 0; i < LCD_DISP_LENGTH; i++) lcd_puts(" ");
}

void clearSpace(unsigned char x1,unsigned char x2, unsigned char y) {
	for (uint8_t i = x1; i < x2; i++) {
		lcd_gotoxy(i,y);
		lcd_puts(" ");
	}
	lcd_gotoxy(x1,y);
}

void displayRefresh(uint8_t mode, uint8_t *modeLast, uint32_t frequency, uint16_t phase, int8_t voltage, uint8_t signal) {
	if (mode != *modeLast) {
		clearScreen();
		lcd_puts(labels[mode]);
	}
	else
		clearLine(1);
	lcd_gotoxy(0,1);
	if (mode == MODE_SIGNAL) lcd_puts(signals[signal]);
	else {
		char val[10];
		if (mode == MODE_VOLTAGE) itoa(voltage,val,10);
		else if (mode == MODE_FREQUENCY) itoa(frequency,val,10);
		else if (mode == MODE_PHASE) itoa(phase,val,10);
		lcd_puts(val);
		lcd_puts(units[mode-1]);
		val[0] = '\0';
	}
}