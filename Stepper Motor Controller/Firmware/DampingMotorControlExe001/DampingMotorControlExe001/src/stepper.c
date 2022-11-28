/*
 * stepper.c
 *
 * Created: 9/11/2022 10:04:36 PM
 *  Author: Alex Jegers
 */ 

/*
Function:
Params:
Returns:
Description:
*/

#include <avr/io.h>
#include <stdbool.h>
#include "system.h"
#include "stepper.h"
#include <avr/eeprom.h>
#include "eeprom.h"


STEPPER_INFO_t motor;								//Stepper object.
STEPPER_INFO_t *pMotor = &motor;					//Pointer to stepper object.
STEPPER_INFO_t motorPrev;							//Holds a copy of motor to see if anything was changed.
STEPPER_INFO_t *pMotorPrev = &motorPrev;			//Pointer to motorCopy.
TCB_t *stepperTimeoutTimer = &TCB1;					//Timer to see if stepper times out and/or gets stuck.

/*
Function: stepperInit
Params: none
Returns: none
Description: Configures the stepper motor driver to default settings.
			Configures a timer to be used at the timeout timer.
			Most importantly, sets the position and set point to -1, this
			is so I can wait to begin the main program until the master writes
			the actual current set point and position from the masters EEPROM 
			memory.
*/
void stepperInit()
{
	/*Default stepper settings*/
	STEPPER_DISABLE;
	stepperSetCurrent(TWO_AMP_bm);
	stepperSetStepMode(MODE_HALF_STEP_bm);
	stepperSetSpeed(100);
	stepperSetDecay(DECAY_MIXED);
	
	/*Configures the TCB used for timeout checking*/
	stepperConfigTimeoutTimer(TCB_PER_1SEC);	
	
	/*Have to directly access these because the function won't allow -1*/
	pMotor->position = -1;
	pMotor->setPoint = -1;
	pMotor->previousPosition = -1;
}


/*
Function: stepperStartMove
Params: none
Returns: none
Description: Calculates the frequency needed based on desired RPM and step mode and
		starts the PWM signal connected to the step pin on the stepper driver.
*/
void stepperStartMove()
{
	/*Using TCB2 on PC0 for PWM*/
	uint16_t frequency = 0;										//Will hold the necessary PWM frequency.

	/*Calculates frequency based on which step mode the driver is in*/
	switch (pMotor->stepMode)
	{
		case MODE_FULL_STEP_bm:
		frequency = (STEPS_FULL_STEP * pMotor->speedInRPM) / 60;
		break;
		
		case MODE_HALF_STEP_bm:
		frequency = (STEPS_HALF_STEP * pMotor->speedInRPM) / 60;
		break;
		
		case MODE_QUARTER_STEP_bm:
		frequency = (STEPS_QUARTER_STEP * pMotor->speedInRPM) / 60;
		break;
		
		case MODE_EIGTH_STEP_bm:
		frequency = (STEPS_EIGTH_STEP * pMotor->speedInRPM) / 60;
		break;
		
		case MODE_SIXTEENTH_STEP_bm:
		frequency = (STEPS_SIXTEENTH_STEP * pMotor->speedInRPM) / 60;
		break;
		
		case MODE_THIRTY_SECOND_STEP_bm:
		frequency = (STEPS_THIRTY_SECOND_STEP * pMotor->speedInRPM) / 60;
		break;
		
		case MODE_CLEAR_REGISTER_bm:
		break;
	}
	
	/*If somehow the frequency ends up being zero, set the error flag and return
		so we don't divide by zero*/
	if (frequency == 0)
	{
		pMotor->flags |= FLAG_ERROR_bm;
		return;
	}
	
	uint8_t compareValue = 23437 / frequency;					//23437 is the frequency of TCA0.
	TCB2.CCMPL = compareValue;									//PWM period.  
	TCB2.CCMPH = compareValue / 2;								//PWM duty cycle.
	TCB2.CTRLA |= TCB_ENABLE_bm;								//Enable TCB2.
}


