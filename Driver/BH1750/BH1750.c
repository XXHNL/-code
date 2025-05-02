#include "BH1750.h"


uint8_t BUFF[2] = {0};

// 静态函数，用于向BH1750传感器发送命令
static void Single_BH1750_Write(uint8_t dat){
    I2C_WriteNByte(0x46 , dat , BUFF , 1);
}

// 静态函数，连续读出BH1750内部数据
static void BH1750_read(){
    I2C_ReadNByte(0x46 ,0 , BUFF , 2);
    delay_1ms(5);
}


// 初始化BH1750函数
void BH1750_Init(void){
    // 初始化I2C软总线
    I2C_soft_Init();

    stop();

    // 向BH1750发送命令，设置工作模式
    Single_BH1750_Write(0x01);

    //复位BH1750
    Single_BH1750_Write(0x07);

}


// 读取BH1750传感器的光照强度
float BH1750_Read(void){
    Single_BH1750_Write(0x10);
    // 延时120ms，等待测量完成
    delay_1ms(120);

    BH1750_read();
    // 将读取到的两个字节的数据合并为一个16位的数据
    uint16_t data = (BUFF[0] << 8) | BUFF[1];
    // 返回光照强度，单位为lux
    return (float) data / 1.2;
}



