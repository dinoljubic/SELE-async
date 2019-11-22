#include "slave_handles.h"
#include "serial.h"
#include "avr/delay.h"
#include "io.h"
#include <avr/interrupt.h>
#include <inttypes.h>

#define MASTER_ADDR 15
#define SLAVE2_ADDR 14
#define SLAVE1_ADDR 13

uint8_t own_address;

ISR (USART_RX_vect)        //interrupt if recive data 
{
	uint16_t frame;

	frame = USART_Receive();
		
	if ((frame & 0x100) != 0) // If waiting for and received address frame
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
	else // wait for a data frame
	{
		io_parse_command( (uint8_t) (frame&0xff) ); 
	}
}

int main() {
	
// initialize
io_init();
own_address = read_address(); // set slave address

if (own_address == MASTER_ADDR)
{
	RS485_init(master);
	uint8_t BUTTON_STATE = 0,BUTTON_STATE_PREV = 0;
	
	while (1)
	{	  
		BUTTON_STATE = io_read_buttons();
		
		if ((~BUTTON_STATE & BUT1_PIN) & (BUTTON_STATE_PREV & BUT1_PIN))
		  {
			Slave_Send_Address (SLAVE1_ADDR);
			Slave_SetLED (ON);
		  }
		else if ((BUTTON_STATE & BUT1_PIN) & (~BUTTON_STATE_PREV & BUT1_PIN))
		  {
			Slave_SetLED (ON);
		  }
		  
		if ((~BUTTON_STATE & BUT1_PIN) & (BUTTON_STATE_PREV & BUT1_PIN))
		  {
			Slave_Send_Address (SLAVE2_ADDR);
			Slave_SetLED (ON);
		  }
		else if ((BUTTON_STATE & BUT1_PIN) & (~BUTTON_STATE_PREV & BUT1_PIN))
		  {
			Slave_SetLED (ON);
		  }
		  

	BUTTON_STATE_PREV = BUTTON_STATE;
	}
}
else 
{
	RS485_init(slave);
	sei(); 
	
	while (1) 
	{}
}	




return 0;
}