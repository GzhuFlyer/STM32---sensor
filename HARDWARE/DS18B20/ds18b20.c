/**
 ****************************************************   
 * @文件：exti.c
 * @作者：方宗武
 * @版本：V2.2
 * @日期：2018年7月
 * @简介：该文件用于驱动DS18B20模块
 * @注意：该模块仅适用GPIOB~5引脚
 *****************************************************
 */
#include "ds18b20.h"
#include "delay.h"	


/**
  * @功能：  复位DS18B20
  * @参数：  无
  * @返回值：无
  */
void DS18B20_Rst(void)	   
{                 
  DS18B20_IO_OUT(); //设置PB5输出
  DS18B20_DQ_OUT=0; //拉低DQ
  delay_us(750);    //拉低750us
  DS18B20_DQ_OUT=1; //DQ=1 
	delay_us(15);     //15US
}
/**
  * @功能：  等待DS18B20的回应
  * @参数：  无
  * @返回值：返回1:未检测到DS18B20的存在
  *          返回0:存在
  */
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
	DS18B20_IO_IN();//设置PB5为输入
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
	};	 
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	if(retry>=240)return 1;	    
	return 0;
}

/**
  * @功能： 从DS18B20读取一个位
  * @参数：  无
  * @返回值： data,即读取到的数据为，1或者0
  */
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
  u8 data;
	DS18B20_IO_OUT();//SET PG11 OUTPUT
  DS18B20_DQ_OUT=0; 
	delay_us(2);
  DS18B20_DQ_OUT=1; 
	DS18B20_IO_IN();//SET PG11 INPUT
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
  else data=0;	 
  delay_us(50);           
  return data;
}

/**
  * @功能： 从DS18B20读取一个字节
  * @参数：  无
  * @返回值：读到的数据
  */
u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
    return dat;
}

/**
  * @功能： 写一个字节到DS18B20
  * @参数：  要写入的字节
  * @返回值：无
  */
void DS18B20_Write_Byte(u8 dat)     
 {             
    u8 j;
    u8 testb;
	  DS18B20_IO_OUT();//SET PG11 OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if (testb) 
        {
            DS18B20_DQ_OUT=0;// Write 1
            delay_us(2);                            
            DS18B20_DQ_OUT=1;
            delay_us(60);             
        }
        else 
        {
            DS18B20_DQ_OUT=0;// Write 0
            delay_us(60);             
            DS18B20_DQ_OUT=1;
            delay_us(2);                          
        }
    }
}
 
/**
  * @功能： 用于开始温度转换的驱动
  * @参数：  要写入的字节
  * @返回值：无
  */
void DS18B20_Start(void)// ds1820 start convert
{   						               
    DS18B20_Rst();	   
	  DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
} 

/**
  * @功能： 初始化DS18B20的IO口 DQ 同时检测DS的存在
  * @参数：  无
  * @返回值：返回1:不存在
  *          返回0:存在 
  */  	 
u8 DS18B20_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化
 
  DS18B20_Rst();
  return DS18B20_Check();
}  

/**
  * @功能：  从ds18b2获取温度值
  * @参数：  无
  * @返回值：温度值 （-550~1250） 
  * @注意：  温度值精度为0.1C
  */
short DS18B20_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
	  short tem;
    DS18B20_Start ();                    // ds1820 start convert
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	    
    TL=DS18B20_Read_Byte(); // LSB   
    TH=DS18B20_Read_Byte(); // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正	  	  
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(double)tem*0.625;//转换     
	if(temp)return tem; //返回温度值
	else return -tem;    
}
















