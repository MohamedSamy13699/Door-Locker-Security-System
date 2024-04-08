/*
 ===================================================================================================
 Module      : DC_MOTOR
 File Name   : dc_motor.c
 Author      : Mohamed Samy
 Date        : Feb 18, 2024
 ===================================================================================================
 */
/*******************************************************************************
 *                          Include Libraries                                 *
 *******************************************************************************/
#include <avr/io.h>
#include "dc_motor.h"
#include "gpio.h"
#include "timer0.h"
#include "std_types.h"

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

void DcMotor_Init(void)
{
	/* setup the direction for the two motor pins through the GPIO driver. */
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_PIN1_ID,PIN_OUTPUT);
	/*Stop at the DC-Motor at the beginning through the GPIO driver.*/
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN0_ID,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN1_ID,LOGIC_LOW);

}
void DcMotor_Rotate(uint8 DcMotor_State,uint8 speed)
{
	if(DcMotor_State == RUN_MOTOR_CW)
	{
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN0_ID,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN1_ID,LOGIC_LOW);
	}else if(DcMotor_State == RUN_MOTOR_A_CW)
	{
		GPIO_writePin(MOTOR_PORT_ID,PIN0_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,PIN1_ID,LOGIC_HIGH);
	}else if(DcMotor_State == STOP_MOTOR)
	{
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN0_ID,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_PIN1_ID,LOGIC_LOW);
	}else
	{

	}
	/* speed  0 ----> 100 %
	 * Duty cycle value 0 -----> 255
	 * */
	uint8 Duty_CycleValue = ( (uint8) ( (float32)(speed * 255) / 100) );
	PWM_Timer0_Start(Duty_CycleValue);
}
