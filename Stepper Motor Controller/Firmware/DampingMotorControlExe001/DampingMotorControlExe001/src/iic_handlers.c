/*
 * iic_handlers.c
 *
 * Created: 9/18/2022 6:46:32 PM
 *  Author: AJ992
 */ 


#include <avr/io.h>
#include "stepper.h"
#include "system.h"
#include "iic.h"

/*For detailed descriptions of the data and access of each address see 
"IIC Procedure" document in "DAMPING_ADJUSTER > Docs" folder.*/

void handlerMotorSetPoint()
{
	uint16_t setPoint = 0;
	if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)
	{
		setPoint = iicRead(sizeof(setPoint));
		stepperSetSetPoint(setPoint);
	}
	else
	{
		setPoint = stepperSetPoint();
		iicWrite(&setPoint, sizeof(setPoint));
	}
}


void handlerMotorPosition()
{
	uint16_t position = 0;
	
	if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)
	{
		position = iicRead(sizeof(position));
		stepperSetPosition(position);
	}
	else
	{
		position = stepperPosition();
		iicWrite(&position, sizeof(position));
	}
}


void handlerMotorSpeed()
{
	uint16_t speed = 0;
	if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)
	{
		speed = iicRead(sizeof(speed));
		stepperSetSpeed(speed);
	}
	else
	{
		speed = stepperSpeed();
		iicWrite(&speed, sizeof(speed));
	}		
}


void handlerMotorCurrent()
{
	CURRENT_LIMIT_t current;
	
	if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)
	{
		current = iicRead(sizeof(current));
		stepperSetCurrent(current);
	}
	else
	{
		current = stepperCurrentLimit();
		iicWrite(&current, sizeof(current));
	}
}


void handlerMotorStepMode()
{
	STEP_MODE_t stepMode;
	
	if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)
	{
		stepMode = iicRead(sizeof(stepMode));
		stepperSetStepMode(stepMode);
	}
	else
	{
		stepMode = stepperStepMode();
		iicWrite(&stepMode, sizeof(stepMode));
	}
}


void handlerMotorStatusFlags()
{
	uint8_t flags = 0;
	
	if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)
	{
		iicRead(0);									//Immediately NACK the data, this is a read only address.
	}
	else
	{
		flags = stepperFlags();
		iicWrite(&flags, sizeof(flags));
	}
}


void handlerBatteryVoltage()
{
	uint16_t voltage = 0;
	if (iicReadOrWriteFlag() == IIC_REQUESTING_WRITE)
	{
		iicRead(0);									//Immediately NACK the data, this is a read only address.
	}
	else
	{
		voltage = systemADCResult();
		iicWrite(&voltage, sizeof(voltage));
	}
}