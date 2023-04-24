/*
 * keypad.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO.h"
#include <stdlib.h>
#include <stdio.h>

//keypad mapping
#define R0							GPIO_PIN_0
#define R1							GPIO_PIN_1
#define R2							GPIO_PIN_3
#define R3							GPIO_PIN_4
#define C0							GPIO_PIN_5
#define C1							GPIO_PIN_6
#define C2							GPIO_PIN_7
#define C3							GPIO_PIN_8

#define Keypad_PORT					GPIOB


//APIs
void keypad_init(void);
char keypad_get_char();



#endif /* INC_KEYPAD_H_ */
