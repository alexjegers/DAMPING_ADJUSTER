/*
 * i2c.cpp
 *
 * Created: 11/12/2022 6:23:08 PM
 *  Author: Alex Jegers
 */ 
/*
Function:
Params:
Returns:
Description:
*/

#include <avr32/io.h>
#include <stdint.h>
#include "iic.h"
#include <intc.h>
#include "system.h"
#include "io.h"

using namespace iic;
DataPacket iicDataPacketHead;

__attribute__((__interrupt__))void iic::intHanlder()
{
	if ((readStatus() & TWI_SR_ANAK_bm) != 0)
	{
		softwareReset();
		clearStatus(TWI_SR_ANAK_bm);
	}
	
	if ((readStatus() & TWI_SR_DNAK_bm) != 0)
	{
		TWIM.cmdr &= ~TWI_CMDR_VALID_bm;
		clearStatus(TWI_SR_DNAK_bm);
	}
	dmaDisable(&DMA_TX);
	dmaDisable(&DMA_RX);
	
	/*If there is an item in the list after the head, begin transmitting it.*/
	if (iicDataPacketHead.nextPacket != NULL)
	{
		/*Make sure the TWI bus is free.*/
		if ((readStatus() & IIC_BUSFREE_bm) != 0)
		{
			/*Create new data packet to get info from the list item.*/
			DataPacket* dataPacket = iicDataPacketHead.nextPacket;
			
			/*Set up DMA transfer size and data pointer.*/
			/*First for the internal address.*/
			DMA_TX.tcr = 1;
			DMA_TX.mar = (unsigned long)&dataPacket->internalAddr;
			
			if (dataPacket->read == TWI_CMDR_READ_bm)
			{
				/*Then for the data to write starting at that internal address.*/
				DMA_RX.tcr = dataPacket->size;
				DMA_RX.mar = (unsigned long)dataPacket->data;
			}
			else
			{
				/*Then for the data to write starting at that internal address.*/
				DMA_TX.tcrr = dataPacket->size;
				DMA_TX.marr = (unsigned long)dataPacket->data;				
			}
			
			/*Set up TWI command register with the slave device address, transfer size, and data direction.*/
			TWIM.cmdr = (1 << TWI_CMDR_NBYTES_bp)
			| TWI_CMDR_START_bm
			| TWI_CMDR_VALID_bm
			| (dataPacket->saddr << TWI_CMDR_SADR_bp)
			| (0 << TWI_CMDR_READ_bp);		
				
			if (dataPacket->read == TWI_CMDR_READ_bm)
			{
				TWIM.ncmdr = (dataPacket->size << TWI_CMDR_NBYTES_bp)
				|TWI_CMDR_START_bm
				| TWI_CMDR_STOP_bm
				| TWI_CMDR_VALID_bm
				| (dataPacket->saddr << TWI_CMDR_SADR_bp)
				| (dataPacket->read << TWI_CMDR_READ_bp);			
			}
			else
			{
				TWIM.ncmdr = (dataPacket->size << TWI_CMDR_NBYTES_bp)
				| TWI_CMDR_STOP_bm
				| TWI_CMDR_VALID_bm
				| (dataPacket->saddr << TWI_CMDR_SADR_bp)
				| (dataPacket->read << TWI_CMDR_READ_bp);				
			}	
					

			
			dmaEnable(&DMA_TX);
			dmaEnable(&DMA_RX);

			/*If there's no packets after this one, point the head to NULL.*/
			/*Can't disable the TWI master here or else it stops the data transmission.*/
			if (dataPacket->nextPacket == NULL)
			{
				free(dataPacket);
				iicDataPacketHead.nextPacket = NULL;

			}
			else
			{
				iicDataPacketHead.nextPacket = dataPacket->nextPacket;
				free(dataPacket);
			}
		}
	}
	/* If iicDataPacketHead.nextPacket == NULL, there's no more data to transmit currently so disable IIC */
	else
	{
		disableMaster();
		disableInterrupt(IIC_BUSFREE_bm);
	}
}



