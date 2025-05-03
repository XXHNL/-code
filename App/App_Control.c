#include "App.h"

float TEMPERATURE_UPPER_LIMIT = 35;
float HUMIDITY_LOWER_LIMIT = 30;
float CONCENTRATION_UPPER_LIMIT = 50;
float LIGHT_INTENSITY_LOWER_LIMIT = 60;
float LIGHT_INTENSITY_UPPER_LIMIT = 100;
int temp_status = 0;
int humi_status = 0;
int gas_status = 0;
int light_status = 0;
int steering_status = 0;
int fan_status = 0;

float CONCENTRATION_ANGLE = 90;

extern float light;
extern TH th;
extern float gas;

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
void App_Control_logic(void) {
    // �ж��¶��Ƿ��������
    if (th.temperature > TEMPERATURE_UPPER_LIMIT&&temp_status == 0 && steering_status == 0) {
        steering_drv_set_angle(180);
				Beep_node(Low);
        temp_status = 1;
        steering_status = 1;
    }else if(th.temperature < TEMPERATURE_UPPER_LIMIT&&temp_status == 1 && steering_status == 1){
        steering_drv_set_angle(0);
        steering_status = 0;
        temp_status = 0;
    }

    // �ж�ʪ���Ƿ�С������
    if (th.humidity < HUMIDITY_LOWER_LIMIT && humi_status == 0) {
			usart2_send_byte('G');//������ʪ��
			humi_status = 1;

    }else if(th.humidity > HUMIDITY_LOWER_LIMIT && humi_status == 1){
        // humidifier_off();
        usart2_send_byte('H');//�رռ�ʪ��
				humi_status = 0;
    }

    // �ж�Ũ���Ƿ��������
    if (gas > CONCENTRATION_UPPER_LIMIT && gas_status == 0) {
        usart2_send_byte('F');//��������
				steering_drv_set_angle(180);
				Beep_node(Hign);
        gas_status = 1;
    }else if(gas < CONCENTRATION_UPPER_LIMIT && gas_status == 1){
    	usart2_send_byte('E');//�رշ���
			steering_drv_set_angle(0);
        gas_status = 0;
    }
		if(steering_status == 1){
			steering_drv_set_angle(180);
		}else{
			steering_drv_set_angle(0);
		}
		if(fan_status == 1){
			steering_drv_set_angle(180);
		}else{
			steering_drv_set_angle(0);
		}
		if(humi_status == 1){
			usart2_send_byte('G');//������ʪ��
		}else{
			usart2_send_byte('H');//�رռ�ʪ��
		}


// light ���Դ�������ȡֵ��Χ��0���������� 200�����
    if (light < 20) {
        // ���ռ�ǿ��light�ӽ�0�����ر�LED
        led_set_brightness(0);
        if (light_status == 1) {
            led_turn_off(LED5);
            light_status = 0;
        }
    }
    else if (light > 150) {
        // ���ռ������ӽ��ڰ�������LED������Ϊ�������
        led_turn_on(LED5);
        light_status = 1;
        led_set_brightness(999);
    }
    else {
        // ������������Χ�ڣ������ȼ������ȣ���Ϊ light Խ���ʾԽ����
        uint16_t brightness = (uint16_t)((light * 999) / 150);

        if (light_status == 0) {
            led_turn_on(LED5);
            light_status = 1;
        }

        led_set_brightness(brightness);  // ���õ�ǰ����
    }
}
// �������е����߼�����
void App_Control_task(void * param){
    while(1){
        App_Control_logic();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}