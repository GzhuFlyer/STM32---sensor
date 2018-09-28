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
#ifndef TM1637_H
#define TM1637_H

#include "sys.h"

//#define TM1637_VCC           PBout(11)  
#define TM1637_CLK           PDout(4)  
#define TM1637_DIO           PDout(3)  
#define TM1637_READ_DIO      PDin(3) 

//================================================================================================
//IO方向设置
//#define TM1637_DIO_IN()     {GPIOC->CRL&=0XFFFF0FFF;GPIOC->CRL|=8<<12;}  
//#define TM1637_DIO_OUT()    {GPIOC->CRL&=0XFFFF0FFF;GPIOC->CRL|=3<<12;}
//***********在STM32F103上面，CRL会配置对应IO输出模式。
//***********在STM32F407上面，MODE，OSPEEDR等寄存器用来配置IO输出模式
//CRL的13~16位清零，操作GPIOC_4。设置CRL的13~16位为1000，操作GPIOC_4为上拉/下拉输入模式
//CRL的13~16位清零，操作GPIOC_4。设置CRL的13~16位为1000，操作GPIOC_4为复用功能输出模式，最大速度为50MHz
#define TM1637_DIO_IN()     PCin(4)
#define TM1637_DIO_OUT()    PCout(4)
//=====================================================================================================

/** 
  * @brief  Configuration Dots's Mode enumeration  
  */
typedef enum
{ 
	DulDot = 0x00,
  LowDot= 0x01,
  HighDot= 0x2,
}DisDotSetting;
typedef enum
{ 
	sty = 0x00,
  Jep = 0x01, 
}DisDotMode;

extern unsigned char NumDis[];
void TM1637_Delay_us(unsigned  int Nus); 
void TM1637_Start(void);
void TM1637_Ack(void);
void TM1637_Stop(void);
void TM1637_WriteByte(unsigned char oneByte);



void TM1637_Init(void);
void TM1637LED_Init(void);
 
//gzhuflyer
void TM1637_NixieTubeClose(void);
void TM1637_NixieTubeDisplay(u8* showNum, u8 Len);
void TM1637_NixieTubeDisplay1(int disAdd);
void nixieTubeShowTemperature(void);
void nixieTubeShowCtl(u8 set_para);


#endif
