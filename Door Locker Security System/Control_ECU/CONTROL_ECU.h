/*
 ===================================================================================================
 Module      : CONTROL_ECU
 File Name   : CONTROL_ECU.h
 Author      : Mohamed Samy
 Description : Header  file for the CONTROL_ECU
 ===================================================================================================
 */
#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define KEY_NOT_PRESSED 			 0xFF
#define PASS_MATCHED				 0x90
#define UNBLOCK_SYSTEM			   	 0x80
#define BLOCK_SYSTEM				 0x70
#define TURN_MOTOR_ON				 0x60
#define PASSWORD_ADDRESS_LOCATION_1  0x20
#define REPEAT_STEP_1				 0x30
#define PASS_NOT_MATCHED	         0x10
#define NUMBER_OF_TICK				 15625 /* to generate 1 sec with pres 256 and Fcpu 8Mhz
 	 	 	 	 	 	 	 	 	 	 	 	 the value of number of tick should be 31250
 	 	 	 	 	 	 	 	 	 	 	 	 but to simulate in proteus should be 15625 */

#define PASS_SIZE					   5
#define OPEN_DOOR					  '+'
#define CHANGE_PASS	 				  '-'
#define TIME_OF_DOOR_UNLOOKED	       15
#define TIME_OF_MOTOR_HOLD             3
#define TIME_OF_DOOR_LOOKED	           15
#define MOTOR_FULL_SPEED 	           100


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void A_DLSS_VoidRecivePassword_1(void);
void A_DLSS_VoidRecivePassword_2(void);
uint8 A_DLSS_VoidCheckPassword(uint8 *Copy_Au8pass1 ,uint8 *Copy_Au8pass2 , uint8 Arr_Size);
void A_DLSS_VoidStep_3_RecievePassword(void);
void A_DLSS_VoidStep_3_MotorState(void);
void A_DLSS_VoidGenerateSecond(void);
void A_DLSS_VoidStep_4_RecievePassword(void);
void  A_DLSS_VoidStep5ErrorMassage(void);


#endif /* CONTROL_ECU_H_ */