/*
Function: stepperSetMode
Params:	mode, from STEPPER_MODE enum
Returns: none
Description: sets the step mode for the motor (full step, half step, etc.)
*/
void stepperSetStepMode(STEP_MODE_t mode)
{
	pMotor->stepMode = mode;
	PORTD.OUT &= MODE_CLEAR_REGISTER_bm;							//Clear the current mode setting out of the bits in port D.
	PORTD.OUT |= pMotor->stepMode;									//Set to the new mode.
}


/*
Function: stepperSetSpeed
Params: speedInRPM
Returns: none
Description: writes to the speedInRPM member of the motor data structure.
*/
void stepperSetSpeed(uint16_t speedInRPM)
{
	pMotor->speedInRPM = speedInRPM;
}


/*
Function:  stepperStopMove
Params: none
Returns: none
Description: Stops the PWM timer driving the step pin on DRV8825.
*/
void stepperStopMove()
{
	TCB2.CTRLA &= ~(TCB_ENABLE_bm);
}


/*
Function: stepperIncrementPosition
Params: direction (-1 or 1)
Returns: none
Description: Adds direction to the current position of the motor.
			Direction should only ever be -5 or 5. This is because there's 2 detents
			for every damping adjustment 1 so position is multiplied by 10 so I
			can do integer math. 5 is essentially .5 and 10 is 1.
*/
void stepperIncrementPosition(int8_t direction)
{
	pMotor->previousPosition = pMotor->position;		//Record the current position before changing it.
	pMotor->position += direction;						//Add the change in rotation to current position.			
}


/*
Function: stepperSetDecay
Params: decayMode
Returns: none
Description: Takes parameter from DECAY_MODE_t.
*/
void stepperSetDecay(DECAY_MODE_t decayMode)
{
	PORTD.DIR |= DECAY_PIN;							//Set decay pin as output in case it's set to input from previous decay mode setting.
	PORTD.PIN7CTRL = 0;								//Clear the input disable bit from mixed decay.

	switch (decayMode)
	{
		case DECAY_FAST:
		PORTD.OUT |= DECAY_PIN;						//Decay high for fast decay.
		break;
		
		case DECAY_SLOW:
		PORTD.OUT &= ~(DECAY_PIN);					//Decay low for slow decay.
		break;
		
		/*Default case is mixed decay*/
		default:
		PORTD.DIR &= ~(DECAY_PIN);					//Decay as input.
		PORTD.PIN7CTRL = PORT_ISC_INPUT_DISABLE_gc;	//Disable input buffer, decay pin open for mixed decay.
		decayMode = DECAY_MIXED;					
		break;		
		
	}		
	pMotor->decayMode = decayMode;					//Write decay mode to motor struct so it can be read later.
}


/*
Function:stepperSetDirection
Params: direction
Returns: none
Description: Changes the state of the DIR_PIN, high is CW, low is CCW.
*/
void stepperSetDirection(uint8_t direction)
{
	if (direction == DIRECTION_CCW)
	{
		PORTC.OUT |= DIR_PIN;
	}
	else
	{
		PORTC.OUT &= ~(DIR_PIN);
	}
}


/*
Function: stepperSetCurrent
Params: current
Returns: none
Description: Changes the DAC output to adjust the current limit of the DRV8285.
			Correlation between voltage and current limit in "Current loop" Excel file.
*/
void stepperSetCurrent(CURRENT_LIMIT_t current)
{
	/*If current is in range, change motor.currentLimit*/
	if (current > 0 && current < 1023)
	{
		pMotor->currentLimit = current;

	}
	/*If it's not in range set motor.currentLimit to ONE_AND_HALF_AMP.*/
	else
	{
		pMotor->currentLimit = ONE_AND_HALF_AMP_bm;
	}
	
	/*Write the value to the DAC*/
	DAC0.CTRLA = 0;
	pMotor->current.dacData = (pMotor->currentLimit << 6);
	DAC0.DATAL = pMotor->current.dacStruct.lsb;
	DAC0.DATAH = pMotor->current.dacStruct.msb;
	DAC0.CTRLA = DAC_ENABLE_bm | DAC_OUTEN_bm;
}


