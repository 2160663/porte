/*
 * ATmega32_Slave.c
 * J�r�my
 *
 */ 


///////////// CODE SOURCE ///////////////
/*
#define F_CPU 8000000UL							// Define CPU clock Frequency e.g. here its 8MHz 
#include <avr/io.h>								// Include AVR std. library file 
#include <util/delay.h>							// Include inbuilt defined Delay header file 
#include <stdio.h>								// Include standard I/O header file 
#include <string.h>								// Include string header file 
#include "LCD_16x2_H_file.h"					// Include LCD header file 
#include "I2C_Slave_H_File.h"					// Include I2C slave header file 
#include <avr/interrupt.h>						// Biblioth�que pour g�rer les interruptions


#define Slave_Address			0x20
#define LED_PIN PB0 // Utilisez la broche PB0 pour la LED (personnalisez selon votre configuration mat�rielle)


void initLED() {
	// Configure la broche de la LED comme sortie
	DDRB |= (1 << LED_PIN);
}

void turnOnLED() {
	// Allume la LED en mettant la broche � l'�tat bas (0)
	PORTB &= ~(1 << LED_PIN);
}

void turnOffLED() {
	// �teint la LED en mettant la broche � l'�tat haut (1)
	PORTB |= (1 << LED_PIN);
}


int main(void)
{
	char buffer[10];
	int8_t count = 0;
	
	LCD_Init();
	I2C_Slave_Init(Slave_Address);
	initLED(); // Initialise la LED

    // Activer les interruptions
    sei();
	
	LCD_String_xy(1, 0, "Slave Device");
	
	while (1)
	{
		switch(I2C_Slave_Listen())				// Check for any SLA+W or SLA+R 
		{
			case 0:
			{
				LCD_String_xy(2, 0, "Receiving :       ");
				do
				{
					sprintf(buffer, "%d    ", count);
					LCD_String_xy(2, 13, buffer);
					count = I2C_Slave_Receive();		// Receive data byte
				} while (count != -1);					// Receive until STOP/REPEATED START received 
				
				// Allume la LED lors de la r�ception des donn�es
				turnOnLED();
				_delay_ms(100);
				turnOffLED();
				_delay_ms(1000);
				
				count = 0;
				break;
			}
			case 1:
			{
				int8_t Ack_status;
				LCD_String_xy(2, 0, "Sending :       ");
				do
				{
					Ack_status = I2C_Slave_Transmit(count);	// Send data byte 
					sprintf(buffer, "%d    ",count);
					LCD_String_xy(2, 13, buffer);
					count++;
				} while (Ack_status == 0);		// Send until Acknowledgment is received 
				break;
			}
			default:
				// Aucune donn�e re�ue, �teignez la LED
				turnOffLED();
				_delay_ms(100);
				break;
		}
	}
}
*/
				
////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////// CODE SALVE FONCTIONNEL/////////////////// 


#define F_CPU 8000000UL              // Define CPU clock Frequency e.g. here it's 8MHz 
#include <avr/io.h>                  // Include AVR std. library file 
#include <util/delay.h>              // Include inbuilt defined Delay header file 
#include "I2C_Slave_H_File.h"        // Include I2C slave header file 
#include <avr/interrupt.h>           // Biblioth�que pour g�rer les interruptions

#define Slave_Address    0x20
#define LED_PIN PB0 // Utilisez la broche PB0 pour la LED (personnalisez selon votre configuration mat�rielle)

void initLED() {
	// Configure la broche de la LED comme sortie
	DDRB |= (1 << LED_PIN);
}

void turnOnLED() {
	// Allume la LED en mettant la broche � l'�tat bas (0)
	PORTB &= ~(1 << LED_PIN);
}

void turnOffLED() {
	// �teint la LED en mettant la broche � l'�tat haut (1)
	PORTB |= (1 << LED_PIN);
}

int main(void) {
	initLED(); // Initialise la LED
	I2C_Slave_Init(Slave_Address);
	// Activer les interruptions
	sei();

	while (1) {
		switch (I2C_Slave_Listen()) {
			case 0: {
				// R�ception de donn�es
				int8_t count = 0;

				do {
					count = I2C_Slave_Receive();				 // Receive data byte 
					} while (count != -1);						// Receive until STOP/REPEATED START received 

					// Allumer la LED lors de la r�ception
					turnOnLED();
					_delay_ms(100);					// Allum�e pendant 0.1 seconde
					turnOffLED();
					break;
				}
				case 1: {
					int8_t Ack_status;

					do {
						Ack_status = I2C_Slave_Transmit(0);					// Send 0 as acknowledgment 
						} while (Ack_status == 0);				// Send until Acknowledgment is received 
						break;
					}
					default:
					// Aucune donn�e re�ue, �teignez la LED
					turnOffLED();
					break;
				}
			}
		}



///////////////////////////////////////////////////////////////////////////////
//----------------------------- PAS FONCTIONNEL --------------------------
/*
#define F_CPU 8000000UL              // Define CPU clock Frequency e.g. here it's 8MHz
#include <avr/io.h>                  // Include AVR std. library file
#include <util/delay.h>              // Include inbuilt defined Delay header file
#include "I2C_Slave_H_File.h"        // Include I2C slave header file
#include <avr/interrupt.h>           // Biblioth�que pour g�rer les interruptions

#define Slave_Address    0x20
#define LED_PIN PB0 // Utilisez la broche PB0 pour la LED (personnalisez selon votre configuration mat�rielle)

// Drapeau (flag) pour indiquer si des donn�es ont �t� re�ues
volatile uint8_t dataReceived = 0;

void initLED() {
	// Configure la broche de la LED comme sortie
	DDRB |= (1 << LED_PIN);
}

void turnOnLED() {
	// Allume la LED en mettant la broche � l'�tat bas (0)
	PORTB &= ~(1 << LED_PIN);
}

void turnOffLED() {
	// �teint la LED en mettant la broche � l'�tat haut (1)
	PORTB |= (1 << LED_PIN);
}

int main(void) {
	initLED(); // Initialise la LED
	I2C_Slave_Init(Slave_Address);
	// Activer les interruptions
	sei();

	while (1) {
		switch (I2C_Slave_Listen()) {
			case 0: {
				// R�ception de donn�es
				int8_t count = 0;

				do {
					count = I2C_Slave_Receive();              // Receive data byte
				} while (count != -1);                        // Receive until STOP/REPEATED START received

				// Allumer la LED lors de la r�ception
				turnOnLED();
				//_delay_ms(100);                // Allum�e pendant 0.1 seconde
				//turnOffLED();

				// D�finir le drapeau pour indiquer que des donn�es ont �t� re�ues
				dataReceived = 1;
				break;
			}
			case 1: {
				int8_t Ack_status;

				do {
					Ack_status = I2C_Slave_Transmit(0);                // Send 0 as acknowledgment
				} while (Ack_status == 0);            // Send until Acknowledgment is received
				break;
			}
			default:
			// Aucune donn�e re�ue, �teignez la LED si aucun donn�es n'ont �t� re�ues
			if (!dataReceived) {
				turnOffLED();
			}
			// R�initialiser le drapeau de r�ception de donn�es
			dataReceived = 0;
			break;
		}
	}
}
*/