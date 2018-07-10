
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

#define DDR_SPI DDRB
#define DD_MOSI DDB3
#define DD_MISO DDB4
#define DD_SCK DDB5

void SPI_init(uint8_t modoCpolCpha,uint8_t prescaler)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
	/* Enable SPI, Master, set clock rate fck/16 */
	switch(modoCpolCpha){
		case 0: 	
			SPCR = (1<<SPE)|(1<<MSTR);
			break;
		case 1: 	
			SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL);
			break;
		case 2: 	
			SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA);
			break;
		case 3: 	
			SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA);
			break;
	}
	SPCR&=~(3<<SPR0);
	SPCR|=(prescaler&3)<<SPR0;
}

uint8_t SPI_transfer(uint8_t dato){
	/* Start transmission */
	SPDR = dato;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void SPI_slaveInit(void){
	/* Set MISO output, all others input */
	DDR_SPI = (1<<DD_MISO);
	/* Enable SPI */
	SPCR = (1<<SPE);
}
char SPI_slaveTransfer(uint8_t dato){
	/* Wait for reception complete */
	SPDR= dato;
	while(!(SPSR & (1<<SPIF)));
	/* Return Data Register */
	return SPDR;
}


void SPI_USART_init( uint32_t baud){
	
	uint32_t aux32=0;
	
	UBRR0 = 0;
	/* Setting the XCKn port pin as output, enables master 	mode. */
	DDRD|=(1<<4);		//PD4 es XCK0
	//XCK_DDR |= (1<<XCK);
	/* Set MSPI mode of operation and SPI data mode 0. */
	UCSR0C = (1<<UMSEL01)|(1<<UMSEL00)|(0<<UCPHA0)|(0<<UCPOL0);
	
	/* Enable receiver and transmitter. */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set baud rate. */
	/* IMPORTANT: The Baud Rate must be set after the transmitter is enabled */
	if (baud) aux32=F_CPU/2/baud-1; // evita div0
	if (aux32<65536) UBRR0 = (uint16_t) aux32;
}

// Función de transacción SPI. Lee y escribe simultáneamente

unsigned char SPI_USART_transfer( uint8_t valor ){
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = valor;
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}