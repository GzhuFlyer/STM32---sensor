#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY_PB12		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)  //PB12
#define KEY_PB13 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) 	//PB13 
#define KEY_PB14 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)  //PB14
#define KEY_PB15 	    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)	//PB15

#define KEY_PD8         GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)   //PD8


void KEY_Init(void);	//IO��ʼ��


#endif
