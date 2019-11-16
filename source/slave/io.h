#ifndef IO_H
#define IO_H

typedef enum {
	ON = 1,
	OFF = 0
} LED_state;

void io_init(void);
uint8_t read_address (void);
void led_set (LED_state state);


#endif /* IO_H */