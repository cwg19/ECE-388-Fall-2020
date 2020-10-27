#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H


#include <avr/io.h>
#define F_CPU 16000000UL

#include <util/delay.h>
#include "lcd.h"
#include "rotary.h"
// I2C
#define LCD_I2C_DDR DDRE			// DDRC or DDRE
#define LCD_I2C_PORT PORTE			// PORTC or PORTE
#define LCD_I2C_PIN PINE			// PINC or PINE
#define LCD_I2C_SDA PINE0			// PINC4 or PINE0
#define LCD_I2C_SCL PINE1			// PINC5 or PINE1

#define V_MAX 20
#define V_MIN -20

#define MODE_SIGNAL 0
#define MODE_VOLTAGE 1
#define MODE_FREQUENCY 2
#define MODE_PHASE 3

void uiInit(void);
void clearScreen(void);
void clearLine(unsigned char y);
void clearSpace(unsigned char x1,unsigned char x2, unsigned char y);
void displayRefresh(uint8_t mode, uint8_t *modeLast, uint32_t frequency, uint16_t phase, int8_t voltage, uint8_t signal);

#endif