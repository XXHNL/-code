#ifndef USART1_H
#define USART1_H

#include "gd32f4xx.h"

#include <stdio.h>

extern int humi_status;
extern int fan_status;
#define RX1_MAX_LENGTH 200
#define USART1_RXBUFF_SIZE   200 

extern unsigned int RxCounter;          //�ⲿ�����������ļ����Ե��øñ���
extern char Usart1RecBuf[USART1_RXBUFF_SIZE]; //�ⲿ�����������ļ����Ե��øñ���

extern uint8_t RX1_Buffer[RX1_MAX_LENGTH];
extern uint32_t RX1_Cnt;
extern uint8_t RX1_Compelete;




void usart1_init();


//===============================================================����==============================

//���͵����ֽ�
void usart1_send_byte(uint8_t data);

//���Ͷ���ֽ�
void usart1_send_data(uint8_t * data , uint8_t len);

//�����ַ���
void usart1_send_string(char * str);

//���Ҫ����printf��Ч����ֱ��������ݺʹ��ڣ�������д���������Ȼ���ں����ڲ������Լ��ķ���һ���ֽڵĺ���
// int fputc(int ch, FILE *f);
int fputc(int ch, FILE *f);

void uart1_SendStr(char*SendBuf);
void uart1_send(unsigned char *bufs,unsigned char len);




#endif