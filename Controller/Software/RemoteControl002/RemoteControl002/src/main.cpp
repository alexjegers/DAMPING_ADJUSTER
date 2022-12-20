/*
 * RemoteControl002.cpp
 *
 * Created: 11/30/2022 11:49:43 AM
 * Author : AJ992
 */ 

#define F_CPU					48000000 //Needed for delay.h

#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "system.h"
#include "io.h"
#include "lcd.h"
#include <intc.h>
#include <delay.h>
#include "fonts.h"
#include "forms.h"
#include "stepper.h"
#include "iic.h"

ST7789 lcd;

form mainScreen;
form settingsScreen;
form loadingScreen;

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
	lcd.init();
/*	lcd.drawBorderedRoundedRect(50, 50, 150, 100, 7, 4, COLOR_WHITE, COLOR_BLACK, COLOR_BLACK);*/

	
	/*Initialize IIC peripheral and pins*/
	ioSetPinPeripheral(&PORTA, TWI_DATA_PIN_bm | TWI_CLK_PIN_bm);				//Give peripheral control of pins.
	ioSetPeripheralFunction(&PORTA, TWI_DATA_PIN_bm, GPIO_PMR_FUNCTION_A);		//Set multiplexing function.
	ioSetPeripheralFunction(&PORTA, TWI_CLK_PIN_bm, GPIO_PMR_FUNCTION_A);		//Set multiplexing function.
	iic::setup();
	iic::setClkSpeed(F_CPU, 100000);

	frontLeft.setDeviceAddr(FRONT_LEFT_DEVICE_ADDR);
	frontRight.setDeviceAddr(FRONT_RIGHT_DEVICE_ADDR);
	rearLeft.setDeviceAddr(REAR_LEFT_DEVICE_ADDR);
	rearRight.setDeviceAddr(REAR_RIGHT_DEVICE_ADDR);

	/*Main screen controls.*/
	mainScreen.addNewBtn("frontBtn", "Front", 120, 30, font16pt);
	mainScreen.addNewBtn("rearBtn", "Rear", 120, 150, font16pt);
	mainScreen.addNewBtn("optsBtn", "Options", 120, 280, font16pt);
	mainScreen.addNewLabel("FLlbl", frontLeft.positionChar, 60, 80, font20pt);
 	mainScreen.addNewLabel("FRlbl", frontRight.positionChar, 180, 80, font20pt);
 	mainScreen.addNewLabel("RLlbl", rearLeft.positionChar, 60, 200, font20pt);
 	mainScreen.addNewLabel("RRlbl", rearRight.positionChar, 180, 200, font20pt);
	
	/*Setting screen controls*/
	settingsScreen.addNewBtn("backBtn", "Back", 120, 30, font16pt);
	settingsScreen.addNewBtn("resetBtn", "Reset", 120, 80, font16pt);
	settingsScreen.addNewBtn("brightBtn", "Brightness", 120, 130, font16pt);
	
 	/*Loading screen controls.*/
	loadingScreen.addNewLabel("FLlbl", "Loading", 60, 80, font20pt);
 	loadingScreen.addNewLabel("FRlbl", "Loading", 180, 80, font20pt);
 	loadingScreen.addNewLabel("RLlbl", "Loading", 60, 200, font20pt);
 	loadingScreen.addNewLabel("RRlbl", "Loading", 180, 200, font20pt);
	loadingScreen.addNewBtn("okBtn", "Okay", 120, 290, font20pt);
	loadingScreen.pButton("okBtn")->show = false;
	loadingScreen.pButton("okBtn")->centerSwitchFocus = &loadMainScreen;
	 
	/*Set button function pointers.*/
	mainScreen.pButton("frontBtn")->upSwitchSelect = &incrementStepperPosition;
	mainScreen.pButton("frontBtn")->downSwitchSelect = &decrementStepperPosition;
	mainScreen.pButton("frontBtn")->centerSwitchSelect = &sendToStepper;
	mainScreen.pButton("rearBtn")->upSwitchSelect = &incrementStepperPosition;
	mainScreen.pButton("rearBtn")->downSwitchSelect = &decrementStepperPosition;
	mainScreen.pButton("rearBtn")->centerSwitchSelect = &sendToStepper;
	mainScreen.pButton("optsBtn")->centerSwitchFocus = &loadSettingsScreen;	
		
	settingsScreen.pButton("backBtn")->centerSwitchFocus = &loadMainScreen;

	//Write 0 and 0 to position and set point.
	iic::fastTransmission(IIC_NEW_TRNS_WRITE, 0x05, 0x00, 4, &frontRight.stepperInfo.position);	
	iic::fastTransmission(IIC_NEW_TRNS_WRITE, 0x06, 0x00, 4, &frontLeft.stepperInfo.position);
	Enable_global_interrupt();
	
	//mainScreen.load();
	
    /* Replace with your application code */
    while (1) 
    {
    }
}


