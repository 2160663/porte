/*
 * spislave.cpp
 *
 * Created: 2023-09-08 15:30:37
 * Author : 2160663
 */ 

/*
#define  F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>

void SPI_SlaveInit(void)
{
	// Set MISO output, all others input 
	DDRB |= (1<<PB4);
	// Enable SPI 
	SPCR |= (1<<SPE);
}
char SPI_SlaveReceive(void)
{
	// Wait for reception complete 
	while(!(SPSR & (1<<SPIF)))
	;
	// Return Data Register 
	return SPDR;
}

int main(void)
{
	// Replace with your application code 
	uint8_t valeur;
	DDRB =0xFF;
	SPI_SlaveInit();
	
	while (1)
	{
		valeur=SPI_SlaveReceive();//lire la donnees du MASTER
		_delay_us(5);
		
		//tester le bit 0 de valeur
		if (bit_is_clear(valeur,0) ) {
			PORTB |=(1<<PB7); //bouton sur PC0 du Master  appuy�
		}
		else{
			
			PORTB &=~(1<<PB7); //bouton sur PC0 du Master non appuy�
		}
		
	}
}

*/

///////////////////////////////////////////////////////////////////////

#define  F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SLAVE_ADDRESS 0x02

void SPI_SlaveInit(void)
{
	/* Configure la broche MISO (PB4) en tant que sortie */
	DDRB |= (1 << PB4);

	/* Active le SPI en mode esclave */
	SPCR = (1 << SPE);
}

uint8_t SPI_Receive(void)
{
	/* Attendre la r�ception de donn�es via SPI */
	while (!(SPSR & (1 << SPIF)));

	/* Retourner les donn�es re�ues */
	return SPDR;
}

int main(void)
{
	SPI_SlaveInit();

	while (1)
	{
		// Attendre la s�lection de l'esclave par le ma�tre
		while (!(PINB & (1 << PB2))) {}

		// Recevoir des donn�es du ma�tre via SPI
		uint8_t receivedData = SPI_Receive();

		// Traiter les donn�es re�ues (exemple : allumer ou �teindre une LED)
		if (receivedData == 0x01)
		{
			// Allumer une LED connect�e � la broche PC0 (exemple)
			PORTC |= (1 << PC0);
		}
		else
		{
			// �teindre la LED
			PORTC &= ~(1 << PC0);
		}

		// D�s�lectionner l'esclave
		while (PINB & (1 << PB2)) {}
	}

	return 0;
}


