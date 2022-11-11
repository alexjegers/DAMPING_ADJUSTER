/*
 * interrupts.cpp
 *
 * Created: 10/16/2022 2:41:30 PM
 *  Author: AJ992
 */ 
#include <avr32/io.h>
#include <stdint.h>
#include "system.h"
#include "io.h"
#include <intc.h>
#include "displayStates.h"

__attribute__((__interrupt__))void intHandlerPortA()
{
	uint32_t buttonEvent = 0;
	buttonEvent = ioIntFlags(&SW_PORT);
	
	switch (buttonEvent)
	{
		case SW1_bm:
		displayCurrentEvent = CENTER_SWITCH;
		break;
		
		case SW2_bm:
		displayCurrentEvent = UP_SWITCH;
		break;
		
		case SW3_bm:
		displayCurrentEvent = RIGHT_SWITCH;
		break;
		
		case SW4_bm:
		displayCurrentEvent = LEFT_SWITCH;
		break;
		
		case SW5_bm:
		displayCurrentEvent = DOWN_SWITCH;
		break;
		
		default:
		displayCurrentEvent = displayCurrentEvent;
		displayCurrentEvent = NONE;
		break;
	}
	handleNewEvent(displayCurrentEvent);
	ioPinHigh(&LED_PORT, LED2_PIN);							//Turn on an LED.
	ioClearIntFlag(&SW_PORT, SW1_bm | SW2_bm | SW3_bm		//Don't care which switch triggered it clear them all.
					| SW4_bm | SW5_bm);
}


void intEnableSwitchInterrupts()
{
	/*intc.h built in functions.*/
	INTC_init_interrupts();
	INTC_register_interrupt(&intHandlerPortA, AVR32_GPIO_IRQ_2, 3);
	INTC_register_interrupt(&intHandlerPortA, AVR32_GPIO_IRQ_3, 3);
	Enable_global_interrupt();
}