/*
 * menu.h
 *
 * Created: 10/12/2022 11:18:30 AM
 *  Author: AJ992
 */ 


#ifndef MENU_H_
#define MENU_H_


	
	class form
	{
		public:
		void load();						//Call the load function off all the elements in the form.

		
		private:
		
	};
	
	class label : private ST7789
	{
		public:
		label(uint16_t x, uint16_t y, char* text, const uint8_t* font);
		label(uint16_t x, uint16_t y, uint8_t numText, const uint8_t* font);
		void load();
		
		private:
		const uint8_t* font;		//Pointer to font array.
		uint8_t y;					//Position of the vertical center of the button.
		uint8_t x;					//Position of the horizontal center of the button.
		char* text;					//Text to display in the button.
		uint8_t numChars;			//Number of characters in the text.
		uint8_t charWidth;			//Width of one character of the font being used.
		uint8_t charHeight;			//Height of one character of the font being used.
		uint8_t charPadding;		//Padding of the font being used.
		uint16_t stringWidth;		//Width of the whole string.
		uint16_t x0;				//Starting point of the string.
		uint16_t y0;				//Starting point of the string.		
	};

	class button : private ST7789
	{
		public:
		button(uint16_t x, uint16_t y, char* text, const uint8_t* font);
		void load();				//Write the contents of the button to the LCD driver.
		bool focus;					//Will toggle whether or not the button is highlighted.
		void setFocus();

		private:
		uint8_t y;					//Position of the vertical center of the button.
		uint8_t x;					//Position of the horizontal center of the button.
		char* text;					//Text to display in the button.
		uint8_t numChars;			//Number of characters in the text.
		uint8_t charWidth;			//Width of one character of the font being used.
		uint8_t charHeight;			//Height of one character of the font being used.
		uint8_t charPadding;		//Padding of the font being used.
		uint16_t stringWidth;		//Width of the whole string.
		uint16_t x0;				//Starting point of the string.
		uint16_t y0;				//Starting point of the string.
		uint16_t x0r;				//Starting point of rectangle.
		uint16_t y0r;				//Starting point of rectangle.
		uint16_t x1r;				//Ending point of rectangle.
		uint16_t y1r;				//Ending point of rectangle.
		const uint8_t* font;		//Pointer to font array.

	};

#endif /* MENU_H_ */