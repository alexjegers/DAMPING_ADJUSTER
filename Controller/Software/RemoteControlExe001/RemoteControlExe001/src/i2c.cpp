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

iicDataPacket iicDataPacketHead;

__attribute__((__interrupt__))void iicIntHanlder()
{
	if ((iicReadStatus() & TWI_SR_ANAK_bm) != 0)
	{
		iicSoftwareReset();
		iicClearStatus(TWI_SR_ANAK_bm);
	}
	
	if ((iicReadStatus() & TWI_SR_DNAK_bm) != 0)
	{
		TWIM.cmdr &= ~TWI_CMDR_VALID_bm;
		iicClearStatus(TWI_SR_DNAK_bm);
	}
	dmaDisable(&DMA_TX);
	dmaDisable(&DMA_RX);
	
	/*If there is an item in the list after the head, begin transmitting it.*/
	if (iicDataPacketHead.nextPacket != NULL)
	{
		/*Make sure the TWI bus is free.*/
		if ((iicReadStatus() & IIC_BUSFREE_bm) != 0)
		{
			/*Create new data packet to get info from the list item.*/
			iicDataPacket* dataPacket = iicDataPacketHead.nextPacket;
		
			/*If read == 0, we are transmitting data.*/
			if (dataPacket->read == 0)
			{
				/*Set up DMA transfer size and data pointer.*/
				DMA_TX.tcr = dataPacket->size;								
				DMA_TX.mar = (unsigned long)dataPacket->data;
			}
			else
			{
				/*Set up DMA transfer size and data pointer.*/
				DMA_RX.tcr = dataPacket->size;					
				DMA_RX.mar = (unsigned long)dataPacket->data; 	
			}
			
			/*Set up TWI command register with the slave device address, transfer size, and data direction.*/		
			TWIM.cmdr = (dataPacket->size << TWI_CMDR_NBYTES_bp)
			| TWI_CMDR_START_bm
			| TWI_CMDR_STOP_bm
			| TWI_CMDR_VALID_bm
			| (dataPacket->saddr << TWI_CMDR_SADR_bp)
			| (dataPacket->read << TWI_CMDR_READ_bp);
			
			/* Transmission NACK'd by slave after sending address. */
			if ((iicReadStatus() & TWI_SR_ANAK_bm) != 0)
			{
			}
			
			/* Enabling the DMA starts the transmission. */
			if (dataPacket->read == 0)
			{
				dmaEnable(&DMA_TX);
			}
			else
			{
				dmaEnable(&DMA_RX);
			}
			
			/* Transmission NACK'd by slave after sending data. */
			if ((iicReadStatus() & TWI_SR_DNAK_bm) != 0)
			{
			}
			
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
		iicDisableMaster();				
		iicDisableInterrupt(IIC_BUSFREE_bm);
	}
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
Function: iicEnableMaster
Params: none
Returns: none
Description: Enables the TWI peripheral. There's only 1 so a passing a pointer
			to it is not necessary.
*/
void iicEnableMaster()
{
	TWIM.cr = TWI_CR_EN_bm;
}

/*
Function: iicDisableMaster
Params: none
Returns: none
Description: Disables the TWI peripheral. There's only 1 so a passing a pointer
			to it is not necessary.
*/
void iicDisableMaster()
{
	TWIM.cr = TWI_CR_DIS_bm;	
}

/*
Function: iicSoftwareReset
Params: none
Returns: none
Description: Performs a software reset of the TWI peripheral.
*/
void iicSoftwareReset()
{
	TWIM.cr = TWI_CR_SWRST;	
}

/*
Function: iicSetup
Params: none
Returns: none
Description: Assigns the TWI peripheral to PDCA channels and enables 
			TWI and PDCA interrupts.
*/
void iicSetup()
{
	/* Assign TWI to DMA */
	DMA_TX.psr = TWIM_TX_PID;
	DMA_RX.psr = TWIM_RX_PID;

	/* Enable IIC interrupts */
	INTC_register_interrupt(&iicIntHanlder, AVR32_TWIM_IRQ, 3);	
}

/*
Function: iicSetClkSpeed
Params: pbaClkSpeed: speed of the PBA clock.
		iicClkSpeed: desired speed of the TWI clock.
Returns: IIC_ERROR: desired clock speed is out of bounds.
		IIC_OK: desired clock speed is in bounds and has been set.
Description: Sets the speed of the TWI clock.
*/
IIC_STATUS_t iicSetClkSpeed(uint32_t pbaClkSpeed, uint32_t iicClkSpeed)
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
Function: iicEnableInterrupt
Params: IIC_INTERRUPT_t interrupt: specified TWI interrupt.
Returns: none
Description: enables TWI interrupt(s).
*/
void iicEnableInterrupt(IIC_INTERRUPT_t interrupt)
{
	TWIM.ier = interrupt;
}

/*
Function: iicdisableInterrupt
Params: IIC_INTERRUPT_t interrupt: specified TWI interrupt.
Returns: none
Description: disables TWI interrupt(s).
*/
void iicDisableInterrupt(IIC_INTERRUPT_t interrupt)
{
	TWIM.idr = interrupt;
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

/*
Function: iicInterruptMask
Params: none
Returns: The TWI interrupt mask register stating which interrupts are currently enabled.
Description: enables TWI interrupt(s).
*/
uint32_t iicInterruptMask()
{
	return TWIM.imr; 
}

/*
Function: iicStatus
Params: none.
Returns: The TWI status register.
Description: Returns the TWI status register.
*/
uint32_t iicReadStatus()
{
	return TWIM.sr;
}

void iicClearStatus(uint32_t status)
{
	TWIM.scr = status;
}

/*
Function: iicStartWrite
Params: uint8_t saddr: slave device address.
		uint8_t size: size of data to send.
		void* data: pointer to data to send.
Returns: IIC_STATUS_t
Description: Adds a new data packet to the IIC buffer.
*/
IIC_STATUS_t iicNewTransmission(uint8_t read, uint8_t saddr, uint8_t size, void* data)
{
	/*Disable global interrupts.*/
	systemDisableInterrupts();
	
	/*Create a new data packet and assign the data to it.*/
	iicDataPacket* iicNewData = new iicDataPacket;
	iicNewData->saddr = saddr;
	iicNewData->size = size;
	iicNewData->data = data;
	iicNewData->read = read;	
	
	/* If there's no on going transmission, put the data in the first data packet after the head. */
	if ((iicReadStatus() & IIC_MENB_bm) == 0)
	{
		iicDataPacketHead.nextPacket = iicNewData;	
		iicEnableInterrupt(IIC_BUSFREE_bm);
		iicEnableMaster();
	}
	/* If there is an on going transmission, put the new packet at the end of the list. */
	else
	{
		LED2_ON;
		iicDataPacket* end = &iicDataPacketHead;
		while (end->nextPacket != NULL)
		{
			end = end->nextPacket;
		}
		end->nextPacket = iicNewData;
		iicNewData->nextPacket = NULL;
	}


}


