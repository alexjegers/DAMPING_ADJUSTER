/*
 * state_machine.cpp
 *
 * Created: 11/4/2022 6:43:10 PM
 *  Author: AJ992
 */ 

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "displayStates.h"
#include "lcd.h"
#include "menu.h"
#include "fonts.h"


LCD_DISPLAY_EVENTS displayCurrentEvent;

button frontButton(120, 30, "Front", font16pt);
label frontLeftLabel(60, 80, "39", font36pt);
label frontRightLabel(180, 80, "39", font36pt);

button rearButton(120, 150, "Rear", font16pt);
label rearLeftLabel(60, 200, "19", font36pt);
label rearRightLabel(180, 200, "19", font36pt);

button optionsButton(120, 280, "Options", font16pt);

/* currentNum is global so I can access it with the debugger */
uint8_t currentNum = 0;

void setBtnEventHandlers()
{	
	frontButton		.focusedEventHandlers(&stateC, &stateB, &stateD, &noChange, &noChange);
	rearButton		.focusedEventHandlers(&stateA, &stateC, &stateE, &noChange, &noChange);
	optionsButton	.focusedEventHandlers(&stateB, &stateA, &stateJ, &noChange, &noChange);
	
	frontButton		.selectedEventHandlers(&stateF, &stateG, &stateA, &noChange, &noChange);
	rearButton		.selectedEventHandlers(&stateH, &stateI, &stateB, &noChange, &noChange);
}

void handleNewEvent(LCD_DISPLAY_EVENTS currentEvent)
{
	if (button::selectedBtn != NULL)
	{
		button btn = *button::selectedBtn;
		
		switch (currentEvent)
		{
			case UP_SWITCH:
			btn.upSwitchSelect();
			break;
			
			case DOWN_SWITCH:
			btn.downSwitchSelect();
			break;
			
			case LEFT_SWITCH:
			btn.leftSwitchSelect();			
			break;
			
			case RIGHT_SWITCH:
			btn.leftSwitchSelect();
			break;
			
			case CENTER_SWITCH:
			btn.centerSwitchSelect();
			break;
			
			default:

			break;
		}		
	}
	else
	{
		button btn = *button::focusedBtn;
		
		switch (currentEvent)
		{
			case UP_SWITCH:
			btn.upSwitchFocus();	
			break;
			
			case DOWN_SWITCH:
			btn.downSwitchFocus();
			break;
			
			case LEFT_SWITCH:
			btn.leftSwitchFocus();			
			break;
			
			case RIGHT_SWITCH:
			btn.leftSwitchFocus();
			break;
			
			case CENTER_SWITCH:
			btn.centerSwitchFocus();
			break;
			
			default:

			break;
		}
		displayCurrentEvent = NONE;	
	}
}

void noChange()
{
	
}

/*
Function: stateA
Params: none
Returns: none
Description: Main form, front button focused, nothing selected
*/
void stateA()
{
	frontButton.setFocus();
	optionsButton.load();
	rearButton.load();
	frontButton.load();
	frontLeftLabel.load();
	frontRightLabel.load();
	rearLeftLabel.load();
	rearRightLabel.load();
}

/*
Function: stateB
Params: none
Returns: none
Description: Main form, rear button focused.
*/
void stateB()
{
	rearButton.setFocus();
	optionsButton.load();
	rearButton.load();
	frontButton.load();
	frontLeftLabel.load();
	frontRightLabel.load();
	rearLeftLabel.load();
	rearRightLabel.load();	
}

/*
Function: stateC
Params: none
Returns: none
Description: Main form, options button focused.
*/
void stateC()
{
	optionsButton.setFocus();
	optionsButton.load();
	rearButton.load();
	frontButton.load();
	frontLeftLabel.load();
	frontRightLabel.load();
	rearLeftLabel.load();
	rearRightLabel.load();	
}

void stateD()
{
	frontButton.setSelected();
	optionsButton.load();
	rearButton.load();
	frontButton.load();
	frontLeftLabel.load();
	frontRightLabel.load();
	rearLeftLabel.load();
	rearRightLabel.load();	
}

void stateE()
{
	rearButton.setSelected();
	optionsButton.load();
	rearButton.load();
	frontButton.load();
	frontLeftLabel.load();
	frontRightLabel.load();
	rearLeftLabel.load();
	rearRightLabel.load();	
}

void stateF()
{
	currentNum = atoi(frontRightLabel.text);
	currentNum++;
	sprintf(frontLeftLabel.text, "%d", currentNum);
	sprintf(frontRightLabel.text, "%d", currentNum);
	frontLeftLabel.load();
	frontRightLabel.load();
}

void stateG()
{
	currentNum = atoi(frontRightLabel.text);			//Get the current number from the label.
	currentNum--;										//Decrement it.
	
	sprintf(frontRightLabel.text, "%d", currentNum);	//Turn it back into a char* for the left.
	sprintf(frontLeftLabel.text, "%d", currentNum);		//Turn it back into a char* for the right.
	
	frontLeftLabel.load();								//Load the left label back onto the LCD.
	frontRightLabel.load();								//Load the right label back to the LCD.
}

void stateH()
{
	currentNum = atoi(rearRightLabel.text);
	currentNum++;
	sprintf(rearLeftLabel.text, "%d", currentNum);
	sprintf(rearRightLabel.text, "%d", currentNum);
	rearLeftLabel.load();
	rearRightLabel.load();	
}

void stateI()
{
	currentNum = atoi(rearRightLabel.text);
	currentNum--;
	sprintf(rearLeftLabel.text, "%d", currentNum);
	sprintf(rearRightLabel.text, "%d", currentNum);
	rearLeftLabel.load();
	rearRightLabel.load();	
}

void stateJ()
{
	
}

void stateK()
{
	
}

void stateL()
{
	
}

void stateM()
{
	
}
