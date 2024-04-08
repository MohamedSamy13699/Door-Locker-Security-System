/*
 ===================================================================================================
 Module      : HMI_ECU
 File Name   : HMI_ECU.c
 Author      : Mohamed Samy
 Description : Source file for the HMI_ECU
 ===================================================================================================
 */

/*******************************************************************************
 *                    Include Library                                          *
 *******************************************************************************/

#include "gpio.h"
#include "uart.h"
#include "keypad.h"
#include "HMI_ECU.h"
#include "lcd.h"
#include "timer1.h"
#include "g_interrupt.h"
#include <util/delay.h>
#define  F_CPU	8000000UL
/*******************************************************************************
 *                       SyStem Configurations                                 *
 *******************************************************************************/
UART_ConfigType uart ={
		EIGHT_BIT,
		Disabled,
		ONE_STOP_BIT,
		BaudRate_9600
};
Timer1_ConfigType Timer1_configCTCMode= {
		0,
		NUMBER_OF_TICK,
		clkIO_256,
		CTC_Mode
};
Timer1_ConfigType Timer1_configNormalMode= {
		1374,
		0,
		clkIO_256,
		Normal_Mode
};
/*******************************************************************************
 *                               Global variables                              *
 *******************************************************************************/
uint8 g_Second =0;
volatile uint8 g_NumberOfEnterpass=0;

/*******************************************************************************
 *                             Application                                     *
 *******************************************************************************/
