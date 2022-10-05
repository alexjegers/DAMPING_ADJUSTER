/*
 * system.c
 *
 * Created: 9/7/2022 7:49:00 PM
 *  Author: AJ992
 */ 

#include <avr/io.h>
#include "iic.h"
#include "system.h"

/*
Function:
Params:
Returns:
Description:
*/


/*
Function: systemPortAConfig
Params:
Returns:
Description: Configures all the pins in port A.
*/
void systemPortAConfig()
{
	
	PORTA.DIR |= LED1_PIN								//LED Pins as outputs.
				| LED2_PIN
				| LED3_PIN
				| CLK_OUT_PIN;							//CLKOUT pin as output.
				
	LED1_OFF;											//Start with all LEDs off.
	LED2_OFF;
	LED3_OFF;
	
	PORTA.DIR &= ~(ENC_A_PIN							//Encoder pins as input.	
				| ENC_B_PIN);
				
	PORTA.PIN0CTRL = PORT_ISC_BOTHEDGES_gc;				//Encoder A pin interrupt enabled.
	
	CCP = CCP_IOREG_gc;
	CLKCTRL.MCLKCTRLA = CLKCTRL_CLKOUT_bm;				//Clock out on PA7 (CLK_OUT_PIN)
}

/*
Function: systemPortCConfig
Params: none
Returns: none
Description: Configures all the pins in port C.
*/
void systemPortCConfig()
{
	PORTC.DIR |= DIR_PIN								//Direction, step, enable, and home pin all outputs.
				| STEP_PIN
				| nENBL_PIN;
				
	PORTC.OUT &= ~(nENBL_PIN);							//Enable pin low enables the DRV8825.
				
	PORTC.DIR &= ~nHOME_PIN;							//Home as input.
	
	TCB2.CTRLB = TCB_CCMPEN_bm							//Waveform will override step pin output.
				|TCB_CNTMODE_PWM8_gc;					//TCB2 set to 8-bit PWM mode.
	TCB2.CTRLA = TCB_CLKSEL_TCA0_gc;					//TCB2 uses TCA0 as clock source.
}

/*
Function: systemPortDConfig
Params: none
Returns: none
Description: Configures all the pins in port D.
*/
void systemPortDConfig()
{
	PORTD.DIR |= MODE0_PIN								//Mode pins, reset, sleep, and decay as outputs.
				| MODE1_PIN
				| MODE2_PIN
				| nRESET_PIN
				| nSLEEP_PIN
				| DECAY_PIN;
				
	PORTD.OUT |= (nSLEEP_PIN | nRESET_PIN);				//Sleep and reset needed high for DRV8825 operation.
				
	PORTD.DIR &= ~nFAULT_PIN;							//Fault as input.
	
	VREF.DAC0REF = VREF_ALWAYSON_bm						//DAC reference voltage always on for VREF_PIN.
					| VREF_REFSEL_2V500_gc;				//DAC VREF set to 2.5V.
}

/*
Function: systemPortFConfig
Params: none
Returns: none
Description: Configures all the pins in port F.
*/
void systemPortFConfig()
{
	PORTF.DIR &= ~(AIN_12V_PIN | AIN_3V3_PIN);			//Analog pins as inputs
	iicInit();
}

/*
Function: systemClockConfig
Params: none
Returns: none
Description: Selects the 24MHz clock with no prescaler for the main/master clock.
*/
void systemClockConfig()
{
	CCP = CCP_IOREG_gc;
	CLKCTRL.OSCHFCTRLA = CLKCTRL_FREQSEL_24M_gc			//24MHz frequency.
					| CLKCTRL_AUTOTUNE_bm;				//Enable autotune.
					
	CCP = CCP_IOREG_gc;					
	CLKCTRL.MCLKCTRLB = 0;						 		//No prescaler, 24MHz clock.
}


/*
Function: systemConfigADC
Params: none
Returns: none
Description: Configure the ADC to sample the supply voltage.
*/
void systemConfigADC()
{
	VREF.ADC0REF |= VREF_REFSEL_2V500_gc;				//ADC0 reference as 2.5V;	
	ADC0.CTRLA |= ADC_RESSEL_12BIT_gc					//12 bit conversions.
				| ADC_FREERUN_bm;						//Enable freerunning mode.
	ADC0.CTRLC |= ADC_PRESC_DIV256_gc;					//ADC clock equals system clock div 2.
	ADC0.SAMPCTRL = 20;									//Set sample length to 20 ADC cycles.
	ADC0.CTRLE = ADC_WINCM_BELOW_gc;					//Compare mode set to below.
	ADC0.MUXPOS = ADC_MUXPOS_AIN21_gc;					//Battery voltage divider is on PF5 which is AIN21.
	ADC0.MUXNEG = ADC_MUXNEG_GND_gc;					//Negative input to ground.
	//ADC0.INTCTRL = ADC_WCMP_bm;						//Enable window comparator interrupt.
	ADC0.WINLT = ADC_LOW_VOLTAGE;						//Low threshold is 11V.
	ADC0.DBGCTRL = ADC_DBGRUN_bm;						//Run while debuggin.
	ADC0.CTRLA |= ADC_ENABLE_bm;						//Enable ADC.
	ADC0.COMMAND = ADC_STCONV_bm;						//Start converting.
}


/*
Function: systemADCResult
Params: none
Returns: The most recent value in the ADC0.RES register.
Description: read returns.
*/
uint16_t systemADCResult()
{
	return ADC0.RES;
}

/*
Function: systemConfigTCB
Params: *tcb: Pointer to a TCB. 
		cmp: The compare value to load into that timer.
Returns: none
Description: Sets up a TCB in periodic interrupt mode to a set interval, cmp.
*/

void systemConfigTCB(TCB_t *tcb, uint16_t cmp)
{
	tcb->CTRLA = TCB_CLKSEL_TCA0_gc;					//Select TCA0 as clock source.
	tcb->CTRLB = TCB_CNTMODE_INT_gc;					//Periodic interrupt mode.
	tcb->INTCTRL = TCB_CAPT_bm;							//Enable capture interrupt.
	tcb->CCMP = cmp;									//Set compare value.
}

/*
Function: systemEnableTCA0
Params: none 
Returns: none
Description: Sets up and enables TCA0. TCA0 is used as the source for TCB.
*/

void systemEnableTCA0()
{
	/*TCA frequency = 23437 Hz = 24,000,000 Hz / 1,024*/
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc			//Will be using TCA0 as source for TCB0
					| TCA_SINGLE_ENABLE_bm;						//Enable TCA0.
	TCA0.SINGLE.DBGCTRL = 1;									//Run while debug.
	TCA0.SINGLE.PER = 0xFFFF;									//Max value for period.
}

/*
Function: systemEnableTCB
Params: *tcb: pointer to a specific TCB.
Returns: none
Description: enables a TCB timer.
*/

void systemEnableTCB(TCB_t *tcb)
{
	tcb->CTRLA |= TCB_ENABLE_bm;
}


/*
Function: systemDisableTCB
Params: *tcb: pointer to a specific TCB.
Returns: none
Description: Disables a TCB timer.
*/

void systemDisableTCB(TCB_t *tcb)
{
	tcb->CTRLA &= ~(TCB_ENABLE_bm);
}