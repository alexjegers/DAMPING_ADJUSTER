/*
 * menu.cpp
 *
 * Created: 10/12/2022 11:18:44 AM
 *  Author: AJ992
 */ 


#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "menu.h"

button::button(uint16_t x, uint16_t y, char* text, const uint8_t font[])
{
	this->y = y;
	this->x = x;
	this->text = text;
	this->font = font;
	uint8_t i = 0;
	while (text[i] != 0)
	{
		numChars++;
		i++;
	}
	
	/*Determine the width and height of the string.*/
	charWidth = font[0];
	charHeight = font[1];
	charPadding = font[2];
	stringWidth = (charWidth * numChars) - (charPadding * (numChars));
	
	/*Determine a starting and ending point for the string.*/
	x0 = x - (stringWidth / 2);
	y0 = y - (charHeight / 2); 	
	
	/*Determine size of rectangle.*/
	x0r = x0 - 20;
	y0r = y0 - 10;
	x1r = x0 + stringWidth + 20;
	y1r = y0 + charHeight + 10;	
}


void button::load()
{

	if (button::focusedBtn == this)
	{		
		if (button::selectedBtn == this)
		{
			drawRectangle(x0r, y0r, x1r, y1r, BUTTON_SELECTED_COLOR);						//Highlight rectangle
			drawText(text, x0, y0, font, COLOR_BLACK, COLOR_WHITE);
		}
		else
		{
			drawBorderedRect(x0r, y0r, x1r, y1r, 6, BUTTON_SELECTED_COLOR, COLOR_BLACK);
			drawText(text,x0,y0,font, BUTTON_FOCUSED_COLOR, COLOR_BLACK);	
		}															
	}
	else
	{
		drawBorderedRect(x0r, y0r, x1r, y1r, 3, BUTTON_COLOR, COLOR_BLACK);			
		drawText(text, x0, y0, font, BUTTON_COLOR, COLOR_BLACK);							//Text.		
	}
}

void button::setFocus()
{
	clearSelected();
	button::focusedBtn = this;
}

button* button::focusedBtn;

bool button::setSelected()
{
	if (button::focusedBtn == this)
	{
		button::selectedBtn = this;
		return true;
	}
	else
	{
		return false;
	}
}

void button::clearSelected()
{
	button::selectedBtn = NULL;
}

button* button::selectedBtn;

void button::focusedEventHandlers(void (*upSwitch)(), void (*downSwitch)(), void(*centerSwitch)(),
									void (*leftSwitch)(), void (*rightSwitch)())
{
	upSwitchFocus = upSwitch;
	downSwitchFocus = downSwitch;
	centerSwitchFocus = centerSwitch;
	leftSwitchFocus = leftSwitch;
	rightSwitchFocus = rightSwitch;
}

void button::selectedEventHandlers(void (*upSwitch)(), void (*downSwitch)(), void(*centerSwitch)(), 
									void (*leftSwitch)(), void (*rightSwitch)())
{
	upSwitchSelect = upSwitch;
	downSwitchSelect = downSwitch;
	centerSwitchSelect = centerSwitch;	
	leftSwitchSelect = leftSwitch;
	rightSwitchSelect = rightSwitch;
}



 label::label(uint16_t x, uint16_t y, char* text, const uint8_t* font)
{
	this->y = y;
	this->x = x;
	this->text = (char*)malloc(sizeof(text));
	memcpy(this->text, text, sizeof(text));
	this->font = font;
	uint8_t i = 0;
	while (text[i] != 0)
	{
		numChars++;
		i++;
	}
	
	/*Determine the width and height of the string.*/
	charWidth = font[0];
	charHeight = font[1];
	charPadding = font[2];
	stringWidth = (charWidth * numChars) - (charPadding * (numChars));
	
	/*Determine a starting and ending point for the string.*/
	x0 = x - (stringWidth / 2);
	y0 = y - (charHeight / 2);	
}

void label::setText(char* newText)
{
	this->text = newText;
}

void label::load()
{
	drawText(text, x0, y0, font, TEXT_COLOR, COLOR_BLACK);
}