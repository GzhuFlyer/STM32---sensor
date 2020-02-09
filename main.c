/**
 ****************************************************   
 * @文件：main.c
 * @作者：方宗武
 * @版本：V2.2
 * @日期：2018年7月
 * @简介：该文件提供程序执行的入口，调用相关函数
 *****************************************************
 */
 
#include "delay.h"
#include "usart.h"
#include "exti.h"
#include "2Y0A21.h"
#include "ds18b20.h"
#include "TM1637.h"
#include "EC35AH.h"



int main(void)
{ 

	

	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200); 	//串口初始化 
	EXTIX_Init();       //初始化外部中断输入 
	Adc_Init();			//2Y0A21接口
	//KEY_Init();
	knob_Init();

	TM1637_Init();
	
	DS18B20_Init();
	
	knob_Init();
 

	while(1)
	{	

		
		sendInfraredResult();	//发送红外光栅测量结果
		
		
	//	nixieTubeShowTemperature();		//数码管上温度的显示
		
			knob_adjust();
		//	nixieTubeShowCtl();

			
		}
}












