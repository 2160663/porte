/*
 * ATmega16_Master_I2C.c
 * Jérémy
 */ 


///////// CODE SOURCE////////////

/*
#define F_CPU 8000000UL						// Define CPU clock Frequency e.g. here its 8MHz 
#include <avr/io.h>							// Include AVR std. library file 
#include <util/delay.h>						// Include inbuilt defined Delay header file 
#include <string.h>							// Include string header file 
#include "I2C_Master_H_file.h"				// Include I2C header file 
#include "LCD_16x2_H_file.h"				// Include LCD header file 

#define EEPROM_Write_Addess		0xA0
#define EEPROM_Read_Addess		0xA1

#define LED_PIN PB0 // Utilisez la broche PB0 pour la LED

void initLED() {
	// Configure la broche de la LED comme sortie
	DDRB |= (1 << LED_PIN);
}

void turnOnLED() {
	// Allume la LED en mettant la broche à l'état bas (0)
	PORTB &= ~(1 << LED_PIN);
}

void turnOffLED() {
	// Éteint la LED en mettant la broche à l'état haut (1)
	PORTB |= (1 << LED_PIN);
}

int main(void)
{
	char array[10] = "test";				// Declare array to be print 
	LCD_Init();								// Initialize LCD 
	I2C_Init();								// Initialize I2C 
	initLED();								// Initialise la LED
	
	I2C_Start(EEPROM_Write_Addess);			// Start I2C with device write address 
	I2C_Write(0x00);						// Write start memory address for data write 
	for (int i = 0; i<strlen(array); i++)	// Write array data 
		{
			I2C_Write(array[i]); 
		}
	I2C_Stop();								// Stop I2C 
	_delay_ms(10);
	I2C_Start(EEPROM_Write_Addess);			// Start I2C with device write address 
	I2C_Write(0x00);						// Write start memory address 
	I2C_Repeated_Start(EEPROM_Read_Addess);	// Repeat start I2C with device read address 
	
	for (int i = 0; i<strlen(array); i++)	// Read data with acknowledgment(ack) 
		{
			char receivedData = I2C_Read_Ack();
			LCD_Char(receivedData);
			
			if (receivedData == 't') {
				// Si un caractère spécifique est envoyé (par exemple, 't'), allume la LED
				turnOnLED();
			}
		}
	I2C_Read_Nack();						// Read flush data with nack 
	I2C_Stop();								// Stop I2C 
	
	turnOffLED();
	
	return 0;
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////// CODE TRANSMET DATA TOUTES LES 1SEC ET SLAVE RECOIT ////////////////////

/*
#define F_CPU 8000000UL                  // Define CPU clock Frequency e.g. here its 8MHz 
#include <avr/io.h>                      // Include AVR std. library file 
#include <util/delay.h>                  // Include inbuilt defined Delay header file 
#include "I2C_Master_H_file.h"           // Include I2C header file 

#define EEPROM_Write_Addess     0x20
#define LED_PIN PB0 // Utilisez la broche PB0 pour la LED

void initLED() {
	// Configure la broche de la LED comme sortie
	DDRB |= (1 << LED_PIN);
}

void turnOnLED() {
	// Allume la LED en mettant la broche à l'état bas (0)
	PORTB &= ~(1 << LED_PIN);
}

void turnOffLED() {
	// Éteint la LED en mettant la broche à l'état haut (1)
	PORTB |= (1 << LED_PIN);
}

int main(void) {
	char dataToSend = 'A'; // Donnée à envoyer

	I2C_Init();                         // Initialize I2C 
	initLED();                          // Initialise la LED

	while (1) {
		// Envoie la donnée via I2C
		I2C_Start(EEPROM_Write_Addess); // Start I2C with device write address 
		I2C_Write(0x00);               // Write start memory address for data write 
		I2C_Write(dataToSend);         // Envoie la donnée 
		I2C_Stop();                    // Stop I2C 

		// Inverse la donnée pour l'envoi suivant
		if (dataToSend == 'A') {
			dataToSend = 'B';
			} else {
			dataToSend = 'A';
		}

		// Allume la LED pendant une courte période
		turnOnLED();
		_delay_ms(100);
		turnOffLED();

		// Attendez une seconde avant d'envoyer la prochaine donnée
		_delay_ms(1000);
	}

	return 0;
}
*/
		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////// CODE AVEC CAPTEUR FENETRE (PAS SUR QUI FONCTIONNE) ///////////////////

#define F_CPU 8000000UL                  /* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>                      /* Include AVR std. library file */
#include <util/delay.h>                  /* Include inbuilt defined Delay header file */
#include "I2C_Master_H_file.h"           /* Include I2C header file */

#define EEPROM_Write_Addess     0x20
#define LED_PIN PB0 // Utilisez la broche PB0 pour la LED
#define SENSOR_PIN PC0 // Utilisez la broche PC0 pour le capteur de fenêtre

void initLED() {
	// Configure la broche de la LED comme sortie
	DDRB |= (1 << LED_PIN);
}

int isWindowOpen() {
	// Lisez l'état du capteur de fenêtre (ouvert ou fermé)
	if (PINC & (1 << SENSOR_PIN)) {
		return 1; // Fenêtre ouverte
		} else {
		return 0; // Fenêtre fermée
	}
}

void turnOnLED() {
	// Allume la LED en mettant la broche à l'état bas (0)
	PORTB &= ~(1 << LED_PIN);
}

void turnOffLED() {
	// Éteint la LED en mettant la broche à l'état haut (1)
	PORTB |= (1 << LED_PIN);
}

int main(void) {
	char dataToSend = 'A'; // Donnée à envoyer

	I2C_Init();                         /* Initialize I2C */
	initLED();                          // Initialise la LED

	// Configure la broche du capteur de fenêtre comme entrée
	DDRC &= ~(1 << SENSOR_PIN);
	PORTC |= (1 << SENSOR_PIN); // Activer la résistance de rappel interne

	while (1) {
		// Vérifiez l'état du capteur de fenêtre
		if (isWindowOpen()) {
			// Envoie la donnée via I2C
			I2C_Start(EEPROM_Write_Addess); /* Start I2C with device write address */
			I2C_Write(0x00);               /* Write start memory address for data write */
			I2C_Write(dataToSend);         /* Envoie la donnée */
			I2C_Stop();                    /* Stop I2C */

			// Inverse la donnée pour l'envoi suivant
			if (dataToSend == 'A') {
				dataToSend = 'B';
				} else {
				dataToSend = 'A';
			}

			// Allume la LED pendant une courte période
			turnOnLED();
			_delay_ms(100);
			turnOffLED();
		}

		// Attendez une seconde avant de vérifier à nouveau le capteur de fenêtre
		_delay_ms(1000);
	}

	return 0;
}
