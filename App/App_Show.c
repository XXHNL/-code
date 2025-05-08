#include "App.h"
TH th;
float light;
extern int gas; // 声明外部气体传感器变量
uint8_t str[20];
// 信号量句柄定义
SemaphoreHandle_t xDataReadySemaphore = NULL;
void App_Show_init(void){
	OLED_Init();				//初始化OLED
    // 创建二值信号量
    xDataReadySemaphore = xSemaphoreCreateBinary();
    // 首次释放信号量，允许其他任务读取
    xSemaphoreGive(xDataReadySemaphore);
}

static void show_oled(void){
	OLED_Clear();
    th = DHT11_GetTH();
    light = BH1750_Read();
    gas = ADC_get_gas();    // 获取气体传感器的值
    memset(str, 0, sizeof(str)); //清空数组
    sprintf((char*)str, "temp:%.2f", th.temperature);
    OLED_ShowString(0, 0, str, 16, 1);
    memset(str, 0, sizeof(str)); //清空数组
    sprintf((char*)str, "humi:%.2f", th.humidity);
    OLED_ShowString(0, 16, str, 16, 1);
    memset(str, 0, sizeof(str)); //清空数组
    sprintf((char*)str, "light:%.2f", light);
    OLED_ShowString(0, 32, str, 16, 1);
    memset(str, 0, sizeof(str)); //清空数组
    sprintf((char*)str, "gas:%d", gas);
    OLED_ShowString(0, 48, str, 16, 1);
    OLED_Refresh();
    // 数据采集后释放信号量，通知其他任务可读取
    xSemaphoreGive(xDataReadySemaphore);
}

//任务
void App_Show_task(void * param){
	while(1){
		show_oled();
		vTaskDelay(pdMS_TO_TICKS(1400));
	}
	vTaskDelete(NULL);
}
