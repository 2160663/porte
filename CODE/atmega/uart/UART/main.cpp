/*
 * UART.cpp
 *
 * Created: 2023-10-12 14:36:03
 * Author : 2140794
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init(unsigned int ubrr) {
	// Configuration du registre de baud
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	// Activation de la r�ception et de la transmission
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Configuration du format de trame : 8 bits de donn�es, 1 bit de stop
	UCSR0C = (3 << UCSZ00);
}

void USART_Transmit(unsigned char data) {
	// Attendre que le tampon de transmission soit vide
	while (!(UCSR0A & (1 << UDRE0))) {}
	// Charger les donn�es � transmettre dans le tampon de transmission
	UDR0 = data;
}

int main(void)
{
    // Initialisation de la communication UART
    USART_Init(MYUBRR);
    // Configuration des E/S pour le moniteur s�rie
    stdout = &uart_output;
    stdin = &uart_input;
	
    while (1) 
	{
		// Attendre la r�ception de donn�es
		while (!(UCSR0A & (1 << RXC0))) {}
		// Lire les donn�es re�ues du tampon de r�ception
		unsigned char data = UDR0;
		// Afficher les donn�es sur le moniteur s�rie
		printf("Donn�es re�ues : %c\n", data);
    }
	
	return 0;
}

