#ifndef SLAVE_HANDLES_H
#define SLAVE_HANDLES_H

#include <avr/io.h>

typedef enum {
	OFF = 0,
	ON = 1
} LED_State_t;

void Slave_SetLED (uint8_t address, LED_State_t state);
void Slave_SendData (uint8_t address, uint8_t data);


#endif /* SLAVE_HANDLES_H */