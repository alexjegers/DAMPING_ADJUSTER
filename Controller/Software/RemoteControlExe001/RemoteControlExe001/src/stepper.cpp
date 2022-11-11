/*
 * stepper.cpp
 *
 * Created: 11/9/2022 7:44:24 PM
 *  Author: AJ992
 */ 
#include <avr32/io.h>
#include <stdint.h>
#include "stepper.h"

STEPPER_MOTOR frontLeft;
STEPPER_MOTOR frontRight;
STEPPER_MOTOR rearLeft;
STEPPER_MOTOR rearRight;

void STEPPER_MOTOR::saveSettings()
{
	
}