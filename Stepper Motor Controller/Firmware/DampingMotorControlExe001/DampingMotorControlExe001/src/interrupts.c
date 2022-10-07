/*
 * interrupts.c
 *
 * Created: 9/7/2022 8:30:47 PM
 *  Author: AJ992
 */ 

#include <avr/interrupt.h>
#include "stepper.h"
#include "encoder.h"
#include "system.h"
#include "iic.h"

/*Port A interrupt for encoder*/
ISR(PORTA_PORT_vect)
{
	stepperClearTimeoutTimer();									//Reset the timeout counter because it just moved a step.
	int8_t direction = encoderRotDirection();					//Determine direction motor is rotating.
	stepperIncrementPosition(direction);						//Add/subtract from the current position.
	PORTA.INTFLAGS = ENC_A_PIN;									//Clear interrupt flag.
}

/*IIC interrupt for incoming data*/
ISR(TWI1_TWIS_vect)
{
	if (iicReadInternalAddress() == BASE_ADDRESS)								//If it's starting for the first time.
	{
		if (iicAddrOrStopFlag() == ADDRESS_FLAG)											//And it's not a stop.
		{
			if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)						//And it's requesting to write means it going to be an internal address.
			{
				iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);		//Acknowledge
				iicWaitForDataIntFlag();
				iicSetInternalAddress((INTERNAL_ADDRESS_t)iicReadSdata());		//Save the internal address.
				iicSendResponse(TWI_SCMD_COMPTRANS_gc, TWI_ACKACT_NACK_gc);		//Acknowledge
			}
		}
	}
	else //Not the BASE_ADDRESS means the internal address is already written.
	{
		iicSendResponse(TWI_SCMD_RESPONSE_gc, TWI_ACKACT_ACK_gc);
		while (iicDataIntFlag() == 0) {}		
		iicHandleData(iicReadInternalAddress());
	}
	
}


/*IIC timeout interrupt.*/
ISR(TCB0_INT_vect)
{
	iicKill();							//Disable IIC slave.
	iicInit();							//Enable IIC slave.
	TCB0.INTFLAGS = TCB_CAPT_bm;		//Clear interrupt flag.
}

/*Stepper timeout interrupt.*/
ISR(TCB1_INT_vect)
{
	stepperIncrementTimeoutCount();		//Increment timeout counter.
	TCB1.INTFLAGS = TCB_CAPT_bm;		//Clear the interrupt flag.
}