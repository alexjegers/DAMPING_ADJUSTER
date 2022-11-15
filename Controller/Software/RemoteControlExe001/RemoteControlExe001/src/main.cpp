/*
 * RemoteControlExe001.cpp
 *
 * Created: 10/11/2022 11:58:49 AM
 * Author : AJ992
 */ 

#define F_CPU					48000000 //Needed for delay.h

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
#include "displayStates.h"
#include "stepper.h"
#include "iic.h"

uint8_t testyRead = 0;
uint8_t readAddr = 0x00;
	uint8_t bigTest[5] = {0, 1, 2, 3, 4};
	uint8_t testRead[5];
int main(void)
{
	/*Debug LEDs as IO and output.*/
	ioSetPinIO(&LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN);		
	ioSetPinOutput(&LED_PORT, LED1_PIN | LED2_PIN | LED3_PIN);
	
	/*Enable OSC0 and select it as the main clock*/
	systemOSC0init();
	systemMainClockSelect();


	/*Switches as IO inputs, enable falling interrupt for all*/
	ioSetPinIO(&SW_PORT, SW1_bm | SW2_bm | SW3_bm | SW4_bm | SW5_bm);
	ioEnableInterrupt(&SW_PORT, SW1_bm | SW2_bm | SW3_bm | SW4_bm | SW5_bm);
	ioInterruptMode(&SW_PORT, GPIO_IMR_FALLING_bm, 
					SW1_bm | SW2_bm | SW3_bm | SW4_bm | SW5_bm);
	
	/*Initialize ASF delay library.*/
	delay_init();
	
	/*Initialize the LCD then delete instance, ST7789 is inherited by button and label*/
	ST7789 *lcd = new ST7789;	
	lcd->init();
	delete lcd;

	/*Assign the function pointers for the switches and load stateA*/
	setBtnEventHandlers();
	stateA();
	
	/*Initialize interrupts for the switches.*/
	intEnableSwitchInterrupts();	
	
	/*Initialize IIC peripheral and pins*/
	ioSetPinPeripheral(&PORTA, TWI_DATA_PIN_bm | TWI_CLK_PIN_bm);				//Give peripheral control of pins.
	ioSetPeripheralFunction(&PORTA, TWI_DATA_PIN_bm, GPIO_PMR_FUNCTION_A);		//Set multiplexing function.
	ioSetPeripheralFunction(&PORTA, TWI_CLK_PIN_bm, GPIO_PMR_FUNCTION_A);		//Set multiplexing function.
	
	iicSetup();
	iicSetClkSpeed(F_CPU, 100000);

	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	
	/* write 5, write 1, read 1 */
 	iicNewTransmission(IIC_NEW_TRNS_WRITE, EEPROM_DEVICE_ADDR, sizeof(bigTest), &bigTest);
	iicNewTransmission(IIC_NEW_TRNS_WRITE, EEPROM_DEVICE_ADDR, 1, &readAddr);	 
	iicNewTransmission(IIC_NEW_TRNS_READ, EEPROM_DEVICE_ADDR, 1, &testyRead);

	
  	frontLeft.stepperInfo.position = 5;
	frontRight.stepperInfo.position = 5;
	while(1)
    {

    }
}

