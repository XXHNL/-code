#include "App.h"

short temperature=0;           //温度
u8    setTempValue  = 40;     //温度上限
u8    setSmokeValue = 60;     //烟雾上限
extern int setLightValue;   // 设置的光照阈值
extern int lightValue;      // 当前光照强度
extern float gas;

bool sendFlag = 1;
bool flameFlag=0;
u8    smoke=0;

float light;
TH th;
uint8_t str[20];

    

void mqtt_send(void){
    
    
    sprintf((char*)str, "hum:%.2f", th.humidity);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //清空数组
    sprintf((char*)str, "temp:%.2f", th.temperature);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //清空数组
    sprintf((char*)str, "light:%.2f", light);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //清空数组
    sprintf((char*)str, "gas:%.2f", gas);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //清空数组
	ESP8266_Clear();

}
//任务
void App_recev_task(void * param){
	//
	while(1){
    mqtt_send();
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	vTaskDelete(NULL);
}