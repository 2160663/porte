/*
 * ATmega32_Slave.c
 * http://www.electronicwings.com
 *
 */ 


#define F_CPU 8000000UL							/* Define CPU clock Frequency e.g. here its 8MHz */
#include <avr/io.h>								/* Include AVR std. library file */
#include <util/delay.h>							/* Include inbuilt defined Delay header file */
#include <stdio.h>								/* Include standard I/O header file */
#include <string.h>								/* Include string header file */
#include "LCD_16x2_H_file.h"					/* Include LCD header file */
#include "I2C_Slave_H_File.h"					/* Include I2C slave header file */
#include <avr/interrupt.h>						// Bibliothèque pour gérer les interruptions


#define Slave_Address			0x20
#define LED_PIN PB0 // Utilisez la broche PB0 pour la LED (personnalisez selon votre configuration matérielle)


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
		switch(I2C_Slave_Listen())				/* Check for any SLA+W or SLA+R */
		{
			case 0:
			{
				LCD_String_xy(2, 0, "Receiving :       ");
				do
				{
					sprintf(buffer, "%d    ", count);
					LCD_String_xy(2, 13, buffer);
					count = I2C_Slave_Receive();/* Receive data byte*/
				} while (count != -1);			/* Receive until STOP/REPEATED START received */
				
				// Allume la LED lors de la réception des données
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
					Ack_status = I2C_Slave_Transmit(count);	/* Send data byte */
					sprintf(buffer, "%d    ",count);
					LCD_String_xy(2, 13, buffer);
					count++;
				} while (Ack_status == 0);		/* Send until Acknowledgment is received */
				break;
			}
			default:
				// Aucune donnée reçue, éteignez la LED
				turnOffLED();
				_delay_ms(100);
				break;
		}
	}
}

