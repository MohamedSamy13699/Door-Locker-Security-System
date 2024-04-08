/*
 ===================================================================================================
 Module      : DC_MOTOR
 File Name   : dc_motor.h
 Author      : Mohamed Samy
 Date        : Feb 18, 2024
 ===================================================================================================
 */
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

/*******************************************************************************
 *                          Include Libraries                                 *
 *******************************************************************************/
#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
#define MOTOR_PORT_ID	PORTB_ID
#define MOTOR_PIN0_ID	PIN0_ID
#define MOTOR_PIN1_ID	PIN1_ID
enum DcMotor_State
{
	RUN_MOTOR_CW,
	RUN_MOTOR_A_CW,
	STOP_MOTOR,
};
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void DcMotor_Init(void);
void DcMotor_Rotate(uint8 DcMotor_State,uint8 speed);



#endif /* DC_MOTOR_H_ */
