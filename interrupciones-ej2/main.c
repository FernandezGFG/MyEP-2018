/*
 * interrupciones-ej2.c
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

int periodo=7812;

//-----------------------------------------------------------------------------
//	Rutina de interrupción por cambio en pin

ISR(PCINT0_vect){
	if(PINB & (1<<PINB3)){
		PORTB^=(1<<DDB2);
	}
}

//	Rutina de interrupción de TIMER

ISR(TIMER1_COMPA_vect){
	PORTB^=(1<<DDB5);
}
//-----------------------------------------------------------------------------
// Rutina principal

int main(void){
	DDRB=(1<<DDB5)|(1<<DDB2);
	PCICR|=(1<<PCIE0);
	PCMSK0|=(1<<PCINT3);
	PRESCALER_conf(1,5);
	TIMER_CTC_init(1,'A',periodo,0);
	sei();
    while (1){
		_delay_us(1);
    }
}

