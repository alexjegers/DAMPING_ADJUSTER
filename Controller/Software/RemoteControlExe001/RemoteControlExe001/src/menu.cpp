/*
 * menu.cpp
 *
 * Created: 10/12/2022 11:18:44 AM
 *  Author: AJ992
 */ 


#include <avr32/io.h>
#include <stdint.h>
#include <stdio.h>
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
	x0r = x0 - 10;
	y0r = y0 - 10;
	x1r = x0 + stringWidth + 10;
	y1r = y0 + charHeight + 10;	
	
	focus = false;
}

void button::load()
{
	/*Write rectangle.*/
	if (focus == false)
	{
		drawRectangle(x0r,y0r,x1r,y1r,COLOR_WHITE);						//Rectangle border.
		drawRectangle(x0r + 3, y0r + 3, x1r - 3, y1r - 3, COLOR_BLACK);	//Inner rectangle.
		drawText(text,x0,y0,font,COLOR_WHITE);							//Text.
	}
	else
	{
		drawRectangle(x0r,y0r,x1r,y1r,COLOR_WHITE);						//Rectangle border.
		drawRectangle(x0r + 3, y0r + 3, x1r - 3, y1r - 3, COLOR_WHITE);	//Inner rectangle.			
		drawText(text,x0,y0,font,COLOR_BLACK);							//Text.
	}
}

void button::setFocus()
{
	focus == true;
}

label::label(uint16_t x, uint16_t y, char* text, const uint8_t* font)
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
}

label::label(uint16_t x, uint16_t y, uint8_t numText, const uint8_t* font)
{
	this->y = y;
	this->x = x;
	sprintf(this->text, "%d", numText);
	this->font = font;
	uint8_t i = 0;
	while (this->text[i] != 0)
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

void label::load()
{
	drawText(text, x0, y0, font, COLOR_WHITE);
}