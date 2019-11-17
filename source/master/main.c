#include "slave_handles.h"
#include "serial.h"
#include "avr/delay.h"

#define SLAVE_NUM		255
#define SEQ_DELAY_MS	500
#define TOGGLE_DELAY_MS	1000			

int main() {
	uint8_t address;
	LED_State state = OFF;
	
	RS485_init(master);
	
	while (1)	// Loop forever
	{
		for (address = 0; address < SLAVE_NUM; i++){
			Slave_SetLED(address, state);
			// Wait before setting state of next node
			_delay_ms(SEQ_DELAY_MS);
		}
		// Wait before toggling states
		_delay_ms(TOGGLE_DELAY_MS);
		state = ~state;
	}
	
	return 0;
}