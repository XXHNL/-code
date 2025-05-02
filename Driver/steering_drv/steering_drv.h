#ifndef _STEERING_DRV_H
#define _STEERING_DRV_H
#include "bsp_timer8.h"
//舵机初始化
void steering_drv_config(void);

//设置舵机角度
void steering_drv_set_angle(float angle);
#endif