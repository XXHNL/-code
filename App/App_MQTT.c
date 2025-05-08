#include "App.h"


extern int setLightValue;   // ???????????
extern int lightValue;      // ??????????
extern float gas;

extern float light;
extern TH th;
uint8_t str1[20];

    

void mqtt_send(void){
    // 在读取数据前获取信号量，确保数据一致性
    xSemaphoreTake(xDataReadySemaphore, portMAX_DELAY);
    
    
    sprintf((char*)str1, "hum:%.2f", th.humidity);
    ESP8266_SendData((unsigned char*)str1, strlen((char*)str1));
    memset(str1, 0, sizeof(str1)); //清空缓存
    delay_1ms(1);
    sprintf((char*)str1, "temp:%.2f", th.temperature);
    ESP8266_SendData((unsigned char*)str1, strlen((char*)str1));
    memset(str1, 0, sizeof(str1)); //清空缓存
    delay_1ms(1);
    sprintf((char*)str1, "light:%.2f", light);
    ESP8266_SendData((unsigned char*)str1, strlen((char*)str1));
    memset(str1, 0, sizeof(str1)); //清空缓存
    delay_1ms(1);
    sprintf((char*)str1, "gas:%.2f", gas);
    ESP8266_SendData((unsigned char*)str1, strlen((char*)str1));
    memset(str1, 0, sizeof(str1)); //清空缓存
    delay_1ms(1);
	ESP8266_Clear();
}
void App_recev_task(void * param){
	//
	while(1){
        mqtt_send();
		vTaskDelay(pdMS_TO_TICKS(500));
	}
	vTaskDelete(NULL);
}