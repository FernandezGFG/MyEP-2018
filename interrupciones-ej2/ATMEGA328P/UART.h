
/*
	UART en ATMEGA328P
	Licencia:
	Autor: Gonzalo Fernandez
*/ 

#include <avr/io.h>
#include <stdio.h>

#define  F_CPU 16000000

FILE uart_io = FDEV_SETUP_STREAM(_putc, _getc, _FDEV_SETUP_RW);

#define getc() _getc(&uart_io)
#define putc(x) _putc(x, &uart_io)

void UART_init(unsigned int ubrr);
int _putc(char c, FILE *stream);
int _getc(FILE *stream);