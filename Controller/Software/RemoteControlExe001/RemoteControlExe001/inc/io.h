/*
 * io.h
 *
 * Created: 10/16/2022 1:46:46 PM
 *  Author: AJ992
 */ 


#ifndef IO_H_
#define IO_H_

#define LED_PORT						PORTB
#define LED1_PIN						1 << 9
#define LED2_PIN						1 << 10
#define LED3_PIN						1 << 11
#define LED1_ON							ioPinHigh(&LED_PORT, LED1_PIN);	
#define LED2_ON							ioPinHigh(&LED_PORT, LED2_PIN);	
#define LED3_ON							ioPinHigh(&LED_PORT, LED3_PIN);	
#define LED1_OFF						ioPinLow(&LED_PORT, LED1_PIN);	
#define LED2_OFF						ioPinLow(&LED_PORT, LED2_PIN)
#define LED3_OFF						ioPinLow(&LED_PORT, LED3_PIN)

#define CLK_OUT_PIN_bm					1 << 30

#define SW_PORT							PORTA
#define SW1_bm							1 << 21
#define SW2_bm							1 << 22
#define SW3_bm							1 << 23
#define SW4_bm							1 << 24
#define SW5_bm							1 << 25

#define PORTA							(*(avr32_gpio_port_t*) 0xFFFF2800)
#define PORTB							(*(avr32_gpio_port_t*) 0xFFFF2A00)
#define GPIO_IMR_CHANGE_bm				0
#define GPIO_IMR_RISING_bm				1
#define GPIO_IMR_FALLING_bm				1 << 2

/*****GPIO Control*****/
void ioSetPinIO(avr32_gpio_port_t* port, uint32_t pin);							//Sets pin to GPIO function.
void ioSetPinPeripheral(avr32_gpio_port_t* port, uint32_t pin);					//Sets pin to peripheral control.
void ioSetPinOutput(avr32_gpio_port_t* port, uint32_t pin);						//Sets pin as an output.
void ioSetPinInput(avr32_gpio_port_t* port, uint32_t pin);						//Sets pin as an input.
void ioPinHigh(avr32_gpio_port_t* port, uint32_t pin);							//Drives pin high.
void ioPinLow(avr32_gpio_port_t* port, uint32_t pin);							//Drives pin low.
uint32_t ioReadPort(avr32_gpio_port_t* port);									//Returns the pin value register for Port x.
void ioEnableInterrupt(avr32_gpio_port_t* port, uint32_t pin);					//Enables an interrupt.
void ioDisableInterrupt(avr32_gpio_port_t* port, uint32_t pin);					//Disables an interrupt.
void ioInterruptMode(avr32_gpio_port_t* port, uint8_t intMode, uint32_t pin);	//Sets interrupt mode.
void ioClearIntFlag(avr32_gpio_port_t* port, uint32_t pin);						//Clears an interrupt flag.

#endif /* IO_H_ */