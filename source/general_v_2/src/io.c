#include "io.h"
#include <avr/io.h>

#define LED_ON_DATA		0x00
#define LED_OFF_DATA	0xFF


void io_init(void)
{
	uint8_t mask,button_master_mask;
 	DDRB |= LED_PIN; // led is output

	mask = DIP0_PIN | DIP1_PIN | DIP2_PIN | DIP3_PIN;
	button_master_mask = BUT1_PIN | BUT2_PIN;
	
	DDRB |= mask; // DIP buttons are inputs, needed to set slave address
	PORTB |= mask; // pull-up DIP buttons
	
	// TODO check if correct port
	DDRB |= button_master_mask; // buttons are inputs, needed to control command send to slave
	PORTB |= button_master_mask; // pull-up control buttons
}

uint8_t read_address (void)
{
	// TODO: mod. for accepting other positions
	return (PINB & 0x0F);
}

void io_led_set (LED_state state)
{
	if (state == ON_LED)
		PORTB |= (uint8_t)LED_PIN;
	else
		PORTB &= ~(uint8_t)LED_PIN;
}

void io_parse_command (uint8_t data)
{
	if (data == LED_ON_DATA)
		io_led_set(ON_LED);
	else if (data == LED_OFF_DATA)
		io_led_set(OFF_LED);
}

uint8_t io_read_buttons(void)
{
	return PINB;
}