/*
Function: iic::EnableMaster
Params: none
Returns: none
Description: Enables the TWI peripheral. There's only 1 so a passing a pointer
			to it is not necessary.
*/
void iic::enableMaster()
{
	TWIM.cr = TWI_CR_EN_bm;
}

/*
Function: iic::DisableMaster
Params: none
Returns: none
Description: Disables the TWI peripheral. There's only 1 so a passing a pointer
			to it is not necessary.
*/
void iic::disableMaster()
{
	TWIM.cr = TWI_CR_DIS_bm;	
}

/*
Function: iic::SoftwareReset
Params: none
Returns: none
Description: Performs a software reset of the TWI peripheral.
*/
void iic::softwareReset()
{
	TWIM.cr = TWI_CR_SWRST;	
}

/*
Function: iic::Setup
Params: none
Returns: none
Description: Assigns the TWI peripheral to PDCA channels and enables 
			TWI and PDCA interrupts.
*/
void iic::setup()
{
	/* Assign TWI to DMA */
	DMA_TX.psr = TWIM_TX_PID;
	DMA_RX.psr = TWIM_RX_PID;

	/* Enable IIC interrupts */
	INTC_register_interrupt(&intHanlder, AVR32_TWIM_IRQ, 3);	
}

/*
Function: iic::SetClkSpeed
Params: pbaClkSpeed: speed of the PBA clock.
		iic::ClkSpeed: desired speed of the TWI clock.
Returns: IIC_ERROR: desired clock speed is out of bounds.
		IIC_OK: desired clock speed is in bounds and has been set.
Description: Sets the speed of the TWI clock.
*/
IIC_STATUS_t iic::setClkSpeed(uint32_t pbaClkSpeed, uint32_t iicClkSpeed)
{
	uint32_t f_prescaled;
	uint8_t cwgr_exp = 0;
	f_prescaled = (pbaClkSpeed / iicClkSpeed / 2);
	// f_prescaled must fit in 8 bits, cwgr_exp must fit in 3 bits
	while ((f_prescaled > 0xFF) && (cwgr_exp <= 0x7)) {
		// increase clock divider
		cwgr_exp++;
		// divide f_prescaled value
		f_prescaled /= 2;
	}
	if (cwgr_exp > 0x7) {
		return IIC_ERROR;
	}
	// set clock waveform generator register
	TWIM.cwgr = ((f_prescaled/2) << AVR32_TWIM_CWGR_LOW_OFFSET)
			| ((f_prescaled - f_prescaled/2) << AVR32_TWIM_CWGR_HIGH_OFFSET)
			| (cwgr_exp << AVR32_TWIM_CWGR_EXP_OFFSET)
			| (0     << AVR32_TWIM_CWGR_DATA_OFFSET)
			| (f_prescaled << AVR32_TWIM_CWGR_STASTO_OFFSET);
	return IIC_OK;
}

/*
Function: iic::EnableInterrupt
Params: IIC_INTERRUPT_t interrupt: specified TWI interrupt.
Returns: none
Description: enables TWI interrupt(s).
*/
void iic::enableInterrupt(IIC_INTERRUPT_t interrupt)
{
	TWIM.ier = interrupt;
}

/*
Function: iic::disableInterrupt
Params: IIC_INTERRUPT_t interrupt: specified TWI interrupt.
Returns: none
Description: disables TWI interrupt(s).
*/
void iic::disableInterrupt(IIC_INTERRUPT_t interrupt)
{
	TWIM.idr = interrupt;
}


/*
Function: iic::InterruptMask
Params: none
Returns: The TWI interrupt mask register stating which interrupts are currently enabled.
Description: enables TWI interrupt(s).
*/
uint32_t iic::interruptMask()
{
	return TWIM.imr; 
}

/*
Function: iic::Status
Params: none.
Returns: The TWI status register.
Description: Returns the TWI status register.
*/
uint32_t iic::readStatus()
{
	return TWIM.sr;
}

/*
Function: iic::clearStatus
Params: status: which status bit/interrupt to clear.
Returns: none
Description: clears an interrupt from the status register.
*/
void iic::clearStatus(uint32_t status)
{
	TWIM.scr = status;
}

