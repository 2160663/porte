/*
 * masteri2c.cpp
 *
 * Created: 2023-09-08 15:08:53
 * Author : 2160663
 */ 
/*
#include <avr/io.h>
#include <util/twi.h>
// Define a macro to set a specific bit in a register
#define SET_BIT(reg, bit) (reg |= (1 << bit))
// Define a macro to clear a specific bit in a register
#define CLEAR_BIT(reg, bit) (reg &= ~(1 << bit))
#include <util/delay.h>      // librairie pour utiliser les fonctions delay
#include <avr/sfr_defs.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define TWI_FREQ 100000 // Fréquence de l'I2C (100 kHz dans cet exemple)

void initI2C() {
	// Initialisation de la vitesse de transmission de l'I2C
	TWSR = 0;
	TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;
	
	// Activer l'I2C
	TWCR = (1 << TWEN);
}

void I2C_start() {
	// Envoyer un signal de démarrage
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	
	// Attendre que l'opération soit terminée
	while (!(TWCR & (1 << TWINT)));
}

void I2C_stop() {
	// Envoyer un signal d'arrêt
	TWCR = (1 << TWSTO) | (1 << TWINT) | (1 << TWEN);
}

void I2C_write(uint8_t data) {
	// Charger les données dans le registre de données
	TWDR = data;
	
	// Lancer la transmission
	TWCR = (1 << TWEN) | (1 << TWINT);
	
	// Attendre que l'opération soit terminée
	while (!(TWCR & (1 << TWINT)));
}

int main() {
	// Initialize I2C
	initI2C();
	
	int fenetreouvert = 0;
	
	// Configure PD2 as an input
	CLEAR_BIT(DDRD, PD2); // Clear PD2 to set it as an input
	
	while (1) {
		// Clear PD2 (set it low)
		CLEAR_BIT(PORTD, PD2);
		
		// Commencer la communication I2C en envoyant l'adresse de l'esclave avec le bit R/W mis à zéro (écriture)
		I2C_start();
		I2C_write(0x10); // Adresse de l'esclave

		// Envoyer des données à l'esclave (fenetreouvert)
		I2C_write(fenetreouvert);

		// Terminer la communication I2C
		I2C_stop();

		// Attendre un certain temps ou effectuer d'autres actions si nécessaire
	}
	
	return 0;
}
*/

//////////////////////////////////////////////////////////////////////////

/*
 * ATmega16_Master_I2C.c
 * http://www.electronicwings.com
 */ 


#define F_CPU 8000000UL		/* Define CPU clock Frequency 8MHz */
#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>		/* Include Delay header file */
#include <string.h>		/* Include string header file */
#include <util/twi.h>
#include <avr/sfr_defs.h>
#include "I2C_Master_H_file.h"	/* Include I2C header file */
#include "LCD_16x2_H_file.h"	/* Include LCD header file */

#define EEPROM_Write_Addess		0xA0
#define EEPROM_Read_Addess		0xA1

int main(void)
{
    char array[10] = "test";	/* Declare array to be print */
    LCD_Init();			/* Initialize LCD */
    I2C_Init();			/* Initialize I2C */
    I2C_Start(EEPROM_Write_Addess);/* Start I2C with device write address */
    I2C_Write(0x00);		/* Write start memory address for data write */
    for (int i = 0; i<strlen(array); i++)/* Write array data */
	{
		I2C_Write(array[i]); 
	}
    I2C_Stop();			/* Stop I2C */
    _delay_ms(10);
    I2C_Start(EEPROM_Write_Addess);/* Start I2C with device write address */
    I2C_Write(0x00);		/* Write start memory address */
    I2C_Repeated_Start(EEPROM_Read_Addess);/* Repeat start I2C SLA+R */
    for (int i = 0; i<strlen(array); i++)/* Read data with acknowledgment */
	{
		LCD_Char(I2C_Read_Ack());
	}
    I2C_Read_Nack();		/* Read flush data with nack */
    I2C_Stop();			/* Stop I2C */
    return 0;
}