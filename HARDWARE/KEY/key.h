#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY_PB12		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)  //PB12
#define KEY_PB13 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) 	//PB13 
#define KEY_PB14 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)  //PB14
#define KEY_PB15 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)	//PB15

#define KEY_PD8         GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)   //PD8


void KEY_Init(void);	//IO初始化


#endif