void loadSettingsScreen(char* btnName)
{
	settingsScreen.load();
}

void loadMainScreen(char* btnName)
{
	mainScreen.load();
}

void incrementStepperPosition(char* btnName)
{
	STEPPER_MOTOR *stepperRight;
	STEPPER_MOTOR *stepperLeft;
	if (btnName == (char*)"frontBtn")
	{
		stepperRight = &frontRight;
		stepperLeft = &frontLeft;
	}
	else
	{
		stepperRight = &rearRight;
		stepperLeft = &rearLeft;
	}
	stepperLeft->stepperInfo.setPoint += 10;
	stepperRight->stepperInfo.setPoint = stepperLeft->stepperInfo.setPoint;
	stepperLeft->positionChar[0] = 48 + (stepperLeft->stepperInfo.setPoint / 100);
	stepperLeft->positionChar[1] = 48 + ((stepperLeft->stepperInfo.setPoint / 10) % 10);
	stepperRight->positionChar[0] = stepperLeft->positionChar[0];
	stepperRight->positionChar[1] = stepperLeft->positionChar[1];	
	mainScreen.lblText("FLlbl", frontLeft.positionChar);
	mainScreen.lblText("FRlbl", frontRight.positionChar);	
	mainScreen.lblText("RLlbl", rearLeft.positionChar);
	mainScreen.lblText("RRlbl", rearRight.positionChar);
}

void decrementStepperPosition(char* btnName)
{
	STEPPER_MOTOR *stepperRight;
	STEPPER_MOTOR *stepperLeft;
	if (btnName == (char*)"frontBtn")
	{
		stepperRight = &frontRight;
		stepperLeft = &frontLeft;
	}
	else
	{
		stepperRight = &rearRight;
		stepperLeft = &rearLeft;
	}
	stepperLeft->stepperInfo.setPoint -= 10;
	stepperRight->stepperInfo.setPoint = stepperLeft->stepperInfo.setPoint;
	stepperLeft->positionChar[0] = 48 + (stepperLeft->stepperInfo.setPoint / 100);
	stepperLeft->positionChar[1] = 48 + ((stepperLeft->stepperInfo.setPoint / 10) % 10);
	stepperRight->positionChar[0] = stepperLeft->positionChar[0];
	stepperRight->positionChar[1] = stepperLeft->positionChar[1];	
	mainScreen.lblText("FLlbl", frontLeft.positionChar);
	mainScreen.lblText("FRlbl", frontRight.positionChar);	
	mainScreen.lblText("RLlbl", rearLeft.positionChar);
	mainScreen.lblText("RRlbl", rearRight.positionChar);		
}

void sendToStepper(char* btnName)
{
	form::toggleSelectedBtn();
	loadingScreen.load();	
	
	if (iic::slowTransmission(IIC_NEW_TRNS_WRITE, FRONT_LEFT_DEVICE_ADDR, MOTOR_SET_POINT, 2, &frontLeft.stepperInfo.setPoint) == iic::IIC_OK)
	{
		loadingScreen.lblText("FLlbl", "OK");
	}
	else
	{
		loadingScreen.lblText("FLlbl", "Error");
	}
	
	if (iic::slowTransmission(IIC_NEW_TRNS_WRITE, FRONT_RIGHT_DEVICE_ADDR, MOTOR_SET_POINT, 2, &frontRight.stepperInfo.setPoint) == iic::IIC_OK)
	{
		loadingScreen.lblText("FRlbl", "OK");
	}
	else
	{
		loadingScreen.lblText("FRlbl", "Error");
	}
	
	if (iic::slowTransmission(IIC_NEW_TRNS_WRITE, REAR_LEFT_DEVICE_ADDR, MOTOR_SET_POINT, 2, &rearLeft.stepperInfo.setPoint) == iic::IIC_OK)
	{
		loadingScreen.lblText("RLlbl", "OK");
	}
	else
	{
		loadingScreen.lblText("RLlbl", "Error");
	}
	
	if (iic::slowTransmission(IIC_NEW_TRNS_WRITE, REAR_RIGHT_DEVICE_ADDR, MOTOR_SET_POINT, 2, &rearRight.stepperInfo.setPoint) == iic::IIC_OK)
	{
		loadingScreen.lblText("RRlbl", "OK");
	}
	else
	{
		loadingScreen.lblText("RRlbl", "Error");
	}
	loadingScreen.pButton("okBtn")->show = true;
	loadingScreen.load();
				
}