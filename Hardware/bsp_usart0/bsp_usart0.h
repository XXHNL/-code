#ifndef USART_H
#define USART_H

#include "gd32f4xx.h"

#include <stdio.h>


#define RX_MAX_LENGTH 1024

extern uint8_t RX_Buffer[RX_MAX_LENGTH];
extern uint32_t RX_Cnt;
extern uint8_t RX_Compelete;




void usart_init();


//===============================================================����==============================

//���͵����ֽ�
void usart_send_byte(uint8_t data);

//���Ͷ���ֽ�
void usart_send_data(uint8_t * data , uint8_t len);

//�����ַ���
void usart_send_string(char * str);

//���Ҫ����printf��Ч����ֱ��������ݺʹ��ڣ�������д���������Ȼ���ں����ڲ������Լ��ķ���һ���ֽڵĺ���
// int fputc(int ch, FILE *f);








#endif