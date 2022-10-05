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
#include "eeprom.h"



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
		
	/*Wait until the correct position is written from the master.*/
	while ((stepperPosition() == -1) || (stepperSetPoint() == -1)) 
	{
		if ((stepperFlags() & FLAG_GO_TO_ZERO_bm) != 0)
		{
			stepperGoToZero();
		}
	}
	
    while (1) 
    {
		/*While the stepper position is not equal to the set point*/
		while (stepperAtSetPoint() == 0)					
		{
			if (stepperPosition() < stepperSetPoint())
			{
				stepperSetDirection(DIRECTION_CW);
			} 
			else
			{
				stepperSetDirection(DIRECTION_CCW);
			}
			stepperStartMove();
			STEPPER_ENABLE;
		}
		
		/*When the stepper position is equal to the set point*/
		STEPPER_DISABLE;
		stepperStopMove();
		
		
		/*If the go to zero flag is set run the go to zero function*/
		if ((stepperFlags() & FLAG_GO_TO_ZERO_bm) != 0)
		{
			stepperGoToZero();
		}		
    }
}

