#ifndef SPI_H
#define SPI_H

#include "spi.h"

void SPI_init (void)
{
	SPI_DDR |= (1<<SPI_CS) | (1<<SPI_MOSI) | (1<<SPI_SCK); // set SCK,MOSI,CS as Fsync 
	SPI_PORT |= (1<<SPI_CS) | (1<<SPI_SCK); // SCK and CS high
	SPCR0 |= (1<<SPE) | (1<<MSTR) | (1<<CPOL); // Enable SPI // Set Master mode //	Set clk to inv.
}


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

#endif