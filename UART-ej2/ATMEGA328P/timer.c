
/*
	Timers en ATMEGA328P
	Licencia:
	Autor: Gonzalo Fernandez
*/ 

#include <stdio.h>
#include <avr/io.h>

/*
	CONFIGURACION DE PRESCALER:
	La función PRESCALER_conf recibe como parámetro el número de TIMER y un entero de 0 a 5, que realizará lo siguiente:
	- 0: desconecta prescaler
	- 1: prescaler en 1
	- 2: prescaler en 8
	- 3: prescaler en 64
	- 4: prescaler en 256
	- 5: prescaler en 1024 
*/
void PRESCALER_conf(int TIMERx, int presc){
	int AUXREG=0;
	switch (TIMERx){
		case 0:
			AUXREG=TCCR0B;
			AUXREG&=~(7<<CS00);
			AUXREG|=(presc<<CS00);
			TCCR0B=AUXREG;
			break;
		case 1:
			AUXREG=TCCR1B;
			AUXREG&=~(7<<CS10);
			AUXREG|=(presc<<CS10);
			TCCR1B=AUXREG;
			break;
		case 2:
			AUXREG=TCCR2B;
			AUXREG&=~(7<<CS20);
			AUXREG|=(presc<<CS20);
			TCCR2B=AUXREG;
			break;
	}
}

/*
-------------------------------------------------------------------------------------------
	IMPORTANTE: Recuerde habilitar el pin como salida en DDRB.
				TIMER 0 y 2 son de 8 bits, por lo tanto cuentan hasta 256.
				TIMER 1 es de 16 bits, por lo tanto cuenta hasta 65536.
	
	Para implementar las rutinas de interrupción de cada TIMER, será de la forma:
		- Para interrupciones por match de OCRnX:
			ISR(TIMERn_COMPx_vect){
				RUTINA DE INTERRUPCIÓN
			}
		- Para interrupciones por overflow:
			ISR(TIMERn_OVF_vect){
				RUTINA DE INTERRUPCIÓN
			}
			
	Siendo n 0, 1 o 2 según el TIMER a utilizar; 
	y X A o B según  se haya utilizado OCRnA o OCRnB respectivamente.
-------------------------------------------------------------------------------------------
*/

/*
	TIMER EN MODO NORMAL
		La función TIMER_init recibe como parámetros:
			- TIMERx: Número de TIMER (0, 1 o 2).
			- X: A o B según la salida a utilizar.
			- mode: El comportamiento de la salida OCnX (siendo n el número de TIMER y X A o B):
				- 0: Salida desconectada.
				- 1: Toggle.
				- 2: Clear.
				- 3: Set (Inverso a clear).
		IMPORTANTE: Para éste código quedan habilitadas las interrupciones por overflow. Para deshabilitarlas
		quitar la línea:	
			TIMSK|=(1<<TOIEn);
		Siendo n el número de TIMER
*/
void TIMER_init(int TIMERx, char X, int mode){
	switch(TIMERx){
		case 0:
			TIFR0&=~(1<<TOV0);
			TIMSK0|=(1<<TOIE0);
			switch(X){
				case 'A':
					TCCR0A|=(mode<<COM0A0);
					break;
				case 'B':
					TCCR0B|=(mode<<COM0B0);
					break;
			}
			break;
		case 1:
			TIFR1&=~(1<<TOV1);
			TIMSK1|=(1<<TOIE1);
			switch(X){
				case 'A':
					TCCR1A|=(mode<<COM1A0);
					break;
				case 'B':
					TCCR1B|=(mode<<COM1B0);
					break;
			}
			break;
		case 2:
			TIFR2&=~(1<<TOV2);
			TIMSK2|=(1<<TOIE2);
			switch(X){
				case 'A':
					TCCR2A|=(mode<<COM2A0);
					break;
				case 'B':
					TCCR2B|=(mode<<COM2B0);
					break;
			}
			break;
	}
}

