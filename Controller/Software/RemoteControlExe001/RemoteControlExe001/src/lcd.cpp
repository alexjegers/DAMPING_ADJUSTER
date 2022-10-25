/*
 * lcd.cpp
 *
 * Created: 10/16/2022 6:59:16 PM
 *  Author: AJ992
 */ 

#define F_CPU					6000000 //Needed for delay.h

#include <avr32/io.h>
#include <stdint.h>
#include "system.h"
#include "io.h"
#include "interrupts.h"
#include "lcd.h"
#include <delay.h>



void ST7789::lcdInit()
{
	ioSetPinIO(&LCD_PORT, LCD_BACKLIGHT_PIN_bm | LCD_CS_bm | LCD_RD_bm
				| LCD_DC_bm | LCD_WR_bm | LCD_RESET_bm);				//All control pins as IO.
	ioSetPinOutput(&LCD_PORT, LCD_BACKLIGHT_PIN_bm | LCD_CS_bm | LCD_RD_bm
				| LCD_DC_bm | LCD_WR_bm | LCD_RESET_bm);				//All control pins as output.
	
	ioSetPinIO(&LCD_DATA_PORT_bm, LCD_DATA_PINS_bm);					//Set all the data pins as IO.
	ioSetPinOutput(&LCD_DATA_PORT_bm, LCD_DATA_PINS_bm);				//Set all data pins as outputs.
	
	ioPinHigh(&LCD_PORT, LCD_CS_bm | LCD_RESET_bm | LCD_RD_bm | LCD_WR_bm);			//Start with chip select high.
	//ioPinLow(&LCD_PORT, LCD_WR_bm);										//Start with the write signal low.

	delay_ms(100);

	/*HW Reset*/
	ioPinHigh(&LCD_PORT, LCD_RESET_bm);
	delay_ms(10);
	ioPinLow(&LCD_PORT, LCD_RESET_bm);
	delay_ms(100);
	ioPinHigh(&LCD_PORT, LCD_RESET_bm);
	delay_ms(1000);

	/*SW Reset*/
	lcdBegin();
	lcdWriteAddr(0x01);
	lcdEnd();
	
	delay_ms(500);

	/*Exit sleep mode*/
	lcdBegin();
	lcdWriteAddr(0x11);
	lcdEnd();
	
	/*Required after exiting sleep mode.*/
	delay_ms(500);			

	/*Select color mode RBG565*/
	lcdBegin();
	lcdWriteAddr(0x3A);
	lcdWriteData8bit(0x55);
	lcdEnd();

	/*MADCTL register pg 217, sets the orientation of the screen*/
	lcdBegin();
	lcdWriteAddr(0x36);
	lcdWriteData8bit(0);
	lcdEnd();
	

	
	/*Normal display mode as opposed to partial display mode*/
	lcdBegin();
	lcdWriteAddr(0x13);
	lcdEnd();

	/*Tearing effect off*/
	lcdBegin();
	lcdWriteAddr(0x34);
	lcdEnd();
	
	/*Display on*/
	lcdBegin();
	delay_ms(500);
	lcdWriteAddr(0x29);
	delay_ms(500);
	lcdEnd();
	
	delay_ms(120);

	delay_ms(1000);	
		
	/*Set range of screen from (0,0) to (239,319).*/
	lcdBegin();
	lcdWriteAddr(0x2A);			//X direction.
	lcdWriteData16bit(X_START);		//X Start.
	lcdWriteData16bit(X_END);		//X End.
	lcdEnd();
	
	waitForStatus();
		
	delay_ms(1000);

	lcdBegin();
	lcdWriteAddr(0x2B);			//Y Direction
	lcdWriteData16bit(Y_START);		//Y Start.
	lcdWriteData16bit(Y_END);		//Y End.
	lcdEnd();	

	waitForStatus();
			
	delay_us(100);

	lcdBegin();
	lcdWriteAddr(0x2C);
	for (uint32_t i = 0; i < TOTAL_PIXELS; i++)
	{
		lcdWriteData16bit(0xf800);
	}
	lcdEnd();
	
}


