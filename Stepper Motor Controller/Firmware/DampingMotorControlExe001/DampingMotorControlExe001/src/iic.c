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
#include "stepper.h"
#include "iic.h"
#include "system.h"

TWI_t *pIIC = &TWI1;					//Pointer to TWI hardware.
INTERNAL_ADDRESS_t internalAddress;		//Holds the current internal address written from master.
TCB_t *iicTimeoutTimer = &TCB0;			//Pointer to the timer used for checking for timeout.

/*
Function: iicInit
Params: none
Returns: none
Description: Sets up the TWI hardware.
*/
void iicInit()
{
	pIIC->SCTRLA = (TWI_ENABLE_bm					//Slave enable.
				| TWI_APIEN_bm						//Address or stop interrupt enable.
				| TWI_DIEN_bp)						//Data interrupt enable.
				& ~(TWI_SMEN_bm						//Smart mode OFF.
				| TWI_PMEN_bm);						//Use address match logic.
				
	pIIC->SADDR = DEVICE_ADDR;						//Set the slave address.
	pIIC->DBGCTRL = 1;								//Run while debuggin.
	systemConfigTCB(iicTimeoutTimer, TCB_PER_1SEC);	//Configure the timeout timer to a 1 second timeout.
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
	if ((pIIC->SSTATUS & TWI_APIF_bm) == 0)
	{
		return NONE_FLAG;
	}
	else
	{
		if ((pIIC->SSTATUS & TWI_AP_bm) == 0)
		{
			return STOP_FLAG;
		}
		return ADDRESS_FLAG;
	}
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
	pIIC->SCTRLB = response | ack;
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
	return pIIC->SSTATUS & TWI_DIR_bm;
}

/*
Function: iicDataIntFlag
Params: none
Returns: State of the data interrupt flag.
Description: Returns zero if the flag is not set, non-zero if the flag is set.
*/
int8_t iicDataIntFlag()
{
	return pIIC->SSTATUS & TWI_DIF_bm;
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
			iicSetInternalAddress(BASE_ADDRESS);
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
	return pIIC->SDATA;
}

/*
Function: iicSetInternalAddress
Params: setInternalAddress ("address" of data the master is trying to access).
Returns: none
Description: sets the internalAddress enum.
*/
void iicSetInternalAddress(INTERNAL_ADDRESS_t setInternalAddr)
{
	internalAddress = setInternalAddr;
}


/*
Function: iicReadInternalAddress
Params: none
Returns: current state of internalAddress enum.
Description:
*/
INTERNAL_ADDRESS_t iicReadInternalAddress(void)
{
	return internalAddress;
}


/*
Function: iicWriteData
Params: data
Returns: none
Description: writes data to the TWI slave data register (SDATA).
*/

void iicWriteSdata(uint8_t data)
{
	pIIC->SDATA = data;
}

/*
Function: iicHandleData
Params: internalAddress
Returns: none
Description: Sends or receives requested data determined by internal address and
			iicReadOrWrite.
*/
void iicHandleData(INTERNAL_ADDRESS_t internalAddress)
{
	switch (internalAddress)
	{
		case BASE_ADDRESS:
		break;
		
		case MOTOR_SET_POINT:
			handlerMotorSetPoint();			
		break;
		
		case MOTOR_POSITION:
			handlerMotorPosition();
		break;
		
		case MOTOR_SPEED:
			handlerMotorSpeed();
		break;
		
		case MOTOR_CURRENT:
			handlerMotorCurrent();
		break;
		
		case MOTOR_STEP_MODE:
			handlerMotorStepMode();
		break;
		
		case MOTOR_STATUS_FLAGS:
			handlerMotorStatusFlags();
		break;
		
		case BATTERY_VOLTAGE:
			handlerBatteryVoltage();
		break;
		
		case MOTOR_GO_TO_ZERO:
			if (iicRead(1) == 0)
			{
				stepperSetFlag(FLAG_GO_TO_ZERO_bm);
			}
		break;
		
		default:
			iicRead(1);										//Process the data in the register to keep the bus from hanging but don't do anything with it.
			iicSetInternalAddress(BASE_ADDRESS);			//Reset to base address.
		break;
	}		
	iicSetInternalAddress(BASE_ADDRESS);					//Reset to base address after handling data.
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

/*
Function: iicWrite
Params: *dataAddr: Pointer to the first byte of data that is going to be transmitted.
		size: size of the data (in bytes) at this address.
Returns: none
Description: Used in response to a IIC Master read request. Puts data on the bus and handles
			the acks/nacks and completes the transaction.
*/
void iicWrite(void *dataAddr, uint8_t size)
{
	uint8_t *data = (uint8_t*)dataAddr;
	
	for (int8_t i = 0; i < size; i++)
	{
		iicWriteSdata(*(data++));
		iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);
		iicWaitForDataIntFlag();
	}
	
	iicSendResponse(TWI_SCMD_COMPTRANS_gc, TWI_ACKACT_ACK_gc);
}

/*
Function: iicRead
Params: size: number of bytes to read in.
Returns: uint64_t
Description: Used in response to an IIC Master write request. Reads in up to
			4 bytes of data and returns them as one whole 64 bit unsigned int.
*/
uint64_t iicRead(uint8_t size)
{
	uint64_t incomingData = 0;
	uint8_t *pIncomingData = &incomingData;
	
	for (uint8_t i = 0; i < size - 1; i++)
	{
		*(pIncomingData++) = (uint8_t)iicReadSdata();
		iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);
		iicWaitForDataIntFlag();		
	}
		*(pIncomingData++) = (uint8_t)iicReadSdata();
		iicSendResponse(TWI_SCMD_COMPTRANS_gc, TWI_ACKACT_NACK_gc);
		iicWaitForDataIntFlag();	
	return incomingData;
}