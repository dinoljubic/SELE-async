#ifndef SLAVE_HANDLES_H
#define SLAVE_HANDLES_H

#include <avr/io.h>

typedef enum {
	OFF = 0,
	ON = 1
} LED_State_t;

void Slave_SetLED (LED_State_t state);
void Slave_Send_Data (uint8_t data);
void Slave_Send_Address (uint8_t address);


#endif /* SLAVE_HANDLES_H */