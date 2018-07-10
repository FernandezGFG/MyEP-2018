/*
 * UART-ej2.c
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

FILE uart_io = FDEV_SETUP_STREAM(_putc, _getc, _FDEV_SETUP_RW);

volatile int periodo=1000;
volatile int N=0, Na=0;

void interprete(void);

//------------INTERPRETE-----------------------
void interprete(void){
	int aux;
  switch (comando[0]) {
    case 'P':
	   if (comando[1]){
		   N=atoi(&comando[1]);
		   printf("P: %d", N);
	   } else {
		   printf(":ERR\n");
	   }
      break;
	  
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
  char dato=getc(); // char dato=getc(&uart_io); (¿?)
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
//------------------------------------------------------------------------------------
//	Rutina de interrupción del timer
ISR (TIMER1_COMPA_vect){
	if (Na!=N){
		if (N<Na){
				PORTB&=~(1<<PORTB3);
				Na--;
			} else if (N>Na){
				PORTB|=(1<<PORTB3);
				Na++;
			} else if (N==Na){
				_delay_us(1);
		}
		PORTB^=(1<<PORTB5);
		TIFR1&=~(1<<OCF1A);
		printf("Pos. actual=%d Pos. deseada=%d\n", Na, N);
	} else if (N==Na){
		PORTB&=~(1<<PORTB5);
	}
}

//----------------------RUTINA PRINCIPAL-----------------------------
int main(void){
	
  DDRB=(1<<DDB5);
  
  UART_init(brate0);
  stdout = stdin = &uart_io;
  indcom=0;
  
  PRESCALER_conf(1,4);
  TIMER_CTC_init(1,'A',periodo,0);
  
  sei();
  
  while (1){
	  _delay_us(1);
  }
}

