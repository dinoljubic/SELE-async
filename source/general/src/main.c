#include <Arduino.h>

#define MASTER_ADDR 15
#define SLAVE1_ADDR 13
#define SLAVE2_ADDR 14

uint8_t ADDR = 0;
uint8_t BUT1_prev;
uint8_t BUT2_prev;

#define LED_PIN   13
#define ADDR3_PIN 11
#define ADDR2_PIN 10
#define ADDR1_PIN 9
#define ADDR0_PIN 8
#define BUT1_PIN  7
#define BUT2_PIN  6
#define WREN_PIN  2

#define COMMAND_ON  0b10000000
#define COMMAND_OFF 0b01000000

//#define MASTER_MODE
#define SLAVE_MODE

void asynch9_init(long BAUD) 
{
		#define BAUDGEN ((16000000/(16*BAUD))-1)  // Calculated divider (may under/overflow for some cases)

		UBRR0 = BAUDGEN;
		UCSR0A |= (1 << MPCM0);
		UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0)| (1 << UCSZ02); // enable  transmit, enable recive, enable recive interrupt , 9 bit mode
		UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);// 2 stop bits USBS0, 9 bit mode  UCSZ0
		
}

void send_addr(uint8_t addr) 
{
  
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;

/* set 9th bit "1" */
UCSR0B |= (1<<TXB80);

/* Put data into buffer, sends the data */
UDR0 = addr;
  
}

void send_data(uint8_t data) 
{
	
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;

/* Set 9th bit "0" */
UCSR0B &= ~(1<<TXB80);

/* Put data into buffer, sends the data */
UDR0 = data;
	
}

uint16_t get_data(void) 
{
   	uint8_t status, resh, resl;
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get status and 9th bit, then data */
	/* from buffer */
	status = UCSR0A;
	resh = UCSR0B;
	resl = UDR0;
	/* If error, return -1 */
	if (status & ((1<<FE0) | (1<<DOR0) | (1<<UPE0)))
		{return -1;}
	/* Filter the 9th bit, then return */
	resh = (resh >> 1) & 0x01;
	return ((resh << 8) | resl);
 }

#ifdef SLAVE_MODE
ISR (USART_RX_vect)        //interrupt if recive data 
{
	uint16_t frame;

	frame = get_data();
		
	if ((frame & 0x100) != 0) // If waiting for and received address frame
	{
			if ((frame & 0xff) == SLAVE2_ADDR)
			{
				// never reach this line
				digitalWrite (LED_PIN,HIGH);
				UCSR0A &= ~(1 << MPCM0); //turn off Multiprocessor mode
			}
			else 
			{
				UCSR0A |= (1 << MPCM0); //turn on Multiprocessor mode
			}
	}
	else // wait for a data frame
	{
		if ((frame & 0xff) == COMMAND_ON)
		{
			digitalWrite (LED_PIN,HIGH);
		}
		else if ((frame & 0xff) == COMMAND_OFF)
		{
			digitalWrite (LED_PIN,LOW);
		}
	}
}
#endif

void setup() 
{
	
#ifdef MASTER_MODE

  pinMode(BUT1_PIN, INPUT_PULLUP);    // sets the digital pin as input
  pinMode(BUT2_PIN, INPUT_PULLUP);    // sets the digital pin as input
  
#endif /* MASTER_MODE */

#ifdef SLAVE_MODE

	pinMode(ADDR0_PIN, INPUT_PULLUP);    // sets the digital pin as input
	pinMode(ADDR1_PIN, INPUT_PULLUP);    // sets the digital pin as input
	pinMode(ADDR2_PIN, INPUT_PULLUP);    // sets the digital pin as input
	pinMode(ADDR3_PIN, INPUT_PULLUP);    // sets the digital pin as input

	pinMode(LED_PIN, OUTPUT);    // sets the digital pin as input

	ADDR += 1*(digitalRead(ADDR0_PIN));    // read the pin and save address, if short to ground "0", if not connected or connected to Vcc "1"
	ADDR += 2*(digitalRead(ADDR1_PIN));    // read the pin and save address
	ADDR += 4*(digitalRead(ADDR2_PIN));    // read the pin and save address
	ADDR += 8*(digitalRead(ADDR3_PIN));    // read the pin and save address

	sei(); // set global interrupts
	
#endif /* SLAVE_MODE */

  asynch9_init(57600);
    
}

void loop() 
{
  
#ifdef MASTER_MODE

		uint8_t BUT1 = digitalRead(BUT1_PIN);
		uint8_t BUT2 = digitalRead(BUT2_PIN);
		  
		if (~BUT1 & BUT1_prev)
		  {
			send_addr(SLAVE1_ADDR);
			send_data(COMMAND_ON);
		  }
		else if (BUT1 & ~BUT1_prev)
		  {
			send_data(COMMAND_OFF);
		  }
		  
		else if (~BUT2 & BUT2_prev)
		  {
			send_addr(SLAVE2_ADDR);
			send_data(COMMAND_ON);
		  }
		else if (BUT2 & ~BUT2_prev)
		  {
			send_data(COMMAND_OFF);
		  }
		  
		BUT1_prev = BUT1;
		BUT2_prev = BUT2;

#endif /* MASTER_MODE */



#ifdef SLAVE_MODE

// waiting for interrupts

#endif /* SLAVE_MODE */

 
}
