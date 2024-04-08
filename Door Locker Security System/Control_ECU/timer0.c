/*
 ===================================================================================================
 Module      : TIMER0
 File Name   : timer0.c
 Author      : Mohamed Samy
 Date        : Feb 18, 2024
 ===================================================================================================
 */

/*******************************************************************************
 *                          Include Libraries                                 *
 *******************************************************************************/
#include <avr/io.h>
#include "gpio.h"
#include "common_macros.h"
#include "timer0.h"

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/
void PWM_Timer0_Start(uint8 duty_cycle)
{
	/*  TCNT0 = 0 */
	    TCNT0 = 0;
	/*  OCR0  =  duty_cycle */
	    OCR0  =  duty_cycle;
	/* Config OC0 As Output PB3*/
	GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);

	/* FOC0: 0  Force Output Compare because we use PWM mode	*/
		CLEAR_BIT(TCCR0,FOC0);

	/* select Waveform Generation Mode
	 * 		select PWM mode
	 * 			WGM01 = 1
	 *			WGM00 = 1
   	 */
		SET_BIT(TCCR0,WGM01);
		SET_BIT(TCCR0,WGM00);

	/* Clear OC0 on compare match, set OC0 at BOTTOM, (nin-inverting mode)
	 * 			COM01 = 1
	 * 		    COM00 = 0
	 */
		SET_BIT(TCCR0,COM01);
		CLEAR_BIT(TCCR0,COM00);

	/* select Clock Select Bit Description  clk/8 (From prescaler)
	 * 			CS02 = 0
	 * 			CS01 = 1
	 * 			CS00 = 0
	 */
		SET_BIT(TCCR0,CS01);
		CLEAR_BIT(TCCR0,CS02);
		CLEAR_BIT(TCCR0,CS00);
}
