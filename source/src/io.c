#include "io.h"
#include <avr/io.h>

void io_init(void)
{
	uint8_t mask,button_master_mask;
 	DDRB |= LED_PIN; // led is output

	mask = DIP0_PIN | DIP1_PIN | DIP2_PIN | DIP3_PIN;
	button_master_mask = BUT1_PIN | BUT2_PIN;
	
	DDRB |= mask; // DIP switches are inputs, needed to set slave address
	PORTB |= mask; // pull-up DIP
	
	DDRD |= button_master_mask; // buttons are inputs, needed to control command send to slave
	PORTD |= button_master_mask; // pull-up control buttons
}

uint8_t read_address (void)
{
	// TODO: mod. for accepting other positions
	return (PINB & 0x0F);
}

void io_led_set (PinState_t state)
{
	if (state == ON)
		PORTB |= (uint8_t)LED_PIN;
	else
		PORTB &= ~(uint8_t)LED_PIN;
}

uint8_t io_read_button(uint8_t pin)
{
	if ((PIND & pin) != 0)
		return 0;
	else
		return 1;
}


