/*
 * USART.c
 *
 *  Created on: Apr 26, 2023
 *      Author: Ahmed Adel Wafdy
 * @linked in	   : https://www.linkedin.com/in/ahmed-wafdy-094567242/
 */


#include "../../MCAL/inc/Stm32_F103C6_USART.h"

/* ================================================================ */
/* ======================== Public APIs =========================== */
/* ================================================================ */

USART_Config_t* Global_USART_Config[3] = {NULL,NULL,NULL};

/* ================================================================
 * @Fn				- MCAL_UART_Init
 * @brief			- Initialize UART Asynchronous  only
 * @param [in] 		- USARTx: where x can be (1..3 depending on device used)
 * @param [in] 		- UART_Config: All UART configuration
 * @retval 			- none
 * Note				- support for now Asynchronous mode & clock 8 MHz
 */
void MCAL_UART_Init(USART_t* USARTx, USART_Config_t* UART_Config)
{
	uint32_t pclk, BRR;

	/* 1. Enable the clock for given USART peripheral */
	if(USARTx == USART1)
	{
		RCC_USART1_CLK_EN();
		Global_USART_Config[0] = UART_Config;
	}
	else if(USARTx == USART2)
	{
		RCC_USART2_CLK_EN();
		Global_USART_Config[1] = UART_Config;
	}
	else if(USARTx == USART3)
	{
		RCC_USART3_CLK_EN();
		Global_USART_Config[2] = UART_Config;
	}
	/* 2. Enable USART Module */
	USARTx->CR1 |= 1<<13;							// bit 13 UE: USART enable

	/* 3. Enable USART TX/RX engines according to the USART_Mode configuration item */
	USARTx->CR1 |= UART_Config->USART_Mode; 		// USART_CR1 bit 3 TE: transmitter enable & bit 2 RE: receiver enable

	/* 4. Pay-load width */
	USARTx->CR1 |= UART_Config->Payload_Length;		// USARTx->CR1 bit 12 M: word length

	/* 5. configuration of parity control bit field */
	USARTx->CR1 |= UART_Config->Parity;				// USARTx->CR1 bit 10 PCE: Parity Control enable bit 9 PS: Parity selection

	/* 6. configuration the no. of stop bits */
	USARTx->CR2 |= UART_Config->StopBits;			// USART_CR2 bits 13:12 STOP: stop bits


	/* 7. USART HW Flow Control */
	USARTx->CR3 |= UART_Config->HwFlowCtl;			// USART_CR3 bit 9 CTSE: CTS enable bit 8 RTSE: RTS enable


	/* 8. configuration of BRR (baud-rate register) */
	if (USARTx == USART1)							// PCLK2 for USART1

	{
		pclk = MCAL_RCC_Get_PCLK2Freq();
	}
	else											// PCLK1 for USART2, 3

	{
		pclk = MCAL_RCC_Get_PCLK1Freq();
	}

	BRR = UART_BRR_Register(pclk, UART_Config->BaudRate);
	USARTx->BRR = BRR;

	/* 9. ENABLE / DISABLE Interrupt */
	if (UART_Config->IRQ_Enable != UART_IRQ_Enable_NONE)
	{
		USARTx->CR1 |= (UART_Config->IRQ_Enable);

		/* Enable NVIC for USARTx IRQ */
		if (USARTx == USART1)
			NVIC_IRQ37_USART1_EN();

		else if (USARTx == USART2)
			NVIC_IRQ38_USART2_EN();

		else if (USARTx == USART3)
			NVIC_IRQ39_USART3_EN();
	}
}

/* ================================================================
 * @Fn				- MCAL_UART_DeInit
 * @brief			- DEInit UART Asynchronous only
 * @param [in] 		- USARTx where x can be (1..3 depending on device used)
 * @param [in] 		- pTxBuffer buffer
 * @param [in] 		- Polling Enable or disable
 * @retval 			- none
 * Note				- Initialize UART first
 */
