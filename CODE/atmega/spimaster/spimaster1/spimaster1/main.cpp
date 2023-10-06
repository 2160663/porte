/*
 * spimaster1.cpp
 *
 * Created: 2023-09-08 15:33:28
 * Author : 2160663
*/  

/*
#define  F_CPU 8000000UL
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

void SPI_MasterInit(void)
{
	// Set MOSI and SCK output SS,  
	DDRB |= (1<<PB3)|(1<<PB5)|(1<<PB1)|(1<<PB2);
	// Enable SPI, Master, set clock rate fck/64 
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR1);
}
void SPI_MasterTransmit(char cData)
{
	// Start transmission 
	SPDR = cData;
	// Wait for transmission complete 
	while(!(SPSR & (1<<SPIF)));
}

int main(void)
{   uint8_t donnees;
	//Bouton Poussoir sur PC0, Activer r�sistance de Rappel
	PORTB |=(1<<PB1);
	PORTB |=(1<<PB2);
	SPI_MasterInit();//Initialisation SPI Master
	
	PORTB|=(1<<PB1); //SS=1
	
	while (1)
	
	{
		//lire l'etat du bouton poussoir
		
		if (bit_is_clear(PINB,PB1))
		donnees=0x01;//si bouton appuy� envoy� 01
		else
		donnees=0x00;//si non envoy� 00
		
		PORTB &= ~(1<<PB1);//Activ� Slave SS=0
		_delay_us(5);
		SPI_MasterTransmit(donnees); //Transmettre la donnee
		_delay_us(5);
		
		PORTB|=(1<<PB1);//D�sactiv� Slave SS=1
		
		
		
		
		
		PORTB &= ~(1<<PB2);//Activ� Slave SS=0
		_delay_us(5);
		SPI_MasterTransmit(donnees); //Transmettre la donnee
		_delay_us(5);
		
		PORTB|=(1<<PB2);//D�sactiv� Slave SS=1
		
	}
}
*/


/////////////////////////////////////////////////////////////////////////


#define  F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SLAVE_ADDRESS 0x02

void SPI_MasterInit(void)
{
	/* Configure les broches SS, MOSI et SCK en tant que sorties */
	DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB5);

	/* Active le SPI, configure-le en mode ma�tre et d�finis la fr�quence de l'horloge � fck/16 */
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_SelectSlave(uint8_t address)
{
	/* D�sactive tous les esclaves */
	PORTB |= (1 << PB2) | (1 << PB3) | (1 << PB5);

	/* S�lectionne l'esclave en fonction de son adresse */
	switch (address)
	{
		case 0x02:
		PORTB &= ~(1 << PB2); // Activer Slave SS=0 (adresse de l'esclave)
		break;
		// Ajoutez d'autres cas pour d'autres esclaves si n�cessaire
	}
}

void SPI_Transmit(uint8_t data)
{
	/* Transmettre des donn�es via SPI */
	SPDR = data;

	/* Attendre la fin de la transmission */
	while (!(SPSR & (1 << SPIF)));
}

int main(void)
{
	 uint8_t donnees;

	 // Bouton Poussoir sur PC0, Activer r�sistance de Rappel
	 DDRC &= ~(1 << PC0); // PC0 comme entr�e
	 PORTC |= (1 << PC0); // Activer la r�sistance de rappel
	 
	SPI_MasterInit();

	while (1)
	{
		
		if (bit_is_clear(PINC, PC0)){
		donnees = 0x01; // Si bouton appuy�, envoyer 0x01
		}
		
		else{
		donnees = 0x00; // Sinon, envoyer 0x00
		}
		
		// S�lectionner l'esclave en utilisant son adresse (0x02 dans cet exemple)
		SPI_SelectSlave(SLAVE_ADDRESS);
		PORTB &= ~(1 << PB2);
		_delay_us(5);

		// Envoyer des donn�es � l'esclave
		//uint8_t sendData = 0xAA; // Exemple de donn�es � envoyer
		SPI_Transmit(donnees);
		_delay_us(5);

		// D�s�lectionner l'esclave
		PORTB |= (1 << PB2); // D�sactiver Slave SS=1

		_delay_ms(1000); // Attendre avant d'envoyer plus de donn�es
	}

	return 0;
}
