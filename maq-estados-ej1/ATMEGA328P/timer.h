
/*
	Timers en ATMEGA328P
	Licencia:
	Autor: Gonzalo Fernandez
*/ 

#include <stdio.h>
#include <avr/io.h>

void PRESCALER_conf(int TIMERx, int presc);
void TIMER_init(int TIMERx, char X, int mode);
void TIMER_CTC_init(int TIMERx, char X, int T, int mode);
void TIMER_CorrectPhasePWM_init(char X, int T, int duty, int mode);
void TIMER_FastPWM_init(char X, int T, int duty, int mode);