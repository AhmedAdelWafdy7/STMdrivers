/*
 * Stm32_F103C6_I2C.h
 *
 *  Created on: May 1, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_STM32_F103C6_I2C_H_
#define INC_STM32_F103C6_I2C_H_

//-----------------------------
//INCLUDES
//-----------------------------
#include "Stm32_F103C6_RCC.h"
#include "Stm32_F103C6_GPIO.h"
#include "stm32f103x6.h"

//-----------------------------
//USER TYPE DEFINITIONs
//-----------------------------

typedef struct{
	uint32_t Enable_Dual_Address;
	uint8_t Slave_First_Address;
	uint8_t Slave_Second_Address;
	uint32_t Slave_Address_Mode;
}I2C_Slave_address_t;

typedef enum{
	I2C_EV_STOP,
	I2C_EV_ADD_MATCHED,
	I2C_EV_DATA_REQ,
	I2C_EV_DATA_RCV
}Slave_State_t;

typedef struct{
	uint32_t I2C_Mode;// specifies I2C MODE Master/Slave
		// This parameter must be set based on @ref I2C_Mode_DEFINE

	uint32_t I2C_Master_Mode;// specifies I2C MODE Master transmitter or receiver
		// This parameter must be set based on @ref I2C_Master_Mode_DEFINE

	uint32_t I2C_CLk_Speed;// specifies I2C Clock Speed
		// This parameter must be set based on @ref I2C_CLk_Speed_DEFINE

	uint32_t I2C_ACK_CR;// specifies I2C Acknowledge Control
		// This parameter must be set based on @ref I2C_ACK_CR_DEFINE

	uint32_t I2C_General_Call;// specifies I2C General Call Enable/Disable
		// This parameter must be set based on @ref I2C_General_Call_DEFINE

	uint32_t I2C_Stretch_Mode;// specifies I2C Stretch Mode Enable/Disable
		// This parameter must be set based on @ref I2C_Stretch_Mode_DEFINE

	I2C_Slave_address_t I2C_Slave_address; //Slave Address information

	void (* P_IRQ_CallBack)(Slave_State_t Slave_State);	//set the C Function which will be called once IRQ Happens

 }I2C_Config_t;

//-----------------------------
//MACROS CONFIGURATION REFERENCES
//-----------------------------

 // @ref I2C_Mode_DEFINE
 /*
 ==>Bit 1 SMBUS: SMBus mode
0: I2C mode
1: SMBus mode*/
#define I2C_Mode_I2C_Mode						(uint32_t)(0)
#define I2C_Mode_SMBUS_Mode						(uint32_t)(1<<1)

 // @ref I2C_Master_Mode_DEFINE
/*Bit 15 F/S: I2C master mode selection
0: Sm mode I2C
1: Fm mode I2C*/
#define I2C_Master_Mode_SM						(uint32_t)(0)
#define I2C_Master_Mode_FM						(uint32_t)(1<<15)

 // @ref I2C_CLk_Speed_DEFINE
/*fPCLK1 must be at least 2 MHz to achieve Sm mode I²C frequencies. It must be at least 4
MHz to achieve Fm mode I²C frequencies. It must be a multiple of 10MHz to reach the
400 kHz maximum I²C Fm mode clock.*/
#define I2C_CLk_Speed_50KHz						(50000U)
#define I2C_CLk_Speed_100KHz					(100000U)

 // @ref I2C_ACK_CR_DEFINE
 /*Bit 10 ACK: Acknowledge enable
This bit is set and cleared by software and cleared by hardware when PE=0.
0: No acknowledge returned
1: Acknowledge returned after a byte is received (matched address or data)*/
#define I2C_ACK_CR_ACK_Disable						(uint32_t)(0)
#define I2C_ACK_CR_ACK_Enable						(uint32_t)(1<<10)

 // @ref I2C_General_Call_DEFINE
/*Bit 6 ENGC: General call enable
0: General call disabled. Address 00h is NACKed.
1: General call enabled. Address 00h is ACKed.*/
#define I2C_General_Call_Disable					(uint32_t)(0)
#define I2C_General_Call_Enable						(uint32_t)(1<<6)

 // @ref I2C_Stretch_Mode_DEFINE
 /*Bit 7 NOSTRETCH: Clock stretching disable (Slave mode)
This bit is used to disable clock stretching in slave mode when ADDR or BTF flag is set, until
it is reset by software.
0: Clock stretching enabled
1: Clock stretching disabled*/
#define I2C_Stretch_Mode_Disable					(uint32_t)(0)
#define I2C_Stretch_Mode_Enable						(uint32_t)(1<<7)

 //@ref I2C_SLAVE_ADDRESS_MODE_DEFINE
 /*Bit 15 ADDMODE Addressing mode (slave mode)
0: 7-bit slave address (10-bit address not acknowledged)
1: 10-bit slave address (7-bit address not acknowledged)*/
#define I2C_SLAVE_ADDRESS_MODE_7_BIT			(uint32_t)(0)
#define I2C_SLAVE_ADDRESS_MODE_10_BIT			(uint32_t)(1<<15)

 /*
 *============================================
 *============================================
 *
 */

 //I2C Mode Selection
#define I2C_Master_Mode
//#define I2C_Slave_Mode

 typedef enum
 {
 	RESET,
 	SET
 }I2C_Flagstatus;


 typedef enum
 {
 	BUS_Busy,
 	SB,
 	ADDR,
 	TXE,
 	RXNE,
 	Master_Transmitter_Event = ((uint32_t)(0x00070080))
 }Status;


 typedef enum
 {
 	With_Stop,
 	Without_Stop
 }STOP_Condition;


 typedef enum
 {
 	START,
 	Repeated_Start
 }START_Condition;


 typedef enum
 {
 	Disable,
 	Enable
 }Functional_State;


 typedef enum
 {
 	Transmitter,
 	Receiver
 }I2C_Direction;
 /*
 *============================================
 *============================================
 * APIs Supported by "MCAL I2C DRIVER"
 *============================================
 *============================================
 *
 */
 void MCAL_I2C_Init(I2C_t *I2Cx, I2C_Config_t *Config);
 void MCAL_I2C_DeInit(I2C_t *I2Cx);

 void MCAL_I2C_GPIO_Set_Pins(I2C_t *I2Cx);

 //Matser with polling
 void MCAL_I2C_Master_Transmit(I2C_t *I2Cx,uint16_t Device_Address, uint8_t *Data, uint8_t Data_Length, STOP_Condition Stop, START_Condition Start);
 void MCAL_I2C_Master_Receive(I2C_t *I2Cx, uint16_t Device_Address, uint8_t *Data, uint8_t Data_Length, STOP_Condition Stop, START_Condition Start);


 //Slave Interrupt
 void MCAL_I2C_Slave_Transmit(I2C_t *I2Cx, uint8_t pTxBuffer);
 uint8_t MCAL_I2C_Slave_Receive(I2C_t *I2Cx);

 //Generic
 void I2C_Generate_Start(I2C_t *I2Cx,START_Condition Start, Functional_State State);
 void I2C_Send_Address(I2C_t *I2Cx, uint16_t Device_Address,I2C_Direction Direction);
 void I2C_Stop(I2C_t *I2Cx, Functional_State State);
 void I2C_ACKConfig(I2C_t *I2Cx, Functional_State State);

 I2C_Flagstatus I2C_Get_FlagStatus(I2C_t *I2Cx, Status Flag);

#endif /* INC_STM32_F103C6_I2C_H_ */
