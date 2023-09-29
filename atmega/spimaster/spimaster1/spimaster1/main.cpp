/*
 * spimaster1.cpp
 *
 * Created: 2023-09-08 15:33:28
 * Author : 2160663
 */ 

#define  F_CPU 8000000UL
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output SS,  */
	DDRB |= (1<<PB3)|(1<<PB5)|(1<<PB1)|(1<<PB2);
	/* Enable SPI, Master, set clock rate fck/64 */
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1);
}
void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}

int main(void)
{   uint8_t donnees;
	//Bouton Poussoir sur PC0, Activer résistance de Rappel
	PORTB |=(1<<PB1);
	PORTB |=(1<<PB2);
	SPI_MasterInit();//Initialisation SPI Master
	
	PORTB|=(1<<PB1); //SS=1
	
	while (1)
	
	{
		//lire l'etat du bouton poussoir
		
		if (bit_is_clear(PINB,PB1))
		donnees=0x01;//si bouton appuyé envoyé 01
		else
		donnees=0x00;//si non envoyé 00
		
		PORTB &= ~(1<<PB1);//Activé Slave SS=0
		_delay_us(5);
		SPI_MasterTransmit(donnees); //Transmettre la donnee
		_delay_us(5);
		
		PORTB|=(1<<PB1);//Désactivé Slave SS=1
		
		
		
		
		
		PORTB &= ~(1<<PB2);//Activé Slave SS=0
		_delay_us(5);
		SPI_MasterTransmit(donnees); //Transmettre la donnee
		_delay_us(5);
		
		PORTB|=(1<<PB2);//Désactivé Slave SS=1
		
	}
}
