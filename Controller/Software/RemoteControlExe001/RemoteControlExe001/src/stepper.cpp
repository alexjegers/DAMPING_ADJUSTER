/*
 * stepper.cpp
 *
 * Created: 11/9/2022 7:44:24 PM
 *  Author: AJ992
 */ 
#include <avr32/io.h>
#include <stdint.h>
#include "stepper.h"
#include "iic.h"
#include "system.h"

STEPPER_MOTOR frontLeft;
STEPPER_MOTOR frontRight;
STEPPER_MOTOR rearLeft;
STEPPER_MOTOR rearRight;

/* Stepper motor static member definitions. */
STEP_MODE_t STEPPER_MOTOR::STEPPER_INFO_struct::stepMode = MODE_HALF_STEP_bm;
CURRENT_LIMIT_t STEPPER_MOTOR::STEPPER_INFO_struct::currentLimit = ONE_AMP_bm;
DECAY_MODE_t STEPPER_MOTOR::STEPPER_INFO_struct::decayMode = DECAY_MIXED;
uint16_t STEPPER_MOTOR::STEPPER_INFO_struct::speedInRPM = 100;


void STEPPER_MOTOR::sendData(STEPPER_INTERNAL_ADDR_t addr, void* data, uint8_t numBytes)
{

}

void STEPPER_MOTOR::requestData(STEPPER_INTERNAL_ADDR_t addr, void* data, uint8_t numbytes)
{

}

void STEPPER_MOTOR::setDeviceAddr(uint8_t addr)
{
	deviceAddr = addr;
}
void STEPPER_MOTOR::saveSettings()
{
	
}