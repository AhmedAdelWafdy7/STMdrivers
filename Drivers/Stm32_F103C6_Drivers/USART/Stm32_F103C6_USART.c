/*
 * Stm32_F103C6_USART.c
 *
 *  Created on: Apr 26, 2023
 *      Author: Ahmed Adel wafdy
 */

#include "Stm32_F103C6_USART.h"

USART_CONFIG_t* Global_UART_Config  = NULL ;


void MCAL_UART_GPIO_Set_Pins (USART_t *USARTx){
	GPIO_PIN_CONFIG_T PinCfg ;

		if ( USARTx == USART1 )
		{
			//PA9 TX
			//PA10 RX
			//PA11 CTS
			//PA12 RTS

			//PA9 TX
			PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_9;
			PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			//PA10 RX
			PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_10;
			PinCfg.GPIO_MODE = GPIO_MODE_AF_INPUT;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			if (Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS  || Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS_RTS  ){
				//PA11 CTS
				PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_11;
				PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}


			if (Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_RTS || Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS_RTS  )
			{
				//PA12 RTS
				PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_12;
				PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
				MCAL_GPIO_Init(GPIOA, &PinCfg);

			}
		}
		else if ( USARTx == USART2 )
		{
			//PA2 TX
			//PA3 RX
			//PA0 CTS
			//PA1 RTS


			//PA2 TX
			PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_2;
			PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			//PA3 RX
			PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_3;
			PinCfg.GPIO_MODE = GPIO_MODE_AF_INPUT;
			MCAL_GPIO_Init(GPIOA, &PinCfg);

			if (Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS  || Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS_RTS )
			{
				//PA0 CTS
				PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_0;
				PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}

			if (Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_RTS || Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS_RTS  )
			{
				//PA1 RTS
				PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_1;
				PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
				MCAL_GPIO_Init(GPIOA, &PinCfg);
			}
		}
		else if ( USARTx == USART3 )
		{
			//PB10 TX
			//PB11 RX
			//PB13 CTS
			//PA14 RTS



			//PB10 TX
			PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_10;
			PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
			MCAL_GPIO_Init(GPIOB, &PinCfg);

			//PB11 RX
			PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_11;
			PinCfg.GPIO_MODE = GPIO_MODE_AF_INPUT;
			MCAL_GPIO_Init(GPIOB, &PinCfg);

			if (Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS  || Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS_RTS  )
			{
				//PB13 CTS
				PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_13;
				PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				MCAL_GPIO_Init(GPIOB, &PinCfg);
			}
			if (Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_RTS || Global_UART_Config->HW_FLW_CTRL  ==USART_HW_FLW_CTS_RTS  )
			{
				//PA14 RTS
				PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_14;
				PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
				MCAL_GPIO_Init(GPIOB, &PinCfg);

			}
		}
}

/**===============================================
 * @Fn			- MCAL_USART_Init
 * @brief		- Initialization USARTx USARTx_config according to specified parameters in the USART_CONFIG_t
 * @param[in]	- USARTx: Where x can be (1...3) to select the USART peripheral
 * @param[in]	- USARTx_config: Pointer to USART_CONFIG_t structure that contains the configurations of USART peripheral
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_USART_Init(USART_t *USARTx , USART_CONFIG_t *USARTx_config){
	Global_UART_Config = USARTx_config;
	uint32_t pclk ,BRR  ;
	//	enable the Clock for given USART peripheral
	if(USARTx == USART1){RCC_USART1_CLK_EN();}
	else if(USARTx == USART2){RCC_USART2_CLK_EN();}
	else if(USARTx == USART3){RCC_USART3_CLK_EN();}

	//enable USART
	USARTx ->CR1 |= (1<<13);//Bit 13 UE: USART enable
	//enable USART mode
	USARTx ->CR1 |= USARTx_config->USART_Mode;
	//Parity selection
	USARTx ->CR1 |= USARTx_config->Parity;
	//M: Word length
	USARTx ->CR1 |= USARTx_config->Word_Length;
	// STOP: STOP bits
	USARTx ->CR2 |= USARTx_config->Stop_Bits;
	//Hardware flow control
	USARTx ->CR3 |= USARTx_config->HW_FLW_CTRL;

	//Configuration of BRR(Baudrate register)
	//PCLK1 for USART2, 3
	//PCLK2 for USART1
	if(USARTx == USART1){pclk = MCAL_RCC_Get_PCLK2Freq();}
	else {
		pclk = MCAL_RCC_Get_PCLK1Freq();
	}
	BRR = UART_BRR_Register(pclk, USARTx_config->Baud_Rate );
	USARTx->BRR = BRR;
	//configure interrupt
	if(USARTx_config->IRQ_Enable != USART_IRQ_NONE ){
		USARTx ->CR1 |= USARTx_config->IRQ_Enable;
		//enable NVIC
		if(USARTx == USART1){NVIC_IRQ37_USART1_ENABLE;}
		else if(USARTx == USART2){NVIC_IRQ38_USART2_ENABLE;}
		else if(USARTx == USART3){NVIC_IRQ39_USART3_ENABLE;}
	}
}
/**===============================================
 * @Fn			- MCAL_USART_DeInit
 * @brief		- Reset GPIO registers and Clocks
 * @param[in]	- USARTx: Where x can be (1...3) to select the USART peripheral
 * @retval		- void ====> none
 * Note			- (Supported feature ASYNCH. Only) & Clock 8 MHZ S
 * */
