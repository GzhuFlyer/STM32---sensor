#ifndef _CRC_H
#define _CRC_H

#include "stm32f4xx.h"


u16 get_crc(u8* ptr,u8 len);
void keyPackageTxFun(u8 i);
int checkRcvPackageFcn(void);
void infraredPackageTxFun(u8 i);
void doRcvResponResutl(u8 i);

#endif
