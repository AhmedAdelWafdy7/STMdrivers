/*
 * Stm32_F103C6_GPIO.h
 *
 *  Created on: Apr 19, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_STM32_F103C6_GPIO_H_
#define INC_STM32_F103C6_GPIO_H_

//-----------------------------
//INCLUDES
//-----------------------------
#include "stm32f103x6.h"

//-----------------------------
//USER TYPE DEFINITIONs
//-----------------------------

//config Structures
typedef struct{
	uint16_t GPIO_PIN_NUMBER;//specify the GPIO pins to be configured
							//this parameter can be a value of @ref GPIO_PIN_define
	uint8_t GPIO_MODE;//specify the GPIO operating MODE selected to the pin
						//this parameter can be a value of @ref GPIO_MODE_define
	uint8_t GPIO_OUTPUT_SPEED;//specify the GPIO Speed selected to the pin
	//this parameter can be a value of @ref GPIO_SPEED_define
}GPIO_PIN_CONFIG_T;

//-----------------------------
//MACROS CONFIGURATION REFERENCES
//-----------------------------

/*@ref GPIO_PIN_define
 */

#define GPIO_PIN_0					((uint16_t)0x0001)
#define GPIO_PIN_1					((uint16_t)0x0002)
#define GPIO_PIN_2					((uint16_t)0x0004)
#define GPIO_PIN_3					((uint16_t)0x0008)
#define GPIO_PIN_4					((uint16_t)0x0010)
#define GPIO_PIN_5					((uint16_t)0x0020)
#define GPIO_PIN_6					((uint16_t)0x0040)
#define GPIO_PIN_7					((uint16_t)0x0080)
#define GPIO_PIN_8					((uint16_t)0x0100)
#define GPIO_PIN_9					((uint16_t)0x0200)
#define GPIO_PIN_10					((uint16_t)0x0400)
#define GPIO_PIN_11					((uint16_t)0x0800)
#define GPIO_PIN_12					((uint16_t)0x1000)
#define GPIO_PIN_13					((uint16_t)0x2000)
#define GPIO_PIN_14					((uint16_t)0x4000)
#define GPIO_PIN_15					((uint16_t)0x8000)
#define GPIO_PIN_ALL				((uint16_t)0xFFFF)
#define GPIO_PIN_MASK				0x0000FFFFu

/*@ref GPIO_MODE_define
*/
/*
0: Analog mode
1: Floating input (reset state)
2: Input with pull-up
3: Input with pull-down
4: General purpose output push-pull
5: General purpose output Open-drain
6: Alternate function output Push-pull
7: Alternate function output Open-drain
8: Alternate function input
*/
#define GPIO_MODE_Analog			0x00000000u//Analog mode
#define GPIO_MODE_INPUT_FLO			0x00000001u//Floating input (reset state)
#define GPIO_MODE_INPUT_PU			0x00000002u//Input with pull-up
#define GPIO_MODE_INPUT_PD			0x00000003u//Input with pull-down
#define GPIO_MODE_OUTPUT_PP			0x00000004u//General purpose output push-pull
#define GPIO_MODE_OUTPUT_OD			0x00000005u//General purpose output Open-drain
#define GPIO_MODE_OUTPUT_AF_PP		0x00000006u// Alternate function output Push-pull
#define GPIO_MODE_OUTPUT_AF_OD		0x00000007u//Alternate function output Open-drain
#define GPIO_MODE_AF_INPUT			0x00000008u//Alternate function input


/*@ref GPIO_SPEED_define
*/
/*
1: Output mode, max speed 10 MHz.
2: Output mode, max speed 2 MHz.
3: Output mode, max speed 50 MHz*/

#define GPIO_SPEED_10MHz			0x00000001u// Output mode, max speed 10 MHz.
#define GPIO_SPEED_2MHz				0x00000002u// Output mode, max speed 2 MHz.
#define GPIO_SPEED_50MHz			0x00000003u// Output mode, max speed 50 MHz.

/*@ref GPIO_PIN_STATE
*/
#define GPIO_PIN_SET				1
#define GPIO_PIN_RESET				0

/*@ref GPIO_RETURN_LOCK
 */
#define GPIO_RETURN_LOCK_OK			1
#define GPIO_RETURN_LOCK_ERROR		0

/*
 *============================================
 *============================================
 * APIs Supported by "MCAL GPIO DRIVER"
 *============================================
 *============================================
 *
 */
void MCAL_GPIO_Init(GPIO_t *GPIOx,GPIO_PIN_CONFIG_T *PinConfig);
void MCAL_GPIO_DeInit(GPIO_t *GPIOx);

//READ APIS
uint8_t MCAL_GPIO_ReadPin(GPIO_t *GPIOx,uint16_t PinNumber);
uint16_t MCAL_GPIO_ReadPort(GPIO_t *GPIOx);

//Write APIs
void MCAL_GPIO_WritePort(GPIO_t *GPIOx,uint8_t value);
void MCAL_GPIO_WritePin(GPIO_t *GPIOx,uint16_t PinNumber,uint8_t value);

void MCAL_GPIO_TogglePin(GPIO_t *GPIOx,uint16_t PinNumber);
uint8_t MCAL_GPIO_LockPin(GPIO_t *GPIOx,uint16_t PinNumber);

#endif /* INC_STM32_F103C6_GPIO_H_ */
