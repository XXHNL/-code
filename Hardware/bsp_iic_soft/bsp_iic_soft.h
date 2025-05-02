#ifndef BSP__I2C_SOFT_H
#define BSP__I2C_SOFT_H

#include "gd32f4xx.h"
#include "systick.h"


#define RCU_SCL    RCU_GPIOB
#define RCU_SDA    RCU_GPIOB


#define SCL_PORT   GPIOB
#ifndef SCL_PIN
#define SCL_PIN    GPIO_PIN_10
#endif


#define SDA_PORT   GPIOB
#ifndef SDA_PIN
#define SDA_PIN    GPIO_PIN_11
#endif


#define SCL(value) gpio_bit_write(GPIOB , GPIO_PIN_10 , value ? SET : RESET)
#define SDA(value) gpio_bit_write(GPIOB , GPIO_PIN_11 , value ? SET : RESET)


#define SDA_OUT()  gpio_mode_set(SDA_PORT , GPIO_MODE_OUTPUT , GPIO_PUPD_NONE , SDA_PIN) 
#define SDA_IN()   gpio_mode_set(SDA_PORT , GPIO_MODE_INPUT , GPIO_PUPD_NONE , SDA_PIN)


void I2C_soft_Init(void);

void start();
void stop();
void send_ack();
void send_nack();
uint8_t recv_byte();
void send_byte(uint8_t data);

void I2C_WriteNByte(uint8_t dev_addr , uint8_t mem_addr , uint8_t * data , uint8_t len);


void I2C_ReadNByte(uint8_t dev_addr , uint8_t mem_addr , uint8_t * data , uint8_t len);



#endif

