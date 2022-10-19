/*
 * lcd.cpp
 *
 * Created: 10/16/2022 6:59:16 PM
 *  Author: AJ992
 */ 

#include <avr32/io.h>
#include <stdint.h>
#include "system.h"
#include "io.h"
#include "interrupts.h"
#include "lcd.h"
#include <delay.h>


void lcdInit()
{
	ioSetPinIO(&LCD_PORT, LCD_BACKLIGHT_PIN_bm | LCD_CS_bm | LCD_RD_bm
				| LCD_RS_bm | LCD_WR_bm | LCD_RESET_bm);				//All control pins as IO.
	ioSetPinOutput(&LCD_PORT, LCD_BACKLIGHT_PIN_bm | LCD_CS_bm | LCD_RD_bm
				| LCD_RS_bm | LCD_WR_bm | LCD_RESET_bm);				//All control pins as output.
	
	ioSetPinIO(&LCD_DATA_PORT_bm, LCD_DATA_PINS_bm);					//Set all the data pins as IO.
	ioSetPinOutput(&LCD_DATA_PORT_bm, LCD_DATA_PINS_bm);				//Set all data pins as outputs.
	
	ioPinHigh(&LCD_PORT, LCD_CS_bm);									//Start with chip select high.
	ioPinLow(&LCD_PORT, LCD_WR_bm);										//Start with the write signal low.

	/*SW Reset*/
	lcdBegin();
	lcdWriteAddr(0x01);
	lcdEnd();
	
	/*Exit sleep mode*/
	lcdBegin();
	lcdWriteAddr(0x11);
	lcdEnd();
	
	/*Required after exiting sleep mode.*/
	delay_ms(250);			
	
	/*Select color mode RBG565*/
	lcdBegin();
	lcdWriteAddr(0x3A);
	lcdWriteData8bit(0x55);
	lcdEnd();
	
	/*Set range of screen from (0,0) to (239,319).*/
	lcdBegin();
	lcdWriteAddr(0x2A);
	lcdWriteData16bit(0x00);
	lcdWriteData16bit(239);
	lcdEnd();
	lcdBegin();
	lcdWriteAddr(0x2B);
	lcdWriteData16bit(0);
	lcdWriteData16bit(319);
	lcdEnd();
	
	/*Normal display mode as opposed to partial display mode*/
	lcdBegin();
	lcdWriteAddr(0x13);
	lcdEnd();
	
	/*MADCTL register pg 217, sets the orientation of the screen*/
	lcdBegin();
	lcdWriteAddr(0x36);
	lcdWriteData8bit(0x78);
	lcdEnd();
	
	/*Tearing effect off*/
	lcdBegin();
	lcdWriteAddr(0x35);
	lcdEnd();
	
	lcdBegin();
	lcdWriteAddr(0x2C);
	for (uint32_t i = 0; i < 76000; i++)
	{
		lcdWriteData16bit(0xFFFF);
	}
	lcdEnd();
}


void lcdWriteData8bit(uint8_t data)
{
	ioPinHigh(&LCD_PORT, LCD_RS_bm);								//DC pin high for data.
	
	/*Write the data to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, 
				(uint32_t)data & 0x000000FF);						//Cast and mask to not overwrite the rest of Port B.
	ioPinLow(&LCD_DATA_PORT_bm, 
				(~(uint32_t)data) & 0x000000FF);					//Cast and mask to not overwrite the rest of Port B.
	ioPinHigh(&LCD_PORT, LCD_WR_bm);								//Latch in the data.
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Release latch.
	
}

void lcdWriteData16bit(uint16_t data)
{
	/*One pixel is 16 bits, can only send 8 bits at a time.*/
	union dataSplit
	{
		uint16_t data;
		struct bytes 
		{
			uint8_t msb;
			uint8_t lsb;
		}bytes;
	}dataUnion;
	
	dataUnion.data = data;
	
	ioPinHigh(&LCD_PORT, LCD_RS_bm);								//DC pin high for data.
	
	/*Write the msb to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, 
				(uint32_t)dataUnion.bytes.msb & 0x000000FF);		//Cast and mask to not overwrite the rest of Port B.
	ioPinLow(&LCD_DATA_PORT_bm, 
				(~(uint32_t)dataUnion.bytes.msb) & 0x000000FF);		//Cast and mask to not overwrite the rest of Port B.
	ioPinHigh(&LCD_PORT, LCD_WR_bm);								//Latch in the data.
	
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Release latch.
	
	/*Write the lsb to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, 
			(uint32_t)dataUnion.bytes.lsb & 0x000000FF);			//Cast and mask to not overwrite the rest of Port B.
	ioPinLow(&LCD_DATA_PORT_bm, 
			(~(uint32_t)dataUnion.bytes.lsb) & 0x000000FF);			//Cast and mask to not overwrite the rest of Port B.
	ioPinHigh(&LCD_PORT, LCD_WR_bm);								//Latch in the data.	
	
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Release latch.
}


void lcdWriteAddr(uint8_t command)
{
	ioPinLow(&LCD_PORT, LCD_RS_bm);									//DC pin high for data.
	
	/*Write the msb to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, (uint32_t)command & 0x000000FF);
	ioPinLow(&LCD_DATA_PORT_bm, (~(uint32_t)command) & 0x000000FF);
	
	ioPinHigh(&LCD_PORT, LCD_WR_bm);
	
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Release latch.		
}

void lcdBegin()
{
	ioPinLow(&LCD_PORT, LCD_CS_bm);
}


void lcdEnd()
{
	ioPinHigh(&LCD_PORT, LCD_CS_bm);
}