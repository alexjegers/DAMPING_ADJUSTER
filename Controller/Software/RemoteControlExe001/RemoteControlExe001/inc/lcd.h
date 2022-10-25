/*
 * lcd.h
 *
 * Created: 10/11/2022 12:12:43 PM
 *  Author: AJ992
 */ 


#ifndef LCD_H_
#define LCD_H_

#define LCD_PORT					PORTA
#define LCD_BACKLIGHT_PIN_bm		1 << 11
#define LCD_CS_bm					1 << 12
#define LCD_RD_bm					1 << 13
#define LCD_DC_bm					1 << 14
#define LCD_WR_bm					1 << 15
#define LCD_RESET_bm				1 << 16

#define LCD_DATA_PORT_bm			PORTB
#define LCD_DATA_PINS_bm			255

/*ST7789V Macros*/
#define MADCTL_MY					1 << 7
#define MADCTL_MX					1 << 6
#define MADCTL_MV					1 << 5
#define MADCTL_ML					1 << 4
#define MADCTL_RGB					1 << 3
#define MADCTL_MH					1 << 2

/*Starting coordinates*/
#define X_START						10
#define X_END						239
#define Y_START						10
#define Y_END						300
#define TOTAL_PIXELS				((X_END + 1) - X_START) * ((Y_END + 1) - Y_START)


class ST7789
{
	public:
	void lcdInit();									//Configures pins and initializes LCD.
	void lcdWriteData8bit(uint8_t data);			//Writes one byte with DC high.
	void lcdWriteData16bit(uint16_t data);			//Writes 2 bytes with DC high.
	void lcdWriteAddr(uint8_t addr);				//Writes  1 byte with DC low.
	uint8_t lcdReadAddr(uint8_t addr, uint8_t numBytes); //Reads numByte bytes from the LCD.
	void lcdBegin();								//Pulls CS low.
	void lcdEnd();									//Pulls CS high.
	void waitForStatus();
	void readStatus();
	
	private:
	uint8_t lcdReadBuffer[5];						//Holds data when reading from the screen.
};



#endif /* LCD_H_ */