void ST7789::lcdWriteData8bit(uint8_t data)
{
	ioPinHigh(&LCD_PORT, LCD_DC_bm);								//DC pin high for data.
	ioPinHigh(&LCD_PORT, LCD_RD_bm);								//Read pin high for write.
	
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Release latch.
	
	delay_us(100);	
	
	/*Write the data to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, 
				(uint32_t)data & 0x000000FF);						//Cast and mask to not overwrite the rest of Port B.
	ioPinLow(&LCD_DATA_PORT_bm, 
				(~(uint32_t)data) & 0x000000FF);					//Cast and mask to not overwrite the rest of Port B.
	
	ioPinHigh(&LCD_PORT, LCD_WR_bm);								//Latch in the data.
	
	delay_us(100);													//Wait for a bit.
	

	
}

void ST7789::lcdWriteData16bit(uint16_t data)
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
	
	ioPinHigh(&LCD_PORT, LCD_DC_bm);								//DCX pin high for data.
	ioPinHigh(&LCD_PORT, LCD_RD_bm);								//RD pin high for write.
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Release latch.

	delay_us(100);													//Wait for a bit	
	/*Write the msb to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, 
				(uint32_t)dataUnion.bytes.msb & 0x000000FF);		//Cast and mask to not overwrite the rest of Port B.
	ioPinLow(&LCD_DATA_PORT_bm, 
				(~(uint32_t)dataUnion.bytes.msb) & 0x000000FF);		//Cast and mask to not overwrite the rest of Port B.
	ioPinHigh(&LCD_PORT, LCD_WR_bm);								//Latch in the data.
	
	delay_us(1000);													//Wait for a bit.
	
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Release latch.
	
	delay_us(100);													//Wait a bit.
	
	/*Write the lsb to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, 
			(uint32_t)dataUnion.bytes.lsb & 0x000000FF);			//Cast and mask to not overwrite the rest of Port B.
	ioPinLow(&LCD_DATA_PORT_bm, 
			(~(uint32_t)dataUnion.bytes.lsb) & 0x000000FF);			//Cast and mask to not overwrite the rest of Port B.
	
	delay_us(100);
	ioPinHigh(&LCD_PORT, LCD_WR_bm);								//Latch in the data.	
	
	delay_us(100);													//Wait for a bit.
	

}


void ST7789::lcdWriteAddr(uint8_t command)
{
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Release latch.		

	delay_us(100);	
	
	ioPinLow(&LCD_PORT, LCD_DC_bm);									//DC pin low for addr.
	ioPinHigh(&LCD_PORT, LCD_RD_bm);								//Read pin high for write.
	
	/*Write the msb to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, (uint32_t)command & 0x000000FF);
	ioPinLow(&LCD_DATA_PORT_bm, (~(uint32_t)command) & 0x000000FF);
	
	ioPinHigh(&LCD_PORT, LCD_WR_bm);								//Latch in data.
	
	delay_us(100);													//Wait a bit
	

}


uint8_t ST7789::lcdReadAddr(uint8_t addr, uint8_t numBytes)
{
	ioPinLow(&LCD_PORT, LCD_DC_bm);									//DC pin low for addr.
	
	/*Write the msb to the bus*/
	ioPinHigh(&LCD_DATA_PORT_bm, (uint32_t)addr & 0x000000FF);
	ioPinLow(&LCD_DATA_PORT_bm, (~(uint32_t)addr) & 0x000000FF);
	ioPinHigh(&LCD_PORT, LCD_WR_bm);								//Latch in the addr and leave it high.
	
	ioSetPinInput(&LCD_DATA_PORT_bm, LCD_DATA_PINS_bm);				//Configure data pins as inputs.
	ioPinLow(&LCD_PORT, LCD_RD_bm);									//Read pin low, ready read data.
	ioPinHigh(&LCD_PORT, LCD_DC_bm);								//DC pin high for data.
	delay_us(10);
	
	for (uint8_t i = 0; i < numBytes; i++)
	{
		ioPinHigh(&LCD_PORT, LCD_RD_bm);							//Read in data.
		delay_us(10);						
		lcdReadBuffer[i] = ioReadPort(&LCD_DATA_PORT_bm);			//Read the port.
		delay_us(10);
		ioPinLow(&LCD_PORT, LCD_RD_bm);								//Release the latch
		delay_us(10);	
	}
	ioSetPinOutput(&LCD_DATA_PORT_bm, LCD_DATA_PINS_bm);			//Return pins to outputs.
	lcdEnd();														//CS high.
	ioPinHigh(&LCD_PORT, LCD_RD_bm);								//Return read to high.
	ioPinLow(&LCD_PORT, LCD_WR_bm);									//Return write to low.
	return lcdReadBuffer[3];
}

void ST7789::lcdBegin()
{
	ioPinLow(&LCD_PORT, LCD_CS_bm);
	delay_ms(5);
}

void ST7789::lcdEnd()
{
	ioPinHigh(&LCD_PORT, LCD_CS_bm);
	delay_ms(5);
}

void ST7789::waitForStatus()
{
	while ((lcdReadBuffer[1] == 0x09) && (lcdReadBuffer[2] == 0x09) && (lcdReadBuffer[3] == 0x09) && (lcdReadBuffer[4] == 0x09))
	{
	}
}

void ST7789::readStatus()
{
	/*Read status register*/
	lcdBegin();
	lcdReadAddr(0x09, 5);
	lcdEnd();

}