/*
Function: stepperSetSetPoint
Params: setPoint
Returns: none
Description: writes the set point to STEPPER_INFO struct.
*/
void stepperSetSetPoint(int16_t setPoint)
{
	/*Makes sure setPoint is a multiple of 10*/
	setPoint = setPoint / 10;
	setPoint = setPoint * 10;
	
	/*Only change the set point if it is within range.*/
	if (setPoint >= 0 && setPoint <= 300)
	{
		pMotor->setPoint = setPoint;		
	}
}

/*
Function: stepperSetPosition
Params: position
Returns: none
Description: Writes param position to position of stepper motor struct.
*/
void stepperSetPosition(int16_t position)
{
	pMotor->previousPosition = pMotor->position;				//Record the current position before changing it.

	/*Makes sure position is a multiple of 10*/
	position = position / 10;
	position = position * 10;
	
	/*Only change if new position is within range*/
	if (position <= 300 && position >= 0)
	{
		pMotor->position = position;	
	}
}


/*
Function: stepperAtSetPoint
Params: none
Returns: true/false
Description: Returns 0 if position != set point.
			Returns non-zero if position == set point.
*/
uint8_t stepperAtSetPoint()
{
 	if (pMotor->position == pMotor->setPoint)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/*
Function: stepperPosition
Params: none
Returns: position from motor struct.
Description: Used to access position member in STEPPER_INFO_struct.
*/
int16_t stepperPosition()
{
	return pMotor->position;
}

/*
Function: stepperSetPoint
Params: none
Returns: setPoint
Description: Used to access setPoint in STEPPER_MOTOR_struct.
*/
int16_t stepperSetPoint()
{
	return pMotor->setPoint;
}


/*
Function: stepperSpeed
Params: none
Returns: returns STEPPER_INFO_struct.speed
Description: see returns.
*/
uint16_t stepperSpeed()
{
	return pMotor->speedInRPM;
}


/*
Function: stepperSaveData
Params: none
Returns: none
Description: Writes stepper info struct (motor) to EEPROM. Copies motor to 
			motorPrev through pointers.
*/
void stepperSaveData()
{
	eepromWriteBlock(pMotor, STEPPER_INFO_EEPROM_ADDR, sizeof(*pMotor));
	*pMotorPrev = *pMotor;
}


/*
Function: stepperLoadData
Params: none
Returns: none
Description: Reads stepper info struct from EEPROM to motor. This is called on start up.
*/
void stepperLoadData()
{
	eepromReadBlock(STEPPER_INFO_EEPROM_ADDR, pMotor, sizeof(*pMotor));
}


/*
Function: stepperCheckForChange
Params: none 
Returns: none
Description: Checks the current and previous versions of stepper info struct
			and returns non zero if any changes were made.
*/
int8_t stepperCheckForChange()
{
	uint8_t *previous = (uint8_t*)pMotorPrev;			//Pointer to previous data cast to uint8_t*.
	uint8_t *current = (uint8_t*)pMotor;				//Pointer to current data cast to uint8_t.
	uint8_t size = sizeof(*pMotor);						//Size of stepper info struct.
	for (uint8_t i = 0; i < size;)
	{
		if (*(previous++) == *(current++))
		{
			i++;
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

/*
Function: stepperConfigTimeoutTimer
Params: none
Returns: none
Description: Just configures the TCB thats being used for the timeout counter.
*/
void stepperConfigTimeoutTimer(uint16_t TCB_PER)
{
	systemConfigTCB(stepperTimeoutTimer, TCB_PER);
}

/*
Function: stepperStartTimeoutTimer
Params: none
Returns: none
Description: The timeout timer starts counting.
*/
void stepperStartTimeoutTimer()
{
	systemEnableTCB(stepperTimeoutTimer);
}

/*
Function:stepperClearTimoutTimer
Params: none
Returns: none
Description: Clears the count register for the actual timer and resets
			the timeoutCounter struct member to zero.
*/
void stepperClearTimeoutAmount()
{
	pMotor->timeoutAmount = 0;
}

/*
Function: stepperStopTimeoutTimer
Params: none
Returns: none
Description: Disables the timer. Clear timer CNT and timeoutAmount.
*/
void stepperStopTimeoutTimer()
{
	systemDisableTCB(stepperTimeoutTimer);
	stepperClearTimeoutAmount();
	stepperClearTimeoutCnt();
}

/*
Function: stepperIncrementTimeoutCount
Params: none
Returns: none
Description: Increments the member of stepper_info "timeoutCounter".
*/
void stepperIncrementTimeoutAmount()
{
	pMotor->timeoutAmount++;
}

/*
Function: stepperSetFlag
Params: flag_bm (defined in stepper.h)
Returns: none
Description: Applies a bitmask to "flags" in stepper_info.
*/
void stepperSetFlag(uint8_t flag_bm)
{
	pMotor->flags |= flag_bm;
}

/*
Function: stepperClearFlag
Params: flag_bm (defined in stepper.h)
Returns:
Description: Clears a bit from "flags". Member in stepper_info.
*/
void stepperClearFlag(uint8_t flag_bm)
{
	pMotor->flags &= ~(flag_bm);
}

/*
Function: stepperFlags
Params: none
Returns: STEPPER_INFO_t.flags
Description: Returns the variable flags from stepper_info. Can be compared against the 
			bitmasks at the top of stepper.h
*/
uint8_t stepperFlags()
{
	return pMotor->flags;
}

/*
Function: stepperGoToZero
Params: none
Returns: none
Description: Starts turning the stepper towards position 0 until it times out.
			This means the motor can't turn any further and is at position 0.
			Then resets the position and set point to 0.
*/
void stepperGoToZero()
{
	stepperSetDirection(DIRECTION_CCW);								//Set direction to count down towards soft
	stepperStartMove();												//Start the stepper PWM signal
	STEPPER_ENABLE;													//Enable the driver.
	
	stepperConfigTimeoutTimer(TCB_FREQ_15HZ);						//Set the period of the timeout timer.
	stepperClearTimeoutAmount();									//Clear the timer count if there is one.
	stepperStartTimeoutTimer();										//Start counter.
	
	/*Wait for the timeout flag to be set or for the previous position to be
	greater than the current position. If the motor stops where the encoder
	is on an edge it will keep firing the encoder interrupt and resetting the
	timeoutCounter.*/
	while (pMotor->timeoutAmount < 15) {}							
	
	stepperStopTimeoutTimer();										//Stop the timer.
	stepperStopMove();												//Stop the motor.
	
	stepperSetSetPoint(0);											//Change the current set point.
	stepperSetPosition(0);											//Change the current position.
	stepperClearFlag(FLAG_GO_TO_ZERO_bm);							//Clear the flag.
}

/*
Function: stepperCurrentLimit
Params: none
Returns: returns STEPPER_INFO_struct.currentLimit
Description: see returns
*/
CURRENT_LIMIT_t stepperCurrentLimit()
{
	return pMotor->currentLimit;
}

/*
Function: stepperStepMode
Params: none
Returns: STEPPER_INFO_struct.stepMode
Description: see returns
*/
STEP_MODE_t stepperStepMode()
{
	return pMotor->stepMode;
}

/*
Function: stepperTimeoutAmount
Params: none
Returns: the amount the stepper timeout timer has timed out.
Description: see returns
*/
uint8_t stepperTimeoutAmount()
{
	return pMotor->timeoutAmount;
}

/*
Function: stepperPreviousPosition
Params: none
Returns: the previous position of the stepper motor...
Description: ... used to determine if it is spinning the correct direction.
*/
int16_t stepperPreviousPosition()
{
	return pMotor->previousPosition;
}

/*
Function: stepperTimeoutCnt
Params: none
Returns: the CNT register of the stepper timeout timer.
Description: see returns.
*/
uint16_t stepperTimeoutCnt()
{
	return stepperTimeoutTimer->CNT;
}

/*
Function: stepperClearTimeoutCnt
Params: none
Returns: none
Description: sets the CNT register of the stepper timeout timer to zero.
*/
void stepperClearTimeoutCnt()
{
	stepperTimeoutTimer->CNT = 0;
}

/*
Function: stpperIsMoving
Params: none
Returns: true if the stepper step timer is running signifying that the motor is turning.
Description: see returns.
*/
bool stepperIsMoving()
{
	return TCB2.STATUS;
}
