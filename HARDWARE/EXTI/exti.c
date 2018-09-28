/**
 ****************************************************   
 * @�ļ���exti.c
 * @���ߣ�������
 * @�汾��V2.2
 * @���ڣ�2018��7��
 * @��飺���ļ����ڴ����ⲿ�ж��¼�
 * @ע�⣺��0x11~0x15�ֱ����ͬ5����ͬ�İ�����
 *****************************************************
 */
 
#include "exti.h"
#include "delay.h" 
#include "key.h"
#include "usart.h"
#include "crc.h"
#include "EC35AH.h"

/**
  * @���ܣ�  �� ����1 �������¼����д���
  * @������  ��
  * @����ֵ����
  */
void EXTI9_5_IRQHandler(void)
{
	delay_ms(10);	//����
	if(KEY_PD8==0)	 
	{				 
		keyPackageTxFun(0x11);
		doRcvResponResutl(0x11);
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line8);//���LINE8�ϵ��жϱ�־λ 	
}

/**
  * @���ܣ�  �� ����2~5 �������¼����д���
  * @������  ��
  * @����ֵ����
  */
void EXTI15_10_IRQHandler(void)
{
	delay_ms(30);	//����
	//============---->>   ����2   <-----===========================
	if(KEY_PB12==0)	 
	{		
		keyPackageTxFun(0x12);	
		doRcvResponResutl(0x12);
	    EXTI_ClearITPendingBit(EXTI_Line12);//���LINE12�ϵ��жϱ�־λ 
	}		 

	//============---->>   ����3   <-----===========================
	if(KEY_PB13==0)	 
	{		
		keyPackageTxFun(0x13);	
		doRcvResponResutl(0x13);		 
		EXTI_ClearITPendingBit(EXTI_Line13);//���LINE13�ϵ��жϱ�־λ 
	}

	//============---->>   ����4   <-----===========================
	if(KEY_PB14==0)	 
	{				 
		keyPackageTxFun(0x14);
		doRcvResponResutl(0x14);		 
		EXTI_ClearITPendingBit(EXTI_Line14);//���LINE14�ϵ��жϱ�־λ 
	}
	//============---->>   ����5   <-----===========================
	if(KEY_PB15==0)	 
	{			 
		keyPackageTxFun(0x15);
        doRcvResponResutl(0x15);		 
		EXTI_ClearITPendingBit(EXTI_Line15);//���LINE15�ϵ��жϱ�־λ
	}
//	//============---->>  ��ť    <-----===========================
//	if(knob_PC10==0)	 
//	{		
//		printf("knob_PC10\r\n");		 
//	    EXTI_ClearITPendingBit(EXTI_Line10);//���LINE10�ϵ��жϱ�־λ 
//	}
//	//============---->>  ��ť   <-----===========================
//	if(knob_PC11==0)	 
//	{		
//		printf("knob_PC11\r\n");		 
//	    EXTI_ClearITPendingBit(EXTI_Line11);//���LINE11�ϵ��жϱ�־λ 
//	}

	//============---->>  ��ť   <-----===========================
//	if(knob_PC12==0)	 
//	{				 
//		printf("knob_PC12\r\n");
//	    EXTI_ClearITPendingBit(EXTI_Line12);//���LINE12�ϵ��жϱ�־λ 
//	}	
//		printf("exti the interrupt from out \r\n");
}	


	   
/**
  * @���ܣ�  ���ⲿ�жϳ�����г�ʼ��
  * @������  ��
  * @����ֵ����
  * @ע�⣺  ��ʼ�������ŷֱ�Ϊ PD8�� PE12~15(������ŷֱ����ڰ������ⲿ����)
  * 		 PA0���ڲ��ģ����ⲿ�����¼�
  */
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	KEY_Init(); //������Ӧ��IO�ڳ�ʼ��
	//knob_Init();//��ת��������ӦIO�ڵĳ�ʼ��
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

	//��������
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource8); //PD8  ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);//PB12 ���ӵ��ж���12
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource13);//PB13 ���ӵ��ж���13
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);//PB14 ���ӵ��ж���14
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);//PB15 ���ӵ��ж���15	
	
//	//��ť����������
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource10);//PB10 ���ӵ��ж���10
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource11);//PB11 ���ӵ��ж���11
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource12);//PB12 ���ӵ��ж���12	
	
	/* ����EXTI_Line8,12,13,14,15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8 |EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
	//EXTI_InitStructure.EXTI_Line = EXTI_Line8 |EXTI_Line10|EXTI_Line11|EXTI_Line12 | EXTI_Line13 | EXTI_Line14 | EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel =  EXTI9_5_IRQn ;//�ⲿ�ж�0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж� 12,13,14,15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
	
	
}












