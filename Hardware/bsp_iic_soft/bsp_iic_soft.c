#include "bsp_iic_soft.h"
#include <stdio.h>

#define DELAY()  delay_1us(5)


// ��������ʼ
void start(){
	// ��ʱ1ms
	delay_1ms(1);
	
	// ����SDAΪ���ģʽ
	SDA_OUT();
	

	// ��SDA�øߵ�ƽ
	SDA(1);
	// ��ʱ
	DELAY();
	

	// ��SCL�øߵ�ƽ
	SCL(1);
	// ��ʱ
	DELAY();
	

	// ��SDA�õ͵�ƽ
	SDA(0);
	// ��ʱ
	DELAY();
	

	// ��SCL�õ͵�ƽ
	SCL(0);
	// ��ʱ
	DELAY();
}


// ����һ���ֽ�
void send_byte(uint8_t data){
	// ��SDA����Ϊ���ģʽ
	SDA_OUT();
	

	// ѭ��8�Σ�����8λ����
	for(uint8_t i = 0 ; i < 8 ; i++){
		

		// ������λΪ1����SDA��Ϊ�ߵ�ƽ
		if(data & 0x80){ 
			SDA(1);
		}else{                    // ����SDA��Ϊ�͵�ƽ
			SDA(0);
		}
		

		// ��SCL��Ϊ�ߵ�ƽ
		SCL(1);
		// ��ʱ
		DELAY();
		

		// ��SCL��Ϊ�͵�ƽ
		SCL(0);
		// ��ʱ
		DELAY();
		

		// ����������һλ��׼��������һλ
		data <<= 1;
	}
	
}



// ����������һ���ֽ�
// ����ֵ�����յ����ֽ�
uint8_t recv_byte(){
	
	// ����һ������data�����ڴ洢���յ����ֽ�
	uint8_t data = 0 ;
	// ��SDA��������Ϊ����ģʽ
	SDA_IN();
	
	// ѭ��8�Σ�����8λ����
	for(uint8_t i = 0 ; i < 8 ; i++){
		

		// ��SCL������Ϊ�͵�ƽ
		SCL(0);
		// ��ʱ
		DELAY();
		

		// ��SCL������Ϊ�ߵ�ƽ
		SCL(1);
		// ��ʱ
		DELAY();
		
	
		// �����յ�����������һλ
		data <<=1;
		// ��SDA���ŵ�ֵ����data
		data |= gpio_input_bit_get(SDA_PORT , SDA_PIN);
		

		// ��ʱ
		DELAY();

		
		
	}
	// ��SCL������Ϊ�͵�ƽ
	SCL(0);
	// ��ʱ
	DELAY();
	// ���ؽ��յ�������
	return data;
	
}


// ����Ӧ���ź�
void send_ack(){
	
	// ����SDAΪ���ģʽ
	SDA_OUT();

	// ��SDA����
	SDA(0);
	// ��ʱ
	DELAY();
	
	// ��SCL����
	SCL(1);
	// ��ʱ
	DELAY();
	
	// ��SCL����
	SCL(0);
	// ��ʱ
	DELAY();
}


// ���ͷ�Ӧ���ź�
void send_nack(){
	// ����SDAΪ���ģʽ
	SDA_OUT();

	// ��SDA�ø�
	SDA(1);
	// ��ʱ
	DELAY();
	
	// ��SCL�ø�
	SCL(1);
	// ��ʱ
	DELAY();
	
	// ��SCL�õ�
	SCL(0);
	// ��ʱ
	DELAY();
}


// �ȴ�Ӧ����
void wait_ack(){
	

	// ��SDA����Ϊ����ģʽ
	SDA_IN();
	

	// ��SDA����Ϊ�ߵ�ƽ
	SDA(1);
	// ��ʱ
	DELAY();
	

	// ��SCL����Ϊ�ߵ�ƽ
	SCL(1);
	// ��ʱ
	DELAY();
	

	// ��SCL����Ϊ�͵�ƽ
	SCL(0);
	// ��ʱ
	DELAY();

}



// ֹͣ�ź�
void stop(){

	// ��SDA����Ϊ���ģʽ
	SDA_OUT();
	

	// ��SDA����
	SDA(0);
	

	// ��SCL����
	SCL(1);
	

	// ��ʱһ��ʱ��
	DELAY();
	

	// ��SDA����
	SDA(1);

	// ��ʱһ��ʱ��
	DELAY();
}
	
	




// ��ʼ��I2C
void I2C_soft_Init(){
	

	// ʹ��SCLʱ��
	rcu_periph_clock_enable(RCU_SCL);
	

	// ����SCL����Ϊ���ģʽ��������������
	gpio_mode_set(SCL_PORT ,GPIO_MODE_OUTPUT , GPIO_PUPD_NONE , SCL_PIN);
	// ����SCL����Ϊ��©���������ٶ�
	gpio_output_options_set(SCL_PORT , GPIO_OTYPE_OD , GPIO_OSPEED_MAX , SCL_PIN);
	
	

	// ʹ��SDAʱ��
	rcu_periph_clock_enable(RCU_SDA);
	// ����SDA����Ϊ���ģʽ��������������
	gpio_mode_set(SDA_PORT , GPIO_MODE_OUTPUT , GPIO_PUPD_NONE , SDA_PIN);
	// ����SDA����Ϊ��©���������ٶ�
	gpio_output_options_set(SDA_PORT , GPIO_OTYPE_OD , GPIO_OSPEED_MAX , SDA_PIN);
}





// ������I2C_WriteNByte
// ���ܣ���I2C�豸д��N���ֽ�
// ������dev_addr���豸��ַ��mem_addr���ڴ��ַ��data�����ݣ�len�����ݳ���
void I2C_WriteNByte(uint8_t dev_addr , uint8_t mem_addr , uint8_t * data , uint8_t len){
	

	// ������ʼ�ź�
	start();
	
	

	// �����豸��ַ
	send_byte(dev_addr);
	

	// �ȴ�Ӧ���ź�
	wait_ack();

	

	// �����ڴ��ַ
	send_byte(mem_addr);
	

	// �ȴ�Ӧ���ź�
	wait_ack();
	

	// ѭ����������
	for(uint8_t i = 0 ; i < len ; i++){
		send_byte(data[i]);
		wait_ack();
	}
	

	// ����ֹͣ�ź�
	stop();
}



void I2C_ReadNByte(uint8_t dev_addr , uint8_t mem_addr , uint8_t * data , uint8_t len){


	// ������ʼ�ź�
	start();
	
	

	// �����豸��ַ
	send_byte(dev_addr);
	

	// �ȴ�Ӧ���ź�
	wait_ack();
	

	// �����ڴ��ַ
	send_byte(mem_addr);
	

	// �ȴ�Ӧ���ź�
	wait_ack();

	// ������ʼ�ź�
	start();
	

	// �����豸��ַ+1����������
	send_byte(dev_addr | 1);
	

	// �ȴ�Ӧ���ź�
	wait_ack();
	

	// ѭ����ȡ����
	do{
		*data = recv_byte();
		data++; 
		if(len != 1) send_ack();
	}while(--len);
	

	send_nack();
	

	stop();
	
	
}

