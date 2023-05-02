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


typedef struct
{
	uint16_t	USART_Mode;			//This bit enables the transmitter or This bit enables the receiver
	// this parameter must be based on @ref USART_Mode_define
	uint32_t	BaudRate;			//specifies Baud rate value
	// this parameter must be based on @ref USART_BaudRate_define

	uint16_t	Payload_Length;		// Specifies the number of data bits transmitted or received in a frame
									// This parameters must be set based on @Ref UART_Payload_Length_define

	uint16_t	Parity;				// Specifies the parity mode
									// This parameters must be set based on @Ref UART_Parity_define

	uint16_t	StopBits;			// Specifies the number of stop bits
									// This parameters must be set based on @Ref UART_StopBits_define

	uint16_t	HwFlowCtl;			// Specifies whether the hardware flow control mode is enabled or disabled
									// This parameters must be set based on @Ref UART_HwFlowCtl_define

	uint16_t	IRQ_Enable;			// enable or disable the interrupt
									// This parameters must be set based on @Ref UART_IRQ_define

	void (*P_IRQ_CallBack)(void);	// Set the C Function() which will be called once the IRQ Happen
}USART_Config_t;

/* ================================================================ */
/* =============== Macros Configuration References ================ */
/* ================================================================ */

// @Ref UART_Mode_define
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
#define UART_Mode_RX				(uint32_t)(1<<2)	// bit 2 RE: transmitter is enabled
#define UART_Mode_TX				(uint32_t)(1<<3)	// bit 3 TE: transmitter is enabled
#define UART_Mode_TX_RX				(uint32_t)(1<<3 | 1<<2)

// @Ref UART_BaudRate_define
#define UART_BaudRate_2400			2400
#define UART_BaudRate_9600			9600
#define UART_BaudRate_19200			19200
#define UART_BaudRate_57600			57600
#define UART_BaudRate_115200		115200
#define UART_BaudRate_230400		230400
#define UART_BaudRate_460800		460800
#define UART_BaudRate_921600		921600
#define UART_BaudRate_225000		2250000
#define UART_BaudRate_4500000		4500000

// @Ref UART_Payload_Legth_define
/*Bit 12 M: Word length
This bit determines the word length. It is set or cleared by software.
0: 1 Start bit, 8 Data bits, n Stop bit
1: 1 Start bit, 9 Data bits, n Stop bit
Note: The M bit must not be modified during a data transfer (both transmission and reception)*/
#define UART_Payload_Length_8B		(uint32_t)(0)
#define UART_Payload_Length_9B		(uint32_t)(1<<12) 	// Bit 12 M: Word length

// @Ref UART_Parity_define
/*Bit 9 PS: Parity selection
This bit selects the odd or even parity when the parity generation/detection is enabled (PCE
bit set). It is set and cleared by software. The parity will be selected after the current byte.
0: Even parity
1: Odd parity*/
#define UART_Parity_NONE			(uint32_t)(0)
#define UART_Parity_EVEN			((uint32_t)(1<<10)) 		// Bit 10 PCE: Parity control enable
#define UART_Parity_ODD				((uint32_t)(1<<10 | 1<<9)) 	// Bit 9 PS: Parity selection

// @Ref UART_StopBits_define
/*Bits 13:12 STOP: STOP bits
These bits are used for programming the stop bits.
00: 1 Stop bit
01: 0.5 Stop bit
10: 2 Stop bits
11: 1.5 Stop bit
The 0.5 Stop bit and 1.5 Stop bit are not available for UART4 & UART5*/
#define UART_StopBits_half			(uint32_t)(1<<12) 	// Bits 13:12 STOP: STOP bits
#define UART_StopBits_1				(uint32_t)(0)	  	// Bits 13:12 STOP: STOP bits
#define UART_StopBits_1_half		(uint32_t)(3<<12)	// Bits 13:12 STOP: STOP bits
#define UART_StopBits_2				(uint32_t)(2<<12)	// Bits 13:12 STOP: STOP bits

