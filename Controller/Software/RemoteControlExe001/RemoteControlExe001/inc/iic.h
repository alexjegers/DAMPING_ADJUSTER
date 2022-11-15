/*
 * iic.h
 *
 * Created: 11/11/2022 7:24:49 PM
 *  Author: AJ992
 */ 


#ifndef IIC_H_
#define IIC_H_

#define TWIM_TX_PID				16
#define TWIM_RX_PID				4

#define TWI_CR_EN_bm			1
#define TWI_CR_DIS_bm			1 << 1	
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

#define DMA_CR_EN_bm			1
#define DMA_CR_DIS_bm			1 << 1
#define DMA_ISR_TRC_bm			1 << 1

#define TWIM				(*(avr32_twim_t*)0xFFFF4000)
#define DMA_TX				(*(avr32_pdca_channel_t*)0xFFFF0000)
#define DMA_RX				(*(avr32_pdca_channel_t*)0xFFFF0040)

#define IIC_NEW_TRNS_READ		1
#define IIC_NEW_TRNS_WRITE		0

typedef enum IIC_STATUS_t
{
	IIC_OK = 			0,
	IIC_ERROR
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
};

typedef enum DMA_INTERRUPT_t
{
	DMA_RCZ_bm =		1,
	DMA_TRC_bm =		1 << 1,
	DMA_TERR_bm =		1 << 2
};

class iicDataPacket
{
	public:
	uint8_t read;					//1 for read, 0 for write.				
	uint8_t saddr;					//Slave device address.
	uint8_t size;					//Size of data.
	void* data;						//Pointer to data.
	iicDataPacket* nextPacket;		//Pointer to next data packet.
};

//iicDataPacket iicDataPacketHead;

void iicIntHanlder();
void dmaIntHandler();
uint32_t dmaIntStatus(avr32_pdca_channel_t* dma);
void dmaEnableInterrupt(avr32_pdca_channel_t* dma, DMA_INTERRUPT_t interrupt);
void dmaDisableInterrupt(avr32_pdca_channel_t* dma, DMA_INTERRUPT_t interrupt);
void iicSetup();
void iicEnableMaster();
void iicDisableMaster();
void iicEnableInterrupt(IIC_INTERRUPT_t interrupt);
void iicDisableInterrupt(IIC_INTERRUPT_t interrupt);
void dmaEnable(avr32_pdca_channel_t* dma);
void dmaDisable(avr32_pdca_channel_t* dma);
uint32_t iicInterruptMask();
uint32_t iicStatus();
IIC_STATUS_t iicSetClkSpeed(uint32_t pbaClkSpeed, uint32_t iicClkSpeed);
IIC_STATUS_t iicNewTransmission(uint8_t read, uint8_t saddr, uint8_t size, void* data);




#endif /* IIC_H_ */