/*
 * stepper.h
 *
 * Created: 9/11/2022 10:04:26 PM
 *  Author: AJ992
 */ 

#ifndef STEPPER_H_
#define STEPPER_H_

#include <stdbool.h>

/*Address in EEPROM where the stepper info struct is stored*/
#define STEPPER_INFO_EEPROM_ADDR	(uint8_t*)0x140A

/*DRV8825 operating modes*/
#define STEPPER_ENABLE				PORTC.OUT &= ~(nENBL_PIN)
#define STEPPER_DISABLE				PORTC.OUT |= nENBL_PIN
#define STEPPER_SLEEP				PORTD.OUT &= ~(nSLEEP_PIN)
#define STEPPER_WAKE				PORTD.OUT |= nSLEEP_PIN

/*Used for setting rotation direction*/
#define DIRECTION_CCW				1
#define DIRECTION_CW				-1

/*Status flag bitmasks*/
#define FLAG_FAULT_bm				1					//Set to 1 if the fault pin goes high (over temp, over current).
#define FLAG_TIMEOUT_bm				(1 << 1)			//Set to 1 if it took too long to reach the setpoint.
#define FLAG_ERROR_bm				(1 << 2)			//Something bad happened somewhere.	
#define FLAG_GO_TO_ZERO_bm			(1 << 3)			//Flag is set to instruct the program to run the stepperGoToZero function.

/*Step mode constants for DRV8825*/
typedef enum STEPPER_STEP_MODES_enum
{
	MODE_FULL_STEP_bm =				0x0,
	MODE_HALF_STEP_bm = 			0x1,
	MODE_QUARTER_STEP_bm =			0x2,
	MODE_EIGTH_STEP_bm	=			0x3,
	MODE_SIXTEENTH_STEP_bm	=		0x4,
	MODE_THIRTY_SECOND_STEP_bm =	0x5,
	MODE_CLEAR_REGISTER_bm	=		~0x07,
}STEP_MODE_t;


/*Pulses needed for a full rotation depending on step mode*/
typedef const enum STEPS_PER_ROTATION_enum
{
	STEPS_FULL_STEP =				200,
	STEPS_HALF_STEP = 				400,
	STEPS_QUARTER_STEP =			800,
	STEPS_EIGTH_STEP =				1600,
	STEPS_SIXTEENTH_STEP =			3200,
	STEPS_THIRTY_SECOND_STEP =		6400
}STEPS_PER_ROTATION_t;


/*Bitmasks for DAC data register when setting current limit*/
typedef enum CURRENT_LIMIT_enum
{
	TENTH_AMP_bm =					21,
	QUART_AMP_bm =					50,
	HALF_AMP_bm =					102,
	ONE_AMP_bm =					203,
	ONE_AND_QUARTER_AMP_bm =		255,
	ONE_AND_HALF_AMP_bm =			307,
	ONE_AND_THREE_QUART_AMP_bm =	357,
	TWO_AMP_bm =					410
}CURRENT_LIMIT_t;


/*Decay modes*/
typedef enum DECAY_MODES_enum
{
	DECAY_MIXED,
	DECAY_FAST,
	DECAY_SLOW
}DECAY_MODE_t;


/*Main struct holding all the live information on the stepper motor*/
typedef struct STEPPER_INFO_struct
{
	int16_t position;
	int16_t previousPosition;						//Had to add this in for stepperGoToZero func.
	int16_t setPoint;
	STEP_MODE_t stepMode;
	CURRENT_LIMIT_t currentLimit;
	DECAY_MODE_t decayMode;
	uint16_t speedInRPM;
	uint8_t timeoutAmount;
	
	uint8_t flags;									//See #define's for description and bitmasks.
		
	/*A write to current.dacData must be shifted left 6 bits.*/	
	union CURRENT_LIMIT
	{
		uint16_t dacData;
		struct DAC_VALUE 
		{
			uint8_t lsb;
			uint8_t msb;
		}dacStruct;
	}current;
		
}STEPPER_INFO_t;


void stepperInit();									//Loads default stepper settings.

/*Motor movement control*/
void stepperStartMove();							//Starts moving the stepper motor.
void stepperStopMove();								//Stops the stepper motor.
void stepperSetDirection(uint8_t direction);		//Sets which way the motor will rotate.
void stepperGoToZero();								//Moves the motor position to zero.
bool stepperIsMoving();								//Returns the status of the STEP PWM timer to determine if the motor is currently moving.


/*Stepper timeout timer control*/
void stepperConfigTimeoutTimer(uint16_t TCB_PER);	//Called in stepperInit.
void stepperStartTimeoutTimer();					//Timer starts counting.
void stepperIncrementTimeoutAmount();				//Increment the amount of times the timer has overflowed.
uint8_t stepperTimeoutAmount();						//Returns timeoutAmount, the amount of times it has timed-out, not the TCB CNT register.
void stepperClearTimeoutAmount();					//Reset the count of timeoutAmount, doesnt effect the actual TCB CNT register.
void stepperStopTimeoutTimer();						//Stops the timeout timer.
uint16_t stepperTimeoutCnt();						//Returns the TCB CNT register.
void stepperClearTimeoutCnt();						//Resets the TCB CNT register to zero.

/*Functions to write to the stepper info struct*/
void stepperSetSetPoint(int16_t setPoint);			//Sets the set point.
void stepperSetPosition(int16_t position);			//Overrides the position.
void stepperSetStepMode(STEP_MODE_t mode);			//Sets the stepping mode for the stepper.
void stepperSetSpeed(uint16_t speedInRPM);			//Writes to speedInRPM in STEPPER_INFO struct.
void stepperIncrementPosition(int8_t direction);	//Increments stepper position by +-1 when the encoder interrupt fires.
void stepperSetDecay(DECAY_MODE_t decayMode);		//Sets the decay mode.
void stepperSetCurrent(CURRENT_LIMIT_t current);	//Sets the current limit.
void stepperSetFlag(uint8_t flag_bm);				//Sets a flag in the flags struct.
void stepperClearFlag(uint8_t flag_bm);				//Clears a flag in the flags struct.

/*Functions to read from the stepper info struct*/
uint8_t stepperAtSetPoint();						//Returns 0 if the position does not match the set point.
int16_t stepperPosition();							//Returns the current position of the stepper motor.
int16_t stepperPreviousPosition();					//Returns the previous position of the stepper.
int16_t stepperSetPoint();							//Returns the current set point of the stepper motor.
uint16_t stepperSpeed();							//Returns speed.
uint8_t stepperFlags();								//Returns the status flags.
CURRENT_LIMIT_t stepperCurrentLimit();				//Returns the current limit.
STEP_MODE_t stepperStepMode();						//Returns the step mode.
DECAY_MODE_t stepperDecayMode();					//Returns the decay mode.

/*Read and write the stepper stuct to EEPROM*/
void stepperSaveData();
void stepperLoadData();
int8_t stepperCheckForChange();						//Returns non-zero if motorPrev != motor.

#endif /* STEPPER_H_ */