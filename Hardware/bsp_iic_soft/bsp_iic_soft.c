#include "bsp_iic_soft.h"
#include <stdio.h>

#define DELAY()  delay_1us(5)


// 函数：开始
void start(){
	// 延时1ms
	delay_1ms(1);
	
	// 设置SDA为输出模式
	SDA_OUT();
	

	// 将SDA置高电平
	SDA(1);
	// 延时
	DELAY();
	

	// 将SCL置高电平
	SCL(1);
	// 延时
	DELAY();
	

	// 将SDA置低电平
	SDA(0);
	// 延时
	DELAY();
	

	// 将SCL置低电平
	SCL(0);
	// 延时
	DELAY();
}


// 发送一个字节
void send_byte(uint8_t data){
	// 将SDA设置为输出模式
	SDA_OUT();
	

	// 循环8次，发送8位数据
	for(uint8_t i = 0 ; i < 8 ; i++){
		

		// 如果最高位为1，则将SDA置为高电平
		if(data & 0x80){ 
			SDA(1);
		}else{                    // 否则将SDA置为低电平
			SDA(0);
		}
		

		// 将SCL置为高电平
		SCL(1);
		// 延时
		DELAY();
		

		// 将SCL置为低电平
		SCL(0);
		// 延时
		DELAY();
		

		// 将数据左移一位，准备发送下一位
		data <<= 1;
	}
	
}



// 函数：接收一个字节
// 返回值：接收到的字节
uint8_t recv_byte(){
	
	// 定义一个变量data，用于存储接收到的字节
	uint8_t data = 0 ;
	// 将SDA引脚设置为输入模式
	SDA_IN();
	
	// 循环8次，接收8位数据
	for(uint8_t i = 0 ; i < 8 ; i++){
		

		// 将SCL引脚置为低电平
		SCL(0);
		// 延时
		DELAY();
		

		// 将SCL引脚置为高电平
		SCL(1);
		// 延时
		DELAY();
		
	
		// 将接收到的数据左移一位
		data <<=1;
		// 将SDA引脚的值赋给data
		data |= gpio_input_bit_get(SDA_PORT , SDA_PIN);
		

		// 延时
		DELAY();

		
		
	}
	// 将SCL引脚置为低电平
	SCL(0);
	// 延时
	DELAY();
	// 返回接收到的数据
	return data;
	
}


// 发送应答信号
void send_ack(){
	
	// 设置SDA为输出模式
	SDA_OUT();

	// 将SDA拉低
	SDA(0);
	// 延时
	DELAY();
	
	// 将SCL拉高
	SCL(1);
	// 延时
	DELAY();
	
	// 将SCL拉低
	SCL(0);
	// 延时
	DELAY();
}


// 发送非应答信号
void send_nack(){
	// 设置SDA为输出模式
	SDA_OUT();

	// 将SDA置高
	SDA(1);
	// 延时
	DELAY();
	
	// 将SCL置高
	SCL(1);
	// 延时
	DELAY();
	
	// 将SCL置低
	SCL(0);
	// 延时
	DELAY();
}


// 等待应答函数
void wait_ack(){
	

	// 将SDA设置为输入模式
	SDA_IN();
	

	// 将SDA设置为高电平
	SDA(1);
	// 延时
	DELAY();
	

	// 将SCL设置为高电平
	SCL(1);
	// 延时
	DELAY();
	

	// 将SCL设置为低电平
	SCL(0);
	// 延时
	DELAY();

}



// 停止信号
void stop(){

	// 将SDA设置为输出模式
	SDA_OUT();
	

	// 将SDA拉低
	SDA(0);
	

	// 将SCL拉高
	SCL(1);
	

	// 延时一段时间
	DELAY();
	

	// 将SDA拉高
	SDA(1);

	// 延时一段时间
	DELAY();
}
	
	




// 初始化I2C
void I2C_soft_Init(){
	

	// 使能SCL时钟
	rcu_periph_clock_enable(RCU_SCL);
	

	// 设置SCL引脚为输出模式，无上下拉电阻
	gpio_mode_set(SCL_PORT ,GPIO_MODE_OUTPUT , GPIO_PUPD_NONE , SCL_PIN);
	// 设置SCL引脚为开漏输出，最大速度
	gpio_output_options_set(SCL_PORT , GPIO_OTYPE_OD , GPIO_OSPEED_MAX , SCL_PIN);
	
	

	// 使能SDA时钟
	rcu_periph_clock_enable(RCU_SDA);
	// 设置SDA引脚为输出模式，无上下拉电阻
	gpio_mode_set(SDA_PORT , GPIO_MODE_OUTPUT , GPIO_PUPD_NONE , SDA_PIN);
	// 设置SDA引脚为开漏输出，最大速度
	gpio_output_options_set(SDA_PORT , GPIO_OTYPE_OD , GPIO_OSPEED_MAX , SDA_PIN);
}





// 函数：I2C_WriteNByte
// 功能：向I2C设备写入N个字节
// 参数：dev_addr：设备地址，mem_addr：内存地址，data：数据，len：数据长度
void I2C_WriteNByte(uint8_t dev_addr , uint8_t mem_addr , uint8_t * data , uint8_t len){
	

	// 发送起始信号
	start();
	
	

	// 发送设备地址
	send_byte(dev_addr);
	

	// 等待应答信号
	wait_ack();

	

	// 发送内存地址
	send_byte(mem_addr);
	

	// 等待应答信号
	wait_ack();
	

	// 循环发送数据
	for(uint8_t i = 0 ; i < len ; i++){
		send_byte(data[i]);
		wait_ack();
	}
	

	// 发送停止信号
	stop();
}



void I2C_ReadNByte(uint8_t dev_addr , uint8_t mem_addr , uint8_t * data , uint8_t len){


	// 发送起始信号
	start();
	
	

	// 发送设备地址
	send_byte(dev_addr);
	

	// 等待应答信号
	wait_ack();
	

	// 发送内存地址
	send_byte(mem_addr);
	

	// 等待应答信号
	wait_ack();

	// 发送起始信号
	start();
	

	// 发送设备地址+1（读操作）
	send_byte(dev_addr | 1);
	

	// 等待应答信号
	wait_ack();
	

	// 循环读取数据
	do{
		*data = recv_byte();
		data++; 
		if(len != 1) send_ack();
	}while(--len);
	

	send_nack();
	

	stop();
	
	
}

