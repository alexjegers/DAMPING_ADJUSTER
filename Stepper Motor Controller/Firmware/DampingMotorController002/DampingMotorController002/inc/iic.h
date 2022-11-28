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

typedef struct IIC_DATA_t  
{
	int16_t position;
	int16_t setPoint;
	uint8_t stepMode;
	uint16_t currentLimit;
	uint8_t decayMode;
	uint16_t speedInRPM;
	uint8_t flags;	
	uint8_t testLights;
}IIC_DATA_struct;




void iicInit();													//Puts TWI hardware in slave mode and enables interrupts.
void iicKill();													//Disables slave mode.
void iicEnableInterrupts();										//Enable the IIC interrupts.
void iicDisableInterrupts();									//Disables the IIC interrupts.

/*TWI int flags*/
int8_t iicAddrOrStopFlag();										//Returns true if the address match interrupt flag is set.
int8_t iicReadOrWriteFlag();									//Returns non-zero if master is requesting read, 0 if master is writing.
int8_t iicDataIntFlag();										//Returns non-zero if flag is set, returns zero if not set.
void iicWaitForDataIntFlag();									//Sits in a while loop until the data interrupt flag is sent.


void iicSendResponse(TWI_SCMD_t response, TWI_ACKACT_t ack);	//Choose to respond or complete transmission with an ACK or NACK.
char iicReadSdata();											//Returns the data in the SDATA register.
void iicWriteSdata(uint8_t data);								//Writes to SDATA register.

void iicLoadFromStepper();
void iicLoadToStepper();

#endif /* IIC_H_ */