#include "serial.h"
#include <avr/interrupt.h>
#include <inttypes.h>

uint8_t own_address;

int main (void)
{
	// initialize
	io_init();
	own_address = read_address(); // set slave address
	RS485_init(slave); 
	sei(); // set global interrupts
	
	while(1){
		// do nothing
	}
}

// XXX: ISR with alternate version - 2nd is not tested
/*
ISR (USART_RX_vect)        //interrupt if recive data 
{
	
	unsigned char resh, resl;
	static uint8_t address_match = 0;
	
	resh = UCSR0B;
	resl = UDR0;

	resh = (resh >> 1) & 0x01;

   if (address_match == 0) // Go there if slave wait for address
   {
	   	if (resh == 1)
		{ 
			if (resl == slave_address) // if 9th bit 0 and address == slave address
			{
				address_match = 1; // if it is slave address read data (point below)
			}
		}	
   }
   else if (address_match == 1)	// if it is slave address read data
   {
		if ((resh == 0)) // if 9th bit 0
		{
			if (resl == slave_data_turn_on) // if command == turn on led
			{
				led_set (ON);	//turn on LED	
			}
			if (resl == slave_data_turn_off)// if command == turn off led
			{
				led_set (OFF);	//turn on LED
			}
			address_match = 0; // wait for my address
		}	
   }
}
*/

ISR (USART_RX_vect)        //interrupt if recive data 
{
	static uint8_t waiting_address = 1;
	uint16_t frame;
	
	// TODO: use RS485_Receive()
	frame = USART_Receive();
	
	if (waiting_address && (frame & 0x100) != 0) // If waiting for and received address frame
	{
		if (frame & 0xff == own_address)
			waiting_address = 0; // waiting for data
	}
	
	else if (!waiting_address) // if waiting for a data frame
	{
		io_parse_command( (uint8_t) (frame&0xff) ); 
 		waiting_address = 1; // Reset state
	}
	
	else // Spurious event; reset ISR behavior and wait for next address
	{
		waiting_address = 1; // Reset state
	}
}

