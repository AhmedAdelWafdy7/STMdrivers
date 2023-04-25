/*
 * Stm32_F103C6_EXTI.c
 *
 *  Created on: Apr 23, 2023
 *      Author: Ahmed Adel Wafdy
 */

#include "Stm32_F103C6_EXTI.h"
#include "stm32f103x6.h"

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*-* GENERIC Variables -*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
void(* GP_IRQ_callBack[15])(void);	//array of pointers to function which has the callback of the ISR
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*-* GENERIC MACROS -*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#define AFIO_EXTI_GPIO_Mapping(x)	(	(x== GPIOA)?0:\
		(x== GPIOB)?1:\
				(x== GPIOC)?2:\
						(x== GPIOD)?3:0 )

void update_EXTI(EXTI_PIN_CONFIG_t *exti);


void Enable_NVIC(uint16_t IL)
{
	switch(IL)
	{
	case 0:
		NVIC_IRQ6_EXTI0_ENABLE;
		break;

	case 1:
		NVIC_IRQ7_EXTI1_ENABLE;
		break;

	case 2:
		NVIC_IRQ8_EXTI2_ENABLE;
		break;

	case 3:
		NVIC_IRQ9_EXTI3_ENABLE;
		break;

	case 4:
		NVIC_IRQ10_EXTI4_ENABLE;
		break;

	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_IRQ23_EXTI9_5_ENABLE;
		break;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_IRQ40_EXTI10_15_ENABLE;
		break;
	}
}


void Disable_NVIC(uint16_t IL)
{
	switch(IL)
	{
	case 0:
		NVIC_IRQ6_EXTI0_DISABLE;
		break;

	case 1:
		NVIC_IRQ7_EXTI1_DISABLE;
		break;

	case 2:
		NVIC_IRQ8_EXTI2_DISABLE;
		break;

	case 3:
		NVIC_IRQ9_EXTI3_DISABLE;
		break;

	case 4:
		NVIC_IRQ10_EXTI4_DISABLE;
		break;

	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_IRQ23_EXTI9_5_DISABLE;
		break;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_IRQ40_EXTI10_15_DISABLE;
		break;
	}
}



/*
 *============================================
 *============================================
 * APIs Supported by "MCAL EXTI DRIVER"
 *============================================
 *============================================
 *
 */


/**===============================================
 * @Fn			- MCAL_EXTI_Deinit
 * @brief		- Reset EXTI registers and NVIC for cortex M3
 * @param[in]	- void ====> none
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_EXTI_Deinit(void){
	EXTI->IMR = 0x00000000;
	EXTI->EMR = 0x00000000;
	EXTI->RTSR = 0x00000000;
	EXTI->FTSR = 0x00000000;
	EXTI->SWIER = 0x00000000;
	EXTI->PR = 0xFFFFFFFF;
//Pending register (EXTI_PR)
	//This bit is set when the selected edge event arrives on the external interrupt line. This bit is
//	cleared by writing a ‘1’ into the bit
//NVIC IRQ reset
	NVIC_IRQ6_EXTI0_DISABLE	;
	NVIC_IRQ7_EXTI1_DISABLE	;
	NVIC_IRQ8_EXTI2_DISABLE	;
	NVIC_IRQ9_EXTI3_DISABLE	;
	NVIC_IRQ10_EXTI4_DISABLE	;
	NVIC_IRQ23_EXTI9_5_DISABLE	;
	NVIC_IRQ40_EXTI10_15_DISABLE;
}
/**===============================================
 * @Fn			- MCAL_EXTI_Init
 * @brief		- Initialization EXTI
 * @param[in]	- exti: exti is a object from EXTI pin configuration
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_EXTI_Init(EXTI_PIN_CONFIG_t *exti){
	update_EXTI(exti);
}
/**===============================================
 * @Fn			- MCAL_EXTI_Update
 * @brief		- Up date EXTI IRQ
 * @param[in]	- exti: exti is a object from EXTI pin configuration
 * @retval		- void ====> none
 * Note			- none
 * */
