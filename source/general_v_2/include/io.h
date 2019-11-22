#ifndef IO_H
#define IO_H

#include "avr/io.h"

/*	LED and DIP switch pins are defined for specific
	IO ports; LED on pB, DIP on pB. This can be
	generalized, but is not necessary in this context.	*/

#define LED_N			5

/*	Solution is hardcoded for these values of DIP pin #
	For other values, modify read_address function		*/
#define	DIP0_N			0
#define	DIP1_N			1
#define	DIP2_N			2
#define	DIP3_N			3
// TODO check if correct button pin
#define BUT1_N  7
#define BUT2_N  6

/* Pin position defines - do not modify */
#define DIP0_PIN		(1<<DIP0_N)
#define DIP1_PIN		(1<<DIP1_N)
#define DIP2_PIN		(1<<DIP2_N)
#define DIP3_PIN		(1<<DIP3_N)
#define LED_PIN			(1<<LED_N)
#define BUT1_PIN		(1<<BUT1_N)
#define BUT2_PIN		(1<<BUT2_N)

typedef enum {
	ON_LED = 1,
	OFF_LED = 0
} LED_state;

void io_init(void);
uint8_t read_address (void);
void io_led_set (LED_state state);
void io_parse_command (uint8_t data);
uint8_t io_read_buttons(void);

#endif /* IO_H */