#include "App.h"


extern int setLightValue;   // ???????????
extern int lightValue;      // ??????????
extern float gas;

extern float light;
extern TH th;
extern uint8_t str[20];

    

void mqtt_send(void){
    
    
    sprintf((char*)str, "hum:%.2f", th.humidity);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //???????
    delay_1ms(1);
    sprintf((char*)str, "temp:%.2f", th.temperature);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //???????
    delay_1ms(1);
    sprintf((char*)str, "light:%.2f", light);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //???????
    delay_1ms(1);
    sprintf((char*)str, "gas:%.2f", gas);
    ESP8266_SendData((unsigned char*)str, strlen((char*)str));
    memset(str, 0, sizeof(str)); //???????
    delay_1ms(1);
	ESP8266_Clear();

}
//????
void App_recev_task(void * param){
	//
	while(1){
        mqtt_send();
		vTaskDelay(pdMS_TO_TICKS(2000));
	}
	vTaskDelete(NULL);
}