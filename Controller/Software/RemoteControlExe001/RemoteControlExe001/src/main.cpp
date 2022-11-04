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
#include "fonts.h"
#include "menu.h"

ST7789 lcd;

button optionsButton(120, 280, "Options", font16pt);
button rearButton(120, 150, "Rear", font16pt);
button frontButton(120, 30, "Front", font16pt);
label frontLeftLabel(60, 80, 12, font36pt);
label frontRightLabel(180, 80, 12, font36pt);
label rearLeftLabel(60, 200, 23, font36pt);
label rearRightLabel(180, 200, 23, font36pt);
int main(void)
{
	/*Debug LEDs as IO and output.*/
	ioSetPinIO(&LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN);		
	ioSetPinOutput(&LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN);
	
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
 	lcd.init();
	optionsButton.load();
	rearButton.load();
	frontButton.load();
	frontLeftLabel.load();
	frontRightLabel.load();
	rearLeftLabel.load();
	rearRightLabel.load();
	LED3_ON;
	while(1)
    {

    }
}