void update_EXTI(EXTI_PIN_CONFIG_t *exti){

	//configure GPIO to be AF input =====> Floating input

	GPIO_PIN_CONFIG_T pinConfig;
	pinConfig.GPIO_PIN_NUMBER = exti->EXTI_PIN.GPIO_pin;
	pinConfig.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(exti->EXTI_PIN.GPIO_Port, &pinConfig);


	//update AFIO to route between EXTI Interrupt lines and PORTx (x:A,B,C,D)
	uint8_t AFIO_EXTICR_index = exti->EXTI_PIN.EXTI_interrupt_line / 4;
	uint8_t AFIO_EXTICR_position = (exti->EXTI_PIN.EXTI_interrupt_line % 4) * 4;


	//clear the four bit
	AFIO->EXTICR[AFIO_EXTICR_index] &= ~(0xF << AFIO_EXTICR_position);
	AFIO->EXTICR[AFIO_EXTICR_index] |= ((AFIO_EXTI_GPIO_Mapping(exti->EXTI_PIN.GPIO_Port) & 0xF) << AFIO_EXTICR_position );

	//update rising and falling edge
	EXTI->RTSR &= ~(1<<exti->EXTI_PIN.EXTI_interrupt_line);
	EXTI->FTSR &= ~(1<<exti->EXTI_PIN.EXTI_interrupt_line);

	if(exti->TRIGGER_CASE ==EXTI_TRIGGER_RISING_EDGE)
	{
		EXTI->RTSR |= (1<<exti->EXTI_PIN.EXTI_interrupt_line);

	}
	else if(exti->TRIGGER_CASE  ==EXTI_TRIGGER_FALLING_EDGE)
	{
		EXTI->FTSR |= (1<<exti->EXTI_PIN.EXTI_interrupt_line);

	}
	else if(exti->TRIGGER_CASE ==EXTI_TRIGGER_RISING_FALLING)
	{
		EXTI->RTSR |= (1<<exti->EXTI_PIN.EXTI_interrupt_line);
		EXTI->FTSR |= (1<<exti->EXTI_PIN.EXTI_interrupt_line);

	}



	//4 Update IRQ Handling CALLBACK
	GP_IRQ_callBack[exti->EXTI_PIN.EXTI_interrupt_line] = exti->IRQ_HANDLER;

	//5 enable/disable IRQ & NVIC
	if(exti->IRQ_EN == EXTI_IRQ_ENABLE)
	{
		//enable interrupt mask register
		EXTI->IMR |= 1<<exti->EXTI_PIN.EXTI_interrupt_line;

		//enable NVIC
		Enable_NVIC(exti->EXTI_PIN.EXTI_interrupt_line);
	}
	else if(exti->IRQ_EN == EXTI_IRQ_DISABLE)
	{
		//disable interrupt mask register
		EXTI->IMR &= ~(1<<exti->EXTI_PIN.EXTI_interrupt_line);

		//disable NVIC
		Disable_NVIC(exti->EXTI_PIN.EXTI_interrupt_line);
	}
}

void MCAL_EXTI_Update(EXTI_PIN_CONFIG_t *exti){
	update_EXTI(exti);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*-* ISR IRQ HANDLERS -*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

/*Bits 19:0 PRx: Pending bit
0: No trigger request occurred
1: selected trigger request occurred
This bit is set when the selected edge event arrives on the external interrupt line. This bit is
cleared by writing a ‘1’ into the bit.*/

void EXTI0_IRQHandler(void)
{
	//clear pending flag
	EXTI->PR |= 1<<0;
	GP_IRQ_callBack[0]();
}

void EXTI1_IRQHandler(void)
{
	EXTI->PR |= 1<<1;
	GP_IRQ_callBack[1]();
}

void EXTI2_IRQHandler(void)
{
	EXTI->PR |= 1<<2;
	GP_IRQ_callBack[2]();
}

void EXTI3_IRQHandler(void)
{
	EXTI->PR |= 1<<3;
	GP_IRQ_callBack[3]();

}

void EXTI4_IRQHandler(void)
{
	EXTI->PR |= 1<<4;
	GP_IRQ_callBack[4]();
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & 1<<5) {EXTI->PR |= 1<<5; GP_IRQ_callBack[5]();}
	if(EXTI->PR & 1<<6) {EXTI->PR |= 1<<6; GP_IRQ_callBack[6]();}
	if(EXTI->PR & 1<<7) {EXTI->PR |= 1<<7; GP_IRQ_callBack[7]();}
	if(EXTI->PR & 1<<8) {EXTI->PR |= 1<<8; GP_IRQ_callBack[8]();}
	if(EXTI->PR & 1<<9) {EXTI->PR |= 1<<9; GP_IRQ_callBack[9]();}

}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & 1<<10) {EXTI->PR |= 1<<10; GP_IRQ_callBack[10]();}
	if(EXTI->PR & 1<<11) {EXTI->PR |= 1<<11; GP_IRQ_callBack[11]();}
	if(EXTI->PR & 1<<12) {EXTI->PR |= 1<<12; GP_IRQ_callBack[12]();}
	if(EXTI->PR & 1<<13) {EXTI->PR |= 1<<13; GP_IRQ_callBack[13]();}
	if(EXTI->PR & 1<<14) {EXTI->PR |= 1<<14; GP_IRQ_callBack[14]();}
	if(EXTI->PR & 1<<15) {EXTI->PR |= 1<<15; GP_IRQ_callBack[15]();}

}

