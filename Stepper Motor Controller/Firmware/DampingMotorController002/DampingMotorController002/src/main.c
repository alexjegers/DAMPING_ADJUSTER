/*
 * DampingMotorControlExe001.c
 *
 * Created: 9/7/2022 7:48:13 PM
 * Author : AJ992
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "stepper.h"
#include "system.h"
#include "iic.h"




int main(void)
{	
	/*Enable global interrupts*/
	sei();											

	/*Configure system clock*/
	systemClockConfig();
	
	/*Configure pins in each port*/
	systemPortAConfig();
	systemPortCConfig();
	systemPortDConfig();
	systemPortFConfig();
	
	/*Enable TCA0 which is used as a source for TCBx*/
	systemEnableTCA0();

	/*Configure ADC to read car battery/stepper supply voltage*/
	systemConfigADC();
	
	/*Load default stepper settings*/
	stepperInit();
		
	/*Populate the IIC data struct that the master will read and write to.*/
	iicLoadFromStepper();

	/*Wait until the correct position is written from the master or go to zero flag is set.*/
	stepperSetFlag(FLAG_STARTUP_bm);	
	while ((stepperPosition() == -1) || (stepperSetPoint() == -1)) 
	{
		/*If the goToZero flag is set, run stepperGoToZero.*/
		if ((stepperFlags() & FLAG_GO_TO_ZERO_bm) != 0)
		{
			stepperGoToZero();
		}
	}
	stepperClearFlag(FLAG_STARTUP_bm);
	
    while (1) 
    {
		/*While the stepper position is not equal to the set point...*/
		while ((stepperAtSetPoint() == 0))					
		{
			/*Disable IIC interrupts while the motor is moving, dont want to miss an encoder interrupt.*/
			iicDisableInterrupts();
			
			/*NACK any IIC message that does come through.*/
			iicSendResponse(TWI_SCMD_COMPTRANS_gc, TWI_ACKACT_NACK_gc);
			
			/*Clear timeout flag in case it was set beforehand.*/
			stepperClearFlag(FLAG_TIMEOUT_bm);

			/*If the stepper isn't already moving, start the timeout timer*/
			if (stepperIsMoving() == false)				
			{
				stepperClearTimeoutAmount();						//Clear the current timeout amount.
				stepperClearTimeoutCnt();							//Clear the actual timer count register.
				stepperStartTimeoutTimer();							//Start the timer.
			}
			
			/*If the stepper timesout more than 10 times in a row while moving, assume
				that it is stuck, change the set point to the position that it's stuck
				on, and stop trying to move it.*/
			if (stepperTimeoutAmount() > 10)
			{
				stepperSetSetPoint(stepperPosition());
				stepperSetFlag(FLAG_TIMEOUT_bm);
				break;
			}
			
			/*Determine direction of rotation.*/
			if (stepperPosition() < stepperSetPoint())
			{
				stepperSetDirection(DIRECTION_CW);
			} 
			else
			{
				stepperSetDirection(DIRECTION_CCW);
			}
			
			/*Start moving*/
			stepperStartMove();
			STEPPER_ENABLE;
		}
		
		/*When the stepper position is equal to the set point*/
		STEPPER_DISABLE;
		stepperStopMove();
		stepperStopTimeoutTimer();			
		iicLoadFromStepper();	
		iicEnableInterrupts();
		
		/*If the go to zero flag is set run the go to zero function.*/
		if ((stepperFlags() & FLAG_GO_TO_ZERO_bm) != 0)
		{
			stepperGoToZero();
		}		
    }
}

