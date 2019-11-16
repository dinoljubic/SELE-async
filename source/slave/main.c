#include "serial.h"
#include <avr/interrupt.h>
#include <inttypes.h>

uint8_t own_address;

int main (void)
{
	// initialize
	io_init();
	own_address = read_address(); // set slave address
	usart_init(slave); 
	sei(); // set global interrupts
	
	while(1){
		// do nothing
	}
}

/* ISR with alternate version - 2nd is not tested */
/*
ISR (USART_RX_vect)        //interrupt if recive data 
{
	
	unsigned char resh, resl;
	static uint8_t is_it_my_address = 0;// wait for this address
	
	resh = UCSR0B;
	resl = UDR0;

	resh = (resh >> 1) & 0x01;

   if (is_it_my_address == 0) // Go there if slave wait for address
   {
	   	if (resh == 1)
		{ 
			if (resl == slave_address) // if 9th bit 0 and address == slave address
			{
				is_it_my_address = 1; // if it is slave address read data (point below)
			}
		}	
   }
   else if (is_it_my_address == 1)	// if it is slave address read data
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
			is_it_my_address = 0; // wait for my address
		}	
   }
}
*/

ISR (USART_RX_vect)        //interrupt if recive data 
{
	// receive both frames
	// TODO: use RS485_Receive()
	
	
	
}

