/*
 * encoder.c
 *
 * Created: 9/16/2022 11:39:38 AM
 *  Author: AJ992
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include "system.h"
#include "encoder.h"


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