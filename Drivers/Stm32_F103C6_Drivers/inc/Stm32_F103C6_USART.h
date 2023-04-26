/*
 * Stm32_F103C6_USART.h
 *
 *  Created on: Apr 26, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_STM32_F103C6_USART_H_
#define INC_STM32_F103C6_USART_H_


//-----------------------------
//INCLUDES
//-----------------------------
#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO.h"
#include "Stm32_F103C6_RCC.h"
//-----------------------------
//MACROS CONFIGURATION REFERENCES
//-----------------------------


//config Structures
typedef struct {
	uint8_t	Baud_Rate;//specifies Baud rate value
	// this parameter must be based on @ref USART_BaudRate_define

	uint8_t Stop_Bits;//These bits are used for programming the stop bits
	// this parameter must be based on @ref USART_StopBits_define

	uint8_t	USART_Mode;//This bit enables the transmitter or This bit enables the receiver
	// this parameter must be based on @ref USART_Mode_define

	uint8_t	Word_Length;//This bit determines the word length.
	// this parameter must be based on @ref USART_WordLength_define

	uint8_t Parity;//This bit selects the odd or even parity when the parity generation/detection is enabled
	// this parameter must be based on @ref USART_Parity_define

	uint8_t IRQ_Enable;// USART interrupt which will generate
	// this parameter must be based on @ref USART_IRQ_define

	uint8_t HW_FLW_CTRL;// control the serial data flow between two devices by using the CTS input and the RTS output
	// this parameter must be based on @ref USART_HW_FLW_define
	void(*IRQ_HANDLER)(void); // setting the C function which will be call when Interrupt occur
}USART_CONFIG_t;

//-----------------------------
//MACROS CONFIGURATION REFERENCES
//-----------------------------

/*@ref USART_BaudRate_define
 */
#define USART_BaudRate_2400                   2400
#define USART_BaudRate_9600                   9600
#define USART_BaudRate_19200                  19200
#define USART_BaudRate_57600                  57600
#define USART_BaudRate_115200                 115200
#define USART_BaudRate_230400                 230400
#define USART_BaudRate_460800                 460800
#define USART_BaudRate_921600                 921600
#define USART_BaudRate_2250000                2250000
#define USART_BaudRate_4500000                4500000

/*@ref USART_StopBits_define
 */

/*Bits 13:12 STOP: STOP bits
These bits are used for programming the stop bits.
00: 1 Stop bit
01: 0.5 Stop bit
10: 2 Stop bits
11: 1.5 Stop bit
The 0.5 Stop bit and 1.5 Stop bit are not available for UART4 & UART5*/

#define USART_StopBits_1B                   (uint32_t)(0)
#define USART_StopBits_halfB                (uint32_t)(1<<12)
#define USART_StopBits_2B                   (uint32_t)(2<<12)
#define USART_StopBits_1halfB               (uint32_t)(3<<12)

/*@ref USART_Mode_define
 */

/*Bit 3 TE: Transmitter enable
This bit enables the transmitter. It is set and cleared by software.
0: Transmitter is disabled
1: Transmitter is enabled
Note: 1: During transmission, a “0” pulse on the TE bit (“0” followed by “1”) sends a preamble
(idle line) after the current word, except in Smartcard mode.
2: When TE is set there is a 1 bit-time delay before the transmission starts.

Bit 2 RE: Receiver enable
This bit enables the receiver. It is set and cleared by software.
0: Receiver is disabled
1: Receiver is enabled and begins searching for a start bit*/

#define USART_Mode_Transmitter           	(uint32_t)(1<<3)
#define USART_Mode_Receiver           	    (uint32_t)(1<<2)
#define USART_Mode_Transmitter_Receiver     (uint32_t)((1<<3 | 1<<2))



/*@ref USART_WordLength_define
 */
/*Bit 12 M: Word length
This bit determines the word length. It is set or cleared by software.
0: 1 Start bit, 8 Data bits, n Stop bit
1: 1 Start bit, 9 Data bits, n Stop bit
Note: The M bit must not be modified during a data transfer (both transmission and reception)*/
#define USART_WordLength_8BIT           	(uint32_t)0
#define USART_WordLength_9BIT           	(uint32_t)(1<<12)


/*@ref USART_Parity_define
 */
/*Bit 9 PS: Parity selection
This bit selects the odd or even parity when the parity generation/detection is enabled (PCE
bit set). It is set and cleared by software. The parity will be selected after the current byte.
0: Even parity
1: Odd parity*/
#define USART_Parity_NONE                    (uint32_t)(0)
#define USART_Parity_even                    ((uint32_t)1<<10)
#define USART_Parity_odd                     ((uint32_t)(1<<10 | 1<<9))



/*@ref USART_IRQ_define
 */
