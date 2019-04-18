#ifndef I2C
#define I2C

uint8_t i2c_start(void);
uint8_t i2c_address_write(uint8_t address);
uint8_t i2c_send_data(uint8_t data);
void i2c_stop(void);
uint8_t i2c_address_read(uint8_t address);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
void i2c_read_bytes(uint8_t *buffer,uint16_t length,uint8_t address);
void i2c_write_bytes(uint8_t *buffer,uint16_t length,uint8_t address);

//general functions of i2c, every peripheral has its own functions to communicate.

uint8_t i2c_start(void)
{
	//TWBR=198; //10khz clock
	TWBR= 72; //100kHz clock
	//TWBR= 12; //400kHz clock
	//TWBR= 0;  //1MHz clock
	TWCR= 1<<TWSTA|1<<TWINT|1<<TWEN;
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) !=0x08)
	{
		return 0;
	}
	return 1;
}
uint8_t i2c_address_write(uint8_t address)
{
	TWDR= address;
	TWCR = 1<<TWINT|1<<TWEN;
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) !=0x18)
	{
		return 0;
	}
	return 1;
}
uint8_t i2c_send_data(uint8_t data)
{
	TWDR= data;
	TWCR = 1<<TWINT|1<<TWEN;
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) !=0x28)
	{
		return 0;
	}
	return 1;
}
void i2c_stop(void)
{
	TWCR = 1<<TWINT|1<<TWSTO|1<<TWEN;
}
uint8_t i2c_address_read(uint8_t address)
{
	TWDR= address|1<<0;
	TWCR = 1<<TWINT|1<<TWEN;
	while (!(TWCR &(1<<TWINT)));
	if ((TWSR & 0xF8) !=0x40)
	{
		return 0;
	}
	return 1;
}
uint8_t i2c_read_ack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}
uint8_t i2c_read_nack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}
void i2c_read_bytes(uint8_t *buffer,uint16_t length,uint8_t address)
{
	i2c_start();
	i2c_address_read(address);
	uint16_t index=0;
	length--;
	while (index<length)
	{
		buffer[index]=i2c_read_ack();
		index++;
	}
	buffer[index]=i2c_read_nack();
	i2c_stop();
}
void i2c_write_bytes(uint8_t *buffer,uint16_t length,uint8_t address)
{
	i2c_start();
	i2c_address_write(address);
	uint16_t index=0;
	while (index<length)
	{
		i2c_send_data(buffer[index]);
		index++;
	}
	i2c_stop();
}
#endif