/*
Function: iic::fastTransmission
Params: uint8_t saddr: slave device address.
		uint8_t size: size of data to send.
		void* data: pointer to data to send.
Returns: none
Description: Adds a new data packet to the IIC buffer.
*/
void iic::fastTransmission(uint8_t read, uint8_t saddr, uint8_t internalAddr, uint8_t size, void* data)
{
	/*Disable global interrupts.*/
	systemDisableInterrupts();
	
	/*Create a new data packet and assign the data to it.*/
	DataPacket* iicNewData = new DataPacket;
	iicNewData->saddr = saddr;
	iicNewData->size = size;
	iicNewData->data = data;
	iicNewData->read = read;	
	iicNewData->internalAddr = internalAddr;

	/* If there's no on going transmission, put the data in the first data packet after the head. */
	if (iicDataPacketHead.nextPacket == NULL)
	{
		iicDataPacketHead.nextPacket = iicNewData;	
		iicNewData->nextPacket = NULL;
		enableInterrupt(IIC_BUSFREE_bm);
		enableMaster();		
	}
	/* If there is an on going transmission, put the new packet at the end of the list. */
	else
	{
		DataPacket* end = &iicDataPacketHead;	
		while (end->nextPacket != NULL)
		{
			end = end->nextPacket;
		}
		end->nextPacket = iicNewData;
		iicNewData->nextPacket = NULL;	
	}
}

/*
Function: probe
Params: saddr: slave device address.
Returns: IIC_STATUS_t
		IIC_OK: the slave device ACK'd the address.
		IIC_ANAK: the slave device NACK'd the address.
Description: test's a slave device address to see if the device is responsive.
*/
IIC_STATUS_t iic::probe(uint8_t saddr)
{
	iic::enableMaster();
	TWIM.thr = 0;
	TWIM.cmdr = (0 << TWI_CMDR_NBYTES_bp)
	| TWI_CMDR_START_bm
	|TWI_CMDR_STOP_bm
	| TWI_CMDR_VALID_bm
	| (saddr << TWI_CMDR_SADR_bp);	
	
	uint32_t status = 0;
	
	while (status & TWI_SR_TXRDY_bm == 0) 
	{
		status = readStatus();		
	}
	
	status = readStatus();
	
	if (status & IIC_ANAK_bm != 0)
	{
		return IIC_ANAK;
	}
	else
	{
		return IIC_OK;
	}
	iic::disableMaster();
}


/*
Function: dmaIntStatus
Params: dma: pointer to PDCA channel.
Returns: The specified PDCA channel's interrupt status register.
Description: see returns.
*/
uint32_t dmaIntStatus(avr32_pdca_channel_t* dma)
{
	return dma->isr;
}

/*
Function: dmaEnableInterrupt
Params: dma: pointer to PDCA channel.
		interrupt: specified interrupt to enable from DMA_INTERRUPT_t.
Returns: none
Description: Enables interrupt(s) for specified PDCA channel.
*/
void dmaEnableInterrupt(avr32_pdca_channel_t* dma, DMA_INTERRUPT_t interrupt)
{
	dma->ier = interrupt;
}

/*
Function: dmaDisableInterrupt
Params: dma: pointer to PDCA channel.
		interrupt: specified interrupt to enable from DMA_INTERRUPT_t.
Returns: none
Description: Disables interrupt(s) for specified PDCA channel.
*/
void dmaDisableInterrupt(avr32_pdca_channel_t* dma, DMA_INTERRUPT_t interrupt)
{
	dma->idr = interrupt;
}

/*
Function: dmaEnable
Params: dma: pointer to PDCA channel.
Returns: none
Description: Enables specified PDCA channel, PDCA sends data once enabled.
*/
void dmaEnable(avr32_pdca_channel_t* dma)
{
	dma->cr = DMA_CR_EN_bm;
}

/*
Function: dmaDisable
Params: dma: pointer to PDCA channel.
Returns: none
Description: Disables specified PDCA channel, must be disabled before reloading
			with new data pointer.
*/
void dmaDisable(avr32_pdca_channel_t* dma)
{
	dma->cr = DMA_CR_DIS_bm;	
}
