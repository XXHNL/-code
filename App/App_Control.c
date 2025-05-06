#include "App.h"

float TEMPERATURE_UPPER_LIMIT = 30;
float HUMIDITY_LOWER_LIMIT = 30;
float CONCENTRATION_UPPER_LIMIT = 10;
float LIGHT_INTENSITY_LOWER_LIMIT = 60;
int temp_status = 0;
int humi_status = 0;
int gas_status = 0;
int light_status = 0;
int steering_status = 0;
int fan_status = 0;
int yuyin_status = 0;
int yuyin_flag = 0;
int app_status = 0;
int con_status = 0;

float CONCENTRATION_ANGLE = 90;

extern float light;
extern TH th;
extern float gas;

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
void App_Control_logic(void) {
   
    // 判断温度是否大于上限
    if (th.temperature > TEMPERATURE_UPPER_LIMIT && temp_status == 0) {
        if (steering_status == 0 && yuyin_status == 0) {
            steering_drv_set_angle(180);
            steering_status = 1;
            fan_status = 1;
        }
        Beep_node(10);
        temp_status = 1;
    } else if (th.temperature <= TEMPERATURE_UPPER_LIMIT && temp_status == 1) {
        if (steering_status == 1 && yuyin_status == 0) {
            steering_drv_set_angle(0);
            steering_status = 0;
        }
        temp_status = 0;
        fan_status = 0;
    }
    // 判断湿度是否小于下限
    if (th.humidity < HUMIDITY_LOWER_LIMIT && humi_status == 0 && yuyin_status == 0 ) {
        usart2_send_byte('G'); // 开启加湿器
        humi_status = 1;
    } else if (th.humidity >= HUMIDITY_LOWER_LIMIT && humi_status == 1 && yuyin_status == 0) {
        usart2_send_byte('H'); // 关闭加湿器
        humi_status = 0;
    }
    // 判断浓度是否大于上限
    gas = ADC_get_gas();
    if (gas > CONCENTRATION_UPPER_LIMIT && gas_status == 0) {
        if (steering_status == 0 && yuyin_status == 0) {
            steering_drv_set_angle(180);
            steering_status = 1;
        }
        if (fan_status == 0 && yuyin_status == 0) {
            usart2_send_byte('I'); // 开启风扇
            fan_status = 1;
        }
        Beep_node(30);
        gas_status = 1;
    } else if (gas <= CONCENTRATION_UPPER_LIMIT && gas_status == 1) {
        if (fan_status == 1 && yuyin_status == 0) {
            usart2_send_byte('J'); // 关闭风扇
            fan_status = 0;
        }
        if (steering_status == 1 && yuyin_status == 0) {
            steering_drv_set_angle(0);
            steering_status = 0;
        }
        gas_status = 0;
    }
    if(yuyin_status == 1){
        if(steering_status == 1) {
            steering_drv_set_angle(180);
        }
        if(steering_status == 0) {
            steering_drv_set_angle(0);
        }
        if(fan_status == 1) {
            usart2_send_byte('I'); // 开启风扇
        }
        if(fan_status == 0) {
            usart2_send_byte('J'); // 关闭风扇
        }
        if(humi_status == 1) {
            usart2_send_byte('G'); // 开启加湿器
        }
        if(humi_status == 0) {
            usart2_send_byte('H'); // 关闭加湿器
        }
    }
    if(app_status == 1){
        if(fan_status == 1 ) {
            usart2_send_byte('I'); // 开启风扇
        }
        if(fan_status == 0) {
            usart2_send_byte('J'); // 关闭风扇
        }
        if(humi_status == 1) {
            usart2_send_byte('G'); // 开启加湿器
        }
        if(humi_status == 0) {
            usart2_send_byte('H'); // 关闭加湿器
        }
        if(steering_status == 1) {
            steering_drv_set_angle(180);
        }
        if(steering_status == 0) {
            steering_drv_set_angle(0);
        }
    }
    // 光照强度控制
    if (light < 20) {
        led_set_brightness(0);
        if (light_status == 1) {
            led_turn_off(LED5);
            light_status = 0;
        }
    } else if (light > 150) {
        led_turn_on(LED5);
        light_status = 1;
        led_set_brightness(999);
    } else {
        uint16_t brightness = (uint16_t)((light * 999) / 150);
        if (light_status == 0) {
            led_turn_on(LED5);
            light_status = 1;
        }
        led_set_brightness(brightness);
    }
}
// 在任务中调用逻辑函数
void App_Control_task(void * param){
    while(1){
        App_Control_logic();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}