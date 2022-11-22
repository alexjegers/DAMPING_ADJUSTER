/*
 * interrupts.h
 *
 * Created: 10/16/2022 2:40:17 PM
 *  Author: AJ992
 */ 


#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#define INTC				(*(avr32_intc_t*)0xFFFF1000)
#define EVBA				(void*)0x04


void intHandlerPortA();
void intEnableSwitchInterrupts();


#endif /* INTERRUPTS_H_ */