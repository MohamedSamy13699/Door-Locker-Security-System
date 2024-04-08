/*
 ===================================================================================================
 Module      : BUZZER
 File Name   : Buzzer.c
 Author      : Mohamed Samy
 Description : Source file for the BUZZER  driver
 ===================================================================================================
 */
#include "std_types.h"
#include "common_macros.h"
#include "Buzzer.h"
#include "gpio.h"

/*
 * Description
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * Turn off the buzzer through the GPIO.
 * Inputs: None
 * Return: None
 */
void Buzzer_init(void)
{
	GPIO_setupPinDirection(BUZZER_PORT,BUZZER_PIN,PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}
/*
 * Description
 * Function to enable the Buzzer through the GPIO
 * Inputs: None
 * Return: None
 */
void Buzzer_on(void)
{
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_HIGH);
}
/*
 * Description
 * Function to disable the Buzzer through the GPIO
 * Inputs: None
 * Return: None
 */
void Buzzer_off(void)
{
	GPIO_writePin(BUZZER_PORT,BUZZER_PIN,LOGIC_LOW);
}
