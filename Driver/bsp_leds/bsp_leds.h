#ifndef __BSP_LEDS_H_
#define __BSP_LEDS_H_

#include "gd32f4xx.h"

#define LED1	1
#define LED2	2
#define LED3	3
#define LED4	4


void led_init();
// ¿ªµÆ
void led_turn_on(uint8_t led_index) ;

// ¹ØµÆ
void led_turn_off(uint8_t led_index) ;

#endif