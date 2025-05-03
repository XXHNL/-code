#include "App.h"
extern  int  humi_status;
extern int steering_status;
void App_Input_init(void){
	//������ʼ��
	bsp_keys_driver_init();
}

//����
void App_Input_task(void * param){
	//
	while(1){
		//����ɨ�� 10ms
		bsp_keys_scan();//delay_1ms(1000); 1s ϣ��PID��key_scan��һ��ִ��֮��,ҲҪִ�� 10s
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	vTaskDelete(NULL);
}

//���������¼��ص�
void bsp_key_press_callback(KEY key){
	if(key==KEY1){//����
		steering_drv_set_angle(180);
		steering_status = 1;
	}else if(key==KEY2){//�ش�
		steering_drv_set_angle(0);
		steering_status = 0;
	}else if(key==KEY3){//���ٶ���
		// led_turn_on(LED1);
		gpio_bit_toggle(GPIOD,GPIO_PIN_9);
	}else if(key==KEY4){//���ٶ���
		usart2_send_byte('G');
	}
}