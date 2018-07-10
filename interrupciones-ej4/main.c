/*
 * interrupciones-ej4.c
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

#include "ATMEGA328P/interrupt.h"
#include "ATMEGA328P/timer.h"

int iter1=50;
int iter2=50;

//--------------------------------------------------------------
//	Rutina de interrupción externa

ISR(INT0_vect){
	iter1=0;
}

//	Rutina de interrupción por cambio en pin

ISR(PCINT0_vect){
	sei();
	iter2=0;
}

//	Rutina de interrupción de TIMER

ISR(TIMER1_COMPA_vect){
	if(iter1<30){
		PORTB^=(1<<DDB1);
		iter1++;
	}
	if(iter2<20){
		PORTB^=(1<<DDB2);
		iter2++;
	}
	TIFR1&=~(1<<OCF1A);
}

//---------------------------------------------------------------
int main(void){
	
    DDRB=(1<<DDB5)|(1<<DDB2)|(1<<DDB1);
	
	PCI_init(3);
	EI_init(0,3);
	
	PRESCALER_conf(1,5);
	TIMER_CTC_init(1,'A',15624,0);
	
	sei();
    while (1){
		PORTB^=(1<<DDB5);
		_delay_ms(500);
    }
}