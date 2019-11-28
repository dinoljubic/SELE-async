# SELE-async
Asynchronous Communication System project for laboratory exercises of Electronic Systems (SELE) course on Faculty of Engineering of University of Porto (FEUP).

## Project wiki
Brief explanation of implemented solution to the task given at the laboratory excercises. Firstly, an overview is given, followed by detailed explanation of each module. Functionality of each file included in source (src) folder of this repository is explained below. 

Generally, the system consists of three Arduino UNO boards connected to a common RS485 bus using the internal USART peripheral and a dedicated transceiver. Two of the boards will act as slave devices with the third being a master. Role of the master is to read the state of the buttons connected to the board's IO pins and control internal LEDs on the slave boards accordingly. 
Devices are identified with addresses assigned to them by setting state of 4 DIP switches to the wanted binary value. 

## System description

**Master device** is implemented to read states of the buttons assigned to a slave device in a continuous loop and compare this to internally stored state of the slave's LED. On differing states, a command is issued through the serial bus to the slave to set it's LED to match the state of the button.
**Slave device** does not do anything in it's normal operation. All data (and addresses) received over the serial bus are processed in interrupts. More on this functionality is given later.
##
**main.c**<br/>
Main module; contains initialization and while(1) loops for master and slave device. After initialization of *io* module, it checks assigned address and determines role of the device.

**io.c**<br/>
Contains functions for initialization of IO pins: the internal LED, buttons and switches. Also contains functions to read state of the switches (as a byte value), *read_address* and of a specified button: *io_read_button*, and for asserting a state to the LED: *io_led_set*. Associated header ***io.h*** contains definitions for all the IO pins used on both the master and slave device and an enumerator for the state of a pin.

**serial.c**<br/>
Contains functions for initialization and use of USART peripheral as well as a higher-level RS485.
Initialization should be initiated through the HL RS485 function *RS485_init*, which takes as an argument role of the node (master or slave). This affects the state of the *WREN* pin on the transceiver. The function also initializes the USART in asynchronous mode, with only transmit enabled for the master and only receive (with interrupts) for the slaves. Also, this enables the Multi-processor Communication Mode.<br/>
Functions for transmitting and receiving data (*RS485_Transmit* and *RS485_Receive*) call accordingly lower level functions of USART, which are implemented as given in the device's datasheet: in 9-bit mode. Only modification in the higher-level functions is on transmission: the function takes two arguments, the address and the data. On transmission of the address frame, it is stored so that for every following transmission to that same address no address frame is sent. This is visible in screenshots of logic analyzer. <br/>
**TODO: record screenshots**<br/>
Receiving is handled in the interrupt routine, again following procedure explained in Multi-processor Communication Mode section. If an address frame is received and the address is same as device's own, Multiprocessor mode is disabled and the device is ready for receiving data. Other devices do not register any of the transmitted frames until a new address frame is received.
Parsing of the data frame messages is done in the last module.

**slave_handles.c**<br/>
Module for implementing functionality requested in the lab assignment. On the master side, it uses the function *Slave_SetLED* to assert the LED. Slave uses the function *Slave_ParseCommand*.

