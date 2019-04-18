#ifndef lcd-i2c
#define lcd-i2c

#include <util/delay.h>
#include <stdlib.h>

#define rs				0
#define rw				1
#define en				2
#define backlight		3 

#define lcd_home        0b00000010
#define lcd_on          0b00001100
#define lcd_off         0b00001000

void lcd_i2c_send_byte(uint8_t);
void lcd_cmd(uint8_t);
void lcd_data(uint8_t);
void lcd_init(void);
void lcd_posit_cursor(uint8_t);
void lcd_clear(void);
void lcd_string(const char *a);
void lcd_cursor_on_noblink(void);
void lcd_cursor_off(void);
void lcd_cursor_on_blink(void);

void lcd_i2c_send_byte(uint8_t data)
{
	i2c_start();
	i2c_address_write(126);
	i2c_send_data(data);
	i2c_stop();
}
void lcd_cmd(uint8_t a)
{
	lcd_i2c_send_byte((a&0xF0)|(1<<en)|1<<backlight);
	_delay_ms(1);
	lcd_i2c_send_byte(1<<backlight);
	lcd_i2c_send_byte(((a&0x0F)<<4)|(1<<en)|1<<backlight);
	_delay_ms(1);
	lcd_i2c_send_byte(1<<backlight);
	_delay_ms(1);
}
void lcd_data(uint8_t a)
{
	lcd_i2c_send_byte((a&0xF0)|1<<backlight|(1<<rs)|(1<<en));
	_delay_ms(1);
	lcd_i2c_send_byte(1<<backlight);
	lcd_i2c_send_byte(((a&0x0F)<<4)|1<<backlight|(1<<rs)|(1<<en));
	_delay_ms(1);
	lcd_i2c_send_byte(1<<backlight);
	_delay_ms(1);
}
void lcd_init(void)
{
	_delay_ms(15);
	lcd_i2c_send_byte(0x30|(1<<en));
	_delay_ms(1);
	lcd_i2c_send_byte(0);
	_delay_ms(1);
	lcd_i2c_send_byte(0x30|(1<<en));
	_delay_ms(1);
	lcd_i2c_send_byte(0);
	_delay_ms(1);
	lcd_i2c_send_byte(0x20|(1<<en));
	_delay_ms(1);
	lcd_i2c_send_byte(0);
	_delay_ms(1);
	lcd_cmd(0x28);
	lcd_cmd(0x0c);
	lcd_cmd(0x01);
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
	lcd_cmd(0x01);
}
void lcd_string(const char *a)
{
	while (*a!='\0')
	{
		lcd_data(*a);
		a++;
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