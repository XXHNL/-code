#include "bsp_keys_driver.h"

//每一个LED初始化 核心配置:时钟 端口 引脚编号
KEY_PARAM keys[4] = {
	{RCU_GPIOC,GPIOC,GPIO_PIN_0},
	{RCU_GPIOC,GPIOC,GPIO_PIN_1},
	{RCU_GPIOC,GPIOC,GPIO_PIN_2},
	{RCU_GPIOC,GPIOC,GPIO_PIN_3},
};

//元素个数
uint8_t key_len = sizeof(keys)/sizeof(keys[0]);

//单个引脚初始化
static void key_pin_config(rcu_periph_enum rcu,uint32_t port,uint32_t pin){
	//1.打开时钟
	rcu_periph_clock_enable(rcu);
	//2.设置输入模式(注意:需要默认上拉高电平)
	gpio_mode_set(port,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,pin);
}

//驱动初始化
void bsp_keys_driver_init(void){
	for(int i=0;i<key_len;i++){
		KEY_PARAM key = keys[i];
		key_pin_config(key.rcu,key.port,key.pin);
	}
}

//上一次状态
FlagStatus key1_pre_state = SET;
FlagStatus key2_pre_state = SET;
FlagStatus key3_pre_state = SET;
FlagStatus key4_pre_state = SET;
//按键扫描
void bsp_keys_scan(void){
	//按键1
	//读取引脚状态
	FlagStatus status = gpio_input_bit_get(keys[KEY1].port,keys[KEY1].pin);
	if(key1_pre_state==SET&&status==RESET){
		//下降沿 按下
		bsp_key_press_callback(KEY1);
		key1_pre_state = status;
	}else if(key1_pre_state==RESET&&status==SET){
		//上升沿 松开
//		printf("key1 released\n");
		key1_pre_state = status;
	}
	
	//按键2
	status = gpio_input_bit_get(keys[KEY2].port,keys[KEY2].pin);
	if(key2_pre_state==SET&&status==RESET){
		//下降沿 按下
		bsp_key_press_callback(KEY2);
		key2_pre_state = status;
	}else if(key2_pre_state==RESET&&status==SET){
		//上升沿 松开
//		printf("key2 released\n");
		key2_pre_state = status;
	}
	
	//按键3
	status = gpio_input_bit_get(keys[KEY3].port,keys[KEY3].pin);
	if(key3_pre_state==SET&&status==RESET){
		//下降沿 按下
		bsp_key_press_callback(KEY3);
		key3_pre_state = status;
	}else if(key3_pre_state==RESET&&status==SET){
		//上升沿 松开
//		printf("key3 released\n");
		key3_pre_state = status;
	}
	
	
	//按键4
	status = gpio_input_bit_get(keys[KEY4].port,keys[KEY4].pin);
	if(key4_pre_state==SET&&status==RESET){
		//下降沿 按下
		bsp_key_press_callback(KEY4);
		key4_pre_state = status;
	}else if(key4_pre_state==RESET&&status==SET){
		//上升沿 松开
//		printf("key4 released\n");
		key4_pre_state = status;
	}
}


