/**
 ****************************************************   
 * @�ļ���exti.c
 * @���ߣ�������
 * @�汾��V2.2
 * @���ڣ�2018��7��
 * @��飺���ļ�����������ת������
 *****************************************************
 */

#include "EC35AH.h"
#include "exti.h"
#include "delay.h" 
#include "key.h"
#include "usart.h"
#include "crc.h"
#include "EC35AH.h"

static int i = 0;
static int j = 0;
static int k = 0;

int old_status = 10;	
int new_status = 10;
int adjuest = 10;

//����ֵΪ5��16~32�ܹ�33������
extern uint16_t temp_set[33];
						  
u8 set_para = 0;

/*
** ���ܣ�  ��EC35AH��ת���������г�ʼ������
** ������  ��
** ����ֵ����
** ע�⣺  ��ʼ�������ŷֱ�Ϊ PC10,PC11,PC12
*/
void knob_Init(void)
{
	
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOC
} 
void knob_statues_init(void)
{
//============================================
			if(knob_PC10 == 0)
			{	
				//	printf("knob_PC10 press\r\n");
					//printf();
					old_status = 0;
			}
			if(knob_PC11 == 0)
			{	
				//	printf("knob_PC11 press\r\n");
				  old_status = 1;
			}
			if(knob_PC12 == 0)
			{	
				//	printf("knob_PC12 press\r\n");
				  old_status = 4;
			}
//===========================================
}

u8 knob_adjust(void)
{
		//=================================		
			if(knob_PC10 == 0 && i!=2)
			{	
					i = 1;
			}
			if(knob_PC10 == 1)
			{	
					i = 0;
			}
//==============================
			if(knob_PC11 == 0&& j!=2)
			{	
					j = 1;
			}
			if(knob_PC11 == 1)
			{	
					j = 0;
			}
//===============================
			if(knob_PC12 == 0&& k!=2)
			{	
					k = 1;
			}
			if(knob_PC12 == 1)
			{	
					k = 0;
			}
//==========================================================		
			if(knob_PC10 == 0 && i == 1)
			{	
				//	printf("knob_PC10 press\r\n");
					//printf();
					i = 2;
				new_status = 0;
				adjuest =  new_status-old_status;
				if(adjuest==3 || adjuest==1 || adjuest==-4 )
				{
					//	printf("contrarotate   left\r\n");
					set_para = 1;
					  nixieTubeShowCtl(set_para);
					//	return 1;
				}

				else if(adjuest==-1 || adjuest==3 || adjuest==4)
				{
						//	printf("contrariwise \r\n");
						//	nixieTubeShowCtl();
							
						//	return 2;
				}

				old_status = new_status;
			}
			if(knob_PC11 == 0 && j == 1)
			{	
				//	printf("knob_PC11 press\r\n");
				  j = 2;
				new_status = 1;
				adjuest =  new_status-old_status;
				if(adjuest==3 || adjuest==1 || adjuest==-4 )
				{
					  	set_para = 1;
					    nixieTubeShowCtl(set_para);
					//	printf("contrarotate  left\r\n");
					//	return 1;
				}

				else if(adjuest==-1 || adjuest==3 || adjuest==4)
				{
							//	printf("contrariwise\r\n");
								//return 2;
				}

				old_status = new_status;
			}
			if(knob_PC12 == 0&& k == 1)
			{	
				//	printf("knob_PC12 press\r\n");
				  k = 2;
				
				new_status = 4;
				adjuest =  new_status-old_status;
				if(adjuest==3 || adjuest==1 || adjuest==-4 )
				{
						set_para = 1;
					  nixieTubeShowCtl(set_para);
						//printf("contrarotate  left\r\n");
					//	return 1;
				}

				else if(adjuest==-1 || adjuest==3 || adjuest==4)
				{
						//printf("contrariwise\r\n");
						//return 2;
				}

				old_status = new_status;
			}
//==============================================
		//	return 0;
			

}











