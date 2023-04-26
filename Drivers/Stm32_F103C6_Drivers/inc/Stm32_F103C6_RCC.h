/*
 * Stm32_F103C6_RCC.h
 *
 *  Created on: Apr 26, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_STM32_F103C6_RCC_H_
#define INC_STM32_F103C6_RCC_H_

//-----------------------------
//INCLUDES
//-----------------------------
#include "stm32f103x6.h"


//-----------------------------
//MACROS CONFIGURATION REFERENCES
//-----------------------------

#define HSE_CLK				(uint32_t)16000000
#define HSI_RC_CLK			(uint32_t)8000000

/*
 *============================================
 *============================================
 * APIs Supported by "MCAL RCC DRIVER"
 *============================================
 *============================================
 *
 */
uint32_t MCAL_RCC_GetSYS_CLKFreq(void);
uint32_t MCAL_RCC_Get_HCLKFreq(void);
uint32_t MCAL_RCC_Get_PCLK1Freq(void);
uint32_t MCAL_RCC_Get_PCLK2Freq(void);


#endif /* INC_STM32_F103C6_RCC_H_ */
