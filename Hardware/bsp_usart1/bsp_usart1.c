#include "bsp_usart1.h"
#include <stdint.h>
#include <stdio.h>

// Make these static to limit their scope to this file
char Usart1RecBuf[USART1_RXBUFF_SIZE];//串口1接收数据缓存
unsigned int RxCounter = 0;   //串口1收到数据标志位




/****************************************************************************

描述：Usart1 初始化 
   PA2： USART1_TX
   PA3: USART1_RX

参数：none

返回: none

****************************************************************************/

// Make these static as well
static void USART_GPIO_Config(){
	
	rcu_periph_clock_enable(RCU_GPIOA);
	
	gpio_mode_set(GPIOA , GPIO_MODE_AF , GPIO_PUPD_NONE , GPIO_PIN_2 | GPIO_PIN_3);

	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2 | GPIO_PIN_3);

	gpio_af_set(GPIOA,  GPIO_AF_7, GPIO_PIN_2 | GPIO_PIN_3);

	
	
}

// Make these static as well
static void USART_Config(){
	
	rcu_periph_clock_enable(RCU_USART1);
	
	usart_baudrate_set(USART1, 115200);
	
	usart_parity_config(USART1, USART_PM_NONE);
	
	usart_word_length_set(USART1, USART_WL_8BIT);
	
	usart_stop_bit_set(USART1, USART_STB_1BIT);
	
	usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
	
	usart_receive_config(USART1, USART_RECEIVE_ENABLE);
	
	
	//打开串口的中断
	nvic_irq_enable(USART1_IRQn, 2, 2);
	
	//配置捕获什么中断
	usart_interrupt_enable(USART1, USART_INT_RBNE);
	usart_interrupt_enable(USART1 ,USART_INT_IDLE);
	

	usart_enable(USART1);
	
}

void usart1_init(){
	
	USART_GPIO_Config();
	
	USART_Config();
	

}







//===============================================================发送==============================

//发送单个字节
void usart1_send_byte(uint8_t data){
	
	usart_data_transmit(USART1, data);
	
	while(usart_flag_get(USART1, USART_FLAG_TBE)== RESET);
	
}

//发送多个字节
void usart1_send_data(uint8_t * data , uint8_t len){
	
	for(uint8_t i = 0; i < len; i++){
		
		usart1_send_byte(data[i]);
		
	}
	
}

//发送字符串
void usart1_send_string(char * str){
	
	while(str && *str){
		
		usart1_send_byte(*str);
		
		str++;
	}
	
}

// // Support printf by redefining fputc
// int fputc1(int ch, FILE *f)
// {      
// while(usart_flag_get(USART1, USART_FLAG_TBE) == RESET) {
//     /* 等待发送缓冲区为空 */
// }
//     usart_data_transmit(USART1, (uint8_t)ch);
// 	return ch;
// }

int __io_putchar(int ch) {
	while(usart_flag_get(USART1, USART_FLAG_TBE) == RESET);
	usart_data_transmit(USART1, (uint8_t)ch);
	return ch;
}

//===============================================================接收==============================

// Make these static as well

// 修改后：
uint8_t RX1_Buffer[RX1_MAX_LENGTH];
uint32_t RX1_Cnt = 0;
uint8_t RX1_Compelete = 0;



// 一旦出发中断，就走这个函数
void USART1_IRQHandler(){
	//printf("recv..");
	//如果是接收数据触发的中断
	if(usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE) == SET){
		char receivedChar = usart_data_receive(USART1);
		if (receivedChar == 'E') {
				// Handle the reception of character 'A'
				// Set a flag or call a control function
				 gpio_bit_toggle(GPIOD,GPIO_PIN_9);
		}
			//从读缓冲区拿出来数据之后，把数据装到一个数组里面去
			if(RxCounter < USART1_RXBUFF_SIZE) {
                Usart1RecBuf[RxCounter++] = usart_data_receive(USART1);//接收模块的数据
            } else {
                RxCounter = 0; // 超出缓存区则重新开始
            }
            RX1_Compelete = 0 ;
	}
	
	//如果是空闲触发的中断
	if(usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE) == SET){
	
		//必须得在这里再去读取串口的数据，否则会影响串口的数据收发
		usart_data_receive(USART1);
		
		RX1_Cnt = 0;
		
		RX1_Compelete = 1 ;
		
	}
}


void uart1_SendStr(char*SendBuf)//串口1打印数据
{
    while(*SendBuf)
    {
        while(usart_flag_get(USART1, USART_FLAG_TBE) == RESET);//等待发送完成
        usart_data_transmit(USART1, (uint8_t)*SendBuf);
        SendBuf++;
    }
}

void uart1_send(unsigned char *bufs,unsigned char len)
{
    if(len != 0xFF)
    {
        while (len--)
        {
            while(usart_flag_get(USART1, USART_FLAG_TBE) == RESET);//等待发送完成
            usart_data_transmit(USART1, *bufs);
            bufs ++;
        }
    }
    else//如果字长等于或超过255，不按用户写入字长发送
    {
        for (; *bufs != 0; bufs++)  //把字符逐个发送出去
        {
            while(usart_flag_get(USART1, USART_FLAG_TBE) == RESET);//等待发送完成
            usart_data_transmit(USART1, (uint8_t)*bufs);
        }
    }
}
