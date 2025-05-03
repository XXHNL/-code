#include "App.h"
extern  int  humi_status;
extern int steering_status;
void App_Input_init(void){
	//按键初始化
	bsp_keys_driver_init();
}

//任务
void App_Input_task(void * param){
	//
	while(1){
		//按键扫描 10ms
		bsp_keys_scan();//delay_1ms(1000); 1s 希望PID在key_scan第一次执行之后,也要执行 10s
		vTaskDelay(pdMS_TO_TICKS(10));
	}
	vTaskDelete(NULL);
}

//按键按下事件回调
void bsp_key_press_callback(KEY key){
	if(key==KEY1){//开窗
		steering_drv_set_angle(180);
		steering_status = 1;
	}else if(key==KEY2){//关窗
		steering_drv_set_angle(0);
		steering_status = 0;
	}else if(key==KEY3){//减少度数
		// led_turn_on(LED1);
		gpio_bit_toggle(GPIOD,GPIO_PIN_9);
	}else if(key==KEY4){//减少度数
		usart2_send_byte('G');
	}
}