#include "slave_handles.h"
#include "serial.h"
#include "avr/delay.h"
#include "io.h"

#define MASTER_ADDR 15
#define SLAVE2_ADDR 14
#define SLAVE1_ADDR 13

uint8_t own_address;

int main() {
		
	// initialize
	io_init();
	own_address = read_address(); // set slave address

	if (own_address == MASTER_ADDR)
	{
		
		RS485_init(master);
		//mainMaster();
		SlaveDevice_t devices[2] = {{SLAVE1_ADDR, OFF, BUT1_PIN}, {SLAVE2_ADDR, OFF, BUT2_PIN}};
		PinState_t buttonState;

		while(1) // Loop forever
		{
			for (uint8_t i=0;i<2;i++) // Iterate through slave devices
			{ 
				buttonState = (PinState_t) io_read_button(devices[i].button_pin);
				if (buttonState != devices[i].state){
					Slave_SetLED(devices[i].address, buttonState);
					devices[i].state = buttonState;
				}
			}
		}
	}
		
	else 
	{
		RS485_init(slave);
		sei(); 
		
		while (1) 
		{
			// Do nothing
		}
	}	

	return 0;
}

/*
void mainMaster(){
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
*/

