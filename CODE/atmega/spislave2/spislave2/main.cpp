/*
 * spislave2.cpp
 *
 * Created: 2023-09-28 15:05:19
 * Author : 2160663
 */ 

#define  F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB |= (1<<PB4);
	/* Enable SPI */
	SPCR |= (1<<SPE);
}
char SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
	;
	/* Return Data Register */
	return SPDR;
}

int main(void)
{
	/* Replace with your application code */
	uint8_t valeur;
	DDRB =0xFF;
	SPI_SlaveInit();
	
	while (1)
	{
		valeur=SPI_SlaveReceive();//lire la donnees du MASTER
		_delay_us(5);
		
		//tester le bit 0 de valeur
		if (bit_is_clear(valeur,0) ) {
			PORTB |=(1<<PB0); //bouton sur PC0 du Master  appuy�
		}
		else{
			
			PORTB &=~(1<<PB0); //bouton sur PC0 du Master non appuy�
		}
		
	}
}

