#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
//#include <mylibs/ATMEGA32A/I2C.h>
//#include <mylibs/ATMEGA32A/LCD-I2C.h>
//#include <mylibs/ATMEGA32A/PCF8563.h>
//#include <mylibs/ATMEGA32A/KEYPAD.h>
//#include <mylibs/ATMEGA32A/PN532_spi.h>
#include <mylibs/ATMEGA32A/uart.h>
#include <mylibs/ATMEGA32A/SPI.h>

#define CE	3

// SPI Commands

#define R_REGISTER          0x00 // 000A AAAA
#define W_REGISTER          0x20 // 001A AAAA
#define R_RX_PAYLOAD        0x61 // 0110 0001
#define W_TX_PAYLOAD        0xA0 // 1010 0000
#define FLUSH_TX            0xE1 // 1110 0001
#define FLUSH_RX            0xE2 // 1110 0010
#define REUSE_TX_PL         0xE3 // 1110 0011
#define R_RX_PL_WID         0x60 // 0110 0000
#define W_ACK_PAYLOAD       0xA8 // 1010 1PPP
#define W_TX_PAYLOAD_NOACK  0xB0 // 1011 0000
#define NOP                 0xFF // 1111 1111


// Register Map

#define CONFIG      0x00
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0

#define EN_AA       0x01
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0

#define EN_RXADDR   0x02
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0

#define SETUP_AW    0x03
#define AW          0

#define SETUP_RETR  0x04
#define ARD         4
#define ARC         0

#define RF_CH       0x05

#define RF_SETUP    0x06
#define CONT_WAVE   7
#define RF_DR_LOW   5
#define PLL_LOCK    4
#define RF_DR_HIGH  3
#define RF_PWR      1

#define STATUS              0x07
#define RX_DR               6
#define TX_DS               5
#define MAX_RT              4
#define RX_P_NO_MASK        0x0E
#define STATUS_TX_FULL      0

#define OBSERVE_TX  0x08
#define PLOS_CNT    4
#define ARC_CNT     0

#define RPD         0x09

#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F

#define TX_ADDR     0x10

#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16

#define FIFO_STATUS         0x17
#define TX_REUSE            6
#define FIFO_TX_FULL        5
#define TX_EMPTY            4
#define RX_FULL             1
#define RX_EMPTY            0

#define DYNPD       0x1C
#define DPL_P5      5
#define DPL_P4      4
#define DPL_P3      3
#define DPL_P2      2
#define DPL_P1      1
#define DPL_P0      0

#define FEATURE     0x1D
#define EN_DPL      2
#define EN_ACK_PAY  1
#define EN_DYN_ACK 0

volatile uint8_t buffer[32]={};

uint8_t spi_write(uint8_t reg,uint8_t length)
{
	uint8_t status=0;
	length--;
	PORTB &= ~(1<<CS);
	status=spi_tranceiver(W_REGISTER|reg);
	while(length!=0)
	{
		spi_tranceiver(buffer[length]);
		length--;
	}
	spi_tranceiver(buffer[0]);
	PORTB |= 1<<CS;
	return status;
}

uint8_t spi_read(uint8_t reg,uint8_t length)
{
	uint8_t status=0;
	length--;
	PORTB &= ~(1<<CS);
	status=spi_tranceiver(R_REGISTER|reg);
	while(length!=0)
	{
		buffer[length]=spi_tranceiver(0);
		length--;
	}
	buffer[0]=spi_tranceiver(0);
	PORTB |= 1<<CS;
	return status;
}

uint8_t spi_send_command(uint8_t com)
{
	uint8_t status=0;
	PORTB &= ~(1<<CS);
	status=spi_tranceiver(com);
	PORTB |= 1<<CS;
	return status;
}

int main(void)
{
	DDRB |= 1<<CLK|1<<MOSI|1<<CS|1<<CE|1<<0;
	PORTB |= 1<<CS;

	SPCR |= 1<<SPE|1<<MSTR;
	SPSR |= 1<<SPI2X;

	uart_init();

	_delay_ms(150);
	buffer[0]= 1<<MASK_RX_DR|1<<MASK_TX_DS|1<<MASK_MAX_RT|1<<PRIM_RX|1<<EN_CRC|1<<PWR_UP;	//CONFIG REGISTER
	spi_write(CONFIG,1);

	_delay_ms(2);
	buffer[0]= 1<<ENAA_P0;	
	spi_write(EN_AA,1);

	buffer[0]=1<<ERX_P0;	
	spi_write(EN_RXADDR,1);

	buffer[0]= 1<<0|1<<1;	//5bytes address width
	spi_write(SETUP_AW,1);

	buffer[0]=50;	
	spi_write(RF_CH,1);

	buffer[0]=1<<RF_DR_HIGH|1<<1|1<<2;	
	spi_write(RF_SETUP,1);

	buffer[0]=1<<EN_DPL;
	spi_write(FEATURE,1);

	buffer[0]=1<<DPL_P0;
	spi_write(DYNPD,1);

	uart_send_string_ln("registers:");

	uart_send_string_ln("config");
	spi_read(CONFIG,1);
	uart_send_number_ln(buffer[0],2);

	uart_send_string_ln("en_aa");
	spi_read(EN_AA,1);
	uart_send_number_ln(buffer[0],2);

	uart_send_string_ln("en_rxaddr");
	spi_read(EN_RXADDR,1);
	uart_send_number_ln(buffer[0],2);

	uart_send_string_ln("SETUP_AW");
	spi_read(SETUP_AW,1);
	uart_send_number_ln(buffer[0],2);

	uart_send_string_ln("SETUP_RETR");
	spi_read(SETUP_RETR,1);
	uart_send_number_ln(buffer[0],2);

	uart_send_string_ln("RF_CH");
	spi_read(RF_CH,1);
	uart_send_number_ln(buffer[0],2);

	uart_send_string_ln("RF_SETUP");
	spi_read(RF_SETUP,1);
	uart_send_number_ln(buffer[0],2);

	uart_send_string_ln("FEATURE");
	spi_read(FEATURE,1);
	uart_send_number_ln(buffer[0],2);

	uart_send_string_ln("DYNPD");
	spi_read(DYNPD,1);
	uart_send_number_ln(buffer[0],2);

	PORTB |= 1<<CE;
	_delay_us(200);
	char temp=0;
	while (1)
	{
		temp = spi_send_command(NOP);
		if (temp&(1<<RX_DR))
		{
			spi_read(R_RX_PAYLOAD,1);
			temp=buffer[0];
			uart_send_character_ln( temp);
			spi_send_command(FLUSH_RX);
			buffer[0]=1<<RX_DR;
			spi_write(STATUS,1);
			if (temp=='1')
			{
				PORTB ^= 1<<0;
			}
		}
		_delay_ms(500);
	}
}




