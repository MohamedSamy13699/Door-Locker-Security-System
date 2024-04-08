/*
 ===================================================================================================
 Module      : UART
 File Name   : uart.h
 Author      : Mohamed Samy
 Description : Header file for the UART AVR driver
 ===================================================================================================
 */
#ifndef UART_H_
#define UART_H_

#include "std_types.h"

#define UART_BitData_MASK	0xF9
#define UART_STOPBIT_MASK	0xF7
#define UART_PARITYBIT_MASK	0xCF
/*******************************************************************************
 *                       UART Configurations                                     *
 *******************************************************************************/
typedef enum
{
	FIVE_BIT,
	SIX_BIT,
	SEVEN_BIT,
	EIGHT_BIT,
	NINE_BIT=7
}UART_BitData;

typedef enum
{
	Disabled,
	Even_Parity=2,
	Odd_Parity
}UART_Parity;

typedef enum
{
	ONE_STOP_BIT,
	TWO_STOP_BIT
}UART_StopBit;

typedef enum
{
	BaudRate_9600=9600,
	BaudRate_115200=115200,

}UART_BaudRate;

typedef struct{
	UART_BitData  bit_data;
	UART_Parity   parity;
	UART_StopBit  stop_bit;
	UART_BaudRate baud_rate;
}UART_ConfigType;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init1(uint32 baud_rate);
void UART_init(const UART_ConfigType * Config_Ptr);
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 copy_dataByte);
/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);
/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *StrData);
/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_recieveString(uint8 *StrData);


#endif /* UART_H_ */
