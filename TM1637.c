/******************************Copyright (c)***********************************
*                Jiangsu Zhihai Electronic Technology Co., Ltd.
*                      Research & Development Department
*
*													www.smarthealth-tech.com
*
-------------------------------------------------------------------------------
* @file    TM1637.h
* @author  GU DONGDONG
* @date    2015-11-25  
*******************************************************************************/
#include "TM1637.h"
#include "delay.h"
#include "usart.h"
#include "ds18b20.h"
#include "EC35AH.h"

/** Write multiple bits in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 */
unsigned char temp=0xCF;//显示跳动的冒号
unsigned char NumDis[25]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0X6F,\
                          0x77,0x7C,0x39,0x5E,0x79,0x71,\
                          0x08,0x48,0x49,\
                          0x76,0x38,0x0E,0x50,0x37,0xff};//0123456789ABCDEF一二三HLJRS
u8 num[7] = {0}; //数码管数值以及亮度的改变控制参数
u8 luminance[9] = {0x80,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F};

//步进值为5，16~32总共33个数。
uint16_t temp_set[33]={160,165,170,175,180,185,190,195,200,205,210,215,\
								       220,225,230,235,240,245,250,255,260,265,270,275,\
								     	 280,285,290,295,300,305,310,315,320};
						  
u8 temp_value = 0;
									
void TM1637_Delay_us(unsigned  int Nus)
{
 for(;Nus>0;Nus--)
	{
	  __nop();
	}
}

void TM1637_Start(void)
{
	TM1637_DIO_OUT(); //??
  TM1637_CLK = 1;
	TM1637_DIO = 1;
	TM1637_Delay_us(5);
	TM1637_DIO = 0;
}
void TM1637_Ack(void)
{
	TM1637_DIO_IN(); //?
  TM1637_CLK = 0;
	TM1637_Delay_us(5);
	//while(TM1637_READ_DIO);
	TM1637_CLK = 1;
	TM1637_Delay_us(2);
	TM1637_CLK = 0;
}
void TM1637_Stop(void)
{
	TM1637_DIO_OUT(); //??
	TM1637_CLK = 0;
	TM1637_Delay_us(2);
	TM1637_DIO = 0;
	TM1637_Delay_us(2);
	TM1637_CLK = 1;
	TM1637_Delay_us(2);
	TM1637_DIO = 1;
}
void TM1637_WriteByte(unsigned char oneByte)
{
 unsigned char i;
	TM1637_DIO_OUT(); 
	for(i=0;i<8;i++)
	{
	 TM1637_CLK = 0;
		if(oneByte&0x01)
		{
			TM1637_DIO = 1;
		}
		else
		{
			TM1637_DIO = 0;
		}
		TM1637_Delay_us(3);
		oneByte=oneByte>>1;
		TM1637_CLK = 1;
		TM1637_Delay_us(3);
	}
}





//程序将四个数码管从0-f进行显示
void TM1637_NixieTubeDisplay1(int disAddr)//
{
	int i;
	TM1637_Start();
	TM1637_WriteByte(0x40);//0x40则采用显示地址自加1模式(Command1)
	TM1637_Ack();
	TM1637_Stop();

	TM1637_Start();
	TM1637_WriteByte(0xC0);//0X00地址开始显示(Command2)
	TM1637_Ack();

	for(i=0;i<4;i++)
	{
		TM1637_WriteByte(NumDis[disAddr]);//0X00地址开始显示 (Data1)
		TM1637_Ack();
	}

		TM1637_WriteByte(0xFF);//0X00地址开始显示 (Data1)
		TM1637_Ack();
		TM1637_WriteByte(0xFF);//0X00地址开始显示 (Data1)
		TM1637_Ack();

	TM1637_Stop();

	TM1637_Start();
	TM1637_WriteByte(0x8f);
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Delay_us(5);
}

//程序将所有传入所有数码设置参数
void TM1637_NixieTubeDisplay(u8* showNum, u8 Len)
{
	int i;
	static int j = 1;
	if(j==0x80)	j=1;
	TM1637_Start();
	TM1637_WriteByte(0x40);//0x40则采用显示地址自加1模式(Command1)
	TM1637_Ack();
	TM1637_Stop();

	TM1637_Start();
	TM1637_WriteByte(0xC0);//0X00地址开始显示(Command2)
	TM1637_Ack();

	
	for(i=0;i<Len-1;i++)
	{
		
		if(i<4)	  //数码管的最高位控制小数点的量灭，此处加上0x80让数码管点亮
			TM1637_WriteByte(NumDis[showNum[i]]+0x80);//0X00地址开始显示 (Data1)
		else
			TM1637_WriteByte(NumDis[showNum[i]]);//0X00地址开始显示 (Data1)
		TM1637_Ack();
	}
	
	TM1637_Stop();

	TM1637_Start();
	TM1637_WriteByte(showNum[i]);
	TM1637_Ack();
	TM1637_Stop();
	TM1637_Delay_us(5);
}

void TM1637_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);		
}
 
void TM1637_NixieTubeClose(void)
{
	TM1637_Start();//启动
	TM1637_WriteByte(0x80);//写一个字节,用于关闭显示
	TM1637_Ack();//响应
	TM1637_Stop();//停止	
}

/**
  * @功能：  获取温度传感器的值并且实时显示出来。
  * @参数：  无
  * @返回值：无
  */
void nixieTubeShowTemperature(void)
{
	//温度显示数据的初始化
	u8 t=0;		
	short temperature; 
		
	int a = 0;
	static int nixieTubeFlag = 0;
	static int k=4;
	
		if(t%10==0)//每100ms读取一次
		{									  
			temperature=DS18B20_Get_Temp();	
			if(temperature<0)
			{
				temperature=-temperature;					//转为正数
			}			
		}
		//delay_ms(10);
		t++;
//=================================================================
		num[0] = (temperature/100);
		num[1] = ((temperature%100)/10);
		num[2] = (temperature%10);
		num[3] = 0x0C;		
		num[4] = 24;
		num[5] = 24;
		num[6] = luminance[5];		//传入参数为K的话可以实现呼吸灯的效果,传入常量0~9可以固定控制亮度。
		
		if(k<8 && nixieTubeFlag ==0)	//k大于或者等于8的时候，自加
		{
			k++;
			
		}else
		{
				k--;
				nixieTubeFlag = 1;
				if(k==1)
					nixieTubeFlag = 0;	
		}

		if(a>15)// 
			a = 0;
		TM1637_NixieTubeDisplay(num,7);
		delay_ms(200);
}


/**
  * @功能：  获取温度传感器的值并且实时显示出来。
  * @参数：  无
  * @返回值：无
  */
void nixieTubeShowCtl(u8 set_para)
{
	//温度显示数据的初始化
	
		
		if(set_para == 1)
		{
					temp_value++;
		}
		if(temp_value==33)
			temp_value=32;
//=================================================================
		num[0] = (temp_set[temp_value]/100);
		num[1] = (temp_set[temp_value]%100/10);
		num[2] = (temp_set[temp_value]%100%10);
		num[3] = 0x0C;		
		num[4] = 24;
		num[5] = 24;
		num[6] = luminance[5];	
		//printf("num[0] is %d\r\n",num[0]);
		//printf("num[1] is %d\r\n",num[1]);
		//printf("num[2] is %d\r\n",num[2]);

		TM1637_NixieTubeDisplay(num,7);
		delay_ms(200);
}





