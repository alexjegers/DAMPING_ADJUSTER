/*
 * encoder.c
 *
 * Created: 11/22/2022 1:48:58 PM
 *  Author: AJ992
 */ 
/*
 * encoder.c
 *
 * Created: 9/16/2022 11:39:38 AM
 *  Author: AJ992
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include "stepper.h"
#include "encoder.h"
#include "system.h"
#include "iic.h"

/*Port A interrupt for encoder*/
ISR(PORTA_PORT_vect)
{
	if (stepperTimeoutCnt() > 850 || stepperTimeoutCnt() < 550)
	{
		stepperIncrementTimeoutAmount();
	}
	else
	{
		stepperClearTimeoutAmount();
	}
	stepperClearTimeoutCnt();
	
	int8_t direction = encoderRotDirection();					//Determine direction motor is rotating.
	stepperIncrementPosition(direction);						//Add/subtract from the current position.
	PORTA.INTFLAGS = ENC_A_PIN;									//Clear interrupt flag.
}


/*
Function: encoderRotDirection
Params: none
Returns: -5 if motor is rotating CW(softer), 5 if rotation CCW (stiffer).
Description: Compares the level of the encoder outputs to determine the 
			direction of rotation. Gets called on an encoder A output 
			interrupt.
*/
int8_t encoderRotDirection()
{
	if (ENC_A_LEVEL == ENC_B_LEVEL)
	{
		return ROTATING_CW;
	}
	else
	{
		return ROTATING_CCW;
	}
}