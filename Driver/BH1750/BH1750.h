#ifndef	__BH1750_H
#define	__BH1750_H

#include "gd32f4xx.h"
#include "systick.h"
#include "bsp_usart0.h"
#include "bsp_iic_soft.h"

//BH1750初始化
void BH1750_Init(void);

//BH1750读取光照强度
float BH1750_Read(void);



#endif