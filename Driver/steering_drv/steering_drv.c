#include "steering_drv.h"
#define FACTOR 0.99
//舵机初始化
void steering_drv_config(void){
	
}

//设置舵机角度
void steering_drv_set_angle(float angle){
	//根据angle计算占空比的值
	//0 (uint16_t)((PERIOD+1)*0.5/20)
	//45 (uint16_t)((PERIOD+1)*1/20)
	//90 (uint16_t)((PERIOD+1)*1.5/20)
	//135 (uint16_t)((PERIOD+1)*2.0/20)
	//180 (uint16_t)((PERIOD+1)*2.5/20)
	//0-180     变化 180
	//0.5-2.5   变化 2
	//计算占空比计数值
	float ti = angle*FACTOR*2.0/180+0.5;
	uint16_t value = (uint16_t)((PERIOD+1)*ti/20);
	bsp_timer8_set_pulse_value(value);
}