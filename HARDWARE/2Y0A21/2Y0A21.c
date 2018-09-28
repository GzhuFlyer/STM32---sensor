/**
 ****************************************************   
 * @文件：2Y0A21.c
 * @作者：方宗武
 * @版本：V2.2
 * @日期：2018年7月
 * @简介：该文件用于驱动红外测距模块
 *****************************************************
 */

#include "2Y0A21.h" 
#include "crc.h"
#include "usart.h"

int infraredFlag = 0;		//定义一个全局变量，用于标志屏幕的熄灭与开启。0：代表熄灭；1：代表开启。
  
/**
  * @功能：  用于Adc通道引脚的初始化
  * @参数：  无
  * @返回值：无
  */
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
  //==============初始化PA0 IO口=============================//
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AN;   //模拟输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;			//通道0
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;	//不带上下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);					//初始化函数
	//===========================================================//

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //APB2总线上的外设ADC1时钟的复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	
	
	//==设置ADC的通用控制寄存器CCR，配置ADC输入时钟分频，模式为独立模式==========================//
	ADC_CommonInitStructure.ADC_DMAAccessMode=ADC_DMAAccessMode_Disabled;		//DMA失能
	ADC_CommonInitStructure.ADC_Mode=ADC_Mode_Independent;				//独立模式
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;			//预分频4分频。ADCCLK=PCLK2/4=84/4=21MHz,ADC时钟最好低于36MHz
	ADC_CommonInitStructure.ADC_TwoSamplingDelay=ADC_TwoSamplingDelay_5Cycles; //两个采样阶段之间的延迟5个时钟
	ADC_CommonInit(&ADC_CommonInitStructure);				//初始化CCR寄存器
	//=============================================================================================//
	
	//===初始化ADC1参数，设置ADC1的转换分辨率，转换方式，对齐方式，以及规则序列等相关信息。=======//
	//以下为单次转换，不需要使用到DMA，连续转换通道需要使用到DMA
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;		     //12位模式
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;	   //右对齐
	ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;  //禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_NbrOfConversion=1;  //存在一个转换，该转换在规则序列中
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;  //12位模式
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;						//失能非扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);										//进行ADC相关参数的初始化
	//=============================================================================================//
	
	ADC_Cmd(ADC1,ENABLE);  //开启ADC初始化	
}

/**
  * @功能：  获取ADC值
  * @参数：  ch（传递通道值，范围为ADC_Channel_0~ADC_Channel_15）
  * @返回值：在规则通道中获取ADC1转换的结果
  */
u16 Get_Adc(u8 ch)
{
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_480Cycles);		//设置ADC1的规则通道组，采样时间为480个周期
	ADC_SoftwareStartConv(ADC1);								//使能指定的ADC1的软件转换启动功能				
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));		//在AD转换中，检查ADC1的标志位是否被设置，并且等待设置完成	
	return ADC_GetConversionValue(ADC1);  

}

/**
  * @功能：  进行多次采样并计算出平均值
  * @参数：  采样的次数
  * @返回值：采样后的ADC平均值
  */
u16 Get_Adc_Average(u8 ch, u8 times)
{
	u32 temp_val = 0;
	u8 t;
	for(t=0; t<times;t++)
	{
			temp_val += Get_Adc(ch);
			delay_ms(5);
	}
	return temp_val/times;
}

/**
  * @功能：  将红外测距的监控结果实时反馈给安卓机
  * @参数：  无
  * @返回值：无
  * @注意：  当监测到有人靠近时候发送数据通知安卓机开屏，有人离开的时候发送数据通知安卓机息屏
  */
void sendInfraredResult(void)
{	
		u16 adcx;
		float temp;
	    adcx = Get_Adc_Average(ADC_Channel_0,20); //获取通道5的转换值，20次取平均，ADC采样后的原始值
		temp = (float)adcx * (3.3/4096);					  //获取计算后的带小数的实际电压值，比如3.1111
		delay_ms(250);
		if(infraredFlag == 0 && temp>(float)1.25 )		//在屏幕熄灭的情况下，检测到有人靠近，发送数据包通知安卓机开屏。
		{
			//printf("screen On\r\n");
			infraredPackageTxFun(0xaa);	
			doRcvResponResutl(0xaa);
			infraredFlag = 1;						//屏幕标志位置1表示开屏。
		}
		if(infraredFlag == 1 && temp <= (float)1.25 )		//在屏幕开启的情况下，检测到有人远离，发送数据包通知安卓机熄屏。
		{
			//printf("screen Off\r\n");
			infraredPackageTxFun(0xbb);	
			doRcvResponResutl(0xbb);
			infraredFlag = 0;						//屏幕标志位置0表示熄屏。
		}
}












