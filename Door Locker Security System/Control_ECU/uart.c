/*
 ===================================================================================================
 Module      : UART
 File Name   : UART.c
 Author      : Mohamed Samy
 Description : Source file for the UART AVR driver
 ===================================================================================================
 */
#include <avr/io.h>
#include "uart.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr)
{
	uint16 ubrr_value = 0;
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);

	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
		UCSRB =0;
		SET_BIT(UCSRB,RXEN);
		SET_BIT(UCSRB,TXEN);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * parity bit set from the user
	 * stop bit set from the user
	 * bit data mode  set from the user
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC=0;
    /* URSEL   = 1 The URSEL must be one when writing the UCSRC*/
	SET_BIT(UCSRC,URSEL);
	/*Select parity bit*/
	UCSRC = (UCSRC & UART_PARITYBIT_MASK) | ( (Config_Ptr->parity) << 4 );
	/*Select Stop Bit*/
	UCSRC = (UCSRC & UART_STOPBIT_MASK) | ( (Config_Ptr->stop_bit) << 3 );

	/* Character Size */
	if((Config_Ptr->bit_data) == NINE_BIT)
	{
		UCSRC = (UCSRC & UART_BitData_MASK) | ( (Config_Ptr->bit_data) << 1 );
		SET_BIT(UCSRB,UCSZ2);
	}else
	{

		UCSRC = (UCSRC & UART_BitData_MASK) | ( (Config_Ptr->bit_data) << 1 );
		CLEAR_BIT(UCSRB,UCSZ2);
	}

	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / ( (Config_Ptr->baud_rate) * 8UL))) - 1);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;

}
void UART_init1(uint32 baud_rate)
{
	uint16 ubrr_value = 0;

	/* U2X = 1 for double transmission speed */
		UCSRA = (1<<U2X);
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
		UCSRB =0;
		SET_BIT(UCSRB,RXEN);
		SET_BIT(UCSRB,TXEN);
	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
		UCSRC=0;
		SET_BIT(UCSRC,URSEL);
		/* Character Size --> 8*/
		SET_BIT(UCSRC,UCSZ0);
		SET_BIT(UCSRC,UCSZ1);

	/* Calculate the UBRR register value */
	ubrr_value = (uint16)(((F_CPU / (baud_rate * 8UL))) - 1);
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = ubrr_value>>8;
	UBRRL = ubrr_value;

}
/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 copy_dataByte)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	{}
	/* Put data into buffer, sends the data */
	UDR = copy_dataByte;
}
/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void)
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	{}
	/* Get and return received data from buffer */
	return UDR;
}
/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *StrData)
{
	uint8 i =0;
	/* Send the whole string */
	while(StrData[i] != '\0')
	{
		UART_sendByte(StrData[i]);
		i++;
	}
}
/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_recieveString(uint8 *StrData)
{
	uint8 i =0;
	StrData[i]= UART_recieveByte();
	if(StrData[i] != '#')
	{
		i++;
		StrData[i]= UART_recieveByte();
	}
	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
	StrData[i] = '\0';
}
