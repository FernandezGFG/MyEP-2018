/*
 * UART-ej1.c
 *
 * Autor : Gonzalo Fernández
 * Legajo N° 11544
 *
 */ 

#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ATMEGA328P/timer.h"
#include "ATMEGA328P/UART.h"

#define  F_CPU 16000000
#define brate0 9600

uint8_t indcom;
char comando[30];

enum tEstado {activo, inactivo, home};
enum tEstado estado = inactivo;

volatile int N=0, Na=0;

int periodo=5000;
int flag_home=0;

void interprete(void);

FILE uart_io = FDEV_SETUP_STREAM(_putc, _getc, _FDEV_SETUP_RW);

//------------INTERPRETE-----------------------
void interprete(void){
	int aux;
  switch (comando[0]) {
    case 'P':
      /*
        Rutina para dar posición
        :PXX donde la posición dependerá del sistema en aplicación
      */
	   if (comando[1] && estado==activo){
		   N=atoi(&comando[1]);
		   printf("P: %d", N);
	   } else if (estado ==inactivo) {
		   printf(":ERR\n");
	   }
      break;
    case 'T':
      /*
        Rutina para dar periodo de pulso
        :TXX siendo un perido de XX ms
      */
	  if (comando[1] && estado==activo){
		  periodo=atoi(&comando[1]);
		  OCR1A=periodo*6250;
		  printf("T: %d\n", periodo);
	  } else if (estado==inactivo) {
		  printf(":ERR\n");
	  }
      break;
	
    case 'W':
      /*
        Rutina para máquina de estado
        :W1 -> ESTADO ACTIVADO
        :W0 -> ESTADO REPOSO
      */
	  if (comando[1]){
		  switch (comando[1]){
				case '1':
					estado = activo;
					printf(":ACT\n");
					break;
				case '0':
					estado = inactivo;
					printf(":INACT\n");
					break;
				default:
					printf(":ERR\n");
					break;
		  }  
	  }
      break;
    case 'H':
      /*
        Rutina para hacer homing (:H)
      */
	  if (estado==activo){
		  // HOMING
		  estado=home;
		  OCR1A=10000;
		  flag_home=1;
	  } else if (estado==inactivo) {
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
//------------------------------------------------------------------------------------
//	Rutina de interrupción del timer

ISR (TIMER1_COMPA_vect){
	if (estado==activo && Na!=N){
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
		//printf("Pos. actual=%d Pos. deseada=%d\n", Na, N);
	} else if (estado==inactivo){
		PORTB&=~(1<<PORTB5);
	} else if (estado==home){
		if (PINB&(1<<0)){
			if (flag_home==1){
				PORTB&=~(1<<PORTB3);
				printf("HOMING...\n");
				PORTB^=(1<<PORTB5);
				TIFR1&=~(1<<OCF1A);
				Na--;
			} else if (flag_home==0){
				N=0;
				Na=0;
				OCR1A=5000;
				printf("HOMING FINALIZADO\n");
				estado=activo;
			}
		} else if (~PINB&(1<<0)){
			flag_home=0;
			PORTB|=(1<<PORTB3);
			printf("RETORNO...\n");
			PORTB^=(1<<PORTB5);
			TIFR1&=~(1<<OCF1A);
			Na++;
		}
	}
}

//----------------------RUTINA PRINCIPAL-----------------------------
int main(void){
	
  UART_init(brate0);
  stdout = stdin = &uart_io;
  indcom=0;
  
  DDRB=(1<<DDB5)|(1<<DDB3);
  
  PRESCALER_conf(1,4);
  TIMER_CTC_init(1,'A',periodo,0);
  
  sei();
  
  while (1){
	  _delay_us(1);
	  if (~PINB&(1<<0)){
		  printf("FINAL DE CARRERA");
	  }
  }
}