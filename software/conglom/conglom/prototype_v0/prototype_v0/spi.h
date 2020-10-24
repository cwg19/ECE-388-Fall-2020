#include <avr/io.h>

// SPI I/O 
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define SPI_PIN PINB
#define SPI_CS PINB2
#define SPI_MOSI PINB3
#define SPI_MISO PINB4
#define SPI_SCK PINB5

void SPI_init (void);
void SPI_write16 (uint16_t data);