#include "bsp_usart0.h"


void USART_GPIO_Config(){
	
	rcu_periph_clock_enable(RCU_GPIOA);
	
	gpio_mode_set(GPIOA , GPIO_MODE_AF , GPIO_PUPD_NONE , GPIO_PIN_9 | GPIO_PIN_10);
	
	
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9 | GPIO_PIN_10);
	
	gpio_af_set(GPIOA ,  GPIO_AF_7, GPIO_PIN_9 | GPIO_PIN_10);
	
}

void UASRT_Config(){
	
	rcu_periph_clock_enable(RCU_USART0);
	
	
	usart_baudrate_set(USART0, 115200);
	
	usart_parity_config(USART0, USART_PM_NONE);
	
	usart_word_length_set(USART0, USART_WL_8BIT);
	
	usart_stop_bit_set(USART0, USART_STB_1BIT);
	
	usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
	
	usart_receive_config(USART0, USART_RECEIVE_ENABLE);
	
	
	//打开串口的中断
	nvic_irq_enable(USART0_IRQn, 2, 2);
	
	//配置捕获什么中断
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	usart_interrupt_enable(USART0 ,USART_INT_IDLE);
	

	usart_enable(USART0);
	
}

void usart_init(){
	
	USART_GPIO_Config();
	
	UASRT_Config();
	

}




//===============================================================发送==============================

//发送单个字节
void usart_send_byte(uint8_t data){
	
	usart_data_transmit(USART0, data);
	
	
	while(usart_flag_get(USART0, USART_FLAG_TBE)== RESET);
	
}

//发送多个字节
void usart_send_data(uint8_t * data , uint8_t len){
	
	for(uint8_t i = 0; i < len; i++){
		
		usart_send_byte(data[i]);
		
	}
	
}

//发送字符串
void usart_send_string(char * str){
	
	while(str && *str){
		
		usart_send_byte(*str);
		
		str++;
	}
	
}

//如果要想让printf生效，能直接输出数据和串口，必须重写这个函数，然后在函数内部调用自己的发送一个字节的函数
// int fputc(int ch, FILE *f){
// 	usart_send_byte((uint8_t)ch);
// 	return ch;
// }



//===============================================================接收==============================

uint8_t RX_Buffer[RX_MAX_LENGTH];
uint32_t RX_Cnt = 0 ;
uint8_t RX_Compelete = 0 ;




// 一旦出发中断，就走这个函数
void USART0_IRQHandler(){
	//printf("recv..");
		
	//如果是接收数据触发的中断
	if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) == SET){
			//从读缓冲区拿出来数据之后，把数据装到一个数组里面去
	   RX_Buffer[RX_Cnt++] = usart_data_receive(USART0);
	
		//如果把数组已经装满了，那么就重新开始装
		if(RX_Cnt >= RX_MAX_LENGTH) RX_Cnt = 0 ;
		
		RX_Compelete = 0 ;
	}
	
	//如果是空闲触发的中断
	if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET){
	
		//必须得在这里再去读取串口的数据，否则会影响串口的数据收发
		usart_data_receive(USART0);
		
		RX_Cnt = 0;
		
		RX_Compelete = 1 ;
		
	}
}