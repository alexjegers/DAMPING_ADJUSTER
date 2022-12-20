/*
 * stepper.h
 *
 * Created: 10/11/2022 12:02:02 PM
 *  Author: AJ992
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_

/*Status flag bitmasks*/
#define FLAG_FAULT_bm				1					//Set to 1 if the fault pin goes high (over temp, over current).
#define FLAG_TIMEOUT_bm				(1 << 1)			//Set to 1 if it took too long to reach the setpoint.
#define FLAG_ERROR_bm				(1 << 2)			//Something bad happened somewhere.
#define FLAG_GO_TO_ZERO_bm			(1 << 3)			//Flag is set to instruct the program to run the stepperGoToZero function.

#define EEPROM_DEVICE_ADDR			0b01010000
#define FRONT_LEFT_DEVICE_ADDR			0x05
#define FRONT_RIGHT_DEVICE_ADDR			0x06
#define REAR_LEFT_DEVICE_ADDR			0x07
#define REAR_RIGHT_DEVICE_ADDR			0x08

/*Internal address for stepper motor controller.*/
typedef enum STEPPER_INTERNAL_ADDR_enum
{
	MOTOR_POSITION =				0x00,
	MOTOR_SET_POINT =				0x02,
	MOTOR_STEP_MODE =				0x04,
	MOTOR_CURRENT_LIMIT =			0x05,
	MOTOR_DECAY_MODE =				0x07,
	MOTOR_SPEED =					0x08,
	MOTOR_FLAGS =					0x0A,
	BATTERY_VOLTAGE =				0x0B
}STEPPER_INTERNAL_ADDR_t;

/*Step mode constants for DRV8825*/
typedef enum STEPPER_STEP_MODES_enum
{
	MODE_FULL_STEP_bm =				0x0,
	MODE_HALF_STEP_bm = 			0x1,
	MODE_QUARTER_STEP_bm =			0x2,
	MODE_EIGTH_STEP_bm	=			0x3,
	MODE_SIXTEENTH_STEP_bm	=		0x4,
	MODE_THIRTY_SECOND_STEP_bm =	0x5,
	MODE_CLEAR_REGISTER_bm	=		~0x07
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
class STEPPER_MOTOR
{
	public:

	struct STEPPER_INFO_struct
	{
		union  
		{
			int16_t val;
			struct
			{
				int8_t msb;
				int8_t lsb;
			}bytes;
		}position;
							
		int16_t setPoint;
		STEP_MODE_t stepMode;
		CURRENT_LIMIT_t currentLimit;
		DECAY_MODE_t decayMode;
		uint16_t speedInRPM;
		uint8_t flags;									//See #define's for description and bitmasks.
		uint16_t voltage;
	}stepperInfo;
	
	struct  
	{
		int anak		:1;
		int dnak		:1;
		int moving		:1;
		int error		:1;
	}statusFlags;
	
	STEPPER_MOTOR();
	char positionChar[2];	
	void sendSetPoint();
	void sendData(STEPPER_INTERNAL_ADDR_t addr, void* data, uint8_t numBytes);
	void requestData(STEPPER_INTERNAL_ADDR_t addr, void* data, uint8_t numbytes);
	void setDeviceAddr(uint8_t addr);
	void saveSettings();								//Writes the STEPPER_INFO struct to EEPROM.
	void loadSettings();								//Loads the STEPPER_INFO struct from EEPROM.
	
	private:
	uint8_t deviceAddr;
};

extern STEPPER_MOTOR frontLeft;
extern STEPPER_MOTOR frontRight;
extern STEPPER_MOTOR rearLeft;
extern STEPPER_MOTOR rearRight;

#endif /* STEPPER_H_ */