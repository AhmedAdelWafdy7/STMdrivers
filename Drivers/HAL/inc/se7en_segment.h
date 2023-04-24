/*
 * se7en_segment.h
 *
 *  Created on: Apr 24, 2023
 *      Author: Ahmed Adel Wafdy
 */

#ifndef INC_SE7EN_SEGMENT_H_
#define INC_SE7EN_SEGMENT_H_

#include "stm32f103x6.h"
#include "Stm32_F103C6_GPIO.h"



/* ===== 7 Segments Macros Definitions ===== */
#define ZERO 	0x01
#define ONE 	0x79
#define TWO 	0x24
#define THREE 	0x30
#define FOUR 	0x4C
#define FIVE 	0x12
#define SIX 	0x02
#define SEVEN 	0x19
#define EIGHT 	0x00
#define NINE 	0x10

/* ============= APIs ============ */
void se7ment_init(void);

#endif /* INC_SE7EN_SEGMENT_H_ */
