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
	
	
	//�򿪴��ڵ��ж�
	nvic_irq_enable(USART0_IRQn, 2, 2);
	
	//���ò���ʲô�ж�
	usart_interrupt_enable(USART0, USART_INT_RBNE);
	usart_interrupt_enable(USART0 ,USART_INT_IDLE);
	

	usart_enable(USART0);
	
}

void usart_init(){
	
	USART_GPIO_Config();
	
	UASRT_Config();
	

}




//===============================================================����==============================

//���͵����ֽ�
void usart_send_byte(uint8_t data){
	
	usart_data_transmit(USART0, data);
	
	
	while(usart_flag_get(USART0, USART_FLAG_TBE)== RESET);
	
}

//���Ͷ���ֽ�
void usart_send_data(uint8_t * data , uint8_t len){
	
	for(uint8_t i = 0; i < len; i++){
		
		usart_send_byte(data[i]);
		
	}
	
}

//�����ַ���
void usart_send_string(char * str){
	
	while(str && *str){
		
		usart_send_byte(*str);
		
		str++;
	}
	
}

//���Ҫ����printf��Ч����ֱ��������ݺʹ��ڣ�������д���������Ȼ���ں����ڲ������Լ��ķ���һ���ֽڵĺ���
// int fputc(int ch, FILE *f){
// 	usart_send_byte((uint8_t)ch);
// 	return ch;
// }



//===============================================================����==============================

uint8_t RX_Buffer[RX_MAX_LENGTH];
uint32_t RX_Cnt = 0 ;
uint8_t RX_Compelete = 0 ;




// һ�������жϣ������������
void USART0_IRQHandler(){
	//printf("recv..");
		
	//����ǽ������ݴ������ж�
	if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) == SET){
			//�Ӷ��������ó�������֮�󣬰�����װ��һ����������ȥ
	   RX_Buffer[RX_Cnt++] = usart_data_receive(USART0);
	
		//����������Ѿ�װ���ˣ���ô�����¿�ʼװ
		if(RX_Cnt >= RX_MAX_LENGTH) RX_Cnt = 0 ;
		
		RX_Compelete = 0 ;
	}
	
	//����ǿ��д������ж�
	if(usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET){
	
		//�������������ȥ��ȡ���ڵ����ݣ������Ӱ�촮�ڵ������շ�
		usart_data_receive(USART0);
		
		RX_Cnt = 0;
		
		RX_Compelete = 1 ;
		
	}
}