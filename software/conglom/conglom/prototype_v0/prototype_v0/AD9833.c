// ******************************************************************************************************************************** //
// *******************  SPI Communication driver for AD9833 DDS with Frame Sync,,,  ***********************************************	//
// ********************************************************************************************************************************	//
// 	Last modification:  April 2008																									//
//	Original source:	AVRwiz http://greschenz.dyndns.org/?title=AvrWiz															//
//	Reference 1: 		http://winavr.scienceprog.com/avr-gcc-tutorial/serial-peripheral-interface-spi-of-avr-microcontrollers.html	//
// 	Reference 2: 		http://www.mikroe.com/forum/viewtopic.php?t=9976															//
//	Modified by: 		Michael Grant (krazatchu at hotmail dot com)																//		
//	Compiler: 			GCC / AVR Studio 4																							//
//	Target device:		ATmega8 @ 10 MHz																							//
// 	Application: 		http://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=62560										//
// ********************************************************************************************************************************	//

#include <avr/io.h>
#include "AD9833.h"
#include "spi.h"

static volatile uint16_t controlReg = 0;

void AD9833_init (void)
{
// 	controlReg |= (1<<RESET);
// 	SPI_write16(controlReg);
// 	controlReg &= ~(1<<RESET);
// 	freqChange(100000,0); //initial frequency of 100kHz on Freq
// 	phaseChange(0,0); // initial phase of 0 deg
	// idk if i did the math here right or if it even matters;
	// this init sequence is maybe more efficient or something
	// just uncomment the above if it doesnt work
	// ... hopefully the above works
	SPI_write16(INIT_RESET);
	SPI_write16(INIT_RESET_B28);
	SPI_write16(INIT_FREQ0_LSB);
	SPI_write16(INIT_FREQ0_MSB);
	SPI_write16(INIT_PHASE0);
	SPI_write16(INIT_GO);
}


void freqChange(uint32_t freqOut, uint8_t select)  // take base10 frequency and do frequency hop
{
	if (freqOut > FREQ_MAX) freqOut = FREQ_MAX;
	else if (freqOut < FREQ_MIN) freqOut = FREQ_MIN;
	//freqReg = freq_out* 2^28/freq_mclk
	uint64_t freqReg = (freqOut * POW2_28)/MCLK; // maybe doesnt need to be 64 bits, but freq changing wasn't working last
	uint16_t regLs = (freqReg & BITS14_MASK);
	uint16_t regMs = ((freqReg>>14) & BITS14_MASK);
	controlReg |= (1<<B28) | (1<<RESET);
	
	if (select == 0) {
		regLs |= FREQ0_D_MASK;
		regMs |= FREQ0_D_MASK;
		controlReg &= ~(1<<FSELECT);
	}
	else if (select == 1) {
		regLs |= FREQ1_D_MASK;
		regMs |= FREQ1_D_MASK;
		controlReg |= (1<<FSELECT);
	}
	
	SPI_write16(controlReg);
	SPI_write16(regLs);
	SPI_write16(regMs);
	controlReg &= ~(1<<RESET);
	SPI_write16(controlReg);
	
}

void phaseChange(uint16_t phaseShift, uint8_t select) {
	uint16_t phaseReg = (phaseShift*POW2_12) / (2*PI);
	controlReg |= (1<<RESET);
	
	if (select == 0) {
		phaseReg |= PHASE0_D_MASK;
		controlReg &= ~(1<<PSELECT);
	}
	if (select == 1) {
		phaseReg |= PHASE1_D_MASK;
		controlReg |= (1<<PSELECT);
	}
	
	SPI_write16(phaseReg);
	controlReg &= ~(1<<RESET);
	
}

void sineOut(void) {
	controlReg |= (1<<RESET);
	controlReg &= ~((1<< SLEEP12) | (1<<OPBITEN) | (1<<MODE));
	SPI_write16(controlReg);
	controlReg &= ~(1<<RESET);
	SPI_write16(controlReg);
}

void cosineOut(uint8_t select) {
	sineOut();
	phaseChange(90,select);
}

void triangleOut(void) {
	controlReg |= (1<<RESET) | (1<<MODE);
	controlReg &= ~((1<< SLEEP12) | (1<<OPBITEN));
	SPI_write16(controlReg);
	controlReg &= ~(1<<RESET);
	SPI_write16(controlReg);
}

void squareOut(void) {
	controlReg |= (1<<RESET) | (1<<SLEEP12) | (1<<OPBITEN) | (1<<DIV2);
	controlReg &= ~(1<<MODE);
	SPI_write16(controlReg);
	controlReg &= ~(1<<RESET);
	SPI_write16(controlReg);
}

void setSignalOut(uint8_t sigMode) {
	switch (sigMode) {
		case SIGNAL_SIN:
			sineOut();
			break;
		case SIGNAL_COS:
			cosineOut(0);
			break;
		case SIGNAL_SQ:
			squareOut();
			break;
		case SIGNAL_TRI:
			triangleOut();
			break;
	}
}