/*
 * eeprom.c
 *
 * Created: 9/21/2022 11:41:01 AM
 *  Author: AJ992
 */ 

#include <avr/io.h>
#include "eeprom.h"


/*
Function:
Params:
Returns:
Description:
*/
void eepromWriteByte(uint8_t *src, uint8_t *dst)
{
	CCP = CCP_SPM_gc;		
	NVMCTRL.CTRLA = NVMCTRL_CMD_EEERWR_gc;
	eepromWaitWhileBusy();
	*dst = *src;
	CCP = CCP_SPM_gc;
	NVMCTRL.CTRLA = NVMCTRL_CMD_NOOP_gc;
}


/*
Function:
Params:
Returns:
Description:
*/
void eepromReadByte(uint8_t *src, uint8_t *dst)
{
	*dst = *src;
}


/*
Function:
Params:
Returns:
Description:
*/
void eepromWriteBlock(void *src, uint8_t* dst, uint8_t size)
{
	CCP = CCP_SPM_gc;								//System instruction register unlock.
	NVMCTRL.CTRLA = NVMCTRL_CMD_EEERWR_gc;			//EEPROM erase/write command.
	
	uint8_t *pSrc = (uint8_t*)src;					//Cast void* to uint8_t*.
	for (uint8_t i = 0; i < size; i++)				
	{
		eepromWaitWhileBusy();
		*(dst++) = *(pSrc++);						//Copy RAM data to EEPROM.
	}
		
	CCP	= CCP_SPM_gc;								//System instruction register unlock.
	NVMCTRL.CTRLA = NVMCTRL_CMD_NOOP_gc;			//EEPROM no operation command.
}


/*
Function:
Params:
Returns:
Description:
*/
void eepromReadBlock(uint8_t *src, void *dst, uint8_t size)
{
	uint8_t *pDst = (uint8_t*)dst;
	for (uint8_t i = 0; i < size; i++)
	{
		*(pDst++) = *src++;
	}
}


void eepromWaitWhileBusy()
{
	while ((NVMCTRL.STATUS & NVMCTRL_EEBUSY_bm) != 0) {}
}