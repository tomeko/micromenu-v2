#ifndef USART_H_
#define USART_H_

#define CR 0x0d
#define LF 0x0a
#define BS 0x08

void usart_init(void);
void usart_putc(unsigned char);
void usart_puts(char *);
unsigned char usart_getc(void);
void usart_getstr(char * str);

#endif