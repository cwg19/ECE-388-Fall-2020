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

// 12.5MHz * 2^28 = 0x B EBC2 0000 0000 = 3,355,443,200,000,000 (16 digits)
// 2^64 = 18,446,744,073,709,551,616 (20 digits)
// need 64 bit int (can shift by 3 i believe)

extern volatile uint16_t controlReg;

int main(void)
{
	SPI_init();
	AD9833_init();
	uint32_t freqOut = 0;
	uint16_t phaseOut = 0;
	while(1) {
		for (uint8_t i = 1; i < 255; i++) {
			freqOut = i*10000;
			freqChange(freqOut,0);
			_delay_ms(2000);
		}
		freqOut = 1000;
		freqChange(freqOut,0);
		for (uint8_t j = 0; j < 180; j++) {
			phaseOut = j*2;
			phaseChange(phaseOut,0);
			_delay_ms(2000);
		}
		phaseOut = 0;
		phaseChange(phaseOut,0);
	}
/*	AD9833_init();*/
// 	SPI_write16(0b0010000101101000);
// 	SPI_write16(0b0101100010010011);
// 	SPI_write16(0b0100000000010000);
// 	SPI_write16(0b0000000001101000);
}

