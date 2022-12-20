/*
 * stepper.cpp
 *
 * Created: 11/9/2022 7:44:24 PM
 *  Author: AJ992
 */ 
#include <avr32/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iic.h"
#include "system.h"
#include "stepper.h"
using namespace iic;

STEPPER_MOTOR frontLeft;
STEPPER_MOTOR frontRight;
STEPPER_MOTOR rearLeft;
STEPPER_MOTOR rearRight;

STEPPER_MOTOR::STEPPER_MOTOR()
{
	stepperInfo.stepMode = MODE_HALF_STEP_bm;
	stepperInfo.currentLimit = ONE_AMP_bm;
	stepperInfo.decayMode = DECAY_MIXED;
	stepperInfo.speedInRPM = 100;	
	memset(positionChar, 0x30, 2);
}

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