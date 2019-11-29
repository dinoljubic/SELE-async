#include "serial.h"
#include <avr/io.h>
#include <util/delay.h>
#include "slave_handles.h"

/*	Pin definition for Driver Enable pin of the MAX485 transciever.
	High level enables line driver (transmiting) 					*/
// XXX: Pin is defined for port pD
#define WREN_N			2
#define WREN_PIN		(1<<WREN_N)

//#define BAUDGEN ((16000000/(16*BAUD))-1)  // Calculated divider (overflows - precalculated)
#define BAUDGEN 103

extern uint8_t own_address;
uint8_t last_addressed;

void usart_init(NodeRole_t role) 
{
	if (role == master){
		// Master enables only transmit
		UBRR0 = BAUDGEN;
		//UBRR0H = (uint8_t)(BAUDGEN >> 8);
		//UBRR0L = (uint8_t)(BAUDGEN&0xff);
		//UCSR0A = (1 << MPCM0);
		UCSR0B = (1 << TXEN0) | (1 << UCSZ02); // enable  transmit
		UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);// 1 stop bits USBS0, 9 bit mode  UCSZ0
	}
	else {
		// Slave enables only Rx (with interrupt)
		UBRR0 = BAUDGEN;
		UCSR0A |= (1 << MPCM0);
		UCSR0B = (1 << RXEN0) | (1 << RXCIE0) | (1 << UCSZ02); // enable recive , enable recive interrupt, 9 bit mode
		UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); // 2 stop bits USBS0, 9 bit mode 7 << UCSZ00,
	}
}


void USART_Transmit(uint16_t data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	/* Copy 9th bit to TXB8 */
	UCSR0B &= ~(1<<TXB80);
	if (data & 0x0100)
		UCSR0B |= (1<<TXB80);
	/* Put data into buffer, sends the data */
	UDR0 = data;
}


uint16_t USART_Receive( void )
{
	uint8_t status, resh, resl;
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get status and 9th bit, then data */
	/* from buffer */
	status = UCSR0A;
	resh = UCSR0B;
	resl = UDR0;
	/* If error, return -1 */
	if (status & ((1<<FE0) | (1<<DOR0) | (1<<UPE0)))
		{return -1;}
	/* Filter the 9th bit, then return */
	resh = (resh >> 1) & 0x01;
	return ((resh << 8) | resl);
}


void RS485_init( NodeRole_t role )
{
	// Set or clear TXEN pin accordingly
	DDRD |= WREN_PIN; // Set to output
	if (role == master){
		PORTD |= (uint8_t)WREN_PIN;
		last_addressed = 0xff;
	}
	else{
		PORTD &= ~(uint8_t)WREN_PIN;
	}
	usart_init(role);
}


void RS485_Transmit( uint8_t address, uint8_t data )
{
	// If node was previously addressed, send only data.
	if (address == last_addressed)
		USART_Transmit( data );
	else
	{
		// Otherwise, send both address and data frame
		USART_Transmit((1 << 8) | address ); // Address frame
		USART_Transmit((0 << 8) | data ); // Data frame
		last_addressed = address;
	}
	
}


// FFU
uint16_t RS485_Receive( void )
{
	return USART_Receive(  );
}


ISR (USART_RX_vect)        //interrupt if recive data 
{
	uint16_t frame;

	frame = USART_Receive();

	if ((frame & 0x100) != 0) // If received an address frame
	{
			if ((frame & 0xff) == own_address)
			{
				UCSR0A &= ~(1 << MPCM0); //turn off Multiprocessor mode
			}
			else 
			{
				UCSR0A |= (1 << MPCM0); //turn on Multiprocessor mode
			}
	}
	else // Received a data frame
	{
		Slave_ParseCommand( (uint8_t) (frame&0xff) ); 
	}
}

