#ifndef _APP_H
#define _APP_H

#include "bsp_keys_driver.h"
#include "steering_drv.h"
#include <stdio.h> 
#include "bsp_usart0.h"
#include "bsp_usart1.h"
#include "oled.h"
#include <string.h>
#include "circular_queue.h"
#include "FreeRTOS.h"
#include "task.h"
#include "BH1750.h"
#include "DHT11.h"
#include "bsp_leds.h"
#include "ESP8266.h"
#include "stdbool.h"
#include "ctype.h"
#include "MQ2.h"
#include "bsp_buzzer.h"
/************** App_Input任务 **************/
void App_Input_init(void);

//任务
void App_Input_task(void * param);

void App_recev_task(void * param);

/************** App_Balance任务 **************/


void App_Control_init(void);

//任务
void App_Control_task(void * param);





/************** App_Show任务 OLED显示状态 **************/
void App_Show_init(void);

//任务
void App_Show_task(void * param);

#endif