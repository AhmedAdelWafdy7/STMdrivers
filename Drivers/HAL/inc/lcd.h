/*
 * lcd.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#define F_CPU 1000000UL

#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO.h"
#include <stdlib.h>
#include <stdio.h>

#define DATA_shift					0
#define LCD_CTRL					GPIOA
#define RS							GPIO_PIN_8
#define RW							GPIO_PIN_9
#define EN							GPIO_PIN_10


//#define EIGHT_BIT_MODE
//#define FOUR_BIT_MODE

//commands

#define LCD_RS_PIN					0
#define LCD_RW_PIN					1
#define LCD_EN_PIN					2
#define LCD_RS_ENABLE				1
#define LCD_RS_DISABLE				0
#define READ_FROM_LCD				1
#define WRITE_TO_LCD				0
#define	LCD_ENABLE					1
#define LCD_DISABLE					0
#define FIRST_LINE					0
#define SECOND_LINE					1


#define LCD_CLEAR                                  0x01
#define LCD_RETURN_HOME                            0x02
#define LCD_ENTERY_MODE_DEC_SHIFT_OFF              0x04
#define LCD_ENTERY_MODE_DEC_SHIFT_ON               0x05
#define LCD_ENTERY_MODE_INC_SHIFT_OFF              0x06
#define LCD_ENTERY_MODE_INC_SHIFT_ON               0x07
#define LCD_CURSOR_MOVE_SHIFT_LEFT                 0x10
#define LCD_CURSOR_MOVE_SHIFT_RIGHT                0x14
#define LCD_DISPLAY_SHIFT_LEFT                     0x18
#define LCD_DISPLAY_SHIFT_RIGHT                    0x1C
#define LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF    0x0C
#define LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON     0x0D
#define LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_OFF     0x0E
#define LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON      0x0F
#define LCD_DISPLAY_OFF_CURSOR_OFF                 0x08
#define LCD_8BIT_MODE_2_LINE                       0x38
#define LCD_4BIT_MODE_2_LINE                       0x28
#define LCD_BEGIN_AT_FIRST_RAW				       0x80
#define LCD_BEGIN_AT_SECOND_RAW				       0xC0



//API's
void lcd_is_busy(void);
void lcd_kick(void);
void lcd_send_command(unsigned char command);
void lcd_init(void);
void lcd_send_char(unsigned char The_char);
void lcd_send_string(unsigned char *The_char);
void lcd_set_position(unsigned char line ,unsigned char pos);
void lcd_clear(void);
void lcd_send_number(int number);
void lcd_send_real_number(float number);






#endif /* INC_LCD_H_ */
