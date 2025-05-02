#ifndef _BSP_IIC0_H
#define _BSP_IIC0_H

#include "gd32f4xx.h"
#include "systick.h"

//0:���  1:Ӳ��
#define IIC0_TYPE  1

//SCL
#define IIC0_SCL_RCU   RCU_GPIOB
#define IIC0_SCL_PORT  GPIOB
#define IIC0_SCL_PIN   GPIO_PIN_6

//SDA
#define IIC0_SDA_RCU   RCU_GPIOB
#define IIC0_SDA_PORT  GPIOB
#define IIC0_SDA_PIN   GPIO_PIN_7

//������
typedef enum{
	IIC_SUCCESS,
	IIC_SEND_ADDR_ERROR,//�����豸��ַʧ��
	IIC_SEND_REG_ERROR,//���ͼĴ���ʧ��
	IIC_SEND_DAT_ERROR,//��������ʧ��
	IIC_BUS_BSY_ERROR,//���߷�æ
	IIC_START_ERROR,//������ʼ�ź�ʧ��
	IIC_STOP_ERROR,//ֹͣ����ʧ��
	IIC_AUTO_ACK_ERROR,//����ʱ�����Զ�Ӧ��ʧ��
	IIC_RECV_BYTE_ERROR//����1byteʧ��
}IIC_STATUS;

//���
#if IIC0_TYPE //Ӳ��
	//AF
	#define IIC0_SCL_AF    GPIO_AF_4
	#define IIC0_SDA_AF    GPIO_AF_4

	//IIC
	#define IIC0_NUM			 I2C0
	#define IIC0_RCU			 RCU_I2C0
	#define IIC0_SPEED     400000
#else         //���
	//SCL��������
	#define SCL_L  gpio_bit_reset(GPIOB,GPIO_PIN_6)
	#define SCL_H  gpio_bit_set(GPIOB,GPIO_PIN_6)
	//SDA��������
	#define SDA_L  gpio_bit_reset(GPIOB,GPIO_PIN_7)
	#define SDA_H  gpio_bit_set(GPIOB,GPIO_PIN_7)
	//SDA���������
	#define SDA_OUT gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_7)
	#define SDA_IN  gpio_mode_set(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_7);
	//��ȡSDA״̬
	#define SDA_STATUS gpio_input_bit_get(GPIOB,GPIO_PIN_7)

	//˯��ʱ��
	#define DELAY   delay_1us(5)
#endif



//��ʼ��
void bsp_iic0_init(void);

//��IIC�豸д������
uint8_t bsp_iic0_write(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t n);

//��IIC�豸д��һ������
//dat: �е�һ��Ԫ�ص�ַ
//col: һ���м���Ԫ��
//row: ��Ҫ���Ͷ���������  
uint8_t bsp_iic0_write_col(uint8_t addr,uint8_t reg,uint8_t *dat,uint8_t col,uint8_t row);

//��IIC�豸�ж�ȡn��byte����(��ȡn��byte����buff��)
uint8_t bsp_iic0_read(uint8_t addr,uint8_t reg,uint8_t *buff,uint8_t n);

#endif