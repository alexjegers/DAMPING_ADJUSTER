/*
 * iic.h
 *
 * Created: 11/11/2022 7:24:49 PM
 *  Author: AJ992
 */ 


#ifndef IIC_H_
#define IIC_H_

/*TWI peripheral IDs.*/
#define TWIM_TX_PID				16
#define TWIM_RX_PID				4

/*Macros for TWI peripheral.*/
#define TWI_CR_EN_bm			1
#define TWI_CR_DIS_bm			1 << 1	
#define TWI_CR_SWRST			1 << 7
#define TWI_CMDR_SADR_bp		1
#define TWI_CMDR_NBYTES_bp		16
#define TWI_CMDR_START_bm		1 << 13
#define TWI_CMDR_STOP_bm		1 << 14
#define TWI_CMDR_STOP_bp		14
#define TWI_CMDR_READ_bm		1
#define TWI_CMDR_READ_bp		0
#define TWI_CMDR_VALID_bm		1 << 15
#define TWI_SR_TXRDY_bm			1 << 1
#define TWI_SR_IDLE_bm			1 << 4
#define TWI_SR_ANAK_bm			1 << 8
#define TWI_SR_DNAK_bm			1 << 9

/*Macros for DMA controller.*/
#define DMA_CR_EN_bm			1
#define DMA_CR_DIS_bm			1 << 1
#define DMA_ISR_TRC_bm			1 << 1

#define TWIM				(*(avr32_twim_t*)0xFFFF4000)
#define DMA_TX				(*(avr32_pdca_channel_t*)0xFFFF0000)
#define DMA_RX				(*(avr32_pdca_channel_t*)0xFFFF0040)

/*Macros for starting a new transmission.*/
#define IIC_NEW_TRNS_READ		1
#define IIC_NEW_TRNS_WRITE		0

/*IIC/TWI functions in iic namespace.*/
namespace iic
{
typedef enum IIC_STATUS_t
{
	IIC_OK = 			0,
	IIC_ERROR,
	IIC_DNAK,
	IIC_ANAK=			3
}IIC_STATUS_t;

typedef enum IIC_INTERRUPT_t
{
	IIC_RXRDY_bm =		1,
	IIC_TXRDY_bm =		1 << 1,
	IIC_CRDY_bm =		1 << 2,
	IIC_CCOMP_bm =		1 << 3,
	IIC_IDLE_bm =		1 << 4,
	IIC_BUSFREE_bm =	1 << 5, 
	IIC_ANAK_bm =		1 << 8,
	IIC_DNAK_bm =		1 << 9,
	IIC_ARBLST_bm =		1 << 10,
	IIC_TOUT_bm =		1 << 12,
	IIC_PECERR_bm =		1 << 13,
	IIC_STOP_bm =		1 << 14,
	IIC_MENB_bm =		1 << 16
}IIC_INTERRUPT_t;

	/*
	DataPacket is a linked list of data to send over IIC. A new DataPacket is created 
	in iic::fastTransmission. A DataPacket is sent when the TWI BUSFREE interrupt is 
	generated.
	*/
	class DataPacket
	{
		public:
		uint8_t read;					//1 for read, 0 for write.	
		uint8_t internalAddr;			//Internal address of slave to begin reading or writing from.
		uint8_t saddr;					//Slave device address.
		uint8_t size;					//Size of data.
		void* data;						//Pointer to data.
		DataPacket* nextPacket;			//Pointer to next data packet.
	};

	void intHanlder();															//Handles  interrupt
	void setup();																//Must be called on device startup.
	void enableMaster();														//Enables the TWI master interface.
	void disableMaster();														//Disables the TWI master interface.
	void softwareReset();														//Resets the TWI peripheral.
	void enableInterrupt(IIC_INTERRUPT_t interrupt);							//Enables a TWI interrupt.
	void disableInterrupt(IIC_INTERRUPT_t interrupt);							//Disables a TWI interrupt.
	uint32_t interruptMask();													//Returns the TWI interrupt mask register.
	uint32_t readStatus();														//Returns the TWI status register.
	void clearStatus(uint32_t status);											//Clears a specified bit in the status register.
	IIC_STATUS_t setClkSpeed(uint32_t pbaClkSpeed, uint32_t ClkSpeed);			//Sets the TWI clock speed.
	void fastTransmission(uint8_t read, uint8_t saddr, uint8_t internalAddr,	//Adds a new  data transmission to the buffer.
									uint8_t size, void* data)
	IIC_STATUS_t slowTransmission(uint8_t read, uint8_t saddr					//Polling driven, waits for response from slave.					
									uint8_t size, void* data);
	IIC_STATUS_t probe(uint8_t saddr);											//Check the status of a slave device given the address.
}

/*DMA functions, need namespace here still.*/	
typedef enum DMA_INTERRUPT_t
{
	DMA_RCZ_bm =		1,
	DMA_TRC_bm =		1 << 1,
	DMA_TERR_bm =		1 << 2
}DMA_INTERRUPT_t;

void dmaEnable(avr32_pdca_channel_t* dma);										//Enables a specified DMA channel.
void dmaDisable(avr32_pdca_channel_t* dma);										//Disables a specified DMA channel.
void dmaIntHandler();															//Handles DMA interrupts.
uint32_t dmaIntStatus(avr32_pdca_channel_t* dma);								//Returns the DMA status register.
void dmaEnableInterrupt(avr32_pdca_channel_t* dma, DMA_INTERRUPT_t interrupt);	//Enables a DMA interrupt.
void dmaDisableInterrupt(avr32_pdca_channel_t* dma, DMA_INTERRUPT_t interrupt);	//Disables a DMA interrupt.


#endif /* IIC_H_ */