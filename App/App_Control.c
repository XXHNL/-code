#include "App.h"


#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define VALUE_LIMIT(value,min,max) MIN(MAX(value,min),max)


//�����ʼ�Ƕ�
float init_angle = 0;

void App_Control_init(void){
	//�����ʼ��
	steering_drv_config();
	
	//�����ʼ���Ƕ�
	steering_drv_set_angle(init_angle);
}

//����
void App_Control_task(void * param){
	
	while(1){
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	vTaskDelete(NULL);
}