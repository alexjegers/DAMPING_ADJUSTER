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
#define LCD_RS_bm					1 << 14
#define LCD_WR_bm					1 << 15
#define LCD_RESET_bm				1 << 16

#define LCD_DATA_PORT_bm			PORTB
#define LCD_DATA_PINS_bm			255

void lcdInit();									//Configures pins and initializes LCD.
void lcdWriteData8bit(uint8_t data);			//Writes one byte with DC high.
void lcdWriteData16bit(uint16_t data);			//Writes 2 bytes with DC high.
void lcdWriteAddr(uint8_t addr);				//Writes  1 byte with DC low.
void lcdBegin();								//Pulls CS low.
void lcdEnd();									//Pulls CS high.
#endif /* LCD_H_ */