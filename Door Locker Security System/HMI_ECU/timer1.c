/*
 ===================================================================================================
 Module      : TIMER
 File Name   : timer1.c
 Author      : Mohamed Samy
 Description : Source file for the TIMER! AVR driver
 ===================================================================================================
 */

/*******************************************************************************
 *                    Include Library                                          *
 *******************************************************************************/
#include "timer1.h"
#include "std_types.h"
#include "common_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/*******************************************************************************
 *                    Static Functions Prototypes                              *
 *******************************************************************************/

static void Timer1_CTCModeConfiguration(const Timer1_ConfigType * Config_Ptr);
static void Timer1_NormalModeConfiguration(const Timer1_ConfigType * Config_Ptr);


/*******************************************************************************
 *                               Global Pointer                                *
 *******************************************************************************/

void (*g_Ptr_OVFCallBack)(void);
void (*g_Ptr_CTCCallBack)(void);
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

ISR(TIMER1_OVF_vect)
{
	g_Ptr_OVFCallBack();
}

ISR(TIMER1_COMPA_vect)
{
	g_Ptr_CTCCallBack();
}

/* Description :
 * Function to initialize the Timer driver
 * */
void Timer1_init(const Timer1_ConfigType * Config_Ptr)
{
	/* check Timer Mode*/
	switch (Config_Ptr->mode)
	{
		case Normal_Mode:	Timer1_NormalModeConfiguration(Config_Ptr);	break;
		case CTC_Mode   :   Timer1_CTCModeConfiguration(Config_Ptr)   ;	break;
		default:                                               		    break;
	}

}
/* Description :
 * Function to disable the Timer1.
 * */
void Timer1_deInit(void)
{
	TCCR1B = TCCR1B & TIMER1_PRESCALLER_MASK;
	TCNT1=0;
	TCCR1A=0;
	TIMSK=0;
	OCR1A=0;
}
/* Description :
 * Function to set the Call Back function address.
 * */
void Timer1_setCallBack(uint8 Copy_U8_Mode , void(*a_ptr)(void))
{
	switch(Copy_U8_Mode)
	{
	case Normal_Mode:g_Ptr_OVFCallBack = a_ptr; break;
	case CTC_Mode:   g_Ptr_CTCCallBack = a_ptr; break;

	default:                                    break;
	}

}
/*******************************************************************************************/

/*******************************************************************************
 *                    Static Functions Definitions                             *
 *******************************************************************************/
static void Timer1_NormalModeConfiguration(const Timer1_ConfigType * Config_Ptr)
{
	/* Normal port operation, OC1A/OC1B disconnected.
	 * Compare Output Mode for Compare unit A  (COM1A1 =0 COM1A0=0)
	 * Compare Output Mode for Compare unit B  (COM1B1 =0 COM1B0=0)
	 * FOC1A = 1: Force Output Compare for Compare unit A --> because non-PWM mode
	 * Waveform Generation Mode (Set Normal MODE)
	 *  Set  (prescaler)
	 *  TCNT1 =0
	 * Overflow Interrupt Enable
	 * */

	SET_BIT(TCCR1A,FOC1A);
	SET_BIT(TCCR1A,FOC1B);
	if(Config_Ptr->mode==Normal_Mode)
	{
		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		CLEAR_BIT(TCCR1B,WGM12);
		CLEAR_BIT(TCCR1B,WGM13);

	}
	TCCR1B = ((TCCR1B & TIMER1_PRESCALLER_MASK)|(Config_Ptr->prescaler & 0x07));
	SET_BIT(TIMSK,TOIE1);
	TCNT1  = Config_Ptr->initial_value;
}
static void Timer1_CTCModeConfiguration(const Timer1_ConfigType * Config_Ptr)
{
	/* Normal port operation, OC1A/OC1B disconnected.
	 * Compare Output Mode for Compare unit A  (COM1A1 =0 COM1A0=0)
	 * Compare Output Mode for Compare unit B  (COM1B1 =0 COM1B0=0)
	 * FOC1A = 1: Force Output Compare for Compare unit A --> because non-PWM mode
	 * Waveform Generation Mode (Set CTC Mode)
	 *  Set  (prescaler)
	 *  TCNT1 =0
	 *  OCR1A = top --> (31250 to generate interrupt each 1 sec at prescaller 256)
	 * Output Compare A Match Interrupt Enable
	 * */
	SET_BIT(TCCR1A,FOC1A);
	SET_BIT(TCCR1A,FOC1B);
	if(Config_Ptr->mode==CTC_Mode)
	{
		CLEAR_BIT(TCCR1A,WGM10);
		CLEAR_BIT(TCCR1A,WGM11);
		SET_BIT  (TCCR1B,WGM12);
		CLEAR_BIT(TCCR1B,WGM13);
	}
	TCCR1B = ((TCCR1B & TIMER1_PRESCALLER_MASK)|(Config_Ptr->prescaler & 0x07));
	SET_BIT(TIMSK,OCIE1A);
	TCNT1  = Config_Ptr->initial_value;
	OCR1A  = Config_Ptr->compare_value;
}
