/*
 * encoder.h
 *
 * Created: 9/16/2022 11:39:29 AM
 *  Author: AJ992
 */ 


#ifndef ENCODER_H_
#define ENCODER_H_

#define ROTATING_CCW			5
#define ROTATING_CW				-5
#define ENC_A_LEVEL			(bool)(PORTA.IN & ENC_A_PIN)
#define ENC_B_LEVEL			(bool)(PORTA.IN & ENC_B_PIN)


int8_t encoderRotDirection();

#endif /* ENCODER_H_ */