#ifndef LCD-8BIT
#define LCD-8BIT

#include <util/delay.h>
#include <stdlib.h>

#define data_PORT		PORTB
#define data_DDR		DDRB
#define data_PIN		PINB
#define control_PORT	PORTD
#define control_DDR		DDRD

#define rs				0
#define rw				1
#define en				2
#define bsy             7

#define lcd_home        0b00000010
#define lcd_on          0b00001100
#define lcd_off         0b00001000

void lcd_wait(void);
void lcd_cmd(uint8_t);
void lcd_data(char a);
void lcd_init(void);
void lcd_posit_cursor(uint8_t);
void lcd_string(const char *a);
void lcd_clear(void);
void lcd_cursor_on_noblink(void);
void lcd_cursor_off(void);
void lcd_cursor_on_blink(void);

void lcd_wait(void)
{
	data_DDR=0;
	while(1)
	{
		control_PORT=0<<rs|1<<rw|1<<en;
		_delay_us(1);
		if (bit_is_clear(data_PIN,bsy))
		{
			break;
		}
		control_PORT=0;
	}
	control_PORT=0;
	data_DDR=255;
}
void lcd_cmd(unsigned int a)
{
	lcd_wait();
	data_PORT=a;
	control_PORT =(0<<rs)|(0<<rw)|(1<<en);
	_delay_ms(1);
	control_PORT =(0<<rs)|(0<<rw)|(0<<en);
}
void lcd_data(char a)
{
	lcd_wait();
	data_PORT=a;
	control_PORT = (1<<rs)|(0<<rw)|(1<<en);
	_delay_ms(1);
	control_PORT =  (1<<rs)|(0<<rw)|(0<<en);
}
void lcd_init(void)
{
	control_DDR = 1<<rs|1<<rw|1<<en;
	data_DDR =255;
	lcd_cmd(0b00111000);               
	_delay_ms(1);
	lcd_cmd(0b00000001);               
	_delay_ms(1);
	lcd_cmd(0b00001100);               
	_delay_ms(1);
	lcd_cmd(0x80);               
	_delay_ms(1);
	lcd_cmd(0b00111000);               
	_delay_ms(1);
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