void MCAL_UART_DeInit(USART_t* USARTx)
{
	if (USARTx == USART1)
	{
		RCC_USART1_CLK_DI();
		NVIC_IRQ37_USART1_DI();
	}

	else if (USARTx == USART3)
	{
		RCC_USART2_CLK_DI();
		NVIC_IRQ38_USART2_DI();
	}

	else if (USARTx == USART2)
	{
		RCC_USART3_CLK_DI();
		NVIC_IRQ39_USART3_DI();
	}
}

/**===============================================
 * @Fn			- MCAL_USART_Transmit
 * @brief		- The transmitter can send data words of either 8 or 9 bits depending on the M bit status.
					When the transmit enable bit (TE) is set
 * @param[in]	- USARTx: Where x can be (1...3) to select the USART peripheral
 * @param[in]	- pTxBuffer: data buffer which will store in registers and transmit
 * @param[in]	- PollingEn: Polling Enable / Disable
 * @retval		- void ====> none
 * Note			- (Supported feature ASYNCH. Only) & Clock 8 MHZ S
 * 	First :
 * 	1. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	2. Program the M bit in USART_CR1 to define the word length.
	3. Program the number of stop bits in USART_CR2.
	4. Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to takeplace. Configure the DMA register as explained in multibuffer communication.
	5. Select the desired baud rate using the USART_BRR register.
	6. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
 * */
void MCAL_UART_SendData(USART_t* USARTx, uint16_t* pTxBuffer, enum Polling_mechanism PollingEn)
{
	//7. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this	for each data to be transmitted in case of single buffer.
		//8. After writing the last data into the USART_DR register, wait until TC=1. This indicates
		//that the transmission of the last frame is complete. This is required for instance when
		//the USART is disabled or enters the Halt mode to avoid corrupting the last
		//transmission
	if (PollingEn == USART_enable)
	{
		/*
		 * The TXE bit is set by hardware and it indicates:
		 * The data has been moved from TDR to the shift register and the data transmission has started.
		 * The TDR register is empty.
		 * The next data can be written in the USART_DR register without overwriting the previous data.
		 */
		while (!(USARTx->SR & 1<<7));
	}

	/* 2. Check the USART_WordLength item for 9-bit or 8-bit in a frame */

	/* When Transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register),
	 * the value written in the MSB (bit 7 or bit 8 depending on the data length) has no effect
	 * because it is replaced by the parity.
	 * When receiving with the parity enabled, the value read in the MSB bit is the received parity bit.
	 */
	if (USARTx == USART1)
	{
		if (Global_USART_Config[0]->Payload_Length == UART_Payload_Length_8B)
		{
			USARTx->DR = (*pTxBuffer) & (uint8_t)0xFF ;
		}
		else if (Global_USART_Config[0]->Payload_Length == UART_Payload_Length_9B)
		{
			USARTx->DR = (*pTxBuffer) & (uint16_t)0x1FF ;
		}
	}
	else if (USARTx == USART2)
	{
		if (Global_USART_Config[1]->Payload_Length == UART_Payload_Length_8B)
		{
			USARTx->DR = (*pTxBuffer) & (uint8_t)0xFF ;
		}
		else if (Global_USART_Config[1]->Payload_Length == UART_Payload_Length_9B)
		{
			USARTx->DR = (*pTxBuffer) & (uint16_t)0x1FF ;
		}
	}
	else if (USARTx == USART3)
	{
		if (Global_USART_Config[2]->Payload_Length == UART_Payload_Length_8B)
		{
			USARTx->DR = (*pTxBuffer) & (uint8_t)0xFF ;
		}
		else if (Global_USART_Config[2]->Payload_Length == UART_Payload_Length_9B)
		{
			USARTx->DR = (*pTxBuffer) & (uint16_t)0x1FF ;
		}
	}
}

