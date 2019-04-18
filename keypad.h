#ifndef KEYPAD
#define KEYPAD

#include <util/delay.h>
#include <avr/pgmspace.h>

#define KBD_PORTX	PORTA
#define KBD_DDRX		DDRA
#define KBD_PINX		PINA

const uint8_t KBDnumbers[16] PROGMEM ={0x82,0x11,0x12,0x14,0x21,0x22,0x24,0x41,0x42,0x44,0x18,0x28,0x48,0x88,0x81,0x84};
//const uint8_t KBDnumbers[16] PROGMEM ={0x28,0x11,0x21,0x41,0x12,0x22,0x42,0x14,0x24,0x44,0x81,0x82,0x84,0x88,0x18,0x48};
const char buttons_label[16] PROGMEM ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','*','#'};

char scan_keypad(void);

char scan_keypad(void)
{
	uint8_t button=255;
	while(button!=0)
	{
		KBD_DDRX=0xf0;
		KBD_PORTX=0x0f;
		_delay_us(2);
		button=KBD_PINX;
		KBD_DDRX=0x0f;
		KBD_PORTX=0xf0;
		_delay_us(2);
		button|=KBD_PINX;
		button=~button;
		_delay_ms(15);
	}
	while(button==0)
	{
		KBD_DDRX=0xf0;
		KBD_PORTX=0x0f;
		_delay_us(2);
		button=KBD_PINX;
		KBD_DDRX=0x0f;
		KBD_PORTX=0xf0;
		_delay_us(2);
		button|=KBD_PINX;
		button=~button;
		_delay_ms(15);
	}
	uint8_t i=0;
	while(button!=pgm_read_byte(&KBDnumbers[i]) && i<16)
	{
		i++;
	}
	return pgm_read_byte(&buttons_label[i]);
}

#endif