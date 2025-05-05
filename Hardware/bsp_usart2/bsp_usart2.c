#include "bsp_usart2.h"
#include <stdint.h>
#include <stdio.h>

// Make these static to limit their scope to this file
char Usart2RecBuf[USART2_RXBUFF_SIZE];//????1???????????
unsigned int Rx2Counter = 0;   //????1?????????¦Ë




/****************************************************************************

??????Usart2 ????? 
   PC10 USART2_TX			PB6
   PC11: USART2_RX   	PB5


****************************************************************************/

// Make these static as well
static void USART_GPIO_Config(){
	
	rcu_periph_clock_enable(RCU_GPIOC);
	
	gpio_mode_set(GPIOC , GPIO_MODE_AF , GPIO_PUPD_NONE , GPIO_PIN_10 | GPIO_PIN_11);

	gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10 | GPIO_PIN_11);

	gpio_af_set(GPIOC,  GPIO_AF_7, GPIO_PIN_10 | GPIO_PIN_11);

	
	
}

// Make these static as well
static void USART_Config(){
	
	rcu_periph_clock_enable(RCU_USART2);
	
	usart_baudrate_set(USART2, 115200);
	
	usart_parity_config(USART2, USART_PM_NONE);
	
	usart_word_length_set(USART2, USART_WL_8BIT);
	
	usart_stop_bit_set(USART2, USART_STB_1BIT);
	
	usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
	
	usart_receive_config(USART2, USART_RECEIVE_ENABLE);
	
	
	//???????§Ø?
	nvic_irq_enable(USART2_IRQn, 2, 2);
	
	//???¨°??????§Ø?
	usart_interrupt_enable(USART2, USART_INT_RBNE);
	usart_interrupt_enable(USART2 ,USART_INT_IDLE);
	

	usart_enable(USART2);
	
}

void usart2_init(){
	
	USART_GPIO_Config();
	
	USART_Config();
	

}


void usart2_send_byte(uint8_t data){
	
	usart_data_transmit(USART2, data);
	
	while(usart_flag_get(USART2, USART_FLAG_TBE)== RESET);
	
}

//?????????
void usart2_send_data(uint8_t * data , uint8_t len){
	
	for(uint8_t i = 0; i < len; i++){
		
		usart2_send_byte(data[i]);
		
	}
	
}

//?????????
void usart2_send_string(char * str){
	
	while(str && *str){
		
		usart2_send_byte(*str);
		
		str++;
	}
	
}

uint8_t RX2_Buffer[RX2_MAX_LENGTH];
uint32_t RX2_Cnt = 0;
uint8_t RX2_Compelete = 0;


void USART2_IRQHandler(){
	if(usart_interrupt_flag_get(USART2, USART_INT_FLAG_RBNE) == SET){
		char receivedChar = usart_data_receive(USART2);
	if (receivedChar == 'E') {
		gpio_bit_toggle(GPIOD,GPIO_PIN_9);
	}
	if (receivedChar == 'F') {
		gpio_bit_toggle(GPIOD,GPIO_PIN_9);
	}
	if (receivedChar == 'G') {
		humi_status = 1;
		yuyin_status = 1;	
		app_status = 0;
	}
	if (receivedChar == 'H') {
		humi_status = 0;
		yuyin_status = 1;
		app_status = 0;
	}
	if (receivedChar == 'I') {
		fan_status = 1;
		yuyin_status = 1;	
		app_status = 0;
	}
	if (receivedChar == 'J') {
		fan_status = 0;
		yuyin_status = 1;
		app_status = 0;
	}
	if(receivedChar == 'K') {
		app_status = 0;
		steering_status = 1;
		yuyin_status = 1;	
	}
	if(receivedChar == 'L') {
		app_status = 0;
		steering_status = 0;
		yuyin_status = 1;
	}
			//??????????¨®??????????????????????????????
			if(Rx2Counter < USART2_RXBUFF_SIZE) {
                Usart2RecBuf[Rx2Counter++] = usart_data_receive(USART1);//????????????
            } else {
                Rx2Counter = 0; // ??????????????????
            }
            RX2_Compelete = 0 ;
	}
	
	//???????§Õ??????§Ø?
	if(usart_interrupt_flag_get(USART2, USART_INT_FLAG_IDLE) == SET){
	
		//?????????????????????????????????????????????
		usart_data_receive(USART2);
		
		RX2_Cnt = 0;
		
		RX2_Compelete = 1 ;
		
	}
}

