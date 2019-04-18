#ifndef uart
#define uart

void uart_send_number(uint32_t value,uint8_t format);
void uart_send_number_ln(uint32_t value, uint8_t format);
void uart_send_string_ln( char *a);
void uart_send_string( char *a);
void uart_send_character(char a);
void uart_send_character_ln(char a);
void uart_init(void);

void uart_send_number(uint32_t value,uint8_t format)
{
	char str[10]={};
	ultoa(value,str,format);
	uint8_t i=0;
	while(str[i]!='\0')
	{
		while ( !( UCSRA & (1<<UDRE)) );
		UDR = str[i];
		i++;
	}
}

void uart_send_number_ln(uint32_t value, uint8_t format)
{
	char str[10]={};
	ultoa(value,str,format);
	uint8_t i=0;
	while(str[i]!='\0')
	{
		while ( !( UCSRA & (1<<UDRE)) );
		UDR = str[i];
		i++;
	}
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = 10;	//new line
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = 13;	//carriage return
}

void uart_send_string_ln( char *a)
{
	while (*a!='\0')
	{
		while ( !( UCSRA & (1<<UDRE)) );
		UDR=(*a);
		a++;
	}
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = 10;	//new line
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = 13;	//carriage return
}

void uart_send_string( char *a)
{
	while (*a!='\0')
	{
		while ( !( UCSRA & (1<<UDRE)) );
		UDR=(*a);
		a++;
	}
}

void uart_send_character(char a)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR=(a);
}

void uart_send_character_ln(char a)
{
	while ( !( UCSRA & (1<<UDRE)) );
	UDR=(a);
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = 10;	//new line
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = 13;	//carriage return
}

void uart_init(void)
{
	UCSRA = 1<<U2X; //setting uart 8bits,1stop,no parity,2Mbaud
	UCSRB = 1<<RXEN|1<<TXEN;
	UCSRC = 1<<URSEL|1<<UCSZ1|1<<UCSZ0;
	UBRRH = 0;
	UBRRL = 0;
}
#endif