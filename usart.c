#include <stdio.h>
#include <avr/io.h>
#include "usart.h"

#define F_CPU 16000000UL	// arduino uno (atmega328p)
#define BAUD 9600			// set baud here
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)

FILE usart_str = FDEV_SETUP_STREAM(usart_putc, NULL, _FDEV_SETUP_RW);

void usart_init(void)
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
	stdout = &usart_str;
}

void usart_putc(unsigned char data)
{
 	while(!(UCSR0A & (1<<UDRE0)));
 		UDR0 = data;
}

void usart_puts(char * str)
{ 	
	while (*str)
 		usart_putc(*str++);
}

unsigned char usart_getc(void)
{
	while (!(UCSR0A & (1<<RXC0)) );
	return UDR0;	
}

/*
 gets a string ending in CR or maxlen 32 (derp)
*/
void usart_getstr(char * str)
{
	char c = 0;
	uint8_t pnt = 0;
	
	while (c != CR && pnt < 32)
	{
		c = usart_getc();
		if (c!= CR){
			usart_putc(c);
			if (c == BS && pnt > 0)
				pnt--;
			str[pnt++] = c;	
		}
		
	}
	str[pnt]=0;
	usart_putc('\r');
	
}