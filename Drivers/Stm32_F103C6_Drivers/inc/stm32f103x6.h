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

#define FLASH_MEMORY						0x08000000
#define SYSTEM_MEMORY						0x1FFFF000
#define SRAM_MEMORY							0x20000000
#define PERIPHERALS							0x40000000
#define CORTEX_M3_INTERNAL_PEERIPHERALS		0xE0000000

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
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
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

#define AFIO					((AFIO_T *)AFIO_BASE)
#define EXTI					((EXTI_T *)EXTI_BASE)

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
//-*-*-*-*-* GENERIC MACROS -*-*-*-*-*-*-*
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#endif /* INC_STM32F103X6_H_ */
