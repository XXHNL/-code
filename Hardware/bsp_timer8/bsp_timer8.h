#ifndef _BSP_TIMER8_H
#define _BSP_TIMER8_H

#include "gd32f4xx.h"

//168000000
//周期20ms 
#define PRESCALER  (168-1)  //168000000/168 = 1000000hz   1000000*/50 20000
#define PERIOD     ((SystemCoreClock/(PRESCALER+1))/50-1)  //20ms周期

//timer8初始化
void bsp_timer8_config(void);

//设置占空比
void bsp_timer8_set_pulse_value(uint16_t value);

#endif