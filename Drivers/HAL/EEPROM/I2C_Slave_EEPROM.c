/*
 * I2C_Slave_EEPROM.c
 *
 *  Created on: May 2, 2023
 *      Author: Ahmed Adel Wafdy
 */
#include "I2C_Slave_EEPROM.h"

void EEPROM_Init(void){
	//I2C Init=================>
	I2C_Config_t i2c_cfg;
	i2c_cfg.I2C_General_Call = I2C_General_Call_Enable;
	i2c_cfg.I2C_ACK_CR = I2C_ACK_CR_ACK_Enable;
	i2c_cfg.I2C_CLk_Speed = I2C_CLk_Speed_100KHz;
	i2c_cfg.I2C_Mode = I2C_Mode_I2C_Mode;
	i2c_cfg.P_IRQ_CallBack = NULL;
	i2c_cfg.I2C_Stretch_Mode = I2C_Stretch_Mode_Enable;

	MCAL_I2C_GPIO_Set_Pins(I2C1);
	MCAL_I2C_Init(I2C1, &i2c_cfg);
}
uint8_t EEPROM_Write_Nbyte(uint32_t Memory_address,uint8_t* bytes,uint8_t Data_length){
	uint8_t i=0;
	uint8_t buffer[256];
	buffer[0] = (uint8_t)(Memory_address>>8); //upper
	buffer[1] = (uint8_t)(Memory_address);//lower

	for(i=0;i<(Data_length+2);i++){
		buffer[i] = bytes[i-2];
	}

	MCAL_I2C_Master_Transmit(I2C1, EEPROM_Slave_Address, buffer, Data_length+2, With_Stop, START);
	return 0;
}
uint8_t EEPROM_Read_Byte(uint32_t address,uint8_t* dataOut,uint8_t datalen){
	unsigned char buffer[2];
	buffer[0] = (uint8_t)(address>>8); //upper
	buffer[1] = (uint8_t)(address);//lower
	MCAL_I2C_Master_Transmit(I2C1, EEPROM_Slave_Address, &buffer,2, Without_Stop, START);
	MCAL_I2C_Master_Receive(I2C1, EEPROM_Slave_Address, dataOut, datalen, With_Stop, Repeated_Start);
	return 0;

}
