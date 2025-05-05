#ifndef USART2_H
#define USART2_H

#include "gd32f4xx.h"

#include <stdio.h>


#define RX2_MAX_LENGTH 200
#define USART2_RXBUFF_SIZE   200 

extern unsigned int RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart2RecBuf[USART2_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���

extern uint8_t RX2_Buffer[RX2_MAX_LENGTH];
extern uint32_t RX2_Cnt;
extern uint8_t RX2_Compelete;
extern int humi_status;
extern int fan_status;
extern int yuyin_status;
extern int steering_status;
extern int app_status;



void usart2_init();


//===============================================================����==============================

//���͵����ֽ�
void usart2_send_byte(uint8_t data);

//���Ͷ���ֽ�
void usart2_send_data(uint8_t * data , uint8_t len);

//�����ַ���
void usart2_send_string(char * str);

//���Ҫ����printf��Ч����ֱ��������ݺʹ��ڣ�������д���������Ȼ���ں����ڲ������Լ��ķ���һ���ֽڵĺ���
// int fputc(int ch, FILE *f);
int fputc(int ch, FILE *f);




#endif