/**
 ****************************************************   
 * @�ļ���2Y0A21.c
 * @���ߣ�������
 * @�汾��V2.2
 * @���ڣ�2018��7��
 * @��飺���ļ���������������ģ��
 *****************************************************
 */

#include "2Y0A21.h"
#include "crc.h"
#include "usart.h"

int infraredFlag = 0; //����һ��ȫ�ֱ��������ڱ�־��Ļ��Ϩ���뿪����0������Ϩ��1������������

/**
  * @���ܣ�  ����Adcͨ�����ŵĳ�ʼ��
  * @������  ��
  * @����ֵ����
  */
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  //ʹ��ADC1ʱ��

	//==============��ʼ��PA0 IO��=============================//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	 //ģ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		 //ͨ��0
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 //��ʼ������
	//===========================================================//

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);  //APB2�����ϵ�����ADC1ʱ�ӵĸ�λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE); //��λ����

	//==����ADC��ͨ�ÿ��ƼĴ���CCR������ADC����ʱ�ӷ�Ƶ��ģʽΪ����ģʽ==========================//
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		 //DMAʧ��
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					 //����ģʽ
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					 //Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21MHz,ADCʱ����õ���36MHz
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //���������׶�֮����ӳ�5��ʱ��
	ADC_CommonInit(&ADC_CommonInitStructure);									 //��ʼ��CCR�Ĵ���
	//=============================================================================================//

	//===��ʼ��ADC1����������ADC1��ת���ֱ��ʣ�ת����ʽ�����뷽ʽ���Լ��������е������Ϣ��=======//
	//����Ϊ����ת��������Ҫʹ�õ�DMA������ת��ͨ����Ҫʹ�õ�DMA
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;							//12λģʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;						//�Ҷ���
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; //��ֹ������⣬ʹ����������
	ADC_InitStructure.ADC_NbrOfConversion = 1;									//����һ��ת������ת���ڹ���������
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;						//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;								//ʧ�ܷ�ɨ��ģʽ
	ADC_Init(ADC1, &ADC_InitStructure);											//����ADC��ز����ĳ�ʼ��
	//=============================================================================================//

	ADC_Cmd(ADC1, ENABLE); //����ADC��ʼ��
}

/**
  * @���ܣ�  ��ȡADCֵ
  * @������  ch������ͨ��ֵ����ΧΪADC_Channel_0~ADC_Channel_15��
  * @����ֵ���ڹ���ͨ���л�ȡADC1ת���Ľ��
  */
u16 Get_Adc(u8 ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles); //����ADC1�Ĺ���ͨ���飬����ʱ��Ϊ480������
	ADC_SoftwareStartConv(ADC1);									 //ʹ��ָ����ADC1������ת����������
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		; //��ADת���У����ADC1�ı�־λ�Ƿ����ã����ҵȴ��������
	return ADC_GetConversionValue(ADC1);
}

/**
  * @���ܣ�  ���ж�β����������ƽ��ֵ
  * @������  �����Ĵ���
  * @����ֵ���������ADCƽ��ֵ
  */
u16 Get_Adc_Average(u8 ch, u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for (t = 0; t < times; t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val / times;
}

/**
  * @���ܣ�  ��������ļ�ؽ��ʵʱ��������׿��
  * @������  ��
  * @����ֵ����
  * @ע�⣺  ����⵽���˿���ʱ��������֪ͨ��׿�������������뿪��ʱ��������֪ͨ��׿��Ϣ��
  */
void sendInfraredResult(void)
{
	u16 adcx;
	float temp;
	adcx = Get_Adc_Average(ADC_Channel_0, 20); //��ȡͨ��5��ת��ֵ��20��ȡƽ����ADC�������ԭʼֵ
	temp = (float)adcx * (3.3 / 4096);		   //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
	delay_ms(250);
	if (infraredFlag == 0 && temp > (float)1.25) //����ĻϨ�������£���⵽���˿������������ݰ�֪ͨ��׿��������
	{
		//printf("screen On\r\n");
		infraredPackageTxFun(0xaa);
		doRcvResponResutl(0xaa);
		infraredFlag = 1; //��Ļ��־λ��1��ʾ������
	}
	if (infraredFlag == 1 && temp <= (float)1.25) //����Ļ����������£���⵽����Զ�룬�������ݰ�֪ͨ��׿��Ϩ����
	{
		//printf("screen Off\r\n");
		infraredPackageTxFun(0xbb);
		doRcvResponResutl(0xbb);
		infraredFlag = 0; //��Ļ��־λ��0��ʾϨ����
	}
}
