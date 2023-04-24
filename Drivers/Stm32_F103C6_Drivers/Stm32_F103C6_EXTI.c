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
void(*IRQ_HANDLER[15])(void);
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*-* GENERIC MACROS -*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#define AFIO_GPIO_EXTI_MAPPING(x)           (  (x==GPIOA)? 0:\
		                                       (x==GPIOB)? 1:\
				                               (x==GPIOC)? 2:\
						                       (x==GPIOD)? 3:0 )



void enable_NVIC(uint16_t IRQ){
	switch(IRQ){
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

void disable_NVIC(uint16_t IRQ){
	switch(IRQ){
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
//Pending register (EXTI_PR)
	//This bit is set when the selected edge event arrives on the external interrupt line. This bit is
//	cleared by writing a ‘1’ into the bit
	EXTI->PR = 0xFFFFFFFF;
//NVIC IRQ reset
	NVIC_IRQ6_EXTI0_DISABLE	;
	NVIC_IRQ7_EXTI1_DISABLE	;
	NVIC_IRQ8_EXTI2_DISABLE	;
	NVIC_IRQ9_EXTI3_DISABLE	;
	NVIC_IRQ10_EXTI4_DISABLE;
	NVIC_IRQ23_EXTI9_5_DISABLE;
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

}
/**===============================================
 * @Fn			- MCAL_EXTI_Update
 * @brief		- Up date EXTI IRQ
 * @param[in]	- exti: exti is a object from EXTI pin configuration
 * @retval		- void ====> none
 * Note			- none
 * */
void MCAL_EXTI_Update(EXTI_PIN_CONFIG_t *exti){

	//configure GPIO to be AF input =====> Floating input
	GPIO_PIN_CONFIG_T pin_cfg;
	pin_cfg.GPIO_PIN_NUMBER = exti->EXTI_PIN.GPIO_pin;
	pin_cfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(exti->EXTI_PIN.GPIO_Port,&pin_cfg);
	//update AFIO to route between EXTI Interrupt lines and PORTx (x:A,B,C,D)
	uint8_t AFIO_EXTICR_index = exti->EXTI_PIN.EXTI_interrupt_line /4;
	uint8_t AFIO_EXTICR_position =( exti->EXTI_PIN.EXTI_interrupt_line %4)*4;
	//clear the four bit
	AFIO->EXTICR[AFIO_EXTICR_index] &= ~(0xFF << AFIO_EXTICR_position );
	AFIO->EXTICR[AFIO_EXTICR_index] |= ((AFIO_GPIO_EXTI_MAPPING(exti->EXTI_PIN.GPIO_Port) &0xF )<< AFIO_EXTICR_position );

	//update rising and falling edge
	EXTI->RTSR &= ~(1 << exti->EXTI_PIN.EXTI_interrupt_line);
	EXTI->FTSR &= ~(1 << exti->EXTI_PIN.EXTI_interrupt_line);

	if(exti->TRIGGER_CASE == EXTI_TRIGGER_FALLING_EDGE){
		EXTI->FTSR |= (1 << exti->EXTI_PIN.EXTI_interrupt_line);
	}
	else if(exti->TRIGGER_CASE == EXTI_TRIGGER_RISING_FALLING){
		EXTI->RTSR |= (1 << exti->EXTI_PIN.EXTI_interrupt_line);
		EXTI->FTSR |= (1 << exti->EXTI_PIN.EXTI_interrupt_line);
	}

	//update IRQ HANDLER callback
	IRQ_HANDLER[exti->EXTI_PIN.EXTI_interrupt_line] = exti->IRQ_HANDLER;

	// ENABLE/DISABLE IRQ & NVIC
	if (exti->IRQ_EN == EXTI_IRQ_ENABLE){
		EXTI->IMR |= (1 << exti->EXTI_PIN.EXTI_interrupt_line);
		enable_NVIC(exti->EXTI_PIN.EXTI_interrupt_line);
	}
	else {
		EXTI->IMR &= ~(1 << exti->EXTI_PIN.EXTI_interrupt_line);
		disable_NVIC(exti->EXTI_PIN.EXTI_interrupt_line);
	}
}


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*-* ISR IRQ HANDLERS -*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

/*Bits 19:0 PRx: Pending bit
0: No trigger request occurred
1: selected trigger request occurred
This bit is set when the selected edge event arrives on the external interrupt line. This bit is
cleared by writing a ‘1’ into the bit.*/
void EXTI0_IRQ_HANDLER(void){
	//clear bit
	EXTI->PR |= (1<<0);
	// call IRQ
	IRQ_HANDLER[0]();
}
void EXTI1_IRQ_HANDLER(void){
	//clear bit
	EXTI->PR |= (1<<1);
	// call IRQ
	IRQ_HANDLER[1]();
}
void EXTI2_IRQ_HANDLER(void){
	//clear bit
	EXTI->PR |= (1<<2);
	// call IRQ
	IRQ_HANDLER[2]();
}
void EXTI3_IRQ_HANDLER(void){
	//clear bit
	EXTI->PR |= (1<<3);
	// call IRQ
	IRQ_HANDLER[3]();
}
void EXTI4_IRQ_HANDLER(void){
	//clear bit
	EXTI->PR |= (1<<4);
	// call IRQ
	IRQ_HANDLER[4]();
}
void EXTI9_5_IRQ_HANDLER(void){
	if(EXTI->PR & 1<<5){	//clear bit
		EXTI->PR |= (1<<5);
		// call IRQ
		IRQ_HANDLER[5]();}

	if(EXTI->PR & 1<<6){	//clear bit
		EXTI->PR |= (1<<6);
		// call IRQ
		IRQ_HANDLER[6]();}

	if(EXTI->PR  & 1<<7){	//clear bit
		EXTI->PR |= (1<<7);
		// call IRQ
		IRQ_HANDLER[7]();}

	if(EXTI->PR & 1<<8){	//clear bit
		EXTI->PR |= (1<<8);
		// call IRQ
		IRQ_HANDLER[8]();}

	if(EXTI->PR  & 1<<9){	//clear bit
		EXTI->PR |= (1<<9);
		// call IRQ
		IRQ_HANDLER[9]();}

}
void EXTI10_15_IRQ_HANDLER(void){
	if(EXTI->PR & 1<<10){	//clear bit
		EXTI->PR |= (1<<10);
		// call IRQ
		IRQ_HANDLER[10]();}

	if(EXTI->PR  & 1<<11){	//clear bit
		EXTI->PR |= (1<<11);
		// call IRQ
		IRQ_HANDLER[11]();}

	if(EXTI->PR & 1<<12){	//clear bit
		EXTI->PR |= (1<<12);
		// call IRQ
		IRQ_HANDLER[12]();}

	if(EXTI->PR & 1<<13){	//clear bit
		EXTI->PR |= (1<<13);
		// call IRQ
		IRQ_HANDLER[13]();}

	if(EXTI->PR  & 1<<14){	//clear bit
		EXTI->PR |= (1<<14);
		// call IRQ
		IRQ_HANDLER[14]();}

	if(EXTI->PR & 1<<15){	//clear bit
		EXTI->PR |= (1<<15);
		// call IRQ
		IRQ_HANDLER[15]();}
}
