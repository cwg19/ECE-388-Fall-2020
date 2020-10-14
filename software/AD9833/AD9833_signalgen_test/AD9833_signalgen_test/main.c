/*
 * AD9833_signalgen_test.c
 *
 * Created: 10/12/2020 11:21:53 PM
 * Author : Christopher
 */ 

#include <avr/io.h>
#include "AD9833.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#define NUMERATOR 0x10000000
#define DENOM 25000000

uint16_t controlReg = 0;
uint32_t freqOut = 0;
uint32_t freqReg = 0;
uint16_t fMsb = 0;
uint16_t fLsb = 0;

int main(void)
{
	SPI_init();
	controlReg |= (1<<B28) | (1<<RESET) | (1<<SLEEP12) | (1<<OPBITEN) | (1<<DIV2);
	freqOut = 500;
	freqReg = (freqOut * NUMERATOR)/ DENOM;
	 //10.73741824;
	fMsb = ((freqReg>>14) & 0x3FFF) | FREQ0_D_MASK;
	fLsb = (freqReg & 0x3FFF) | FREQ0_D_MASK;
	SPI_write16(controlReg);
	SPI_write16(fLsb);
	SPI_write16(fMsb);
	SPI_write16(controlReg&(~(1<<RESET)));
	
	
	
/*	AD9833_init();*/
// 	SPI_write16(0b0010000101101000);
// 	SPI_write16(0b0101100010010011);
// 	SPI_write16(0b0100000000010000);
// 	SPI_write16(0b0000000001101000);
}

