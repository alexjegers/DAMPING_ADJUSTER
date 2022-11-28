/*
* iic.c
*
* Created: 9/17/2022 11:54:56 AM
*  Author: AJ992
*/

/*
Function:
Params:
Returns:
Description:
*/


#include <avr/io.h>
#include <avr/interrupt.h>
#include "stepper.h"
#include "iic.h"
#include "system.h"

TCB_t *iicTimeoutTimer = &TCB0;			//Pointer to the timer used for checking for timeout.
IIC_DATA_struct IIC_DATA;


ISR(TWI1_TWIS_vect)
{
	/*Get the starting address of p_IIC_DATA and find the end of it to make sure we don't write beyond it.*/
	uint8_t* p_IIC_DATA = &IIC_DATA;
	uint8_t size = sizeof(IIC_DATA);
	
	/*ACK the device address.*/
	iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);

	iicWaitForDataIntFlag();
	
	/*First byte is the offset from the start of the p_IIC_DATA struct where it will begin writing to.*/
	uint8_t startingOffset = iicReadSdata();
	uint8_t* maxOffset = p_IIC_DATA + size;	
	p_IIC_DATA += startingOffset;
	iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);
	
	/*While a stop condition hasn't been received, sit in a loop and wait for the data interrupt flag.*/
	while (iicAddrOrStopFlag() != 0)
	{
		if (iicDataIntFlag() != 0)
		{
			/*NACK a message that is trying to write to or read from out of the bounds of the struct.*/

			if (p_IIC_DATA > maxOffset)
			{
				iicWriteSdata(0x00);
				iicSendResponse(TWI_SCMD_COMPTRANS_gc, TWI_ACKACT_NACK_gc);
				break;
			}
			/*Read data in from SDATA.*/
			if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)
			{
				*p_IIC_DATA = iicReadSdata();
				p_IIC_DATA++;
				iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);
			}
			/*Write data to SDATA starting from &p_IIC_DATA + startingOffset.*/
			else
			{
				iicWriteSdata(*p_IIC_DATA);
				iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);
				p_IIC_DATA++;
			}
		}
		/*Check for a restart.*/
		if ((TWI1.SSTATUS & TWI_APIF_bm) != 0)
		{
			iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);
		}
	}
	
	/*Load the new data to the stepper motor.*/
	iicLoadToStepper();
}

/*
Function: iicInit
Params: none
Returns: none
Description: Sets up the TWI hardware.
*/
void iicInit()
{
	TWI1.SCTRLA = TWI_ENABLE_bm		//Slave enable.
	| TWI_APIEN_bm						//Address or stop interrupt enable.
	& ~(TWI_SMEN_bm						//Smart mode OFF.
	| TWI_PMEN_bm);						//Use address match logic.
	
	TWI1.SADDR = DEVICE_ADDR;						//Set the slave address.
	TWI1.DBGCTRL = 1;								//Run while debuggin.
	systemConfigTCB(iicTimeoutTimer, TCB_PER_1SEC);	//Configure the timeout timer to a 1 second timeout.
}

void iicEnableInterrupts()
{
	TWI1.SCTRLA |= TWI_APIEN_bm
				| TWI_DIEN_bm;
}

void iicDisableInterrupts()
{
	TWI1.SCTRLA &= ~(TWI_APIEN_bm
				| TWI_DIEN_bm);
}

/*
Function: iicAddrOrStop
Params: none
Returns: ADDRESS, STOP, or NONE
Description: Reads the TWI.STATUS register and returns ADDRESS if the APIF and AP
bits are setting signifying a start condition was issues. Returns
STOP if APIF is set but not AP signifying a STOP was issued. Returns
NONE if neither APIF nor AP are set signifying the interrupt was caused
by something else.
*/
int8_t iicAddrOrStopFlag()
{
	return TWI1.SSTATUS & TWI_AP_bm;
}


/*
Function: iicSendResponse
Params: TWI_SCMD_t, TWI_ACKACT_t types from avr header.
Returns: none
Description: Writes to TWI.SCTRLB register in response to data and address
interrupts. See data sheet for details on what the parameters do.
*/
void iicSendResponse(TWI_SCMD_t response, TWI_ACKACT_t ack)
{
	TWI1.SCTRLB = response | ack;
}

/*
Function: iicReadOrWrite
Params: none
Returns: 0 if master is writing, 1 if master is reading.
Description: Reads status register to see if the IIC master is requesting to read
or write.
*/
int8_t iicReadOrWriteFlag()
{
	return TWI1.SSTATUS & TWI_DIR_bm;
}

/*
Function: iicDataIntFlag
Params: none
Returns: State of the data interrupt flag.
Description: Returns zero if the flag is not set, non-zero if the flag is set.
*/
int8_t iicDataIntFlag()
{
	return TWI1.SSTATUS & TWI_DIF_bm;
}


/*
Function: iicWaitForDataIntFlag
Params: none
Returns: none
Description: Sits in a while loop until the data interrupt flag is set.
*/

void iicWaitForDataIntFlag()
{
	systemEnableTCB(iicTimeoutTimer);
	
	/*Wait for data*/
	while (iicDataIntFlag() == 0)
	{
		if (iicTimeoutTimer->INTFLAGS != 0)
		{
			LED1_ON;
			break;
		}
	}

	systemDisableTCB(iicTimeoutTimer);
}

/*
Function: iicReadData
Params: none
Returns: Data in TWI slave data (SDATA) register.
Description:
*/
char iicReadSdata()
{
	return TWI1.SDATA;
}

/*
Function: iicWriteData
Params: data
Returns: none
Description: writes data to the TWI slave data register (SDATA).
*/

void iicWriteSdata(uint8_t data)
{
	TWI1.SDATA = data;
}

/*
Function: iicKill
Params: none
Returns: none
Description: Disables slave mode.
*/
void iicKill()
{
	TWI1.CTRLA &= ~(TWI_ENABLE_bm);
}


void iicLoadFromStepper()
{
	IIC_DATA.position = stepperPosition();
	IIC_DATA.setPoint = stepperSetPoint();
	IIC_DATA.stepMode = stepperStepMode();
	IIC_DATA.currentLimit = stepperCurrentLimit();
	IIC_DATA.decayMode = stepperDecayMode();
	IIC_DATA.speedInRPM = stepperSpeed();
	IIC_DATA.flags = stepperFlags();
}

void iicLoadToStepper()
{
	stepperSetPosition(IIC_DATA.position);
	stepperSetSetPoint(IIC_DATA.setPoint);
	stepperSetStepMode(IIC_DATA.stepMode);
	stepperSetCurrent(IIC_DATA.currentLimit);
	stepperSetDecay(IIC_DATA.decayMode);
	stepperSetSpeed(IIC_DATA.speedInRPM);
	stepperSetFlag(IIC_DATA.flags);
}