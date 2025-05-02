#ifndef _BSP_KEYS_DRIVER_H
#define _BSP_KEYS_DRIVER_H

#include "gd32f4xx.h"
#include <stdio.h>

//LED��ʼ�����Խṹ��
typedef struct {
	rcu_periph_enum rcu;  
	uint32_t port;
	uint32_t pin;
}KEY_PARAM;

//��ö��
typedef enum{
	KEY1 = 0,
	KEY2,
	KEY3,
	KEY4
}KEY;

//��ʼ������
void bsp_keys_driver_init(void);

//����ɨ��
void bsp_keys_scan(void);

//���������¼��ص�
extern void bsp_key_press_callback(KEY key);

#endif