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

// AD9833 Control Register
#define B28 13
#define HLB 12
#define FSELECT 11
#define PSELECT 10
#define RESET 8
#define SLEEP1 7
#define SLEEP12 6 // potentially useful for PWM
#define OPBITEN 5
#define DIV2 3 // when DIV2 == 1 then MSB of DAC is passed to vout // when DIV2 = 0 then MSB/2 of DAC is passed to vout
#define MODE 1

// Bit masks for addressing frequency and phase registers
#define BITS14_MASK 0x3FFF
#define FREQ0_D_MASK 0x4000
#define FREQ1_D_MASK 0x8000
#define PHASE0_D_MASK 0xC000
#define PHASE1_D_MASK 0xE000

// init sequence
#define INIT_RESET 0x0100
#define INIT_RESET_B28 0x2100
#define INIT_FREQ0_LSB 0x624D
#define INIT_FREQ0_MSB 0x4041
#define INIT_PHASE0 0xC000
#define INIT_GO 0x0000
#define INIT_FREQ 100000

// for calculating value in FREQN reg and PHASEN reg
#define POW2_28 0x10000000
#define POW2_12 0x1000
#define MCLK 25000000
#define PI 3.14159265358979323846 /* pi */ //precision copied from <math.h>

// Maximums and minimums frequency and phase
#define FREQ_MAX 12500000
#define FREQ_MIN 1
#define PHASE_MAX 360
#define PHASE_MIN 0

#define SIGNAL_SIN 0
#define SIGNAL_COS 1
#define SIGNAL_SQ 2
#define SIGNAL_TRI 3

// functions to interact with AD9833
void AD9833_init (void);
void freqChange(uint32_t freqOut, uint8_t select);
void phaseChange(uint16_t phaseShift, uint8_t select);
void sineOut(void);
void cosineOut(uint8_t select);
void triangleOut(void);
void squareOut(void);
void setSignalOut(uint8_t sigMode);

#endif // AD9833_H
