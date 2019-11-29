#ifndef SERIAL_H
#define SERIAL_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define	BAUD 9600                   // Desired baud rate

typedef enum {
	master, // master transmits only
	slave // slave receives only
} NodeRole_t;

void usart_init(NodeRole_t role);
void USART_Transmit(uint16_t data);
uint16_t USART_Receive( void );

void RS485_init( NodeRole_t role );
void RS485_Transmit( uint8_t address, uint8_t data );
uint16_t RS485_Receive( void );


#endif /* SERIAL_H */