int main()
{

/*******************************************************************************
 *                               LOCAL variables                               *
 *******************************************************************************/
	uint8 Local_CheckPassFlag=0xff;
	uint8 Local_Operation = 0;
	uint8 local_flag = 0;
	uint8 Step1_Flag = 1;
	uint8 Step2_Flag = 1;

	/* Enable the Global Interrupt */
	Global_interruptEnable();
	/*Call initialization function of LCD driver*/
	LCD_init();
	/*Call initialization function of UART driver*/
	UART_init(&uart);
	/*Call initialization function of TIMER1 driver*/
	Timer1_init(&Timer1_configCTCMode);
	/*CallBack Function  of Timer1  of CTC Mode*/
	Timer1_setCallBack(CTC_Mode,&A_DLSS_VoidGenerateSecond);
	/*CallBack Function  of Timer1  of Normal Mode*/
	Timer1_setCallBack(Normal_Mode,&A_DLSS_VoidGenerateMinute);
	LCD_clearScreen();
	LCD_displayString("   Welcome to ");
	_delay_ms(500);
	LCD_clearScreen();
	LCD_displayString("Door Locker");
	LCD_moveCursor(1,0);
	LCD_displayString("Security System");
	_delay_ms(1000);
	while(1)
	{
/*********************************************************************************************************/
/*************************************** STEP 1***********************************************************/
/*********************************************************************************************************/

		while(Step1_Flag)
		{
			/*Call A_DLSS_VoidTakePassword_1 to take the pass for first time */
			A_DLSS_VoidTakePassword_1();
			/*Call A_DLSS_VoidTakePassword_1 to take the pass for second time */
			A_DLSS_VoidTakePassword_2();
			/* Receive the check password flag  */
			Local_CheckPassFlag = UART_recieveByte();
			_delay_ms(50);
			/* if the two passwords matched */
			if(Local_CheckPassFlag == PASS_MATCHED)
			{
				LCD_clearScreen();
				_delay_ms(20);
				LCD_displayString("Correct Pass");
				_delay_ms(500);
				Step1_Flag =0; /* Go out from this while loop for first step */
				Step2_Flag =2; /* Active the Second State Loop*/

			}
			/* if the two passwords not matched */
			else if (Local_CheckPassFlag == PASS_NOT_MATCHED)
			{
				/* Display
				 * Stay on this loop until the two password matched
				 */
				LCD_clearScreen();
				_delay_ms(20);
				LCD_displayString("Not Matched");
				_delay_ms(500);

			}
		}
/*********************************************************************************************************/
/*********************************** END OF STEP 1 *******************************************************/
/*********************************************************************************************************/


/*********************************************************************************************************/
/*************************************** STEP 2***********************************************************/
/*********************************************************************************************************/

		/* this loop will be active only if the two password are matched at first step
		 * and the user enter the pass less than MAX_TRIES_ENTER_PASS
		 */
			while(Step2_Flag == 2 && g_NumberOfEnterpass < MAX_TRIES_ENTER_PASS)
			{

				/* take the operation for open door or change pass*/
				Local_Operation = A_DLSS_u8Step2TakeOperation();
/*********************************************************************************************************/
/*********************************** END OF STEP 2 *******************************************************/
/*********************************************************************************************************/

/*********************************************************************************************************/
/*************************************** STEP 3***********************************************************/
/*********************************************************************************************************/

				/*if  the operation is open door */
				if(Local_Operation == OPEN_DOOR)
				{
					/* initial value of receive flag from Control ECU */
					local_flag =0;
					/* Send open door command to control ECU*/
					UART_sendByte(OPEN_DOOR);
					_delay_ms(20);
					/* Send the same password */
					A_DLSS_VoidStep3EnterPass();
					/* receive byte of Control ECU to check the pass*/
					local_flag = UART_recieveByte();
					if(local_flag == PASS_MATCHED)
					{
						/* clear the number of entered password Wrong */
						g_NumberOfEnterpass=0;
						LCD_clearScreen();
						_delay_ms(20);
						/* Motor State */
						UART_sendByte(TURN_MOTOR_ON);
						/* Display the state of the door*/
						A_DLSS_VoidStep_3_MotorState();

					}else if(local_flag == PASS_NOT_MATCHED)
					{
						/* Increment the number of entered password Wrong */
						g_NumberOfEnterpass++;
						/* Send PASS_NOT_MATCHED
						 * and display it on LCD
						 * and repeat this step if the number of tries less the the max */
						UART_sendByte(PASS_NOT_MATCHED);
						LCD_clearScreen();
						LCD_displayString("Wrong Pass");
						_delay_ms(50);
						LCD_displayStringRowColumn(1,0,"Have    tries");
						LCD_moveCursor(1,6);
						LCD_intgerToString(MAX_TRIES_ENTER_PASS - g_NumberOfEnterpass);
						_delay_ms(500);
					}

/*********************************************************************************************************/
/*********************************** END OF STEP 3 *******************************************************/
/*********************************************************************************************************/

/*********************************************************************************************************/
/*************************************** STEP 4***********************************************************/
/*********************************************************************************************************/

				}else if(Local_Operation == CHANGE_PASS)
				{
					/* initial value of receive flag from Control ECU */
					local_flag =0;
					/* Send change password command to control ECU*/
					UART_sendByte(CHANGE_PASS);
					_delay_ms(20);
					/* Send the same password */
					A_DLSS_VoidStep4ChangePass();
					/* receive byte of Control ECU to check the pass*/
					local_flag = UART_recieveByte();
					if(local_flag == PASS_MATCHED)
					{
						/* go to step 1*/
						UART_sendByte(REPEAT_STEP_1);
						g_NumberOfEnterpass=0;
						Step2_Flag =0;/*  Go out from this loop */
						Step1_Flag=1;/* Active step 1 loop to enter the two passwords again*/


					}else if(local_flag == PASS_NOT_MATCHED)
					{
						/* Increment the number of entered password Wrong */
						g_NumberOfEnterpass++;
						/* Send PASS_NOT_MATCHED command
						 * and display it on LCD
						 * and repeat this step if the number of tries less the the max
						 * */
						UART_sendByte(PASS_NOT_MATCHED);
						LCD_clearScreen();
						LCD_displayString("Wrong Pass");
						_delay_ms(50);
						LCD_displayStringRowColumn(1,0,"Have    tries");
						LCD_moveCursor(1,6);
						LCD_intgerToString(MAX_TRIES_ENTER_PASS - g_NumberOfEnterpass);
						_delay_ms(500);

					}
				}
/*********************************************************************************************************/
/*********************************** END OF STEP 4 *******************************************************/
/*********************************************************************************************************/



			}
			if(g_NumberOfEnterpass == MAX_TRIES_ENTER_PASS)
			{

/*********************************************************************************************************/
/*************************************** STEP 5***********************************************************/
/*********************************************************************************************************/
				/* Send block system command to control ECU*/
				UART_sendByte(BLOCK_SYSTEM);
				_delay_ms(20);
				A_DLSS_VoidStep5ErrorMassage();
				/* the system will still here until the interrupt clear the g_NumberOfEnterpass*/
				while(g_NumberOfEnterpass == MAX_TRIES_ENTER_PASS);
				/* Send Unblock system command to control ECU after 1 minute*/
				UART_sendByte(UNBLOCK_SYSTEM);
				_delay_ms(20);
/*********************************************************************************************************/
/*********************************** END OF STEP 5 *******************************************************/
/*********************************************************************************************************/
			}
		}/*end of main while loop*/

	return 0;
}/*end of Application*/
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void A_DLSS_VoidTakePassword_1(void)
{
	uint8 Local_u8KeyValue= KEY_NOT_PRESSED;
	uint8 Copy_u8_counter =0;
	LCD_clearScreen();
	_delay_ms(20);
	LCD_displayString("PLZ Enter Pass: ");
	_delay_ms(10);
	LCD_moveCursor(1,4);

	while(Copy_u8_counter < PASS_SIZE)
	{

		Local_u8KeyValue = KEYPAD_getPressedKey();
		if((Local_u8KeyValue >= 0) && (Local_u8KeyValue <= 9))
		{
			UART_sendByte(Local_u8KeyValue);
			_delay_ms(50);
			LCD_displayCharacter('*');
			Copy_u8_counter++;
		}

	}
	while(KEYPAD_getPressedKey() != '=' );

}
void A_DLSS_VoidTakePassword_2(void)
{
	uint8 Local_u8KeyValue= KEY_NOT_PRESSED;
	uint8 Copy_u8_counter =0;
	LCD_clearScreen();
	_delay_ms(20);
	LCD_displayString("PLZ Re-Enter the");
	_delay_ms(10);
	LCD_moveCursor(1,0);
	LCD_displayString("same pass:");
	while(Copy_u8_counter < PASS_SIZE)
	{

		Local_u8KeyValue = KEYPAD_getPressedKey();
		if((Local_u8KeyValue >= 0) && (Local_u8KeyValue <= 9))
		{
			UART_sendByte(Local_u8KeyValue);
			_delay_ms(50);
			LCD_displayCharacter('*');
			Copy_u8_counter++;
		}

	}
	while(KEYPAD_getPressedKey() != '=');
}

