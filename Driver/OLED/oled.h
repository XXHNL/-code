#ifndef __OLED_H
#define __OLED_H 

#include "gd32f4xx.h"
#include "stdlib.h"	


#define RCU_SCL  RCU_GPIOB  
#define SCL_PORT GPIOB      
#define SCL_PIN  GPIO_PIN_6 


#define RCU_SDA  RCU_GPIOB
#define SDA_PORT GPIOB
#define SDA_PIN  GPIO_PIN_7

//-----------------OLED??????---------------- 

#define OLED_SCL_Clr() gpio_bit_write(SCL_PORT , SCL_PIN , RESET)//SCL
#define OLED_SCL_Set() gpio_bit_write(SCL_PORT , SCL_PIN , SET)

#define OLED_SDA_Clr() gpio_bit_write(SDA_PORT , SDA_PIN , RESET)//SDA
#define OLED_SDA_Set() gpio_bit_write(SDA_PORT , SDA_PIN , SET)

#define OLED_RES_Clr() gpio_bit_write(GPIOD , GPIO_PIN_4 , RESET)//RES
#define OLED_RES_Set() gpio_bit_write(GPIOD , GPIO_PIN_4 , SET)


#define OLED_CMD  0	//§Õ????
#define OLED_DATA 1	//§Õ????


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;


void OLED_ClearPoint(u8 x,u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat,u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void OLED_DrawCircle(u8 x,u8 y,u8 r);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void OLED_ShowChar6x8(u8 x,u8 y,u8 chr,u8 mode);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void OLED_Init(void);

#endif

