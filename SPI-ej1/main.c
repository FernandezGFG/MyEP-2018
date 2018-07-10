/*
 * SPI-ej1.c
 *
 * Autor : Gonzalo Fernández
 * Legajo N° 11544
 *
 */ 

#define F_CPU 16000000

#include <avr/io.h>		
#include <stdio.h>
#include <avr/interrupt.h>	
#include <stdint.h>		
#include <util/delay.h>	
	
#include "ATMEGA328P/SPI.h"	
#include "ATMEGA328P/UART.h"
#include "ATMEGA328P/timer.h"

#define brate0 9600

uint8_t indcom;
char comando[10];

void interprete(void);

uint8_t valor;

FILE uart_io = FDEV_SETUP_STREAM(_putc, _getc, _FDEV_SETUP_RW);

#define pinCS PORTB2

#define control 0b00010001

//------------INTERPRETE DE COMANDO-----------------------
void interprete(void){
	switch (comando[0]) {
		case 'P':
		if (comando[1]){
			valor=atoi(&comando[1]);
			printf("Sending...");
			PORTB&=~(1<<pinCS);
			SPI_transfer(control);
			SPI_transfer(valor);
			_delay_us(1);
			PORTB|=(1<<pinCS);
			_delay_us(1);
			printf("P: %d\n", valor);
			} else {
			printf(":ERR\n");
		}
		break;

		default:
		printf(":ERR\n");
		break;
	}
}

//----------------------------------------------------------------------------
//    Rutina de servicio de interrupción por recepción de datos en UART
ISR (USART_RX_vect){
	char dato=getc();
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

//----------------------------------------------------------------------------
//	RUTINA PRINCIPAL

int main(void){
	
	DDRB=(1<<DDB1);
	
	UART_init(brate0);
	stdout = stdin = &uart_io;
	indcom=0;
	
	PRESCALER_conf(1,4);
	TIMER_CTC_init(1,'A',2,1);
	TIMSK1&=~(1<<OCIE1A);
	
	SPI_init(0,0);				   // inicializa en modo 0, con prescaler x4 (SCK=16/4=4Mbps)
	
	PORTB|=(1<<pinCS);
	DDRB|=(1<<pinCS);  
	
	sei();
	
	while (1){
		_delay_us(1);
	}
}

