/*
 * UART-ej1.c
 *
 * Autor : Gonzalo Fernández
 * Legajo N° 11544
 *
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ATMEGA328P/timer.h"
#include "ATMEGA328P/UART.h"

#define brate0 9600

uint8_t indcom;
char comando[10];

void interprete(void);

volatile int periodo=1000;

FILE uart_io = FDEV_SETUP_STREAM(_putc, _getc, _FDEV_SETUP_RW);

//------------INTERPRETE DE COMANDO-----------------------
void interprete(void){
  switch (comando[0]) {
    case 'T':
	  if (comando[1]){
		  periodo=atoi(&comando[1]);
		  OCR1A=periodo*6250;
		  printf("T: %d\n", periodo);
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

//----------------------RUTINA PRINCIPAL-----------------------------
int main(void){
	
  UART_init(brate0);
  stdout = stdin = &uart_io;
  indcom=0;
  
  DDRB=(1<<DDB1);
  
  PRESCALER_conf(1,4);
  TIMER_CTC_init(1,'A',periodo,1);
  TIMSK1&=~(1<<OCIE1A);
  sei();
  
  while (1){
	  _delay_us(1);
  }
}

