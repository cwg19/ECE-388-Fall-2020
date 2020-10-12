#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "rotary.h"

// I2C
#define LCD_I2C_DDR DDRE			// DDRC or DDRE
#define LCD_I2C_PORT PORTE			// PORTC or PORTE
#define LCD_I2C_PIN PINE			// PINC or PINE
#define LCD_I2C_SDA PINE0			// PINC4 or PINE0
#define LCD_I2C_SCL PINE1			// PINC5 or PINE1

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

void uiInit(void);
void clearScreen(void);
void clearLine(unsigned char y);
void clearSpace(unsigned char x1,unsigned char x2, unsigned char y);
void displayRefresh(void);

#endif