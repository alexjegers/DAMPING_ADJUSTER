/*
 * forms.cpp
 *
 * Created: 11/30/2022 11:57:23 AM
 *  Author: AJ992
 */ 

#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <intc.h>
#include "io.h"
#include "iic.h"
#include "lcd.h"
#include "forms.h"

__attribute__((__interrupt__))void form::switchInterruptHanlder()
{
	button* btn = button::focusedBtn; 
	switch (ioIntFlags(&SW_PORT) & SWITCHES_bm)
	{
		case CENTER_SWITCH:
			if (btn->centerSwitchSelect == NULL && btn->centerSwitchFocus == NULL)
			{
				form::toggleSelectedBtn();
			}
			if (button::selectedBtn == btn)
			{
				if (btn->centerSwitchSelect != NULL)
				{
					btn->centerSwitchSelect(btn->name);
				}
			}
			else
			{
				if (btn->centerSwitchFocus != NULL)
				{
					btn->centerSwitchFocus(btn->name);
				}
				else
				{
					form::toggleSelectedBtn();
				}
			}
			
		break;
	
		case UP_SWITCH:
			/*If there's no function to run for click on focus...*/
			if (btn->upSwitchFocus == NULL)
			{
				/*Check to see if the button is selected, if it is not focus previous button.*/
				if (button::selectedBtn == NULL)
				{
					form::focusPrevBtn();
				}
				/*If it is selected, check the function pointer, then run it's click on selected function.*/
				else
				{
					if (btn->upSwitchSelect != NULL)
					{
						btn->upSwitchSelect(btn->name);
					}
				}

			}
			/*If there is a function to run for click on focus, run that function.*/
			else
			{
				btn->upSwitchFocus(btn->name);					
			}			
		break;
		
		case DOWN_SWITCH:
			/*If there's no function to run for click on focus...*/
			if (btn->downSwitchFocus == NULL)
			{
				/*Check to see if the button is selected, if it is not focus previous button.*/
				if (button::selectedBtn == NULL)
				{
					form::focusNextBtn();
				}
				/*If it is selected, check the function pointer, then run it's click on selected function.*/
				else
				{
					if (btn->downSwitchSelect != NULL)
					{
						btn->downSwitchSelect(btn->name);
					}
				}

			}
			/*If there is a function to run for click on focus, run that function.*/
			else
			{
				btn->downSwitchFocus(btn->name);					
			}
		break;
		
		case LEFT_SWITCH:

		break;
		
		case RIGHT_SWITCH:

		break;
		
		default:

		break;
	}	
	/*Clear all switch interrupts.*/
	ioClearIntFlag(&SW_PORT, SWITCHES_bm);
	
	/*Load the form with the changes.*/
	form::activeForm->load();
}

