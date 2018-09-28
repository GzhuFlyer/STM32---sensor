/**
 ****************************************************   
 * @文件：exti.c
 * @作者：方宗武
 * @版本：V2.2
 * @日期：2018年7月
 * @简介：该文件用于处理外部中断事件
 * @注意：用0x11~0x15分别代表不同5个不同的按键。
 *****************************************************
 */
 
#include "exti.h"
#include "delay.h" 
#include "key.h"
#include "usart.h"
#include "crc.h"
#include "EC35AH.h"

/**
  * @功能：  对 按键1 的摁下事件进行处理
  * @参数：  无
  * @返回值：无
  */
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(KEY_PD8==0)	 
	{				 
		keyPackageTxFun(0x11);
		doRcvResponResutl(0x11);
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line8);//清除LINE8上的中断标志位 	
}

/**
  * @功能：  对 按键2~5 的摁下事件进行处理
  * @参数：  无
  * @返回值：无
  */
void EXTI15_10_IRQHandler(void)
{
	delay_ms(30);	//消抖
	//============---->>   按键2   <-----===========================
	if(KEY_PB12==0)	 
	{		
		keyPackageTxFun(0x12);	
		doRcvResponResutl(0x12);
	    EXTI_ClearITPendingBit(EXTI_Line12);//清除LINE12上的中断标志位 
	}		 

	//============---->>   按键3   <-----===========================
	if(KEY_PB13==0)	 
	{		
		keyPackageTxFun(0x13);	
		doRcvResponResutl(0x13);		 
		EXTI_ClearITPendingBit(EXTI_Line13);//清除LINE13上的中断标志位 
	}

	//============---->>   按键4   <-----===========================
	if(KEY_PB14==0)	 
	{				 
		keyPackageTxFun(0x14);
		doRcvResponResutl(0x14);		 
		EXTI_ClearITPendingBit(EXTI_Line14);//清除LINE14上的中断标志位 
	}
	//============---->>   按键5   <-----===========================
	if(KEY_PB15==0)	 
	{			 
		keyPackageTxFun(0x15);
        doRcvResponResutl(0x15);		 
		EXTI_ClearITPendingBit(EXTI_Line15);//清除LINE15上的中断标志位
	}
//	//============---->>  旋钮    <-----===========================
//	if(knob_PC10==0)	 
//	{		
//		printf("knob_PC10\r\n");		 
//	    EXTI_ClearITPendingBit(EXTI_Line10);//清除LINE10上的中断标志位 
//	}
//	//============---->>  旋钮   <-----===========================
//	if(knob_PC11==0)	 
//	{		
//		printf("knob_PC11\r\n");		 
//	    EXTI_ClearITPendingBit(EXTI_Line11);//清除LINE11上的中断标志位 
//	}

	//============---->>  旋钮   <-----===========================
//	if(knob_PC12==0)	 
//	{				 
//		printf("knob_PC12\r\n");
//	    EXTI_ClearITPendingBit(EXTI_Line12);//清除LINE12上的中断标志位 
//	}	
//		printf("exti the interrupt from out \r\n");
}	


	   
/**
  * @功能：  对外部中断程序进行初始化
  * @参数：  无
  * @返回值：无
  * @注意：  初始化的引脚分别为 PD8， PE12~15(五个引脚分别用于按键的外部触发)
  * 		 PA0用于测距模块的外部触发事件
  */
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	KEY_Init(); //按键对应的IO口初始化
	//knob_Init();//旋转编码器对应IO口的初始化
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	//按键部分
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource8); //PD8  连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);//PB12 连接到中断线12
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13);//PB13 连接到中断线13
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);//PB14 连接到中断线14
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);//PB15 连接到中断线15	
	
//	//旋钮编码器部分
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource10);//PB10 连接到中断线10
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource11);//PB11 连接到中断线11
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource12);//PB12 连接到中断线12	
	
	/* 配置EXTI_Line8,12,13,14,15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8 |EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
	//EXTI_InitStructure.EXTI_Line = EXTI_Line8 |EXTI_Line10|EXTI_Line11|EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn ;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断 12,13,14,15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
	
}












