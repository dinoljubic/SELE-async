#ifndef SLAVE_HANDLES_H
#define SLAVE_HANDLES_H

#include <avr/io.h>

typedef enum {
	OFF = 0;
	ON = 1;
} LED_state;

void Slave_SetLED (uint8_t address, LED_state state);
void Slave_SendData (uint8_t address, uint8_t data);


#endif /* SLAVE_HANDLES_H */