void MCAL_UART_WAIT_TC(USART_t* USARTx)
{
	/* Wait till TC flag is set in the SR */
	while(!(USARTx->SR & 1<<6));
}
/**===============================================
 * @Fn			- MCAL_USART_Receive
 * @brief		- The USART can receive data words of either 8 or 9 bits depending on the M bit in the
					USART_CR1 register
 * @param[in]	- USARTx: Where x can be (1...3) to select the USART peripheral
 * @param[in]	- pRxBuffer: data buffer which will store in registers and transmit
 * @param[in]	- PollingEn: Polling Enable / Disable
 * @retval		- void ====> none
 * Note			- (Supported feature ASYNCH. Only) & Clock 8 MHZ S
 *  First:
 *  1. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	2. Program the M bit in USART_CR1 to define the word length.
	3. Program the number of stop bits in USART_CR2.
	4. Select DMA enable (DMAR) in USART_CR3 if multibuffer communication is to take
	place. Configure the DMA register as explained in multibuffer communication. STEP 3
	5. Select the desired baud rate using the baud rate register USART_BRR
	6. Set the RE bit USART_CR1. This enables the receiver which begins searching for a start bit
 * */
void MCAL_UART_ReceiveData(USART_t* USARTx, uint16_t* pRxBuffer, enum Polling_mechanism PollingEn)
{
	/* 1. Wait until RXNE flag is set in the SR */
	if (PollingEn == USART_enable)
	{
		while (!(USARTx->SR & 1<<5));
	}

	/* 2. Check the USART_WordLength item for 9-bit or 8-bit in a frame */
	if (USARTx == USART1)
	{
		/* 9 Bits Data */
		if (Global_USART_Config[0]->Payload_Length == UART_Payload_Length_9B)
		{
			if (Global_USART_Config[0]->Parity == UART_Parity_NONE)
			{
				/* All 9 bits are data */
				*pRxBuffer = USARTx->DR ;
			}
			else
			{
				/* Just least byte is data */
				*pRxBuffer = USARTx->DR & (uint8_t)0xFF ;
			}
		}
		/* 8 Bits Data */
		else
		{
			if (Global_USART_Config[0]->Parity == UART_Parity_NONE)
			{
				/* All 8 bits are data */
				*pRxBuffer = USARTx->DR & (uint8_t)0xFF  ;
			}
			else
			{
				/* just least 7 bits are data */
				*pRxBuffer = USARTx->DR & (uint8_t)0x7F ;
			}
		}
	}
	else if (USARTx == USART2)
	{
		/* 9 Bits Data */
		if (Global_USART_Config[1]->Payload_Length == UART_Payload_Length_9B)
		{
			if (Global_USART_Config[1]->Parity == UART_Parity_NONE)
			{
				/* All 9 bits are data */
				*pRxBuffer = USARTx->DR ;
			}
			else
			{
				/* Just least byte is data */
				*pRxBuffer = USARTx->DR & (uint8_t)0xFF ;
			}
		}
		/* 8 Bits Data */
		else
		{
			if (Global_USART_Config[1]->Parity == UART_Parity_NONE)
			{
				/* All 8 bits are data */
				*pRxBuffer = USARTx->DR & (uint8_t)0xFF  ;
			}
			else
			{
				/* just least 7 bits are data */
				*pRxBuffer = USARTx->DR & (uint8_t)0x7F ;
			}
		}
	}
	else if (USARTx == USART3)
	{
		/* 9 Bits Data */
		if (Global_USART_Config[2]->Payload_Length == UART_Payload_Length_9B)
		{
			if (Global_USART_Config[2]->Parity == UART_Parity_NONE)
			{
				/* All 9 bits are data */
				*pRxBuffer = USARTx->DR ;
			}
			else
			{
				/* Just least byte is data */
				*pRxBuffer = USARTx->DR & (uint8_t)0xFF ;
			}
		}
		/* 8 Bits Data */
		else
		{
			if (Global_USART_Config[2]->Parity == UART_Parity_NONE)
			{
				/* All 8 bits are data */
				*pRxBuffer = USARTx->DR & (uint8_t)0xFF  ;
			}
			else
			{
				/* just least 7 bits are data */
				*pRxBuffer = USARTx->DR & (uint8_t)0x7F ;
			}
		}
	}
}


