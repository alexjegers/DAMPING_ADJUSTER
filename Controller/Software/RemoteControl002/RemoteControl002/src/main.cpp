/*
 * RemoteControl002.cpp
 *
 * Created: 11/30/2022 11:49:43 AM
 * Author : AJ992
 */ 

#define F_CPU					48000000 //Needed for delay.h

#include <avr32/io.h>
#include <stdint.h>
#include <stdio.h>
#include "system.h"
#include "io.h"
#include "lcd.h"
#include <intc.h>
#include <delay.h>
#include "fonts.h"
#include "forms.h"
#include "stepper.h"
#include "iic.h"

form mainScreen;
form settings;

void loadSettingsScreen(char* btnName);
void loadMainScreen(char* btnName);
void incrementStepperPosition(char* btnName);
void decrementStepperPosition(char* btnName);
void sendToStepper(char* btnName);

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
	
	/*Initialize IIC peripheral and pins*/
	ioSetPinPeripheral(&PORTA, TWI_DATA_PIN_bm | TWI_CLK_PIN_bm);				//Give peripheral control of pins.
	ioSetPeripheralFunction(&PORTA, TWI_DATA_PIN_bm, GPIO_PMR_FUNCTION_A);		//Set multiplexing function.
	ioSetPeripheralFunction(&PORTA, TWI_CLK_PIN_bm, GPIO_PMR_FUNCTION_A);		//Set multiplexing function.
	
	iic::setup();
	iic::setClkSpeed(F_CPU, 100000);

	/*Main screen buttons.*/
	mainScreen.addNewBtn("frontBtn", "Front", 120, 30, font16pt);
	mainScreen.addNewBtn("rearBtn", "Rear", 120, 150, font16pt);
	mainScreen.addNewBtn("optsBtn", "Options", 120, 280, font16pt);
	
	/*Main screen labels.*/
	mainScreen.addNewLabel("FLlbl", frontLeft.positionChar, 60, 80, font20pt);
	mainScreen.addNewLabel("FRlbl", frontRight.positionChar, 180, 80, font20pt);
	mainScreen.addNewLabel("RLlbl", rearLeft.positionChar, 60, 200, font20pt);
	mainScreen.addNewLabel("RRlbl", rearRight.positionChar, 180, 200, font20pt);
	
	/*Set button function pointers.*/
	mainScreen.btnSelectedBehavior("frontBtn", UP_SWITCH, &incrementStepperPosition);	
	mainScreen.btnSelectedBehavior("rearBtn", UP_SWITCH, &incrementStepperPosition);
	mainScreen.btnSelectedBehavior("frontBtn", DOWN_SWITCH, &decrementStepperPosition);
	mainScreen.btnSelectedBehavior("rearBtn", DOWN_SWITCH, &decrementStepperPosition);
	mainScreen.btnSelectedBehavior("frontBtn", CENTER_SWITCH, &sendToStepper);
	mainScreen.btnSelectedBehavior("rearBtn", CENTER_SWITCH, &sendToStepper);
	mainScreen.btnFocusBehavior("optsBtn", CENTER_SWITCH, &loadSettingsScreen);
	
	settings.addNewBtn("backBtn", "Back", 120, 30, font16pt);
	settings.addNewBtn("resetBtn", "Reset", 120, 80, font16pt);
	settings.addNewBtn("brightBtn", "Brightness", 120, 130, font16pt);
	
	
	settings.btnFocusBehavior("backBtn", CENTER_SWITCH, &loadMainScreen);
	
	mainScreen.load();

 	if (iic::probe(EEPROM_DEVICE_ADDR) == iic::IIC_OK)
 	{
 		LED3_ON;
 	}
	else
	{
		LED1_ON;
	}
	
	LED1_OFF;
	LED3_OFF;
	
	if (iic::probe(0x21) == iic::IIC_OK)
 	{
 		LED3_ON;
 	}
	else
	{
		LED1_ON;
	}
    /* Replace with your application code */
    while (1) 
    {
    }
}


void loadSettingsScreen(char* btnName)
{
	settings.load();
}

void loadMainScreen(char* btnName)
{
	mainScreen.load();
}

void incrementStepperPosition(char* btnName)
{
	if (btnName == "frontBtn")
	{
		frontLeft.stepperInfo.position += 10;
		frontRight.stepperInfo.position = frontLeft.stepperInfo.position;
		frontLeft.positionChar[0] = 48 + (frontLeft.stepperInfo.position / 100);
		frontLeft.positionChar[1] = 48 + ((frontLeft.stepperInfo.position / 10) % 10);
		frontRight.positionChar[0] = frontLeft.positionChar[0];
		frontRight.positionChar[1] = frontLeft.positionChar[1];
	}
	else
	{
		rearLeft.stepperInfo.position += 10;
		rearRight.stepperInfo.position = rearLeft.stepperInfo.position;
		rearLeft.positionChar[0] = 48 + (rearLeft.stepperInfo.position / 100);
		rearLeft.positionChar[1] = 48 + ((rearLeft.stepperInfo.position / 10) % 10);
		rearRight.positionChar[0] = rearLeft.positionChar[0];
		rearRight.positionChar[1] = rearLeft.positionChar[1];		
	}
}

void decrementStepperPosition(char* btnName)
{
	if (btnName == "frontBtn")
	{
		frontLeft.stepperInfo.position -= 10;
		frontRight.stepperInfo.position = frontLeft.stepperInfo.position;
		frontLeft.positionChar[0] = 48 + (frontLeft.stepperInfo.position / 100);
		frontLeft.positionChar[1] = 48 + ((frontLeft.stepperInfo.position / 10) % 10);
		frontRight.positionChar[0] = frontLeft.positionChar[0];
		frontRight.positionChar[1] = frontLeft.positionChar[1];
	}
	else
	{
		rearLeft.stepperInfo.position -= 10;
		rearRight.stepperInfo.position = rearLeft.stepperInfo.position;
		rearLeft.positionChar[0] = 48 + (rearLeft.stepperInfo.position / 100);
		rearLeft.positionChar[1] = 48 + ((rearLeft.stepperInfo.position / 10) % 10);
		rearRight.positionChar[0] = rearLeft.positionChar[0];
		rearRight.positionChar[1] = rearLeft.positionChar[1];		
	}	
}

void sendToStepper(char* btnName)
{
	form::toggleSelectedBtn();
}