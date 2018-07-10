
/*
	SPI en ATMEGA328P
	Licencia:
	Autor: Gonzalo Fernandez
 */ 

#include <stdio.h>
#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 16000000
#endif

void SPI_init(uint8_t modoCpolCpha,uint8_t prescaler);
uint8_t SPI_transfer(uint8_t dato);
void SPI_slaveInit(void);
uint8_t SPI_slaveTransfer(uint8_t dato);
void SPI_USART_init( uint32_t baud, uint8_t modo);
uint8_t SPI_USART_transfer( uint8_t valor );