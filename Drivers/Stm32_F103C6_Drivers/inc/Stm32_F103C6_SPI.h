/*
 * Stm32_F103C6_SPI.h
 *
 *  Created on: Apr 27, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_STM32_F103C6_SPI_H_
#define INC_STM32_F103C6_SPI_H_

//-----------------------------
//INCLUDES
//-----------------------------
#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO.h"




struct S_IRQ_SRC
{
	uint8_t		TXE:1;						// TX Buffer Empty Interrupt
	uint8_t		RXNE:1;						// RX Buffer Not Empty Interrupt
	uint8_t		ERRI:1;						// Error Interrupt
	uint8_t		Reserved:5;
};

typedef struct
{
	uint16_t 	SPI_Mode;					// specifies SPI MODE Master/Slave
											// This parameter must be set based on @ref SPI_MODE_DEFINE

	uint16_t	Communication_Mode;			// specifies SPI Communication mode
											// This parameter must be set based on @ref COMMUNICATION_MODE_DEFINE

	uint16_t	Frame_Format;				// specifies Data MSB transmit first or LSB transmit first
											// This parameter must be set based on @ref FRAME_FORMAT_DEFINE

	uint16_t	Frame_Size;					// specifies Data frame size 8-bit or 16_bit
											// This parameter must be set based on @ref FRABaudRate_PrescalerME_SIZE_DEFINE

	uint16_t	CLK_Polarity;				// specifies SPI clock Polarity idle on low or idle on high
											// This parameter must be set based on @ref CLK_POLARITY_DEFINE

	uint16_t	CLK_Phase;					// specifies SPI clock Phase,first clock capture or second clock capture
											// This parameter must be set based on @ref CLK_PHASE_DEFINE

	uint16_t	NSS;						// specifies slave select management by hardware or by software
											// This parameter must be set based on @ref NSS_DEFINE

	uint16_t	BaudRate_Prescaler;		    // specifies SPI clock BaudRate Prescaler
											// This parameter must be set based on @ref BAUDERATE_PRESCALER_DEFINE

	uint16_t	IRQ_Enable;					// specifies SPI Interrupt Requests
											// This parameter must be set based on @ref IRQ_ENABLE_DEFINE

	void (* P_IRQ_CallBack)(struct S_IRQ_SRC IRQ_src);	//set the C Function which will be called once IRQ Happens


}SPI_Config_t;

//-----------------------------
//MACROS CONFIGURATION REFERENCES
//-----------------------------

// @ref SPI_MODE_DEFINE
/*
 * ===> Bit 2 MSTR: Master selection
0: Slave configuration
1: Master configuration*/

#define SPI_MODE_MASTER							(uint16_t) (1<<2) 	/* CR1.Bit 2 MSTR: Master selection -> 	1: Master configuration */
#define SPI_MODE_SLAVE							(uint16_t) (0) 		/* 										0: Slave configuration  */

// @ref COMMUNICATION_MODE_DEFINE
/*
====>Bit 15 BIDIMODE: Bidirectional data mode enable
0: 2-line unidirectional data mode selected
1: 1-line bidirectional data mode selected

===>Bit 14 BIDIOE: Output enable in bidirectional mode
This bit combined with the BIDImode bit selects the direction of transfer in bidirectional mode
0: Output disabled (receive-only mode)
1: Output enabled (transmit-only mode)

====>Bit 10 RXONLY: Receive only
This bit combined with the BIDImode bit selects the direction of transfer in 2-line
unidirectional mode. This bit is also useful in a multislave system in which this particular
slave is not accessed, the output from the accessed slave is not corrupted.
0: Full duplex (Transmit and receive)
1: Output disabled (Receive-only mode)*/

#define COMMUNICATION_MODE_1LINE_RX_ONLY		(uint16_t) (1<<15)				/* CR1.Bit 15 BIDIMODE: Bidirectional data mode enable -> 	1: 1-line bidirectional data mode selected */
#define COMMUNICATION_MODE_1LINE_TX_ONLY		(uint16_t) ((1<<15) | (1<<14))	/* CR1.Bit 15 BIDIMODE: Bidirectional data mode enable -> 	1: 1-line bidirectional data mode selected */
																				/* CR1. Bit 14 BIDIOE: Output enable in bidirectional mode -> 1: Output enabled (transmit-only mode) */
#define COMMUNICATION_MODE_2LINE_FULL_DUPLEX	(uint16_t) (0)					/* CR1.Bit 15 BIDIMODE: Bidirectional data mode enable -> 	0: 2-line unidirectional data mode selected */
#define COMMUNICATION_MODE_2LINE_RXE_ONLY		(uint16_t) (1<<10)				/* CR1.Bit 10 RXONLY: Receive only -> 	1: Output disabled (Receive-only mode) */

// @ref FRAME_FORMAT_DEFINE
/*Bit 7 LSBFIRST: Frame format
0: MSB transmitted first
1: LSB transmitted firs*/

#define FRAME_FORMAT_MSB_FIRST					(uint16_t) (0)		/* CR1.Bit 7 LSBFIRST: Frame format -> 	0: MSB transmitted first */
#define FRAME_FORMAT_LSB_FIRST					(uint16_t) (1<<7)	/* 									 	1: LSB transmitted first */


// @ref FRAME_SIZE_DEFINE
/*Bit 11 DFF: Data frame format
0: 8-bit data frame format is selected for transmission/reception
1: 16-bit data frame format is selected for transmission/reception*/

