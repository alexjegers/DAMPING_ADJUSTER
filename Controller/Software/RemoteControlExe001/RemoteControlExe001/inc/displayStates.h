/*
 * state_machine.h
 *
 * Created: 11/4/2022 6:43:21 PM
 *  Author: AJ992
 */ 


#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

typedef enum
{
	UP_SWITCH =		0,
	DOWN_SWITCH,
	LEFT_SWITCH,
	RIGHT_SWITCH,
	CENTER_SWITCH,
	NONE
}LCD_DISPLAY_EVENTS;

extern LCD_DISPLAY_EVENTS displayCurrentEvent;

void setBtnEventHandlers();
void handleNewEvent(LCD_DISPLAY_EVENTS newEvent);
void noChange();
void stateA();
void stateB();
void stateC();
void stateD();
void stateE();
void stateF();
void stateG();
void stateH();
void stateI();
void stateJ();
void stateK();
void stateL();
void stateM();

#endif /* STATE MACHINE_H_ */