/*
 ================================================================================================
 Name        : MC2.c
 Author      : Mohamed Samy
 Description : MC2 Code in Challenge
 ================================================================================================
 */

/*******************************************************************************
 *                    Include Library                                          *
 *******************************************************************************/

#include "uart.h"
#include "CONTROL_ECU.h"
#include "twi.h"
#include "external_eeprom.h"
#include "g_interrupt.h"
#include "timer0.h"
#include "timer1.h"
#include "dc_motor.h"
#include "gpio.h"
#include "Buzzer.h"
#include <util/delay.h>

/*******************************************************************************
 *                Global Arrays to store the passwords                         *
 *******************************************************************************/

uint8 g_Arrpass1[PASS_SIZE]={KEY_NOT_PRESSED , KEY_NOT_PRESSED , KEY_NOT_PRESSED , KEY_NOT_PRESSED , KEY_NOT_PRESSED};
uint8 g_Arrpass2[PASS_SIZE]={KEY_NOT_PRESSED , KEY_NOT_PRESSED , KEY_NOT_PRESSED , KEY_NOT_PRESSED , KEY_NOT_PRESSED};

/*******************************************************************************
 *                               Global variables                              *
 *******************************************************************************/
uint8 volatile g_Second =0;
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
TWI_ConfigType TWI_ConfigPtr =
{
		FirstAddress,
		BautRate_400K
};

/*******************************************************************************
 *                             Application                                     *
 *******************************************************************************/

int main(void)
{

/*******************************************************************************
 *                               LOCAL variables                               *
 *******************************************************************************/

	uint8 Local_PassCheck =0;
	uint8 Return_checkPass =0;
	uint8 Local_TakeOperation = 0;
	uint8 Step1_Flag =1;

	/* Enable the Global Interrupt */
	Global_interruptEnable();
	/* Initialize the UART driver with Baud-rate = 9600 bits/sec */
	UART_init(&uart);
	/*Call initialization function of TWI driver*/
	TWI_init(&TWI_ConfigPtr);
	/* Initialize the dc motor Driver */
	DcMotor_Init();
	/*Call initialization function of TIMER1 driver*/
	Timer1_init(&Timer1_configCTCMode);
	/*CallBack Function  of Timer1  of CTC Mode*/
	Timer1_setCallBack(CTC_Mode,&A_DLSS_VoidGenerateSecond);
	/* Initialize the Buzzer Driver */
	Buzzer_init();

	while(1)
    {

/*********************************************************************************************************/
/*************************************** STEP 1***********************************************************/
/*********************************************************************************************************/
		while(Step1_Flag)
		{
			Local_PassCheck =0;
			/*Call A_DLSS_VoidTakePassword_1 to receive the pass for first time */
			A_DLSS_VoidRecivePassword_1();
			/*Call A_DLSS_VoidTakePassword_1 to receive the pass for second time */
			A_DLSS_VoidRecivePassword_2();
			/* check the two pass matched or not */
			Local_PassCheck = A_DLSS_VoidCheckPassword(g_Arrpass1,g_Arrpass2,PASS_SIZE);
			_delay_ms(10);
			/* send the check password flag  */
			UART_sendByte(Local_PassCheck);
			_delay_ms(50);
			/* if the two passwords matched */
			if(Local_PassCheck == PASS_MATCHED)
			{
				uint8 count =0;
				/* store pass in EEPROM*/
				while(count < PASS_SIZE)
				{
					EEPROM_writeByte(PASSWORD_ADDRESS_LOCATION_1 + count ,g_Arrpass2[count]);
					_delay_ms(500);
					count++;
				}
				/* Go out from this while loop for first step */
				Step1_Flag =0;
			}
		}
/*********************************************************************************************************/
/*********************************** END OF STEP 1 *******************************************************/
/*********************************************************************************************************/

/*********************************************************************************************************/
/*************************************** STEP 2***********************************************************/
/*********************************************************************************************************/
			/* receive  the operation for open door or change pass*/
				Local_TakeOperation = UART_recieveByte();
				_delay_ms(20);

/*********************************************************************************************************/
/*********************************** END OF STEP 2 *******************************************************/
/*********************************************************************************************************/

/*********************************************************************************************************/
/*************************************** STEP 3***********************************************************/
/*********************************************************************************************************/
				/*if  the operation is open door */
				if(Local_TakeOperation == OPEN_DOOR)
				{

					Return_checkPass =0;
					Local_TakeOperation=0;
					/* TAKE PASS FROM THE USER*/
					A_DLSS_VoidStep_3_RecievePassword();
					/* READ THE PASS STORED INTO EEPROM AND SAVE IT INTO g_Arrpass2*/
					uint8 count =0;
					while(count < PASS_SIZE)
					{
						EEPROM_readByte(PASSWORD_ADDRESS_LOCATION_1+count , g_Arrpass2[count]);
						_delay_ms(50);
						count++;
					}
					/*check pass  stored into g_Arrpass2  with the pass stored into g_Arrpass1*/
					Return_checkPass = A_DLSS_VoidCheckPassword(g_Arrpass1,g_Arrpass2,PASS_SIZE);
					/* Return_checkPass should return to HMI_ECU by UART*/
					UART_sendByte(Return_checkPass);
					_delay_ms(50);
					/* receive byte after the two pass matched to start open motor*/
					Local_TakeOperation = UART_recieveByte();
					_delay_ms(30);
					if(Local_TakeOperation == TURN_MOTOR_ON)
					{
						/* Motor State */
						A_DLSS_VoidStep_3_MotorState();
					}
					else
					{

					}

				}
/*********************************************************************************************************/
/*********************************** END OF STEP 3 *******************************************************/
/*********************************************************************************************************/

/*********************************************************************************************************/
/***************************************  STEP 4  ********************************************************/
/*********************************************************************************************************/
				else if(Local_TakeOperation == CHANGE_PASS)
				{
					Return_checkPass=0;
					Local_TakeOperation=0;
					/* TAKE PASS FROM THE USER*/
					A_DLSS_VoidStep_4_RecievePassword();
					/* READ THE PASS STORED INTO EEPROM AND SAVE IT INTO g_Arrpass2*/
					uint8 count =0;
					while(count < PASS_SIZE)
					{
						EEPROM_readByte(PASSWORD_ADDRESS_LOCATION_1+count , g_Arrpass2[count]);
						_delay_ms(50);
						count++;
					}
					/*check pass  stored into g_Arrpass2  with the pass stored into g_Arrpass1*/
					Return_checkPass = A_DLSS_VoidCheckPassword(g_Arrpass1,g_Arrpass2,PASS_SIZE);
					/* Return_checkPass should return to HMI_ECU by UART*/
					UART_sendByte(Return_checkPass);
					_delay_ms(50);
					/* receive byte after the two pass matched to start change pass*/
					Local_TakeOperation = UART_recieveByte();
					if(Local_TakeOperation == REPEAT_STEP_1)
					{
						/* return to step 1 to take two pass again*/
						Step1_Flag =1;
					}
					else
					{

					}
				}
/*********************************************************************************************************/
/*********************************** END OF STEP 4 *******************************************************/
/*********************************************************************************************************/

/*********************************************************************************************************/
/***************************************  STEP 5  ********************************************************/
/*********************************************************************************************************/
				else if(Local_TakeOperation == BLOCK_SYSTEM)
				{
					/* the system will still here until the HMI ECU send UNBLOCK_SYSTEM command*/
					while(Local_TakeOperation == BLOCK_SYSTEM)
					{
						/*Turn on Buzzer*/
						Buzzer_on();
						Local_TakeOperation = UART_recieveByte();

					}
					if(Local_TakeOperation == UNBLOCK_SYSTEM)
					{
						/*Turn off Buzzer*/
						Buzzer_off();

					}
				}
/*********************************************************************************************************/
/*********************************** END OF STEP 5 *******************************************************/
/*********************************************************************************************************/

    }/*end of main while loop*/

}/*end of Application*/

