#include "BH1750.h"


uint8_t BUFF[2] = {0};

// ��̬������������BH1750��������������
static void Single_BH1750_Write(uint8_t dat){
    I2C_WriteNByte(0x46 , dat , BUFF , 1);
}

// ��̬��������������BH1750�ڲ�����
static void BH1750_read(){
    I2C_ReadNByte(0x46 ,0 , BUFF , 2);
    delay_1ms(5);
}


// ��ʼ��BH1750����
void BH1750_Init(void){
    // ��ʼ��I2C������
    I2C_soft_Init();

    stop();

    // ��BH1750����������ù���ģʽ
    Single_BH1750_Write(0x01);

    //��λBH1750
    Single_BH1750_Write(0x07);

}


// ��ȡBH1750�������Ĺ���ǿ��
float BH1750_Read(void){
    Single_BH1750_Write(0x10);
    // ��ʱ120ms���ȴ��������
    delay_1ms(120);

    BH1750_read();
    // ����ȡ���������ֽڵ����ݺϲ�Ϊһ��16λ������
    uint16_t data = (BUFF[0] << 8) | BUFF[1];
    // ���ع���ǿ�ȣ���λΪlux
    return (float) data / 1.2;
}



