#ifndef SPI
#define SPI

#if defined (__AVR_ATmega32A__)
	#define CLK		7
	#define MISO	6
	#define MOSI	5
	#define CS		4
#elif defined (__AVR_ATmega328P__)
	#define CLK		5
	#define MISO	4
	#define MOSI	3
	#define CS		2
#endif

uint8_t spi_tranceiver (uint8_t);

uint8_t spi_tranceiver (uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF) ));
	return(SPDR);
}

#endif