void A_DLSS_VoidRecivePassword_1(void)
{
	uint8 Copy_u8_counter =0;
	while(Copy_u8_counter < PASS_SIZE)
	{
		g_Arrpass1[Copy_u8_counter] = UART_recieveByte();
		_delay_ms(50);
		Copy_u8_counter++;
	}

}


void A_DLSS_VoidRecivePassword_2(void)
{
	uint8 Copy_u8_counter =0;
	while(Copy_u8_counter < PASS_SIZE)
	{
		g_Arrpass2[Copy_u8_counter] = UART_recieveByte();
		_delay_ms(50);
		Copy_u8_counter++;
	}

}

uint8 A_DLSS_VoidCheckPassword(uint8 *Copy_Au8pass1 ,uint8 *Copy_Au8pass2 , uint8 Arr_Size)

{
	uint8 Check_Flag = PASS_MATCHED;

	for(int i = 0 ; i < Arr_Size ; i++)
	{
		if(g_Arrpass1[i] != g_Arrpass2[i])
		{
			Check_Flag = PASS_NOT_MATCHED;
			break;
		}

	}
	return Check_Flag;
}

void A_DLSS_VoidStep_3_RecievePassword(void)
{
	uint8 Copy_u8_counter =0;
	while(Copy_u8_counter < PASS_SIZE)
	{
		g_Arrpass1[Copy_u8_counter] = UART_recieveByte();
		_delay_ms(50);
	    Copy_u8_counter++;
	}

}

void A_DLSS_VoidStep_3_MotorState(void)
{
	Timer1_init(&Timer1_configCTCMode);
	g_Second =0;

	while(g_Second < TIME_OF_DOOR_UNLOOKED)
	{
		DcMotor_Rotate(RUN_MOTOR_CW,MOTOR_FULL_SPEED);
	}

	g_Second =0;

	while(g_Second < TIME_OF_MOTOR_HOLD)
	{
		DcMotor_Rotate(STOP_MOTOR,0);
	}
	g_Second=0;

	while(g_Second < TIME_OF_DOOR_LOOKED)
	{
		DcMotor_Rotate(RUN_MOTOR_A_CW,MOTOR_FULL_SPEED);
	}


	g_Second=0;
	Timer1_deInit();
	DcMotor_Rotate(STOP_MOTOR,0);

}
void A_DLSS_VoidGenerateSecond(void)
{
	g_Second++;

}

void A_DLSS_VoidStep_4_RecievePassword(void)
{

	uint8 Copy_u8_counter =0;
	while(Copy_u8_counter < PASS_SIZE)
	{
		g_Arrpass1[Copy_u8_counter] = UART_recieveByte();
		_delay_ms(50);
		Copy_u8_counter++;
	}

}
