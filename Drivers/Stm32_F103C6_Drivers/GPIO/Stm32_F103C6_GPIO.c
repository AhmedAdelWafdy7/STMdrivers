/*
 * Stm32_F103C6_GPIO.c
 *
 *  Created on: Apr 19, 2023
 *      Author: Ahmed Adel Wafdy
 */
#include "Stm32_F103C6_GPIO.h"


uint8_t Get_Bit_Location(uint16_t PinNumber){
	switch(PinNumber){
		case GPIO_PIN_0:
			return 0;
			break;
		case GPIO_PIN_1:
			return 4;
			break;
		case GPIO_PIN_2:
			return 8;
			break;
		case GPIO_PIN_3:
			return 12;
			break;
		case GPIO_PIN_4:
			return 16;
			break;
		case GPIO_PIN_5:
			return 20;
			break;
		case GPIO_PIN_6:
			return 24;
			break;
		case GPIO_PIN_7:
			return 28;
			break;
		case GPIO_PIN_8:
			return 0;
			break;
		case GPIO_PIN_9:
			return 4;
			break;
		case GPIO_PIN_10:
			return 8;
			break;
		case GPIO_PIN_11:
			return 12;
			break;
		case GPIO_PIN_12:
			return 16;
			break;
		case GPIO_PIN_13:
			return 20;
			break;
		case GPIO_PIN_14:
			return 24;
			break;
		case GPIO_PIN_15:
			return 28;
			break;
	}
	return 0;
}

/**===============================================
 * @Fn			- MCAL_GPIO_Init
 * @brief		- Initialization GPIOx PINy according to specified parameters in the PinConfig
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @param[in]	- PinConfig: Pointer to GPIO_PIN_CONFIG_T structure that contains the configurations of GPIO pin
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_GPIO_Init(GPIO_t *GPIOx,GPIO_PIN_CONFIG_T *PinConfig){
	//Port configuration register low (GPIOx_CRL) (x=A..E)
	//Port configuration register high (GPIOx_CRH) (x=A..E)
	volatile uint32_t *ConfiguredRegister = NULL;
	uint32_t pin_config = 0;
	if(PinConfig->GPIO_PIN_NUMBER < GPIO_PIN_8){
		ConfiguredRegister = &(GPIOx->CRL);
	}
	else{
		ConfiguredRegister = &(GPIOx->CRH);
	}
	// We should clear registers CNFy[1:0]: Port x configuration bits (y= 0 .. 7), MODEy[1:0]: Port x mode bits (y= 0 .. 7)
	(*ConfiguredRegister) &= ~(0xf << (Get_Bit_Location(PinConfig->GPIO_PIN_NUMBER)));
	// if GPIO ====>Output
	if( (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_OD) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_PP) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_AF_OD) || (PinConfig->GPIO_MODE == GPIO_MODE_OUTPUT_AF_PP))
	{
		//set CNFy[1:0],MODEy[1:0]
		pin_config = ( ( (PinConfig->GPIO_MODE-4) << 2 ) | (PinConfig->GPIO_OUTPUT_SPEED) ) &0x0f ;
	}
	// if GPIO ====>Input 00: Input mode (reset state)
	else{
		if( (PinConfig->GPIO_MODE == GPIO_MODE_INPUT_FLO) || (PinConfig->GPIO_MODE == GPIO_MODE_Analog)){
			//set CNFy[1:0],MODEy[1:0]
			pin_config = (( ( (PinConfig->GPIO_MODE) << 2 ) | 0x0) &0x0f);
		}
		else if(PinConfig->GPIO_MODE == GPIO_MODE_AF_INPUT){
			//set CNFy[1:0],MODEy[1:0]
			pin_config = (( ( GPIO_MODE_INPUT_FLO << 2 ) | 0x0) &0x0f);
		}
		//Input with pull-up / pull-down
		else{
			pin_config = (( ( GPIO_MODE_INPUT_PU << 2 ) | 0x0) &0x0f);
			if(PinConfig->GPIO_MODE ==  GPIO_MODE_INPUT_PU){
				//PxODR register 1 Input pull-up
				GPIOx->ODR |= PinConfig->GPIO_PIN_NUMBER;
			}
			else{
				//PxODR register 0 Input pull-down
				GPIOx->ODR &= ~(PinConfig->GPIO_PIN_NUMBER);
			}
		}
	}
	// Write on CNFy[1:0]: Port x configuration bits (y= 0 .. 7), MODEy[1:0]: Port x mode bits (y= 0 .. 7)
	(*ConfiguredRegister) |= (pin_config << (Get_Bit_Location(PinConfig->GPIO_PIN_NUMBER)));
}

/**===============================================
 * @Fn			- MCAL_GPIO_DeInit
 * @brief		- Reset GPIO registers and bits
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_GPIO_DeInit(GPIO_t *GPIOx){
	//Reset Clock
	if(GPIOx == GPIOA){
		//Bit 2 IOPARST: IO port A reset
		RCC->APB2RSTR |= (1<<2);
		RCC->APB2RSTR &= ~(1<<2);
	}
	else if(GPIOx == GPIOB){
		//Bit 3 IOPBRST: IO port B reset
		RCC->APB2RSTR |= (1<<3);
		RCC->APB2RSTR &= ~(1<<3);
	}
	else if(GPIOx == GPIOC){
		//Bit 4 IOPCRST: IO port C reset
		RCC->APB2RSTR |= (1<<4);
		RCC->APB2RSTR &= ~(1<<4);
	}
	else if(GPIOx == GPIOD){
		//Bit 5 IOPDRST: IO port D reset
		RCC->APB2RSTR |= (1<<5);
		RCC->APB2RSTR &= ~(1<<5);
	}
	else if(GPIOx == GPIOE){
		//Bit 6 IOPERST: IO port E reset
		RCC->APB2RSTR |= (1<<6);
		RCC->APB2RSTR &= ~(1<<6);
	}
}

//READ APIS

/**===============================================
 * @Fn			- MCAL_GPIO_ReadPin
 * @brief		- Read value in pin(PinNumber)
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @param[in]	- PinNumber : set pin number according to @ref GPIO_PIN_NUMBER
 * @retval		- return pin state according to @ref GPIO_PIN_STATE
 * Note			- none
 * */