// @Ref UART_HwFlowCtl_define
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
#define UART_HwFlowCtl_NONE			(uint32_t)(0)
#define UART_HwFlowCtl_RTS			(uint32_t)(1<<8)	// Bit 8 RTSE: RTS enable
#define UART_HwFlowCtl_CTS			(uint32_t)(1<<9) 	// Bit 9 CTSE: CTS enable
#define UART_HwFlowCtl_RTS_CTS		(uint32_t)(1<<8 | 1<<9)

// @Ref UART_IRQ_Enable_define
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
#define UART_IRQ_Enable_NONE		(uint32_t)(0)
#define UART_IRQ_Enable_TXE			(uint32_t)(1<<7)	// Bit 7 TXEIE: TXE interrupt enable // Transmit data register empty
#define UART_IRQ_Enable_TC			(uint32_t)(1<<6)	// Bit 6 TCIE: Transmission complete interrupt enable // Transmission complete
#define UART_IRQ_Enable_RXNEIE		(uint32_t)(1<<5)	// Bit 5 RXNEIE: RXNE interrupt enable // Received data ready to be read
#define UART_IRQ_Enable_PE			(uint32_t)(1<<8)	// Bit 8 PEIE: PE interrupt enable // Parity error

enum Polling_mechanism
{
	USART_disable,
	USART_enable
};



/* ================================================================ */
/* ===================== BaudRate Calculation ===================== */
/* ================================================================ */

// USARTDIV = fclk / (16 * Baudrate)
// USARTDIV_MUL100 = uint32((100 * fclk ) / ( 16 * Baudrate) == (25 * fclk) / (4 * Baudrate))
// DIV_Mantissa_MUL100 = Integer Part (USARTDIV) * 100
// DIV_Mantissa = Integer Part (USARTDIV)
// DIV_Fraction = (( USARTDIV_MUL100 - DIV_Mantissa_MUL100 ) *16) / 100

#define USARTDIV(_PCLK_, _BAUD_)				(uint32_t) (_PCLK_/(16 * _BAUD_ ))
#define USARTDIV_MUL100(_PCLK_, _BAUD_)			(uint32_t) ((25 * _PCLK_ ) / (4 * _BAUD_))
#define Mantissa_MUL100(_PCLK_, _BAUD_)			(uint32_t) (USARTDIV(_PCLK_, _BAUD_) * 100)
#define Mantissa(_PCLK_, _BAUD_)				(uint32_t) (USARTDIV(_PCLK_, _BAUD_ ))
#define DIV_Fraction(_PCLK_, _BAUD_)			(uint32_t) (((USARTDIV_MUL100(_PCLK_, _BAUD_) - Mantissa_MUL100(_PCLK_, _BAUD_) )*16) / 100)
#define UART_BRR_Register(_PCLK_, _BAUD_)		(( Mantissa (_PCLK_, _BAUD_ ) ) << 4 )|((DIV_Fraction(_PCLK_, _BAUD_)) & 0xF )
/*
 *============================================
 *============================================
 * APIs Supported by "MCAL GPIO DRIVER"
 *============================================
 *============================================
 *
 */
void MCAL_UART_Init(USART_t* USARTx, USART_Config_t* UART_Config);
void MCAL_UART_DeInit(USART_t* USARTx);

void MCAL_UART_GPIO_Set_Pins(USART_t* USARTx);

void MCAL_UART_SendData(USART_t* USARTx, uint16_t* pTxBuffer, enum Polling_mechanism PollingEn);
void MCAL_UART_ReceiveData(USART_t* USARTx, uint16_t* pRxBuffer, enum Polling_mechanism PollingEn);

void MCAL_UART_WAIT_TC(USART_t* USARTx);

// TODO MCAL_USART_LIN_Init()	// LIN
// TODO MCAL_USART_Init()		// Synchronous
// TODO MCAL_USART_DMA_Init()	// Multi-buffer communication

#endif /* INC_STM32_F103C6_USART_H_ */
