
/*
	Interrrupciones en ATMEGA328P
	Licencia:
	Autor: Gonzalo Fernandez
*/

#include <avr/io.h>
#include <avr/interrupt.h>

void EI_init(int INTx, int mode);
void PCI_init(int PCINTx);