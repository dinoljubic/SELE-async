#include "serial.h"
#include "slave_handles.h"

#define LED_ON_DATA		0x00
#define LED_OFF_DATA	0xFF

void Slave_SetLED (uint8_t address, LED_State_t state)
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
	
	Slave_SendData(address, led_data);
}

void Slave_SendData (uint8_t address, uint8_t data)
{
	// transmit address frame
	RS485_Transmit((1 << 8) | data);
	// transmit data frame
	RS485_Transmit((0 << 8) | data);
}

