#ifndef LCD-4BIT
#define LCD-4BIT

#include <util/delay.h>
#include <stdlib.h>

#define lcd				PORTC 
#define lcd_DDR			DDRC
#define rs				0
#define en				2

#define lcd_home        0b00000010
#define lcd_on          0b00001100
#define lcd_off         0b00001000

void lcd_cmd(uint8_t);
void lcd_data(char);
void lcd_init(void);
void lcd_posit_cursor(uint8_t);
void lcd_string(const char *a);
void lcd_clear(void);
void lcd_cursor_on_noblink(void);
void lcd_cursor_off(void);
void lcd_cursor_on_blink(void);

void lcd_cmd(uint8_t a)
{
	lcd =(a&0xF0);
	lcd |=(1<<en);
	_delay_ms(1);
	lcd &=~(1<<en);
	_delay_ms(1);
	lcd =((a&0x0F)<<4);
	lcd |=(1<<en);
	_delay_ms(1);
	lcd &=~(1<<en);
	_delay_ms(1);
	return;
}
void lcd_data(char a)
{
	lcd =(a&0xF0);
	lcd |=(1<<rs);
	lcd |=(1<<en);
	_delay_ms(1);
	lcd &=~(1<<en);
	lcd &=~(1<<rs);
	_delay_ms(1);
	lcd =((a&0x0F)<<4);
	lcd |=(1<<rs);
	lcd |=(1<<en);
	_delay_ms(1);
	lcd &=~(1<<en);
	lcd &=~(1<<rs);
	_delay_ms(1);
	return;
}
void lcd_init(void)
{
	_delay_ms(15);
	lcd=0;
	lcd_DDR=0xFF;
	lcd=0b00110000;
	lcd|=(1<<en);
	_delay_ms(1);
	lcd&=~(1<<en);
	_delay_ms(1);
	lcd=0b00110000;
	lcd|=(1<<en);
	_delay_ms(1);
	lcd&=~(1<<en);
	_delay_ms(1);
	lcd=0b00100000;
	lcd|=(1<<en);
	_delay_ms(1);
	lcd&=~(1<<en);
	_delay_ms(1);
	lcd_cmd(0b00101000);
	lcd_cmd(0b00001100);
	lcd_cmd(lcd_clear);
	return;
}
void lcd_posit_cursor(uint8_t c)
{
	c &= 0x1f;
	if (c<=15)
	{
		lcd_cmd(c|0x80);
	}
	else
	{
		lcd_cmd(((c-15)+63)|0x80);
	}
	return;
}
void lcd_clear(void)
{
	lcd_cmd(1);
}
void lcd_string(const char *a)
{
	uint8_t i=0;
	while (a[i]!='\0')
	{
		lcd_data(a[i]);
		i++;
	}
	return;
}
void lcd_cursor_on_noblink(void)
{
	lcd_cmd(0x0e);
}
void lcd_cursor_off(void)
{
	lcd_cmd(0x0c);
}
void lcd_cursor_on_blink(void)
{
	lcd_cmd(0x0f);
}

#endif