/*
 ===================================================================================================
 Module      : TIMER
 File Name   : timer1.h
 Author      : Mohamed Samy
 Description : Header file for the TIMER! AVR driver
 ===================================================================================================
 */

#ifndef TIMER1_H_
#define TIMER1_H_

/*******************************************************************************
 *                    Include Library                                          *
 *******************************************************************************/
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define TIMER1_PRESCALLER_MASK	0xF8

/*******************************************************************************
 *                       TiMER1 Configurations                                 *
 *******************************************************************************/
typedef enum
{
	No_Clock_Source,
	clkIO_1,
	clkIO_8,
	clkIO_64,
	clkIO_256,
	clkIO_1024
}Timer1_Prescaler;
typedef enum
{
	Normal_Mode,
	CTC_Mode
}Timer1_Mode;
typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/* Description :
 * Function to initialize the Timer driver
 * */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);
/*Description :
 * Function to disable the Timer1.
 * */
void Timer1_deInit(void);
/*Description :
 * Function to set the Call Back function address.
 * */
void Timer1_setCallBack(uint8 Copy_U8_Mode , void(*a_ptr)(void));

#endif /* TIMER1_H_ */