uint8_t MCAL_GPIO_ReadPin(GPIO_t *GPIOx,uint16_t PinNumber){
	uint8_t bit_status;
	if( ( (GPIOx->IDR)&PinNumber) != GPIO_PIN_RESET){
		bit_status = GPIO_PIN_SET;
	}
	else{
		bit_status = GPIO_PIN_RESET;
	}
	return bit_status;
}

/**===============================================
 * @Fn			- MCAL_GPIO_ReadPort
 * @brief		- Read value in all port pins
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @retval		- return port value
 * Note			- none
 * */
uint16_t MCAL_GPIO_ReadPort(GPIO_t *GPIOx){
	uint16_t port_value;
	port_value = (uint16_t)GPIOx->IDR;
	return port_value;
}

//Write APIs

/**===============================================
 * @Fn			- MCAL_GPIO_WritePort
 * @brief		- Write value on port
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @param[in]	- value: value which will be written on port
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_GPIO_WritePort(GPIO_t *GPIOx,uint8_t value){
	GPIOx->ODR = (uint32_t)value;
}

/**===============================================
 * @Fn			- MCAL_GPIO_WritePin
 * @brief		- write value on specified pin on port
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @param[in]	- PinNumber : set pin number according to @ref GPIO_PIN_NUMBER
 * @param[in]	- value: value which will be written on pin
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_GPIO_WritePin(GPIO_t *GPIOx,uint16_t PinNumber,uint8_t value){
	if(value != GPIO_PIN_RESET){
		//Bits 15:0 BSy: Port x Set bit y (y= 0 .. 15)
		//These bits are write-only and can be accessed in Word mode only.
		//0: No action on the corresponding ODRx bit
		//1: Set the corresponding ODRx bit
		GPIOx->BSRR = PinNumber;
	}
	else{
		/*Bits 15:0 BRy: Port x Reset bit y (y= 0 .. 15)
These bits are write-only and can be accessed in Word mode only.
0: No action on the corresponding ODRx bit
1: Reset the corresponding ODRx bit*/
		GPIOx->BRR =(uint32_t)PinNumber;

	}
}


/**===============================================
 * @Fn			- MCAL_GPIO_TogglePin
 * @brief		- Toggle state on specified pin on port
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @param[in]	- PinNumber : set pin number according to @ref GPIO_PIN_NUMBER
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_GPIO_TogglePin(GPIO_t *GPIOx,uint16_t PinNumber){
	GPIOx->ODR ^= PinNumber;
}

/**===============================================
 * @Fn			- MCAL_GPIO_LockPin
 * @brief		- lock the configuration of the GPIO. During the write sequence
 * @param[in]	- GPIOx: Where x can be (A...E) to select the GPIO peripheral
 * @param[in]	- PinNumber : set pin number according to @ref GPIO_PIN_NUMBER
 * @retval		- OK if pin confg is locked or ERROR if nmot locked , OK & ERROR are defined @ref GPIO_RETURN_LOCK
 * Note			- none
 * */
uint8_t MCAL_GPIO_LockPin(GPIO_t *GPIOx,uint16_t PinNumber){
/*Bit 16 LCKK[16]: Lock key
This bit can be read anytime. It can only be modified using the Lock Key Writing Sequence.
0: Port configuration lock key not active
1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
LOCK key writing sequence:
Write 1
Write 0
Write 1
Read 0
Read 1 (this read is optional but confirms that the lock is active)*/
	volatile uint32_t temp = 1<<16;
	temp |= PinNumber;
	//Write 1
	GPIOx->LCKR = temp;
	//Write 0
	GPIOx->LCKR = PinNumber;
	//Write 1
	GPIOx->LCKR = temp;
	//Read 0
	temp = GPIOx->LCKR;
	//Read 1 (this read is optional but confirms that the lock is active)
	if((uint32_t)(GPIOx->LCKR & 1 << 16)){
		return GPIO_RETURN_LOCK_OK;
	}
	else{
		return GPIO_RETURN_LOCK_ERROR;
	}
}
