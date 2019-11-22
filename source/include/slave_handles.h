#ifndef SLAVE_HANDLES_H
#define SLAVE_HANDLES_H

#include <avr/io.h>
#include "io.h"

typedef struct {
	uint8_t		address;
	PinState_t	state;
	uint8_t		button_pin;
} SlaveDevice_t;

void Slave_SetLED (uint8_t address, PinState_t state);
void Slave_Send (uint8_t address, uint8_t data);
void Slave_ParseCommand (uint8_t data);


#endif /* SLAVE_HANDLES_H */