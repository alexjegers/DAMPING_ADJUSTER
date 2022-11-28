/*
 * system.h
 *
 * Created: 9/7/2022 7:48:51 PM
 *  Author: AJ992
 */ 


#ifndef SYSTEM_H_
#define SYSTEM_H_

/*Port A Pins*/
#define ENC_A_PIN				PIN0_bm
#define ENC_B_PIN				PIN1_bm
#define LED1_PIN				PIN2_bm
#define LED2_PIN				PIN3_bm
#define LED3_PIN				PIN4_bm
#define CLK_OUT_PIN				PIN7_bm

/*Port C Pins*/
#define STEP_PIN				PIN0_bm
#define DIR_PIN					PIN1_bm
#define nENBL_PIN				PIN2_bm
#define nHOME_PIN				PIN3_bm

/*Port D Pins*/
#define MODE0_PIN				PIN0_bm
#define MODE1_PIN				PIN1_bm
#define MODE2_PIN				PIN2_bm
#define nRESET_PIN				PIN3_bm
#define nSLEEP_PIN				PIN4_bm
#define nFAULT_PIN				PIN5_bm
#define VREF_PIN				PIN6_bm
#define DECAY_PIN				PIN7_bm

/*Port F Pins*/
#define SDA_PIN					PIN2_bm
#define SCL_PIN					PIN3_bm
#define AIN_3V3_PIN				PIN4_bm
#define AIN_12V_PIN				PIN5_bm

/*Debug LED Control*/
#define LED1_ON					PORTA.OUT |= LED1_PIN
#define LED2_ON					PORTA.OUT |= LED2_PIN
#define LED3_ON					PORTA.OUT |= LED3_PIN
#define LED1_OFF				PORTA.OUT &= ~LED1_PIN
#define LED2_OFF				PORTA.OUT &= ~LED2_PIN
#define LED3_OFF				PORTA.OUT &= ~LED3_PIN

/*Periods for timeout timers*/
#define TCB_FREQ_15HZ			1562
#define TCB_PER_EIGHT_SEC		2929
#define TCB_PER_QUART_SEC		5859
#define TCB_PER_HALF_SEC		11718
#define TCB_PER_1SEC			23437
#define TCB_PER_2SEC			46875
#define TCB_PER_2x79SEC			65535


/*Low battery voltage (11V) for ADC window comparator interrupt*/
#define ADC_LOW_VOLTAGE			2388

/*Port configurations*/
void systemPortAConfig();						//Configures Port A pins to default state.
void systemPortCConfig();						//Configures Port C pins to default state.
void systemPortDConfig();						//Configures Port D pins to default state.
void systemPortFConfig();						//Configures Port F pins to default state.

/*Clock config*/
void systemClockConfig();						//Configures to clock to 24 MHz, no prescaler.

/*ADC Controls*/
void systemConfigADC();							//Configures the ADC and interrupt.
uint16_t systemADCResult();						//Returns the most recent value in the ADC result register.

/*Timer Controls*/
void systemConfigTCB(TCB_t *tcb, uint16_t cmp);//Configures a TCB to be used as a time-out timer.
void systemEnableTCB(TCB_t *tcb);				//Start TCB.
void systemDisableTCB(TCB_t *tcb);				//Stop TCB.
void systemEnableTCA0();						//The TCBs use TCA as a clock source, call this on start up.

#endif /* SYSTEM_H_ */