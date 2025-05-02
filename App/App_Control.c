#include "App.h"


#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define VALUE_LIMIT(value,min,max) MIN(MAX(value,min),max)


//舵机初始角度
float init_angle = 0;

void App_Control_init(void){
	//舵机初始化
	steering_drv_config();
	
	//舵机初始化角度
	steering_drv_set_angle(init_angle);
}

//任务
void App_Control_task(void * param){
	
	while(1){
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	vTaskDelete(NULL);
}