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


/*************************************************************************
Function: SPI_init()
Purpose:  initialize the SPI bus 
Input:    none
Returns:  none
**************************************************************************/
void SPI_init (void)
{
	DDRB |= (1<<SPI_CS) | (1<<SPI_MOSI) | (SPI_SCK); // set SCK,MOSI,CS as Fsync 
	PORTB |= (1<<SPI_CS) | (1<<SPI_SCK); // SCK and CS high
	SPCR0 |= (1<<SPE) | (1<<MSTR) | (1<<CPOL); // Enable SPI // Set Master mode //	Set clk to inv.
}


/*************************************************************************
Function: SPI_write16()
Purpose:  send a 16bit word to the AD9833 
Input:    unsigned short data = 16bits
Returns:  none
Comment:  uses 8bit filter and two consecutive writes while fsync stays low
**************************************************************************/
void SPI_write16 (uint16_t data)    	// 	send a 16bit word and use fsync
{

	unsigned char MSdata = ((data>>8) & 0x00FF);  	//filter out MS
	unsigned char LSdata = (data & 0x00FF);			//filter out LS

	SPI_PORT &= ~(1<<SPI_CS);				// 	Fsync Low --> begin frame
	
	SPDR0 = MSdata;							// 	send First 8 MS of data
	while (!(SPSR0 & (1<<SPIF)));			//	while busy

	SPDR0 = LSdata;							// 	send Last 8 LS of data
	while (!(SPSR0 & (1<<SPIF)));			//	while busy

	SPI_PORT |= (1<<SPI_CS);				// 	Fsync High --> End of frame
}


/*************************************************************************
Function: Freq_change()
Purpose:  change the frequency and select AD9833 onboard register
Input:    unsigned short freq_out = frequency, unsigned int select = register 0 or 1
Returns:  none
Comment:  uses 14 bit filter and adds control words, 
**************************************************************************/
void Freq_change ( uint32_t freq_out, uint8_t select )  // take base10 frequency and do frequency hop
{
	//freq_reg = freq_out* 2^28/freq_mclk
	uint32_t freq_reg = freq_out * 10.73741824; 		// make freq register from frequency // set for 25 MHz Mclk
	uint16_t MS_reg = ((freq_reg>>14) & 0x3FFF);		// filter out MS -- make 2 x 14 bit frequency words
	uint16_t LS_reg = (freq_reg & 0x3FFF);				// filter out LS -- make 2 x 14 bit frequency words
	uint16_t commandWord = 0;

	if (select == 0 ) { 
		MS_reg |= FREQ0_D_MASK;			// control bits for data words
		MS_reg |= FREQ0_D_MASK;			// control bits for data words
		commandWord |= (1<<B28);
		commandWord &= ~(1<<FSELECT); // fselect is 0 for FREQ0 reg, but just so i remember
		SPI_write16(commandWord);
		}												 
	else if (select == 1 ) {
		MS_reg |= FREQ1_D_MASK;			// control bits for data word
		LS_reg |= FREQ1_D_MASK;			// control bits for data word
		commandWord |= (1<<B28) | (1<<FSELECT);
		SPI_write16(commandWord);
	}			

	SPI_write16(LS_reg);								// send the LS word first, to the ad9833
	SPI_write16(MS_reg);								// send the MS word last,  to the ad9833
}

void phaseChange(int16_t phaseShift, uint8_t select) {
	uint16_t phaseReg = (phaseShift*4096) / (2*PI);
	uint16_t commandWord = 0;
	
	if (select == 0) {
		phaseReg |= PHASE0_D_MASK;
		commandWord |= (1<<B28);
		commandWord &= ~(1<<PSELECT);
		SPI_write16(commandWord);
	}
	else if (select == 1) {
		phaseReg |= PHASE1_D_MASK;
		commandWord |= (1<<B28) | (1<<PSELECT);
		SPI_write16(commandWord);
	}
	
	SPI_write16(phaseReg);
	
}


/*************************************************************************
Function: AD9833_init()
Purpose:  Init the AD9833
Input:    none
Returns:  none
Comment:  this function isn't nessecary, can be done manually
**************************************************************************/
void AD9833_init (void)
{

SPI_write16(RESET_CMD);		// control word, set output to mid value voltage 


Freq_change(29000,0);
// SPI_write16(0x7288);		// Freq0 registerdata MSB  = approx. 29 khz
// SPI_write16(0x4017);		// Freq0 registerdata LSB  = approx. 29 khz

Freq_change(24000,1);
// SPI_write16(0xACEA);		// Freq1 registerdata MSB  = approx. 24 khz
// SPI_write16(0x8013); 		// Freq1 registerdata LSB  = approx. 24 khz

phaseChange(0,0);		// Phase offset of Freq0 = 0
phaseChange(0,1);		// Phase offset of Freq1 = 0

SPI_write16(SINE_CMD);	// control word, set output = sine

}


