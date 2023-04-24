/*
 * keypad.c
 *
 *  Created on: Apr 24, 2023
 *      Author: Ahmed Adel Wafdy
 */

#include "keypad.h"
#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO.h"




int ROWS[] = {R0,R1,R2,R3};
int COLS[] = {C0,C1,C2,C3}	;
void keypad_init(void){
	//set upper to be input
	//Keypad_data_dir_PORT &= ~(1<<R0 | 1<<R1 | 1<<R2 | 1<< R3);
	//Keypad_data_dir_PORT |= (1<<C0 | 1<<C1 | 1<<C2 | 1<<C3);
	//Keypad_PORT = 0xFF;
	GPIO_PIN_CONFIG_T PinCfg ;
	PinCfg.GPIO_PIN_NUMBER = R0;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(Keypad_PORT, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = R1;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(Keypad_PORT, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = R2;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(Keypad_PORT, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = R3;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(Keypad_PORT, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = C0;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(Keypad_PORT, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = C1;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(Keypad_PORT, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = C2;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(Keypad_PORT, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = C3;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(Keypad_PORT, &PinCfg);
	MCAL_GPIO_WritePort(Keypad_PORT, 0xFF);
}
char keypad_get_char(){
	int i,j;
	for(i=0;i<4;i++){
		//Keypad_PORT |= (1<<COLS[0] | 1<<COLS[1] | 1<<COLS[2] | 1<<COLS[3]);
		//Keypad_PORT &= ~(1<<COLS[i]);
		MCAL_GPIO_WritePin(Keypad_PORT, COLS[0], GPIO_PIN_SET);
		MCAL_GPIO_WritePin(Keypad_PORT, COLS[1], GPIO_PIN_SET);
		MCAL_GPIO_WritePin(Keypad_PORT, COLS[2], GPIO_PIN_SET);
		MCAL_GPIO_WritePin(Keypad_PORT, COLS[3], GPIO_PIN_SET);
		MCAL_GPIO_WritePin(Keypad_PORT, COLS[i], GPIO_PIN_RESET);
		for(j=0;j<4;j++){
			if(MCAL_GPIO_ReadPin(Keypad_PORT, ROWS[j])==0){
				while(MCAL_GPIO_ReadPin(Keypad_PORT, ROWS[j])==0);
				switch(i){
					case 0:
					{
						if(j==0)return '7';
						else if(j==1)return '4';
						else if(j==2)return '1';
						else if(j==3)return '?';
						break;
					}
					case 1:
					{
						if(j==0)return '8';
						else if(j==1)return '5';
						else if(j==2)return '2';
						else if(j==3)return '0';
						break;
					}
					case 2:
					{
						if(j==0)return '9';
						else if(j==1)return '6';
						else if(j==2)return '3';
						else if(j==3)return '=';
						break;
					}
					case 3:
					{
						if(j==0)return '/';
						else if(j==1)return '*';
						else if(j==2)return '-';
						else if(j==3)return '+';
						break;
					}
				}
			}
		}
	}
	return 'A';
}
