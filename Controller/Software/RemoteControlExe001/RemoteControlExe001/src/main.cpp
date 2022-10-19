/*
 * RemoteControlExe001.cpp
 *
 * Created: 10/11/2022 11:58:49 AM
 * Author : AJ992
 */ 

#define F_CPU					24000000 //Needed for delay.h

#include <avr32/io.h>
#include <stdint.h>
#include "system.h"
#include "io.h"
#include "interrupts.h"
#include "lcd.h"
#include <intc.h>
#include <delay.h>


int main(void)
{
	/*Debug LEDs as IO and output.*/
	ioSetPinIO(&LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN);		
	ioSetPinOutput(&LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN);
	LED3_ON;
	
	/*Enable OSC0 and select it as the main clock*/
	systemOSC0init();
	if (systemMainClockSelect() == true)
	{
		LED1_ON;
	}
	else
	{
		LED2_ON;
	}


	/*Switches as IO inputs, enable falling interrupt for all*/
	ioSetPinIO(&SW_PORT, SW1_bm | SW2_bm | SW3_bm | SW4_bm | SW5_bm);
	ioEnableInterrupt(&SW_PORT, SW1_bm | SW2_bm | SW3_bm | SW4_bm | SW5_bm);
	ioInterruptMode(&SW_PORT, GPIO_IMR_FALLING_bm, 
					SW1_bm | SW2_bm | SW3_bm | SW4_bm | SW5_bm);
	
	intEnableSwitchInterrupts();				

	delay_init();
 	lcdInit();
	
	while(1)
    {

    }
}

