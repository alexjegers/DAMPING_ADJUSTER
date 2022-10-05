/*
 * iic.h
 *
 * Created: 9/17/2022 11:54:48 AM
 *  Author: AJ992
 */ 


#ifndef IIC_H_
#define IIC_H_

#define DEVICE_ADDR				(0x05 << 1)

#define IIC_REQUESTING_READ		TWI_DIR_bm
#define IIC_REQUESTING_WRITE	0

#define ADDRESS_FLAG			0
#define STOP_FLAG				1
#define NONE_FLAG				2

typedef enum INTERNAL_ADDRESS_enum
{
	BASE_ADDRESS,
	MOTOR_SET_POINT,					//RW 16-bit
	MOTOR_POSITION,						//RW 16-bit					
	MOTOR_SPEED,						//RW 16 bit
	MOTOR_CURRENT,						//RW 16 bit
	MOTOR_STEP_MODE,					//RW 8 bit
	MOTOR_STATUS_FLAGS,					//RW 8 bit
	BATTERY_VOLTAGE,					//R 16 bit
	MOTOR_GO_TO_ZERO					//W 8 bit
}INTERNAL_ADDRESS_t;


union iicBuffer
{
	uint64_t data;
	struct dataBytes 
	{
		uint8_t llsb;					//"least least significant bit"
		uint8_t lsb;
		uint8_t msb;
		uint8_t mmsb;					//"most most significant bit"
	}bytes;
		
};

void iicInit();													//Puts TWI hardware in slave mode and enables interrupts.
void iicKill();													//Disables slave mode.

/*TWI int flags*/
int8_t iicAddrOrStopFlag();										//Returns true if the address match interrupt flag is set.
int8_t iicReadOrWriteFlag();									//Returns non-zero if master is requesting read, 0 if master is writing.
int8_t iicDataIntFlag();										//Returns non-zero if flag is set, returns zero if not set.
void iicWaitForDataIntFlag();									//Sits in a while loop until the data interrupt flag is sent.

/*Data transmission functions*/
void iicWrite(void *data, uint8_t size);						//Max size for one write is 4 bytes.
uint64_t iicRead(uint8_t size);									//Max size for one read is 4 bytes.
/*These are only used locally with iicWrite & iicRead*/
void iicSendResponse(TWI_SCMD_t response, TWI_ACKACT_t ack);	//Choose to respond or complete transmission with an ACK or NACK.
char iicReadSdata();											//Returns the data in the SDATA register.
void iicWriteSdata(uint8_t data);								//Writes to SDATA register.

/*Internal address management*/
void iicSetInternalAddress(INTERNAL_ADDRESS_t setInternalAddr);	//Set the current internal address.
INTERNAL_ADDRESS_t iicReadInternalAddress();					//Returns the current internal address.
void iicHandleData(INTERNAL_ADDRESS_t internalAddress);			//Calls an IIC response handler depending on the current internal address.

/*Handlers for IIC response*/
void handlerMotorSetPoint();
void handlerMotorPosition();
void handlerMotorSpeed();
void handlerMotorCurrent();
void handlerMotorStepMode();
void handlerMotorStatusFlags();
void handlerBatteryVoltage();

#endif /* IIC_H_ */