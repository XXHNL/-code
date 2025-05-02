#include "bsp_keys_driver.h"

//ÿһ��LED��ʼ�� ��������:ʱ�� �˿� ���ű��
KEY_PARAM keys[4] = {
	{RCU_GPIOC,GPIOC,GPIO_PIN_0},
	{RCU_GPIOC,GPIOC,GPIO_PIN_1},
	{RCU_GPIOC,GPIOC,GPIO_PIN_2},
	{RCU_GPIOC,GPIOC,GPIO_PIN_3},
};

//Ԫ�ظ���
uint8_t key_len = sizeof(keys)/sizeof(keys[0]);

//�������ų�ʼ��
static void key_pin_config(rcu_periph_enum rcu,uint32_t port,uint32_t pin){
	//1.��ʱ��
	rcu_periph_clock_enable(rcu);
	//2.��������ģʽ(ע��:��ҪĬ�������ߵ�ƽ)
	gpio_mode_set(port,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,pin);
}

//������ʼ��
void bsp_keys_driver_init(void){
	for(int i=0;i<key_len;i++){
		KEY_PARAM key = keys[i];
		key_pin_config(key.rcu,key.port,key.pin);
	}
}

//��һ��״̬
FlagStatus key1_pre_state = SET;
FlagStatus key2_pre_state = SET;
FlagStatus key3_pre_state = SET;
FlagStatus key4_pre_state = SET;
//����ɨ��
void bsp_keys_scan(void){
	//����1
	//��ȡ����״̬
	FlagStatus status = gpio_input_bit_get(keys[KEY1].port,keys[KEY1].pin);
	if(key1_pre_state==SET&&status==RESET){
		//�½��� ����
		bsp_key_press_callback(KEY1);
		key1_pre_state = status;
	}else if(key1_pre_state==RESET&&status==SET){
		//������ �ɿ�
//		printf("key1 released\n");
		key1_pre_state = status;
	}
	
	//����2
	status = gpio_input_bit_get(keys[KEY2].port,keys[KEY2].pin);
	if(key2_pre_state==SET&&status==RESET){
		//�½��� ����
		bsp_key_press_callback(KEY2);
		key2_pre_state = status;
	}else if(key2_pre_state==RESET&&status==SET){
		//������ �ɿ�
//		printf("key2 released\n");
		key2_pre_state = status;
	}
	
	//����3
	status = gpio_input_bit_get(keys[KEY3].port,keys[KEY3].pin);
	if(key3_pre_state==SET&&status==RESET){
		//�½��� ����
		bsp_key_press_callback(KEY3);
		key3_pre_state = status;
	}else if(key3_pre_state==RESET&&status==SET){
		//������ �ɿ�
//		printf("key3 released\n");
		key3_pre_state = status;
	}
	
	
	//����4
	status = gpio_input_bit_get(keys[KEY4].port,keys[KEY4].pin);
	if(key4_pre_state==SET&&status==RESET){
		//�½��� ����
		bsp_key_press_callback(KEY4);
		key4_pre_state = status;
	}else if(key4_pre_state==RESET&&status==SET){
		//������ �ɿ�
//		printf("key4 released\n");
		key4_pre_state = status;
	}
}


