#ifndef AD9833_H
#define AD9833_H

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

#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_PIN PINB
#define SPI_CS PINB2
#define SPI_MOSI PINB3
#define SPI_MISO PINB4
#define SPI_SCK PINB5

#define B28 13
#define HLB 12
#define FSELECT 11
#define PSELECT 10
#define RESET 8
#define SLEEP1 7
#define SLEEP12 6 // potentially useful for PWM
#define OPBITEN 5
#define DIV2 3
#define MODE 1
#define FREQ0_D_MASK 0x4000
#define FREQ1_D_MASK 0x8000
#define PHASE0_D_MASK 0xC000
#define PHASE1_D_MASK 0xE000

#define RESET_CMD 0x2100
#define SINE_CMD 0x2000
#define TRI_CMD 0x2002
#define SQ1_CMD 0x2020
#define SQ2_CMD 0x2028

#define PI 3.14159265358979323846 /* pi */ //precision copied from <math.h>

/*
#define F_MCLCK 25000000UL
#define F_BITS 268435456UL
*/


/*************************************************************************
Function: SPI_init()
Purpose:  initialize the SPI bus 
Input:    none
Returns:  none
**************************************************************************/
void SPI_init (void);


/*************************************************************************
Function: SPI_write16()
Purpose:  send a 16bit word to the AD9833 
Input:    unsigned short data = 16bits
Returns:  none
Comment:  uses 8bit filter and two consecutive writes while fsync stays low
**************************************************************************/
void SPI_write16 (uint16_t data);


/*************************************************************************
Function: Freq_change()
Purpose:  change the frequency and select AD9833 onboard register
Input:    unsigned short freq_out = frequency, unsigned int select = register 0 or 1
Returns:  none
Comment:  uses 14 bit filter and adds control words, 
**************************************************************************/
void Freq_change ( uint32_t freq_out, uint8_t select );

void phaseChange(int16_t phaseShift, uint8_t select);

/*************************************************************************
Function: AD9833_init()
Purpose:  Init the AD9833
Input:    none
Returns:  none
Comment:  this function isn't nessecary, can be done manually
**************************************************************************/
void AD9833_init (void);

#endif // UART_H
