#include "App.h"
extern TH th;
extern float light;
extern int gas; // �����ⲿ���崫��������
extern uint8_t str[20];
void App_Show_init(void){
	OLED_Init();				//��ʼ��OLED
}

static void show_oled(void){
	OLED_Clear();
    th = DHT11_GetTH();
    light = BH1750_Read();
    gas = ADC_get_gas();    // ��ȡ���崫������ֵ
    memset(str, 0, sizeof(str)); //�������
    sprintf((char*)str, "temp:%.2f", th.temperature);
    OLED_ShowString(0, 0, str, 16, 1);
    memset(str, 0, sizeof(str)); //�������
    sprintf((char*)str, "humi:%.2f", th.humidity);
    OLED_ShowString(0, 16, str, 16, 1);
    memset(str, 0, sizeof(str)); //�������
    sprintf((char*)str, "light:%.2f", light);
    OLED_ShowString(0, 32, str, 16, 1);
    memset(str, 0, sizeof(str)); //�������
    sprintf((char*)str, "gas:%d", gas);
    OLED_ShowString(0, 48, str, 16, 1);
    OLED_Refresh();
}

//����
void App_Show_task(void * param){
	while(1){
		show_oled();
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	vTaskDelete(NULL);
}
