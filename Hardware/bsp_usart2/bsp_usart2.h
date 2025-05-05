#ifndef USART2_H
#define USART2_H

#include "gd32f4xx.h"

#include <stdio.h>


#define RX2_MAX_LENGTH 200
#define USART2_RXBUFF_SIZE   200 

extern unsigned int RxCounter;          //外部声明，其他文件可以调用该变量
extern char Usart2RecBuf[USART2_RXBUFF_SIZE]; //外部声明，其他文件可以调用该变量

extern uint8_t RX2_Buffer[RX2_MAX_LENGTH];
extern uint32_t RX2_Cnt;
extern uint8_t RX2_Compelete;
extern int humi_status;
extern int fan_status;
extern int yuyin_status;
extern int steering_status;
extern int app_status;



void usart2_init();


//===============================================================发送==============================

//发送单个字节
void usart2_send_byte(uint8_t data);

//发送多个字节
void usart2_send_data(uint8_t * data , uint8_t len);

//发送字符串
void usart2_send_string(char * str);

//如果要想让printf生效，能直接输出数据和串口，必须重写这个函数，然后在函数内部调用自己的发送一个字节的函数
// int fputc(int ch, FILE *f);
int fputc(int ch, FILE *f);




#endif