#include "serial.h"
#include "slave_handles.h"

#define LED_ON_DATA		0x00
#define LED_OFF_DATA	0xFF

void Slave_SetLED (uint8_t address, PinState_t state)
{
	uint8_t led_data;

	switch (state){
	case ON:
		led_data = LED_ON_DATA;
		break;
	case OFF:
		led_data = LED_OFF_DATA;
		break;
	}
	
	Slave_Send(address, led_data);
}

void Slave_Send (uint8_t address, uint8_t data)
{	
	// transmit data frame
	RS485_Transmit(address, data);
}

// TODO : this function shouldn't be in io - find better place
void Slave_ParseCommand (uint8_t data)
{
	if (data == LED_ON_DATA)
		io_led_set(ON);
	else if (data == LED_OFF_DATA)
		io_led_set(OFF);
}