#define FRAME_SIZE_8BIT							(uint16_t) (0)		/* CR1.Bit 11 DFF: Data frame format -> 0: 8-bit data frame format is selected for transmission/reception */
#define FRAME_SIZE_16BIT						(uint16_t) (1<<11)	/* 										1: 16-bit data frame format is selected for transmission/reception */


// @ref CLK_POLARITY_DEFINE
/*Bit1 CPOL: Clock polarity
0: CK to 0 when idle
1: CK to 1 when idle*/

#define CLK_POLARITY_IDLE_LOW					(uint16_t) (0)		/* CR1.Bit1 CPOL: Clock polarity -> 0: CK to 0 when idle */
#define CLK_POLARITY_IDLE_HIGH					(uint16_t) (1<<1)	/* 									1: CK to 1 when idle */

// @ref CLK_PHASE_DEFINE
/*Bit 0 CPHA: Clock phase
0: The first clock transition is the first data capture edge
1: The second clock transition is the first data capture edge*/
#define CLK_PHASE_FIRST							(uint16_t) (0)		/* CR1.Bit 0 CPHA: Clock phase -> 0: The first clock transition is the first data capture edge */
#define CLK_PHASE_SECOND						(uint16_t) (1<<0)   /*								  1: The second clock transition is the first data capture edge */

// @ref NSS_DEFINE
/*
====>Bit 2 SSOE: SS output enable
	0: SS output is disabled in master mode and the cell can work in multimaster configuration
	1: SS output is enabled in master mode and when the cell is enabled. The cell cannot work
	in a multimaster environment.

=====>Bit 9 SSM: Software slave management
	When the SSM bit is set, the NSS pin input is replaced with the value from the SSI bit.
	0: Software slave management disabled
	1: Software slave management enabled
=====>Bit 8 SSI: Internal slave select
	This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the
	NSS pin and the IO value of the NSS pin is ignored.*/
#define NSS_HW_SLAVE							(uint16_t) (0)
#define NSS_HW_MASTER_SS_OUTPUT_ENABLED			(uint16_t) (1<<2)	/* Bit 2 SSOE: SS output enable -> 1: SS output is enabled in master mode and when the cell is enabled.
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	  	  	  	  	  	  	  	  The cell cannot work in a multimaster environment. */
#define NSS_HW_MASTER_SS_OUTPUT_DISABLED		(uint16_t) ~(1<<2)

/* Software (NSS is driven by Software for "Master or Slave" )*/
#define NSS_SW_SSI_SET							(uint16_t) ((1<<9) | (1<<8))
#define NSS_SW_SSI_RESET						(uint16_t) (1<<9)

// @ref BAUDERATE_PRESCALER_DEFINE
/*Baud rate control
000: fPCLK/2
001: fPCLK/4
010: fPCLK/8
011: fPCLK/16
100: fPCLK/32
101: fPCLK/64
110: fPCLK/128
111: fPCLK/256*/
#define BAUDERATE_PRESCALER_2					(uint16_t) (0)
#define BAUDERATE_PRESCALER_4					(uint16_t) (1<<3)
#define BAUDERATE_PRESCALER_8					(uint16_t) (2<<3)
#define BAUDERATE_PRESCALER_16					(uint16_t) (3<<3)
#define BAUDERATE_PRESCALER_32					(uint16_t) (4<<3)
#define BAUDERATE_PRESCALER_64					(uint16_t) (5<<3)
#define BAUDERATE_PRESCALER_128					(uint16_t) (6<<3)
#define BAUDERATE_PRESCALER_256					(uint16_t) (7<<3)

// @ref IRQ_ENABLE_DEFINE
/*
====>Bit 7 TXEIE: Tx buffer empty interrupt enable
	0: TXE interrupt masked
	1: TXE interrupt not masked. Used to generate an interrupt request when the TXE flag is set.
====>Bit 6 RXNEIE: RX buffer not empty interrupt enable
	0: RXNE interrupt masked
	1: RXNE interrupt not masked. Used to generate an interrupt request when the RXNE flag is
	set.
===>Bit 5 ERRIE: Error interrupt enable
	This bit controls the generation of an interrupt when an error condition occurs (CRCERR,
	OVR, MODF in SPI mode and UDR, OVR in I2S mode).
	0: Error interrupt is masked
	1: Error interrupt is enabled*/
#define IRQ_ENABLE_NONE							(uint16_t) (0)			// IRQ Disable
#define IRQ_ENABLE_TXEIE						(uint16_t) (1<<7)		// TX buffer empty interrupt enable
#define IRQ_ENABLE_RXNEIE						(uint16_t) (1<<6)		// RX buffer not empty interrupt enable
#define IRQ_ENABLE_ERRIE						(uint16_t) (1<<5)		// Error interrupt enable

enum Polling_Mechanism
{
	SPI_disable,
	SPI_enable
};

/*
 *============================================
 *============================================
 * APIs Supported by "MCAL SPI DRIVER"
 *============================================
 *============================================
 *
 */

void MCAL_SPI_Init(SPI_t *SPIx, SPI_Config_t *Config);
void MCAL_SPI_DeInit(SPI_t *SPIx);

void MCAL_SPI_GPIO_Set_Pins(SPI_t *SPIx);

void MCAL_SPI_Send_Data(SPI_t *SPIx, uint16_t *pTxBuffer, enum Polling_Mechanism Polling_En);
void MCAL_SPI_Recieve_Data(SPI_t *SPIx, uint16_t *pRxBuffer, enum Polling_Mechanism Polling_En);

void MCAL_SPI_TX_RX(SPI_t *SPIx, uint16_t *pTxBuffer, enum Polling_Mechanism Polling_En);

#endif /* INC_STM32_F103C6_SPI_H_ */