void MCAL_USART_DeInit(USART_t *USARTx){
	if(USARTx == USART1){NVIC_IRQ37_USART1_DISABLE; RCC_USART1_CLK_RESET();}
	else if(USARTx == USART2){NVIC_IRQ38_USART2_DISABLE; RCC_USART2_CLK_RESET();}
	else if(USARTx == USART3){NVIC_IRQ39_USART3_DISABLE; RCC_USART3_CLK_RESET();}
}
/**===============================================
 * @Fn			- MCAL_USART_Transmit
 * @brief		- The transmitter can send data words of either 8 or 9 bits depending on the M bit status.
					When the transmit enable bit (TE) is set
 * @param[in]	- USARTx: Where x can be (1...3) to select the USART peripheral
 * @param[in]	- Buffer: data buffer which will store in registers and transmit
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
void MCAL_USART_Transmit(USART_t * USARTx , uint16_t *Buffer,enum Polling_mechism PollingEn){


	//7. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this	for each data to be transmitted in case of single buffer.
	//8. After writing the last data into the USART_DR register, wait until TC=1. This indicates
	//that the transmission of the last frame is complete. This is required for instance when
	//the USART is disabled or enters the Halt mode to avoid corrupting the last
	//transmission
	if (PollingEn == enable)
		while(! (USARTx->SR & 1<<7 ) );
	if(Global_UART_Config->Word_Length == USART_WordLength_9BIT){
		USARTx->DR = (*Buffer & (uint16_t) 0x01FF);
	}
	else{
		USARTx->DR = (*Buffer & (uint16_t) 0xFF);
	}
}

void MCAL_USART_Transmit_String(USART_t* USARTx , uint8_t *Buffer ,enum Polling_mechism PollingEn)
{
	uint8_t Count = 0 ;
	//wait until TXE is set
	if(PollingEn == enable)
		while(!(USARTx->SR &(1<<7)));
	while(Buffer[Count] != 0 )
	{
		MCAL_USART_Transmit(USART1, &Buffer[Count], enable);
		Count++;
	}
}
/**===============================================
 * @Fn			- MCAL_USART_Receive
 * @brief		- The USART can receive data words of either 8 or 9 bits depending on the M bit in the
					USART_CR1 register
 * @param[in]	- USARTx: Where x can be (1...3) to select the USART peripheral
 * @param[in]	- Buffer: data buffer which will store in registers and transmit
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
void MCAL_USART_Receive(USART_t * USARTx , uint16_t *Buffer,enum Polling_mechism PollingEn){
	if (PollingEn == enable)
		//Bit 5 RXNE: Read data register not empty
		while(! (USARTx->SR & 1<<5 ) );
	if(Global_UART_Config->Word_Length == USART_WordLength_9BIT){
		if (Global_UART_Config->Parity ==USART_Parity_NONE)
		{
			//no parity So all 9bit are considered data
			*((uint16_t*) Buffer) = USARTx->DR ;

		}else
		{
			//Parity is used, so, 8bits will be of user data and 1 bit is parity
			*((uint16_t*) Buffer) = ( USARTx->DR  & (uint8_t)0xFF );

		}
	}else{
		//This is 8bit data
			if (Global_UART_Config->Parity ==USART_Parity_NONE)
			{
				//no parity So all 8bit are considered data
				*((uint16_t*) Buffer) = ( USARTx->DR  & (uint8_t)0xFF ) ;

			}else
			{
				//Parity is used, so,7 bits will be of user data and 1 bit is parity
				*((uint16_t*) Buffer) = ( USARTx->DR  & (uint8_t)0X7F );

			}

	}

}
/**===============================================
 * @Fn			- MCAL_GPIO_Init
 * @brief		- Initialization GPIOx PINy according to specified parameters in the PinConfig
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @param[in]	- PinConfig: Pointer to GPIO_PIN_CONFIG_T structure that contains the configurations of GPIO pin
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_USART_TransmitComplete(USART_t *USARTx){
/*
==> Bit 6 TC: Transmission complete
This bit is set by hardware if the transmission of a frame containing data is complete and if
TXE is set. An interrupt is generated if TCIE=1 in the USART_CR1 register. It is cleared by a
software sequence (a read from the USART_SR register followed by a write to the
USART_DR register). The TC bit can also be cleared by writing a '0' to it. This clearing
sequence is recommended only for multibuffer communication.
0: Transmission is not complete
1: Transmission is complete*/
	while(!(USARTx->SR &1<<6));

}


//ISR
void USART1_IRQHandler (void)
{

	Global_UART_Config->IRQ_HANDLER();

}

void USART2_IRQHandler (void)
{
	Global_UART_Config->IRQ_HANDLER() ;

}
void USART3_IRQHandler (void)
{
	Global_UART_Config->IRQ_HANDLER();


}
