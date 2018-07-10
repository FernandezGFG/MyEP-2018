
/*
	Interrrupciones en ATMEGA328P
	Licencia:
	Autor: Gonzalo Fernandez
*/

#include <avr/io.h>
#include <avr/interrupt.h>

/*
INTERRUPCION EXTERNA:
  La función EI_init recibe como parámetros la entrada (pin), en el caso de ATMEGA328P
   0 o 1, y el modo de interrrupción.
   Los modos de interrupción son:
   - mode = 0: Interrumpe por nivel bajo.
   - mode = 1: Interrumpe por cualquier cambio.
   - mode = 2: Interrumpe por flanco de bajada.
   - mode = 3: Interrumpe por flanco de subida.
   La rutina de interrupción será de la forma:
   ISR(INTx_vect){
      Rutina a realizar.
   }
   Siendo x 0 o 1 según el pin a utilizar.
   Existe un registro de flag EIFR, al que se puede acceder con los bits INTF0 o INTF1
   según sea el caso. Al producirse la interrupción se activan los flags correspondientes.
*/

void EI_init(int INTx, int mode){
  switch (INTx){
    case 0:
      EICRA|=(mode<<ISC00);
      EIMSK|=(1<<INT0);
      break;
    case 1:
      EICRA|=(mode<<ISC10);
      EIMSK|=(1<<INT1);
      break;
    default:
      printf(":ERR EI_init\n");
  }
}

/*
INTERRUPCIÓN POR CAMBIO EN PIN:
	La función PCI_init recibe como parámetro el número de pin (de 0 a 23) que se utilizará 
	como interrupción.
	 La rutina de interrupción será de la forma:
	 ISR(PCINTx_vect){
		 Rutina a realizar.
	 }
	Siendo x de 0 a 23 según el pin a utilizar.
	Existe un registro de flag PCIFR, al que se puede acceder con los bits PCIF0, PCIF1 o PCIF2
	según sea el grupo de la interrupción deseada. Al producirse la interrupción se activan los flags correspondientes.
*/

void PCI_init(int PCINTx){
  if (PCINTx>=0 && PCINTx<=7){
    PCICR|=(1<<PCIE0);
    switch(PCINTx){
		case 0:
			PCMSK0|=(1<<PCINT0);
			break;
		case 1:
			PCMSK0|=(1<<PCINT1);
			break;
		case 2:
			PCMSK0|=(1<<PCINT2);
			break;
		case 3:
			PCMSK0|=(1<<PCINT3);
			break;
		case 4:
			PCMSK0|=(1<<PCINT4);
			break;
		case 5:
			PCMSK0|=(1<<PCINT5);
			break;
		case 6:
			PCMSK0|=(1<<PCINT6);
			break;
		case 7:
			PCMSK0|=(1<<PCINT7);
			break;
	}
  }
  if (PCINTx>7 && PCINTx<=14){
	  PCICR|=(1<<PCIE1);
	  switch(PCINTx){
		  case 8:
		  PCMSK1|=(1<<PCINT8);
		  break;
		  case 9:
		  PCMSK1|=(1<<PCINT9);
		  break;
		  case 10:
		  PCMSK1|=(1<<PCINT10);
		  break;
		  case 11:
		  PCMSK1|=(1<<PCINT11);
		  break;
		  case 12:
		  PCMSK1|=(1<<PCINT12);
		  break;
		  case 13:
		  PCMSK1|=(1<<PCINT13);
		  break;
		  case 14:
		  PCMSK1|=(1<<PCINT14);
		  break;
	  }
  }
  if (PCINTx>15 && PCINTx<=23){
	  PCICR|=(1<<PCIE2);
	  switch(PCINTx){
		  case 16:
		  PCMSK2|=(1<<PCINT16);
		  break;
		  case 17:
		  PCMSK2|=(1<<PCINT17);
		  break;
		  case 18:
		  PCMSK2|=(1<<PCINT18);
		  break;
		  case 19:
		  PCMSK2|=(1<<PCINT19);
		  break;
		  case 20:
		  PCMSK2|=(1<<PCINT20);
		  break;
		  case 21:
		  PCMSK2|=(1<<PCINT21);
		  break;
		  case 22:
		  PCMSK2|=(1<<PCINT22);
		  break;
		  case 23:
		  PCMSK2|=(1<<PCINT23);
		  break;
	  }
  }
}