/* ================================================================
 * @Fn				- MCAL_UART_GPIO_Set_Pins
 * @brief			- Initializes GPIO Pins
 * @param[in]		- USARTx: where x can be (1..3 depending on device used)
 * @retval 			- None
 * @Notes			- Should enable the corresponding ALT & GPIO in RCC clock . Also called after MCAL_UART_Init()
 */
void MCAL_UART_GPIO_Set_Pins(USART_t* USARTx)
{
	GPIO_PIN_CONFIG_T Pin_Config;

	if (USARTx == USART1)
	{
		// PA9 Tx
		Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_9;
		Pin_Config.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		Pin_Config.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOA, &Pin_Config);

		// PA10 Rx
		Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_10;
		Pin_Config.GPIO_MODE = GPIO_MODE_INPUT_FLO;
		MCAL_GPIO_Init(GPIOA, &Pin_Config);

		if (Global_USART_Config[0]->HwFlowCtl == UART_HwFlowCtl_CTS || Global_USART_Config[0]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			// PA11 CTS
			Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_11;
			Pin_Config.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &Pin_Config);
		}

		if (Global_USART_Config[0]->HwFlowCtl == UART_HwFlowCtl_RTS || Global_USART_Config[0]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			// PA12 RTS
			Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_12;
			Pin_Config.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin_Config.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
			MCAL_GPIO_Init(GPIOA, &Pin_Config);
		}

	}
	else if (USARTx == USART2)
	{

		// PA2 Tx
		Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_2;
		Pin_Config.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		Pin_Config.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOA, &Pin_Config);

		// PA3 Rx
		Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_3;
		Pin_Config.GPIO_MODE = GPIO_MODE_INPUT_FLO;
		MCAL_GPIO_Init(GPIOA, &Pin_Config);

		if (Global_USART_Config[1]->HwFlowCtl == UART_HwFlowCtl_CTS || Global_USART_Config[1]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			// PA0 CTS
			Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_0;
			Pin_Config.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOA, &Pin_Config);
		}

		if (Global_USART_Config[1]->HwFlowCtl == UART_HwFlowCtl_RTS || Global_USART_Config[1]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			// PA1 RTS
			Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_1;
			Pin_Config.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin_Config.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
			MCAL_GPIO_Init(GPIOA, &Pin_Config);
		}

	}
	else if (USARTx == USART3)
	{

		// PB10 Tx
		Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_10;
		Pin_Config.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		Pin_Config.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

		// PB11 Rx
		Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_11;
		Pin_Config.GPIO_MODE = GPIO_MODE_INPUT_FLO;
		MCAL_GPIO_Init(GPIOB, &Pin_Config);

		if (Global_USART_Config[2]->HwFlowCtl == UART_HwFlowCtl_CTS || Global_USART_Config[2]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			// PA11 CTS
			Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_13;
			Pin_Config.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			MCAL_GPIO_Init(GPIOB, &Pin_Config);
		}

		if (Global_USART_Config[2]->HwFlowCtl == UART_HwFlowCtl_RTS || Global_USART_Config[2]->HwFlowCtl == UART_HwFlowCtl_RTS_CTS)
		{
			// PA12 RTS
			Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN_14;
			Pin_Config.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			Pin_Config.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
			MCAL_GPIO_Init(GPIOB, &Pin_Config);
		}
	}
}

void USART1_IRQHandler (void)
{
	Global_USART_Config[0]->P_IRQ_CallBack();
}

void USART2_IRQHandler (void)
{
	Global_USART_Config[1]->P_IRQ_CallBack();
}

void USART3_IRQHandler (void)
{
	Global_USART_Config[2]->P_IRQ_CallBack();
}