uint8 A_DLSS_u8Step2TakeOperation(void)
{
	uint8 Local_u8KeyValue= KEY_NOT_PRESSED;
	while(1)
	{
		LCD_clearScreen();
		_delay_ms(20);
		LCD_displayString("+ : Open Door ");
		_delay_ms(10);
		LCD_moveCursor(1,0);
		_delay_ms(20);
		LCD_displayString("- : Change pass ");
		Local_u8KeyValue = KEYPAD_getPressedKey();
		if( Local_u8KeyValue == '+' || Local_u8KeyValue == '-')
		{
			return Local_u8KeyValue;
		}else
		{
			LCD_clearScreen();
			_delay_ms(20);
			LCD_displayString("Enter correct ");
			_delay_ms(10);
			LCD_moveCursor(1,0);
			LCD_displayString("operation ");
			_delay_ms(500);
		}
	}

}
void A_DLSS_VoidStep3EnterPass(void)
{
	uint8 Local_u8KeyValue= KEY_NOT_PRESSED;
	uint8 Copy_u8_counter =0;
	LCD_clearScreen();
	_delay_ms(20);
	LCD_displayString("Open door ");
	_delay_ms(500);
	LCD_clearScreen();
	_delay_ms(20);
	LCD_displayString("PLZ Enter Pass: ");
	_delay_ms(10);
	LCD_moveCursor(1,4);

	while(Copy_u8_counter < PASS_SIZE)
	{

		Local_u8KeyValue = KEYPAD_getPressedKey();
		if((Local_u8KeyValue >= 0) && (Local_u8KeyValue <= 9))
		{
			UART_sendByte(Local_u8KeyValue);
			_delay_ms(50);
			LCD_displayCharacter('*');
			Copy_u8_counter++;
		}

	}
	while(KEYPAD_getPressedKey() != '=');
}
void A_DLSS_VoidStep_3_MotorState(void)
{

	Timer1_init(&Timer1_configCTCMode);
	g_Second =0;
	LCD_clearScreen();

	while(g_Second < TIME_OF_DOOR_UNLOOKED)
	{
		LCD_displayStringRowColumn(0,0,"Door Unlocking");
	}
	g_Second =0;
	LCD_clearScreen();
	while(g_Second < TIME_OF_MOTOR_HOLD)
	{
		LCD_displayStringRowColumn(0,0,"Door Hold");
	}
	g_Second=0;
	LCD_clearScreen();

	while(g_Second < TIME_OF_DOOR_LOOKED)
	{
		LCD_displayStringRowColumn(0,0,"Door is locked");
	}

	g_Second=0;
	Timer1_deInit();
	LCD_clearScreen();

}
void A_DLSS_VoidGenerateSecond(void)
{
	g_Second++;
}

void A_DLSS_VoidStep4ChangePass(void)
{
	uint8 Local_u8KeyValue= KEY_NOT_PRESSED;
	uint8 Copy_u8_counter =0;
	LCD_clearScreen();
	_delay_ms(20);
	LCD_displayString("Change Pass ");
	_delay_ms(500);
	LCD_clearScreen();
	_delay_ms(20);
	LCD_displayString("PLZ Enter Pass: ");
	_delay_ms(10);
	LCD_moveCursor(1,4);

	while(Copy_u8_counter < PASS_SIZE)
	{

		Local_u8KeyValue = KEYPAD_getPressedKey();
		if((Local_u8KeyValue >= 0) && (Local_u8KeyValue <= 9))
		{
			UART_sendByte(Local_u8KeyValue);
			_delay_ms(50);
			LCD_displayCharacter('*');
			Copy_u8_counter++;
		}

	}
	while(KEYPAD_getPressedKey() != '=');
}


void  A_DLSS_VoidStep5ErrorMassage(void)
{
	Timer1_init(&Timer1_configNormalMode);
	LCD_clearScreen();
	LCD_displayString("ERROR!!!!!");
	LCD_moveCursor(1,0);
	_delay_ms(20);
	LCD_displayString("Wait Minute");
}
void A_DLSS_VoidGenerateMinute(void)
{
	static uint8 counter =0;
	counter++;
	if(counter == 11)//29
	{
		/* Clear the number of entered password Wrong */
		g_NumberOfEnterpass=0;
		Timer1_deInit();
		counter =0;
	}
}
