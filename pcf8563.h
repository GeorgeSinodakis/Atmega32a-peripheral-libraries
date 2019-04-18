#ifndef PCF8563
#define PCF8563

volatile uint8_t seconds=0,minutes=0,hours=0,days=0,weekdays=0,months=0,years=0;
volatile uint8_t init_values[16]={0,0,0,0x07,0x21,0x06,0x05,0x10,0x17,0x80,0x80,0x80,0x80,0x83,0,0};

// control_status_1,control_status_2,vl_seconds,minutes,hours,days,weekdays,century_months,years,minute_alarm,
//hour_alarm,day_alarm,weekday_alarm,clkout_control,timer_control,timer
//address	0x00-0x0f

void pcf8563_write_init(void);
void pcf8563_read(void);

void pcf8563_write_init()
{
	i2c_start();
	i2c_address_write(0xa2);
	i2c_send_data(0);
	for (uint8_t i=0;i<=15;i++)
	{
		i2c_send_data(init_values[i]);
	}
	i2c_stop();
}

void pcf8563_read()
{
	i2c_start();
	i2c_address_write(0xa2);
	i2c_send_data(2);
	i2c_start();
	i2c_address_read(0xa2);
	seconds=i2c_read_ack();
	minutes=i2c_read_ack();
	hours=i2c_read_ack();
	days=i2c_read_ack();
	weekdays=i2c_read_ack();
	months=i2c_read_ack();
	years=i2c_read_nack();
	i2c_stop();
}

#endif