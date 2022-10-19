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

__attribute__((__interrupt__))void intHandlerPortA()
{
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