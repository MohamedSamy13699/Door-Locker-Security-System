/*
 ===================================================================================================
 Module      : BUZZER
 File Name   : Buzzer.h
 Author      : Mohamed Samy
 Description : Header file for the BUZZER  driver
 ===================================================================================================
 */
#ifndef BUZZER_H_
#define BUZZER_H_

#include "gpio.h"

#define BUZZER_PORT		PORTB_ID
#define BUZZER_PIN		PIN2_ID
/*
 * Description
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * Turn off the buzzer through the GPIO.
 * Inputs: None
 * Return: None
 */
void Buzzer_init(void);
/*
 * Description
 * Function to enable the Buzzer through the GPIO
 * Inputs: None
 * Return: None
 */
void Buzzer_on(void);
/*
 * Description
 * Function to disable the Buzzer through the GPIO
 * Inputs: None
 * Return: None
 */
void Buzzer_off(void);

#endif /* BUZZER_H_ */
