/**
 ****************************************************   
 * @�ļ���main.c
 * @���ߣ�������
 * @�汾��V2.2
 * @���ڣ�2018��7��
 * @��飺���ļ��ṩ����ִ�е���ڣ�������غ���
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

	

	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200); 	//���ڳ�ʼ�� 
	EXTIX_Init();       //��ʼ���ⲿ�ж����� 
	Adc_Init();			//2Y0A21�ӿ�
	//KEY_Init();
	knob_Init();

	TM1637_Init();
	
	DS18B20_Init();
	
	knob_Init();
 

	while(1)
	{	

		
		sendInfraredResult();	//���ͺ����դ�������
		
		
	//	nixieTubeShowTemperature();		//��������¶ȵ���ʾ
		
			knob_adjust();
		//	nixieTubeShowCtl();

			
		}
}












