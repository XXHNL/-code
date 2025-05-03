#ifndef __BSP_LEDS_H_
#define __BSP_LEDS_H_

#include "gd32f4xx.h"

#define LED1	1
#define LED2	2
#define LED5	3
#define LED4	4


void led_init();
// ??
void led_turn_on(uint8_t led_index) ;

// ??
void led_turn_off(uint8_t led_index) ;

// ??LED5??? (0-999?0????999???)
void led_set_brightness(uint16_t brightness);

#endif