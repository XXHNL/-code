#ifndef _BSP_IIC0_H
#define _BSP_IIC0_H

#include "gd32f4xx.h"
#include "systick.h"

//0:软件  1:硬件
#define IIC0_TYPE  1

//SCL
#define IIC0_SCL_RCU   RCU_GPIOB
#define IIC0_SCL_PORT  GPIOB
#define IIC0_SCL_PIN   GPIO_PIN_6

//SDA
#define IIC0_SDA_RCU   RCU_GPIOB
#define IIC0_SDA_PORT  GPIOB
#define IIC0_SDA_PIN   GPIO_PIN_7

//返回码
typedef enum{
	IIC_SUCCESS,
	IIC_SEND_ADDR_ERROR,//发送设备地址失败
	IIC_SEND_REG_ERROR,//发送寄存器失败
	IIC_SEND_DAT_ERROR,//发送数据失败
	IIC_BUS_BSY_ERROR,//总线繁忙
	IIC_START_ERROR,//发送起始信号失败
	IIC_STOP_ERROR,//停止总线失败
	IIC_AUTO_ACK_ERROR,//接收时开启自动应答失败
	IIC_RECV_BYTE_ERROR//接收1byte失败
}IIC_STATUS;

//软件
#if IIC0_TYPE //硬件
	//AF
	#define IIC0_SCL_AF    GPIO_AF_4
	#define IIC0_SDA_AF    GPIO_AF_4

	//IIC
	#define IIC0_NUM			 I2C0
	#define IIC0_RCU			 RCU_I2C0
	#define IIC0_SPEED     400000
#else         //软件
	//SCL拉高拉低
	#define SCL_L  gpio_bit_reset(GPIOB,GPIO_PIN_6)
	#define SCL_H  gpio_bit_set(GPIOB,GPIO_PIN_6)
	//SDA拉高拉低
	#define SDA_L  gpio_bit_reset(GPIOB,GPIO_PIN_7)
	#define SDA_H  gpio_bit_set(GPIOB,GPIO_PIN_7)
	//SDA输出和输入
	#define SDA_OUT gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7)
	#define SDA_IN  gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
	//读取SDA状态
	#define SDA_STATUS gpio_input_bit_get(GPIOB,GPIO_PIN_7)

	//睡眠时间
	#define DELAY   delay_1us(5)
#endif



//初始化
void bsp_iic0_init(void);

//向IIC设备写入数据
uint8_t bsp_iic0_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n);

//向IIC设备写入一列数据
//dat: 列第一个元素地址
//col: 一列有几个元素
//row: 需要发送多少行数据  
uint8_t bsp_iic0_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row);

//从IIC设备中读取n个byte数据(读取n个byte放入buff中)
uint8_t bsp_iic0_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n);

#endif