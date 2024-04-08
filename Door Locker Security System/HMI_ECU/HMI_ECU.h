/*
 ===================================================================================================
 Module      : HMI_ECU
 File Name   : HMI_ECU.h
 Author      : Mohamed Samy
 Description : Header  file for the HMI_ECU
 ===================================================================================================
 */

#ifndef HMI_ECU_H_
#define HMI_ECU_H_
/*******************************************************************************
 *                    Include Library                                          *
 *******************************************************************************/

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define KEY_NOT_PRESSED              0xFF
#define PASS_MATCHED	             0x90
#define UNBLOCK_SYSTEM	             0x80
#define BLOCK_SYSTEM	             0x70
#define TURN_MOTOR_ON				 0x60
#define REPEAT_STEP_1				 0x30
#define PASS_NOT_MATCHED	         0x10
#define NUMBER_OF_TICK	             15625  /* to generate 1 sec with pres 256 and Fcpu 8Mhz
 	 	 	 	 	 	 	 	 	 	 	 	 the value of number of tick should be 31250
 	 	 	 	 	 	 	 	 	 	 	 	 but to simulate in proteus should be 15625 */
#define PASS_SIZE	                   5
#define MAX_TRIES_ENTER_PASS  	       3
#define OPEN_DOOR	                  '+'
#define CHANGE_PASS	                  '-'
#define TIME_OF_DOOR_UNLOOKED  	       15
#define TIME_OF_MOTOR_HOLD	           3
#define TIME_OF_DOOR_LOOKED	           15

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void A_DLSS_VoidTakePassword_1(void);
void A_DLSS_VoidTakePassword_2(void);
uint8 A_DLSS_u8Step2TakeOperation(void);
void A_DLSS_VoidStep3EnterPass(void);
void A_DLSS_VoidStep_3_MotorState(void);
void A_DLSS_VoidGenerateSecond(void);
void A_DLSS_VoidStep4ChangePass(void);
void  A_DLSS_VoidStep5ErrorMassage(void);
void A_DLSS_VoidGenerateMinute(void);
#endif /* HMI_ECU_H_ */
