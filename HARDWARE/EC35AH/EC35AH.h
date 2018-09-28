#ifndef _EC35AH_H
#define _EC35AH_H

#include "stm32f4xx.h"
#define knob_PC10 	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)  //PC10
#define knob_PC11  	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)  //PC11
#define knob_PC12   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)  //PC12

void knob_Init(void);
u8 knob_adjust(void);

#endif
