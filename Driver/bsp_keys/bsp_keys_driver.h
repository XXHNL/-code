#ifndef _BSP_KEYS_DRIVER_H
#define _BSP_KEYS_DRIVER_H

#include "gd32f4xx.h"
#include <stdio.h>

//LED初始化属性结构体
typedef struct {
	rcu_periph_enum rcu;  
	uint32_t port;
	uint32_t pin;
}KEY_PARAM;

//灯枚举
typedef enum{
	KEY1 = 0,
	KEY2,
	KEY3,
	KEY4
}KEY;

//初始化驱动
void bsp_keys_driver_init(void);

//按键扫描
void bsp_keys_scan(void);

//按键按下事件回调
extern void bsp_key_press_callback(KEY key);

#endif