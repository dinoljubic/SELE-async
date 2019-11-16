#include "io.h"
#include <avr/io.h>

/*	LED and DIP switch pins are defined for specific
	IO ports; LED on pB, DIP on pC. This can be
	generalized, but is not necessary in this context.	*/

#define LED_N			0

/*	Solution is hardcoded for these values of DIP pin #
	For other values, modify read_address function		*/
#define	DIP0_N			0
#define	DIP1_N			1
#define	DIP2_N			2
#define	DIP3_N			3

/* Pin position defines - do not modify */
#define DIP0_PIN		(1<<DIP0_N)
#define DIP1_PIN		(1<<DIP1_N)
#define DIP2_PIN		(1<<DIP2_N)
#define DIP3_PIN		(1<<DIP3_N)
#define LED_PIN			(1<<LED_N)

#define LED_ON_DATA		0x00
#define LED_OFF_DATA	0xFF


void io_init(void)
{
	uint8_t mask;
 	DDRB |= LED_PIN; // led is output

	mask = DIP0_PIN | DIP1_PIN | DIP2_PIN | DIP3_PIN;
	
	DDRC |= mask; // DIP buttons are inputs, needed to set slave address
	PORTC |= mask; // pull-up DIP buttons
}

uint8_t read_address (void)
{
	// TODO: mod. for accepting other positions
	return (PINC & 0x0F);
}

void led_set (LED_state state)
{
	if (state == ON)
		PORTB |= (uint8_t)LED_PIN;
	else
		PORTB &= ~(uint8_t)LED_PIN;
}
