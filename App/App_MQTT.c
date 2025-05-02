#include "App.h"

short temperature=0;           //???
u8    setTempValue  = 40;     //???????
u8    setSmokeValue = 60;     //????????
extern int setLightValue;   // ???????????
extern int lightValue;      // ??????????
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
    memset(str, 0, sizeof(str)); //???????
    sprintf((char*)str, "temp:%.2f", th.temperature);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //???????
    sprintf((char*)str, "light:%.2f", light);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //???????
    sprintf((char*)str, "gas:%.2f", gas);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //???????
	ESP8266_Clear();

}
//????
void App_recev_task(void * param){
	//
	while(1){
    mqtt_send();
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	vTaskDelete(NULL);
}