
/*
	UART en ATMEGA328P
	Licencia:
	Autor: Gonzalo Fernandez
*/ 

#include <avr/io.h>
#include <stdio.h>

#ifndef F_CPU
#define F_CPU 16000000
#endif

#define getc() _getc(&uart_io)
#define putc(x) _putc(x, &uart_io)

/*
	CONFIGURACI�N DE LA UART
	En este c�digo la configuraci�n que se plantea es como E/S est�ndar con 8 bits de datos y 2 bits stop.
	Para m�s informaci�n sobre la configuraci�n leer README.md
	La funci�n UART_init es de inicializaci�n, recibe como par�metro el baud rate de la comunicaci�n.
	
-------------------------------IMPORTANTE--------------------------------------------
	En la cabecera de main.c colocar la l�nea siguiente:
		FILE uart_io = FDEV_SETUP_STREAM(_putc, _getc, _FDEV_SETUP_RW);
	Y dentro del main al declarar entradas y salidas, colocar la siguiente l�nea:
		stdout = stdin = &uart_io;
--------------------------------------------------------------------------------------
*/

void UART_init(unsigned int ubrr){
	UBRR0=F_CPU/16/ubrr-1;
	UCSR0A=0;
	UCSR0B=(1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C=(1<<USBS0)|(3<<UCSZ00);
}

//	Funci�n de TRANSMISI�N

int _putc(char c, FILE *stream){
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0=c;
	return 0;
}

//	Funci�n de RECEPCI�N

int _getc(FILE *stream){
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

/*
-----------------------------------------------------------------------------------
	EJEMPLO DE RUTINA DE INTERRUPCI�N PARA UART
	
		ISR (USART_RX_vect){
			char dato=getc(); // char dato=getc(&uart_io); (�?)
			switch (dato) {
				case ':':
				indcom=0;
				break;
				case '\r':
				comando[indcom]=0;
				interprete();
				break;
				default:
				comando[indcom++]=dato;
				break;
			}
		}
-----------------------------------------------------------------------------------
*/