/*
	TIMER EN MODO CTC (Clear Timer on Compare match)
		La función TIMER_CTC_init recibe como parámetros:
			- TIMERx: Número de TIMER (0, 1 o 2).
			- X: A o B según la salida a utilizar.
			- T: Periodo dado en pulsos de clock, teniendo en cuenta la configuración de prescaler.
			- mode: El comportamiento de la salida OCnX (siendo n el número de TIMER y X A o B):
				- 0: Salida desconectada.
				- 1: Toggle.
				- 2: Clear.
				- 3: Set (Inverso a clear).
		IMPORTANTE: Para éste código quedan habilitadas las interrupciones por match de OCRnX. 
		Esta configuración puede cambiarse en el registro TIFRn tiene los bits ICIEn, OCIEnA y OCIEnB, y TOIE1 que en 1 son:
			- ICIEn: Habilitación por captura de entrada.
			- OCIEnA y OCIEnB: Habilitación de interrupciones por match de OCRnA y OCRnB respectivamente.
			- TOIEn: Habilitación de interrupciones por overflow.
*/
void TIMER_CTC_init(int TIMERx, char X, int T, int mode){
	switch(TIMERx){
		case 0:
			TCCR0B|=(1<<WGM02);
			switch(X){
				case 'A':
					TCCR0A|=(mode<<COM0A0);
					TIFR0&=~(1<<OCF0A);
					TIMSK0|=(1<<OCIE0A);
					OCR0A=T;
					break;
				case 'B':
					TCCR0B|=(mode<<COM0B0);
					TIFR0&=~(1<<OCF0B);
					TIMSK0|=(1<<OCIE0B);
					OCR0B=T;
				break;
			}
			break;
		case 1:
			TCCR1B|=(1<<WGM12);
			switch(X){
				case 'A':
					TCCR1A|=(mode<<COM1A0);
					TIFR1&=~(1<<OCF1A);
					TIMSK1|=(1<<OCIE1A);
					OCR1A=T;
					break;
				case 'B':
					TCCR1B|=(mode<<COM1B0);
					TIFR1&=~(1<<OCF1B);
					TIMSK1|=(1<<OCIE1B);
					OCR1B=T;
					break;
			}
			break;
		case 2:
			TCCR2B|=(1<<WGM22);
			switch(X){
				case 'A':
					TCCR2A|=(mode<<COM2A0);
					TIFR2&=~(1<<OCF2A);
					TIMSK2|=(1<<OCIE2A);
					OCR2A=T;
					break;
				case 'B':
					TCCR2B|=(mode<<COM2B0);
					TIFR2&=~(1<<OCF2B);
					TIMSK2|=(1<<OCIE2B);
					OCR2B=T;
					break;
			}
			break;
	}
}

/*
	TIMER EN MODO CORRECT PHASE PWM	(SOLO PARA TIMER 1)
		La función TIMER_CorrectPhasePWM_init recibe como parámetros:
			- X: A o B según la salida a utilizar.
			- T: Periodo dado en pulsos de clock, teniendo en cuenta la configuración de prescaler.
			- duty: Duty cycle dado en pulsos de clock, teniendo en cuenta la configuración de prescaler.
			- mode: El comportamiento de la salida OCnX (siendo n el número de TIMER y X A o B):
				- 0: Salida desconectada.
				- 1: Toggle.
				- 2: Clear.
				- 3: Set (Inverso a clear).
		IMPORTANTE: Para éste código quedan habilitadas las interrupciones por match de OCRnX.
		Esta configuración puede cambiarse en el registro TIFRn tiene los bits ICIEn, OCIEnA y OCIEnB, y TOIE1 que en 1 son:
			- ICIEn: Habilitación por captura de entrada.
			- OCIEnA y OCIEnB: Habilitación de interrupciones por match de OCRnA y OCRnB respectivamente.
			- TOIEn: Habilitación de interrupciones por overflow.
*/
void TIMER_CorrectPhasePWM_init(char X, int T, int duty, int mode){
	ICR1=T;
	TCCR1B|=(1<<WGM13);
	switch(X){
		case 'A':
			OCR1A=duty;
			TCCR1A|=(mode<<COM1A0);
			TIFR1&=~(1<<OCF1A);
			TIMSK1=(1<<OCIE1A);
			break;
		case 'B':
			OCR1B=duty;
			TCCR1B|=(mode<<COM1B0);
			TIFR1&=~(1<<OCF1B);
			TIMSK1=(1<<OCIE1B);
			break;
	}
}

/*
	TIMER EN MODO FAST PWM (SOLO PARA TIMER 1)
		La función TIMER_FastPWM_init recibe como parámetros:
			- X: A o B según la salida a utilizar.
			- T: Periodo dado en pulsos de clock, teniendo en cuenta la configuración de prescaler.
			- duty: Duty cycle dado en pulsos de clock, teniendo en cuenta la configuración de prescaler.
			- mode: El comportamiento de la salida OCnX (siendo n el número de TIMER y X A o B):
				- 0: Salida desconectada.
				- 1: Toggle.
				- 2: Clear.
			- 3: Set (Inverso a clear).
		IMPORTANTE: Para éste código quedan habilitadas las interrupciones por match de OCRnX.
		Esta configuración puede cambiarse en el registro TIFRn tiene los bits ICIEn, OCIEnA y OCIEnB, y TOIE1 que en 1 son:
			- ICIEn: Habilitación por captura de entrada.
			- OCIEnA y OCIEnB: Habilitación de interrupciones por match de OCRnA y OCRnB respectivamente.
			- TOIEn: Habilitación de interrupciones por overflow.
*/
void TIMER_FastPWM_init(char X, int T, int duty, int mode){
	ICR1=T;
	TCCR1A|=(1<<WGM11);
	TCCR1B|=(1<<WGM12)|(1<<WGM13);
	switch(X){
		case 'A':
			OCR1A=duty;
			TCCR1A|=(mode<<COM1A0);
			TIFR1&=~(1<<OCF1A);
			TIMSK1=(1<<OCIE1A);
			break;
		case 'B':
			OCR1B=duty;
			TCCR1A|=(mode<<COM1B0);
			TIFR1&=~(1<<OCF1B);
			TIMSK1=(1<<OCIE1B);
			break;
	}
}