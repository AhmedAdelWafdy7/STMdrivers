/*
 * stm32f103x6.h
 *
 *  Created on: Apr 19, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_STM32F103X6_H_
#define INC_STM32F103X6_H_



//-----------------------------
//Includes
//-----------------------------

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//-----------------------------
//Base addresses for Memories
//-----------------------------

#define FLASH_Memory_BASE						0x08000000UL
#define System_Memory_BASE						0x1FFFF000UL
#define SRAM_BASE								0x20000000UL

#define Peripherals_BASE						0x40000000UL

#define Cortex_M3_Internal_Peripherals_BASE		0xE0000000UL

//NVIC register map
#define NVIC_BASE								0xE000E100UL

#define NVIC_ISER0								*(volatile uint32_t *)(NVIC_BASE + 0x00)
#define NVIC_ISER1								*(volatile uint32_t *)(NVIC_BASE + 0x04)
#define NVIC_ISER2								*(volatile uint32_t *)(NVIC_BASE + 0x08)
#define NVIC_ICER0								*(volatile uint32_t *)(NVIC_BASE + 0x80)
#define NVIC_ICER1								*(volatile uint32_t *)(NVIC_BASE + 0x84)
#define NVIC_ICER2								*(volatile uint32_t *)(NVIC_BASE + 0x88)


//-----------------------------
//Base addresses for AHB Peripherals
//-----------------------------

#define RCC_BASE								(Peripherals_BASE + 0x00021000UL)

//-----------------------------
//Base addresses for APB1 Peripherals
//-----------------------------


//-----------------------------
//Base addresses for APB2 Peripherals
//-----------------------------

//GPIO
//A,B fully included in LQFP48 Package
#define GPIOA_BASE								(Peripherals_BASE + 0x00010800UL)
#define GPIOB_BASE								(Peripherals_BASE + 0x00010C00UL)
//C,D Partial included in LQFP48 Package
#define GPIOC_BASE								(Peripherals_BASE + 0x00011000UL)
#define GPIOD_BASE								(Peripherals_BASE + 0x00011400UL)
//E not included in LQFP48 Package
#define GPIOE_BASE								(Peripherals_BASE + 0x00011800UL)
//--------

#define AFIO_BASE								(Peripherals_BASE + 0x00010000UL)
#define EXTI_BASE								(Peripherals_BASE + 0x00010400UL)


//------------------------------------------------------------------------

//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: GPIO
//-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
}GPIO_t;


//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: AFIO
//-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR[4];
	uint32_t RESERVED0;
	volatile uint32_t MAPR2;

}AFIO_t;


//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: EXTI
//-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_t;


//-*-*-*-*-*-*-*-*-*-*-*-
//Peripheral register: RCC
//-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;

}RCC_t;


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-* PERIPHERAL INSTANTS -*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
#define RCC						((RCC_t *)RCC_BASE)

#define GPIOA					((GPIO_t *)GPIOA_BASE)
#define GPIOB					((GPIO_t *)GPIOB_BASE)
#define GPIOC					((GPIO_t *)GPIOC_BASE)
#define GPIOD					((GPIO_t *)GPIOD_BASE)
#define GPIOE					((GPIO_t *)GPIOE_BASE)

#define AFIO					((AFIO_t *)AFIO_BASE)
#define EXTI					((EXTI_t *)EXTI_BASE)



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-* CLOCK ENABLE MACROS  -*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
#define RCC_GPIOA_CLK_EN()		(RCC->APB2ENR |= 1 << 2)
#define RCC_GPIOB_CLK_EN()		(RCC->APB2ENR |= 1 << 3)
#define RCC_GPIOC_CLK_EN()		(RCC->APB2ENR |= 1 << 4)
#define RCC_GPIOD_CLK_EN()		(RCC->APB2ENR |= 1 << 5)
#define RCC_GPIOE_CLK_EN()		(RCC->APB2ENR |= 1 << 6)

#define RCC_AFIO_CLK_EN()		(RCC->APB2ENR |= 1 << 0)




//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-* IVT  -*-*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#define EXTI0_IRQ	6
#define EXTI1_IRQ	7
#define EXTI2_IRQ	8
#define EXTI3_IRQ	9
#define EXTI4_IRQ	10
#define EXTI5_IRQ	23
#define EXTI6_IRQ	23
#define EXTI7_IRQ	23
#define EXTI8_IRQ	23
#define EXTI9_IRQ	23
#define EXTI10_IRQ	40
#define EXTI11_IRQ	40
#define EXTI12_IRQ	40
#define EXTI13_IRQ	40
#define EXTI14_IRQ	40
#define EXTI15_IRQ	40



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-* NVIC IRQ (en/dis)able MACROS -*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
#define	NVIC_IRQ6_EXTI0_ENABLE			(NVIC_ISER0 |= 1<<6)
#define	NVIC_IRQ7_EXTI1_ENABLE			(NVIC_ISER0 |= 1<<7)
#define	NVIC_IRQ8_EXTI2_ENABLE			(NVIC_ISER0 |= 1<<8)
#define	NVIC_IRQ9_EXTI3_ENABLE			(NVIC_ISER0 |= 1<<9)
#define	NVIC_IRQ10_EXTI4_ENABLE			(NVIC_ISER0 |= 1<<10)
#define	NVIC_IRQ23_EXTI9_5_ENABLE		(NVIC_ISER0 |= 1<<23)
#define	NVIC_IRQ40_EXTI10_15_ENABLE		(NVIC_ISER1 |= 1<<8)//40-32=8 position-bit


#define	NVIC_IRQ6_EXTI0_DISABLE			(NVIC_ICER0 |= 1<<6)
#define	NVIC_IRQ7_EXTI1_DISABLE			(NVIC_ICER0 |= 1<<7)
#define	NVIC_IRQ8_EXTI2_DISABLE			(NVIC_ICER0 |= 1<<8)
#define	NVIC_IRQ9_EXTI3_DISABLE			(NVIC_ICER0 |= 1<<9)
#define	NVIC_IRQ10_EXTI4_DISABLE		(NVIC_ICER0 |= 1<<10)
#define	NVIC_IRQ23_EXTI9_5_DISABLE		(NVIC_ICER0 |= 1<<23)
#define	NVIC_IRQ40_EXTI10_15_DISABLE	(NVIC_ICER1 |= 1<<8)//40-32=8 position-bit

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*-* GENERIC MACROS -*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#endif /* INC_STM32F103X6_H_ */
