/*
 * lcd.c
 *
 *  Created on: Apr 24, 2023
 *      Author: Ahmed Adel Wafdy
 */



#include "lcd.h"
#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO.h"
GPIO_PIN_CONFIG_T PinCfg ;
static void delay_ms(uint32_t delay)
{
	uint32_t i, j;
	for (i = 0; i < delay; i++)
	{
		for (j = 0; j < 255; j++);
	}
}

void LCD_GPIO_init() {
	PinCfg.GPIO_PIN_NUMBER = RS;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = RW;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = EN;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	//============================
	// SET THE NEXT 8 PINS AS INPUT
	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_0;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_1;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_2;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_3;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_4;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_5;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_6;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_7;
	PinCfg.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinCfg.GPIO_OUTPUT_SPEED = GPIO_SPEED_10MHz;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	//	DataDir_LCD_CTRL |= (1 << EN_SWITCH | 1 << RS_SWITCH | 1 << RW_SWITCH);
	MCAL_GPIO_WritePin(LCD_CTRL, EN, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RS, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RW, GPIO_PIN_RESET);

}


void lcd_is_busy(void)
{
	//LCD_DATA_dir_PORT &= ~(0xff<<DATA_shift);
	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_0;
	PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_1;
	PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_2;
	PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_3;
	PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_4;
	PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_5;
	PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_6;
	PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

	PinCfg.GPIO_PIN_NUMBER = GPIO_PIN_7;
	PinCfg.GPIO_MODE = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(LCD_CTRL, &PinCfg);

//	LCD_CTRL |= (1<<RW); //read
	MCAL_GPIO_WritePin(LCD_CTRL, RW, GPIO_PIN_SET);
	//LCD_CTRL  &= ~(1<<RS);
	MCAL_GPIO_WritePin(LCD_CTRL, RS, GPIO_PIN_RESET);

	lcd_kick();
	//LCD_DATA_dir_PORT = 0xff;

	//LCD_CTRL &= ~(1<<RW); //write
	MCAL_GPIO_WritePin(LCD_CTRL, RW, GPIO_PIN_RESET);
}
void lcd_kick(void)
{
	MCAL_GPIO_WritePin(LCD_CTRL, EN, GPIO_PIN_SET);
	delay_ms(50);
	MCAL_GPIO_WritePin(LCD_CTRL, EN, GPIO_PIN_RESET);

}
void lcd_send_command(unsigned char command)
{
	MCAL_GPIO_WritePort(LCD_CTRL, command);
	MCAL_GPIO_WritePin(LCD_CTRL, RW, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RS, GPIO_PIN_RESET);
	delay_ms(1);
	lcd_kick();
}
void lcd_init(void)
{
	delay_ms(20);
	LCD_GPIO_init();
	delay_ms(15);

	lcd_clear();
	lcd_send_command(LCD_8BIT_MODE_2_LINE);
	lcd_send_command(LCD_ENTERY_MODE_INC_SHIFT_OFF);
	lcd_send_command(LCD_BEGIN_AT_FIRST_RAW);
	lcd_send_command(LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON);
}
void lcd_send_char(unsigned char The_char)
{
	MCAL_GPIO_WritePort(LCD_CTRL, The_char);
	MCAL_GPIO_WritePin(LCD_CTRL, RW, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_CTRL, RS, GPIO_PIN_SET);
	delay_ms(1);
	lcd_kick();

}
void lcd_send_string(unsigned char* The_char)
{
	int count=0;
	while(*The_char>0){
		count++;
		lcd_send_char(*The_char++);
		if(count == 16){
			lcd_set_position(2,0);
		}
		else if( count == 32){
			lcd_clear();
			lcd_set_position(1,0);
			count = 0;
		}
	}

}
void lcd_set_position(unsigned char line ,unsigned char pos)
{
	if(line == 1){
		if(pos <16 && pos>=0){
			lcd_send_command(LCD_BEGIN_AT_FIRST_RAW+pos);
		}else if(line == 2){
			if(pos <16 && pos>=0){
				lcd_send_command(LCD_BEGIN_AT_SECOND_RAW+pos);
				}
		}
	}

}
void lcd_clear(void)
{
	lcd_send_command(LCD_CLEAR);
	}
void lcd_send_number(int number)
{
	char str[7];
	sprintf(str,"%d",number);
	lcd_send_string(str);

}
void lcd_send_real_number(float number)
{
		char str[16];
		char *tmpSign = (number < 0)?"-":"-";
		float tmpVal = (number < 0)?-number :number;

		int tmpInt1 = tmpVal;
		float tmpFrac = tmpVal -tmpInt1;
		int tmpInt2 = tmpFrac*10000;
		sprintf(str,"%s%d.%o4d",tmpSign,tmpInt1,tmpInt2);
		lcd_send_string(str);
}
