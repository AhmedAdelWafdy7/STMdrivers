/*
 * stm32f103x6.h
 *
 *  Created on: Apr 19, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_STM32F103X6_H_
#define INC_STM32F103X6_H_


//-----------------------------
//INCLUDES
//-----------------------------
#include <stdio.h>
#include <stdint.h>

//-----------------------------
//-----------------------------
//BASE ADDRESSES FOR MEMORIES
//-----------------------------
//-----------------------------

#define FLASH_MEMORY						0x08000000UL
#define SYSTEM_MEMORY						0x1FFFF000UL
#define SRAM_MEMORY							0x20000000UL
#define PERIPHERALS							0x40000000UL
#define CORTEX_M3_INTERNAL_PEERIPHERALS		0xE0000000UL
//NVIC Register map
#define NVIC_BASE							0xE000E100UL
#define NVIC_ISER0							*(volatile uint32_t*)(NVIC_BASE + 0x000)
#define NVIC_ISER1							*(volatile uint32_t*)(NVIC_BASE + 0x004)
#define NVIC_ISER2							*(volatile uint32_t*)(NVIC_BASE + 0x008)
#define NVIC_ICER0							*(volatile uint32_t*)(NVIC_BASE + 0x080)
#define NVIC_ICER1							*(volatile uint32_t*)(NVIC_BASE + 0x084)
#define NVIC_ICER2							*(volatile uint32_t*)(NVIC_BASE + 0x088)

//-----------------------------
//-----------------------------
//BASE ADDRESSES FOR BUS PERIPHERALS
//-----------------------------
//-----------------------------

//-----------------------------
//BASE ADDRESSES FOR AHB PERIPHERALS
//-----------------------------
#define RCC_BASE							0x40021000

//-----------------------------
//BASE ADDRESSES FOR APB2 PERIPHERALS
//-----------------------------
#define AFIO_BASE							0x40010000
#define EXTI_BASE							0x40010400
//GPIO
#define GPIOA_BASE							0x40010800
#define GPIOB_BASE							0x40010C00
#define GPIOC_BASE							0x40011000
#define GPIOD_BASE							0x40011400
#define GPIOE_BASE							0x40011800


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-* PERIPHERAL REGISTARS -*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//RCC
typedef struct{
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


//GPIO
typedef struct{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
}GPIO_t;

//AFIO
typedef struct{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR[4];
	uint32_t RESERVED;
	volatile uint32_t MAPR2;
}AFIO_t;

//EXTI
typedef struct{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_t;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-* PERIPHERAL INSTANTS -*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*=
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
#define	RCC_GPIOA_CLK_EN()		(RCC->APB2ENR |= 1<<2)
#define	RCC_GPIOB_CLK_EN()		(RCC->APB2ENR |= 1<<3)
#define	RCC_GPIOC_CLK_EN()		(RCC->APB2ENR |= 1<<4)
#define	RCC_GPIOD_CLK_EN()		(RCC->APB2ENR |= 1<<5)
#define	RCC_GPIOE_CLK_EN()		(RCC->APB2ENR |= 1<<6)
#define	RCC_AFIO_CLK_EN()		(RCC->APB2ENR |= 1<<0)

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-* IVT  -*-*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

//EXTI
#define	EXTI0_IRQHandler          			/* EXTI Line0 interrupt                             */
#define	EXTI1_IRQHandler          			/* EXTI Line1 interrupt                             */
#define	EXTI2_IRQHandler          			/* EXTI Line2 interrupt                             */
#define	EXTI3_IRQHandler          			/* EXTI Line3 interrupt                             */
#define	EXTI4_IRQHandler          			/* EXTI Line4 interrupt                             */

#define	EXTI9_5_IRQHandler        			/* EXTI Line[9:5] interrupts                        */

#define	EXTI15_10_IRQHandler      			/* EXTI Line[15:10] interrupts                      */

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