/*
===> Bit 8 PEIE: PE interrupt enable
This bit is set and cleared by software.
0: Interrupt is inhibited
1: A USART interrupt is generated whenever PE=1 in the USART_SR register

==>Bit 7 TXEIE: TXE interrupt enable
This bit is set and cleared by software.
0: Interrupt is inhibited
1: A USART interrupt is generated whenever TXE=1 in the USART_SR register

==>Bit 6 TCIE: Transmission complete interrupt enable
This bit is set and cleared by software.
0: Interrupt is inhibited
1: A USART interrupt is generated whenever TC=1 in the USART_SR register

==>Bit 5 RXNEIE: RXNE interrupt enable
This bit is set and cleared by software.
0: Interrupt is inhibited
1: A USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register
*/
#define USART_IRQ_NONE						(uint32_t)(0)
#define USART_IRQ_RXNE						(uint32_t)(1<<5)
#define USART_IRQ_TC						(uint32_t)(1<<6)
#define USART_IRQ_TXE						(uint32_t)(1<<7)
#define USART_IRQ_PE						(uint32_t)(1<<8)


/*@ref USART_HW_FLW_define
 */
/*
==>Bit 9 CTSE: CTS enable
0: CTS hardware flow control disabled
1: CTS mode enabled, data is only transmitted when the CTS input is asserted (tied to 0). If
the CTS input is deasserted while a data is being transmitted, then the transmission is
completed before stopping. If a data is written into the data register while CTS is deasserted,
the transmission is postponed until CTS is asserted.
This bit is not available for UART4 & UART5.

==>Bit 8 RTSE: RTS enable
0: RTS hardware flow control disabled
1: RTS interrupt enabled, data is only requested when there is space in the receive buffer.
The transmission of data is expected to cease after the current character has been
transmitted. The RTS output is asserted (tied to 0) when a data can be received.
This bit is not available for UART4 & UART5
*/
#define USART_HW_FLW_NONE						(uint32_t)(0)
#define USART_HW_FLW_CTS						(uint32_t)(1<<9)
#define USART_HW_FLW_RTS						(uint32_t)(1<<8)
#define USART_HW_FLW_CTS_RTS					((uint32_t)(1<<8 | 1<<9))


/*
BaudRate Calculation

==>USARTDIV = fclk / (16 * Baudrate)

==>USARTDIV_MUL100 = uint32((100 *fclk ) / (16 * Baudrate) == (25 *fclk ) / (4* Baudrate) )

==>DIV_Mantissa_MUL100 = Integer Part (USARTDIV  )  * 100

==>DIV_Mantissa = Integer Part (USARTDIV  )

==>DIV_Fraction = (( USARTDIV_MUL100  - DIV_Mantissa_MUL100  ) * 16 ) / 100
*/
#define USARTDIV(_PCLK_, _BAUD_)							(uint32_t) (_PCLK_/(16 * _BAUD_ ))
#define USARTDIV_MUL100(_PCLK_, _BAUD_)						(uint32_t) ( (25 * _PCLK_ ) / (4  * _BAUD_ ))
#define Mantissa_MUL100(_PCLK_, _BAUD_)						(uint32_t) (USARTDIV(_PCLK_, _BAUD_) * 100)
#define Mantissa(_PCLK_, _BAUD_)							(uint32_t) (USARTDIV(_PCLK_, _BAUD_) )
#define DIV_Fraction(_PCLK_, _BAUD_)						(uint32_t) (((USARTDIV_MUL100(_PCLK_, _BAUD_) -  Mantissa_MUL100(_PCLK_, _BAUD_) ) * 16 ) / 100 )
#define UART_BRR_Register(_PCLK_, _BAUD_)					(( Mantissa (_PCLK_, _BAUD_) ) <<4 )|( (DIV_Fraction(_PCLK_, _BAUD_)) & 0xF )


//polling
enum Polling_mechism{
	enable ,
	disable
};


/*
 *============================================
 *============================================
 * APIs Supported by "MCAL GPIO DRIVER"
 *============================================
 *============================================
 *
 */

void MCAL_USART_Init(USART_t *USARTx , USART_CONFIG_t *USARTx_config);
void MCAL_USART_DeInit(USART_t *USARTx);
void MCAL_USART_Transmit(USART_t * USARTx , uint16_t *Buffer,enum Polling_mechism PollingEn);
void MCAL_USART_Receive(USART_t * USARTx , uint16_t *Buffer,enum Polling_mechism PollingEn);
void MCAL_USART_TransmitComplete(USART_t *USARTx);
void MCAL_USART_Transmit_String(USART_t* USARTx , uint8_t *Buffer ,enum Polling_mechism PollingEn);

void MCAL_UART_GPIO_Set_Pins (USART_t *USARTx);


#endif /* INC_STM32_F103C6_USART_H_ */
