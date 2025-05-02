#ifndef USART_H
#define USART_H

#include "gd32f4xx.h"

#include <stdio.h>


#define RX_MAX_LENGTH 1024

extern uint8_t RX_Buffer[RX_MAX_LENGTH];
extern uint32_t RX_Cnt;
extern uint8_t RX_Compelete;




void usart_init();


//===============================================================发送==============================

//发送单个字节
void usart_send_byte(uint8_t data);

//发送多个字节
void usart_send_data(uint8_t * data , uint8_t len);

//发送字符串
void usart_send_string(char * str);

//如果要想让printf生效，能直接输出数据和串口，必须重写这个函数，然后在函数内部调用自己的发送一个字节的函数
// int fputc(int ch, FILE *f);








#endif