/********Button Functions*********/
button::button(char* name, uint16_t x, uint16_t y, char* text, const uint8_t font[], form* thisForm)
{
	this->name = name;
	this->y = y;
	this->x = x;
	this->text = text;
	this->font = font;
	this->nextBtn = NULL;
	this->previousBtn = NULL;
	
	/*Initialize function pointers.*/
	upSwitchSelect = NULL;
	downSwitchSelect = NULL;
	centerSwitchSelect = NULL;
	leftSwitchSelect = NULL;
	rightSwitchSelect = NULL;
	
	/*If this is the first button created for this form, make it firstBtn.*/
	if (thisForm->firstBtn == NULL)
	{
		thisForm->firstBtn = this;
	}
	/*Otherwise put it at the end of all the buttons.*/
	else
	{
		button* end = thisForm->firstBtn;
		button* prev = thisForm->firstBtn;
		while (end->nextBtn != NULL)
		{
			prev = end;
			end = end->nextBtn;
		}
		end->nextBtn = this;
		this->previousBtn = end;
	}
	
	numChars = strlen(text);
	
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

button::button(){}

void button::clearAll()
{
	clearDisplay();
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

button* button::focusedBtn;

button* button::selectedBtn;


/*********Label Functions*********/
 label::label(char* name,uint16_t x, uint16_t y, char* text, const uint8_t* font, form* thisForm)
{
	this->y = y;
	this->x = x;
	this->text = text;
	this->font = font;
	nextLabel = NULL;
	
	/*Add the new label to the end of the list.*/
	if (thisForm->firstLabel == NULL)
	{
		thisForm->firstLabel = this;
	}
	else
	{
		label* end = thisForm->firstLabel;
		while (end->nextLabel != NULL)
		{
			end = end->nextLabel;
		}
		end->nextLabel = this;
	}
	
	/*Determine how many characters are in the text.*/
	numChars = strlen(text);
	
	/*Determine the width and height of the string.*/
	charWidth = font[0];
	charHeight = font[1];
	charPadding = font[2];
	stringWidth = (charWidth * numChars) - (charPadding * (numChars));
	
	/*Determine a starting and ending point for the string.*/
	x0 = x - (stringWidth / 2);
	y0 = y - (charHeight / 2);	
}

label::label(){}

void label::setText(char* newText)
{
	this->text = newText;
}

void label::load()
{
	drawText(text, x0, y0, font, TEXT_COLOR, COLOR_BLACK);
}


/**********Form Functions***********/
form::form()
{
	/*intc.h built in functions.*/
	INTC_init_interrupts();
	INTC_register_interrupt(&switchInterruptHanlder, AVR32_GPIO_IRQ_2, 2);
	INTC_register_interrupt(&switchInterruptHanlder, AVR32_GPIO_IRQ_3, 2);
	Enable_global_interrupt();
	firstBtn = NULL;
	firstLabel = NULL;
}

form* form::activeForm = NULL;

void form::addNewBtn(char* name, char* text, uint16_t x, uint16_t y, const uint8_t* font)
{
	new button(name, x, y, text, font, this);
}

void form::setBtnFocus(char* name)
{
	button* search = firstBtn;
	while (search->nextBtn != NULL)
	{
		if (search->name == name)
		{
			break;
		}
		else
		{
			search = search->nextBtn;
		}
	}
	button::focusedBtn = search;
}

void form::addNewLabel(char* name, char* text, uint16_t x, uint16_t y, const uint8_t* font)
{
	new label(name, x, y, text, font, this);
}

void form::load()
{
	if (form::activeForm != this)
	{
		clearAll();
		button::focusedBtn = this->firstBtn;
	}

	form::activeForm = this;
	
	if (firstBtn != NULL)
	{
		button* btn = firstBtn;
		while (btn->nextBtn != NULL)
		{
			btn->load();
			btn = btn->nextBtn;
		}
		btn->load();
	}
	
	if (firstLabel != NULL)
	{
		label* lbl = firstLabel;
		while (lbl->nextLabel != NULL)
		{
			lbl->load();
			lbl = lbl->nextLabel;
		}
		lbl->load();
	}
}

void form::focusNextBtn()
{
	button* btn = form::activeForm->firstBtn;	
	while(form::activeForm->button::focusedBtn->name != btn->name)
	{
		btn = btn->nextBtn;
	}
	if (btn->nextBtn != NULL)
	{
		button::focusedBtn = btn->nextBtn;
	}
}

void form::focusPrevBtn()
{
	button* btn = form::activeForm->firstBtn;	
	while(form::activeForm->button::focusedBtn->name != btn->name)
	{
		btn = btn->nextBtn;
	}
	if (btn->previousBtn != NULL)
	{
		button::focusedBtn = btn->previousBtn;
	}	
}

void form::toggleSelectedBtn()
{
	button* btn = form::activeForm->button::focusedBtn;
	if (button::selectedBtn == NULL)
	{
		button::selectedBtn = btn;
	}
	else
	{
		button::selectedBtn = NULL;
	}
}

void form::btnFocusBehavior(char* name, uint32_t dir, void (*func)(char* btnName))
{
	/*Find the button by name in the list.*/
	button* btn = firstBtn;
	while(btn->name != name)
	{
		btn = btn->nextBtn;
	}
	
	switch (dir)
	{

		case CENTER_SWITCH:
			btn->centerSwitchFocus = func;
		break;
		
		case UP_SWITCH:
			btn->upSwitchFocus = func;
		break;
		
		case DOWN_SWITCH:
			btn->downSwitchFocus = func;
		break;
		
		case LEFT_SWITCH:
			btn->leftSwitchFocus = func;
		break;
		
		case RIGHT_SWITCH:
			btn->rightSwitchFocus = func;
		break;
		
		default:

		break;
	}
}

void form::btnSelectedBehavior(char* name, uint32_t dir, void (*func)(char* btnName))
{
	/*Find the button by name in the list.*/
	button* btn = firstBtn;
	while(btn->name != name)
	{
		btn = btn->nextBtn;
	}
	
	switch (dir)
	{

		case CENTER_SWITCH:
			btn->centerSwitchSelect = func;
		break;
		
		case UP_SWITCH:
			btn->upSwitchSelect = func;
		break;
		
		case DOWN_SWITCH:
			btn->downSwitchSelect = func;
		break;
		
		case LEFT_SWITCH:
			btn->leftSwitchSelect = func;
		break;
		
		case RIGHT_SWITCH:
			btn->rightSwitchSelect = func;
		break;
		
		default:

		break;
	}
}