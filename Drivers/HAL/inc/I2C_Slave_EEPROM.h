/*
 * I2C_Slave_EEPROM.h
 *
 *  Created on: May 2, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_I2C_SLAVE_EEPROM_H_
#define INC_I2C_SLAVE_EEPROM_H_

#include "Stm32_F103C6_I2C.h"

#define EEPROM_Slave_Address		0x2A

void EEPROM_Init(void);
uint8_t EEPROM_Write_Nbyte(uint32_t Memory_address,uint8_t* bytes,uint8_t Data_length);
uint8_t EEPROM_Read_Byte(uint32_t address,uint8_t* dataOut,uint8_t datalen);

#endif /* INC_I2C_SLAVE_EEPROM_H_ */
