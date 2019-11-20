#ifndef IO_H
#define IO_H

#include "avr/io.h"

typedef enum {
	ON = 1,
	OFF = 0
} LED_state;

void io_init(void);
uint8_t read_address (void);
void io_led_set (LED_state state);
void io_parse_command (uint8_t data);

#endif /* IO_H */