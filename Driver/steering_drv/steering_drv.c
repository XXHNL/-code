#include "steering_drv.h"
#define FACTOR 0.99
//�����ʼ��
void steering_drv_config(void){
	
}

//���ö���Ƕ�
void steering_drv_set_angle(float angle){
	//����angle����ռ�ձȵ�ֵ
	//0 (uint16_t)((PERIOD+1)*0.5/20)
	//45 (uint16_t)((PERIOD+1)*1/20)
	//90 (uint16_t)((PERIOD+1)*1.5/20)
	//135 (uint16_t)((PERIOD+1)*2.0/20)
	//180 (uint16_t)((PERIOD+1)*2.5/20)
	//0-180     �仯 180
	//0.5-2.5   �仯 2
	//����ռ�ձȼ���ֵ
	float ti = angle*FACTOR*2.0/180+0.5;
	uint16_t value = (uint16_t)((PERIOD+1)*ti/20);
	bsp_timer8_set_pulse_value(value);
}